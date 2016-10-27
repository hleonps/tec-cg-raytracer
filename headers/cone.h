
typedef struct cgCone cgCone;
typedef cgVector3f (*cgNormalVectorCone)(cgPoint3f,void*);

struct cgCone {
	long double radius_k;
	long double distance_k;
	cgVector3f direction;
	cgPoint3f anchor;
	long double distance_a;
	long double distance_b;
};

cgVector3f cgConeNormalVector(cgPoint3f intersection, void *information);
cgIntersection * cgConeIntersection(cgPoint3f eye, cgVector3f ray_direction, void *information);
