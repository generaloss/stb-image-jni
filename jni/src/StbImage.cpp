#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "StbImage.h"
#include <cstdio>
#include <string>

struct StbImageIOCallbacksWrapper {
    JNIEnv *env;
    jobject callbackObj;
};

int stbiRead(void *user, char *data, int size) {
    const auto *wrapper = static_cast<StbImageIOCallbacksWrapper *>(user);
    JNIEnv *env = wrapper->env;

    jclass cls = env->GetObjectClass(wrapper->callbackObj);
    jmethodID readMethod = env->GetMethodID(cls, "read", "([BI)I");

    jbyteArray byteArray = env->NewByteArray(size);
    const jint result = env->CallIntMethod(wrapper->callbackObj, readMethod, byteArray, size);

    if(result > 0)
        env->GetByteArrayRegion(byteArray, 0, result, reinterpret_cast<jbyte *>(data));

    env->DeleteLocalRef(byteArray);
    return result;
}

void stbiSkip(void *user, int n) {
    const auto *wrapper = static_cast<StbImageIOCallbacksWrapper *>(user);
    JNIEnv *env = wrapper->env;

    jclass cls = env->GetObjectClass(wrapper->callbackObj);
    jmethodID skipMethod = env->GetMethodID(cls, "skip", "(I)V");

    env->CallVoidMethod(wrapper->callbackObj, skipMethod, n);
}

int stbiEof(void *user) {
    const auto *wrapper = static_cast<StbImageIOCallbacksWrapper *>(user);
    JNIEnv *env = wrapper->env;

    jclass cls = env->GetObjectClass(wrapper->callbackObj);
    jmethodID eofMethod = env->GetMethodID(cls, "eof", "()I");

    const jboolean result = env->CallBooleanMethod(wrapper->callbackObj, eofMethod);
    return result ? 1 : 0;
}


static std::string getFilePath(JNIEnv* env, jobject fileObj) {
    jclass fileClass = env->GetObjectClass(fileObj);
    jmethodID getPathMethod = env->GetMethodID(fileClass, "getPath", "()Ljava/lang/String;");
    const auto pathStr = (jstring) env->CallObjectMethod(fileObj, getPathMethod);

    const char* pathCStr = env->GetStringUTFChars(pathStr, nullptr);
    std::string path(pathCStr);
    env->ReleaseStringUTFChars(pathStr, pathCStr);
    return path;
}



// String failureReason()
JNIEXPORT jstring JNICALL Java_generaloss_stb_image_StbImage_failureReason
(JNIEnv *env, jclass clazz)
{
    const char *reason = stbi_failure_reason();
    if(reason == nullptr)
        return env->NewStringUTF("Unknown error");

    return env->NewStringUTF(reason);
}


// boolean info(String filename, int[] width, int[] height, int[] channels)
JNIEXPORT jboolean JNICALL Java_generaloss_stb_image_StbImage_info
(JNIEnv *env, jclass clazz, jstring filename, jintArray width, jintArray height, jintArray channels)
{
    const char *cFilename = env->GetStringUTFChars(filename, JNI_FALSE);

    int w, h, c;
    const int result = stbi_info(cFilename, &w, &h, &c);

    env->ReleaseStringUTFChars(filename, cFilename);

    if(result) {
        if(width)
            env->SetIntArrayRegion(width, 0, 1, &w);
        if(height)
            env->SetIntArrayRegion(height, 0, 1, &h);
        if(channels)
            env->SetIntArrayRegion(channels, 0, 1, &c);
    }

    return result;
}

// boolean infoFromFile(long filePtr, int[] width, int[] height, int[] channels)
JNIEXPORT jboolean JNICALL Java_generaloss_stb_image_StbImage_infoFromFile
(JNIEnv *env, jclass clazz, jlong filePtr, jintArray width, jintArray height, jintArray channels)
{
    const auto file = reinterpret_cast<FILE *>(filePtr);
    if(!file)
        return JNI_FALSE;

    int w, h, c;
    const jboolean result = stbi_info_from_file(file, &w, &h, &c) ? JNI_TRUE : JNI_FALSE;

    if(width)
        env->SetIntArrayRegion(width, 0, 1, &w);
    if(height)
        env->SetIntArrayRegion(height, 0, 1, &h);
    if(channels)
        env->SetIntArrayRegion(channels, 0, 1, &c);

    return result;
}

