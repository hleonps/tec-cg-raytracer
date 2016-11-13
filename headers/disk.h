
typedef struct cgDisk cgDisk;
typedef cgVector3f (*cgNormalVectorDisk)(void*);

struct cgDisk {
	cgPoint3f center;
	long double inner_radius;
	long double outer_radius;
	cgPoint3f *plane_points;
	cgVector3f *normal_vector;
};

cgVector3f cgDiskNormalVector(void *information);
cgIntersection * cgDiskIntersection(cgPoint3f camera, cgVector3f ray_direction, void * data);
