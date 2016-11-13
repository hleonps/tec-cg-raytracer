#include "light_source.h"

typedef struct cgScene cgScene;

struct cgScene {
	cgObject * objects;
	unsigned int num_objects;
	cgLight * lights;
	unsigned int num_lights;
	long double environment_lighting;
};

cgScene scene;

void cgAddSphereToScene(cgObject sphere);
void cgAddPolygonToScene(cgObject polygon);
void cgAddCylinderToScene(cgObject cylinder);
void cgAddConeToScene(cgObject cone);
void cgAddDiskToScene(cgObject disk);
void cgAddLightSourceToScene(cgLight light_source);
void cgSetEnvironmentLighting(long double intensity);
cgIntersection * cgFirstIntersection(cgPoint3f camera, cgVector3f ray_direction);
