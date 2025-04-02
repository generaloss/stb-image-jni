import generaloss.stb.image.StbImage;
import generaloss.stb.image.StbImageIoCallbacks;
import jpize.util.Utils;
import jpize.util.res.Resource;

import java.io.InputStream;

public class Test {

    public static void main(String[] args) {
        int[] width = new int[1];
        int[] height = new int[1];
        int[] channels = new int[1];

        //final ByteBuffer imageBuffer = StbImage.load("./src/test/resources/image_3.png", width, height, channels, 4);
        final InputStream in = Resource.internal("/image_2.jpg").inStream();
        final StbImageIoCallbacks callbacks = new StbImageIoCallbacks() {
            @Override
            public int read(byte[] buffer, int size) {
                System.out.println("read " + size);
                try {
                    return in.read(buffer);
                }catch (Exception e) {
                    return -1;
                }
            }
            @Override
            public void skip(int n) {
                System.out.println("skip " + n);
                try {
                    // noinspection ResultOfMethodCallIgnored
                    in.skip(n);
                }catch (Exception e) {
                    e.printStackTrace();
                }
            }
            @Override
            public int eof() {
                System.out.println("eof");
                return 0;
            }
        };
        StbImage.loadFromCallbacks(callbacks, 0L, width, height, channels, 4);
        Utils.close(in);
        System.out.println("Image info: " + width[0] + "x" + height[0] + ", channels: " + channels[0]);
    }

}
