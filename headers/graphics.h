
typedef struct {
	long double r;
	long double b;
	long double g;
	long double a;
} cgColor;

unsigned int framebuffer_h;
unsigned int framebuffer_v;

cgColor ** framebuffer;

void init_framebuffer(unsigned int h, unsigned int v);
