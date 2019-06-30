/* fast allocation */

const int MAX_MEM = 4e8;
int mpos = 0;
char mem[MAX_MEM];

inline void * operator new(size_t n) {
    assert((mpos += n) <= MAX_MEM);
    return (void *)(mem + mpos - n);
}

inline void operator delete(void *) noexcept {}
