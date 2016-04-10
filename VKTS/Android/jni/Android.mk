LOCAL_PATH := $(call my-dir)

#
# VKTS.
#
 
include $(CLEAR_VARS)

LOCAL_MODULE := VKTS

# Add all files.

PROJECT_FILES := $(wildcard $(LOCAL_PATH)/../../src/**/**/*.cpp)

# Create list of operating system specific files and remove them.

IGNORE_FILES += $(LOCAL_PATH)/../../src/layer0/file/fn_file_general.cpp
IGNORE_FILES := $(LOCAL_PATH)/../../src/layer0/processor/fn_processor_linux.cpp
IGNORE_FILES += $(LOCAL_PATH)/../../src/layer0/processor/fn_processor_windows.cpp
IGNORE_FILES += $(LOCAL_PATH)/../../src/layer0/visual/fn_visual_none.cpp
IGNORE_FILES += $(LOCAL_PATH)/../../src/layer0/visual/fn_visual_linux.cpp
IGNORE_FILES += $(LOCAL_PATH)/../../src/layer0/visual/fn_visual_xlib.cpp
IGNORE_FILES += $(LOCAL_PATH)/../../src/layer0/visual/fn_visual_xlib_key.cpp
IGNORE_FILES += $(LOCAL_PATH)/../../src/layer0/visual/fn_visual_win32.cpp
IGNORE_FILES += $(LOCAL_PATH)/../../src/layer0/visual/fn_visual_win32_key.cpp
IGNORE_FILES += $(LOCAL_PATH)/../../src/layer0/visual/fn_visual_display.cpp
IGNORE_FILES += $(LOCAL_PATH)/../../src/layer0/visual/fn_visual_display_linux.cpp
IGNORE_FILES += $(LOCAL_PATH)/../../src/layer0/visual/fn_visual_display_linux_key.cpp
IGNORE_FILES += $(LOCAL_PATH)/../../src/layer0/wsi/fn_wsi_xlib.cpp
IGNORE_FILES += $(LOCAL_PATH)/../../src/layer0/wsi/fn_wsi_win32.cpp
IGNORE_FILES += $(LOCAL_PATH)/../../src/layer0/wsi/fn_wsi_display.cpp

PROJECT_FILES := $(filter-out $(IGNORE_FILES), $(PROJECT_FILES))

# Generate the final source list.

PROJECT_FILES := $(PROJECT_FILES:$(LOCAL_PATH)/%=%)

# Enable C++11.

LOCAL_CPPFLAGS := -std=c++11
LOCAL_CPPFLAGS += -fexceptions

# Includes.

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../VKTS_External/include
 
# Sources.

LOCAL_SRC_FILES := $(PROJECT_FILES)

# Libs.
 
LOCAL_STATIC_LIBRARIES := android_native_app_glue
LOCAL_STATIC_LIBRARIES += cpufeatures

include $(BUILD_STATIC_LIBRARY)

$(call import-module,android/native_app_glue)
$(call import-module,android/cpufeatures)
