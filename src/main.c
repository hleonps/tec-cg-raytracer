#include <stdio.h>
#include <math.h>
#include "cg_raytracer.h"
#include "parser.h"

#define SWAP(x) ( ((x) << 24) | \
	(((x) << 8) & 0x00ff0000) | \
	(((x) >> 8) & 0x0000ff00) | \
	((x) >> 24) )

void save_image();

const char* FILE_NAME = "scene.avs";

int main(int argc, char const *argv[]) {

	if (argc == 1){
		printf("No file available\n");
		return -1;
	}
	else{
		FILE *fp = fopen(argv[1], "r");
		parser_init(fp);
		fclose(fp);

		cgGenerateImage();
		save_image();
	}
	return 0;
}

void save_image(){
	FILE * fp = fopen(FILE_NAME, "w");
	/* Attempt to open the file */
	if (fp == NULL) {
		printf("failed to create file\n");
	}
	else{
		// Store header
		unsigned int width = SWAP(framebuffer_h);
		unsigned int height = SWAP(framebuffer_v);
		fwrite(&width, sizeof(unsigned int), 1, fp);
		fwrite(&height, sizeof(unsigned int), 1, fp);

		for(int i = 0; i < framebuffer_v; i++){
			for(int j = 0; j < framebuffer_h; j++){
				/* Write the current pixel */
				unsigned char alpha = round(framebuffer[j][i].a * 255);
				unsigned char red = round(framebuffer[j][i].r * 255);
				unsigned char green = round(framebuffer[j][i].g * 255);
				unsigned char blue = round(framebuffer[j][i].b * 255);

				fwrite(&alpha, sizeof(unsigned char), 1, fp);
				fwrite(&red, sizeof(unsigned char), 1, fp);
				fwrite(&green, sizeof(unsigned char), 1, fp);
				fwrite(&blue, sizeof(unsigned char), 1, fp);
			}
		}

		fclose(fp);
	}
}
