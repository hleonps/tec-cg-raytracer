#include <stdlib.h>
#include <math.h>
#include "vector.h"
#include "graphics.h"
#include "object.h"
#include "cylinder.h"

#define MIN(X,Y) ((X < Y) ? X : Y)

extern const long double EPSILON;
extern const long double NO_INTERSECTION_T_VALUE;

cgIntersection * cgCylinderIntersection(cgPoint3f eye, cgVector3f ray_direction, void * information){
	cgCylinder cylinder_information = (*(cgCylinder*) (information));
	cgIntersection * intersection = NULL;

	cgVector3f vector_q = cylinder_information.direction;
	cgPoint3f anchor = cylinder_information.anchor;

	long double alpha = (cgDotProduct(vector_q, vector_q) - 2) * powl(cgDotProduct(vector_q, ray_direction), 2)
		+ powl(ray_direction.x, 2) + powl(ray_direction.y, 2) + powl(ray_direction.z, 2);

	long double beta = 2
		* ( ( (vector_q.x * cgDotProduct(ray_direction, vector_q) - ray_direction.x)
		* ( (anchor.x - eye.x) * (1 - powl(vector_q.x, 2)) + vector_q.x
		* (vector_q.y * (eye.y - anchor.y) + vector_q.z * (eye.z - anchor.z) ) ) )
		+ ( (vector_q.y * cgDotProduct(vector_q, ray_direction) - ray_direction.y)
		* ( (anchor.y - eye.y) * (1 - powl(vector_q.y, 2)) + vector_q.y
		* (vector_q.x * (eye.x - anchor.x) + vector_q.z * (eye.z - anchor.z) ) ) )
		+ ( (vector_q.z * cgDotProduct(vector_q, ray_direction) - ray_direction.z)
		* ( (anchor.z - eye.z) * (1 - powl(vector_q.z, 2)) + vector_q.z
		* (vector_q.x * (eye.x - anchor.x) + vector_q.y * (eye.y - anchor.y) ) ) ) );

	long double delta = powl((anchor.x - eye.x) * (1 - powl(vector_q.x, 2))
 		+ vector_q.x * ((eye.y * vector_q.y) - (anchor.y * vector_q.y) + (eye.z * vector_q.z) - (anchor.z * vector_q.z)), 2)
		+ powl((anchor.y - eye.y) * (1 - powl(vector_q.y, 2))
	 	+ vector_q.y * ((eye.x * vector_q.x) - (anchor.x * vector_q.x) + (eye.z * vector_q.z) - (anchor.z * vector_q.z)), 2)
		+ powl((anchor.z - eye.z) * (1 - powl(vector_q.z, 2))
	 	+ vector_q.z * ((eye.x * vector_q.x) - (anchor.x * vector_q.x) + (eye.y * vector_q.y) - (anchor.y * vector_q.y)), 2)
		- powl(cylinder_information.radius, 2);

	long double discriminant = (beta * beta) - (4 * alpha * delta);
	long double t_min = NO_INTERSECTION_T_VALUE;

	if(discriminant > EPSILON){
		long double discriminant_root = sqrtl(discriminant);
		long double t1 = ((long double) -beta + discriminant_root) / (2 * alpha);
		long double t2 = ((long double) -beta - discriminant_root) / (2 * alpha);

		if(t1 > EPSILON && t2 > EPSILON) {
			t_min = MIN(t1, t2);
		}
		else if(t1 > EPSILON){
			t_min = t1;
		}
		else if(t2 > EPSILON){
			t_min = t2;
		}
	}

	if(t_min > (NO_INTERSECTION_T_VALUE + EPSILON)){
		intersection = (cgIntersection *) malloc(sizeof(cgIntersection));

		intersection->distance = t_min;

		cgPoint3f intersection_point = {eye.x + (t_min * ray_direction.x),
			eye.y + (t_min * ray_direction.y), eye.z + (t_min * ray_direction.z)};

		intersection->point = intersection_point;
	}

	return intersection;
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

	cgVector3f unit_vector = cgNormalizedVector(normal_vector, cgVectorMagnitude(normal_vector));

	return unit_vector;
}
