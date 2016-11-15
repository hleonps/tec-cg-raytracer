#define PI 3.14159265
typedef struct cgSphere cgSphere;
typedef cgVector3f (*cgNormalVectorSphere)(cgPoint3f,void*);
typedef cgColor (*cgTextureColorSphere)(cgAVS_t*, cgPoint3f, void*);

struct cgSphere {
	long double radius;
	cgPoint3f center;
	cgVector3f *texture_start;
};

cgVector3f cgSphereNormalVector(cgPoint3f intersection, void *information);
cgIntersection * cgSphereIntersection(cgPoint3f anchor, cgVector3f ray_direction, cgObject * sphere);
cgColor cgSphereTextureColor(cgAVS_t* texture, cgPoint3f intersection, void* data);
