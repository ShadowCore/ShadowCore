SET(shadowframework_STAT_SRCS
   Policies/ObjectLifeTime.cpp
   Utilities/EventProcessor.cpp
)

include_directories(
  ${ACE_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR}/src/framework
)

add_library(shadowframework STATIC ${shadowframework_STAT_SRCS})
