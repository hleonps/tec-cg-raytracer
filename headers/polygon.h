
typedef struct cgPolygon cgPolygon;

struct cgPolygon {
	cgPoint3f *points;
	int points_count;
	cgVector3f *normal_vector;
};

cgVector3f cgPolygonNormalVector(cgPoint dummy_point, void *information);
cgIntersection * cgPolygonIntersection(cgPoint3f camera, cgVector3f ray_direction, void * data);
