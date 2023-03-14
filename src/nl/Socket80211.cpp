#include "Socket80211.h"

#include <linux/nl80211.h>
#include <net/if.h>
#include <netlink/genl/ctrl.h>
#include <netlink/genl/family.h>
#include <netlink/genl/genl.h>
#include <netlink/netlink.h>

#include <cstring>
#include <stdexcept>

#include "../Station.h"
#include "../TimeoutException.h"
#include "../logging/loginit.h"

nl::Socket80211::Socket80211() {
    socket = nl_socket_alloc();
    if (socket == nullptr) {
        throw std::runtime_error(std::string("could not allocate netlink socket: ") + std::strerror(errno));
    }
    nl_socket_disable_seq_check(socket);
    int ret = genl_connect(socket);
    if (ret < 0) {
        throw std::runtime_error(std::string("could not connect to generic netlink family: ") + std::strerror(errno));
    }
    //nl80211_family = genl_ctrl_resolve(socket, "nl80211");

    //struct nl_cache *cache;
    //ret = genl_ctrl_alloc_cache(socket, &cache);
    //if (ret < 0) {
    //    throw std::runtime_error(std::string("could not alloc cache: ") + nl_geterror(ret));
    //}
    //struct genl_family* family = genl_ctrl_search_by_name(cache, "nl80211");
    //if (!family) {
    //    throw std::runtime_error(std::string("could not resolve nl80211 family: ") + nl_geterror(ret));
    //}
    //nl80211_family = static_cast<int>(genl_family_get_id(family));
}

nl::Socket80211::~Socket80211() {
    nl_close(socket);
    nl_socket_free(socket);
}

static std::string format_mac(uint8_t mac[6]) {
    char formatted_mac[2 * 6 + 5 + 1];
    snprintf(formatted_mac, sizeof(formatted_mac), "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return formatted_mac;
}

static int interface_event_handler(struct nl_msg *msg, void *arg) {
    auto *hdr = static_cast<genlmsghdr *>(nlmsg_data(nlmsg_hdr(msg)));
    struct nlattr *attrs[NL80211_ATTR_MAX + 1];

    int err = nla_parse(attrs, NL80211_ATTR_MAX, genlmsg_attrdata(hdr, 0), genlmsg_attrlen(hdr, 0), nullptr);
    if (err < 0) {
        throw std::runtime_error(std::string("could not create attribute index: ") + nl_geterror(err));
    }
    uint32_t ifindex;
    uint32_t iftype;

    auto *socket = reinterpret_cast<nl::Socket80211*>(arg);

    switch(hdr->cmd) {
        case NL80211_CMD_NEW_INTERFACE:
            iftype = nla_get_u32(attrs[NL80211_ATTR_IFTYPE]);
            if (iftype == NL80211_IFTYPE_AP_VLAN) {
                ifindex = nla_get_u32(attrs[NL80211_ATTR_IFINDEX]);
                char name[IFNAMSIZ];
                if (if_indextoname(ifindex, name) == nullptr) {
                    GAFFALOG(WARNING) << "NL80211_CMD_NEW_INTERFACE ifindex=" << ifindex << " has no associated name";
                } else {
                    GAFFALOG(DEBUG) << "NL80211_CMD_NEW_INTERFACE ifindex=" << ifindex << " name=" << name;
                }
            }
        case NL80211_CMD_NEW_STATION:
            std::string mac = format_mac(static_cast<uint8_t *>(nla_data(attrs[NL80211_ATTR_MAC])));
            ifindex = nla_get_u32(attrs[NL80211_ATTR_IFINDEX]);
            char name[IFNAMSIZ];
            if (if_indextoname(ifindex, name) == nullptr) {
                GAFFALOG(DEBUG) << "NL80211_CMD_NEW_STATION mac=" << mac << " ifindex=" << ifindex << " with no associated name";
            } else {
                GAFFALOG(DEBUG) << "NL80211_CMD_NEW_STATION mac=" << mac << " ifindex=" << ifindex << " name=" << name;
                std::string ifacename(name);
                if (ifacename.rfind("vlan", 0) == 0) {
                    // TODO: improve
                    socket->new_stations.emplace_back(mac, std::strtoull(ifacename.substr(4).c_str(), nullptr, 10));
                }
            }
            break;
    }
    return NL_OK;
}

void nl::Socket80211::subscribe() {
    for (const std::string& family : {"config",	"mlme"}) {
        int ret = genl_ctrl_resolve_grp(socket, "nl80211", family.c_str());
        if (ret < 0) {
            throw std::runtime_error(std::string("could not resolve netlink group " + family + ": ") + nl_geterror(ret));
        }

        ret = nl_socket_add_membership(socket, ret);
        if (ret < 0) {
            throw std::runtime_error(std::string("could not resolve add netlink membership: ") + std::strerror(errno));
        }
    }
    int err = nl_socket_modify_cb(socket, NL_CB_VALID, NL_CB_CUSTOM, interface_event_handler, this);
    if (err < 0) {
        throw std::runtime_error(std::string("could not set interface callback: ") + nl_geterror(err));
    }


}
std::vector<Station> nl::Socket80211::wait_for_stations() {
    int err = nl_recvmsgs_default(socket);
    if (err < 0) {
        if (err == -NLE_AGAIN) {
            throw TimeoutException("timeout in wait_for_events");
        } else {
            throw std::runtime_error(std::string("could not receive netlink events: ") + nl_geterror(err));
        }
    }
    std::vector<Station> ret = std::move(new_stations);
    new_stations.clear();
    return ret;
}
void nl::Socket80211::set_receive_timeout(const std::chrono::duration<int> &timeout) {
    auto timeout_usec = std::chrono::duration_cast<std::chrono::microseconds>(timeout);
    struct timeval tv {};
    tv.tv_sec = timeout_usec.count() / 1000000;
    tv.tv_usec = timeout_usec.count() % 1000000;
    if (setsockopt(nl_socket_get_fd(socket), SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv)) == -1) {
        throw std::runtime_error(std::string("could not set netlink socket receive timeout: ") + std::strerror(errno));
    }

}

