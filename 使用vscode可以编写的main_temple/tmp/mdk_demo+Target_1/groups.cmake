# groups.cmake

# group main  code
add_library(Group_main__code OBJECT
  "${SOLUTION_ROOT}/main/circular_queue.c"
  "${SOLUTION_ROOT}/main/FSP_UART.c"
  "${SOLUTION_ROOT}/main/BSP_IIC_EEPROM.c"
  "${SOLUTION_ROOT}/main/BSP_BQ76920_I2C.c"
  "${SOLUTION_ROOT}/main/BSP_WIFI.c"
)
target_include_directories(Group_main__code PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(Group_main__code PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
add_library(Group_main__code_ABSTRACTIONS INTERFACE)
target_link_libraries(Group_main__code_ABSTRACTIONS INTERFACE
  ${CONTEXT}_ABSTRACTIONS
)
target_compile_options(Group_main__code PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Group_main__code PUBLIC
  Group_main__code_ABSTRACTIONS
)

# group Renesas RA Smart Configurator Common Sources
add_library(Group_Renesas_RA_Smart_Configurator_Common_Sources OBJECT
  "${SOLUTION_ROOT}/generated/Renesas RA Smart Configurator/src/hal_entry.c"
)
target_include_directories(Group_Renesas_RA_Smart_Configurator_Common_Sources PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
  "${SOLUTION_ROOT}/generated/Renesas RA Smart Configurator/src"
)
target_compile_definitions(Group_Renesas_RA_Smart_Configurator_Common_Sources PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
add_library(Group_Renesas_RA_Smart_Configurator_Common_Sources_ABSTRACTIONS INTERFACE)
target_link_libraries(Group_Renesas_RA_Smart_Configurator_Common_Sources_ABSTRACTIONS INTERFACE
  ${CONTEXT}_ABSTRACTIONS
)
target_compile_options(Group_Renesas_RA_Smart_Configurator_Common_Sources PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Group_Renesas_RA_Smart_Configurator_Common_Sources PUBLIC
  Group_Renesas_RA_Smart_Configurator_Common_Sources_ABSTRACTIONS
)
