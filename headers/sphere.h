
typedef struct cgSphere cgSphere;

struct cgSphere {
	long double radius;
	cgPoint3f center;
};

cgIntersection * cgSphereIntersection(cgPoint3f camera, cgVector3f ray_direction, void * data);
