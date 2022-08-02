cmake_minimum_required(VERSION 3.10)

##############################################################################################################################
# Other CMake Global settings:
include(GNUInstallDirs)
##############################################################################################################################

#############################################################################################################################
# Subprojects:
ADD_SUBDIRECTORY(mdz_chainsockets_aeschat_client)
ADD_SUBDIRECTORY(mdz_chainsockets_aeschat_server)
ADD_SUBDIRECTORY(mdz_chainsockets_doublexor)
ADD_SUBDIRECTORY(mdz_tcp_client)
ADD_SUBDIRECTORY(mdz_tcp_server)
ADD_SUBDIRECTORY(mdz_tls_client)
ADD_SUBDIRECTORY(mdz_tls_server)

#############################################################################################################################

