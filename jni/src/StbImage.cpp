#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "StbImage.h"
#include <cstdio>
#include <string>

struct StbImageIoCallbacksWrapper {
    JNIEnv *env;
    jobject callbackObj;
};

int stbiRead(void *user, char *data, int size) {
    const auto *wrapper = static_cast<StbImageIoCallbacksWrapper *>(user);
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
    const auto *wrapper = static_cast<StbImageIoCallbacksWrapper *>(user);
    JNIEnv *env = wrapper->env;

    jclass cls = env->GetObjectClass(wrapper->callbackObj);
    jmethodID skipMethod = env->GetMethodID(cls, "skip", "(I)V");

    env->CallVoidMethod(wrapper->callbackObj, skipMethod, n);
}

int stbiEof(void *user) {
    const auto *wrapper = static_cast<StbImageIoCallbacksWrapper *>(user);
    JNIEnv *env = wrapper->env;

    jclass cls = env->GetObjectClass(wrapper->callbackObj);
    jmethodID eofMethod = env->GetMethodID(cls, "eof", "()I");

    return env->CallIntMethod(wrapper->callbackObj, eofMethod);
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

// boolean infoFromMemory(ByteBuffer buffer, int length, int[] width, int[] height, int[] channels)
JNIEXPORT jboolean JNICALL Java_generaloss_stb_image_StbImage_infoFromMemory
(JNIEnv *env, jclass clazz, jobject buffer, jint length, jintArray width, jintArray height, jintArray channels)
{
    const auto *data = static_cast<const unsigned char *>(env->GetDirectBufferAddress(buffer));
    if(!data)
        return JNI_FALSE;

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

    StbImageIoCallbacksWrapper wrapper = {env, callbackObj};

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
(JNIEnv *env, jclass clazz, jbyteArray imageData, jintArray width, jintArray height, jintArray channels, jint desiredChannels)
{
    const jsize length = env->GetArrayLength(imageData);
    jbyte *data = env->GetByteArrayElements(imageData, nullptr);

    int w, h, c;
    const unsigned char *imgData = stbi_load_from_memory(reinterpret_cast<unsigned char *>(data), length, &w, &h, &c, desiredChannels);

    env->ReleaseByteArrayElements(imageData, data, JNI_ABORT);
    if(!imgData)
        return nullptr;

    env->SetIntArrayRegion(width, 0, 1, &w);
    env->SetIntArrayRegion(height, 0, 1, &h);
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

    StbImageIoCallbacksWrapper wrapper = {env, callbackObj};

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

    StbImageIoCallbacksWrapper wrapper = { env, callbackObj };

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

// ByteBuffer loadFromCallbacks16(StbImageIoCallbacks callbacks, int[] width, int[] height, int[] channels, int desiredChannels);
JNIEXPORT jobject JNICALL Java_generaloss_stb_image_StbImage_loadFromCallbacks16
(JNIEnv* env, jclass, jobject callbackObj, jintArray width, jintArray height, jintArray channels, jint desiredChannels)
{
    stbi_io_callbacks callbacks;
    callbacks.read = stbiRead;
    callbacks.skip = stbiSkip;
    callbacks.eof  = stbiEof;

    StbImageIoCallbacksWrapper wrapper = {env, callbackObj};

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


// void imageFree(ByteBuffer imageBuffer)
JNIEXPORT void JNICALL Java_generaloss_stb_image_StbImage_imageFree
(JNIEnv *env, jclass clazz, jobject imageBuffer)
{
    void *data = env->GetDirectBufferAddress(imageBuffer);
    if(data)
        stbi_image_free(data);
}
