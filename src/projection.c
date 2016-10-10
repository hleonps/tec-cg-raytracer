#include "vector.h"
#include "graphics.h"
#include "projection.h"

cgPoint3f camera = {0, 0, -1};
cgProjectionMatrix projection_matrix = {{0,0}, {0, 0}};

void cgSetCamaraPosition(int x, int y, int z){
	camera.x = x;
	camera.y = y;
	camera.z = (z < -1) ? z : -1;
}

void cgSetProjectionMatrix(int left, int right, int bottom, int top){
	projection_matrix.min.x = left;
	projection_matrix.max.x = right;

	projection_matrix.min.y = bottom;
	projection_matrix.max.y = top;
}

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
