file(GLOB LIBSRCS
    "*.h"
    "*.hpp"
    "*.cpp"
)

set (CMAKE_CXX_STANDARD 11)
include_directories(../include)
add_library(pcl STATIC ${LIBSRCS})

install(TARGETS pcl DESTINATION lib)
install(DIRECTORY ../include/ DESTINATION include)