// boolean infoFromMemory(ByteBuffer buffer, int[] width, int[] height, int[] channels)
JNIEXPORT jboolean JNICALL Java_generaloss_stb_image_StbImage_infoFromMemory
(JNIEnv *env, jclass clazz, jbyteArray dataArray, jintArray width, jintArray height, jintArray channels)
{
    jbyte *dataPtr = env->GetByteArrayElements(dataArray, nullptr);
    if(!dataPtr)
        return JNI_FALSE;

    const auto* data = reinterpret_cast<const unsigned char *>(dataPtr);
    const jsize length = env->GetArrayLength(dataArray);

    int w, h, c;
    const jboolean result = stbi_info_from_memory(data, length, &w, &h, &c) ? JNI_TRUE : JNI_FALSE;

    if(width)
        env->SetIntArrayRegion(width, 0, 1, &w);
    if(height)
        env->SetIntArrayRegion(height, 0, 1, &h);
    if(channels)
        env->SetIntArrayRegion(channels, 0, 1, &c);

    return result;
}

// boolean infoFromCallbacks(StbImageIoCallbacks callbacks, long userData, int[] width, int[] height, int[] channels)
JNIEXPORT jboolean JNICALL Java_generaloss_stb_image_StbImage_infoFromCallbacks
(JNIEnv *env, jclass clazz, jobject callbackObj, jintArray width, jintArray height, jintArray channels)
{
    stbi_io_callbacks callbacks;
    callbacks.read = stbiRead;
    callbacks.skip = stbiSkip;
    callbacks.eof = stbiEof;

    StbImageIOCallbacksWrapper wrapper = {env, callbackObj};

    int w, h, c;
    const jboolean result = stbi_info_from_callbacks(&callbacks, &wrapper, &w, &h, &c) ? JNI_TRUE : JNI_FALSE;

    if(width)
        env->SetIntArrayRegion(width, 0, 1, &w);
    if(height)
        env->SetIntArrayRegion(height, 0, 1, &h);
    if(channels)
        env->SetIntArrayRegion(channels, 0, 1, &c);

    return result;
}


// void setFlipVerticallyOnLoad(boolean flip)
JNIEXPORT void JNICALL Java_generaloss_stb_image_StbImage_setFlipVerticallyOnLoad
(JNIEnv *env, jclass clazz, jboolean flip)
{
    stbi_set_flip_vertically_on_load(flip);
}

// void setFlipVerticallyOnLoadThread(boolean shouldFlip)
JNIEXPORT void JNICALL Java_generaloss_stb_image_StbImage_setFlipVerticallyOnLoadThread
(JNIEnv *env, jclass clazz, jboolean shouldFlip)
{
    stbi_set_flip_vertically_on_load_thread(shouldFlip ? 1 : 0);
}

// void setUnpremultiplyOnLoad(boolean shouldUnpremultiply)
JNIEXPORT void JNICALL Java_generaloss_stb_image_StbImage_setUnpremultiplyOnLoad
(JNIEnv *env, jclass clazz, jboolean shouldUnpremultiply)
{
    stbi_set_unpremultiply_on_load(shouldUnpremultiply ? 1 : 0);
}

// void setUnpremultiplyOnLoadThread(boolean shouldUnpremultiply)
JNIEXPORT void JNICALL Java_generaloss_stb_image_StbImage_setUnpremultiplyOnLoadThread
(JNIEnv *env, jclass clazz, jboolean shouldUnpremultiply)
{
    stbi_set_unpremultiply_on_load_thread(shouldUnpremultiply ? 1 : 0);
}


// ByteBuffer load(String filename, int[] width, int[] height, int[] channels, int desiredChannels)
JNIEXPORT jobject JNICALL Java_generaloss_stb_image_StbImage_load
(JNIEnv *env, jclass clazz, jstring filename, jintArray width, jintArray height, jintArray channels, jint desiredChannels)
{
    const char *cFilename = env->GetStringUTFChars(filename, JNI_FALSE);

    int w, h, c;
    unsigned char *data = stbi_load(cFilename, &w, &h, &c, desiredChannels);

    env->ReleaseStringUTFChars(filename, cFilename);
    if(!data)
        return nullptr;

    if(width)
        env->SetIntArrayRegion(width, 0, 1, &w);
    if(height)
        env->SetIntArrayRegion(height, 0, 1, &h);
    if(channels)
        env->SetIntArrayRegion(channels, 0, 1, &c);

    return env->NewDirectByteBuffer(data, w * h * (desiredChannels ? desiredChannels : c));
}

