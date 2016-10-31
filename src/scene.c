#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include "vector.h"
#include "figures.h"
#include "scene.h"

void cgAddPolygonToSceneAux(cgPoint3f * points, int points_count, cgColor color);

extern const long double EPSILON;
cgScene scene = {NULL, 0, NULL, 0, 0};

void cgAddSphereToScene(cgPoint3f center, long double radius, cgColor color){
	cgObject sphere;
	sphere.type = SPHERE;
	sphere.color = color;
	sphere.intersection = &cgSphereIntersection;
	sphere.normal_vector = (cgNormalVector) &cgSphereNormalVector;
	sphere.diffuse_factor = 0.8;
	sphere.specular_factor = 0.8;
	sphere.specular_focus = 50;
	sphere.environment_lighting = 0.2;
	sphere.transparency_factor = 0;
	sphere.reflection_factor = 0.2;

	cgSphere * information = (cgSphere *) malloc(sizeof(cgSphere));
	information->radius = radius;
	information->center = center;

	sphere.data = (void *) information;

	scene.num_objects++;

	scene.objects = (cgObject *) realloc(scene.objects, sizeof(cgObject) * scene.num_objects);
	scene.objects[scene.num_objects - 1] = sphere;
}

void cgAddPolygonToScene(cgPoint3f * points, int points_count, cgColor color){
	if(points_count > 2){
		cgAddPolygonToSceneAux(points, points_count, color);
	}
}

void cgAddPolygonToSceneAux(cgPoint3f * points, int points_count, cgColor color){
	cgObject polygon;
	polygon.type = POLYGON;
	polygon.color = color;
	polygon.intersection = &cgPolygonIntersection;
	polygon.normal_vector = (cgNormalVector) &cgPolygonNormalVector;
	polygon.diffuse_factor = 0.8;
	polygon.specular_factor = 0.8;
	polygon.specular_focus = 50;
	polygon.environment_lighting = 0.6;
	polygon.transparency_factor = 0;
	polygon.reflection_factor = 0;

	cgPolygon *information = (cgPolygon *) malloc(sizeof(cgPolygon));
	information->points_3d = (cgPoint3f *) malloc(sizeof(cgPoint3f) * points_count);
	information->points_count = points_count;
	information->normal_vector = NULL;

	for (int i = 0; i < points_count; i++)
	{
		information->points_3d[i].x = points[i].x;
		information->points_3d[i].y = points[i].y;
		information->points_3d[i].z = points[i].z;
	}

	/* Generate 2D Points of the polygon */
	cgVector3f normal_vector = cgPolygonNormalVector(information);

	long double a = fabsl(normal_vector.x), b = fabsl(normal_vector.y), c = fabsl(normal_vector.z);
	information->removed_coordinate = X;
	information->points_2d = (cgPoint2f *) malloc(sizeof(cgPoint2f) * points_count);

	if(b > a){
		information->removed_coordinate = Y;
	}

	if(c > b && c > a){
		information->removed_coordinate = Z;
	}

	for(int i = 0; i < points_count; i++){
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

	polygon.data = (void *) information;

	scene.num_objects++;

	scene.objects = (cgObject *) realloc(scene.objects, sizeof(cgObject) * scene.num_objects);
	scene.objects[scene.num_objects - 1] = polygon;
}

void cgAddCylinderToScene(cgPoint3f anchor, cgVector3f direction, long double radius,
		long double distance_a, long double distance_b, cgColor color){

	cgObject cylinder;
	cylinder.type = CYLINDER;
	cylinder.color = color;
	cylinder.intersection = &cgCylinderIntersection;
	cylinder.normal_vector = (cgNormalVector) &cgCylinderNormalVector;
	cylinder.diffuse_factor = 0.8;
	cylinder.specular_factor = 0.8;
	cylinder.specular_focus = 50;
	cylinder.environment_lighting = 0.2;
	cylinder.transparency_factor = 0;
	cylinder.reflection_factor = 0;

	cgCylinder * information = (cgCylinder *) malloc(sizeof(cgCylinder));
	information->anchor = anchor;

	// Vector Q (Direction) is an unit vector.
	cgVector3f unit_direction = cgNormalizedVector(direction, cgVectorMagnitude(direction));
	information->direction = unit_direction;
	information->radius = radius;
	information->distance_a = distance_a;
	information->distance_b = distance_b;

	cylinder.data = (void *) information;

	scene.num_objects++;

	scene.objects = (cgObject *) realloc(scene.objects, sizeof(cgObject) * scene.num_objects);
	scene.objects[scene.num_objects - 1] = cylinder;
}

void cgAddConeToScene(cgPoint3f anchor, cgVector3f direction, long double radius_k, long double distance_k,
		long double distance_a, long double distance_b, cgColor color){

	cgObject cone;
	cone.type = CONE;
	cone.color = color;
	cone.intersection = &cgConeIntersection;
	cone.normal_vector = (cgNormalVector) &cgConeNormalVector;
	cone.diffuse_factor = 0.8;
	cone.specular_factor = 0.8;
	cone.specular_focus = 50;
	cone.environment_lighting = 0.2;
	cone.transparency_factor = 0;
	cone.reflection_factor = 0;

	cgCone * information = (cgCone *) malloc(sizeof(cgCone));
	information->anchor = anchor;

	// Vector Q (Direction) is an unit vector.
	cgVector3f unit_direction = cgNormalizedVector(direction, cgVectorMagnitude(direction));
	information->direction = unit_direction;
	information->radius_k = radius_k;
	information->distance_k = distance_k;
	information->distance_a = distance_a;
	information->distance_b = distance_b;

	cone.data = (void *) information;

	scene.num_objects++;

	scene.objects = (cgObject *) realloc(scene.objects, sizeof(cgObject) * scene.num_objects);
	scene.objects[scene.num_objects - 1] = cone;
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

			if(intersection){
				free(intersection);
			}
			intersection = temp_intersection;
		}
		else if(temp_intersection){
			free(temp_intersection);
		}
	}

	return intersection;
}
