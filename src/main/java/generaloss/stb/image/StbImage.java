package generaloss.stb.image;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.ByteBuffer;
import java.nio.file.Files;
import java.nio.file.Path;

public class StbImage {

    static {
        loadNative();
    }

    @SuppressWarnings("UnsafeDynamicallyLoadedCode")
    private static void loadNative() {
        final String os = detectOS();

        if(os.equals("android")) {
            System.loadLibrary("stb_image_jni");
            return;
        }

        final String arch = detectArch();
        final String libName = (os.equals("windows") ? "stb_image_jni.dll" : "libstb_image_jni.so");
        final String pathInJar = String.format("/jni/%s/%s/%s", os, arch, libName);

        try(InputStream in = StbImage.class.getResourceAsStream(pathInJar)) {
            if(in == null)
                throw new UnsatisfiedLinkError("Native library not found: " + pathInJar);

            final Path temp = Files.createTempFile("stb_image_jni", libName);
            temp.toFile().deleteOnExit();

            try(OutputStream out = Files.newOutputStream(temp)) {
                final byte[] buffer = new byte[4096];
                int read;
                while((read = in.read(buffer)) != -1)
                    out.write(buffer, 0, read);
            }

            System.load(temp.toAbsolutePath().toString());
        }catch(IOException e) {
            throw new RuntimeException("Failed to load native library", e);
        }
    }

    private static String detectOS() {
        final String os = System.getProperty("os.name").toLowerCase();
        if(os.contains("win"))
            return "windows";

        if(os.contains("linux")) {
            String vm = System.getProperty("java.vm.name").toLowerCase();
            if(vm.contains("dalvik") || vm.contains("art"))
                return "android";
            return "linux";
        }
        throw new UnsupportedOperationException("Unsupported OS: " + os);
    }

    private static String detectArch() {
        final String arch = System.getProperty("os.arch").toLowerCase();
        if(arch.contains("amd64") || arch.contains("x86_64")) return "x86_64";
        if(arch.contains("86")) return "i686";
        if(arch.contains("aarch64") || arch.contains("arm64")) return "aarch64";
        if(arch.contains("riscv")) return "riscv64";
        if(arch.contains("x86")) return "x86";
        throw new UnsupportedOperationException("Unsupported architecture: " + arch);
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
