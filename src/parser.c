#include "parser.h"
#include "cg_raytracer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* readline(FILE* fp);
int readScene(FILE* fp);
int readForGenericObject(char* token, cgObject* object);
cgObject *createGenericObject(cgObjectType type);

cgObject *readSphere(FILE* fp);
cgObject *readPolygon(FILE* fp);
cgObject *readCylinder(FILE* fp);
cgObject *readCone(FILE* fp);
cgObject *readDisk(FILE* fp);
cgLight *readLightSource(FILE *fp);

int* ivalues (char* token, int total);
long double * ldvalues(char* token, int total);
int line_count = 0;

void parser_init(FILE* fp){
	char* line = NULL;
	while((line = readline(fp)) != NULL){
		if(strcmp(line, "@defscene") == 0){
			int success = readScene(fp);

			if(!success){
				printf("Failed to create scene\n");
				return;
			}
		}
		else if(strcmp(line, "#defsphere") == 0){
			cgObject *sphere = readSphere(fp);
			if(sphere != NULL){
				cgAddSphereToScene(*sphere);
			}
			else{
				printf("Failed to create scene\n");
				return;
			}
		}
		else if(strcmp(line, "#defpolygon") == 0){
			cgObject *polygon = readPolygon(fp);

			if(polygon != NULL){
				cgPolygon *data = (cgPolygon*)polygon->data;
				if(data->points_count > 2){
					cgAddPolygonToScene(*polygon);
				}
				else{
					printf("ERROR: Polygon points must be at least 3: %i Found\n", data->points_count);
				}
			}
			else{
				printf("Failed to create scene\n");
				return;
			}
			
		}
		else if(strcmp(line, "#defcylinder") == 0){
			cgObject *cylinder = readCylinder(fp);
			if(cylinder != NULL){
				cgAddCylinderToScene(*cylinder);
			}
			else{
				printf("Failed to create scene\n");
				return;
			}
		}
		else if(strcmp(line, "#defcone") == 0){
			cgObject *cone = readCone(fp);
			if(cone != NULL){
				cgAddConeToScene(*cone);
			}
			else{
				printf("Failed to create scene\n");
				return;
			}
		}
		else if(strcmp(line, "#defdisk") == 0){
			cgObject *disk = readDisk(fp);

			if(disk != NULL){
				cgAddDiskToScene(*disk);
			}
			else{
				printf("Failed to create scene\n");
				return;
			}
			
		}
		else if(strcmp(line, "@deflight") == 0){
			cgLight *light_source = readLightSource(fp);
			if(light_source != NULL){
				cgAddLightSourceToScene(*light_source);
			}
			else{
				printf("Failed to create scene\n");
				return;
			}
		}
		else if(strcmp(line, "") == 0){
			continue;
		}
		else{
			printf("Warning: Unexpected token: '%s' at line %i\n", line, line_count);
		}
	}
}

int readScene(FILE* fp){
	char *line;
	while((line = readline(fp)) != NULL){
		if(strcmp(line, "@enddef") == 0){
			break;
		}
		else{
			char* token;
			token = strtok(line, " ");
			if(token == NULL){
				/* Blank line under body scope */
				continue;
			}
			else if(strcmp(token, "camera") == 0){
				token = strtok(NULL, " ");
				if(token == NULL){
					printf("Syntax error at line %d\n", line_count);
					return 0;
				}
				int *values = ivalues(token, 3);
				cgSetCameraPosition(values[0], values[1], values[2]);
			}
			else if(strcmp(token, "projection-matrix") == 0){
				token = strtok(NULL, " ");
				if(token == NULL){
					printf("Syntax error at line %d\n", line_count);
					return 0;
				}
				int *values = ivalues(token, 4);
				cgSetProjectionMatrix(values[0], values[1], values[2], values[3]);
			}
			else if(strcmp(token, "environment-lighting") == 0){
				token = strtok(NULL, " ");
				if(token == NULL){
					printf("Syntax error at line %d\n", line_count);
					return 0;
				}

				long double *values = ldvalues(token, 1);
				cgSetEnvironmentLighting(values[0]);
			}
			else if(strcmp(token, "resolution") == 0){
				token = strtok(NULL, " ");
				if(token == NULL){
					printf("Syntax error at line %d\n", line_count);
					return 0;
				}
				int *values = ivalues(token, 2);
				cgInitFramebuffer(values[0], values[1]);
			}
			else{
				printf("Warning: '%s' is not a valid property at line %i\n", token, line_count);
				continue;
			}
		}
	}

	return 1;
}

