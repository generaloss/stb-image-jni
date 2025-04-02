#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "StbImage.h"

JNIEXPORT jobject JNICALL Java_generaloss_stb_image_StbImage_load
(JNIEnv* env, jclass clazz, jstring filename, jintArray width, jintArray height, jintArray channels,
 jint desiredChannels)
{
    const char* cFilename = env->GetStringUTFChars(filename, 0);

    int w, h, c;
    unsigned char* data = stbi_load(cFilename, &w, &h, &c, desiredChannels);

    env->ReleaseStringUTFChars(filename, cFilename);
    if(!data)
        return nullptr;

    env->SetIntArrayRegion(width, 0, 1, &w);
    env->SetIntArrayRegion(height, 0, 1, &h);
    env->SetIntArrayRegion(channels, 0, 1, &c);

    return env->NewDirectByteBuffer(data, w * h * (desiredChannels ? desiredChannels : c));
}

JNIEXPORT jobject JNICALL Java_generaloss_stb_image_StbImage_loadFromMemory
(JNIEnv* env, jclass clazz, jbyteArray imageData, jintArray width, jintArray height, jintArray channels, jint desiredChannels)
{
    jsize length = env->GetArrayLength(imageData);
    jbyte* data = env->GetByteArrayElements(imageData, nullptr);

    int w, h, c;
    unsigned char* imgData = stbi_load_from_memory(reinterpret_cast<unsigned char*>(data), length, &w, &h, &c, desiredChannels);

    env->ReleaseByteArrayElements(imageData, data, JNI_ABORT);
    if(!imgData)
        return nullptr;

    env->SetIntArrayRegion(width, 0, 1, &w);
    env->SetIntArrayRegion(height, 0, 1, &h);
    env->SetIntArrayRegion(channels, 0, 1, &c);

    return env->NewDirectByteBuffer(data, w * h * (desiredChannels ? desiredChannels : c));
}

JNIEXPORT void JNICALL Java_generaloss_stb_image_StbImage_imageFree
(JNIEnv* env, jclass clazz, jobject imageBuffer)
{
    void* data = env->GetDirectBufferAddress(imageBuffer);
    if(data)
        stbi_image_free(data);
}

JNIEXPORT jboolean JNICALL Java_generaloss_stb_image_StbImage_info
(JNIEnv* env, jclass clazz, jstring filename, jintArray width, jintArray height, jintArray channels)
{
    const char* cFilename = env->GetStringUTFChars(filename, 0);

    int w, h, c;
    int result = stbi_info(cFilename, &w, &h, &c);

    env->ReleaseStringUTFChars(filename, cFilename);

    if(result) {
        env->SetIntArrayRegion(width, 0, 1, &w);
        env->SetIntArrayRegion(height, 0, 1, &h);
        env->SetIntArrayRegion(channels, 0, 1, &c);
    }

    return result;
}

JNIEXPORT void JNICALL Java_generaloss_stb_image_StbImage_setFlipVerticallyOnLoad
(JNIEnv* env, jclass clazz, jboolean flip)
{
    stbi_set_flip_vertically_on_load(flip);
}
