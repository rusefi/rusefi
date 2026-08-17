/*
 * PCANBasic JNI bridge for macOS via MacCAN-Core (libPCBUSB).
 *
 * The rusEFI console's Java layer (peak.can.basic.PCANBasic) loads this
 * library as "pcanbasic_jni". On Windows the equivalent is the prebuilt
 * PCANBasic_JNI.dll (linked against PEAK's PCANBasic.dll); on macOS there is
 * no official PEAK driver, so this bridge loads MacCAN-Core's PCBUSB library
 * (https://www.mac-can.com) which provides the same PCANBasic API for
 * PCAN-USB adapters in user space.
 *
 * The PCBUSB library is loaded dynamically (dlopen) so this dylib builds and
 * links without MacCAN installed; initializeAPI() reports whether the
 * library was found. Install it first:
 *
 *   brew tap mac-can/maccan
 *   brew install pcbusb
 *
 * Build (JDK headers required):
 *
 *   cc -dynamiclib -O2 -o libpcanbasic_jni.dylib \
 *      -I"$JAVA_HOME/include" -I"$JAVA_HOME/include/darwin" \
 *      PCANBasic_JNI_macos.c
 *
 * Run the console with: -Djava.library.path=<dir containing the dylib>
 *
 * Status/parameter semantics follow the PCANBasic API reference (see the
 * Java doc comments in peak/can/basic/PCANBasic.java).
 */
#include <jni.h>
#include <dlfcn.h>
#include <string.h>
#include <stdio.h>

/* ---- PCANBasic type subset (PCANBasic.h) ---- */
/* NOTE: DWORD MUST be 32-bit on all platforms (Windows DWORD semantics).
 * Using 'unsigned long' (64-bit on LP64 macOS) silently shifts every field
 * after ID by 4 bytes -> TX frames go out with DLC=0. */
typedef unsigned short WORD;
typedef unsigned int   DWORD;
typedef unsigned char  BYTE;

typedef struct tagTPCANMsg {
	DWORD ID;
	BYTE  MSGTYPE;
	BYTE  LEN;
	BYTE  DATA[8];
} TPCANMsg;

typedef struct tagTPCANTimestamp {
	DWORD millis;
	WORD  millis_overflow;
	WORD  micros;
} TPCANTimestamp;

/* ---- dynamically resolved PCANBasic entry points ---- */
typedef DWORD (*pfn_Initialize)(WORD, DWORD, BYTE, DWORD, WORD);
typedef DWORD (*pfn_Uninitialize)(WORD);
typedef DWORD (*pfn_Reset)(WORD);
typedef DWORD (*pfn_GetStatus)(WORD);
typedef DWORD (*pfn_Read)(WORD, TPCANMsg*, TPCANTimestamp*);
typedef DWORD (*pfn_Write)(WORD, TPCANMsg*);
typedef DWORD (*pfn_FilterMessages)(WORD, DWORD, DWORD, BYTE);
typedef DWORD (*pfn_GetValue)(WORD, BYTE, void*, DWORD);
typedef DWORD (*pfn_SetValue)(WORD, BYTE, void*, DWORD);
typedef DWORD (*pfn_GetErrorText)(DWORD, WORD, char*);
typedef DWORD (*pfn_LookUpChannel)(char*, WORD*);

static void *g_lib = NULL;
static pfn_Initialize       p_Initialize;
static pfn_Uninitialize     p_Uninitialize;
static pfn_Reset            p_Reset;
static pfn_GetStatus        p_GetStatus;
static pfn_Read             p_Read;
static pfn_Write            p_Write;
static pfn_FilterMessages   p_FilterMessages;
static pfn_GetValue         p_GetValue;
static pfn_SetValue         p_SetValue;
static pfn_GetErrorText     p_GetErrorText;
static pfn_LookUpChannel    p_LookUpChannel;

#define PCAN_ERROR_ILLPARAMTYPE 0x04000L

