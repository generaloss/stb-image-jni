import generaloss.stb.image.*;
import jpize.util.res.Resource;

import java.io.File;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.util.Arrays;

public class Test {

    // presets for faster testing
    private static final String IMAGE1_PATH = "/image_1.png";
    private static final String IMAGE2_PATH = "/image_2.jpg";
    private static final String IMAGE3_PATH = "/image_3.gif";
    private static final String IMAGEHDR_PATH = "/image_hdr.hdr";
    private static final String IMAGE1_FILEPATH = ("./src/test/resources" + IMAGE1_PATH);
    private static final String IMAGE2_FILEPATH = ("./src/test/resources" + IMAGE2_PATH);
    private static final String IMAGE3_FILEPATH = ("./src/test/resources" + IMAGE3_PATH);
    private static final String IMAGEHDR_FILEPATH = ("./src/test/resources" + IMAGEHDR_PATH);
    private static final File IMAGE1_FILE = new File(IMAGE1_FILEPATH);
    private static final File IMAGE2_FILE = new File(IMAGE2_FILEPATH);
    private static final File IMAGE3_FILE = new File(IMAGE3_FILEPATH);
    private static final File IMAGEHDR_FILE = new File(IMAGEHDR_FILEPATH);
    private static final Resource IMAGE1_RES = Resource.internal(IMAGE1_PATH);
    private static final Resource IMAGE2_RES = Resource.internal(IMAGE2_PATH);
    private static final Resource IMAGE3_RES = Resource.internal(IMAGE3_PATH);
    private static final Resource IMAGEHDR_RES = Resource.internal(IMAGEHDR_PATH);
    private static final InputStream IMAGE1_STREAM = IMAGE1_RES.inStream();
    private static final InputStream IMAGE2_STREAM = IMAGE2_RES.inStream();
    private static final InputStream IMAGE3_STREAM = IMAGE3_RES.inStream();
    private static final InputStream IMAGEHDR_STREAM = IMAGEHDR_RES.inStream();
    private static final byte[] IMAGE1_DATA = IMAGE1_RES.readBytes();
    private static final byte[] IMAGE2_DATA = IMAGE2_RES.readBytes();
    private static final byte[] IMAGE3_DATA = IMAGE3_RES.readBytes();
    private static final byte[] IMAGEHDR_DATA = IMAGEHDR_RES.readBytes();
    private static final StbImageIOCallbacks IMAGE1_CALLBACKS = StbImageIOCallbacks.create(IMAGE1_STREAM);
    private static final StbImageIOCallbacks IMAGE2_CALLBACKS = StbImageIOCallbacks.create(IMAGE2_STREAM);
    private static final StbImageIOCallbacks IMAGE3_CALLBACKS = StbImageIOCallbacks.create(IMAGE3_STREAM);
    private static final StbImageIOCallbacks IMAGEHDR_CALLBACKS = StbImageIOCallbacks.create(IMAGEHDR_STREAM);
    private static final int[] DELAYS = new int[128];
    private static final int[] WIDTH = new int[1];
    private static final int[] HEIGHT = new int[1];
    private static final int[] FRAMES = new int[1];
    private static final int[] CHANNELS = new int[1];

    // test
    public static void main(String[] args) {
        System.out.println("HDR: " + StbImage.isHdr(IMAGEHDR_FILEPATH));

        // GIF:
        final ByteBuffer buffer = StbImage.loadGifFromMemory(IMAGE3_DATA, DELAYS, WIDTH, HEIGHT, FRAMES, CHANNELS, 4);
        // JPG: final ByteBuffer buffer = StbImage.loadFromCallbacks(IMAGE2_CALLBACKS, WIDTH, HEIGHT, CHANNELS, 4);
        // PNG: final ByteBuffer buffer = StbImage.load16FromMemory(IMAGE1_DATA, WIDTH, HEIGHT, CHANNELS, 4);

        // print result
        System.out.println("Data: " + ((buffer == null) ? null : buffer.capacity() / 1024 + "kb"));
        System.out.println("Size: " + WIDTH[0] + "x" + HEIGHT[0]);
        System.out.println("Channels: " + CHANNELS[0]);
        System.out.println("Frames: " + FRAMES[0]);
        System.out.println("Delays: " + Arrays.toString(Arrays.copyOf(DELAYS, FRAMES[0])));
    }

}
