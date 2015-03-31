LOCAL_PATH := $(call my-dir)
NDK_TOOLCHAIN_ROOT=/android-toolchain

include $(CLEAR_VARS)
LOCAL_MODULE := catcake

LOCAL_SRC_FILES := \
	../../../source/cdt/ck_cdt_aabb.cpp \
	../../../source/cdt/ck_cdt_box.cpp \
	../../../source/cdt/ck_cdt_ray.cpp \
	../../../source/cdt/ck_cdt_sph.cpp \
	../../../source/cdt/ck_cdt_tri.cpp \
	../../../source/conf/ck_conf.cpp \
	../../../source/conf/ck_conf_mgr.cpp \
	../../../source/conf/ck_conf_parse.cpp \
	../../../source/conf/ck_ent.cpp \
	../../../source/dbg/ck_dbg_mgr.cpp \
	../../../source/dbg/ck_dbg_mgr_draw.cpp \
	../../../source/dbg/ck_dbg_mgr_dump.cpp \
	../../../source/dbg/ck_dbg_mgr_font.cpp \
	../../../source/draw/ck_col.cpp \
	../../../source/draw/ck_draw.cpp \
	../../../source/draw/ck_draw_mgr.cpp \
	../../../source/draw/ck_font.cpp \
	../../../source/draw/ck_lit.cpp \
	../../../source/draw/ck_lts.cpp \
	../../../source/draw/ck_mdl.cpp \
	../../../source/draw/ck_mdl_data.cpp \
	../../../source/draw/ck_mot.cpp \
	../../../source/draw/ck_mot_data.cpp \
	../../../source/draw/ck_node.cpp \
	../../../source/draw/ck_prim.cpp \
	../../../source/draw/ck_rend.cpp \
	../../../source/draw/ck_scr.cpp \
	../../../source/draw/ck_shd.cpp \
	../../../source/draw/ck_sprt.cpp \
	../../../source/draw/ck_tex.cpp \
	../../../source/draw/rend/ck_rend_light.cpp \
	../../../source/draw/rend/ck_rend_shader.cpp \
	../../../source/gen/ck_id.cpp \
	../../../source/key/ck_key_mgr.cpp \
	../../../source/low_level_api/android/ck_low_level_snd_android.cpp \
	../../../source/low_level_api/android/ck_low_level_sys_android.cpp \
	../../../source/low_level_api/android/opensl_io.c \
	../../../source/low_level_api/ck_low_level_draw_opengl.cpp \
	../../../source/low_level_api/ck_low_level_io_gen.cpp \
	../../../source/low_level_api/ck_low_level_math_gen.cpp \
	../../../source/low_level_api/ck_low_level_mem_gen.cpp \
	../../../source/low_level_api/ck_low_level_util_freetype.cpp \
	../../../source/low_level_api/ck_low_level_util_libjpeg.cpp \
	../../../source/low_level_api/ck_low_level_util_libpng.cpp \
	../../../source/low_level_api/iphone/ck_low_level_snd_iphone.cpp \
	../../../source/low_level_api/iphone/ck_low_level_sys_iphone.cpp \
	../../../source/low_level_api/linux/ck_low_level_snd_linux.cpp \
	../../../source/low_level_api/linux/ck_low_level_sys_linux.cpp \
	../../../source/low_level_api/macosx/ck_low_level_snd_macosx.cpp \
	../../../source/low_level_api/macosx/ck_low_level_sys_macosx.cpp \
	../../../source/low_level_api/win32/ck_low_level_snd_win32.cpp \
	../../../source/low_level_api/win32/ck_low_level_sys_win32.cpp \
	../../../source/math/ck_fix.cpp \
	../../../source/math/ck_mat.cpp \
	../../../source/math/ck_math.cpp \
	../../../source/math/ck_quat.cpp \
	../../../source/math/ck_vec.cpp \
	../../../source/mem/ck_mem_mgr.cpp \
	../../../source/mem/ck_mem_util.cpp \
	../../../source/res/ck_res.cpp \
	../../../source/res/ck_res_mgr.cpp \
	../../../source/snd/ck_snd_mgr.cpp \
	../../../source/snd/ck_snd_mgr_mix.cpp \
	../../../source/sys/ck_sys_mgr.cpp \
	../../../source/sys/ck_sys_util.cpp \
	../../../source/task/ck_task.cpp \
	../../../source/task/ck_task_mgr.cpp \
	../../../source/util/ck_util.cpp \
	../../../source/util/ck_util_import_3ds_model.cpp \
	../../../source/util/ck_util_load_pixel_art_model.cpp

LOCAL_C_INCLUDES := \
	../../../include \
	../../../source \
	../../include/zlib \
	../../include/libpng \
	../../include/freetype \
	../../include/libjpeg \
	../../include/libmad \
	.

LOCAL_CXXFLAGS := -DNDEBUG -DCK_ANDROID

LOCAL_LDLIBS := -L$(LOCAL_PATH) -L$(LOCAL_PATH)/../lib -lfreetype -lpng -lz -lGLESv1_CM -lOpenSLES

#LOCAL_CFLAGS += -DPLATFORM_SDK_VERSION=$(PLATFORM_SDK_VERSION)

include $(BUILD_STATIC_LIBRARY)
#include $(BUILD_SHARED_LIBRARY)
