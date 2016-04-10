#
# VKTS Example CMake file.
#

cmake_minimum_required(VERSION 3.2)

set (VKTS_Test "VKTS_Test_Input")

project (${VKTS_Test})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../VKTS_External/include ${CMAKE_CURRENT_SOURCE_DIR}/../VKTS/include)


if (${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm")

	set(VKTS_ARCHITECTURE "arm")
	
else ()

	set(VKTS_ARCHITECTURE "intel")
	
endif ()

if (CMAKE_SIZEOF_VOID_P MATCHES 8)

	set(VKTS_BITS "64")
	
else ()

	set(VKTS_BITS "32")
	
endif ()


if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")

	set(VKTS_OS "Windows")
	
	set(VKTS_WINDOW "Win32")

    if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
		
		set(VKTS_COMPILER "MSVC")

		set(VKTS_LIB ${VKTS_COMPILER}/lib)
		
	else ()
        
		set(VKTS_COMPILER "GNU")
		
		set(VKTS_LIB "lib")
		
    endif ()        

	set(VKTS_ADDITIONAL_LIBS vulkan-1 WinMM Xinput9_1_0)

	include_directories(AFTER C:/VulkanSDK/1.0.8.0/Include)
    
    if (${VKTS_BITS} MATCHES "64")
    
       link_directories(C:/VulkanSDK/1.0.8.0/Bin)
       
    else ()
    
       link_directories(C:/VulkanSDK/1.0.8.0/Bin32)
    
    endif ()
        
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

	set(VKTS_OS "Linux")
	
	set(VKTS_WINDOW "Xlib")
		
	set(VKTS_COMPILER "GNU")

	set(VKTS_LIB "lib")
	
	set(VKTS_ADDITIONAL_LIBS vulkan pthread xcb X11-xcb Xrandr X11)

endif ()


link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../VKTS/${VKTS_LIB})

file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable(${VKTS_Test} ${CPP_FILES})

set_property(TARGET ${VKTS_Test} PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../VKTS_Binaries)
set_property(TARGET ${VKTS_Test} PROPERTY RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/../VKTS_Binaries)
set_property(TARGET ${VKTS_Test} PROPERTY RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/../VKTS_Binaries)
set_property(TARGET ${VKTS_Test} PROPERTY RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_CURRENT_SOURCE_DIR}/../VKTS_Binaries)
set_property(TARGET ${VKTS_Test} PROPERTY RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_CURRENT_SOURCE_DIR}/../VKTS_Binaries)

set_property(TARGET ${VKTS_Test} PROPERTY CXX_STANDARD 11)
set_property(TARGET ${VKTS_Test} PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(${VKTS_Test} VKTS ${VKTS_ADDITIONAL_LIBS})