static const char *libraryCandidates[] = {
	"libPCBUSB.dylib",
	"libPCBUSB.0.dylib",
	/* Homebrew installs into /opt/homebrew (Apple Silicon) or /usr/local
	 * (Intel); neither is in the default dlopen search path. */
	"/opt/homebrew/lib/libPCBUSB.dylib",
	"/opt/homebrew/lib/libPCBUSB.0.dylib",
	"/usr/local/lib/libPCBUSB.dylib",
	"/usr/local/lib/libPCBUSB.0.dylib",
	"libpcanbasic.dylib",
	NULL
};

static int loadLibrary(void) {
	if (g_lib != NULL) {
		return 1;
	}
	for (int i = 0; libraryCandidates[i] != NULL; i++) {
		g_lib = dlopen(libraryCandidates[i], RTLD_NOW);
		if (g_lib != NULL) {
			break;
		}
	}
	if (g_lib == NULL) {
		fprintf(stderr, "pcanbasic_jni: MacCAN PCBUSB library not found "
			"(tried libPCBUSB.dylib); install it with "
			"'brew tap mac-can/maccan && brew install pcbusb'\n");
		return 0;
	}
	p_Initialize     = (pfn_Initialize)dlsym(g_lib, "CAN_Initialize");
	p_Uninitialize   = (pfn_Uninitialize)dlsym(g_lib, "CAN_Uninitialize");
	p_Reset          = (pfn_Reset)dlsym(g_lib, "CAN_Reset");
	p_GetStatus      = (pfn_GetStatus)dlsym(g_lib, "CAN_GetStatus");
	p_Read           = (pfn_Read)dlsym(g_lib, "CAN_Read");
	p_Write          = (pfn_Write)dlsym(g_lib, "CAN_Write");
	p_FilterMessages = (pfn_FilterMessages)dlsym(g_lib, "CAN_FilterMessages");
	p_GetValue       = (pfn_GetValue)dlsym(g_lib, "CAN_GetValue");
	p_SetValue       = (pfn_SetValue)dlsym(g_lib, "CAN_SetValue");
	p_GetErrorText   = (pfn_GetErrorText)dlsym(g_lib, "CAN_GetErrorText");
	p_LookUpChannel  = (pfn_LookUpChannel)dlsym(g_lib, "CAN_LookUpChannel");

	if (p_Initialize == NULL || p_Read == NULL || p_Write == NULL) {
		fprintf(stderr, "pcanbasic_jni: PCBUSB library missing core symbols\n");
		return 0;
	}
	return 1;
}

/* ---- Java enum/object access helpers ---- */

static jclass classOf(JNIEnv *env, jobject obj) {
	return (*env)->GetObjectClass(env, obj);
}

/* Maps an int/short status code to the matching TPCANStatus enum constant. */
static jobject statusFromCode(JNIEnv *env, jint code) {
	static jclass cls = NULL;
	static jobjectArray values = NULL;
	static jfieldID fidValue = NULL;

	if (cls == NULL) {
		jclass local = (*env)->FindClass(env, "peak/can/basic/TPCANStatus");
		if (local == NULL) {
			return NULL;
		}
		cls = (jclass)(*env)->NewGlobalRef(env, local);
		jmethodID mid = (*env)->GetStaticMethodID(env, cls, "values",
			"()[Lpeak/can/basic/TPCANStatus;");
		values = (jobjectArray)(*env)->NewGlobalRef(env,
			(*env)->CallStaticObjectMethod(env, cls, mid));
		fidValue = (*env)->GetFieldID(env, cls, "value", "I");
	}
	jsize n = (*env)->GetArrayLength(env, values);
	for (jsize i = 0; i < n; i++) {
		jobject e = (*env)->GetObjectArrayElement(env, values, i);
		if ((*env)->GetIntField(env, e, fidValue) == code) {
			return e;
		}
	}
	return NULL;
}

