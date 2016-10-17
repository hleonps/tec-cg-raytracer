#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "graphics.h"
#include "object.h"
#include "polygon.h"

int isIntersectionOnPolygon(cgPoint2f intersection, cgPolygon polygon);
cgPoint2f* moveIntersectionToOrigin(cgPoint2f intersection, int points_count, cgPoint2f* points);

cgVector3f cgPolygonNormalVector(void *information){
	cgPolygon polygon_information = (*(cgPolygon*) (information));

	cgPoint3f point_a = polygon_information.points_3d[0];
	cgPoint3f point_b = polygon_information.points_3d[1];
	cgPoint3f point_c = polygon_information.points_3d[2];

	cgVector3f vector_a;
	vector_a.x = point_b.x - point_a.x;
	vector_a.y = point_b.y - point_a.y;
	vector_a.z = point_b.z - point_a.z;

	cgVector3f vector_b;
	vector_b.x = point_c.x - point_a.x;
	vector_b.y = point_c.y - point_a.y;
	vector_b.z = point_c.z - point_a.z;

	cgVector3f normal_vector = cgCrossProduct(vector_a, vector_b);
	cgVector3f unit_vector = cgNormalizedVector(normal_vector, cgVectorMagnitude(normal_vector));

	return unit_vector;
}

cgIntersection * cgPolygonIntersection(cgPoint3f camera, cgVector3f ray_direction, void * data){
	cgPolygon polygon_information = (*(cgPolygon*) (data));
	cgPoint3f polygon_point = polygon_information.points_3d[0];
	cgVector3f normal_vector = cgPolygonNormalVector(data);

	long double a = normal_vector.x, b = normal_vector.y,  c = normal_vector.z;

	/* Plane equation is required to get D, normal vector is [A, B, C]
		Ax + By + Cz + D = 0 */

	long double d = -(a * polygon_point.x + b * polygon_point.y + c * polygon_point.z);
	long double denominator = a * ray_direction.x + b * ray_direction.y + c * ray_direction.z;

	if(denominator == 0){
		return NULL;
	}

	long double t = -(a * camera.x + b * camera.y + c * camera.z + d)/denominator;

	cgPoint3f intersection_point;
	intersection_point.x = camera.x + t * ray_direction.x;
	intersection_point.y = camera.y + t * ray_direction.y;
	intersection_point.z = camera.z + t * ray_direction.z;

	cgPoint2f intersection_2d;

	if(polygon_information.removed_coordinate == X){
		intersection_2d.x = intersection_point.y;
		intersection_2d.y = intersection_point.z;
	}
	else if(polygon_information.removed_coordinate == Y){
		intersection_2d.x = intersection_point.x;
		intersection_2d.y = intersection_point.z;
	}
	else{
		intersection_2d.x = intersection_point.x;
		intersection_2d.y = intersection_point.y;
	}

	if(isIntersectionOnPolygon(intersection_2d, polygon_information)){
		cgIntersection *intersection = malloc(sizeof(cgIntersection));
		intersection->distance = t;
		intersection->point = intersection_point;

		return intersection;
	}
	else{
		return NULL;
	}
}

int isIntersectionOnPolygon(cgPoint2f intersection, cgPolygon polygon){
	int borders_count = 0;

	cgPoint2f *moved_points = moveIntersectionToOrigin(intersection, polygon.points_count, polygon.points_2d);

	for(int i = 0; i < polygon.points_count; i++){
		cgPoint2f border_min = moved_points[i];
		cgPoint2f border_max;
		if(i == polygon.points_count - 1){
			border_max = moved_points[0];
		}
		else{
			border_max = moved_points[i + 1];
		}

		int both_x_are_positive = border_min.x >= 0 && border_max.x >= 0;
		int both_x_are_negative = border_min.x < 0 && border_max.x < 0;

		int both_y_are_positive = border_min.y >= 0 && border_max.y >= 0;
		int both_y_are_negative = border_min.y < 0 && border_max.y < 0;
		/* Trivial reject */
		if(both_x_are_negative && (both_y_are_negative || both_y_are_positive)){
			continue;
		}

		/* Trivial accept */

		if(both_x_are_positive && (!both_y_are_positive && !both_y_are_negative)){
			borders_count++;
			continue;
		}

		/* Complicated case */
		if(!both_x_are_positive && !both_x_are_negative && !both_y_are_positive && !both_y_are_negative){
			/* Get Segment equation */
			long double m = (border_max.y - border_min.y)/(border_max.x - border_min.x);
			long double b = border_max.y - m * border_max.x;

			/* Y = 0, calculate X */
			long double x = -b/m;

			if(x >= 0){
				borders_count++;
				continue;
			}
		}
	}

	return borders_count % 2;
}

cgPoint2f* moveIntersectionToOrigin(cgPoint2f intersection, int points_count, cgPoint2f* points){
	cgPoint2f *new_points = malloc(sizeof(cgPoint2f) * points_count);

	for(int i = 0; i < points_count; i++){
		new_points[i].x = points[i].x - intersection.x;
		new_points[i].y = points[i].y - intersection.y;
	}

	return new_points;
}