cgObject *readSphere(FILE* fp){
	cgObject *sphere = createGenericObject(SPHERE);
	
	cgSphere *information = (cgSphere*)malloc(sizeof(cgSphere));

	char *line;
	while((line = readline(fp)) != NULL){
		if(strcmp(line, "#enddef") == 0){
			break;
		}
		else{
			char* token;
			token = strtok(line, " ");

			if(token == NULL){
				/* Blank line under body scope */
				continue;
			}
			else if(strcmp(token, "center") == 0){
				token = strtok(NULL, " ");
				if(token == NULL){
					printf("Syntax error at line %d\n", line_count);
					return NULL;
				}
				long double *values = ldvalues(token, 3);
				cgPoint3f center = {.x = values[0], .y = values[1], .z = values[2]};
				information->center = center;
			}
			else if(strcmp(token, "radius") == 0){
				token = strtok(NULL, " ");
				if(token == NULL){
					printf("Syntax error at line %d\n", line_count);
					return NULL;
				}
				long double *values = ldvalues(token, 1);
				information->radius = values[0];
			}
			else{
				if(!readForGenericObject(token, sphere)){
					return NULL;
				}
			}
		}
	}

	sphere->data = (void*)information;

	return sphere;
}

cgObject *readPolygon(FILE* fp){
	cgObject *polygon = createGenericObject(POLYGON);
	
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

			if(token == NULL){
				/* Blank line under body scope */
				continue;
			}
			else if(strcmp(token, "points") == 0){
				int points_count = 0;
				char** point_vals = NULL;
				token = strtok(NULL, " ");

				if(token == NULL){
					printf("Syntax error at line %d\n", line_count);
					return NULL;
				}

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
			else{
				if(!readForGenericObject(token, polygon)){
					return NULL;
				}
			}
		}
	}

	polygon->data = (void*)information;

	return polygon;
}

cgObject *readCylinder(FILE* fp){
	cgObject *cylinder = createGenericObject(CYLINDER);
	
	cgCylinder *information = (cgCylinder*)malloc(sizeof(cgCylinder));

	char *line;
	while((line = readline(fp)) != NULL){
		if(strcmp(line, "#enddef") == 0){
			break;
		}
		else{
			char* token;
			token = strtok(line, " ");

			if(token == NULL){
				/* Blank line under body scope */
				continue;
			}
			else if(strcmp(token, "anchor") == 0){
				token = strtok(NULL, " ");
				if(token == NULL){
					printf("Syntax error at line %d\n", line_count);
					return NULL;
				}
				long double *values = ldvalues(token, 3);
				cgPoint3f anchor = {.x = values[0], .y = values[1], .z = values[2]};
				information->anchor = anchor;
			}
			else if(strcmp(token, "direction") == 0){
				token = strtok(NULL, " ");
				if(token == NULL){
					printf("Syntax error at line %d\n", line_count);
					return NULL;
				}
				long double *values = ldvalues(token, 3);
				cgVector3f direction = {.x = values[0], .y = values[1], .z = values[2]};

				/* Q Vector is normalized */
				cgVector3f unit_direction = cgNormalizedVector(direction, cgVectorMagnitude(direction));
				information->direction = unit_direction;
			}
			else if(strcmp(token, "min-limit") == 0){
				token = strtok(NULL, " ");
				if(token == NULL){
					printf("Syntax error at line %d\n", line_count);
					return NULL;
				}
				long double *values = ldvalues(token, 1);
				information->distance_a = values[0];
			}
			else if(strcmp(token, "max-limit") == 0){
				token = strtok(NULL, " ");
				if(token == NULL){
					printf("Syntax error at line %d\n", line_count);
					return NULL;
				}
				long double *values = ldvalues(token, 1);
				information->distance_b = values[0];
			}
			else if(strcmp(token, "radius") == 0){
				token = strtok(NULL, " ");
				if(token == NULL){
					printf("Syntax error at line %d\n", line_count);
					return NULL;
				}
				long double *values = ldvalues(token, 1);
				information->radius = values[0];
			}
			else{
				if(!readForGenericObject(token, cylinder)){
					return NULL;
				}
			}
		}
	}

	cylinder->data = (void*)information;

	return cylinder;
}