// ByteBuffer loadFromFile(File file, int[] width, int[] height, int[] channels, int reqComp)
JNIEXPORT jobject JNICALL Java_generaloss_stb_image_StbImage_loadFromFile
(JNIEnv* env, jclass, jobject fileObj, jintArray width, jintArray height, jintArray channels, jint desiredChannels)
{
    const std::string path = getFilePath(env, fileObj);
    FILE* file = fopen(path.c_str(), "rb");
    if(!file)
        return nullptr;

    int w, h, c;
    unsigned char* data = stbi_load_from_file(file, &w, &h, &c, desiredChannels);
    fclose(file);

    if(!data)
        return nullptr;

    if(width)
        env->SetIntArrayRegion(width, 0, 1, &w);
    if(height)
        env->SetIntArrayRegion(height, 0, 1, &h);
    if(channels)
        env->SetIntArrayRegion(channels, 0, 1, &c);

    const size_t bufferSize = w * h * (desiredChannels ? desiredChannels : c);
    return env->NewDirectByteBuffer(data, static_cast<jlong>(bufferSize));
}

// ByteBuffer loadFromMemory(byte[] imageData, int[] width, int[] height, int[] channels, int desiredChannels)
JNIEXPORT jobject JNICALL Java_generaloss_stb_image_StbImage_loadFromMemory
(JNIEnv *env, jclass clazz, jbyteArray dataArray, jintArray width, jintArray height, jintArray channels, jint desiredChannels)
{
    const jsize length = env->GetArrayLength(dataArray);
    jbyte *data = env->GetByteArrayElements(dataArray, nullptr);

    int w, h, c;
    const unsigned char *imgData = stbi_load_from_memory(reinterpret_cast<unsigned char *>(data), length, &w, &h, &c, desiredChannels);

    env->ReleaseByteArrayElements(dataArray, data, JNI_ABORT);
    if(!imgData)
        return nullptr;

    if(width)
        env->SetIntArrayRegion(width, 0, 1, &w);
    if(height)
        env->SetIntArrayRegion(height, 0, 1, &h);
    if(channels)
        env->SetIntArrayRegion(channels, 0, 1, &c);

    return env->NewDirectByteBuffer(data, w * h * (desiredChannels ? desiredChannels : c));
}

// ByteBuffer loadFromCallbacks(StbImageIoCallbacks callbacks, long userData, int[] width, int[] height, int[] channels, int desiredChannels)
JNIEXPORT jobject JNICALL Java_generaloss_stb_image_StbImage_loadFromCallbacks
(JNIEnv *env, jclass clazz, jobject callbackObj, jintArray width, jintArray height, jintArray channels,jint desiredChannels)
{
    stbi_io_callbacks callbacks;
    callbacks.read = stbiRead;
    callbacks.skip = stbiSkip;
    callbacks.eof = stbiEof;

    StbImageIOCallbacksWrapper wrapper = {env, callbackObj};

    int w, h, c;
    unsigned char *imageData = stbi_load_from_callbacks(&callbacks, &wrapper, &w, &h, &c, desiredChannels);
    if(!imageData)
        return nullptr;

    jobject byteBuffer = env->NewDirectByteBuffer(imageData, w * h * (desiredChannels ? desiredChannels : c));

    if(width)
        env->SetIntArrayRegion(width, 0, 1, &w);
    if(height)
        env->SetIntArrayRegion(height, 0, 1, &h);
    if(channels)
        env->SetIntArrayRegion(channels, 0, 1, &c);

    return byteBuffer;
}

// ByteBuffer loadGifFromMemory(byte[] data, int[] delays, int[] width, int[] height, int[] frames, int[] channels, int desiredChannels)
JNIEXPORT jobject JNICALL Java_generaloss_stb_image_StbImage_loadGifFromMemory
(JNIEnv* env, jclass clazz, jbyteArray dataArray, jintArray delaysArray, jintArray width, jintArray height, jintArray frames, jintArray channels, jint desiredChannels)
{
    jbyte* dataPtr = env->GetByteArrayElements(dataArray, JNI_FALSE);
    if(!dataPtr)
        return nullptr;

    const auto* data = reinterpret_cast<const unsigned char *>(dataPtr);
    const jsize length = env->GetArrayLength(dataArray);
    int* delays = nullptr;
    int w, h, z, c;

    unsigned char* result = stbi_load_gif_from_memory(data, length, &delays, &w, &h, &z, &c, desiredChannels);

    env->ReleaseByteArrayElements(dataArray, dataPtr, JNI_ABORT);

    if(!result)
        return nullptr;

    if(delaysArray && delays)
        env->SetIntArrayRegion(delaysArray, 0, z, delays);

    if(width)
        env->SetIntArrayRegion(width, 0, 1, &w);
    if(height)
        env->SetIntArrayRegion(height, 0, 1, &h);
    if(frames)
        env->SetIntArrayRegion(frames, 0, 1, &z);
    if(channels)
        env->SetIntArrayRegion(channels, 0, 1, &c);

    return env->NewDirectByteBuffer(result, w * h * z * (desiredChannels ? desiredChannels : c));
}

