cmake_minimum_required(VERSION 3.26)

SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable Hot Reload for MSVC compilers if supported.
if (MSVC AND POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("HorizonEngine")

# Include sub-projects.
add_subdirectory("HorizonEngine")

add_subdirectory("HorizonEditor")
