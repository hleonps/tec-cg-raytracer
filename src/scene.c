
#include <stdio.h>
#include <float.h>
#include "graphics.h"
#include "vector.h"
#include "object.h"
#include "scene.h"

cgScene scene;

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
