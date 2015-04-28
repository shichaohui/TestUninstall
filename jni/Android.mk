LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := appuninstallcallback
LOCAL_SRC_FILES := com_sch_uninstallcallback_UninstallCallback.c

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -llog
 
include $(BUILD_SHARED_LIBRARY)