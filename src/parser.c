#include "parser.h"
#include "cg_raytracer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* readline(FILE* fp);
void readScene(FILE* fp);
cgObject readSphere(FILE* fp);
cgObject readPolygon(FILE* fp);
cgObject readCylinder(FILE* fp);
cgObject readCone(FILE* fp);

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
		else if(strcmp(line, "#defpolygon") == 0){
			cgObject polygon = readPolygon(fp);

			cgPolygon *data = (cgPolygon*)polygon.data;

			if(data->points_count > 2){
				cgAddPolygonToScene(polygon);
			}
			else{
				printf("ERROR: Polygon points must be at least 3: %i Found\n", data->points_count);
			}
		}
		else if(strcmp(line, "#defcylinder") == 0){
			cgObject cylinder = readCylinder(fp);
			cgAddCylinderToScene(cylinder);
		}
		else if(strcmp(line, "#defcone") == 0){
			cgObject cone = readCone(fp);
			cgAddConeToScene(cone);
		}
	}
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
				printf("Warning: %s is not a valid property\n", token);
				continue;
			}
		}
	}
}

cgObject readSphere(FILE* fp){
	cgObject sphere;
	sphere.type = SPHERE;
	sphere.intersection = &cgSphereIntersection;
	sphere.normal_vector = (cgNormalVector) &cgSphereNormalVector;
	sphere.diffuse_factor = 0;
	sphere.specular_factor = 0;
	sphere.specular_focus = 0;
	sphere.environment_lighting = 0;
	sphere.transparency_factor = 0;
	sphere.reflection_factor = 0;
	
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
				printf("Warning: %s is not a valid property\n", token);
				continue;
			}
		}
	}

	sphere.data = (void*)information;

	return sphere;
}

cgObject readPolygon(FILE* fp){
	cgObject polygon;
	polygon.type = POLYGON;
	polygon.intersection = &cgPolygonIntersection;
	polygon.normal_vector = (cgNormalVector) &cgPolygonNormalVector;
	polygon.diffuse_factor = 0;
	polygon.specular_factor = 0;
	polygon.specular_focus = 0;
	polygon.environment_lighting = 0;
	polygon.transparency_factor = 0;
	polygon.reflection_factor = 0;
	
	cgPolygon *information = (cgPolygon*)malloc(sizeof(cgPolygon));
	information->normal_vector = NULL;

	char *line;
	while((line = readline(fp)) != NULL){
		if(strcmp(line, "#enddef") == 0){
			break;
		}
		else{
			char* token;
			token = strtok(line, " ");

			if(strcmp(token, "points") == 0){
				int points_count = 0;
				char** point_vals = NULL;
				token = strtok(NULL, " ");

				while(token != NULL){
					points_count++;
					point_vals = realloc(point_vals, sizeof(char*) * points_count);
					point_vals[points_count - 1] = token;
					token = strtok(NULL, " ");
				}

				cgPoint3f *points = malloc(sizeof(cgPoint3f) * points_count);
				for(int i = 0; i < points_count; i++){
					long double *values = ldvalues(point_vals[i], 3);
					cgPoint3f point = {.x = values[0], .y = values[1], .z = values[2]};
					points[i] = point;
				}

				information->points_count = points_count;
				information->points_3d = points;
			}
			else if(strcmp(token, "color") == 0){
				token = strtok(NULL, " ");
				long double *values = ldvalues(token, 3);
				cgColor color = {.r = values[0], .g = values[1], .b = values[2], .a = 1};
				polygon.color = color;
			}
			else if(strcmp(token, "diffuse-factor") == 0){
				token = strtok(NULL, " ");
				long double *values = ldvalues(token, 1);
				polygon.diffuse_factor = values[0];
			}
			else if(strcmp(token, "specular-factor") == 0){
				token = strtok(NULL, " ");
				long double *values = ldvalues(token, 1);
				polygon.specular_factor = values[0];
			}
			else if(strcmp(token, "specular-focus") == 0){
				token = strtok(NULL, " ");
				int *values = ivalues(token, 1);
				polygon.specular_focus = values[0];
			}
			else if(strcmp(token, "environment-lighting") == 0){
				token = strtok(NULL, " ");
				long double *values = ldvalues(token, 1);
				polygon.environment_lighting = values[0];
			}
			else if(strcmp(token, "transparency-factor") == 0){
				token = strtok(NULL, " ");
				long double *values = ldvalues(token, 1);
				polygon.transparency_factor = values[0];
			}
			else if(strcmp(token, "reflection-factor") == 0){
				token = strtok(NULL, " ");
				long double *values = ldvalues(token, 1);
				polygon.reflection_factor = values[0];
			}
			else{
				printf("Warning: %s is not a valid property\n", token);
				continue;
			}
		}
	}

	polygon.data = (void*)information;

	return polygon;
}

