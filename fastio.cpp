inline int readChar();
inline int readWord(char * s);
template<typename T = int> inline T readInt();

inline void writeChar(int x);
inline void writeWord(const char * s);
template<typename T> inline void writeInt(T x, char end = 0);

static const int buf_size = 4096;

/* read */

inline int getChar() {
    static char buf[buf_size];
    static int len = 0, pos = 0;
    if (pos == len)
        len = fread(buf, 1, buf_size, stdin), pos = 0;
    return pos == len ? -1 : buf[pos++];
}

inline int readChar() {
    int c = getChar();
    while (c <= 32)
        c = getChar();
    return c;
}

inline int readWord(char * s) {
    int c = readChar(), len = 0;
    while (c > 32)
        s[len++] = c, c = getChar();
    return len;
}

template<typename T>
inline T readInt() {
    int s = 1, c = readChar();
    T x = 0;
    if (c == '-')
        s = -1, c = getChar();
    while ('0' <= c && c <= '9')
        x = x * 10 + c - '0', c = getChar();
    return s == 1 ? x : -x;
}

/* write */

static int write_pos = 0;
static char write_buf[buf_size];

inline void writeChar(int x) {
    if (write_pos == buf_size)
        fwrite(write_buf, 1, buf_size, stdout), write_pos = 0;
    write_buf[write_pos++] = x;
}

template<typename T>
inline void writeInt(T x, char end) {
    char s[24];
    int n = 0;
    if (x < 0)
        writeChar('-'), x = -x;
    while (x || !n) 
        s[n++] = '0' + x % 10, x /= 10;
    while (n--) 
        writeChar(s[n]);
    if (end) 
        writeChar(end);
}

inline void writeWord(const char * s) {
    while (*s)
        writeChar(*s++);
}

struct Flusher {
    ~Flusher() {
        if (write_pos)
            fwrite(write_buf, 1, write_pos, stdout);
    }
} flusher;
