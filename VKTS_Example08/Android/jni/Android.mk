LOCAL_PATH			:= $(call my-dir)

#
# VKTS.
#

include $(CLEAR_VARS)

LOCAL_MODULE := VKTS

LOCAL_SRC_FILES := $(LOCAL_PATH)/../../../VKTS/Android/obj/local/$(TARGET_ARCH_ABI)/libVKTS.a

include $(PREBUILT_STATIC_LIBRARY)

#
# Example.
#

include $(CLEAR_VARS)

LOCAL_MODULE := VKTS_Example08

# All files.

PROJECT_FILES := $(wildcard $(LOCAL_PATH)/../../src/*.cpp)

# Generate the final list.

PROJECT_FILES := $(PROJECT_FILES:$(LOCAL_PATH)/%=%)

# Enable C++11.

LOCAL_CPPFLAGS := -std=c++11
LOCAL_CPPFLAGS += -fexceptions

# No visual/window.

LOCAL_CPPFLAGS += -DVKTS_NO_VISUAL=1

# Includes.

LOCAL_C_INCLUDES:= $(LOCAL_PATH)/../../../VKTS/include/
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../VKTS_External/include

# Sources.

LOCAL_SRC_FILES := $(PROJECT_FILES)

# Libs.

LOCAL_LDLIBS    := -landroid -lvulkan

LOCAL_STATIC_LIBRARIES := VKTS
LOCAL_STATIC_LIBRARIES += android_native_app_glue
LOCAL_STATIC_LIBRARIES += cpufeatures

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
$(call import-module,android/cpufeatures)
