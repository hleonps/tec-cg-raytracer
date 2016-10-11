
typedef struct cgLight cgLight;

struct cgLight {
	cgPoint3f position;
	long double intensity;
	long double c1;
	long double c2;
	long double c3;
};


long double cgAttenuationFactor(cgLight * light_source, long double distance);
