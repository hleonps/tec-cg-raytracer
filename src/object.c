#include <stdio.h>
#include <float.h>
#include "graphics.h"
#include "vector.h"
#include "object.h"

cgObjectList object_list;

cgIntersection * first_intersection(cgPoint3f camera, cgVector3f ray_direction){

	cgIntersection * intersection = NULL;
	cgIntersection * temp_intersection = NULL;
	long double t_min = DBL_MAX;

	for (int i = 0; i < object_list.size; i++) {

		temp_intersection = object_list.objects[i].intersection(camera, ray_direction, object_list.objects[i].data);

		if(temp_intersection && temp_intersection->distance < t_min){
			t_min = temp_intersection->distance;

			intersection = temp_intersection;
		}
	}

	return intersection;
}
