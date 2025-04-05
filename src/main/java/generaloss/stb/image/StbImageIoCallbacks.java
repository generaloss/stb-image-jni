package generaloss.stb.image;

import java.io.IOException;

public interface StbImageIoCallbacks {
    
    int read(byte[] buffer, int size) throws IOException;
    
    void skip(int n) throws IOException;
    
    int eof() throws IOException;
    
}