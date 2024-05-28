##############################################################
#
# MODULE
#
##############################################################

MODULE_SITE = $(BR2_EXTERNAL)/../misc-modules
MODULE_LICENSE = GPL-2.0
MODULE_LICENSE_FILES = COPYING
MODULE_SITE_METHOD = local

define MODULE_BUILD_CMDS
     cp -rf $(BR2_EXTERNAL)/../include/* $(@D)/
endef

$(eval $(kernel-module))
$(eval $(generic-package))
