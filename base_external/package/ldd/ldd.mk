##############################################################
#
# LDD
#
##############################################################

LDD_SITE = $(BR2_EXTERNAL)/../scull
LDD_LICENSE = GPL-2.0
LDD_LICENSE_FILES = COPYING
LDD_SITE_METHOD = local

define LDD_BUILD_CMDS
    cp $(BR2_EXTERNAL)/../include/* $(@D)
endef


define LDD_INSTALL_TARGET_CMDS
     $(INSTALL) -D -m 0755 $(@D)/scull.ko $(BR2_ROOTFS_OVERLAY)/lib/modules/6.8.9/ldd/scull
endef

$(eval $(kernel-module))
$(eval $(generic-package))
