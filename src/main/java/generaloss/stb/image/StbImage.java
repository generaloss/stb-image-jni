package generaloss.stb.image;

import jpize.util.NativeLib;
import jpize.util.res.Resource;

import java.io.File;
import java.nio.ByteBuffer;

public class StbImage {

    private static void loadLibrary() {
        final String os = System.getProperty("os.name").toLowerCase();
        final String architecture = System.getProperty("os.arch").toLowerCase();
        final String filename = NativeLib.getFilename("stb_image_jni");
        NativeLib.load(Resource.internal("/lib/stb_image/" + os + "-" + architecture + "/" + filename));
    }

    static {
        loadLibrary();
    }

    public static native String failureReason();


    public static native boolean info(String filename, int[] width, int[] height, int[] channels);

    public static native boolean infoFromFile(long filePtr, int[] width, int[] height, int[] channels);

    public static native boolean infoFromMemory(ByteBuffer buffer, int length, int[] width, int[] height, int[] channels);

    public static native boolean infoFromCallbacks(StbImageIoCallbacks callbacks, long userData, int[] width, int[] height, int[] channels);


    public static native void setFlipVerticallyOnLoad(boolean flip);

    public static native void setFlipVerticallyOnLoadThread(boolean shouldFlip);

    public static native void setUnpremultiplyOnLoad(boolean shouldUnpremultiply);

    public static native void setUnpremultiplyOnLoadThread(boolean shouldUnpremultiply);


    public static native ByteBuffer load(String filename, int[] width, int[] height, int[] channels, int desiredChannels);

    public static native ByteBuffer loadFromFile(File file, int[] x, int[] y, int[] comp, int reqComp);

    public static native ByteBuffer loadFromMemory(byte[] imageData, int[] width, int[] height, int[] channels, int desiredChannels);

    public static native ByteBuffer loadFromCallbacks(StbImageIoCallbacks callbacks, long userData, int[] width, int[] height, int[] channels, int desiredChannels);

    public static native ByteBuffer loadfFromFile(File file, int[] x, int[] y, int[] comp, int reqComp);

    public static native ByteBuffer loadFromFile16(File file, int[] x, int[] y, int[] comp, int reqComp);


    public static native void imageFree(ByteBuffer imageBuffer);


    // ## const char stbi_failure_reason()
    //
    // ## int stbi_info(char const* filename, int *x, int *y, int *comp)
    // ## int stbi_info_from_file(FILE *f, int *x, int *y, int *comp)
    // ## int stbi_info_from_memory(stbi_uc const *buffer, int len, int *x, int *y, int *comp)
    // ## int stbi_info_from_callbacks(stbi_io_callbacks const *c, void *user, int *x, int *y, int *comp)
    //
    // ## void stbi_set_flip_vertically_on_load(int flag_true_if_should_flip)
    // ## void stbi_set_flip_vertically_on_load_thread(int flag_true_if_should_flip)
    // ## void stbi_set_unpremultiply_on_load(int flag_true_if_should_unpremultiply)
    // ## void stbi_set_unpremultiply_on_load_thread(int flag_true_if_should_unpremultiply)
    //
    // ## stbi_uc *stbi_load(char const *filename, int *x, int *y, int *comp, int req_comp)
    // ## stbi_uc *stbi_load_from_file(FILE *f, int *x, int *y, int *comp, int req_comp)
    // ## stbi_uc *stbi_load_from_memory(stbi_uc const *buffer, int len, int *x, int *y, int *comp, int req_comp)
    // ## stbi_uc *stbi_load_from_callbacks(stbi_io_callbacks const *clbk, void *user, int *x, int *y, int *comp, int req_comp)
    // stbi_uc *stbi_load_gif_from_memory(stbi_uc const *buffer, int len, int **delays, int *x, int *y, int *z, int *comp, int req_comp)
    // float *stbi_loadf(char const *filename, int *x, int *y, int *comp, int req_comp)
    // ## float *stbi_loadf_from_file(FILE *f, int *x, int *y, int *comp, int req_comp)
    // float *stbi_loadf_from_memory(stbi_uc const *buffer, int len, int *x, int *y, int *comp, int req_comp)
    // # float *stbi_loadf_from_callbacks(stbi_io_callbacks const *clbk, void *user, int *x, int *y, int *comp, int req_comp)
    // stbi_us *stbi_load_16(char const *filename, int *x, int *y, int *comp, int req_comp)
    // ## stbi__uint16 *stbi_load_from_file_16(FILE *f, int *x, int *y, int *comp, int req_comp)
    // stbi_us *stbi_load_16_from_memory(stbi_uc const *buffer, int len, int *x, int *y, int *channels_in_file, int desired_channels)
    // # stbi_us *stbi_load_16_from_callbacks(stbi_io_callbacks const *clbk, void *user, int *x, int *y, int *channels_in_file, int desired_channels)
    //
    // ## void stbi_image_free(void* retval_from_stbi_load)
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

}
