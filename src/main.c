#include "projection.h"
#include "graphics.h"

void generate_image(void);
cgColor pick_color(cgPoint3f camera, cgVector3f ray_direction);

int main(int argc, char const *argv[]) {

	init_framebuffer(640, 800);
	cgSetProjectionMatrix(0, 0, 1000, 1000);
	cgSetCamaraPosition(100, 200, -100);

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
}

cgColor pick_color(cgPoint3f camera, cgVector3f ray_direction){
	
}
