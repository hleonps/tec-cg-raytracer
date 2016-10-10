#include "vector.h"
#include "projection.h"

unsigned int framebuffer_h;
unsigned int framebuffer_v;

cg3DPoint camera = {0, 0, -1};
cgProjectionMatrix projection_matrix = {{0,0}, {1000, 1000}};

long double cgMapXFrameToProjectionMatrix(int x){
	long double min_x = projection_matrix.min.x;
	long double max_x = projection_matrix.max.x;

	return (((long double) ((x + 0.5) * (max_x - min_x)))/ framebuffer_h) + min_x;
}

long double cgMapYFrameToProjectionMatrix(int y){
	long double min_y = projection_matrix.min.y;
	long double max_y = projection_matrix.max.y;

	return (((long double) ((y + 0.5) * (max_y - min_y)))/ framebuffer_v) + min_y;
}
