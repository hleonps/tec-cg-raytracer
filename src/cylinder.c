#include <stdlib.h>
#include <math.h>
#include "vector.h"
#include "graphics.h"
#include "object.h"
#include "cylinder.h"

int cgIsInsideFiniteCylinder(cgPoint3f intersection_point, cgCylinder information);

#define MIN(X,Y) ((X < Y) ? X : Y)
#define MAX(X,Y) ((X > Y) ? X : Y)

extern const long double EPSILON;
extern const long double NO_INTERSECTION_T_VALUE;

cgIntersection * cgCylinderIntersection(cgPoint3f anchor, cgVector3f ray_direction, cgObject * cylinder){
	cgCylinder cylinder_information = (*(cgCylinder*) (cylinder->data));
	cgIntersection * intersection = NULL;

	cgVector3f vector_q = cylinder_information.direction;
	cgPoint3f cylinder_anchor = cylinder_information.anchor;

	long double alpha = (cgDotProduct(vector_q, vector_q) - 2) * powl(cgDotProduct(vector_q, ray_direction), 2)
		+ powl(ray_direction.x, 2) + powl(ray_direction.y, 2) + powl(ray_direction.z, 2);

	long double beta = 2
		* ( ( (vector_q.x * cgDotProduct(ray_direction, vector_q) - ray_direction.x)
		* ( (cylinder_anchor.x - anchor.x) * (1 - powl(vector_q.x, 2)) + vector_q.x
		* (vector_q.y * (anchor.y - cylinder_anchor.y) + vector_q.z * (anchor.z - cylinder_anchor.z) ) ) )
		+ ( (vector_q.y * cgDotProduct(vector_q, ray_direction) - ray_direction.y)
		* ( (cylinder_anchor.y - anchor.y) * (1 - powl(vector_q.y, 2)) + vector_q.y
		* (vector_q.x * (anchor.x - cylinder_anchor.x) + vector_q.z * (anchor.z - cylinder_anchor.z) ) ) )
		+ ( (vector_q.z * cgDotProduct(vector_q, ray_direction) - ray_direction.z)
		* ( (cylinder_anchor.z - anchor.z) * (1 - powl(vector_q.z, 2)) + vector_q.z
		* (vector_q.x * (anchor.x - cylinder_anchor.x) + vector_q.y * (anchor.y - cylinder_anchor.y) ) ) ) );

	long double delta = powl((cylinder_anchor.x - anchor.x) * (1 - powl(vector_q.x, 2))
 		+ vector_q.x * ((anchor.y * vector_q.y) - (cylinder_anchor.y * vector_q.y) + (anchor.z * vector_q.z) - (cylinder_anchor.z * vector_q.z)), 2)
		+ powl((cylinder_anchor.y - anchor.y) * (1 - powl(vector_q.y, 2))
	 	+ vector_q.y * ((anchor.x * vector_q.x) - (cylinder_anchor.x * vector_q.x) + (anchor.z * vector_q.z) - (cylinder_anchor.z * vector_q.z)), 2)
		+ powl((cylinder_anchor.z - anchor.z) * (1 - powl(vector_q.z, 2))
	 	+ vector_q.z * ((anchor.x * vector_q.x) - (cylinder_anchor.x * vector_q.x) + (anchor.y * vector_q.y) - (cylinder_anchor.y * vector_q.y)), 2)
		- powl(cylinder_information.radius, 2);

	long double discriminant = (beta * beta) - (4 * alpha * delta);
	long double t = NO_INTERSECTION_T_VALUE;
	long double first_t = NO_INTERSECTION_T_VALUE;
	long double second_t = NO_INTERSECTION_T_VALUE;
	cgPoint3f point_t;

	if(discriminant > EPSILON){
		long double discriminant_root = sqrtl(discriminant);
		long double t1 = ((long double) -beta + discriminant_root) / (2 * alpha);
		long double t2 = ((long double) -beta - discriminant_root) / (2 * alpha);

		if(t1 > EPSILON && t2 > EPSILON) {
			first_t = MIN(t1, t2);
			second_t = MAX(t1, t2);
		}
		else if(t1 > EPSILON){
			first_t = t1;
		}
		else if(t2 > EPSILON){
			first_t = t2;
		}
	}

	cgPoint3f first_point = {
		anchor.x + (first_t * ray_direction.x),
		anchor.y + (first_t * ray_direction.y),
		anchor.z + (first_t * ray_direction.z)
	};

	cgPoint3f second_point = {
		anchor.x + (second_t * ray_direction.x),
		anchor.y + (second_t * ray_direction.y),
		anchor.z + (second_t * ray_direction.z)
	};

	if(first_t > EPSILON && cgIsInsideFiniteCylinder(first_point, cylinder_information)){
		t = first_t;
		point_t = first_point;
	}
	else if(second_t > EPSILON && cgIsInsideFiniteCylinder(second_point, cylinder_information)){
		t = second_t;
		point_t = second_point;
	}

	if(t > (NO_INTERSECTION_T_VALUE + EPSILON)){
		intersection = (cgIntersection *) malloc(sizeof(cgIntersection));

		intersection->distance = t;

		cgPoint3f intersection_point = point_t;

		intersection->point = intersection_point;
	}

	return intersection;
}

