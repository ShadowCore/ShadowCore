# Copyright (C) 2008-2010 Trinity <http://www.trinitycore.org/>
#
# Copyright (C) 2009-2010 shadow <http://www.shadowcore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

file(GLOB sources_localdir *.cpp *.h)

set(shadow-core_SRCS
  ${shadow-core_SRCS}
  ${sources_localdir}
)

if( WIN32 )
  set(shadow-core_SRCS
    ${shadow-core_SRCS}
    ${sources_Debugging}
    shadowCore.rc
  )
endif()

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/dep/include
  ${CMAKE_SOURCE_DIR}/dep/include/gsoap
  ${CMAKE_SOURCE_DIR}/src/collision
  ${CMAKE_SOURCE_DIR}/src/shared
  ${CMAKE_SOURCE_DIR}/src/shared/Database
  ${CMAKE_SOURCE_DIR}/src/framework
  ${CMAKE_SOURCE_DIR}/src/game
  ${ACE_INCLUDE_DIR}
  ${MYSQL_INCLUDE_DIR}
  ${OPENSSL_INCLUDE_DIR}
)

set(shadow-core_LINK_FLAGS "")

add_executable(shadow-core ${shadow-core_SRCS})

if( NOT WIN32 )
  add_definitions(-D_shadow_CORE_CONFIG="${CONF_DIR}/shadowcore.conf")
endif()

add_dependencies(shadow-core revision.h)

if( UNIX )
  set(shadow-core_LINK_FLAGS "-pthread ${shadow-core_LINK_FLAGS}")
endif()

set_target_properties(shadow-core PROPERTIES LINK_FLAGS "${shadow-core_LINK_FLAGS}")

target_link_libraries(shadow-core
  game
  shared
  scripts
  shadowframework
  oregonsockets
  collision
  g3dlite
  gsoap
  ${READLINE_LIBRARY}
  ${TERMCAP_LIBRARY}
  ${ACE_LIBRARY}
  ${MYSQL_LIBRARY}
  ${OPENSSL_LIBRARIES}
  ${OPENSSL_EXTRA_LIBRARIES}
  ${ZLIB_LIBRARIES}
  ${OSX_LIBS}
)

if( WIN32 )
  add_custom_command(TARGET shadow-core
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/shadowcore.conf.dist ${CMAKE_BINARY_DIR}/bin/$(ConfigurationName)/
  )
endif()

if( UNIX )
  install(TARGETS shadow-core DESTINATION bin)
  install(FILES shadowcore.conf.dist DESTINATION etc)
elseif( WIN32 )
  install(TARGETS shadow-core DESTINATION "${CMAKE_INSTALL_PREFIX}")
  install(FILES shadowcore.conf.dist DESTINATION "${CMAKE_INSTALL_PREFIX}")
endif()
