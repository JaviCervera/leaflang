#Minimum version
cmake_minimum_required(VERSION 2.6)

#Define project
project(pico)

#Define file groups
file(GLOB PICO_FILES "src/*.cpp" "compiler/*.cpp")

#Add executable targets
add_executable(pico ${PICO_FILES})

#Add platform specific options
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	add_definitions(-DWIN32)
	if (MSVC)
		message("Platform is Windows (MSVC)")
	else (MSVC)
		message("Platform is Windows (MinGW)")
		set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-m32")
		set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-m32 -fno-exceptions -fno-rtti")
		set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-s -static")
	endif (MSVC)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	message("Platform is macOS")
	add_definitions(-DGL_SILENCE_DEPRECATION)
	set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-fno-exceptions -fno-rtti")
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	message("Platform is Linux")
	set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-fno-exceptions -fno-rtti")
else (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	message("Unrecognized platform " ${CMAKE_SYSTEM_NAME})
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
