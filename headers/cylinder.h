
typedef struct cgCylinder cgCylinder;
typedef cgVector3f (*cgNormalVectorCylinder)(cgPoint3f,void*);
typedef cgColor (*cgTextureColorCylinder)(cgAVS_t*, cgPoint3f, void*);

struct cgCylinder{
	long double radius;
	cgVector3f direction;
	cgPoint3f anchor;
	long double distance_a;
	long double distance_b;
	cgVector3f *texture_start;
};

cgIntersection * cgCylinderIntersection(cgPoint3f eye, cgVector3f ray_direction, void * information);
cgVector3f cgCylinderNormalVector(cgPoint3f point, void * information);
cgColor cgCylinderTextureColor(cgAVS_t* texture, cgPoint3f intersection, void* data);