cgObject *readCone(FILE* fp){
	cgObject *cone = createGenericObject(CONE);
	
	cgCone *information = (cgCone*)malloc(sizeof(cgCone));

	char *line;
	while((line = readline(fp)) != NULL){
		if(strcmp(line, "#enddef") == 0){
			break;
		}
		else{
			char* token;
			token = strtok(line, " ");

			if(token == NULL){
				/* Blank line under body scope */
				continue;
			}
			else if(strcmp(token, "anchor") == 0){
				token = strtok(NULL, " ");
				if(token == NULL){
					printf("Syntax error at line %d\n", line_count);
					return NULL;
				}
				long double *values = ldvalues(token, 3);
				cgPoint3f anchor = {.x = values[0], .y = values[1], .z = values[2]};
				information->anchor = anchor;
			}
			else if(strcmp(token, "direction") == 0){
				token = strtok(NULL, " ");
				if(token == NULL){
					printf("Syntax error at line %d\n", line_count);
					return NULL;
				}
				long double *values = ldvalues(token, 3);
				cgVector3f direction = {.x = values[0], .y = values[1], .z = values[2]};

				/* Q Vector is normalized */
				cgVector3f unit_direction = cgNormalizedVector(direction, cgVectorMagnitude(direction));
				information->direction = unit_direction;
			}
			else if(strcmp(token, "min-limit") == 0){
				token = strtok(NULL, " ");
				if(token == NULL){
					printf("Syntax error at line %d\n", line_count);
					return NULL;
				}
				long double *values = ldvalues(token, 1);
				information->distance_a = values[0];
			}
			else if(strcmp(token, "max-limit") == 0){
				token = strtok(NULL, " ");
				if(token == NULL){
					printf("Syntax error at line %d\n", line_count);
					return NULL;
				}
				long double *values = ldvalues(token, 1);
				information->distance_b = values[0];
			}
			else if(strcmp(token, "radius-k") == 0){
				token = strtok(NULL, " ");
				if(token == NULL){
					printf("Syntax error at line %d\n", line_count);
					return NULL;
				}
				long double *values = ldvalues(token, 1);
				information->radius_k = values[0];
			}
			else if(strcmp(token, "distance-k") == 0){
				token = strtok(NULL, " ");
				if(token == NULL){
					printf("Syntax error at line %d\n", line_count);
					return NULL;
				}
				long double *values = ldvalues(token, 1);
				information->distance_k = values[0];
			}
			else{
				if(!readForGenericObject(token, cone)){
					return NULL;
				}
			}
		}
	}

	cone->data = (void*)information;

	return cone;
}

cgObject *readDisk(FILE* fp){
	cgObject *disk = createGenericObject(DISK);
	
	cgDisk *information = (cgDisk*)malloc(sizeof(cgDisk));
	information->normal_vector = NULL;
	information->inner_radius = 0;
	information->outer_radius = 0;

	char *line;
	while((line = readline(fp)) != NULL){
		if(strcmp(line, "#enddef") == 0){
			break;
		}
		else{
			char* token;
			token = strtok(line, " ");

			if(token == NULL){
				/* Blank line under body scope */
				continue;
			}
			else if(strcmp(token, "center") == 0){
				token = strtok(NULL, " ");
				if(token == NULL){
					printf("Syntax error at line %d\n", line_count);
					return NULL;
				}
				long double *values = ldvalues(token, 3);
				cgPoint3f center = {.x = values[0], .y = values[1], .z = values[2]};
				information->center = center;
			}
			else if(strcmp(token, "plane-points") == 0){
				int points_count = 0;
				char** point_vals = NULL;
				token = strtok(NULL, " ");

				if(token == NULL){
					printf("Syntax error at line %d\n", line_count);
					return NULL;
				}

				while(token != NULL || points_count < 2){
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

				information->plane_points = points;
			}
			else if(strcmp(token, "inner-radius") == 0){
				token = strtok(NULL, " ");
				if(token == NULL){
					printf("Syntax error at line %d\n", line_count);
					return NULL;
				}
				long double *values = ldvalues(token, 1);
				information->inner_radius = values[0];
			}
			else if(strcmp(token, "outer-radius") == 0){
				token = strtok(NULL, " ");
				if(token == NULL){
					printf("Syntax error at line %d\n", line_count);
					return NULL;
				}
				long double *values = ldvalues(token, 1);
				information->outer_radius = values[0];
			}
			else{
				if(!readForGenericObject(token, disk)){
					return NULL;
				}
			}
		}
	}

	disk->data = (void*)information;

	return disk;
}

cgLight *readLightSource(FILE* fp){
	cgLight *light_source = calloc(sizeof(cgLight), 1);

	char *line;
	while((line = readline(fp)) != NULL){
		if(strcmp(line, "@enddef") == 0){
			break;
		}
		else{
			char* token;
			token = strtok(line, " ");

			if(token == NULL){
				/* Blank line under body scope */
				continue;
			}
			else if(strcmp(token, "position") == 0){
				token = strtok(NULL, " ");
				if(token == NULL){
					printf("Syntax error at line %d\n", line_count);
					return NULL;
				}
				long double *values = ldvalues(token, 3);
				cgPoint3f position = {.x = values[0], .y = values[1], .z = values[2]};
				light_source->position = position;
			}
			else if(strcmp(token, "intensity") == 0){
				token = strtok(NULL, " ");
				if(token == NULL){
					printf("Syntax error at line %d\n", line_count);
					return NULL;
				}
				long double *values = ldvalues(token, 1);
				light_source->intensity = values[0];
			}
			else if(strcmp(token, "c1") == 0){
				token = strtok(NULL, " ");
				if(token == NULL){
					printf("Syntax error at line %d\n", line_count);
					return NULL;
				}
				long double *values = ldvalues(token, 1);
				light_source->c1 = values[0];
			}
			else if(strcmp(token, "c2") == 0){
				token = strtok(NULL, " ");
				if(token == NULL){
					printf("Syntax error at line %d\n", line_count);
					return NULL;
				}
				long double *values = ldvalues(token, 1);
				light_source->c2 = values[0];
			}
			else if(strcmp(token, "c3") == 0){
				token = strtok(NULL, " ");
				if(token == NULL){
					printf("Syntax error at line %d\n", line_count);
					return NULL;
				}
				long double *values = ldvalues(token, 1);
				light_source->c3 = values[0];
			}
			else{
				printf("Warning: '%s' is not a valid property at line %i\n", token, line_count);
				continue;
			}
		}
	}
	return light_source;
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
			bytes_read++;
			line = (char*)realloc(line, sizeof(char)*(bytes_read));
			if(curr_char != '\n'){
				line[bytes_read - 1] = curr_char;
			}
			else{
				line[bytes_read - 1] = '\0';
			}
		}	
		else{
			/* Ignore tabs */
			continue;
		}
	}
	while(curr_char != '\n' && curr_char != EOF);

	if(line != NULL){
		line_count++;
	}
	return line;
}

