#include <stdlib.h>
#include "graphics.h"

cgColor ** framebuffer;

unsigned int framebuffer_h;
unsigned int framebuffer_v;

void cgInitFramebuffer(unsigned int h, unsigned int v){
	framebuffer = (cgColor **) malloc(sizeof(cgColor *) * h);
	for(int i = 0; i < h; i++){
		framebuffer[i] = (cgColor *) malloc(sizeof(cgColor) * v);

		for(int j = 0; j < v; j++){
			framebuffer[i][j].r = 0;
			framebuffer[i][j].g = 0;
			framebuffer[i][j].b = 0;
			framebuffer[i][j].a = 1;
		}
	}

	framebuffer_h = h;
	framebuffer_v = v;
}
