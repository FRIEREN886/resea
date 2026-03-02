# components.cmake

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
