
typedef struct cgCylinder cgCylinder;
typedef cgVector3f (*cgNormalVectorCylinder)(cgPoint3f,void*);

struct cgCylinder{
	long double radius;
	cgVector3f direction;
	cgPoint3f anchor;
};

cgIntersection * cgCylinderIntersection(cgPoint3f eye, cgVector3f ray_direction, void * information);
cgVector3f cgCylinderNormalVector(cgPoint3f point, void * information);
