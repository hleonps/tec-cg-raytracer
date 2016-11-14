#define SWAP(x) ( ((x) << 24) | \
         (((x) << 8) & 0x00ff0000) | \
         (((x) >> 8) & 0x0000ff00) | \
         ((x) >> 24) )
#define FIX(x) (*(unsigned *)&(x) = \
         SWAP(*(unsigned *)&(x)))

typedef struct cgAVS_t cgAVS_t;
typedef struct cgAVS_Pixel cgAVS_Pixel;

struct cgAVS_t {
	int width;
	int height;
	cgAVS_Pixel **data;
};

struct cgAVS_Pixel {
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

cgAVS_t * readAVS(FILE* fp);