/* Maps a WORD channel handle to the matching TPCANHandle enum constant. */
static jobject handleFromCode(JNIEnv *env, WORD code) {
	static jclass cls = NULL;
	static jobjectArray values = NULL;
	static jfieldID fidValue = NULL;

	if (cls == NULL) {
		jclass local = (*env)->FindClass(env, "peak/can/basic/TPCANHandle");
		if (local == NULL) {
			return NULL;
		}
		cls = (jclass)(*env)->NewGlobalRef(env, local);
		jmethodID mid = (*env)->GetStaticMethodID(env, cls, "values",
			"()[Lpeak/can/basic/TPCANHandle;");
		values = (jobjectArray)(*env)->NewGlobalRef(env,
			(*env)->CallStaticObjectMethod(env, cls, mid));
		fidValue = (*env)->GetFieldID(env, cls, "value", "S");
	}
	jsize n = (*env)->GetArrayLength(env, values);
	for (jsize i = 0; i < n; i++) {
		jobject e = (*env)->GetObjectArrayElement(env, values, i);
		if ((*env)->GetShortField(env, e, fidValue) == (jshort)code) {
			return e;
		}
	}
	return NULL;
}

static WORD channelFromJava(JNIEnv *env, jobject channel) {
	static jclass cls = NULL;
	static jfieldID fidValue = NULL;
	if (cls == NULL) {
		jclass local = (*env)->FindClass(env, "peak/can/basic/TPCANHandle");
		cls = (jclass)(*env)->NewGlobalRef(env, local);
		fidValue = (*env)->GetFieldID(env, cls, "value", "S");
	}
	return (WORD)(*env)->GetShortField(env, channel, fidValue);
}

static DWORD baudrateFromJava(JNIEnv *env, jobject baud) {
	static jfieldID fid = NULL;
	if (fid == NULL) {
		jclass local = (*env)->FindClass(env, "peak/can/basic/TPCANBaudrate");
		fid = (*env)->GetFieldID(env, local, "value", "I");
	}
	return (DWORD)(*env)->GetIntField(env, baud, fid);
}

static BYTE byteValueFromJava(JNIEnv *env, jobject obj, const char *className) {
	static jclass clsType = NULL, clsMode = NULL;
	jclass *cls = NULL;
	jfieldID fid;
	if (strcmp(className, "TPCANType") == 0) {
		cls = &clsType;
	} else {
		cls = &clsMode;
	}
	if (*cls == NULL) {
		char name[64];
		snprintf(name, sizeof(name), "peak/can/basic/%s", className);
		jclass local = (*env)->FindClass(env, name);
		*cls = (jclass)(*env)->NewGlobalRef(env, local);
	}
	fid = (*env)->GetFieldID(env, *cls, "value", "B");
	return (BYTE)(*env)->GetByteField(env, obj, fid);
}

static int intValueFromJava(JNIEnv *env, jobject obj, const char *className) {
	char name[64];
	snprintf(name, sizeof(name), "peak/can/basic/%s", className);
	jclass local = (*env)->FindClass(env, name);
	jfieldID fid = (*env)->GetFieldID(env, local, "value", "I");
	return (*env)->GetIntField(env, obj, fid);
}

/* TPCANMsg field access */
static void msgToNative(JNIEnv *env, jobject msg, TPCANMsg *out) {
	static jclass cls = NULL;
	static jfieldID fidId, fidType, fidLen, fidData;
	if (cls == NULL) {
		jclass local = (*env)->FindClass(env, "peak/can/basic/TPCANMsg");
		cls = (jclass)(*env)->NewGlobalRef(env, local);
		fidId = (*env)->GetFieldID(env, cls, "_id", "I");
		fidType = (*env)->GetFieldID(env, cls, "_type", "B");
		fidLen = (*env)->GetFieldID(env, cls, "_length", "B");
		fidData = (*env)->GetFieldID(env, cls, "_data", "[B");
	}
	out->ID = (DWORD)(*env)->GetIntField(env, msg, fidId);
	out->MSGTYPE = (BYTE)(*env)->GetByteField(env, msg, fidType);
	out->LEN = (BYTE)(*env)->GetByteField(env, msg, fidLen);
	jbyteArray arr = (jbyteArray)(*env)->GetObjectField(env, msg, fidData);
	if (arr != NULL) {
		jbyte data[8] = {0};
		jsize n = (*env)->GetArrayLength(env, arr);
		if (n > 8) {
			n = 8;
		}
		(*env)->GetByteArrayRegion(env, arr, 0, n, data);
		memcpy(out->DATA, data, 8);
	}
}

