package generaloss.stb.image;

import jpize.util.NativeLib;
import jpize.util.res.Resource;

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

    public static native ByteBuffer load(String filename, int[] width, int[] height, int[] channels, int desiredChannels);

    public static native ByteBuffer loadFromMemory(byte[] imageData, int[] width, int[] height, int[] channels, int desiredChannels);

    public static native void imageFree(ByteBuffer imageBuffer);

    public static native boolean info(String filename, int[] width, int[] height, int[] channels);

    public static native void setFlipVerticallyOnLoad(boolean flip);

}
