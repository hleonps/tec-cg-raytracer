
typedef struct cgIntersection * (*cgIntersectionFunction)(cgPoint3f,cgVector3f,void*);
typedef cgVector3f (*cgNormalVector)(void);
typedef struct cgObject cgObject;
typedef struct cgIntersection cgIntersection;
typedef struct cgObjectList cgObjectList;

typedef enum {
	SPHERE,
	POLYGON,
	CYLINDER
} cgObjectType;

struct cgObject {
	cgObjectType type;
	cgColor color;
	void * data;
	cgIntersectionFunction intersection;
	cgNormalVector normal_vector;
	long double diffuse_factor;
	long double specular_factor;
	int specular_focus;
	long double environment_lighting;
	long double reflection_factor;
	long double transparency_factor;
};

struct cgIntersection{
	long double distance;
	cgPoint3f point;
	cgObject object;
};