static void nativeToMsg(JNIEnv *env, jobject msg, const TPCANMsg *in) {
	static jclass cls = NULL;
	static jfieldID fidId, fidType, fidLen, fidData;
	if (cls == NULL) {
		jclass local = (*env)->FindClass(env, "peak/can/basic/TPCANMsg");
		cls = (jclass)(*env)->NewGlobalRef(env, local);
		fidId = (*env)->GetFieldID(env, cls, "_id", "I");
		fidType = (*env)->GetFieldID(env, cls, "_type", "B");
		fidLen = (*env)->GetFieldID(env, cls, "_length", "B");
		fidData = (*env)->GetFieldID(env, cls, "_data", "[B");
	}
	(*env)->SetIntField(env, msg, fidId, (jint)in->ID);
	(*env)->SetByteField(env, msg, fidType, (jbyte)in->MSGTYPE);
	(*env)->SetByteField(env, msg, fidLen, (jbyte)in->LEN);
	jbyteArray arr = (jbyteArray)(*env)->GetObjectField(env, msg, fidData);
	if (arr != NULL) {
		jbyte data[8];
		for (int i = 0; i < 8; i++) {
			data[i] = (jbyte)in->DATA[i];
		}
		(*env)->SetByteArrayRegion(env, arr, 0, 8, data);
	}
}

static void nativeToTimestamp(JNIEnv *env, jobject ts, const TPCANTimestamp *in) {
	static jclass cls = NULL;
	static jfieldID fidMillis, fidOverflow, fidMicros;
	if (cls == NULL) {
		jclass local = (*env)->FindClass(env, "peak/can/basic/TPCANTimestamp");
		cls = (jclass)(*env)->NewGlobalRef(env, local);
		fidMillis = (*env)->GetFieldID(env, cls, "millis", "J");
		fidOverflow = (*env)->GetFieldID(env, cls, "millis_overflow", "S");
		fidMicros = (*env)->GetFieldID(env, cls, "micros", "S");
	}
	(*env)->SetLongField(env, ts, fidMillis, (jlong)in->millis);
	(*env)->SetShortField(env, ts, fidOverflow, (jshort)in->millis_overflow);
	(*env)->SetShortField(env, ts, fidMicros, (jshort)in->micros);
}

/* ---- JNI entry points (names fixed by the generated header) ---- */