int cgIsInsideFiniteCylinder(cgPoint3f intersection_point, cgCylinder information){
	cgPoint3f anchor = information.anchor;
	cgVector3f direction = information.direction;

	cgVector3f h = cgDirectionVector(anchor, intersection_point);
	long double distance_m = cgDotProduct(h, direction);

	long double d_min, d_max;

	d_min = MIN(information.distance_a, information.distance_b);
	d_max = MAX(information.distance_a, information.distance_b);

	if(d_min <= distance_m && distance_m <= d_max){
		return 1;
	}

	return 0;
}

cgVector3f cgCylinderNormalVector(cgPoint3f point, void * information){
	cgCylinder cylinder_information = (*(cgCylinder*) (information));

	cgPoint3f anchor = cylinder_information.anchor;
	cgVector3f direction = cylinder_information.direction;
	long double radius = cylinder_information.radius;

	cgVector3f h = cgDirectionVector(anchor, point);
	long double distance_m = cgDotProduct(h, direction);

	cgPoint3f point_m = {
		anchor.x + (distance_m * direction.x),
		anchor.y + (distance_m * direction.y),
		anchor.z + (distance_m * direction.z)
	};

	cgVector3f normal_vector = {
		(point.x - point_m.x)/radius,
		(point.y - point_m.y)/radius,
		(point.z - point_m.z)/radius
	};

	return normal_vector;
}

cgColor cgCylinderTextureColor(cgAVS_t* texture, cgPoint3f intersection, void* data){
	cgCylinder cylinder_information = (*(cgCylinder*) (data));
	cgVector3f intersection_vector = {
		.x = intersection.x - cylinder_information.anchor.x,
		.y = intersection.y - cylinder_information.anchor.y,
		.z = intersection.z - cylinder_information.anchor.z
	};

	long double H = fabsl(cylinder_information.distance_b - cylinder_information.distance_a);
	long double v = cgDotProduct(cylinder_information.direction, intersection_vector)/H;

	v = (v > 1) ? 1.0 : v; /* Temporary fix for small cylinders */

	cgVector3f normal_vector = cgCylinderNormalVector(intersection, data);

	cgVector3f g = *cylinder_information.texture_start;
	long double u = acos(cgDotProduct(normal_vector, g))/(2.0*PI);

	cgVector3f q = cylinder_information.direction;
	cgVector3f plane_normal = cgCrossProduct(g, q);
	long double d = -plane_normal.x * q.x - plane_normal.y*q.y - plane_normal.z*q.z;

	long double result = plane_normal.x*intersection.x + plane_normal.y*intersection.y +
		plane_normal.z*intersection.z + d;

	if(result < 0){
		u = 1 - u;
	}
	int i = texture->height * (1 - v);
	int j = texture->width * u;

	cgAVS_Pixel texel = texture->data[i][j];

	cgColor color = {.r = texel.r/255.0, .g = texel.g/255.0, .b = texel.b/255.0};

	return color;
}
