LOCAL_PATH := $(call my-dir)

#
## libCatNative.so
#
include $(CLEAR_VARS)
LOCAL_MODULE := CatNative
#LOCAL_SRC_FILES := @@SOURCE@@ android_native_app_glue.c
LOCAL_SRC_FILES := @@SOURCE@@
LOCAL_C_INCLUDES := @@INCLUDES@@
LOCAL_CXXFLAGS := -DNDEBUG -DCK_ANDROID
LOCAL_LDLIBS := -L@@LIBS@@ -lcatcake -lfreetype -lpng -lturbojpeg -lmad -lz -lGLESv1_CM -lOpenSLES -lgcc -landroid -llog
include $(BUILD_SHARED_LIBRARY)
