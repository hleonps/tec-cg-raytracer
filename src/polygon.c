#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "graphics.h"
#include "object.h"
#include "polygon.h"

int cgIsIntersectionInsidePolygon(cgPoint2f intersection, cgPolygon polygon);
cgPoint2f* cgMoveIntersectionToOrigin(cgPoint2f intersection, int points_count, cgPoint2f* points);

cgVector3f cgPolygonNormalVector(void *information){
	cgPolygon polygon_information = (*(cgPolygon*) (information));

	// If it was calculated earlier, use the stored vector
	if(polygon_information.normal_vector != NULL){
		return *polygon_information.normal_vector;
	}

	cgPoint3f point_a = polygon_information.points_3d[0];
	cgPoint3f point_b = polygon_information.points_3d[1];
	cgPoint3f point_c = polygon_information.points_3d[2];

	cgVector3f vector_a = cgDirectionVector(point_a, point_b);
	cgVector3f vector_b = cgDirectionVector(point_a, point_c);

	cgVector3f normal_vector = cgCrossProduct(vector_a, vector_b);
	cgVector3f unit_vector = cgNormalizedVector(normal_vector, cgVectorMagnitude(normal_vector));

	// Optimization. Store normal vector
	((cgPolygon *) information)->normal_vector = (cgVector3f *) malloc(sizeof(cgVector3f));
	((cgPolygon *) information)->normal_vector->x = unit_vector.x;
	((cgPolygon *) information)->normal_vector->y = unit_vector.y;
	((cgPolygon *) information)->normal_vector->z = unit_vector.z;

	return unit_vector;
}

cgIntersection * cgPolygonIntersection(cgPoint3f anchor, cgVector3f ray_direction, cgObject * polygon){
	cgPolygon polygon_information = (*(cgPolygon*) (polygon->data));

	cgIntersection * intersection = NULL;

	cgPoint3f polygon_point = polygon_information.points_3d[0];
	cgVector3f normal_vector = cgPolygonNormalVector(polygon->data);

	long double a = normal_vector.x, b = normal_vector.y,  c = normal_vector.z;

	/* Plane equation is required to get D, normal vector is [A, B, C]
		Ax + By + Cz + D = 0 */
	long double d = -(a * polygon_point.x + b * polygon_point.y + c * polygon_point.z);
	long double denominator = a * ray_direction.x + b * ray_direction.y + c * ray_direction.z;

	if(denominator == 0){
		return intersection;
	}

	long double t = -(a * anchor.x + b * anchor.y + c * anchor.z + d)/denominator;

	cgPoint3f intersection_point;
	intersection_point.x = anchor.x + t * ray_direction.x;
	intersection_point.y = anchor.y + t * ray_direction.y;
	intersection_point.z = anchor.z + t * ray_direction.z;

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

	if(cgCanUseIntersectionPoint(&intersection_point, polygon) && cgIsIntersectionInsidePolygon(intersection_2d, polygon_information)){
		intersection = malloc(sizeof(cgIntersection));
		intersection->distance = t;
		intersection->point = intersection_point;
	}

	return intersection;
}

int cgIsIntersectionInsidePolygon(cgPoint2f intersection, cgPolygon polygon){
	int borders_count = 0;
	cgPoint2f *moved_points = cgMoveIntersectionToOrigin(intersection, polygon.points_count, polygon.points_2d);

	for(int i = 0; i < polygon.points_count; i++){
		cgPoint2f border_min = moved_points[i];
		cgPoint2f border_max = (i == polygon.points_count - 1) ? moved_points[0] : moved_points[i + 1];

		int both_x_are_positive = border_min.x >= 0 && border_max.x >= 0;
		int both_x_are_negative = border_min.x < 0 && border_max.x < 0;

		int both_y_are_positive = border_min.y >= 0 && border_max.y >= 0;
		int both_y_are_negative = border_min.y < 0 && border_max.y < 0;

		int y_are_different = (border_min.y >= 0 && border_max.y < 0) || (border_min.y < 0 && border_max.y >= 0);
		int x_are_different = (border_min.x >= 0 && border_max.x < 0) || (border_min.x < 0 && border_max.x >= 0);

		/* Trivial reject */
		if(both_x_are_negative || both_y_are_negative || both_y_are_positive){
			continue;
		}

		/* Trivial accept */
		if(both_x_are_positive && y_are_different){
			borders_count++;
			continue;
		}

		/* Complicated case */
		if(y_are_different && x_are_different){
			/* Get Segment equation */
			long double m = (border_max.y - border_min.y)/(border_max.x - border_min.x);
			long double b = border_max.y - m * border_max.x;

			/* Y = 0, calculate X */
			long double x = -b/m;

			if(x >= 0){
				borders_count++;
			}

			continue;
		}
	}

	free(moved_points);

	return borders_count % 2;
}

cgPoint2f* cgMoveIntersectionToOrigin(cgPoint2f intersection, int points_count, cgPoint2f* points){
	cgPoint2f *new_points = (cgPoint2f*) malloc(sizeof(cgPoint2f) * points_count);

	for(int i = 0; i < points_count; i++){
		new_points[i].x = points[i].x - intersection.x;
		new_points[i].y = points[i].y - intersection.y;
	}

	return new_points;
}

cgColor cgPolygonTextureColor(cgAVS_t* texture, cgPoint3f intersection, void* data){
	cgPolygon information = (*(cgPolygon*) (data));
	long double H = information.texture->width;
	long double L = information.texture->height;

	cgVector3f a_vector = {
		.x = intersection.x - information.texture->rectangle[0].x,
		.y = intersection.y - information.texture->rectangle[0].y,
		.z = intersection.z - information.texture->rectangle[0].z
	};

	cgVector3f U_vector = {
		.x = (information.texture->rectangle[1].x - information.texture->rectangle[0].x)/H,
		.y = (information.texture->rectangle[1].y - information.texture->rectangle[0].y)/H,
		.z = (information.texture->rectangle[1].z - information.texture->rectangle[0].z)/H
	};

	cgVector3f V_vector = {
		.x = (information.texture->rectangle[3].x - information.texture->rectangle[0].x)/L,
		.y = (information.texture->rectangle[3].y - information.texture->rectangle[0].y)/L,
		.z = (information.texture->rectangle[3].z - information.texture->rectangle[0].z)/L
	};

	long double u = cgDotProduct(a_vector, U_vector)/H;
	long double v = cgDotProduct(a_vector, V_vector)/L;

	if(v < 0){
		v = 1 + v;
	}

	int i = texture->height * v;
	int j = texture->width * u;

	cgAVS_Pixel texel = texture->data[i][j];
	

	cgColor color = {.r = texel.r/255.0, .g = texel.g/255.0, .b = texel.b/255.0};
	return color;
}
