
typedef struct cgScene cgScene;

struct cgScene {
	cgObject * objects;
	unsigned int num_objects;
};

void cgAddSphereToScene(cgPoint3f center, long double radius, cgColor color);
cgIntersection * first_intersection(cgPoint3f camera, cgVector3f ray_direction);
