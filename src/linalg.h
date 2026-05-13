#ifndef INCLUDE_LINALG_H
#define INCLUDE_LINALG_H

#include <stdint.h>

typedef struct {
  int32_t x,y;
} ivec2_t;
typedef struct {
  int32_t x,y,z;
} ivec3_t;
typedef struct {
  int32_t x,y,z,w;
} ivec4_t;
typedef struct {
  int32_t m[2*2];
} imat2_t;
typedef struct {
  int32_t m[3*3];
} imat3_t;
typedef struct {
  int32_t m[4*4];
} imat4_t;

static inline ivec2_t add_v2(ivec2_t a, ivec2_t b){
  return (ivec2_t){
    .x=a.x+b.x, 
    .y=a.y+b.y
  };
}

static inline ivec2_t mul_sv2(int32_t s, ivec2_t v){
  return (ivec2_t){
    .x=v.x*s,
    .y=v.y*s
  };
}

static inline ivec2_t mul_mv2(imat2_t m, ivec2_t v){
  return (ivec2_t){
    .x=m.m[0]*v.x+m.m[1]*v.y, 
    .y=m.m[2]*v.x+m.m[3]*v.y
  };
}

static inline imat2_t mul_m2(imat2_t a, imat2_t b){
  return (imat2_t){.m={
    a.m[0]*b.m[0]+a.m[2]*b.m[1],
    a.m[1]*b.m[0]+a.m[3]*b.m[1],

    a.m[0]*b.m[2]+a.m[2]*b.m[3],
    a.m[1]*b.m[2]+a.m[3]*b.m[3],
  }};
}

static inline imat2_t scale_m2(int32_t s){
  return (imat2_t){.m={
    s,0,
    0,s
  }};
}

#endif // INCLUDE_LINALG_H
