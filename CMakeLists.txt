cmake_minimum_required(VERSION 3.12)
project (app)
file (GLOB MAIN_SOURCES "*.h" "*.cpp")
add_executable(app ${MAIN_SOURCES})
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT app)

if(MSVC)
	# Properties > C/C++ > Output Files > Assembler Output
	target_compile_options(app PUBLIC "/Fa${PROJECT_BINARY_DIR}/${ASSEMBLY_LISTING_FILE}" /FAs)
	target_compile_features(app PUBLIC cxx_std_20)
endif()
