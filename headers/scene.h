
typedef struct cgScene cgScene;

struct cgScene {
	cgObject * objects;
	unsigned int num_objects;
};

cgIntersection * first_intersection(cgPoint3f camera, cgVector3f ray_direction);
