#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "StbImage.h"

// // const char stbi_failure_reason()
//
// // int stbi_info(char const* filename, int *x, int *y, int *comp)
// int stbi_info_from_callbacks(stbi_io_callbacks const *c, void *user, int *x, int *y, int *comp)
// int stbi_info_from_file(FILE *f, int *x, int *y, int *comp)
// // int stbi_info_from_memory(stbi_uc const *buffer, int len, int *x, int *y, int *comp)
//
// // void stbi_set_flip_vertically_on_load(int flag_true_if_should_flip)
// void stbi_set_flip_vertically_on_load_thread(int flag_true_if_should_flip)
// void stbi_set_unpremultiply_on_load(int flag_true_if_should_unpremultiply)
// void stbi_set_unpremultiply_on_load_thread(int flag_true_if_should_unpremultiply)
//
// // stbi_uc *stbi_load(char const *filename, int *x, int *y, int *comp, int req_comp)
// stbi_uc *stbi_load_from_file(FILE *f, int *x, int *y, int *comp, int req_comp)
// // stbi_uc *stbi_load_from_memory(stbi_uc const *buffer, int len, int *x, int *y, int *comp, int req_comp)
// stbi_uc *stbi_load_from_callbacks(stbi_io_callbacks const *clbk, void *user, int *x, int *y, int *comp, int req_comp)
// stbi_uc *stbi_load_gif_from_memory(stbi_uc const *buffer, int len, int **delays, int *x, int *y, int *z, int *comp, int req_comp)
// float *stbi_loadf(char const *filename, int *x, int *y, int *comp, int req_comp)
// float *stbi_loadf_from_file(FILE *f, int *x, int *y, int *comp, int req_comp)
// float *stbi_loadf_from_memory(stbi_uc const *buffer, int len, int *x, int *y, int *comp, int req_comp)
// float *stbi_loadf_from_callbacks(stbi_io_callbacks const *clbk, void *user, int *x, int *y, int *comp, int req_comp)
// stbi_us *stbi_load_16(char const *filename, int *x, int *y, int *comp, int req_comp)
// stbi__uint16 *stbi_load_from_file_16(FILE *f, int *x, int *y, int *comp, int req_comp)
// stbi_us *stbi_load_16_from_memory(stbi_uc const *buffer, int len, int *x, int *y, int *channels_in_file, int desired_channels)
// stbi_us *stbi_load_16_from_callbacks(stbi_io_callbacks const *clbk, void *user, int *x, int *y, int *channels_in_file, int desired_channels)
//
// // void stbi_image_free(void* retval_from_stbi_load)
//
// void stbi_hdr_to_ldr_gamma(float gamma)
// void stbi_hdr_to_ldr_scale(float scale)
// void stbi_ldr_to_hdr_gamma(float gamma)
// void stbi_ldr_to_hdr_scale(float scale)
// void stbi_convert_iphone_png_to_rgb(int flag_true_if_should_convert);
// void stbi_convert_iphone_png_to_rgb_thread(int flag_true_if_should_convert);
//
// int stbi_is_16_bit(char const *filename)
// int stbi_is_16_bit_from_file(FILE *f)
// int stbi_is_16_bit_from_memory(stbi_uc const *buffer, int len)
// int stbi_is_16_bit_from_callbacks(stbi_io_callbacks const *c, void *user)
// int stbi_is_hdr(char const *filename)
// int stbi_is_hdr_from_file(FILE *f)
// int stbi_is_hdr_from_memory(stbi_uc const *buffer, int len)
// int stbi_is_hdr_from_callbacks(stbi_io_callbacks const *clbk, void *user)
//
// int stbi_zlib_decode_buffer(char *obuffer, int olen, char const *ibuffer, int ilen)
// char *stbi_zlib_decode_malloc(char const *buffer, int len, int *outlen)
// char *stbi_zlib_decode_malloc_guesssize(const char *buffer, int len, int initial_size, int *outlen)
// char *stbi_zlib_decode_malloc_guesssize_headerflag(const char *buffer, int len, int initial_size, int *outlen, int parse_header)
// int stbi_zlib_decode_noheader_buffer(char *obuffer, int olen, const char *ibuffer, int ilen)
// char *stbi_zlib_decode_noheader_malloc(char const *buffer, int len, int *outlen)

JNIEXPORT jstring JNICALL Java_generaloss_stb_image_StbImage_failureReason
  (JNIEnv* env, jclass clazz) {
    const char* reason = stbi_failure_reason();
    if (reason == NULL) {
        return env->NewStringUTF("Unknown error");
    }
    return env->NewStringUTF(reason);
}

JNIEXPORT jboolean JNICALL Java_generaloss_stb_image_StbImage_info
(JNIEnv* env, jclass clazz, jstring filename, jintArray width, jintArray height, jintArray channels)
{
    const char* cFilename = env->GetStringUTFChars(filename, nullptr);

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

JNIEXPORT jboolean JNICALL Java_generaloss_stb_image_StbImage_infoFromMemory
  (JNIEnv* env, jclass clazz, jobject buffer, jint length, jintArray width, jintArray height, jintArray channels) {
    const unsigned char* data = (const unsigned char*) env->GetDirectBufferAddress(buffer);
    if (!data) return JNI_FALSE;

    int w, h, c;
    jboolean result = stbi_info_from_memory(data, length, &w, &h, &c) ? JNI_TRUE : JNI_FALSE;

    env->SetIntArrayRegion(width, 0, 1, &w);
    env->SetIntArrayRegion(height, 0, 1, &h);
    env->SetIntArrayRegion(channels, 0, 1, &c);

    return result;
}

JNIEXPORT void JNICALL Java_generaloss_stb_image_StbImage_setFlipVerticallyOnLoad
(JNIEnv* env, jclass clazz, jboolean flip)
{
    stbi_set_flip_vertically_on_load(flip);
}

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
