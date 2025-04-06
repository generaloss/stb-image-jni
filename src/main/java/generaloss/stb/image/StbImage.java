package generaloss.stb.image;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardCopyOption;

public class StbImage {

    private static final String LIB_NAME = "stb_image_jni";

    static {
        load();
    }

    public static void load() {
        if (isAndroid()) {
            loadAndroid();
        } else {
            loadDesktop();
        }
    }

    private static void loadAndroid() {
        System.loadLibrary(LIB_NAME);
    }

    private static void loadDesktop() {
        String os = getOS();
        String arch = getArch();
        String libPath = "/natives/" + os + "/" + getLibFileName(os);

        try (InputStream in = StbImage.class.getResourceAsStream(libPath)) {
            if (in == null) {
                throw new UnsatisfiedLinkError("Native library not found: " + libPath);
            }

            Path tempDir = Files.createTempDirectory("natives");
            Path tempFile = tempDir.resolve(getLibFileName(os));
            Files.copy(in, tempFile, StandardCopyOption.REPLACE_EXISTING);
            tempFile.toFile().deleteOnExit();

            System.load(tempFile.toAbsolutePath().toString());
        } catch (IOException e) {
            throw new RuntimeException("Failed to load native library", e);
        }
    }

    private static String getLibFileName(String os) {
        if (os.equals("windows")) {
            return LIB_NAME + ".dll";
        } else if (os.equals("macos")) {
            return "lib" + LIB_NAME + ".dylib";
        }
        return "lib" + LIB_NAME + ".so";
    }

    private static String getOS() {
        String os = System.getProperty("os.name").toLowerCase();
        if (os.contains("win")) return "windows";
        if (os.contains("mac")) return "macos";
        if (os.contains("nix") || os.contains("nux")) return "linux";
        throw new UnsupportedOperationException("Unsupported OS: " + os);
    }

    private static String getArch() {
        String arch = System.getProperty("os.arch").toLowerCase();
        if (arch.contains("64")) return "x86_64";
        return "x86";
    }

    private static boolean isAndroid() {
        return System.getProperty("java.runtime.name", "").toLowerCase().contains("android");
    }


    public static native String failureReason();


    public static native boolean info(String filename, int[] width, int[] height, int[] channels);

    public static native boolean infoFromFile(long filePtr, int[] width, int[] height, int[] channels);

    public static native boolean infoFromMemory(byte[] data, int[] width, int[] height, int[] channels);

    public static native boolean infoFromCallbacks(StbImageIOCallbacks callbacks, int[] width, int[] height, int[] channels);


    public static native void setFlipVerticallyOnLoad(boolean flip);

    public static native void setFlipVerticallyOnLoadThread(boolean shouldFlip);

    public static native void setUnpremultiplyOnLoad(boolean shouldUnpremultiply);

    public static native void setUnpremultiplyOnLoadThread(boolean shouldUnpremultiply);


    public static native ByteBuffer load(String filename, int[] width, int[] height, int[] channels, int desiredChannels);

    public static native ByteBuffer loadFromFile(File file, int[] width, int[] height, int[] channels, int desiredChannels);

    public static native ByteBuffer loadFromMemory(byte[] data, int[] width, int[] height, int[] channels, int desiredChannels);

    public static native ByteBuffer loadFromCallbacks(StbImageIOCallbacks callbacks, int[] width, int[] height, int[] channels, int desiredChannels);

    public static native ByteBuffer loadGifFromMemory(byte[] data, int[] delays, int[] width, int[] height, int[] frames, int[] channels, int desiredChannels);

    public static native ByteBuffer loadf(String filename, int[] width, int[] height, int[] channels, int desiredChannels);

    public static native ByteBuffer loadfFromFile(File file, int[] width, int[] height, int[] channels, int desiredChannels);

    public static native ByteBuffer loadfFromMemory(byte[] data, int[] width, int[] height, int[] channels, int desiredChannels);

    public static native ByteBuffer loadfFromCallbacks(StbImageIOCallbacks callbacks, int[] width, int[] height, int[] channels, int desiredChannels);

    public static native ByteBuffer load16(String filename, int[] width, int[] height, int[] channels, int desiredChannels);

    public static native ByteBuffer loadFromFile16(File file, int[] width, int[] height, int[] channels, int desiredChannels);

    public static native ByteBuffer load16FromMemory(byte[] data, int[] widht, int[] height, int[] channels, int desiredChannels);

    public static native ByteBuffer load16FromCallbacks(StbImageIOCallbacks callbacks, int[] width, int[] height, int[] channels, int desiredChannels);


    public static native void imageFree(ByteBuffer imageBuffer);


    public static native void hdrToLdrGamma(float gamma);

    public static native void hdrToLdrScale(float scale);

    public static native void ldrToHdrGamma(float gamma);

    public static native void ldrToHdrScale(float scale);

    public static native void convertIphonePngToRgb(boolean shouldConvert);

    public static native void convertIphonePngToRgbThread(boolean shouldConvert);


    public static native boolean is16Bit(String filename);

    public static native boolean is16BitFromFile(File file);

    public static native boolean is16BitFromMemory(byte[] data);

    public static native boolean is16BitFromCallbacks(StbImageIOCallbacks callbacks);

    public static native boolean isHdr(String filename);

    public static native boolean isHdrFromFile(File file);

    public static native boolean isHdrFromMemory(byte[] data);

    public static native boolean isHdrFromCallbacks(StbImageIOCallbacks callbacks);


    public static native int zlibDecodeBuffer(byte[] output, byte[] input);

    public static native byte[] zlibDecodeMalloc(byte[] input);

    public static native byte[] zlibDecodeMallocGuesssize(byte[] input, int initialSize);

    public static native byte[] zlibDecodeMallocGuesssizeHeaderflag(byte[] input, int initialSize, boolean parseHeader);

    public static native int zlibDecodeNoheaderBuffer(byte[] output, byte[] input);

    public static native byte[] zlibDecodeNoheaderMalloc(byte[] input);

}
