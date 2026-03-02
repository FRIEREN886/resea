# groups.cmake

# group Drivers
add_library(Group_Drivers OBJECT
  "${SOLUTION_ROOT}/drivers/drv_gpio.c"
  "${SOLUTION_ROOT}/drivers/drv_uart.c"
)
target_include_directories(Group_Drivers PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(Group_Drivers PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
add_library(Group_Drivers_ABSTRACTIONS INTERFACE)
target_link_libraries(Group_Drivers_ABSTRACTIONS INTERFACE
  ${CONTEXT}_ABSTRACTIONS
)
target_compile_options(Group_Drivers PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Group_Drivers PUBLIC
  Group_Drivers_ABSTRACTIONS
)

# group main_code
add_library(Group_main_code OBJECT
  "${SOLUTION_ROOT}/main/BSP_IIC_EEPROM.c"
)
target_include_directories(Group_main_code PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
  "${SOLUTION_ROOT}/main"
)
target_compile_definitions(Group_main_code PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
add_library(Group_main_code_ABSTRACTIONS INTERFACE)
target_link_libraries(Group_main_code_ABSTRACTIONS INTERFACE
  ${CONTEXT}_ABSTRACTIONS
)
target_compile_options(Group_main_code PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Group_main_code PUBLIC
  Group_main_code_ABSTRACTIONS
)