// ByteBuffer loadf(String filename, int[] width, int[] height, int[] channels, int desiredChannels)
JNIEXPORT jobject JNICALL Java_generaloss_stb_image_StbImage_loadf
(JNIEnv* env, jclass clazz, jstring filename, jintArray width, jintArray height, jintArray channels, jint desiredChannels)
{
    const char* file = env->GetStringUTFChars(filename, JNI_FALSE);
    int w, h, c;
    float* result = stbi_loadf(file, &w, &h, &c, desiredChannels);
    env->ReleaseStringUTFChars(filename, file);

    if(!result)
        return nullptr;

    if(width)
        env->SetIntArrayRegion(width, 0, 1, &w);
    if(height)
        env->SetIntArrayRegion(height, 0, 1, &h);
    if(channels)
        env->SetIntArrayRegion(channels, 0, 1, &c);

    const jlong size = w * h * (desiredChannels ? desiredChannels : c) * static_cast<jlong>(sizeof(float));
    return env->NewDirectByteBuffer(result, size);
}

// ByteBuffer loadfFromFile(File file, int[] width, int[] height, int[] channels, int reqComp)
JNIEXPORT jobject JNICALL Java_generaloss_stb_image_StbImage_loadfFromFile
(JNIEnv* env, jclass, jobject fileObj, jintArray width, jintArray height, jintArray channels, jint reqComp)
{
    const std::string path = getFilePath(env, fileObj);
    FILE* file = fopen(path.c_str(), "rb");
    if(!file)
        return nullptr;

    int w, h, c;
    float* data = stbi_loadf_from_file(file, &w, &h, &c, reqComp);
    fclose(file);

    if(!data)
        return nullptr;

    if(width)
        env->SetIntArrayRegion(width, 0, 1, &w);
    if(height)
        env->SetIntArrayRegion(height, 0, 1, &h);
    if(channels)
        env->SetIntArrayRegion(channels, 0, 1, &c);

    const size_t bufferSize = w * h * (reqComp ? reqComp : c) * sizeof(float);
    return env->NewDirectByteBuffer(data, static_cast<jlong>(bufferSize));
}

// ByteBuffer loadfFromMemory(byte[] data, int[] width, int[] height, int[] channels, int desiredChannels)
JNIEXPORT jobject JNICALL Java_generaloss_stb_image_StbImage_loadfFromMemory
(JNIEnv* env, jclass clazz, jbyteArray dataArray, jintArray width, jintArray height, jintArray channels, jint desiredChannels)
{
    jbyte* dataPtr = env->GetByteArrayElements(dataArray, JNI_FALSE);
    if(!dataPtr)
        return nullptr;

    const auto* data = reinterpret_cast<const unsigned char *>(dataPtr);
    const jsize length = env->GetArrayLength(dataArray);

    int w, h, c;
    float* result = stbi_loadf_from_memory(data, length, &w, &h, &c, desiredChannels);

    env->ReleaseByteArrayElements(dataArray, dataPtr, JNI_ABORT);

    if(!result)
        return nullptr;

    if(width)
        env->SetIntArrayRegion(width, 0, 1, &w);
    if(height)
        env->SetIntArrayRegion(height, 0, 1, &h);
    if(channels)
        env->SetIntArrayRegion(channels, 0, 1, &c);

    const jlong size = w * h * (desiredChannels ? desiredChannels : c) * static_cast<jlong>(sizeof(float));
    return env->NewDirectByteBuffer(result, size);
}

