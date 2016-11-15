#include "vector.h"
#include "graphics.h"
#include "object.h"

cgCuttingDirection cgPointPositionFromPlane(cgPoint3f * intersection_point, cgCuttingPlane * plane);

int canUseIntersectionPoint(cgPoint3f * intersection_point, cgObject * object){
  for (size_t i = 0; i < object->cutting_planes_count; i++) {
    if(cgPointPositionFromPlane(intersection_point, &(object->cutting_planes[i])) == object->cutting_planes[i].direction){
      return 0;
    }
  }
  return 1;
}

cgCuttingDirection cgPointPositionFromPlane(cgPoint3f * point, cgCuttingPlane * plane){
  return (
    ((plane->A*point->x) + (plane->B * point->y)
      + (plane->C * point->z) + plane->D) > 0) ? ABOVE : BELOW;
}
