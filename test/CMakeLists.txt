# Host-side unit tests. Built with the PC's native gcc, never the ARM toolchain.
cmake_minimum_required(VERSION 3.22)
project(nucleo_host_tests C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# CMock pulls in a matching Unity as a git submodule (vendor/unity),
# so one fetch gives us both, at versions tested together.
include(FetchContent)
FetchContent_Declare(cmock
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/CMock.git
    GIT_TAG        v2.6.0
    GIT_SHALLOW    TRUE
    SOURCE_SUBDIR  no-cmake)    # CMock has no CMakeLists: just download it
FetchContent_MakeAvailable(cmock)
set(CMOCK_DIR ${cmock_SOURCE_DIR})
set(UNITY_DIR ${CMOCK_DIR}/vendor/unity)
add_subdirectory(${UNITY_DIR} ${CMAKE_CURRENT_BINARY_DIR}/unity)

find_program(RUBY ruby REQUIRED)

set(REPO_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(APP_INC   ${REPO_ROOT}/App/Inc)
set(APP_SRC   ${REPO_ROOT}/App/Src)
set(BSP_INC   ${REPO_ROOT}/Bsp/Inc)
set(MOCK_DIR  ${CMAKE_CURRENT_BINARY_DIR}/mocks)
set(CMOCK_CFG ${CMAKE_CURRENT_SOURCE_DIR}/cmock.yml)

option(ENABLE_COVERAGE "Build tests with gcov instrumentation" OFF)
add_compile_options(-Wall -Wextra -Werror -g)
if(ENABLE_COVERAGE)
    add_compile_options(--coverage)
    add_link_options(--coverage)
endif()

# CMock runtime, shared by every test that uses mocks.
add_library(cmock STATIC ${CMOCK_DIR}/src/cmock.c)
target_include_directories(cmock PUBLIC ${CMOCK_DIR}/src)
target_link_libraries(cmock PUBLIC unity)
target_compile_options(cmock PRIVATE -Wno-error)   # third-party code

# mock_<name>.c/.h generated from a header in Bsp/Inc.
function(add_mock header)
    get_filename_component(name ${header} NAME_WE)
    add_custom_command(
        OUTPUT  ${MOCK_DIR}/mock_${name}.c ${MOCK_DIR}/mock_${name}.h
        COMMAND ${RUBY} ${CMOCK_DIR}/lib/cmock.rb -o${CMOCK_CFG} --mock_path=${MOCK_DIR} ${header}
        DEPENDS ${header} ${CMOCK_CFG}
        COMMENT "CMock: mocking ${name}.h")
endfunction()

add_mock(${BSP_INC}/bsp_led.h)
add_mock(${BSP_INC}/bsp_time.h)

# add_unity_test(<name> SOURCES <code under test...> MOCKS <header names...>)
# Generates the test runner (main) from test/unit/<name>.c.
function(add_unity_test name)
    cmake_parse_arguments(T "" "" "SOURCES;MOCKS" ${ARGN})
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/unit/${name}.c)
    set(runner   ${CMAKE_CURRENT_BINARY_DIR}/runners/${name}_runner.c)
    add_custom_command(
        OUTPUT  ${runner}
        COMMAND ${RUBY} ${UNITY_DIR}/auto/generate_test_runner.rb ${CMOCK_CFG} ${test_src} ${runner}
        DEPENDS ${test_src} ${CMOCK_CFG}
        COMMENT "Unity: generating runner for ${name}")
    set(mock_srcs "")
    foreach(m ${T_MOCKS})
        list(APPEND mock_srcs ${MOCK_DIR}/mock_${m}.c)
    endforeach()
    add_executable(${name} ${test_src} ${runner} ${T_SOURCES} ${mock_srcs})
    target_include_directories(${name} PRIVATE ${APP_INC} ${BSP_INC} ${MOCK_DIR})
    target_link_libraries(${name} PRIVATE unity cmock)
    add_test(NAME ${name} COMMAND ${name})
endfunction()

enable_testing()

add_unity_test(test_sanity)
add_unity_test(test_blinky SOURCES ${APP_SRC}/blinky.c)
add_unity_test(test_app    SOURCES ${APP_SRC}/app.c ${APP_SRC}/blinky.c
                           MOCKS   bsp_led bsp_time)