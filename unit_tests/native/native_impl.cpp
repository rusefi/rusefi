/*
 * native_impl.cpp
 *
 * @date Feb 26, 2022
 * @author Andrey Belomutskiy, (c) 2012-2022
 */

#include "pch.h"
#include "com_rusefi_native__EngineLogic.h"
#include "auto_generated_sensor.h"

//static EngineTestHelper eth(TEST_ENGINE);

JNIEXPORT jstring JNICALL Java_com_rusefi_native_1_EngineLogic_getVersion(JNIEnv * env, jobject) {
	const char msg[60] = "Hello from unit tests";
	const char* fileName = msg;
	jstring result = env->NewStringUTF(msg);
	printf("[native] hello\n");
	return result;
}

JNIEXPORT jbyteArray JNICALL Java_com_rusefi_native_1_EngineLogic_getConfiguration(JNIEnv *env, jobject instance) {
	return nullptr;
}


JNIEXPORT void JNICALL Java_com_rusefi_native_1_EngineLogic_setConfiguration(JNIEnv *env, jobject instance,
		jbyteArray data, jint offset, jint size) {

	printf("[native] setConfiguration %d %d\n", offset, size);
//	printf("[native] engine %d %d\n", engineConfiguration->engineType);
}

JNIEXPORT void JNICALL Java_com_rusefi_native_1_EngineLogic_setSensor
  (JNIEnv *env, jobject instance, jstring sensorName, jdouble sensorValue) {

	const char *sensorNameNative = env->GetStringUTFChars(sensorName, 0);

	SensorType type = findSensorTypeByName(sensorNameNative);

	printf("Setting [%s] to %f\n", getSensorType(type), sensorValue);
	Sensor::setMockValue(type, sensorValue);

	env->ReleaseStringUTFChars(sensorName, sensorNameNative);
}

JNIEXPORT void JNICALL Java_com_rusefi_native_1_EngineLogic_invokePeriodicCallback
  (JNIEnv *, jobject) {

}