cgObject readCylinder(FILE* fp){
	cgObject cylinder;
	cylinder.type = CYLINDER;
	cylinder.intersection = &cgCylinderIntersection;
	cylinder.normal_vector = (cgNormalVector) &cgCylinderNormalVector;
	cylinder.diffuse_factor = 0;
	cylinder.specular_factor = 0;
	cylinder.specular_focus = 0;
	cylinder.environment_lighting = 0;
	cylinder.transparency_factor = 0;
	cylinder.reflection_factor = 0;
	
	cgCylinder *information = (cgCylinder*)malloc(sizeof(cgCylinder));

	char *line;
	while((line = readline(fp)) != NULL){
		if(strcmp(line, "#enddef") == 0){
			break;
		}
		else{
			char* token;
			token = strtok(line, " ");

			if(strcmp(token, "anchor") == 0){
				token = strtok(NULL, " ");
				long double *values = ldvalues(token, 3);
				cgPoint3f anchor = {.x = values[0], .y = values[1], .z = values[2]};
				information->anchor = anchor;
			}
			else if(strcmp(token, "direction") == 0){
				token = strtok(NULL, " ");
				long double *values = ldvalues(token, 3);
				cgVector3f direction = {.x = values[0], .y = values[1], .z = values[2]};

				/* Q Vector is normalized */
				cgVector3f unit_direction = cgNormalizedVector(direction, cgVectorMagnitude(direction));
				information->direction = unit_direction;
			}
			else if(strcmp(token, "min-limit") == 0){
				token = strtok(NULL, " ");
				long double *values = ldvalues(token, 1);
				information->distance_a = values[0];
			}
			else if(strcmp(token, "max-limit") == 0){
				token = strtok(NULL, " ");
				long double *values = ldvalues(token, 1);
				information->distance_b = values[0];
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
				cylinder.color = color;
			}
			else if(strcmp(token, "diffuse-factor") == 0){
				token = strtok(NULL, " ");
				long double *values = ldvalues(token, 1);
				cylinder.diffuse_factor = values[0];
			}
			else if(strcmp(token, "specular-factor") == 0){
				token = strtok(NULL, " ");
				long double *values = ldvalues(token, 1);
				cylinder.specular_factor = values[0];
			}
			else if(strcmp(token, "specular-focus") == 0){
				token = strtok(NULL, " ");
				int *values = ivalues(token, 1);
				cylinder.specular_focus = values[0];
			}
			else if(strcmp(token, "environment-lighting") == 0){
				token = strtok(NULL, " ");
				long double *values = ldvalues(token, 1);
				cylinder.environment_lighting = values[0];
			}
			else if(strcmp(token, "transparency-factor") == 0){
				token = strtok(NULL, " ");
				long double *values = ldvalues(token, 1);
				cylinder.transparency_factor = values[0];
			}
			else if(strcmp(token, "reflection-factor") == 0){
				token = strtok(NULL, " ");
				long double *values = ldvalues(token, 1);
				cylinder.reflection_factor = values[0];
			}
			else{
				printf("Warning: %s is not a valid property\n", token);
				continue;
			}
		}
	}

	cylinder.data = (void*)information;

	return cylinder;
}