// ByteBuffer loadFromFile16(File file, int[] width, int[] height, int[] channels, int reqComp)
JNIEXPORT jobject JNICALL Java_generaloss_stb_image_StbImage_loadFromFile16
(JNIEnv* env, jclass, jobject fileObj, jintArray width, jintArray height, jintArray channels, jint reqComp)
{
    const std::string path = getFilePath(env, fileObj);
    FILE* file = fopen(path.c_str(), "rb");
    if(!file)
        return nullptr;

    int w, h, c;
    stbi_us* data = stbi_load_from_file_16(file, &w, &h, &c, reqComp);
    fclose(file);

    if(!data)
        return nullptr;

    if(width)
        env->SetIntArrayRegion(width, 0, 1, &w);
    if(height)
        env->SetIntArrayRegion(height, 0, 1, &h);
    if(channels)
        env->SetIntArrayRegion(channels, 0, 1, &c);

    const size_t bufferSize = w * h * (reqComp ? reqComp : c) * sizeof(stbi_us);
    return env->NewDirectByteBuffer(data, static_cast<jlong>(bufferSize));
}

// ByteBuffer loadfFromCallbacks(StbImageIoCallbacks callbacks, int[] width, int[] height, int[] channels, int desiredChannels);
JNIEXPORT jobject JNICALL Java_generaloss_stb_image_StbImage_loadfFromCallbacks
(JNIEnv* env, jclass, jobject callbackObj, jintArray width, jintArray height, jintArray channels, jint reqComp)
{
    stbi_io_callbacks callbacks;
    callbacks.read = stbiRead;
    callbacks.skip = stbiSkip;
    callbacks.eof  = stbiEof;

    StbImageIOCallbacksWrapper wrapper = { env, callbackObj };

    int w, h, c;
    float* data = stbi_loadf_from_callbacks(&callbacks, &wrapper, &w, &h, &c, reqComp);
    if(!data)
        return nullptr;

    if(width)
        env->SetIntArrayRegion(width, 0, 1, &w);
    if(height)
        env->SetIntArrayRegion(height, 0, 1, &h);
    if(channels)
        env->SetIntArrayRegion(channels, 0, 1, &c);

    const size_t size = w * h * (reqComp ? reqComp : c) * sizeof(float);
    return env->NewDirectByteBuffer(data, static_cast<jlong>(size));
}

// ByteBuffer load16(String filename, int[] width, int[] height, int[] channels, int desiredChannels)
JNIEXPORT jobject JNICALL Java_generaloss_stb_image_StbImage_load16
(JNIEnv* env, jclass clazz, jstring filename, jintArray width, jintArray height, jintArray channels, jint desiredChannels)
{
    const char* file = env->GetStringUTFChars(filename, JNI_FALSE);
    int w, h, c;
    unsigned short* result = stbi_load_16(file, &w, &h, &c, desiredChannels);
    env->ReleaseStringUTFChars(filename, file);

    if(!result)
        return nullptr;

    if(width)
        env->SetIntArrayRegion(width, 0, 1, &w);
    if(height)
        env->SetIntArrayRegion(height, 0, 1, &h);
    if(channels)
        env->SetIntArrayRegion(channels, 0, 1, &c);

    const jlong size = w * h * (desiredChannels ? desiredChannels : c) * static_cast<jlong>(sizeof(unsigned short));
    return env->NewDirectByteBuffer(result, size);
}

// ByteBuffer load16FromCallbacks(StbImageIoCallbacks callbacks, int[] width, int[] height, int[] channels, int desiredChannels);
JNIEXPORT jobject JNICALL Java_generaloss_stb_image_StbImage_load16FromCallbacks
(JNIEnv* env, jclass, jobject callbackObj, jintArray width, jintArray height, jintArray channels, jint desiredChannels)
{
    stbi_io_callbacks callbacks;
    callbacks.read = stbiRead;
    callbacks.skip = stbiSkip;
    callbacks.eof  = stbiEof;

    StbImageIOCallbacksWrapper wrapper = {env, callbackObj};

    int w, h, c;
    stbi_us* data = stbi_load_16_from_callbacks(&callbacks, &wrapper, &w, &h, &c, desiredChannels);
    if(!data)
        return nullptr;

    if(width)
        env->SetIntArrayRegion(width, 0, 1, &w);
    if(height)
        env->SetIntArrayRegion(height, 0, 1, &h);
    if(channels)
        env->SetIntArrayRegion(channels, 0, 1, &c);

    const size_t size = w * h * (desiredChannels ? desiredChannels : c) * sizeof(stbi_us);
    return env->NewDirectByteBuffer(data, static_cast<jlong>(size));
}

