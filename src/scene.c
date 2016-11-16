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

	if(polygon.texture != NULL){
		/* Rectangle points (Xmin, Ymin, Zmin) (Xmin, Ymax, Zmin) (Xmax, Ymin, Zmax) (Xmax, Ymax, Zmax) */
		long double min_x = information->points_3d[0].x,
			min_y = information->points_3d[0].y,
			min_z = information->points_3d[0].z,
			max_x = information->points_3d[0].x,
			max_y = information->points_3d[0].y,
			max_z = information->points_3d[0].z;

		information->texture = malloc(sizeof(cgPolygonTexture));
		information->texture->rectangle = malloc(sizeof(cgPoint3f) * 4);

		cgPoint3f *tmp;
		/* Get values */
		for(int i = 0; i < information->points_count; i++){
			tmp = &information->points_3d[i];
			if(tmp->x < min_x){
				min_x = tmp->x;
			}

			if(tmp->x > max_x){
				max_x = tmp->x;
			}

			if(tmp->y < min_y){
				min_y = tmp->y;
			}

			if(tmp->y > min_y){
				max_y = tmp->y;
			}

			if(tmp->z < min_z){
				min_z = tmp->z;
			}

			if(tmp->z > max_z){
				max_z = tmp->z;
			}
		}

		cgPoint3f p3 = {.x = min_x, .y = min_y, .z = min_z}, p1 = {.x = max_x, .y = max_y, .z = max_z};
		cgPoint3f p2 = {.x = max_x, .y = min_y, .z = max_z}, p0 = {.x = min_x, .y = max_y, .z = min_z};

		information->texture->rectangle[0] = p0;
		information->texture->rectangle[1] = p1;
		information->texture->rectangle[2] = p2;
		information->texture->rectangle[3] = p3;

		cgVector3f height_vector = {.x = p3.x - p0.x, .y = p3.y - p0.y, .z = p3.z - p0.z};
		cgVector3f width_vector = {.x = p1.x - p0.x, .y = p1.y - p0.y, .z = p1.z - p0.z};

		information->texture->height = cgVectorMagnitude(height_vector);
		information->texture->width = cgVectorMagnitude(width_vector);
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
	cgCylinder *information = (cgCylinder *) cylinder.data;

	if(cylinder.texture != NULL){
		cgPoint3f tmp = {
			.x = information->anchor.x,
			.y = information->anchor.y,
			.z = information->anchor.z
		};

		if(information->direction.x == 0){
			tmp.x += information->radius;
		}
		else if(information->direction.y == 0){
			tmp.y += information->radius;
		}
		else if(information->direction.z == 0){
			tmp.z += information->radius;
		}
		else{
			tmp.z += information->radius;
		}

		cgVector3f g = {
			.x = tmp.x - information->anchor.x,
			.y = tmp.y - information->anchor.y,
			.z = tmp.z - information->anchor.z
		};

		cgVector3f normalized_g = cgNormalizedVector(g, cgVectorMagnitude(g));

		cgVector3f *new_g = malloc(sizeof(cgVector3f));

		new_g->x = normalized_g.x;
		new_g->y = normalized_g.y;
		new_g->z = normalized_g.z;

		information->texture_start = new_g;

	}
	else{
		information->texture_start = NULL;
	}

	scene.num_objects++;

	scene.objects = (cgObject *) realloc(scene.objects, sizeof(cgObject) * scene.num_objects);
	scene.objects[scene.num_objects - 1] = cylinder;
}

void cgAddConeToScene(cgObject cone){

	cgCone *information = (cgCone *) cone.data;

	if(cone.texture != NULL){
		cgPoint3f tmp = {
			.x = information->anchor.x,
			.y = information->anchor.y,
			.z = information->anchor.z
		};

		if(information->direction.x == 0){
			tmp.x += information->radius_k;
		}
		else if(information->direction.y == 0){
			tmp.y += information->radius_k;
		}
		else if(information->direction.z == 0){
			tmp.z += information->radius_k;
		}
		else{
			tmp.z += information->radius_k;
		}

		cgVector3f g = {
			.x = tmp.x - information->anchor.x,
			.y = tmp.y - information->anchor.y,
			.z = tmp.z - information->anchor.z
		};

		cgVector3f normalized_g = cgNormalizedVector(g, cgVectorMagnitude(g));

		cgVector3f *new_g = malloc(sizeof(cgVector3f));

		new_g->x = normalized_g.x;
		new_g->y = normalized_g.y;
		new_g->z = normalized_g.z;

		information->texture_start = new_g;

	}
	else{
		information->texture_start = NULL;
	}

	scene.num_objects++;

	scene.objects = (cgObject *) realloc(scene.objects, sizeof(cgObject) * scene.num_objects);
	scene.objects[scene.num_objects - 1] = cone;
}

void cgAddDiskToScene(cgObject disk){
	cgDisk *information = (cgDisk *) disk.data;

	if(disk.texture != NULL){
		cgVector3f normal_vector = ((cgNormalVectorDisk) disk.normal_vector)(disk.data);

		long double a = normal_vector.x, b = normal_vector.y,  c = normal_vector.z;

		/* Plane equation is required to get D, normal vector is [A, B, C]
			Ax + By + Cz + D = 0 */
		long double d = -(a * information->center.x + b * information->center.y + c * information->center.z);

		long double min_x = information->center.x - information->outer_radius,
			min_y = information->center.y - information->outer_radius,
			min_z = (-a*min_x -b*min_y - d)/c,
			max_x = information->center.x + information->outer_radius,
			max_y = information->center.y + information->outer_radius,
			max_z = (-a*max_x -b*max_y - d)/c;

		information->texture = malloc(sizeof(cgDiskTexture));
		information->texture->rectangle = malloc(sizeof(cgPoint3f) * 4);

		cgPoint3f p3 = {.x = min_x, .y = min_y, .z = min_z}, p1 = {.x = max_x, .y = max_y, .z = max_z};
		cgPoint3f p2 = {.x = max_x, .y = min_y, .z = max_z}, p0 = {.x = min_x, .y = max_y, .z = min_z};

		information->texture->rectangle[0] = p0;
		information->texture->rectangle[1] = p1;
		information->texture->rectangle[2] = p2;
		information->texture->rectangle[3] = p3;

		cgVector3f height_vector = {.x = p3.x - p0.x, .y = p3.y - p0.y, .z = p3.z - p0.z};
		cgVector3f width_vector = {.x = p1.x - p0.x, .y = p1.y - p0.y, .z = p1.z - p0.z};

		information->texture->height = cgVectorMagnitude(height_vector);
		information->texture->width = cgVectorMagnitude(width_vector);
	}

	scene.num_objects++;

	scene.objects = (cgObject *) realloc(scene.objects, sizeof(cgObject) * scene.num_objects);
	scene.objects[scene.num_objects - 1] = disk;
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
		temp_intersection = scene.objects[i].intersection(camera, ray_direction, &(scene.objects[i]));

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
