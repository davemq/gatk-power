project(GKL)
cmake_minimum_required(VERSION 2.8)

# add jni include path
find_package(Java 1.8 REQUIRED)
find_package(JNI)
include_directories(${JNI_INCLUDE_DIRS})

# common compiler and linker settings
set(CMAKE_C_COMPILER /usr/bin/gcc)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_FORTIFY_SOURCE=2 ")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=power9 -mtune=power9 -mvsx -maltivec")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wformat -Werror=format-security -Wconversion -Wfloat-conversion")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fstack-protector-all ")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-delete-null-pointer-checks -fwrapv ")


string(APPEND CMAKE_C_FLAGS " -O3")
#
# If debugging, comment the line above, uncomment the line below, and rebuild
#
#string(APPEND CMAKE_C_FLAGS " -g -Og")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
if(UNIX)
  set(CMAKE_SHARED_LINKER_FLAGS "-static-libgcc -Wl,-z,now")
endif()
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}")
#
# If doing memory debugging in PowerPairHmm.c, uncomment the following line and rebuild
#
#string(APPEND CMAKE_SHARED_LINKER_FLAGS " -Wl,--wrap=posix_memalign -Wl,--wrap=calloc -Wl,--wrap=free -Wl,--wrap=malloc")

# add CMakeLists.txt in subdirectories
set(NATIVE_DIR ${PROJECT_SOURCE_DIR}/src/main/native)

add_subdirectory("${NATIVE_DIR}/pairhmm")
