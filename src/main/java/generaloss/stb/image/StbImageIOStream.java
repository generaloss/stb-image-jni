package generaloss.stb.image;

import java.io.IOException;
import java.io.InputStream;

public class StbImageIOStream implements StbImageIOCallbacks {

    private final InputStream input;

    public StbImageIOStream(InputStream input) {
        this.input = input;
    }

    @Override
    public int read(byte[] buffer, int size) {
        try{
            final int length = Math.min(buffer.length, size);
            return input.read(buffer, 0, length);
        }catch(IOException e) {
            return 0;
        }
    }

    @Override
    public void skip(int n) throws IOException {
        input.skip(n);
    }

    @Override
    public boolean eof() {
        try{
            return (input.available() <= 0);
        }catch(IOException e) {
            return true;
        }
    }

}