JNIEXPORT jboolean JNICALL Java_peak_can_basic_PCANBasic_initializeAPI
  (JNIEnv *env, jobject obj) {
	(void)env; (void)obj;
	return loadLibrary() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jobject JNICALL Java_peak_can_basic_PCANBasic_Initialize
  (JNIEnv *env, jobject obj, jobject channel, jobject baud, jobject hwType,
   jint ioPort, jshort interrupt) {
	(void)obj;
	if (!loadLibrary()) {
		return statusFromCode(env, PCAN_ERROR_ILLPARAMTYPE);
	}
	DWORD result = p_Initialize(channelFromJava(env, channel),
		baudrateFromJava(env, baud),
		byteValueFromJava(env, hwType, "TPCANType"),
		(DWORD)ioPort, (WORD)interrupt);
	return statusFromCode(env, (jint)result);
}

JNIEXPORT jobject JNICALL Java_peak_can_basic_PCANBasic_InitializeFD
  (JNIEnv *env, jobject obj, jobject channel, jobject bitrateFD) {
	(void)obj; (void)channel; (void)bitrateFD;
	/* PCAN-USB is not FD capable; MacCAN's PCBUSB does not implement it */
	return statusFromCode(env, PCAN_ERROR_ILLPARAMTYPE);
}

JNIEXPORT jobject JNICALL Java_peak_can_basic_PCANBasic_Uninitialize
  (JNIEnv *env, jobject obj, jobject channel) {
	(void)obj;
	if (!loadLibrary() || p_Uninitialize == NULL) {
		return statusFromCode(env, PCAN_ERROR_ILLPARAMTYPE);
	}
	return statusFromCode(env, (jint)p_Uninitialize(channelFromJava(env, channel)));
}

JNIEXPORT jobject JNICALL Java_peak_can_basic_PCANBasic_Reset
  (JNIEnv *env, jobject obj, jobject channel) {
	(void)obj;
	if (!loadLibrary() || p_Reset == NULL) {
		return statusFromCode(env, PCAN_ERROR_ILLPARAMTYPE);
	}
	return statusFromCode(env, (jint)p_Reset(channelFromJava(env, channel)));
}

JNIEXPORT jobject JNICALL Java_peak_can_basic_PCANBasic_GetStatus
  (JNIEnv *env, jobject obj, jobject channel) {
	(void)obj;
	if (!loadLibrary() || p_GetStatus == NULL) {
		return statusFromCode(env, PCAN_ERROR_ILLPARAMTYPE);
	}
	return statusFromCode(env, (jint)p_GetStatus(channelFromJava(env, channel)));
}

JNIEXPORT jobject JNICALL Java_peak_can_basic_PCANBasic_Read
  (JNIEnv *env, jobject obj, jobject channel, jobject msg, jobject ts) {
	(void)obj;
	if (!loadLibrary()) {
		return statusFromCode(env, PCAN_ERROR_ILLPARAMTYPE);
	}
	TPCANMsg m;
	TPCANTimestamp t;
	memset(&m, 0, sizeof(m));
	memset(&t, 0, sizeof(t));
	DWORD result = p_Read(channelFromJava(env, channel), &m, ts != NULL ? &t : NULL);
	if (result == 0) {
		nativeToMsg(env, msg, &m);
		if (ts != NULL) {
			nativeToTimestamp(env, ts, &t);
		}
	}
	return statusFromCode(env, (jint)result);
}

JNIEXPORT jobject JNICALL Java_peak_can_basic_PCANBasic_ReadFD
  (JNIEnv *env, jobject obj, jobject channel, jobject msg, jobject ts) {
	(void)obj; (void)channel; (void)msg; (void)ts;
	return statusFromCode(env, PCAN_ERROR_ILLPARAMTYPE);
}

JNIEXPORT jobject JNICALL Java_peak_can_basic_PCANBasic_Write
  (JNIEnv *env, jobject obj, jobject channel, jobject msg) {
	(void)obj;
	if (!loadLibrary()) {
		return statusFromCode(env, PCAN_ERROR_ILLPARAMTYPE);
	}
	TPCANMsg m;
	msgToNative(env, msg, &m);
	return statusFromCode(env, (jint)p_Write(channelFromJava(env, channel), &m));
}

JNIEXPORT jobject JNICALL Java_peak_can_basic_PCANBasic_WriteFD
  (JNIEnv *env, jobject obj, jobject channel, jobject msg) {
	(void)obj; (void)channel; (void)msg;
	return statusFromCode(env, PCAN_ERROR_ILLPARAMTYPE);
}

JNIEXPORT jobject JNICALL Java_peak_can_basic_PCANBasic_FilterMessages
  (JNIEnv *env, jobject obj, jobject channel, jint fromId, jint toId, jobject mode) {
	(void)obj;
	if (!loadLibrary() || p_FilterMessages == NULL) {
		return statusFromCode(env, PCAN_ERROR_ILLPARAMTYPE);
	}
	DWORD result = p_FilterMessages(channelFromJava(env, channel),
		(DWORD)fromId, (DWORD)toId,
		byteValueFromJava(env, mode, "TPCANMode"));
	return statusFromCode(env, (jint)result);
}

JNIEXPORT jobject JNICALL Java_peak_can_basic_PCANBasic_GetValue
  (JNIEnv *env, jobject obj, jobject channel, jobject parameter,
   jobject buffer, jint bufferLength) {
	(void)obj;
	if (!loadLibrary() || p_GetValue == NULL) {
		return statusFromCode(env, PCAN_ERROR_ILLPARAMTYPE);
	}
	BYTE param = (BYTE)intValueFromJava(env, parameter, "TPCANParameter");
	char buf[128] = {0};
	if ((DWORD)bufferLength > sizeof(buf)) {
		return statusFromCode(env, PCAN_ERROR_ILLPARAMTYPE);
	}
	DWORD result = p_GetValue(channelFromJava(env, channel), param,
		buf, (DWORD)bufferLength);
	(void)buffer;
	return statusFromCode(env, (jint)result);
}

JNIEXPORT jobject JNICALL Java_peak_can_basic_PCANBasic_SetValue
  (JNIEnv *env, jobject obj, jobject channel, jobject parameter,
   jobject buffer, jint bufferLength) {
	(void)obj;
	if (!loadLibrary() || p_SetValue == NULL) {
		return statusFromCode(env, PCAN_ERROR_ILLPARAMTYPE);
	}
	BYTE param = (BYTE)intValueFromJava(env, parameter, "TPCANParameter");
	char buf[128] = {0};
	if ((DWORD)bufferLength > sizeof(buf)) {
		return statusFromCode(env, PCAN_ERROR_ILLPARAMTYPE);
	}
	DWORD result = p_SetValue(channelFromJava(env, channel), param,
		buf, (DWORD)bufferLength);
	(void)buffer;
	return statusFromCode(env, (jint)result);
}

JNIEXPORT jobject JNICALL Java_peak_can_basic_PCANBasic_GetErrorText
  (JNIEnv *env, jobject obj, jobject error, jshort language, jobject buffer) {
	(void)obj;
	if (!loadLibrary() || p_GetErrorText == NULL) {
		return statusFromCode(env, PCAN_ERROR_ILLPARAMTYPE);
	}
	jint code = (*env)->GetIntField(env, error,
		(*env)->GetFieldID(env, classOf(env, error), "value", "I"));
	char text[256] = {0};
	DWORD result = p_GetErrorText((DWORD)code, (WORD)language, text);
	jstring s = (*env)->NewStringUTF(env, text);
	jclass cls = (*env)->GetObjectClass(env, buffer);
	jmethodID append = (*env)->GetMethodID(env, cls, "append",
		"(Ljava/lang/String;)Ljava/lang/StringBuffer;");
	(*env)->CallObjectMethod(env, buffer, append, s);
	return statusFromCode(env, (jint)result);
}

JNIEXPORT jobject JNICALL Java_peak_can_basic_PCANBasic_LookUpChannel
  (JNIEnv *env, jobject obj, jobject parameters, jobject foundChannel) {
	(void)obj;
	if (!loadLibrary() || p_LookUpChannel == NULL) {
		return statusFromCode(env, PCAN_ERROR_ILLPARAMTYPE);
	}
	const char *params = (*env)->GetStringUTFChars(env, (jstring)parameters, NULL);
	WORD channel = 0;
	DWORD result = p_LookUpChannel((char *)params, &channel);
	(*env)->ReleaseStringUTFChars(env, (jstring)parameters, params);

	jclass mutCls = (*env)->GetObjectClass(env, foundChannel);
	jfieldID fid = (*env)->GetFieldID(env, mutCls, "value",
		"Lpeak/can/basic/TPCANHandle;");
	jobject handle = handleFromCode(env, channel);
	(*env)->SetObjectField(env, foundChannel, fid, handle);
	return statusFromCode(env, (jint)result);
}

/* Receive-event support is not part of MacCAN's PCBUSB subset; the console
 * polls CAN_Read and does not use these. */
JNIEXPORT jobject JNICALL Java_peak_can_basic_PCANBasic_SetRcvEvent
  (JNIEnv *env, jobject obj, jobject channel) {
	(void)obj; (void)channel;
	return statusFromCode(env, PCAN_ERROR_ILLPARAMTYPE);
}

JNIEXPORT jobject JNICALL Java_peak_can_basic_PCANBasic_ResetRcvEvent
  (JNIEnv *env, jobject obj, jobject channel) {
	(void)obj; (void)channel;
	return statusFromCode(env, PCAN_ERROR_ILLPARAMTYPE);
}
