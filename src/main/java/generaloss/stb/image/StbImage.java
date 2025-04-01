package generaloss.stb.image;

import jpize.util.NativeLib;
import jpize.util.res.Resource;

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

    public static native long stbi_load(String filename, int[] width, int[] height, int[] channels, int desiredChannels);

    // Освобождает память, выделенную stb_image
    public static native void stbi_image_free(long imagePtr);

    // Проверяет, поддерживается ли файл
    public static native boolean stbi_info(String filename, int[] width, int[] height, int[] channels);

    // Устанавливает режим переворота по вертикали
    public static native void stbi_set_flip_vertically_on_load(boolean flip);

    // Загружает изображение из памяти (массив байтов)
    public static native long stbi_load_from_memory(byte[] imageData, int[] width, int[] height, int[] channels, int desiredChannels);

}