// ByteBuffer load16FromMemory(byte[] data, int[] widht, int[] height, int[] channels, int desiredChannels)
JNIEXPORT jobject JNICALL Java_generaloss_stb_image_StbImage_load16FromMemory
(JNIEnv* env, jclass clazz, jbyteArray dataArray, jintArray width, jintArray height, jintArray channels, jint desiredChannels)
{
    jbyte* dataPtr = env->GetByteArrayElements(dataArray, JNI_FALSE);
    if(!dataPtr)
        return nullptr;

    const auto* data = reinterpret_cast<const unsigned char *>(dataPtr);
    const jsize length = env->GetArrayLength(dataArray);

    int w, h, c;
    unsigned short* result = stbi_load_16_from_memory(data, length, &w, &h, &c, desiredChannels);

    env->ReleaseByteArrayElements(dataArray, dataPtr, JNI_ABORT);

    if(!result)
        return nullptr;

    if(width)
        env->SetIntArrayRegion(width, 0, 1, &w);
    if(height)
        env->SetIntArrayRegion(height, 0, 1, &h);
    if(channels)
        env->SetIntArrayRegion(channels, 0, 1, &c);

    const jlong size = w * h * (desiredChannels ? desiredChannels : c) * static_cast<jlong>(sizeof(unsigned short));
    return env->NewDirectByteBuffer(result, size);
}


// void imageFree(ByteBuffer imageBuffer)
JNIEXPORT void JNICALL Java_generaloss_stb_image_StbImage_imageFree
(JNIEnv *env, jclass clazz, jobject imageBuffer)
{
    void *data = env->GetDirectBufferAddress(imageBuffer);
    if(data)
        stbi_image_free(data);
}


// void hdrToLdrGamma(float gamma)
JNIEXPORT void JNICALL Java_generaloss_stb_image_StbImage_hdrToLdrGamma(JNIEnv* env, jclass, jfloat gamma) {
    stbi_hdr_to_ldr_gamma(gamma);
}

// void hdrToLdrScale(float scale)
JNIEXPORT void JNICALL Java_generaloss_stb_image_StbImage_hdrToLdrScale(JNIEnv* env, jclass, jfloat scale) {
    stbi_hdr_to_ldr_scale(scale);
}

// void ldrToHdrGamma(float gamma)
JNIEXPORT void JNICALL Java_generaloss_stb_image_StbImage_ldrToHdrGamma(JNIEnv* env, jclass, jfloat gamma) {
    stbi_ldr_to_hdr_gamma(gamma);
}

// void ldrToHdrScale(float scale)
JNIEXPORT void JNICALL Java_generaloss_stb_image_StbImage_ldrToHdrScale(JNIEnv* env, jclass, jfloat scale) {
    stbi_ldr_to_hdr_scale(scale);
}

// void convertIphonePngToRgb(boolean shouldConvert)
JNIEXPORT void JNICALL Java_generaloss_stb_image_StbImage_convertIphonePngToRgb(JNIEnv* env, jclass, jboolean shouldConvert) {
    stbi_convert_iphone_png_to_rgb(shouldConvert ? 1 : 0);
}

// void convertIphonePngToRgbThread(boolean shouldConvert)
JNIEXPORT void JNICALL Java_generaloss_stb_image_StbImage_convertIphonePngToRgbThread(JNIEnv* env, jclass, jboolean shouldConvert) {
    stbi_convert_iphone_png_to_rgb_thread(shouldConvert ? 1 : 0);
}


// boolean is16Bit(String filename)
JNIEXPORT jboolean JNICALL Java_generaloss_stb_image_StbImage_is16Bit
(JNIEnv* env, jclass, jstring filename)
{
    const char* file = env->GetStringUTFChars(filename, nullptr);
    int result = stbi_is_16_bit(file);
    env->ReleaseStringUTFChars(filename, file);
    return result ? JNI_TRUE : JNI_FALSE;
}

// boolean is16BitFromFile(File file)
JNIEXPORT jboolean JNICALL Java_generaloss_stb_image_StbImage_is16BitFromFile
(JNIEnv* env, jclass, jobject fileObj)
{
    const std::string path = getFilePath(env, fileObj);
    FILE* file = fopen(path.c_str(), "rb");
    if(!file)
        return JNI_FALSE;

    return stbi_is_16_bit_from_file(file) ? JNI_TRUE : JNI_FALSE;
}

