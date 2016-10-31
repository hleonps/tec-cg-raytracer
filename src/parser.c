#include "parser.h"
#include "cg_raytracer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* readline(FILE* fp);
void readScene(FILE* fp);
cgObject readSphere(FILE* fp);
int* ivalues (char* token, int total);
long double * ldvalues(char* token, int total);

void parser_init(FILE* fp){
	char* line = NULL;
	while((line = readline(fp)) != NULL){
		if(strcmp(line, "#defscene") == 0){
			readScene(fp);
		}
		else if(strcmp(line, "#defsphere") == 0){
			cgObject sphere = readSphere(fp);
			cgAddSphereToScene(sphere);
		}
	}
}

char* readline(FILE* fp){
	if(fp == NULL){
		printf("Failed to read file\n");
		return NULL;
	}

	int bytes_read = 0;
	char *line = NULL;
	int curr_char;

	do {
		curr_char = fgetc(fp);
		if(curr_char == EOF){
			line = NULL;
			break;
		}
		else if(curr_char != '\t'){
			line = (char*)realloc(line, sizeof(char)*(bytes_read + 1));
			if(curr_char != '\n'){
				line[bytes_read++] = curr_char;
			}
			else{
				line[bytes_read] = '\0';
			}
		}	
		else{
			/* Ignore tabs */
			continue;
		}
	}
	while(curr_char != '\n' && curr_char != EOF);

	return line;
}

void readScene(FILE* fp){
	char *line;
	while((line = readline(fp)) != NULL){
		if(strcmp(line, "#enddef") == 0){
			break;
		}
		else{
			char* token;
			token = strtok(line, " ");

			if(strcmp(token, "camera") == 0){
				token = strtok(NULL, " ");
				int *values = ivalues(token, 3);
				cgSetCameraPosition(values[0], values[1], values[2]);
			}
			else if(strcmp(token, "projection-matrix") == 0){
				token = strtok(NULL, " ");
				int *values = ivalues(token, 4);
				cgSetProjectionMatrix(values[0], values[1], values[2], values[3]);
			}
			else if(strcmp(token, "environment-lighting") == 0){
				token = strtok(NULL, " ");

				long double *values = ldvalues(token, 1);
				cgSetEnvironmentLighting(values[0]);
			}
			else if(strcmp(token, "resolution") == 0){
				token = strtok(NULL, " ");

				int *values = ivalues(token, 2);
				cgInitFramebuffer(values[0], values[1]);
			}
			else{
				break;
			}
		}
	}
}

cgObject readSphere(FILE* fp){
	cgObject sphere;
	sphere.type = SPHERE;
	sphere.intersection = &cgSphereIntersection;
	sphere.normal_vector = (cgNormalVector) &cgSphereNormalVector;
	cgSphere *information = (cgSphere*)malloc(sizeof(cgSphere));

	char *line;
	while((line = readline(fp)) != NULL){
		if(strcmp(line, "#enddef") == 0){
			break;
		}
		else{
			char* token;
			token = strtok(line, " ");

			if(strcmp(token, "center") == 0){
				token = strtok(NULL, " ");
				long double *values = ldvalues(token, 3);
				cgPoint3f center = {.x = values[0], .y = values[1], .z = values[2]};
				information->center = center;
			}
			else if(strcmp(token, "radius") == 0){
				token = strtok(NULL, " ");
				long double *values = ldvalues(token, 1);
				information->radius = values[0];
			}
			else if(strcmp(token, "color") == 0){
				token = strtok(NULL, " ");
				long double *values = ldvalues(token, 3);
				cgColor color = {.r = values[0], .g = values[1], .b = values[2], .a = 1};
				sphere.color = color;
			}
			else if(strcmp(token, "diffuse-factor") == 0){
				token = strtok(NULL, " ");
				long double *values = ldvalues(token, 1);
				sphere.diffuse_factor = values[0];
			}
			else if(strcmp(token, "specular-factor") == 0){
				token = strtok(NULL, " ");
				long double *values = ldvalues(token, 1);
				sphere.specular_factor = values[0];
			}
			else if(strcmp(token, "specular-focus") == 0){
				token = strtok(NULL, " ");
				int *values = ivalues(token, 1);
				sphere.specular_focus = values[0];
			}
			else if(strcmp(token, "environment-lighting") == 0){
				token = strtok(NULL, " ");
				long double *values = ldvalues(token, 1);
				sphere.environment_lighting = values[0];
			}
			else if(strcmp(token, "transparency-factor") == 0){
				token = strtok(NULL, " ");
				long double *values = ldvalues(token, 1);
				sphere.transparency_factor = values[0];
			}
			else if(strcmp(token, "reflection-factor") == 0){
				token = strtok(NULL, " ");
				long double *values = ldvalues(token, 1);
				sphere.reflection_factor = values[0];
			}
			else{
				break;
			}
		}
	}

	sphere.data = (void*)information;

	return sphere;
}

int* ivalues(char* token, int total){
	int *values = calloc(total, sizeof(int));
	char *tmp = strtok(token,",");

	for(int i = 0; i < total; i++){
		int val;
		sscanf(tmp, "%d", &val);
		values[i] = val;
		tmp = strtok(NULL, ",");
	}

	return values;
}

long double* ldvalues(char* token, int total){
	long double *values = calloc(total, sizeof(long double));
	char *tmp = strtok(token,",");

	for(int i = 0; i < total; i++){
		long double val;
		sscanf(tmp, "%Lf", &val);
		values[i] = val;
		tmp = strtok(NULL, ",");
	}

	return values;
}