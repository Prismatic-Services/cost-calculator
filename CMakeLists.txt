﻿# Zach - what the fun does this mean


# CMakeList.txt : CMake project for contractor-calculator-instance, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("contractor-calculator-instance")

add_executable(contractor-calculator-instance
        "contractor-calculator-instance.cpp"
        "file_controller.h"
        "utils.h"
        "calculations.h"
        "calls.h")

# Apply static runtime on the target directly (this works)
set_property(TARGET contractor-calculator-instance PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET contractor-calculator-instance PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
