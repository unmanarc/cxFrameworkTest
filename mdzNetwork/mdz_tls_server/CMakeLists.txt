cmake_minimum_required(VERSION 3.12)
include(GNUInstallDirs)

set(APP_NAME "mantidsTLSServer")
set(SVERSION 1.0.0)

project(${APP_NAME})
project(${PROJECT_NAME} VERSION ${SVERSION} DESCRIPTION "TLS Server")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

configure_file(src/config.h.in ${${APP_NAME}_SOURCE_DIR}/src/config.h)

file(GLOB_RECURSE EDV_INCLUDE_FILES "src/*.h*")
file(GLOB_RECURSE EDV_SOURCE_FILES "src/*.c*")
add_executable( ${APP_NAME} ${EDV_INCLUDE_FILES} ${EDV_SOURCE_FILES} )

install( TARGETS ${APP_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} )

if (EXTRAPREFIX)
    target_include_directories(${APP_NAME} PUBLIC ${EXTRAPREFIX}/include)
    target_link_libraries(${APP_NAME} "-L${EXTRAPREFIX}/lib")
    target_link_libraries(${APP_NAME} "-L${EXTRAPREFIX}/lib64")
endif()

if (CMAKE_INSTALL_PREFIX)
    target_include_directories(${APP_NAME} PUBLIC ${CMAKE_INSTALL_PREFIX}/include)
    target_include_directories(${APP_NAME} PUBLIC ${CMAKE_INSTALL_PREFIX}/usr/include)
    target_link_libraries(${APP_NAME} "-L${CMAKE_INSTALL_PREFIX}/lib")
    target_link_libraries(${APP_NAME} "-L${CMAKE_INSTALL_PREFIX}/lib64")
endif()

target_link_libraries(${APP_NAME} "-lmdz_net_sockets")
target_link_libraries(${APP_NAME} "-lmdz_thr_threads")
target_link_libraries(${APP_NAME} "-lmdz_hlp_functions")
target_link_libraries(${APP_NAME} "-lmdz_thr_mutex")
target_link_libraries(${APP_NAME} "-lmdz_thr_safecontainers")
target_link_libraries(${APP_NAME} "-lmdz_mem_vars")
target_link_libraries(${APP_NAME} "-lssl")

target_link_libraries(${APP_NAME} "-lpthread")

#find_package(PkgConfig REQUIRED)
#pkg_check_modules(JSONCPP jsoncpp)
#link_libraries(${JSONCPP_LIBRARIES})
#target_include_directories(${APP_NAME} PUBLIC ${JSONCPP_INCLUDE_DIRS})
#target_link_libraries(${APP_NAME} ${JSONCPP_LIBRARIES})

