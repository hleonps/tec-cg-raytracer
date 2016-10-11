#include "vector.h"
#include "light_source.h"

#define MIN(X,Y) ((X < Y) ? X : Y)

long double cgAttenuationFactor(cgLight * light_source, long double distance){
	long double attenuation_factor = ((long double) 1.0)/
		(light_source->c1 + (light_source->c2 * distance) + (light_source->c3 * (distance * distance)));

	return MIN(1.0, attenuation_factor);
}
