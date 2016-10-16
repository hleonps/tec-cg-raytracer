
#include <stdio.h>
#include <math.h>
#include "vector.h"
#include "projection.h"
#include "graphics.h"
#include "object.h"
#include "light_source.h"
#include "scene.h"

#define MIN(X,Y) ((X < Y) ? X : Y)

#define SWAP(x) ( ((x) << 24) | \
	(((x) << 8) & 0x00ff0000) | \
	(((x) >> 8) & 0x0000ff00) | \
	((x) >> 24) )

void generate_image(void);
cgColor pick_color(cgPoint3f camera, cgVector3f ray_direction);
void save_image();

const char* FILE_NAME = "scene.avs";

int main(int argc, char const *argv[]) {

	init_framebuffer(800, 800);
	cgSetProjectionMatrix(0, 1000, 0, 1000);
	cgSetCamaraPosition(500, 500, -1000);

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
	cgColor color = {0.3, 0.3, 0.3, 1};
	cgIntersection * intersection;
	cgIntersection * intersection_between_object_light;

	intersection = cgFirstIntersection(camera, ray_direction);

	if(intersection){
		cgObject object = intersection->object;

		color = object.color;
		long double diffused_intensity = 0;
		long double specular_intensity = 0;

		// Calculate normal vector of current object
		cgVector3f normal_vector = object.normal_vector(intersection->point, object.data);

		for(int i = 0; i < scene.num_lights; i++){
			// Vector from intersection point to light source
			cgVector3f to_light_vector = cgDirectionVector(intersection->point, scene.lights[i].position);
			long double to_light_distance = cgVectorMagnitude(to_light_vector);
			// Unit vector from intersection point to light source
			cgVector3f unit_light_vector = cgNormalizedVector(to_light_vector, to_light_distance);

			// Look for obstacle between current object and current light source
			intersection_between_object_light = cgFirstIntersection(intersection->point, unit_light_vector);

			// Check if there is a obstacle. Subtract 0.000001 to light distance to fix black little points in object
			if(!intersection_between_object_light || (to_light_distance - 0.000001) < intersection_between_object_light->distance){

				long double light_normal_dot_product = cgDotProduct(unit_light_vector, normal_vector);

				// Calculate reflected ray
				cgVector3f reflected_ray = {
					2 * (normal_vector.x * light_normal_dot_product) - unit_light_vector.x,
					2 * (normal_vector.y * light_normal_dot_product) - unit_light_vector.y,
					2 * (normal_vector.z * light_normal_dot_product) - unit_light_vector.z
				};

				// Vector from intersection point to eye
				cgVector3f to_camera_direction = cgInvertedDirectionVector(ray_direction);
				long double reflected_dot_product = cgDotProduct(reflected_ray, to_camera_direction);

				if(light_normal_dot_product > 0){

					// Calculate the attenuation factor of current light
					long double attenuation_factor = cgAttenuationFactor(&scene.lights[i], to_light_distance);

					long double attenuated_light = scene.lights[i].intensity * attenuation_factor;

					if(reflected_dot_product > 0){
						// Calculate specular value for current light. Accumulate
						specular_intensity = specular_intensity +
							(pow(reflected_dot_product, object.specular_focus) * object.specular_factor * attenuated_light);
					}

					// Calculate diffuse factor for current light. Accumulate.
					diffused_intensity = diffused_intensity +
						(light_normal_dot_product * object.diffuse_factor * attenuated_light);
				}
			}
		}

		// Add enviroment lightning to diffused intensity
		diffused_intensity = diffused_intensity + (scene.environment_lighting * object.environment_lighting);

		// Values from 0 to 1
		diffused_intensity = MIN(1.0, diffused_intensity);
		specular_intensity = MIN(1.0, specular_intensity);

		// Applies diffuse intensity to color
		color.r = diffused_intensity * color.r;
		color.g = diffused_intensity * color.g;
		color.b = diffused_intensity * color.b;

		// Applies specular intensity to diffused color
		color.r = color.r + (specular_intensity * (1.0 - color.r));
		color.g = color.g + (specular_intensity * (1.0 - color.g));
		color.b = color.b + (specular_intensity * (1.0 - color.b));
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
