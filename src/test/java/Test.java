import generaloss.stb.image.StbImage;
import jpize.util.res.Resource;

import java.nio.ByteBuffer;

public class Test {

    public static void main(String[] args) {
        int[] width = new int[1];
        int[] height = new int[1];
        int[] channels = new int[1];

        //final ByteBuffer imageBuffer = StbImage.load("./src/test/resources/image_3.png", width, height, channels, 4);
        final ByteBuffer imageBuffer = StbImage.loadFromMemory(Resource.internal("/image_3.png").readBytes(), width, height, channels, 4);
        if(imageBuffer != null) {
            System.out.println("Loaded image: " + width[0] + "x" + height[0] + ", channels: " + channels[0]);
            StbImage.imageFree(imageBuffer);
        }
    }

}
