#include <math.h>
#include "vector.h"

long double cgVectorMagnitude(cgVector3f vector){
	return sqrt((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
}

cgVector3f cgNormalizedVector(cgVector3f vector, long double magnitude){
	long double normalized_x = (long double) vector.x / magnitude;
	long double normalized_y = (long double) vector.y / magnitude;
	long double normalized_z = (long double) vector.z / magnitude;

	cgVector3f normalized_vector = {normalized_x, normalized_y, normalized_z};

	return normalized_vector;
}

cgVector3f cgDirectionVector(cgPoint3f origin, cgPoint3f end){
	long double x_component = end.x - origin.x;
	long double y_component = end.y - origin.y;
	long double z_component = end.z - origin.z;

	cgVector3f direction_vector = {x_component, y_component, z_component};

	return direction_vector;
}

long double cgDotProduct(cgVector3f vectorA, cgVector3f vectorB){
	return (vectorA.x * vectorB.x) + (vectorA.y * vectorB.y) + (vectorA.z * vectorB.z);
}

cgVector3f cgCrossProduct(cgVector3f vectorA, cgVector3f vectorB){
	long double x_component = (vectorA.y * vectorB.z) - (vectorA.z * vectorB.y);
	long double y_component = (vectorA.z * vectorB.x) - (vectorA.x * vectorB.z);
	long double z_component = (vectorA.x * vectorB.y) - (vectorA.y * vectorB.x);

	cgVector3f vector_cross_product = {x_component, y_component, z_component};

	return vector_cross_product;
}
