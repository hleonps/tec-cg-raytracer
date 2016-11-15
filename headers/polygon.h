typedef struct cgPolygon cgPolygon;
typedef struct cgPolygonTexture cgPolygonTexture;
typedef cgVector3f (*cgNormalVectorPolygon)(void*);
typedef cgColor (*cgTextureColorPolygon)(cgAVS_t*, cgPoint3f, void*);

struct cgPolygon {
	cgPoint3f *points_3d;
	cgPolygonTexture *texture;
	cgPoint2f *points_2d;
	cgVector3f *normal_vector;
	cgCoordinate removed_coordinate;
	int points_count;
};

struct cgPolygonTexture {
	cgPoint3f *rectangle;
	long double height;
	long double width;
};

cgVector3f cgPolygonNormalVector(void *information);
cgIntersection * cgPolygonIntersection(cgPoint3f camera, cgVector3f ray_direction, void * data);
cgColor cgPolygonTextureColor(cgAVS_t* texture, cgPoint3f intersection, void* data);