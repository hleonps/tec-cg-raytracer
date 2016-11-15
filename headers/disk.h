typedef struct cgDisk cgDisk;
typedef struct cgDiskTexture cgDiskTexture;
typedef cgVector3f (*cgNormalVectorDisk)(void*);
typedef cgColor (*cgTextureColorDisk)(cgAVS_t*, cgPoint3f, void*);

struct cgDisk {
	cgPoint3f center;
	long double inner_radius;
	long double outer_radius;
	cgPoint3f *plane_points;
	cgVector3f *normal_vector;
	cgDiskTexture *texture;
};

struct cgDiskTexture {
	cgPoint3f *rectangle;
	long double height;
	long double width;
};

cgVector3f cgDiskNormalVector(void *information);
cgIntersection * cgDiskIntersection(cgPoint3f anchor, cgVector3f ray_direction, cgObject * disk);
cgColor cgDiskTextureColor(cgAVS_t* texture, cgPoint3f intersection, void* data);
