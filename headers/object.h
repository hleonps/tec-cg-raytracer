
typedef struct cgIntersection * (*cgIntersectionFunction)(cgPoint3f,cgVector3f,void*);
typedef struct cgObject cgObject;
typedef struct cgIntersection cgIntersection;
typedef struct cgObjectList cgObjectList;

typedef enum {
	SPHERE
} cgObjectType;

struct cgObject {
	cgObjectType type;
	cgColor color;
	void * data;
	cgIntersectionFunction intersection;
};

struct cgIntersection{
	long double distance;
	cgPoint3f point;
	cgObject object;
};
