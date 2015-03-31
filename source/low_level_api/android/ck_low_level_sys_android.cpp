/*
    Copyright (c) 2007-2010 Takashi Kitao

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/


#ifdef CK_ANDROID


#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <libgen.h>
#include <pthread.h>
#include <jni.h>

#include "ck_low_level_api.h"

#include "ck_sys_all.h" // for ckSysMgr::SysFlag
#include "ck_key_all.h" // for ckKeyMgr::KeyType
#include "ck_draw_all.h" // for ckDrawMgr::deleteAllVramObjForSystem


static ckLowLevelAPI::KeyEventHandler s_key_event_handler = NULL;
static ckLowLevelAPI::MouseEventHandler s_mouse_event_handler = NULL;
static ckLowLevelAPI::ExtraEventHandler s_extra_event_handler = NULL;

static const char* s_app_name;
static u16 s_framebuffer_width;
static u16 s_framebuffer_height;
static u16 s_sys_flag;
static bool s_is_framebuffer_size_changed;
static bool s_is_fullscreen;
static bool s_is_mouse_visible;
static bool (*s_update_func)();


static void destroyFramebuffer() {}


static bool createFramebuffer(u16 new_width, u16 new_height)
{
    return true;
}


bool ckLowLevelAPI::createApplication(const char* title, u16 width, u16 height, u16 sys_flag)
{
    s_app_name = title;
    s_framebuffer_width = width;
    s_framebuffer_height = height;
    s_sys_flag = sys_flag;
    s_is_framebuffer_size_changed = false;
    s_is_fullscreen = (sys_flag & ckSysMgr::FLAG_FULLSCREEN_START) ? true : false;
    s_is_mouse_visible = true;

    s_update_func = NULL; // added for Android

    if (!createFramebuffer(width, height))
    {
        return false;
    }

    setupShaderAPI((sys_flag & ckSysMgr::FLAG_DISABLE_SHADER) ? false : true);

    return true;
}


void ckLowLevelAPI::destroyApplication()
{
    destroyFramebuffer();
}


void ckLowLevelAPI::startApplication(bool (*update_func)(void))
{
    s_update_func = update_func;
}


u16 ckLowLevelAPI::getFramebufferWidth()
{
    return s_framebuffer_width;
}


u16 ckLowLevelAPI::getFramebufferHeight()
{
    return s_framebuffer_height;
}


void ckLowLevelAPI::updateFramebufferSize()
{
    /*
    XGetGeometry(s_dpy, s_win, &root, &win_x, &win_y, &win_width, &win_height, &boarder_width, &depth);

    if (win_width != s_framebuffer_width || win_height != s_framebuffer_height)
    {
        s_framebuffer_width = static_cast<u16>(win_width);
        s_framebuffer_height = static_cast<u16>(win_height);
        s_is_framebuffer_size_changed = true;
    }
    else
    {
        s_is_framebuffer_size_changed = false;
    }
    */
}


bool ckLowLevelAPI::isFramebufferSizeChanged()
{
    return s_is_framebuffer_size_changed;
}


void ckLowLevelAPI::swapFramebuffer() {}


bool ckLowLevelAPI::isFullScreen()
{
    return s_is_fullscreen;
}


bool ckLowLevelAPI::toggleFullScreen(u16 width, u16 height)
{
    destroyFramebuffer();

    s_is_fullscreen = !s_is_fullscreen;

    return createFramebuffer(width, height);
}


void ckLowLevelAPI::setKeyEventHandler(KeyEventHandler handler)
{
    s_key_event_handler = handler;
}


void ckLowLevelAPI::setMouseEventHandler(MouseEventHandler handler)
{
    s_mouse_event_handler = handler;
}


void ckLowLevelAPI::setExtraEventHandler(ExtraEventHandler handler)
{
    s_extra_event_handler = handler;
}


void ckLowLevelAPI::setMousePos(s16 mouse_x, s16 mouse_y)
{
    // TODO
}


bool ckLowLevelAPI::isMouseVisible()
{
    return s_is_mouse_visible;
}


void ckLowLevelAPI::setMouseVisible(bool is_visible)
{
    if (is_visible != s_is_mouse_visible)
    {
        s_is_mouse_visible = is_visible;

        if (s_is_mouse_visible)
        {
            // TODO
        }
        else
        {
            // TODO
        }
    }
}


u64 ckLowLevelAPI::getUsecTime()
{
    static u64 s_start_time;
    static bool s_is_first = true;

    struct timeval time;
    struct timezone tz;

    if (s_is_first)
    {
        s_is_first = false;

        gettimeofday(&time, &tz);
        s_start_time = time.tv_sec * 1000000 + time.tv_usec;
    }

    gettimeofday(&time, &tz);
    u64 cur_time = time.tv_sec * 1000000 + time.tv_usec;

    return cur_time - s_start_time;
}


