
if (NOT DEFINED ENV{MXE_PATH})
  message(FATAL_ERROR "MXE_PATH MUST BE DEFINED!")
endif()

# ---

set(MXE_TARGET i686-w64-mingw32.static
  CACHE STRING "mxe_target"
)

#
# TODO: AVOID DOUBLE-INCLUSION OF FLAGS
#
set(CMAKE_CXX_FLAGS "-Wno-deprecated-declarations -std=c++1y"
  CACHE STRING "cmake_cxx_flags/mxe"
)

include("$ENV{MXE_PATH}/usr/${MXE_TARGET}/share/cmake/mxe-conf.cmake")

# ---

if (DEFINED RUN AND NOT PROJECT_NAME STREQUAL CMAKE_TRY_COMPILE)
  if (RUN MATCHES EXE)
    set(CONFIG_INSTALL "${CROSS_ROOT}/cmake/install.nop.sh.in")
    set(CONFIG_RUN "${CROSS_ROOT}/cmake/mxe/run.sh.in")
    
  else()
    message(FATAL_ERROR "UNSUPPORTED RUN-MODE!")
  endif()
endif()