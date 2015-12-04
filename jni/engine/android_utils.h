#ifndef H_ANDROID_UTILS
#define H_ANDROID_UTILS

#include <jni.h>
#include <android/native_activity.h>

#include <SFML/System/NativeActivity.hpp>
#include <SFML/System/Time.hpp>
#include "game_core.h"

class CAndroidUtils
{
public:

	static void get_internal_file_path(char *finalPath, size_t bsize, const char *filePath)
	{
		ANativeActivity *pNatAct =  sf::getNativeActivity();
		memset(finalPath, 0, bsize);
		snprintf(finalPath, bsize, "%s/%s", pNatAct->internalDataPath, filePath);
	}

	static void get_external_file_path(char *finalPath, size_t bsize, const char *filePath)
	{
		ANativeActivity *pNatAct =  sf::getNativeActivity();
		memset(finalPath, 0, bsize);
		snprintf(finalPath, bsize, "%s/%s", pNatAct->externalDataPath, filePath);
	}

	// Original source by MarioLiebisch (https://github.com/SFML/SFML/blob/7272c0b2ac7fbbdc9923e8d4873178db1875261b/examples/android/jni/main.cpp)
	static void vibrate(sf::Time duration)
	{
		CGameCore *pCore = CGameCore::getInstance();
		if (!pCore->m_Config.m_Vibration)
			return;

	    // First we'll need the native activity handle
	    ANativeActivity *activity = sf::getNativeActivity();

	    // Retrieve the JVM and JNI environment
	    JavaVM* vm = activity->vm;
	    JNIEnv* env = activity->env;

	    // First, attach this thread to the main thread
	    JavaVMAttachArgs attachargs;
	    attachargs.version = JNI_VERSION_1_6;
	    attachargs.name = "NativeThread";
	    attachargs.group = NULL;
	    jint res = vm->AttachCurrentThread(&env, &attachargs);

	    if (res == JNI_ERR)
	        return;

	    // Retrieve class information
	    jclass natact = env->FindClass("android/app/NativeActivity");
	    jclass context = env->FindClass("android/content/Context");

	    // Get the value of a constant
	    jfieldID fid = env->GetStaticFieldID(context, "VIBRATOR_SERVICE", "Ljava/lang/String;");
	    jobject svcstr = env->GetStaticObjectField(context, fid);

	    // Get the method 'getSystemService' and call it
	    jmethodID getss = env->GetMethodID(natact, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
	    jobject vib_obj = env->CallObjectMethod(activity->clazz, getss, svcstr);

	    // Get the object's class and retrieve the member name
	    jclass vib_cls = env->GetObjectClass(vib_obj);
	    jmethodID vibrate = env->GetMethodID(vib_cls, "vibrate", "(J)V");

	    // Determine the timeframe
	    jlong length = duration.asMilliseconds();

	    // Bzzz!
	    env->CallVoidMethod(vib_obj, vibrate, length);

	    // Free references
	    env->DeleteLocalRef(vib_obj);
	    env->DeleteLocalRef(vib_cls);
	    env->DeleteLocalRef(svcstr);
	    env->DeleteLocalRef(context);
	    env->DeleteLocalRef(natact);

	    // Detach thread again
	    vm->DetachCurrentThread();
	}
};

#endif