cgObject readCone(FILE* fp){
	cgObject cone;
	cone.type = CONE;
	cone.intersection = &cgConeIntersection;
	cone.normal_vector = (cgNormalVector) &cgConeNormalVector;
	cone.diffuse_factor = 0;
	cone.specular_factor = 0;
	cone.specular_focus = 0;
	cone.environment_lighting = 0;
	cone.transparency_factor = 0;
	cone.reflection_factor = 0;
	
	cgCone *information = (cgCone*)malloc(sizeof(cgCone));

	char *line;
	while((line = readline(fp)) != NULL){
		if(strcmp(line, "#enddef") == 0){
			break;
		}
		else{
			char* token;
			token = strtok(line, " ");

			if(strcmp(token, "anchor") == 0){
				token = strtok(NULL, " ");
				long double *values = ldvalues(token, 3);
				cgPoint3f anchor = {.x = values[0], .y = values[1], .z = values[2]};
				information->anchor = anchor;
			}
			else if(strcmp(token, "direction") == 0){
				token = strtok(NULL, " ");
				long double *values = ldvalues(token, 3);
				cgVector3f direction = {.x = values[0], .y = values[1], .z = values[2]};

				/* Q Vector is normalized */
				cgVector3f unit_direction = cgNormalizedVector(direction, cgVectorMagnitude(direction));
				information->direction = unit_direction;
			}
			else if(strcmp(token, "min-limit") == 0){
				token = strtok(NULL, " ");
				long double *values = ldvalues(token, 1);
				information->distance_a = values[0];
			}
			else if(strcmp(token, "max-limit") == 0){
				token = strtok(NULL, " ");
				long double *values = ldvalues(token, 1);
				information->distance_b = values[0];
			}
			else if(strcmp(token, "radius-k") == 0){
				token = strtok(NULL, " ");
				long double *values = ldvalues(token, 1);
				information->radius_k = values[0];
			}
			else if(strcmp(token, "distance-k") == 0){
				token = strtok(NULL, " ");
				long double *values = ldvalues(token, 1);
				information->distance_k = values[0];
			}
			else if(strcmp(token, "color") == 0){
				token = strtok(NULL, " ");
				long double *values = ldvalues(token, 3);
				cgColor color = {.r = values[0], .g = values[1], .b = values[2], .a = 1};
				cone.color = color;
			}
			else if(strcmp(token, "diffuse-factor") == 0){
				token = strtok(NULL, " ");
				long double *values = ldvalues(token, 1);
				cone.diffuse_factor = values[0];
			}
			else if(strcmp(token, "specular-factor") == 0){
				token = strtok(NULL, " ");
				long double *values = ldvalues(token, 1);
				cone.specular_factor = values[0];
			}
			else if(strcmp(token, "specular-focus") == 0){
				token = strtok(NULL, " ");
				int *values = ivalues(token, 1);
				cone.specular_focus = values[0];
			}
			else if(strcmp(token, "environment-lighting") == 0){
				token = strtok(NULL, " ");
				long double *values = ldvalues(token, 1);
				cone.environment_lighting = values[0];
			}
			else if(strcmp(token, "transparency-factor") == 0){
				token = strtok(NULL, " ");
				long double *values = ldvalues(token, 1);
				cone.transparency_factor = values[0];
			}
			else if(strcmp(token, "reflection-factor") == 0){
				token = strtok(NULL, " ");
				long double *values = ldvalues(token, 1);
				cone.reflection_factor = values[0];
			}
			else{
				printf("Warning: %s is not a valid property\n", token);
				continue;
			}
		}
	}

	cone.data = (void*)information;

	return cone;
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