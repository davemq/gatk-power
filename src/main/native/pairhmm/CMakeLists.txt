#---------------------------------------------------------------------
# common
#---------------------------------------------------------------------
if(NOT APPLE)
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-as-needed")
endif()
set_property(SOURCE PowerPairHmm.c APPEND_STRING PROPERTY COMPILE_FLAGS " -O3 -mcpu=power9 -mtune=power9 -mvsx")

#---------------------------------------------------------------------
# common includes
#---------------------------------------------------------------------
include_directories(../common)

#---------------------------------------------------------------------
# pairhmm
#---------------------------------------------------------------------
set(TARGET vsx_pairhmm)
add_library(${TARGET} SHARED PowerPairHmm.c)
install(TARGETS ${TARGET} DESTINATION ${CMAKE_BINARY_DIR})
