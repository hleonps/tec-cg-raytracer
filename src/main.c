
#include <stdio.h>
#include <math.h>
#include "vector.h"
#include "projection.h"
#include "graphics.h"
#include "object.h"
#include "scene.h"

#define SWAP(x) ( ((x) << 24) | \
	(((x) << 8) & 0x00ff0000) | \
	(((x) >> 8) & 0x0000ff00) | \
	((x) >> 24) )
#define FIX(x) (*(unsigned *)&(x) = \
	SWAP(*(unsigned *)&(x)))

void generate_image(void);
cgColor pick_color(cgPoint3f camera, cgVector3f ray_direction);
void save_image();

#define FILE_NAME "scene.avs"

int main(int argc, char const *argv[]) {

	init_framebuffer(800, 800);
	cgSetProjectionMatrix(0, 1000, 0, 1000);
	cgSetCamaraPosition(500, 500, -1000);

	cgPoint3f center = {500,500,0};
	cgColor red = {1,0,0,1};
	cgAddSphereToScene(center, 100, red);

	generate_image();

	return 0;
}

void generate_image(){
	for (int i = 0; i < framebuffer_h; i++) {
		for (int j = 0; j < framebuffer_v; j++) {
			cgPoint3f window_point = {cgMapXFrameToProjectionMatrix(i), cgMapYFrameToProjectionMatrix(j), 0};

			cgVector3f ray_vector = cgDirectionVector(camera, window_point);
			cgVector3f unit_ray_vector = cgNormalizedVector(ray_vector, cgVectorMagnitude(ray_vector));

			framebuffer[i][j] = pick_color(camera, unit_ray_vector);
		}
	}

	save_image();
}

cgColor pick_color(cgPoint3f camera, cgVector3f ray_direction){
	cgColor color = {0, 0, 0, 1};
	cgIntersection * intersection;

	intersection = first_intersection(camera, ray_direction);

	if(intersection){
		color = intersection->object.color;
	}

	return color;
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

		for(int i = 0; i < framebuffer_h; i++){
			for(int j = 0; j < framebuffer_v; j++){
				/* Write the current pixel */
				unsigned char alpha = round(framebuffer[i][j].a * 255);
				unsigned char red = round(framebuffer[i][j].r * 255);
				unsigned char green = round(framebuffer[i][j].g * 255);
				unsigned char blue = round(framebuffer[i][j].b * 255);

				fwrite(&alpha, sizeof(unsigned char), 1, fp);
				fwrite(&red, sizeof(unsigned char), 1, fp);
				fwrite(&green, sizeof(unsigned char), 1, fp);
				fwrite(&blue, sizeof(unsigned char), 1, fp);
			}
		}

		fclose(fp);
	}
}
