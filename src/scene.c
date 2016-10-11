
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "graphics.h"
#include "vector.h"
#include "object.h"
#include "sphere.h"
#include "light_source.h"
#include "scene.h"

cgScene scene = {NULL, 0, NULL, 0, 0};

void cgAddSphereToScene(cgPoint3f center, long double radius, cgColor color){
	// cgObject * sphere = (cgObject *) malloc(sizeof(cgObject));
	cgObject sphere;
	sphere.type = SPHERE;
	sphere.color = color;
	sphere.intersection = &cgSphereIntersection;
	sphere.normal_vector = &cgSphereNormalVector;
	sphere.diffuse_factor = 0.8;
	sphere.environment_lighting = 0.2;

	cgSphere * information = (cgSphere *) malloc(sizeof(cgSphere));
	information->radius = radius;
	information->center = center;

	sphere.data = (void *) information;

	scene.num_objects++;

	scene.objects = (cgObject *) realloc(scene.objects, sizeof(cgObject) * scene.num_objects);
	scene.objects[scene.num_objects - 1] = sphere;
}

void cgAddLightSourceToScene(cgPoint3f position, long double intensity, long double c1, long double c2, long double c3){
	cgLight light_source;

	light_source.position = position;
	light_source.intensity = intensity;

	light_source.c1 = c1;
	light_source.c2 = c2;
	light_source.c3 = c3;

	scene.num_lights++;

	scene.lights = (cgLight *) realloc(scene.lights, sizeof(cgLight) * scene.num_lights);
	scene.lights[scene.num_lights - 1] = light_source;
}

void cgSetEnvironmentLighting(long double intensity){
	scene.environment_lighting = intensity;
}

cgIntersection * first_intersection(cgPoint3f camera, cgVector3f ray_direction){

	cgIntersection * intersection = NULL;
	cgIntersection * temp_intersection = NULL;
	long double t_min = DBL_MAX;

	for (int i = 0; i < scene.num_objects; i++) {

		temp_intersection = scene.objects[i].intersection(camera, ray_direction, scene.objects[i].data);

		if(temp_intersection && temp_intersection->distance < t_min){
			temp_intersection->object = scene.objects[i];
			t_min = temp_intersection->distance;

			intersection = temp_intersection;
		}
	}

	return intersection;
}
