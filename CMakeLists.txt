cmake_minimum_required(VERSION 3.19)
project(muduo_m)

SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")

set(INCLUDE_DIR  ${PROJECT_SOURCE_DIR}/include)

file(GLOB MLIB CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_library(mlib SHARED ${MLIB})
target_link_libraries(mlib pthread)
target_include_directories(mlib PUBLIC ${INCLUDE_DIR})




# add executable
file (GLOB TEST_SOURCE "${PROJECT_SOURCE_DIR}/test/*.cpp")

foreach(testsourcefile ${TEST_SOURCE} )
    get_filename_component(test_executable_name ${testsourcefile} NAME_WE)  
    add_executable(${test_executable_name} ${testsourcefile})
    target_include_directories(${test_executable_name} PUBLIC ${INCLUDE_DIR})
    target_link_libraries(${test_executable_name} mlib)
endforeach( testsourcefile ${TEST_SOURCE} )







# add_executable(test_mutex zz_test/test_mutex.cpp)
# target_include_directories(test_mutex PUBLIC ${PROJECT_SOURCE_DIR}/include)
# target_link_libraries(test_mutex mlib)


# add_executable(test_cond zz_test/test_condition.cpp)
# target_include_directories(test_mutex PUBLIC ${PROJECT_SOURCE_DIR}/include)
# target_link_libraries(test_cond mlib)