//extern "C" int station_handler(struct nl_msg *msg, void *arg) {
//    auto *hdr = static_cast<genlmsghdr *>(nlmsg_data(nlmsg_hdr(msg)));
//    struct nlattr *attrs[NL80211_ATTR_MAX + 1];
//
//    int err = nla_parse(attrs, NL80211_ATTR_MAX, genlmsg_attrdata(hdr, 0), genlmsg_attrlen(hdr, 0), nullptr);
//    if (err < 0) {
//        GAFFALOG(ERROR) << "could not create attribute index: " << nl_geterror(err);
//        //throw std::runtime_error(std::string("could not create attribute index: ") + nl_geterror(err));
//        return NL_OK;
//    }
//    if (hdr->cmd != NL80211_CMD_NEW_STATION) {
//        return NL_OK;
//    }
//
//    if (attrs[NL80211_ATTR_MAC] == nullptr) {
//        GAFFALOG(ERROR) << "attribute NL80211_ATTR_MAC not set";
//        //throw std::runtime_error("attribute NL80211_ATTR_MAC not set");
//        return NL_OK;
//    }
//    auto *socket = reinterpret_cast<nl::Socket80211*>(arg);
//    socket->station_mac = format_mac((uint8_t *) nla_data(attrs[NL80211_ATTR_MAC]));
//
//    return NL_OK;
//}

//static int station_error_handler(struct nl_msg *msg, void *arg) {
//    GAFFALOG(WARNING) << "error handler triggered";
//    return NL_OK;
//}

//Station nl::Socket80211::station_for_event(const Event& event) {
//    //std::this_thread::sleep_for(std::chrono::milliseconds(100));
//    struct nl_cb *cb = nl_cb_alloc(NL_CB_CUSTOM);
//    if (cb == nullptr) {
//        throw std::runtime_error(std::string("could not allocate cb: ") + std::strerror(errno));
//    }
//    int ret = nl_cb_set(cb, NL_CB_VALID, NL_CB_CUSTOM, station_handler, this);
//    if (ret < 0) {
//        throw std::runtime_error(std::string("could not set cb: ") + nl_geterror(ret));
//    }
//
//    struct nl_msg *msg = nlmsg_alloc();
//    if (!msg) {
//        throw std::runtime_error(std::string("failed to allocate netlink message: ") + std::strerror(errno));
//    }
//    if (genlmsg_put(msg, NL_AUTO_PORT, NL_AUTO_SEQ, nl80211_family, 0, NLM_F_DUMP, NL80211_CMD_GET_STATION, 0) == nullptr) {
//        throw std::runtime_error(std::string("could put netlink message: ") + std::strerror(errno));
//    }
//    ret = nla_put_u32(msg, NL80211_ATTR_IFINDEX, event.ifindex);
//    if (ret < 0) {
//        throw std::runtime_error(std::string("could not add ifindex to netlink message: ") + nl_geterror(ret));
//    }
//    //ret = nl_send_auto(socket, msg);
//    ret = nl_send_auto_complete(socket, msg);
//    if (ret < 0) {
//        throw std::runtime_error(std::string("could not send netlink message: ") + nl_geterror(ret));
//    }
//    //ret = nl_socket_modify_cb(socket, NL_CB_VALID, NL_CB_CUSTOM, station_handler, this);
//    //if (ret < 0) {
//    //    throw std::runtime_error(std::string("could not modify netlink socket cb: ") + nl_geterror(ret));
//    //}
//
//    station_mac = "";
//    while (station_mac.empty()) {
//        ret = nl_recvmsgs(socket, cb);
//        if (ret < 0) {
//            throw std::runtime_error(std::string("could not send receive netlink messages: ") + nl_geterror(ret));
//        }
//    }
//    std::string return_mac = std::move(station_mac);
//    return return_mac;
//}
