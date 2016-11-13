#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "graphics.h"
#include "object.h"
#include "disk.h"

cgVector3f cgDiskNormalVector(void *information){
	cgDisk disk_information = (*(cgDisk*) (information));

	// If it was calculated earlier, use the stored vector
	if(disk_information.normal_vector != NULL){
		return *disk_information.normal_vector;
	}

	cgPoint3f point_a = disk_information.center;
	cgPoint3f point_b = disk_information.plane_points[0];
	cgPoint3f point_c = disk_information.plane_points[1];

	cgVector3f vector_a = cgDirectionVector(point_a, point_b);
	cgVector3f vector_b = cgDirectionVector(point_a, point_c);

	cgVector3f normal_vector = cgCrossProduct(vector_a, vector_b);
	cgVector3f unit_vector = cgNormalizedVector(normal_vector, cgVectorMagnitude(normal_vector));

	// Optimization. Store normal vector
	((cgDisk *) information)->normal_vector = (cgVector3f *) malloc(sizeof(cgVector3f));
	((cgDisk *) information)->normal_vector->x = unit_vector.x;
	((cgDisk *) information)->normal_vector->y = unit_vector.y;
	((cgDisk *) information)->normal_vector->z = unit_vector.z;

	return unit_vector;
}

cgIntersection * cgDiskIntersection(cgPoint3f camera, cgVector3f ray_direction, void * data){
	cgDisk disk_information = (*(cgDisk*) (data));

	cgIntersection * intersection = NULL;

	cgPoint3f disk_center = disk_information.center;
	cgVector3f normal_vector = cgDiskNormalVector(data);

	long double a = normal_vector.x, b = normal_vector.y,  c = normal_vector.z;

	/* Plane equation is required to get D, normal vector is [A, B, C]
		Ax + By + Cz + D = 0 */
	long double d = -(a * disk_center.x + b * disk_center.y + c * disk_center.z);
	long double denominator = a * ray_direction.x + b * ray_direction.y + c * ray_direction.z;

	if(denominator == 0){
		return intersection;
	}

	long double t = -(a * camera.x + b * camera.y + c * camera.z + d)/denominator;

	cgPoint3f intersection_point;
	intersection_point.x = camera.x + t * ray_direction.x;
	intersection_point.y = camera.y + t * ray_direction.y;
	intersection_point.z = camera.z + t * ray_direction.z;

	cgVector3f intersection_vector;
	intersection_vector.x = intersection_point.x - disk_center.x;
	intersection_vector.y = intersection_point.y - disk_center.y;
	intersection_vector.z = intersection_point.z - disk_center.z;

	long double distance = cgVectorMagnitude(intersection_vector);

	if(distance <= disk_information.outer_radius && distance >= disk_information.inner_radius){
		intersection = malloc(sizeof(cgIntersection));
		intersection->distance = t;
		intersection->point = intersection_point;
	}

	return intersection;
}