void ckLowLevelAPI::sleepUsec(u64 usec)
{
    usleep(usec);
}


void ckLowLevelAPI::exit(s32 status)
{
    ckDestroyCatcake();	// by Nakada
    ::exit(status);
}


void ckLowLevelAPI::error(const char* msg)
{
    printf(msg);
    printf("\n");

    exit(1);
}


void ckLowLevelAPI::readLittleEndian(void* dest, const void* src, u32 size)
{
    memcpy(dest, src, size);
}


void ckLowLevelAPI::writeLittleEndian(void* dest, const void* src, u32 size)
{
    memcpy(dest, src, size);
}


struct ThreadStartFuncAndUserParam
{
    void (*start_func)(void*);
    void* user_param;
};


static void* threadStartFunc(void* user_param)
{
    ThreadStartFuncAndUserParam* func_and_param = static_cast<ThreadStartFuncAndUserParam*>(user_param);

    func_and_param->start_func(func_and_param->user_param);

    ckLowLevelAPI::free(user_param);

    return NULL;
}


void* ckLowLevelAPI::newThread(void (*start_func)(void*), void* user_param)
{
    static pthread_mutex_t s_mutex = PTHREAD_MUTEX_INITIALIZER;

    pthread_mutex_lock(&s_mutex);

    void* thread_handler = malloc(sizeof(pthread_t));

    ThreadStartFuncAndUserParam* func_and_param = static_cast<ThreadStartFuncAndUserParam*>(malloc(sizeof(ThreadStartFuncAndUserParam)));
    func_and_param->start_func = start_func;
    func_and_param->user_param = user_param;

    if (pthread_create(static_cast<pthread_t*>(thread_handler), NULL, threadStartFunc, func_and_param))
    {
        free(thread_handler);
        free(func_and_param);

        thread_handler = NULL;
    }

    pthread_mutex_unlock(&s_mutex);

    return thread_handler;
}


void ckLowLevelAPI::deleteThread(void* thread_handler)
{
    free(thread_handler);
}


void ckLowLevelAPI::joinThread(void* thread_handler)
{
    pthread_join(*static_cast<pthread_t*>(thread_handler), NULL);
}


void* ckLowLevelAPI::newMutex()
{
    void* mutex_handler = malloc(sizeof(pthread_mutex_t));

    if (pthread_mutex_init(static_cast<pthread_mutex_t*>(mutex_handler), NULL))
    {
        free(mutex_handler);

        return NULL;
    }
    else
    {
        return mutex_handler;
    }
}


void ckLowLevelAPI::deleteMutex(void* mutex_handler)
{
    pthread_mutex_destroy(static_cast<pthread_mutex_t*>(mutex_handler));

    free(mutex_handler);
}


void ckLowLevelAPI::lockMutex(void* mutex_handler)
{
    pthread_mutex_lock(static_cast<pthread_mutex_t*>(mutex_handler));
}


void ckLowLevelAPI::unlockMutex(void* mutex_handler)
{
    pthread_mutex_unlock(static_cast<pthread_mutex_t*>(mutex_handler));
}


void ckLowLevelAPI::setInitialDirectory(s32 argc, char** argv)
{
    if (argc > 0)
    {
        if (chdir(dirname(argv[0])) != 0)
        {
            return; // to avoid gcc warning
        }
    }
}


void ckLowLevelAPI::getWindowsFontDirectory(char* buf, u32 buf_size) {}


extern void ckMain_();


