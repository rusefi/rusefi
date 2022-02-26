/*
 * native_impl.cpp
 *
 * @date Feb 26, 2022
 * @author Andrey Belomutskiy, (c) 2012-2022
 */

#include "pch.h"
#include "com_rusefi_native__EngineLogic.h"

JNIEXPORT jstring JNICALL Java_com_rusefi_native_1_EngineLogic_getVersion(JNIEnv * env, jobject) {
	const char msg[60] = "Hello from unit tests";
	const char* fileName = msg;
	jstring result = env->NewStringUTF(msg);
	return result;
}

