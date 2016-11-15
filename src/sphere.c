#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "graphics.h"
#include "object.h"
#include "sphere.h"

#define MIN(X,Y) ((X < Y) ? X : Y)

extern const long double NO_INTERSECTION_T_VALUE;
extern const long double EPSILON;

cgVector3f cgSphereNormalVector(cgPoint3f intersection, void *information){
	cgSphere sphere_information = (*(cgSphere*) (information));

	cgVector3f normal_vector = cgDirectionVector(sphere_information.center, intersection);
	cgVector3f unit_vector = cgNormalizedVector(normal_vector, sphere_information.radius);

	return unit_vector;
}

cgIntersection * cgSphereIntersection(cgPoint3f camera, cgVector3f ray_direction, void * data){
	cgSphere sphere_information = (*(cgSphere*) (data));

	cgIntersection * intersection = NULL;
	cgVector3f intersection_direction =  cgDirectionVector(sphere_information.center, camera);

	long double beta = 2*(cgDotProduct(ray_direction, intersection_direction));
	long double delta = (intersection_direction.x * intersection_direction.x) + (intersection_direction.y * intersection_direction.y)
		+ (intersection_direction.z * intersection_direction.z) - (sphere_information.radius * sphere_information.radius);

	long double discriminant = (beta * beta) - (4 * delta);
	long double t_min = NO_INTERSECTION_T_VALUE;

	if(discriminant > EPSILON){
		long double discriminant_root = sqrt(discriminant);
		long double t1 = ((long double) -beta + discriminant_root) / 2;
		long double t2 = ((long double) -beta - discriminant_root) / 2;

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

		cgPoint3f intersection_point = {camera.x + (t_min * ray_direction.x),
			camera.y + (t_min * ray_direction.y), camera.z + (t_min * ray_direction.z)};

		intersection->point = intersection_point;
	}

	return intersection;
}

cgColor cgSphereTextureColor(cgAVS_t* texture, cgPoint3f intersection, void* data){
	cgSphere sphere_information = (*(cgSphere*) (data));

	cgVector3f intersection_to_center_vector = {
		.x = intersection.x - sphere_information.center.x,
		.y = (intersection.y - sphere_information.center.y),
		.z = intersection.z - sphere_information.center.z,
	};

	long double theta = acos(intersection_to_center_vector.y/sphere_information.radius);
    long double phi = atan2(intersection_to_center_vector.x, intersection_to_center_vector.z);

    if(phi < 0.0){
    	phi += 2*PI;
    }
        
    long double u = phi/(2*PI);
    long double v = (PI - theta)/PI;

    int j = (int)((texture->width  - 1) * u); 
    int i = (int)((texture->height - 1) * (1 - v));

	cgAVS_Pixel texel = texture->data[i][j];
	
	cgColor color = {.r = texel.r/255.0, .g = texel.g/255.0, .b = texel.b/255.0};

	return color;
}