JNIEnv *jenv;
unsigned char s_camera_yuv[640*480+640*480/2];
/*unsigned char s_camera_yuv_1[640*480+640*480/2];
unsigned char s_camera_yuv_2[640*480+640*480/2];
unsigned char *s_camera_yuv = s_camera_yuv_1;*/
void _cameraFrame(unsigned char *yuv, int width, int height)
{
	//ckLowLevelAPI::printf("_cameraFrame");
}
extern "C" { void (*cameraFrame)(unsigned char *yuv, int width, int height) = _cameraFrame; }
void /*ckLowLevelAPI::*/startCameraPreview(void (*func)(unsigned char*, int, int))
{
	cameraFrame = func;

	/*JNIEnv *env;
	JavaVM *vm = android_app->activity->vm;
	vm->AttachCurrentThread(&env, NULL);*/

	// javap -s CameraView.class
	//jclass jcls = env->GetObjectClass(env, thiz);
	jclass jcls = /*(jclass)jenv->NewGlobalRef*/(jenv->FindClass("berry/catcake/CameraView"));
	jmethodID mj = jenv->GetMethodID(jcls, "StartPreview", "()V");
	jobject jobj = jenv->NewObject(jcls, mj);
	jenv->CallObjectMethod(jobj, mj);
	//jenv->DeleteGlobalRef(jcls);
}
void /*ckLowLevelAPI::*/stopCameraPreview()
{
	jclass jcls = /*(jclass)jenv->NewGlobalRef*/(jenv->FindClass("berry/catcake/CameraView"));
	jmethodID mj = jenv->GetMethodID(jcls, "StopPreview", "()V");
	jobject jobj = jenv->NewObject(jcls, mj);
	jenv->CallObjectMethod(jobj, mj);
	//jenv->DeleteGlobalRef(jcls);
}
void /*ckLowLevelAPI::*/flashOn()
{
	jclass jcls = jenv->FindClass("berry/catcake/CameraView");
	jmethodID mj = jenv->GetMethodID(jcls, "FlashOn", "()V");
	jobject jobj = jenv->NewObject(jcls, mj);
	jenv->CallObjectMethod(jobj, mj);
}
void /*ckLowLevelAPI::*/flashOff()
{
	jclass jcls = jenv->FindClass("berry/catcake/CameraView");
	jmethodID mj = jenv->GetMethodID(jcls, "FlashOff", "()V");
	jobject jobj = jenv->NewObject(jcls, mj);
	jenv->CallObjectMethod(jobj, mj);
}
void /*ckLowLevelAPI::*/setCameraId(int id)
{
	jclass jcls = jenv->FindClass("berry/catcake/CameraView");
	jmethodID mj = jenv->GetMethodID(jcls, "SetCameraId", "(I)V");
	jobject jobj = jenv->NewObject(jcls, mj);
	jenv->CallObjectMethod(jobj, mj, id);
}

#define JNIDEFINE(fname)	Java_berry_catcake_Catcake_##fname
#define JNIFUNC(cname, fname)	Java_berry_catcake_##cname##_##fname
extern "C"
{
	static bool s_is_resumed = false;

	#include <android/asset_manager.h>
	#include <android/asset_manager_jni.h>
	AAssetManager *amgr;

	JNIEXPORT void JNICALL JNIDEFINE(nativeInitialize)(JNIEnv* env, jobject thiz, jobject assetManager, jint width, jint height)
	{
		jenv = env;

		amgr = AAssetManager_fromJava(env, assetManager);
		s_framebuffer_width = width;
		s_framebuffer_height = height;

		ckMain_();
	}

	JNIEXPORT void JNICALL JNIDEFINE(nativeUpdate)(JNIEnv*, jobject)
	{
		if (s_update_func)
		{
			/*if (s_is_resumed)	// by Nakada
			{
				s_is_resumed = false;

				ckDrawMgr::deleteAllVramObjForSystem();
			}*/

			while (!(*s_update_func)());
		}
	}

	JNIEXPORT void JNICALL JNIDEFINE(nativeFinalize)(JNIEnv*, jobject)
	{
		ckLowLevelAPI::exit(0);
	}

	JNIEXPORT void JNICALL JNIDEFINE(nativePause)(JNIEnv*, jobject)
	{
		// do nothing for now
	}

	JNIEXPORT void JNICALL JNIDEFINE(nativeResume)(JNIEnv*, jobject)
	{
		s_is_resumed = true;
		//ckLowLevelAPI::printf("nativeResume");
	}

	JNIEXPORT void JNICALL JNIDEFINE(nativeTouch)(JNIEnv*, jobject, jint action, jint x, jint y)
	{
		switch (action) {
		case 0: // ACTION_DOWN
			(*s_mouse_event_handler)(static_cast<s16>(x), static_cast<s16>(y));
			(*s_key_event_handler)(ckKeyMgr::KEY_LBUTTON, true);
			break;

		case 1: // ACTION_UP:
			(*s_mouse_event_handler)(static_cast<s16>(x), static_cast<s16>(y));
			(*s_key_event_handler)(ckKeyMgr::KEY_LBUTTON, false);
			break;

		case 2: // ACTION_MOVE:
			(*s_mouse_event_handler)(static_cast<s16>(x), static_cast<s16>(y));
			break;

//		default:
//			break;
		}
	}


	JNIEXPORT void JNICALL JNIFUNC(CameraView, nativeCameraFrame)(JNIEnv* env, jobject thiz, jbyteArray yuv420sp, jint width, jint height)
	{
		/*s_camera_yuv = (s_camera_yuv==s_camera_yuv_1) ? s_camera_yuv_2 : s_camera_yuv_1;*/
		env->GetByteArrayRegion(yuv420sp, 0, 640*480+640*480/2, (jbyte*)s_camera_yuv);
		//cameraFrame(s_camera_yuv, width, height);

		/*jbyte *yuv = env->GetByteArrayElements(yuv420sp, (jboolean*)NULL);
		cameraFrame((unsigned char*)yuv, width, height);
		env->ReleaseByteArrayElements(yuv420sp, yuv, JNI_ABORT);*/
	}
}


#endif // CK_ANDROID
