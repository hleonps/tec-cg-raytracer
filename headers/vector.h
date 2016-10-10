typedef struct {
	long double x;
	long double y;
} cgPoint2f;

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
cg3DVector cgDirectionVector(cg3DPoint origin, cg3DPoint end);
long double cgDotProduct(cg3DVector vectorA, cg3DVector vectorB);
cg3DVector cgCrossProduct(cg3DVector vectorA, cg3DVector vectorB);
