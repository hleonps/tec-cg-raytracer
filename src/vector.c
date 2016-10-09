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
