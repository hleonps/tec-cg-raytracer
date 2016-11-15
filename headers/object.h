#include <stdio.h>
#include "avs.h"

typedef struct cgObject cgObject;
typedef struct cgIntersection cgIntersection;
typedef struct cgObjectList cgObjectList;
typedef struct cgCuttingPlane cgCuttingPlane;

typedef cgIntersection * (*cgIntersectionFunction)(cgPoint3f,cgVector3f, cgObject *);
typedef cgVector3f (*cgNormalVector)(void);
typedef cgColor (*cgTextureColor)(void);

typedef enum {
	SPHERE,
	POLYGON,
	CYLINDER,
	CONE,
	DISK
} cgObjectType;

typedef enum{
	ABOVE,
	BELOW
} cgCuttingDirection;

struct cgObject {
	cgObjectType type;
	cgColor color;
	void * data;
	cgIntersectionFunction intersection;
	cgNormalVector normal_vector;
	cgTextureColor texture_color;
	long double diffuse_factor;
	long double specular_factor;
	int specular_focus;
	long double environment_lighting;
	long double reflection_factor;
	long double transparency_factor;
	cgAVS_t* texture;
	cgCuttingPlane * cutting_planes;
};

struct cgIntersection{
	long double distance;
	cgPoint3f point;
	cgObject object;
};

struct cgCuttingPlane {
	cgPoint3f * points;
	size_t points_count;
	long double A;
	long double B;
	long double C;
	long double D;
	cgCuttingDirection direction;
};
