
typedef struct cgPolygon cgPolygon;
typedef cgVector3f (*cgNormalVectorPolygon)(void*);

struct cgPolygon {
	cgPoint3f *points_3d;
	cgPoint2f *points_2d;
	cgVector3f *normal_vector;
	cgCoordinate removed_coordinate;
	int points_count;
};

cgVector3f cgPolygonNormalVector(void *information);
cgIntersection * cgPolygonIntersection(cgPoint3f camera, cgVector3f ray_direction, void * data);
