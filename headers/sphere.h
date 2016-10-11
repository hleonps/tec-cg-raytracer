
typedef struct cgSphere cgSphere;

struct cgSphere {
	long double radius;
	cgPoint3f center;
};

cgVector3f cgSphereNormalVector(cgPoint3f intersection, void *information);
cgIntersection * cgSphereIntersection(cgPoint3f camera, cgVector3f ray_direction, void * data);
