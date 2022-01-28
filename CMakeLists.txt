cmake_minimum_required (VERSION 3.15)

project ("photon-find")

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED True)

if (MSVC)
    add_compile_options(/W4 /bigobj)
else()
    add_compile_options(-Wall -Wextra -pedantic -Wa,-mbig-obj)
endif()


add_executable (photon-find "src/common.h" "src/floating_search.h" "src/neighbourfind.cpp" "src/neighbourfind.h" "src/normal_search.h" "src/parse_rule.h" "src/photon-find.cpp" "src/photon-find.h" "src/searcher.h")

