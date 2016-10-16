
typedef struct {
	long double r;
	long double g;
	long double b;
	long double a;
} cgColor;

unsigned int framebuffer_h;
unsigned int framebuffer_v;

cgColor ** framebuffer;

void cgInitFramebuffer(unsigned int h, unsigned int v);
