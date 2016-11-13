
typedef struct {
	cgPoint2f min;
	cgPoint2f max;
} cgProjectionMatrix;

cgPoint3f camera;

void cgSetCameraPosition(int x, int y, int z);
void cgSetProjectionMatrix(int left, int right, int bottom, int top);
long double cgMapXFrameToProjectionMatrix(int x, long double offset);
long double cgMapYFrameToProjectionMatrix(int y, long double offset);
