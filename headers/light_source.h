
typedef struct cgLight cgLight;

struct cgLight {
	cgPoint3f position;
	long double intensity;
	long double c1;
	long double c2;
	long double c3;
};

long double cgAttenuationFactor(cgLight * light_source, long double distance);
long double cgSpecularIntensity(long double dot_product, long double specular_factor, int specular_focus, long double light_intensity);
long double cgDiffuseIntensity(long double dot_product, long double diffuse_factor, long double light_intensity);
