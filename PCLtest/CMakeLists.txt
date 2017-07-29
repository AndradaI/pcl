file(GLOB LIBSRCS
    "*.h"
    "*.hpp"
    "*.cpp"
)
set (CMAKE_CXX_STANDARD 11)
include_directories(../src)
add_executable(PCLtest ${LIBSRCS})

install(TARGETS PCLtest DESTINATION bin)
