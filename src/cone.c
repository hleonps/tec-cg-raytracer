#include <stdlib.h>
#include <math.h>
#include "vector.h"
#include "graphics.h"
#include "object.h"
#include "cone.h"

int cgIsInsideFiniteCone(cgPoint3f intersection_point, cgCone information);

#define MIN(X,Y) ((X < Y) ? X : Y)
#define MAX(X,Y) ((X > Y) ? X : Y)

extern const long double EPSILON;
extern const long double NO_INTERSECTION_T_VALUE;

cgIntersection * cgConeIntersection(cgPoint3f anchor, cgVector3f ray_direction, cgObject * cone){
	cgCone cone_information = (*(cgCone*) (cone->data));
	cgIntersection * intersection = NULL;

	cgVector3f vector_q = cone_information.direction;
	cgPoint3f cone_anchor = cone_information.anchor;
	long double radius_k = cone_information.radius_k;
	long double distance_k = cone_information.distance_k;

	long double alpha = (cgDotProduct(vector_q, vector_q) - 2) * powl(cgDotProduct(vector_q, ray_direction), 2)
		+ powl(ray_direction.x, 2) + powl(ray_direction.y, 2) + powl(ray_direction.z, 2)
		- powl((radius_k / distance_k) * cgDotProduct(ray_direction, vector_q), 2);

	long double beta = 2
		* ( ( (vector_q.x * cgDotProduct(ray_direction, vector_q) - ray_direction.x)
		* ( (cone_anchor.x - anchor.x) * (1 - powl(vector_q.x, 2)) + vector_q.x
		* (vector_q.y * (anchor.y - cone_anchor.y) + vector_q.z * (anchor.z - cone_anchor.z) ) ) )
		+ ( (vector_q.y * cgDotProduct(vector_q, ray_direction) - ray_direction.y)
		* ( (cone_anchor.y - anchor.y) * (1 - powl(vector_q.y, 2)) + vector_q.y
		* (vector_q.x * (anchor.x - cone_anchor.x) + vector_q.z * (anchor.z - cone_anchor.z) ) ) )
		+ ( (vector_q.z * cgDotProduct(vector_q, ray_direction) - ray_direction.z)
		* ( (cone_anchor.z - anchor.z) * (1 - powl(vector_q.z, 2)) + vector_q.z
		* (vector_q.x * (anchor.x - cone_anchor.x) + vector_q.y * (anchor.y - cone_anchor.y) ) ) )
	 	- (powl(radius_k / distance_k, 2) * cgDotProduct(ray_direction, vector_q)
		* ((anchor.x * vector_q.x) + (anchor.y * vector_q.y) + (anchor.z * vector_q.z) - ((cone_anchor.x * vector_q.x) + (cone_anchor.y * vector_q.y)
	 	+ (cone_anchor.z * vector_q.z)) ) ) );

	long double delta = powl((cone_anchor.x - anchor.x) * (1 - powl(vector_q.x, 2))
 		+ vector_q.x * ((anchor.y * vector_q.y) - (cone_anchor.y * vector_q.y) + (anchor.z * vector_q.z) - (cone_anchor.z * vector_q.z)), 2)
		+ powl((cone_anchor.y - anchor.y) * (1 - powl(vector_q.y, 2))
	 	+ vector_q.y * ((anchor.x * vector_q.x) - (cone_anchor.x * vector_q.x) + (anchor.z * vector_q.z) - (cone_anchor.z * vector_q.z)), 2)
		+ powl((cone_anchor.z - anchor.z) * (1 - powl(vector_q.z, 2))
	 	+ vector_q.z * ((anchor.x * vector_q.x) - (cone_anchor.x * vector_q.x) + (anchor.y * vector_q.y) - (cone_anchor.y * vector_q.y)), 2)
		- powl((radius_k / distance_k) * ((anchor.x * vector_q.x) + (anchor.y * vector_q.y) + (anchor.z * vector_q.z) - ((cone_anchor.x * vector_q.x) + (cone_anchor.y * vector_q.y)
	 	+ (cone_anchor.z * vector_q.z)) ), 2);

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

	if(first_t > EPSILON && cgIsInsideFiniteCone(first_point, cone_information)){
		t = first_t;
		point_t = first_point;
	}
	else if(second_t > EPSILON && cgIsInsideFiniteCone(second_point, cone_information)){
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

int cgIsInsideFiniteCone(cgPoint3f intersection_point, cgCone information){
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

cgVector3f cgConeNormalVector(cgPoint3f point, void * information){
	cgCone cone_information = (*(cgCone*) (information));

	cgPoint3f anchor = cone_information.anchor;
	cgVector3f direction = cone_information.direction;

	cgVector3f h = cgDirectionVector(anchor, point);
	long double distance_m = cgDotProduct(h, direction);

	cgPoint3f point_m = {
		anchor.x + (distance_m * direction.x),
		anchor.y + (distance_m * direction.y),
		anchor.z + (distance_m * direction.z)
	};

	cgVector3f normal_vector = cgDirectionVector(point_m, point);
	cgVector3f unit_vector = cgNormalizedVector(normal_vector, cgVectorMagnitude(normal_vector));

	return unit_vector;
}

cgColor cgConeTextureColor(cgAVS_t* texture, cgPoint3f intersection, void* data){
	cgCone cone_information = (*(cgCone*) (data));
	cgVector3f intersection_vector = {
		.x = intersection.x - cone_information.anchor.x,
		.y = intersection.y - cone_information.anchor.y,
		.z = intersection.z - cone_information.anchor.z
	};

	long double H = fabsl(cone_information.distance_b - cone_information.distance_a);
	long double v = fabsl(cgDotProduct(cone_information.direction, intersection_vector))/H;

	v = (v > 1) ? 1.0 : v; /* Temporary fix for small cones */

	cgVector3f normal_vector = cgConeNormalVector(intersection, data);

	cgVector3f g = *cone_information.texture_start;
	long double u = acos(cgDotProduct(normal_vector, g))/(2.0*PI);

	cgVector3f q = cone_information.direction;
	cgVector3f plane_normal = cgCrossProduct(g, q);
	long double d = -plane_normal.x * q.x - plane_normal.y*q.y - plane_normal.z*q.z;

	long double result = plane_normal.x*intersection.x + plane_normal.y*intersection.y +
		plane_normal.z*intersection.z + d;

	if(result < 0){
		u = 1 - u;
	}
	int i = texture->height * (1 - v);
	int j = texture->width* u;

	cgAVS_Pixel texel = texture->data[i][j];

	cgColor color = {.r = texel.r/255.0, .g = texel.g/255.0, .b = texel.b/255.0};

	return color;
}
