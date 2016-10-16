
typedef struct {
	long double x;
	long double y;
} cgPoint2f;

typedef struct {
	long double x;
	long double y;
	long double z;
} cgPoint3f;

typedef struct {
	long double x;
	long double y;
	long double z;
} cgVector3f;

long double cgVectorMagnitude(cgVector3f vector);
cgVector3f cgNormalizedVector(cgVector3f vector, long double magnitude);
cgVector3f cgDirectionVector(cgPoint3f origin, cgPoint3f end);
cgVector3f cgInvertedDirectionVector(cgVector3f direction_vector);
long double cgDotProduct(cgVector3f vectorA, cgVector3f vectorB);
cgVector3f cgCrossProduct(cgVector3f vectorA, cgVector3f vectorB);
