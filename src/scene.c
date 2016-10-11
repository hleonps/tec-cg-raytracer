
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "graphics.h"
#include "vector.h"
#include "object.h"
#include "sphere.h"
#include "scene.h"

cgScene scene = {NULL, 0};

void cgAddSphereToScene(cgPoint3f center, long double radius, cgColor color){
	// cgObject * sphere = (cgObject *) malloc(sizeof(cgObject));
	cgObject sphere;
	sphere.type = SPHERE;
	sphere.color = color;
	sphere.intersection = &cgSphereIntersection;

	cgSphere * information = (cgSphere *) malloc(sizeof(cgSphere));
	information->radius = radius;
	information->center = center;

	sphere.data = (void *) information;

	scene.num_objects++;

	scene.objects = (cgObject *) realloc(scene.objects, sizeof(cgObject) * scene.num_objects);
	scene.objects[scene.num_objects - 1] = sphere;
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
