import generaloss.stb.image.StbImage;
import generaloss.stb.image.StbImageIoCallbacks;
import jpize.util.res.Resource;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;

public class Test {

    private static final String IMAGE1_PATH = "/image_1.jpg";
    private static final String IMAGE2_PATH = "/image_2.jpg";
    private static final String IMAGE3_PATH = "/image_3.png";
    private static final String IMAGE1_FILEPATH = ("./src/test/resources" + IMAGE1_PATH);
    private static final String IMAGE2_FILEPATH = ("./src/test/resources" + IMAGE2_PATH);
    private static final String IMAGE3_FILEPATH = ("./src/test/resources" + IMAGE3_PATH);
    private static final File IMAGE1_FILE = new File(IMAGE1_FILEPATH);
    private static final File IMAGE2_FILE = new File(IMAGE2_FILEPATH);
    private static final File IMAGE3_FILE = new File(IMAGE3_FILEPATH);
    private static final Resource IMAGE1_RES = Resource.internal(IMAGE1_PATH);
    private static final Resource IMAGE2_RES = Resource.internal(IMAGE2_PATH);
    private static final Resource IMAGE3_RES = Resource.internal(IMAGE3_PATH);
    private static final InputStream IMAGE1_STREAM = IMAGE1_RES.inStream();
    private static final InputStream IMAGE2_STREAM = IMAGE2_RES.inStream();
    private static final InputStream IMAGE3_STREAM = IMAGE3_RES.inStream();
    private static final int[] WIDTH = new int[1];
    private static final int[] HEIGHT = new int[1];
    private static final int[] CHANNELS = new int[1];
    private static StbImageIoCallbacks createIOCallbacks(InputStream stream) {
        return new StbImageIoCallbacks() {
            public int read(byte[] buffer, int size) throws IOException {
                return stream.read(buffer, 0, size);
            }
            public void skip(int n) throws IOException {
                stream.skip(n);
            }
            public int eof() {
                return 0;
            }
        };
    }
    private static final StbImageIoCallbacks IMAGE1_IO_CALLBACKS = createIOCallbacks(IMAGE1_STREAM);
    private static final StbImageIoCallbacks IMAGE2_IO_CALLBACKS = createIOCallbacks(IMAGE2_STREAM);
    private static final StbImageIoCallbacks IMAGE3_IO_CALLBACKS = createIOCallbacks(IMAGE3_STREAM);

    public static void main(String[] args) {
        final ByteBuffer buffer = StbImage.loadFromCallbacks16(IMAGE2_IO_CALLBACKS, WIDTH, HEIGHT, CHANNELS, 4);

        // print result
        System.out.println(
            "Result: " +
            ((buffer == null) ? null : buffer.capacity() / 1024 + "kb") + ", " +
            WIDTH[0] + "x" + HEIGHT[0] +
            ", channels: " + CHANNELS[0]
        );
    }

}
