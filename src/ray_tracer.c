#include <math.h>
#include "vector.h"
#include "projection.h"
#include "figures.h"
#include "scene.h"

#define MIN(X,Y) ((X < Y) ? X : Y)
const long double EPSILON = 0.000002;
const long double NO_INTERSECTION_T_VALUE = -1.000000;
const unsigned int MAX_REFLECTION_LEVEL = 1;

cgColor cgPickColor(cgPoint3f camera, cgVector3f ray_direction, unsigned int reflexion_level);

void cgGenerateImage(){
	for (int i = 0; i < framebuffer_h; i++) {
		for (int j = 0; j < framebuffer_v; j++) {
			cgPoint3f window_point = {cgMapXFrameToProjectionMatrix(i), cgMapYFrameToProjectionMatrix(j), 0};

			cgVector3f ray_vector = cgDirectionVector(camera, window_point);
			cgVector3f unit_ray_vector = cgNormalizedVector(ray_vector, cgVectorMagnitude(ray_vector));

			framebuffer[i][j] = cgPickColor(camera, unit_ray_vector, MAX_REFLECTION_LEVEL);
		}
	}
}

cgColor cgPickColor(cgPoint3f camera, cgVector3f ray_direction, unsigned int reflexion_level){
	cgColor color = {0.3, 0.3, 0.3, 1};
	cgIntersection * intersection;
	cgIntersection * intersection_between_object_light;

	intersection = cgFirstIntersection(camera, ray_direction);

	if(intersection){
		cgObject object = intersection->object;

		color = object.color;
		long double diffuse_intensity = 0;
		long double specular_intensity = 0;

		// Calculate normal vector of current object
		cgVector3f normal_vector;
		switch (object.type) {
			case SPHERE:
				normal_vector = ((cgNormalVectorSphere) object.normal_vector)(intersection->point, object.data);
				break;
			case POLYGON:
				normal_vector = ((cgNormalVectorPolygon) object.normal_vector)(object.data);
				break;
			case CYLINDER:
				normal_vector = ((cgNormalVectorCylinder) object.normal_vector)(intersection->point, object.data);
				break;
		}

		if(cgDotProduct(ray_direction, normal_vector) > 0){
			normal_vector = cgInvertedDirectionVector(normal_vector);
		}

		for(int i = 0; i < scene.num_lights; i++){
			// Vector from intersection point to light source
			cgVector3f to_light_vector = cgDirectionVector(intersection->point, scene.lights[i].position);
			long double to_light_distance = cgVectorMagnitude(to_light_vector);
			// Unit vector from intersection point to light source
			cgVector3f unit_light_vector = cgNormalizedVector(to_light_vector, to_light_distance);

			// Look for obstacle between current object and current light source
			intersection_between_object_light = cgFirstIntersection(intersection->point, unit_light_vector);

			// Check if there is a obstacle. Subtract 0.000001 to light distance to fix black little points in object
			if(!intersection_between_object_light || (to_light_distance - EPSILON) < intersection_between_object_light->distance){

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
							cgSpecularIntensity(reflected_dot_product, object.specular_factor, object.specular_focus, attenuated_light);
					}

					// Calculate diffuse value for current light. Accumulate.
					diffuse_intensity = diffuse_intensity +
						cgDiffuseIntensity(light_normal_dot_product, object.diffuse_factor, attenuated_light);
				}
			}
		}

		// Add enviroment lightning to diffuse intensity
		diffuse_intensity = diffuse_intensity + (scene.environment_lighting * object.environment_lighting);

		// Values from 0 to 1
		diffuse_intensity = MIN(1.0, diffuse_intensity);
		specular_intensity = MIN(1.0, specular_intensity);

		// Applies diffuse intensity to color
		color.r = diffuse_intensity * color.r;
		color.g = diffuse_intensity * color.g;
		color.b = diffuse_intensity * color.b;

		// Applies specular intensity to diffused color
		color.r = color.r + (specular_intensity * (1.0 - color.r));
		color.g = color.g + (specular_intensity * (1.0 - color.g));
		color.b = color.b + (specular_intensity * (1.0 - color.b));

		if(reflexion_level > 0){
			cgVector3f to_anchor_vector = cgInvertedDirectionVector(ray_direction);
			cgVector3f to_anchor_unit_vector = cgNormalizedVector(to_anchor_vector, cgVectorMagnitude(to_anchor_vector));

			long double anchor_normal_dot_product = cgDotProduct(normal_vector, to_anchor_unit_vector);

			cgVector3f to_anchor_reflected_ray = {
				2 * (normal_vector.x * anchor_normal_dot_product) - to_anchor_unit_vector.x,
				2 * (normal_vector.y * anchor_normal_dot_product) - to_anchor_unit_vector.y,
				2 * (normal_vector.z * anchor_normal_dot_product) - to_anchor_unit_vector.z
			};

			cgColor reflected_color = cgPickColor(intersection->point, to_anchor_reflected_ray, reflexion_level - 1);

			long double object_color_factor = 1.0 - object.reflection_factor - object.transparency_factor;

			color.r = (object_color_factor * color.r) + (object.reflection_factor * reflected_color.r);
			color.g = (object_color_factor * color.g) + (object.reflection_factor * reflected_color.g);
			color.b = (object_color_factor * color.b) + (object.reflection_factor * reflected_color.b);
		}
	}

	return color;
}
