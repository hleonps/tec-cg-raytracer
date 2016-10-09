typedef struct {
	long double x;
	long double y;
	long double z;
} cg3DPoint;

typedef struct {
	long double x;
	long double y;
	long double z;
} cg3DVector;

long double cgVectorMagnitude(cg3DVector vector);
cg3DVector cgNormalizedVector(cg3DVector vector, long double magnitude);
