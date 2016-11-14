#include <stdio.h>
#include <stdlib.h>
#include "avs.h"

cgAVS_t* readAVS(FILE* fp){
	char r, g, b, a;
	int width, height;

	cgAVS_t *avs = malloc(sizeof(cgAVS_t));
	/* Read the header */
	fread(&width, sizeof(int), 1, fp);
	avs->width = FIX(width);

	fread(&height,sizeof(int), 1, fp);
	avs->height = FIX(height);

	avs->data = malloc(avs->height * sizeof(cgAVS_Pixel*));

	for (int i = 0; i < avs->height; i++) {
	    avs->data[i] = malloc(avs->width * sizeof(cgAVS_Pixel));
	}

	/* Read the ARGB for each pixel */
	/* Reading is done backwards because top most pixels are athe end of the file */

	for(int i = avs->height - 1; i >= 0; i--){
		for(int j = avs->width - 1; j >= 0; j--){
			//Read the current pixel
			fread(&a, sizeof(unsigned char), 1, fp); // alpha component is ignored
			fread(&r, sizeof(unsigned char), 1, fp);
			fread(&g, sizeof(unsigned char), 1, fp);
			fread(&b, sizeof(unsigned char), 1, fp);

			avs->data[i][j].r = r;
			avs->data[i][j].g = g;
			avs->data[i][j].b = b;
		}
	}

	fclose(fp);

	return avs;
}