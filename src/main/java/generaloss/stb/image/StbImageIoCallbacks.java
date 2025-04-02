package generaloss.stb.image;

public interface StbImageIoCallbacks {
    
    int read(byte[] buffer, int size);
    
    void skip(int n);
    
    int eof();
    
}