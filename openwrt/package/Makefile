# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2022 BPHK2022

include $(TOPDIR)/rules.mk

PKG_NAME:=wimoved
PKG_RELEASE:=1
PKG_VERSION?=1

PKG_FLAGS:=nonshared
PKG_LICENSE:=MIT
PKG_MAINTAINER:=BPHK2022

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

CMAKE_OPTIONS += \
        -DSTAGING_DIR=$(STAGING_DIR) \
        -DWIMOVED_ENABLE_TESTS=OFF

CMAKE_INSTALL:=1

define Package/wimoved
  SECTION:=network
  CATEGORY:=Networking
  TITLE:=Scalable WiFi Daemon
  DEPENDS:=+libstdcpp +libnl +libatomic
  EXTRA_DEPENDS:=frr, frr-bgpd, frr-zebra, frr-vtysh, frr-watchfrr
endef

define Package/wimoved/description
  Scalable WiFi Daemon
endef

define Package/wimoved/config
endef

define Package/wimoved/install
	$(INSTALL_DIR) $(1)/usr/bin $(1)/etc/init.d $(1)/etc/wimoved
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/wimoved $(1)/usr/bin
	$(INSTALL_BIN) ./files/wimoved $(1)/etc/init.d
	$(INSTALL_CONF) ./files/config $(1)/etc/wimoved
endef

$(eval $(call BuildPackage,wimoved))
