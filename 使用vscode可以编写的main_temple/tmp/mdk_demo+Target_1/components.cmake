# components.cmake

# component Renesas::Flex Software:Build Configuration
add_library(Renesas_Flex_Software_Build_Configuration INTERFACE)
target_include_directories(Renesas_Flex_Software_Build_Configuration INTERFACE
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
  "${SOLUTION_ROOT}/generated/Renesas RA Smart Configurator/ra_cfg/fsp_cfg"
  "${SOLUTION_ROOT}/generated/Renesas RA Smart Configurator/ra_cfg/fsp_cfg/bsp"
)
target_compile_definitions(Renesas_Flex_Software_Build_Configuration INTERFACE
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
target_link_libraries(Renesas_Flex_Software_Build_Configuration INTERFACE
  ${CONTEXT}_ABSTRACTIONS
)

# component Renesas::Flex Software:Components:ra
add_library(Renesas_Flex_Software_Components_ra OBJECT
  "${SOLUTION_ROOT}/generated/Renesas RA Smart Configurator/ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/startup.c"
  "${SOLUTION_ROOT}/generated/Renesas RA Smart Configurator/ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/system.c"
  "${SOLUTION_ROOT}/generated/Renesas RA Smart Configurator/ra/fsp/src/bsp/mcu/all/bsp_clocks.c"
  "${SOLUTION_ROOT}/generated/Renesas RA Smart Configurator/ra/fsp/src/bsp/mcu/all/bsp_common.c"
  "${SOLUTION_ROOT}/generated/Renesas RA Smart Configurator/ra/fsp/src/bsp/mcu/all/bsp_delay.c"
  "${SOLUTION_ROOT}/generated/Renesas RA Smart Configurator/ra/fsp/src/bsp/mcu/all/bsp_group_irq.c"
  "${SOLUTION_ROOT}/generated/Renesas RA Smart Configurator/ra/fsp/src/bsp/mcu/all/bsp_guard.c"
  "${SOLUTION_ROOT}/generated/Renesas RA Smart Configurator/ra/fsp/src/bsp/mcu/all/bsp_io.c"
  "${SOLUTION_ROOT}/generated/Renesas RA Smart Configurator/ra/fsp/src/bsp/mcu/all/bsp_irq.c"
  "${SOLUTION_ROOT}/generated/Renesas RA Smart Configurator/ra/fsp/src/bsp/mcu/all/bsp_register_protection.c"
  "${SOLUTION_ROOT}/generated/Renesas RA Smart Configurator/ra/fsp/src/bsp/mcu/all/bsp_rom_registers.c"
  "${SOLUTION_ROOT}/generated/Renesas RA Smart Configurator/ra/fsp/src/bsp/mcu/all/bsp_sbrk.c"
  "${SOLUTION_ROOT}/generated/Renesas RA Smart Configurator/ra/fsp/src/bsp/mcu/all/bsp_security.c"
  "${SOLUTION_ROOT}/generated/Renesas RA Smart Configurator/ra/fsp/src/r_ioport/r_ioport.c"
  "${SOLUTION_ROOT}/generated/Renesas RA Smart Configurator/ra/fsp/src/r_sci_i2c/r_sci_i2c.c"
  "${SOLUTION_ROOT}/generated/Renesas RA Smart Configurator/ra/fsp/src/r_sci_uart/r_sci_uart.c"
)
target_include_directories(Renesas_Flex_Software_Components_ra PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
  "${SOLUTION_ROOT}/generated/Renesas RA Smart Configurator/ra/arm/CMSIS_5/CMSIS/Core/Include"
  "${SOLUTION_ROOT}/generated/Renesas RA Smart Configurator/ra/fsp/inc"
  "${SOLUTION_ROOT}/generated/Renesas RA Smart Configurator/ra/fsp/inc/api"
  "${SOLUTION_ROOT}/generated/Renesas RA Smart Configurator/ra/fsp/inc/instances"
  "${SOLUTION_ROOT}/generated/Renesas RA Smart Configurator/ra/fsp/src/bsp/cmsis/Device/RENESAS/Include"
  "${SOLUTION_ROOT}/generated/Renesas RA Smart Configurator/ra/fsp/src/bsp/mcu/all"
  "${SOLUTION_ROOT}/generated/Renesas RA Smart Configurator/ra/fsp/src/bsp/mcu/ra6m5"
)
target_compile_definitions(Renesas_Flex_Software_Components_ra PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
target_compile_options(Renesas_Flex_Software_Components_ra PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Renesas_Flex_Software_Components_ra PUBLIC
  ${CONTEXT}_ABSTRACTIONS
)

# component Renesas::Flex Software:Generated Data
add_library(Renesas_Flex_Software_Generated_Data OBJECT
  "${SOLUTION_ROOT}/generated/Renesas RA Smart Configurator/ra_gen/common_data.c"
  "${SOLUTION_ROOT}/generated/Renesas RA Smart Configurator/ra_gen/hal_data.c"
  "${SOLUTION_ROOT}/generated/Renesas RA Smart Configurator/ra_gen/main.c"
  "${SOLUTION_ROOT}/generated/Renesas RA Smart Configurator/ra_gen/pin_data.c"
  "${SOLUTION_ROOT}/generated/Renesas RA Smart Configurator/ra_gen/vector_data.c"
)
target_include_directories(Renesas_Flex_Software_Generated_Data PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
  "${SOLUTION_ROOT}/generated/Renesas RA Smart Configurator/ra_gen"
)
target_compile_definitions(Renesas_Flex_Software_Generated_Data PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
target_compile_options(Renesas_Flex_Software_Generated_Data PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Renesas_Flex_Software_Generated_Data PUBLIC
  ${CONTEXT}_ABSTRACTIONS
)

# component Renesas::Flex Software:Linker Script
add_library(Renesas_Flex_Software_Linker_Script INTERFACE)
target_include_directories(Renesas_Flex_Software_Linker_Script INTERFACE
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(Renesas_Flex_Software_Linker_Script INTERFACE
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
target_link_libraries(Renesas_Flex_Software_Linker_Script INTERFACE
  ${CONTEXT}_ABSTRACTIONS
)

# component Renesas::Flex Software:RA Configuration@1.0.0
add_library(Renesas_Flex_Software_RA_Configuration_1_0_0 INTERFACE)
target_include_directories(Renesas_Flex_Software_RA_Configuration_1_0_0 INTERFACE
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(Renesas_Flex_Software_RA_Configuration_1_0_0 INTERFACE
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
target_link_libraries(Renesas_Flex_Software_RA_Configuration_1_0_0 INTERFACE
  ${CONTEXT}_ABSTRACTIONS
)
