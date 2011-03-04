# Copyright (C) 2008-2010 Trinity <http://www.trinitycore.org/>
#
# Copyright (C) 2009-2010 shadow <http://www.oregoncore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

file(GLOB sources_localdir *.cpp *.h)

set(shadow-realm_SRCS
  ${shadow-realm_SRCS}
  ${sources_localdir}
)

if( WIN32 )
  set(shadow-realm_SRCS
    ${shadow-realm_SRCS}
    ${sources_Debugging}
    shadow.rc
  )
endif()

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/src/shared
  ${CMAKE_SOURCE_DIR}/src/shared/Database
  ${CMAKE_SOURCE_DIR}/src/framework
  ${ACE_INCLUDE_DIR}
  ${MYSQL_INCLUDE_DIR}
  ${OPENSSL_INCLUDE_DIR}
)

set(oregon-realm_LINK_FLAGS "")

add_executable(shadow-realm
  ${shadow-realm_SRCS}
)

add_dependencies(shadow-realm revision.h)

if( NOT WIN32 )
  add_definitions(-D_SHADOW_REALM_CONFIG="${CONF_DIR}/Shadowrealm.conf")
endif()

if( UNIX )
  set(authserver_LINK_FLAGS "-pthread ${shadow-realm_LINK_FLAGS}")
endif()

if( CMAKE_SYSTEM_NAME MATCHES "Darwin" )
  set(shadow-realm_LINK_FLAGS "-framework Carbon ${shadow-realm_LINK_FLAGS}")
endif()

set_target_properties(shadow-realm PROPERTIES LINK_FLAGS "${shadow-realm_LINK_FLAGS}")

target_link_libraries(shadow-realm
  shared
  shadowframework
  ${MYSQL_LIBRARY}
  ${OPENSSL_LIBRARIES}
  ${OPENSSL_EXTRA_LIBRARIES}
  ${OSX_LIBS}
)

if( WIN32 )
  add_custom_command(TARGET shadow-realm
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/shadowrealm.conf.dist ${CMAKE_BINARY_DIR}/bin/$(ConfigurationName)/
  )
endif()

if( UNIX )
  install(TARGETS oregon-realm DESTINATION bin)
  install(FILES oregonrealm.conf.dist DESTINATION etc)
elseif( WIN32 )
  install(TARGETS shadow-realm DESTINATION "${CMAKE_INSTALL_PREFIX}")
  install(FILES shadowrealm.conf.dist DESTINATION "${CMAKE_INSTALL_PREFIX}")
endif()
