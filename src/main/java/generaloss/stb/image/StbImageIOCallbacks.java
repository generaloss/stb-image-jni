package generaloss.stb.image;

import java.io.IOException;
import java.io.InputStream;

public interface StbImageIOCallbacks {
    
    int read(byte[] buffer, int size) throws IOException;
    
    void skip(int n) throws IOException;
    
    boolean eof() throws IOException;

    static StbImageIOCallbacks create(InputStream stream) {
        return new StbImageIOStream(stream);
    }

}