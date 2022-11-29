/*
 * native_impl.cpp
 *
 * @date Feb 26, 2022
 * @author Andrey Belomutskiy, (c) 2012-2022
 */

#include "pch.h"
#include "com_rusefi_native__EngineLogic.h"
#include "auto_generated_sensor.h"
#include "tunerstudio.h"
#include "live_data.h"
#include "init.h"

#include <memory>

static std::unique_ptr<EngineTestHelper> ethPtr;

static EngineTestHelper* getEth() {
	if (!ethPtr) {
	    printf("make_unique<EngineTestHelper>(TEST_ENGINE)\n");
		ethPtr = std::make_unique<EngineTestHelper>(TEST_ENGINE);
	}
	return ethPtr.get();
}

JNIEXPORT void JNICALL Java_com_rusefi_native_1_EngineLogic_resetTest
  (JNIEnv *, jobject) {
  printf("[native] resetTest\n");
  ethPtr = nullptr;
}

JNIEXPORT jstring JNICALL Java_com_rusefi_native_1_EngineLogic_getVersion(JNIEnv * env, jobject) {
	const char msg[60] = "Hello from unit tests";
	const char* fileName = msg;
	jstring result = env->NewStringUTF(msg);
	printf("[native] hello\n");
	return result;
}

JNIEXPORT jbyteArray JNICALL Java_com_rusefi_native_1_EngineLogic_getConfiguration(JNIEnv *env, jobject instance) {
	jbyteArray retVal = env->NewByteArray(sizeof(engine_configuration_s));
	jbyte *buf = env->GetByteArrayElements(retVal, NULL);
	EngineTestHelper* eth = getEth();

	memcpy(buf, (const void*)&eth->persistentConfig.engineConfiguration, sizeof(engine_configuration_s));
	env->ReleaseByteArrayElements(retVal, buf, 0);

	return retVal;
}

JNIEXPORT void JNICALL Java_com_rusefi_native_1_EngineLogic_setConfiguration(JNIEnv *env, jobject instance,
		jbyteArray data, jint offset, jint size) {
	EngineTestHelper* eth = getEth();

	printf("[native] setConfiguration offset=%d size=%d\n", offset, size);
	jbyte *buf = env->GetByteArrayElements(data, NULL);
	int printSize = size > 4 ? 4 : size;
	for (int i = 0;i < printSize;i++) {
    	printf("[native] buf[%d] %x\n", i, buf[i]);
	}
	memcpy(((uint8_t*)&eth->persistentConfig.engineConfiguration) + offset, buf, size);
}

JNIEXPORT void JNICALL Java_com_rusefi_native_1_EngineLogic_setSensor
  (JNIEnv *env, jobject instance, jstring sensorName, jdouble sensorValue) {

	const char *sensorNameNative = env->GetStringUTFChars(sensorName, 0);

	SensorType type = findSensorTypeByName(sensorNameNative);

	printf("[native] Setting [%s] to %f\n", getSensorType(type), sensorValue);
	Sensor::setMockValue(type, sensorValue);

	env->ReleaseStringUTFChars(sensorName, sensorNameNative);
}

JNIEXPORT void JNICALL Java_com_rusefi_native_1_EngineLogic_setEngineType
  (JNIEnv *, jobject, jint engineType) {
    EngineTestHelper* eth = getEth();
    resetConfigurationExt((engine_type_e)engineType);
}

JNIEXPORT void JNICALL Java_com_rusefi_native_1_EngineLogic_invokePeriodicCallback
  (JNIEnv *, jobject) {

    EngineTestHelper* eth = getEth();
    eth->engine.periodicSlowCallback();
    eth->engine.periodicFastCallback();
}

JNIEXPORT void JNICALL Java_com_rusefi_native_1_EngineLogic_invokeEtbCycle
  (JNIEnv *, jobject) {
  EngineTestHelper* eth = getEth();
  Engine *engine = &eth->engine;

	printf("[native] invokeEtbCycle\n");
	for (int i = 0; i < ETB_COUNT; i++) {
		if (auto etb = engine->etbControllers[i]) {
			etb->update();
		}
	}
}

JNIEXPORT void JNICALL Java_com_rusefi_native_1_EngineLogic_burnRequest
  (JNIEnv *, jobject) {
	printf("[native] onBurnRequest\n");
	getEth();// just to make sure we have initialized
    onBurnRequest();
}

JNIEXPORT void JNICALL Java_com_rusefi_native_1_EngineLogic_initTps
  (JNIEnv *, jobject) {
	printf("[native] initTps\n");
	getEth();// just to make sure we have initialized
    initTps();
}

JNIEXPORT jbyteArray JNICALL Java_com_rusefi_native_1_EngineLogic_getOutputs(JNIEnv * env, jobject instance) {
	jbyteArray retVal = env->NewByteArray(TS_TOTAL_OUTPUT_SIZE);
	jbyte *buf = env->GetByteArrayElements(retVal, NULL);
	EngineTestHelper* eth = getEth();
	updateTunerStudioState();
	copyRange((uint8_t*)buf, getLiveDataFragments(), 0, TS_TOTAL_OUTPUT_SIZE);
	env->ReleaseByteArrayElements(retVal, buf, 0);

	return retVal;
}
