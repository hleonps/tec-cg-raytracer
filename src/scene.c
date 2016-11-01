#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include "vector.h"
#include "figures.h"
#include "scene.h"

extern const long double EPSILON;
cgScene scene = {NULL, 0, NULL, 0, 0};

void cgAddPolygonToScene(cgObject polygon){

	cgPolygon *information = (cgPolygon *) polygon.data;

	/* Generate 2D Points of the polygon */
	cgVector3f normal_vector = cgPolygonNormalVector(information);

	long double a = fabsl(normal_vector.x), b = fabsl(normal_vector.y), c = fabsl(normal_vector.z);
	information->removed_coordinate = X;
	information->points_2d = (cgPoint2f *) malloc(sizeof(cgPoint2f) * information->points_count);

	if(b > a){
		information->removed_coordinate = Y;
	}

	if(c > b && c > a){
		information->removed_coordinate = Z;
	}

	for(int i = 0; i < information->points_count; i++){
		switch(information->removed_coordinate){
			case X:
				information->points_2d[i].x = information->points_3d[i].y;
				information->points_2d[i].y = information->points_3d[i].z;
				break;
			case Y:
				information->points_2d[i].x = information->points_3d[i].x;
				information->points_2d[i].y = information->points_3d[i].z;
				break;
			case Z:
				information->points_2d[i].x = information->points_3d[i].x;
				information->points_2d[i].y = information->points_3d[i].y;
				break;
			default:
				break;
		}
	}

	scene.num_objects++;

	scene.objects = (cgObject *) realloc(scene.objects, sizeof(cgObject) * scene.num_objects);
	scene.objects[scene.num_objects - 1] = polygon;
}

void cgAddSphereToScene(cgObject sphere){
	scene.num_objects++;

	scene.objects = (cgObject *) realloc(scene.objects, sizeof(cgObject) * scene.num_objects);
	scene.objects[scene.num_objects - 1] = sphere;
}

void cgAddCylinderToScene(cgObject cylinder){
	scene.num_objects++;

	scene.objects = (cgObject *) realloc(scene.objects, sizeof(cgObject) * scene.num_objects);
	scene.objects[scene.num_objects - 1] = cylinder;
}

void cgAddConeToScene(cgObject cone){
	scene.num_objects++;

	scene.objects = (cgObject *) realloc(scene.objects, sizeof(cgObject) * scene.num_objects);
	scene.objects[scene.num_objects - 1] = cone;
}

void cgAddLightSourceToScene(cgLight light_source){
	scene.num_lights++;

	scene.lights = (cgLight *) realloc(scene.lights, sizeof(cgLight) * scene.num_lights);
	scene.lights[scene.num_lights - 1] = light_source;
}

void cgSetEnvironmentLighting(long double intensity){
	scene.environment_lighting = intensity;
}

cgIntersection * cgFirstIntersection(cgPoint3f camera, cgVector3f ray_direction){

	cgIntersection * intersection = NULL;
	cgIntersection * temp_intersection = NULL;
	long double t_min = DBL_MAX;

	for (int i = 0; i < scene.num_objects; i++) {
		temp_intersection = scene.objects[i].intersection(camera, ray_direction, scene.objects[i].data);

		if(temp_intersection && temp_intersection->distance < t_min
				&& temp_intersection->distance > EPSILON){
			temp_intersection->object = scene.objects[i];
			t_min = temp_intersection->distance;

			intersection = temp_intersection;
		}
	}

	return intersection;
}
