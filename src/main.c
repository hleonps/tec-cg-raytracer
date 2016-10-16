#include <stdio.h>
#include <math.h>
#include "cg_raytracer.h"

#define SWAP(x) ( ((x) << 24) | \
	(((x) << 8) & 0x00ff0000) | \
	(((x) >> 8) & 0x0000ff00) | \
	((x) >> 24) )

void save_image();

const char* FILE_NAME = "scene.avs";

int main(int argc, char const *argv[]) {

	cgInitFramebuffer(800, 800);
	cgSetProjectionMatrix(0, 1000, 0, 1000);
	cgSetCameraPosition(500, 500, -1000);

	cgPoint3f center = {500,500, 500};
	cgColor red = {1,0,0,1};
	cgAddSphereToScene(center, 200, red);

	center.x = 200;
	center.y = 500;
	center.z = 800;

	cgColor blue = {0,0,1,1};
	cgAddSphereToScene(center, 150, blue);

	cgColor green = {0,1,0,1};
	center.x = 700;
	center.y = 600;
	center.z = 200;

	cgAddSphereToScene(center, 150, green);

	cgColor purple = {1,0,1,1};
	center.x = 500;
	center.y = 200;
	center.z = 350;

	cgAddSphereToScene(center, 50, purple);

	cgSetEnvironmentLighting(0.4);
	cgPoint3f light_position = {200, 270, 0};
	cgAddLightSourceToScene(light_position, 0.8, 0, 0.0025, 0);

	light_position.x = 750;
	light_position.y = 700;
	cgAddLightSourceToScene(light_position, 0.6, 0, 0.01, 0);

	light_position.x = 550;
	light_position.y = 100;
	light_position.z = 300;
	cgAddLightSourceToScene(light_position, 1, 0, 0.01, 0);

	cgGenerateImage();
	save_image();

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