// boolean is16BitFromMemory(byte[] data, int length)
JNIEXPORT jboolean JNICALL Java_generaloss_stb_image_StbImage_is16BitFromMemory
(JNIEnv* env, jclass, jbyteArray dataArray)
{
    const jsize len = env->GetArrayLength(dataArray);
    jbyte* buffer = env->GetByteArrayElements(dataArray, nullptr);
    const int result = stbi_is_16_bit_from_memory(reinterpret_cast<stbi_uc*>(buffer), len);
    env->ReleaseByteArrayElements(dataArray, buffer, JNI_ABORT);
    return result ? JNI_TRUE : JNI_FALSE;
}

// boolean is16BitFromCallbacks(StbImageIOCallbacks callbacks)
JNIEXPORT jboolean JNICALL Java_generaloss_stb_image_StbImage_is16BitFromCallbacks
(JNIEnv* env, jclass, jobject callbackObj)
{
    stbi_io_callbacks callbacks;
    callbacks.read = stbiRead;
    callbacks.skip = stbiSkip;
    callbacks.eof  = stbiEof;

    StbImageIOCallbacksWrapper wrapper = {env, callbackObj};
    return stbi_is_16_bit_from_callbacks(&callbacks, &wrapper) ? JNI_TRUE : JNI_FALSE;
}

// boolean isHdr(String filename)
JNIEXPORT jboolean JNICALL Java_generaloss_stb_image_StbImage_isHdr
(JNIEnv* env, jclass, jstring filename)
{
    const char* file = env->GetStringUTFChars(filename, nullptr);
    const int result = stbi_is_hdr(file);
    env->ReleaseStringUTFChars(filename, file);
    return result ? JNI_TRUE : JNI_FALSE;
}

// boolean isHdrFromFile(File file)
JNIEXPORT jboolean JNICALL Java_generaloss_stb_image_StbImage_isHdrFromFile(JNIEnv* env, jclass, jlong filePtr) {
    return stbi_is_hdr_from_file(reinterpret_cast<FILE*>(filePtr)) ? JNI_TRUE : JNI_FALSE;
}

// boolean isHdrFromMemory(byte[] data, int length)
JNIEXPORT jboolean JNICALL Java_generaloss_stb_image_StbImage_isHdrFromMemory
(JNIEnv* env, jclass, jbyteArray dataArray)
{
    const jsize len = env->GetArrayLength(dataArray);
    jbyte* buffer = env->GetByteArrayElements(dataArray, nullptr);
    const int result = stbi_is_hdr_from_memory(reinterpret_cast<stbi_uc*>(buffer), len);
    env->ReleaseByteArrayElements(dataArray, buffer, JNI_ABORT);
    return result ? JNI_TRUE : JNI_FALSE;
}

// boolean isHdrFromCallbacks(StbImageIOCallbacks callbacks)
JNIEXPORT jboolean JNICALL Java_generaloss_stb_image_StbImage_isHdrFromCallbacks
(JNIEnv* env, jclass, jobject callbackObj)
{
    stbi_io_callbacks callbacks;
    callbacks.read = stbiRead;
    callbacks.skip = stbiSkip;
    callbacks.eof  = stbiEof;

    StbImageIOCallbacksWrapper wrapper = {env, callbackObj};
    return stbi_is_hdr_from_callbacks(&callbacks, &wrapper) ? JNI_TRUE : JNI_FALSE;
}


// int zlibDecodeBuffer(byte[] output, byte[] input)
JNIEXPORT jint JNICALL Java_generaloss_stb_image_StbImage_zlibDecodeBuffer
(JNIEnv* env, jclass, jbyteArray outputArray, jbyteArray inputArray)
{
    const jsize olen = env->GetArrayLength(outputArray);
    const jsize ilen = env->GetArrayLength(inputArray);

    jbyte* obuffer = env->GetByteArrayElements(outputArray, nullptr);
    jbyte* ibuffer = env->GetByteArrayElements(inputArray, nullptr);

    const int result = stbi_zlib_decode_buffer(reinterpret_cast<char*>(obuffer), olen, reinterpret_cast<char*>(ibuffer), ilen);

    env->ReleaseByteArrayElements(outputArray, obuffer, 0);
    env->ReleaseByteArrayElements(inputArray, ibuffer, JNI_ABORT);

    return result;
}

