#include <math.h>
#include "vector.h"

long double cgVectorMagnitude(cg3DVector vector){
	return sqrt((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
}

cg3DVector cgNormalizedVector(cg3DVector vector, long double magnitude){
	long double normalized_x = (long double) vector.x / magnitude;
	long double normalized_y = (long double) vector.y / magnitude;
	long double normalized_z = (long double) vector.z / magnitude;

	cg3DVector normalized_vector = {normalized_x, normalized_y, normalized_z};

	return normalized_vector;
}

cg3DVector cgDirectionVector(cg3DPoint origin, cg3DPoint end){
	long double x_component = end.x - origin.x;
	long double y_component = end.y - origin.y;
	long double z_component = end.z - origin.z;

	cg3DVector direction_vector = {x_component, y_component, z_component};

	return direction_vector;
}

long double cgDotProduct(cg3DVector vectorA, cg3DVector vectorB){
	return (vectorA.x * vectorB.x) + (vectorA.y * vectorB.y) + (vectorA.z * vectorB.z);
}

cg3DVector cgCrossProduct(cg3DVector vectorA, cg3DVector vectorB){
	long double x_component = (vectorA.y * vectorB.z) - (vectorA.z * vectorB.y);
	long double y_component = (vectorA.z * vectorB.x) - (vectorA.x * vectorB.z);
	long double z_component = (vectorA.x * vectorB.y) - (vectorA.y * vectorB.x);

	cg3DVector vector_cross_product = {x_component, y_component, z_component};

	return vector_cross_product;
}
