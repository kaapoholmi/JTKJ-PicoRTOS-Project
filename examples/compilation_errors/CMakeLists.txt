
set(DEFAULT_TARGET compilation_error)

add_executable(${DEFAULT_TARGET}
  ${CMAKE_CURRENT_LIST_DIR}/src/main.c
)


target_link_libraries(${DEFAULT_TARGET} PRIVATE
  pico_stdlib
  FreeRTOS-Kernel
  FreeRTOS-Kernel-Heap4
)
if (PICO_CYW43_SUPPORTED)
    target_link_libraries(${DEFAULT_TARGET} PRIVATE pico_cyw43_arch_none)
endif()

pico_enable_stdio_usb(${DEFAULT_TARGET} 1)
pico_enable_stdio_uart(${DEFAULT_TARGET} 0)

pico_add_extra_outputs(${DEFAULT_TARGET})