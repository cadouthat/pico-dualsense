cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(pico_dualsense C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_library(pico_dualsense
  src/dualsense_hci.cpp
  src/dualsense_l2cap.cpp
  src/dualsense_parser.cpp
  src/pico_dualsense.cpp
)

target_include_directories(pico_dualsense PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include )
target_include_directories(pico_dualsense PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src )

target_link_libraries(pico_dualsense
  pico_stdlib
  pico_btstack_ble
  pico_btstack_classic
  pico_btstack_cyw43
)
