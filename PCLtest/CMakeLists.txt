file(GLOB LIBSRCS
    "*.h"
    "*.hpp"
    "*.cpp"
)
set (CMAKE_CXX_STANDARD 11)
include_directories(../src ../include)
add_definitions(-DDEBUG)
add_executable(PCLtest ${LIBSRCS})
target_link_libraries(PCLtest pcl)

install(TARGETS PCLtest DESTINATION bin)
