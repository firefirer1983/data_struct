set(CMAKE_CXX_FLAGS "-Wall -std=c++11 -g -O0")

set(BSTREE
	bstree.cpp
)

include_directories("${PROJECT_SOURCE_DIR}/../googletest/googletest/include")

find_library(GTEST
	NAMES "gtest"
	PATHS ${PROJECT_SOURCE_DIR}/../googletest/build/googlemock/gtest/
	NO_DEFAULT_PATH
)

find_library(PTHREAD
	NAMES "pthread"
	PATHS /usr/lib/x86_64-linux-gnu/
	NO_DEFAULT_PATH)

add_executable(bstree-exe
	${BSTREE}
)

target_link_libraries(
	bstree-exe
	${GTEST}
	${PTHREAD}
)

