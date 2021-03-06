cmake_minimum_required(VERSION 3.17)
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIGURATION>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIGURATION>)
set(CMAKE_XCODE_ATTRIBUTE_EXCLUDED_ARCHS[sdk=iphonesimulator*] "arm64" CACHE INTERNAL "")

# linux detection
if(UNIX AND NOT APPLE)
	set(LINUX TRUE CACHE INTERNAL "")
endif()

if(LINUX)
	set(CMAKE_CXX_STANDARD_LIBRARIES -latomic) # need libatomic on linux
endif()

if(MSVC)
  add_definitions(/MP)
  add_definitions(/Qpar)
endif()

#Enable iPad as well as iPhone (Universal)
set(CMAKE_XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2,3" CACHE INTERNAL "")

project("RavEngine_Samples")

set(RAVENGINE_BUILD_TESTS ON CACHE INTERNAL "")
add_subdirectory("RavEngine")

function (add_sample sample_dir)
	# create the test executable
	SET(APPNAME "RavEngine${sample_dir}")
	SET(sample_dir "Samples/${sample_dir}")

	if (CMAKE_SYSTEM_NAME MATCHES "iOS")
		set(PlatformFiles "iosLaunchScreen.storyboard")
	endif()

	# create the helper header file
	make_directory("${APPNAME}")
	configure_file("Samples/AppInfo.hpp" "${CMAKE_BINARY_DIR}/${APPNAME}/" @ONLY)

	file(GLOB TEST_SOURCES "${sample_dir}/*.cpp" "${sample_dir}/*.hpp" "${sample_dir}/*.h" "${CMAKE_BINARY_DIR}/${APPNAME}/*.hpp" ${PlatformFiles})

	# creates a mac app if applicable
	add_executable("${APPNAME}" ${TEST_SOURCES})
	set_target_properties("${APPNAME}" 
		PROPERTIES 
		MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in"
		XCODE_ATTRIBUTE_BUNDLE_IDENTIFIER "com.ravbug.${APPNAME}"		# with templated plist we can set this
		VS_GLOBAL_OutputType AppContainerExe
	)
	target_link_libraries("${APPNAME}" PUBLIC "RavEngine" )
	target_include_directories("${APPNAME}" PUBLIC "${CMAKE_BINARY_DIR}/${APPNAME}")

	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "${APPNAME}")
	#set_property(TARGET "${APPNAME}" PROPERTY WIN32_EXECUTABLE FALSE) 

	file(GLOB objects "${sample_dir}/objects/*.fbx" "${sample_dir}/objects/*.dae" "${sample_dir}/objects/*.ozz" "${sample_dir}/objects/*.obj", "${sample_dir}/objects/*.ply")
	file(GLOB textures "${sample_dir}/textures/*")
	file(GLOB shaders "${sample_dir}/shaders/*.cmake")
	file(GLOB uis "${sample_dir}/ui/*.rml" "${sample_dir}/ui/*.rcss" "${sample_dir}/ui/*.png")
	file(GLOB sounds "${sample_dir}/sounds/*")

	pack_resources(TARGET "${APPNAME}"
		OBJECTS ${objects}
		SHADERS ${shaders}
		TEXTURES ${textures}
		UIS ${uis}
		SOUNDS ${sounds}
	)

	# fixup mac bundle
	if(APPLE)
		INSTALL(CODE 
			"include(BundleUtilities)
			fixup_bundle(\"${CMAKE_INSTALL_PREFIX}/$<CONFIGURATION>/${APPNAME}.app\" \"\" \"\")
			" 
			COMPONENT Runtime
		)
	endif()
endfunction()

add_sample("AirHockey")
add_sample("Playground")
add_sample("Animation")
add_sample("Perf-ECS")
add_sample("Perf-Draw")
add_sample("Perf-DrawAnimated")
add_sample("Perf-RigidBodies")
add_sample("Perf-Network")
add_sample("Perf-Lighting")
add_sample("GUI")