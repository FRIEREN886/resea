cmake_minimum_required(VERSION 3.27)

# Roots
include("../roots.cmake")

set(CONTEXT mdk_demo+Target_1)
set(TARGET ${CONTEXT})
set(DNAME R7FA6M5BF)
set(DPACK Renesas::RA_DFP@4.6.0)
set(DPACK_DIR "${CMSIS_PACK_ROOT}/Renesas/RA_DFP/4.6.0")
set(OUT_DIR "${SOLUTION_ROOT}/Objects")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(LD_SCRIPT "${SOLUTION_ROOT}/script/fsp.scat")
set(LD_SCRIPT_PP ${LD_SCRIPT})

# Processor Options
set(CPU Cortex-M33)
set(FPU SP_FPU)
set(DSP DSP)
set(SECURE Non-secure)
set(BYTE_ORDER Little-endian)

# Toolchain config map
set(COMPILER AC6)
include("toolchain.cmake")

# Setup project
project(${CONTEXT} LANGUAGES C)

# Enable color diagnostics
set(CMAKE_COLOR_DIAGNOSTICS ON)

# Compilation database
add_custom_target(database
  COMMAND ${CMAKE_COMMAND} -E make_directory "${OUT_DIR}"
  COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" "${OUT_DIR}"
)
set(CMAKE_C_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES})

# Setup context
add_executable(${CONTEXT})
set_target_properties(${CONTEXT} PROPERTIES PREFIX "" SUFFIX ".axf" OUTPUT_NAME "mdk_demo")
set_target_properties(${CONTEXT} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUT_DIR})
add_library(${CONTEXT}_GLOBAL INTERFACE)

# Includes
target_include_directories(${CONTEXT} PUBLIC
  $<$<COMPILE_LANGUAGE:C,CXX>:
    "${SOLUTION_ROOT}/main"
    "${SOLUTION_ROOT}/ra_gen"
    "${SOLUTION_ROOT}/ra"
    "${SOLUTION_ROOT}/ra_cfg"
  >
  "${SOLUTION_ROOT}/RTE/_Target_1"
  "${SOLUTION_ROOT}/generated/Renesas RA Smart Configurator"
  "${SOLUTION_ROOT}/generated/Renesas RA Smart Configurator/ra/arm/CMSIS_5/CMSIS/Core/Include"
  "${SOLUTION_ROOT}/generated/Renesas RA Smart Configurator/ra/fsp/inc"
  "${SOLUTION_ROOT}/generated/Renesas RA Smart Configurator/ra/fsp/inc/api"
  "${SOLUTION_ROOT}/generated/Renesas RA Smart Configurator/ra/fsp/inc/instances"
  "${SOLUTION_ROOT}/generated/Renesas RA Smart Configurator/ra/fsp/src/bsp/cmsis/Device/RENESAS/Include"
  "${SOLUTION_ROOT}/generated/Renesas RA Smart Configurator/ra/fsp/src/bsp/mcu/all"
  "${SOLUTION_ROOT}/generated/Renesas RA Smart Configurator/ra/fsp/src/bsp/mcu/ra6m5"
  "${SOLUTION_ROOT}/generated/Renesas RA Smart Configurator/ra_cfg/fsp_cfg"
  "${SOLUTION_ROOT}/generated/Renesas RA Smart Configurator/ra_cfg/fsp_cfg/bsp"
  "${SOLUTION_ROOT}/generated/Renesas RA Smart Configurator/ra_gen"
  "${SOLUTION_ROOT}/generated/Renesas RA Smart Configurator/src"
)

# Defines
target_compile_definitions(${CONTEXT} PUBLIC
  $<$<COMPILE_LANGUAGE:C,CXX>:
    __MICROLIB
    _RTE_
  >
)

# Compile options
target_compile_options(${CONTEXT} PUBLIC
  $<$<COMPILE_LANGUAGE:C>:
    "SHELL:${CC_CPU}"
    "SHELL:${CC_FLAGS}"
    "SHELL:${CC_SECURE}"
    "SHELL:${CC_BYTE_ORDER}"
    "SHELL:-std=c99"
    "SHELL:-fno-rtti"
    "SHELL:-funsigned-char"
    "SHELL:-ffunction-sections"
    "SHELL:-Wno-license-management -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Waggregate-return -Wfloat-equal"
  >
)

# Compile Options Abstractions
add_library(${CONTEXT}_ABSTRACTIONS INTERFACE)
cbuild_set_options_flags(CC "debug" "on" "" "" CC_OPTIONS_FLAGS_${CONTEXT})
target_compile_options(${CONTEXT}_ABSTRACTIONS INTERFACE
  $<$<COMPILE_LANGUAGE:C>:
    "SHELL:${CC_OPTIONS_FLAGS_${CONTEXT}}"
  >
)

# Add groups and components
include("groups.cmake")
include("components.cmake")

target_link_libraries(${CONTEXT} PUBLIC
  Group_main__code
  Group_Renesas_RA_Smart_Configurator_Common_Sources
  Renesas_Flex_Software_Build_Configuration
  Renesas_Flex_Software_Components_ra
  Renesas_Flex_Software_Generated_Data
  Renesas_Flex_Software_Linker_Script
  Renesas_Flex_Software_RA_Configuration_1_0_0
)

# Linker options
target_link_options(${CONTEXT} PUBLIC
  "SHELL:${LD_CPU}"
  "SHELL:${_LS}\"${LD_SCRIPT_PP}\""
  "SHELL:${LD_SECURE}"
  "SHELL:--library_type=microlib"
  "SHELL:--map"
  "SHELL:--load_addr_map_info"
  "SHELL:--xref"
  "SHELL:--callgraph"
  "SHELL:--symbols"
  "SHELL:--info sizes"
  "SHELL:--info totals"
  "SHELL:--info unused"
  "SHELL:--info veneers"
  "SHELL:--entry=Reset_Handler --no_startup --via=\"C:/Users/32991/Desktop/mian/script/ac6/fsp_keep.via\""
  "SHELL:--diag_suppress=6319,6314"
  "SHELL:--summary_stderr"
  "SHELL:--info summarysizes"
)
set(LD_DEPS ${LD_SCRIPT})
set_target_properties(${CONTEXT} PROPERTIES LINK_DEPENDS "${LD_DEPS}")