int readForGenericObject(char* token, cgObject* object){
	if(strcmp(token, "color") == 0){
		token = strtok(NULL, " ");
		if(token == NULL){
			printf("Syntax error at line %d\n", line_count);
			return 0;
		}
		int *values = ivalues(token, 3);
		cgColor color = {.r = values[0]/255.0, .g = values[1]/255.0, .b = values[2]/255.0, .a = 1};
		object->color = color;
	}
	else if(strcmp(token, "diffuse-factor") == 0){
		token = strtok(NULL, " ");
		if(token == NULL){
			printf("Syntax error at line %d\n", line_count);
			return 0;
		}
		long double *values = ldvalues(token, 1);
		object->diffuse_factor = values[0];
	}
	else if(strcmp(token, "specular-factor") == 0){
		token = strtok(NULL, " ");
		if(token == NULL){
			printf("Syntax error at line %d\n", line_count);
			return 0;
		}
		long double *values = ldvalues(token, 1);
		object->specular_factor = values[0];
	}
	else if(strcmp(token, "specular-focus") == 0){
		token = strtok(NULL, " ");
		if(token == NULL){
			printf("Syntax error at line %d\n", line_count);
			return 0;
		}
		int *values = ivalues(token, 1);
		object->specular_focus = values[0];
	}
	else if(strcmp(token, "environment-lighting") == 0){
		token = strtok(NULL, " ");
		if(token == NULL){
			printf("Syntax error at line %d\n", line_count);
			return 0;
		}
		long double *values = ldvalues(token, 1);
		object->environment_lighting = values[0];
	}
	else if(strcmp(token, "transparency-factor") == 0){
		token = strtok(NULL, " ");
		if(token == NULL){
			printf("Syntax error at line %d\n", line_count);
			return 0;
		}
		long double *values = ldvalues(token, 1);
		object->transparency_factor = values[0];
	}
	else if(strcmp(token, "reflection-factor") == 0){
		token = strtok(NULL, " ");
		if(token == NULL){
			printf("Syntax error at line %d\n", line_count);
			return 0;
		}
		long double *values = ldvalues(token, 1);
		object->reflection_factor = values[0];
	}
	else{
		printf("Warning: %s is not a valid property at line %i\n", token, line_count);
	}

	return 1;
}

cgObject* createGenericObject(cgObjectType type){
	cgObject *object = (cgObject*)malloc(sizeof(cgObject));
	object->type = type;
	object->diffuse_factor = 0;
	object->specular_factor = 0;
	object->specular_focus = 0;
	object->environment_lighting = 0;
	object->transparency_factor = 0;
	object->reflection_factor = 0;

	switch(type){
		case SPHERE:
			object->intersection = &cgSphereIntersection;
			object->normal_vector = (cgNormalVector) &cgSphereNormalVector;
			break;
		case POLYGON:
			object->intersection = &cgPolygonIntersection;
			object->normal_vector = (cgNormalVector) &cgPolygonNormalVector;
			break;
		case CYLINDER:
			object->intersection = &cgCylinderIntersection;
			object->normal_vector = (cgNormalVector) &cgCylinderNormalVector;
			break;
		case CONE:
			object->intersection = &cgConeIntersection;
			object->normal_vector = (cgNormalVector) &cgConeNormalVector;
			break;
		case DISK:
			object->intersection = &cgDiskIntersection;
			object->normal_vector = (cgNormalVector) &cgDiskNormalVector;
			break;
		default:
			object = NULL;
			break;
	}

	return object;
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