// byte[] zlibDecodeMalloc(byte[] input)
JNIEXPORT jbyteArray JNICALL Java_generaloss_stb_image_StbImage_zlibDecodeMalloc
(JNIEnv* env, jclass, jbyteArray inputArray)
{
    const jsize len = env->GetArrayLength(inputArray);
    jbyte* buffer = env->GetByteArrayElements(inputArray, nullptr);

    int outlen;
    char* result = stbi_zlib_decode_malloc(reinterpret_cast<char*>(buffer), len, &outlen);

    env->ReleaseByteArrayElements(inputArray, buffer, JNI_ABORT);

    if(!result || outlen <= 0)
        return nullptr;

    jbyteArray output = env->NewByteArray(outlen);
    env->SetByteArrayRegion(output, 0, outlen, reinterpret_cast<jbyte*>(result));
    STBI_FREE(result);
    return output;
}

// byte[] zlibDecodeMallocGuesssize(byte[] input, int initialSize)
JNIEXPORT jbyteArray JNICALL Java_generaloss_stb_image_StbImage_zlibDecodeMallocGuesssize
(JNIEnv* env, jclass, jbyteArray inputArray, jint initialSize)
{
    const jsize len = env->GetArrayLength(inputArray);
    jbyte* buffer = env->GetByteArrayElements(inputArray, nullptr);

    int outlen;
    char* result = stbi_zlib_decode_malloc_guesssize(reinterpret_cast<char*>(buffer), len, initialSize, &outlen);

    env->ReleaseByteArrayElements(inputArray, buffer, JNI_ABORT);

    if(!result || outlen <= 0)
        return nullptr;

    jbyteArray output = env->NewByteArray(outlen);
    env->SetByteArrayRegion(output, 0, outlen, reinterpret_cast<jbyte*>(result));
    STBI_FREE(result);
    return output;
}

// byte[] zlibDecodeMallocGuesssizeHeaderflag(byte[] input, int initialSize, boolean parseHeader)
JNIEXPORT jbyteArray JNICALL Java_generaloss_stb_image_StbImage_zlibDecodeMallocGuesssizeHeaderflag
(JNIEnv* env, jclass, jbyteArray inputArray, jint initialSize, jboolean parseHeader)
{
    const jsize len = env->GetArrayLength(inputArray);
    jbyte* buffer = env->GetByteArrayElements(inputArray, nullptr);

    int outlen;
    char* result = stbi_zlib_decode_malloc_guesssize_headerflag(reinterpret_cast<char*>(buffer), len, initialSize, &outlen, parseHeader);

    env->ReleaseByteArrayElements(inputArray, buffer, JNI_ABORT);

    if(!result || outlen <= 0)
        return nullptr;

    jbyteArray output = env->NewByteArray(outlen);
    env->SetByteArrayRegion(output, 0, outlen, reinterpret_cast<jbyte*>(result));
    STBI_FREE(result);
    return output;
}

// int zlibDecodeNoheaderBuffer(byte[] output, byte[] input)
JNIEXPORT jint JNICALL Java_generaloss_stb_image_StbImage_zlibDecodeNoheaderBuffer
(JNIEnv* env, jclass, jbyteArray outputArray, jbyteArray inputArray)
{
    const jsize olen = env->GetArrayLength(outputArray);
    const jsize ilen = env->GetArrayLength(inputArray);

    jbyte* obuffer = env->GetByteArrayElements(outputArray, nullptr);
    jbyte* ibuffer = env->GetByteArrayElements(inputArray, nullptr);

    const int result = stbi_zlib_decode_noheader_buffer(reinterpret_cast<char*>(obuffer), olen, reinterpret_cast<char*>(ibuffer), ilen);

    env->ReleaseByteArrayElements(outputArray, obuffer, 0);
    env->ReleaseByteArrayElements(inputArray, ibuffer, JNI_ABORT);

    return result;
}

// byte[] zlibDecodeNoheaderMalloc(byte[] input)
JNIEXPORT jbyteArray JNICALL Java_generaloss_stb_image_StbImage_zlibDecodeNoheaderMalloc
(JNIEnv* env, jclass, jbyteArray inputArray)
{
    const jsize len = env->GetArrayLength(inputArray);
    jbyte* buffer = env->GetByteArrayElements(inputArray, nullptr);

    int outlen;
    char* result = stbi_zlib_decode_noheader_malloc(reinterpret_cast<char*>(buffer), len, &outlen);

    env->ReleaseByteArrayElements(inputArray, buffer, JNI_ABORT);

    if(!result || outlen <= 0)
        return nullptr;

    jbyteArray output = env->NewByteArray(outlen);
    env->SetByteArrayRegion(output, 0, outlen, reinterpret_cast<jbyte*>(result));
    STBI_FREE(result);
    return output;
}