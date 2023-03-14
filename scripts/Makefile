# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2022 BPHK2022

include $(TOPDIR)/rules.mk

PKG_NAME:=gaffa
PKG_RELEASE:=1

PKG_FLAGS:=nonshared
PKG_LICENSE:=MIT
PKG_MAINTAINER:=BPHK2022

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

#TARGET_CFLAGS += \
#        -I$(STAGING_DIR)/usr/include/libnl3 \
#        -I$(STAGING_DIR)/usr/include \

CMAKE_OPTIONS += \
        -DSTAGING_DIR=$(STAGING_DIR) \


CMAKE_INSTALL:=1

define Package/gaffa
  SECTION:=network
  CATEGORY:=Networking
  TITLE:=Scalable WiFi Daemon
  DEPENDS:=+libstdcpp +libnl +libatomic
  EXTRA_DEPENDS:=frr, frr-bgpd, frr-zebra, frr-vtysh, frr-watchfrr
endef

define Package/gaffa/description
  Scalable WiFi Daemon
endef

define Package/gaffa/config
endef

define Package/gaffa/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/gaffa $(1)/usr/bin
endef

$(eval $(call BuildPackage,gaffa))
