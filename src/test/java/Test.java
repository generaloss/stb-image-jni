import generaloss.stb.image.StbImage;
import generaloss.stb.image.StbImageIoCallbacks;
import jpize.util.Utils;
import jpize.util.res.Resource;

import java.io.File;
import java.io.InputStream;
import java.nio.ByteBuffer;

public class Test {

    public static void main(String[] args) {
        int[] width = new int[1];
        int[] height = new int[1];
        int[] channels = new int[1];

        //final ByteBuffer imageBuffer = StbImage.load("./src/test/resources/image_3.png", width, height, channels, 4);
        final InputStream in = Resource.internal("/image_2.jpg").inStream();
        final File file = new File("src/test/resources/image_2.jpg");
        final ByteBuffer buffer = StbImage.loadFromFile16(file, width, height, channels, 4);
        Utils.close(in);
        System.out.println("Image: " + ((buffer == null) ? null : buffer.capacity() / 1024 + "kb") + ", " + width[0] + "x" + height[0] + ", channels: " + channels[0]);
    }

}
