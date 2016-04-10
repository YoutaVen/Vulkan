#
# VulKan ToolS (VKTS) CMake file.
#

cmake_minimum_required(VERSION 3.2)

project (VKTS)

set(VKTS_WSI "" CACHE STRING "Enter VKTS_DISPLAY_VISUAL for alternative WSI. Enter VKTS_NO_VISUAL for no WSI.")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../VKTS_External/include ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

list(APPEND IGNORE_CPP_FILES )

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")

	include_directories(AFTER C:/VulkanSDK/1.0.8.0/Include)

    if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)

        add_definitions(-D_CRT_SECURE_NO_WARNINGS)
        
    endif ()        

    list(APPEND IGNORE_CPP_FILES    ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/file/fn_file_android.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/processor/fn_processor_android.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/processor/fn_processor_linux.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/visual/fn_visual_android.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/visual/fn_visual_android_key.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/visual/fn_visual_linux.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/visual/fn_visual_display.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/visual/fn_visual_display_linux.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/visual/fn_visual_display_linux_key.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/visual/fn_visual_xlib.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/visual/fn_visual_xlib_key.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/wsi/fn_wsi_android.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/wsi/fn_wsi_display.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/wsi/fn_wsi_xlib.cpp
    )

    if (${VKTS_WSI} MATCHES "VKTS_NO_VISUAL")

        add_definitions(-DVKTS_NO_VISUAL)

        list(APPEND IGNORE_CPP_FILES        ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/visual/fn_visual_win32.cpp
                                            ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/visual/fn_visual_win32_key.cpp
                                            ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/wsi/fn_wsi.cpp
                                            ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/wsi/fn_wsi_win32.cpp
                                            ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/wsi/Surface.cpp
                                            ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/wsi/Swapchain.cpp
        )

    else ()

        list(APPEND IGNORE_CPP_FILES        ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/visual/fn_visual_none.cpp
        )
        
    endif ()

elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

    list(APPEND IGNORE_CPP_FILES        ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/file/fn_file_android.cpp
                                        ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/processor/fn_processor_android.cpp
                                        ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/processor/fn_processor_windows.cpp
                                        ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/visual/fn_visual_android.cpp
                                        ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/visual/fn_visual_android_key.cpp
                                        ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/visual/fn_visual_win32.cpp
                                        ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/visual/fn_visual_win32_key.cpp
                                        ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/wsi/fn_wsi_android.cpp
                                        ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/wsi/fn_wsi_win32.cpp
    )

    if (${VKTS_WSI} MATCHES "VKTS_NO_VISUAL")

        add_definitions(-DVKTS_NO_VISUAL)

        list(APPEND IGNORE_CPP_FILES        ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/visual/fn_visual_display.cpp
                                            ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/visual/fn_visual_display_linux.cpp
                                            ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/visual/fn_visual_display_linux_key.cpp
                                            ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/visual/fn_visual_xlib.cpp
                                            ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/visual/fn_visual_xlib_key.cpp
                                            ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/wsi/fn_wsi.cpp
                                            ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/wsi/fn_wsi_display.cpp
                                            ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/wsi/fn_wsi_xlib.cpp
                                            ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/wsi/Surface.cpp
                                            ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/wsi/Swapchain.cpp
        )
    
    elseif (${VKTS_WSI} MATCHES "VKTS_DISPLAY_VISUAL")

        add_definitions(-DVKTS_DISPLAY_VISUAL)

        list(APPEND IGNORE_CPP_FILES    ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/visual/fn_visual_xlib.cpp
                                        ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/visual/fn_visual_xlib_key.cpp
                                        ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/visual/fn_visual_none.cpp
                                        ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/wsi/fn_wsi_xlib.cpp
        )

    else ()

        list(APPEND IGNORE_CPP_FILES    ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/visual/fn_visual_display.cpp
                                        ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/visual/fn_visual_display_linux.cpp
                                        ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/visual/fn_visual_display_linux_key.cpp
                                        ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/visual/fn_visual_none.cpp
                                        ${CMAKE_CURRENT_SOURCE_DIR}/src/layer0/wsi/fn_wsi_display.cpp
        )
        
    endif ()

endif ()

list(REMOVE_ITEM CPP_FILES ${IGNORE_CPP_FILES})

add_library(VKTS ${CPP_FILES})

set_property(TARGET VKTS PROPERTY CXX_STANDARD 11)
set_property(TARGET VKTS PROPERTY CXX_STANDARD_REQUIRED ON)
