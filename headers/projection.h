
typedef struct {
	cgPoint2f min;
	cgPoint2f max;
} cgProjectionMatrix;

void cgSetCamaraPosition(int x, int y, int z);
void cgSetProjectionMatrix(int left, int right, int bottom, int top);
long double cgMapXFrameToProjectionMatrix(int x);
long double cgMapYFrameToProjectionMatrix(int y);
