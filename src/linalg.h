#ifndef INCLUDE_LINALG_H
#define INCLUDE_LINALG_H

#include <stdint.h>

int16_t fpsin(int16_t i);
//Cos(x) = sin(x + pi/2)
#define fpcos(i) fpsin((int16_t)(((uint16_t)(i)) + 8192U))
#define i16_PI 16384
#define SIN_RANGE 4096

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

static inline ivec3_t v2_to_v3(ivec2_t a){
  return (ivec3_t){ .x=a.x, .y=a.y, .z=0 };
}
static inline ivec2_t v3_to_v2(ivec3_t a){
  return (ivec2_t){ .x=a.x, .y=a.y, };
}

static inline imat3_t m2_to_m3(imat2_t a){
  return (imat3_t){ .m={
    a.m[0], a.m[1], 0,
    a.m[2], a.m[3], 0,
    0,      0,      1
  }};
}
static inline imat2_t m3_to_m2(imat3_t a){
  return (imat2_t){ .m={
    a.m[0], a.m[1],
    a.m[3], a.m[4],
  }};
}

static inline ivec2_t add_v2(ivec2_t a, ivec2_t b){
  return (ivec2_t){
    .x=a.x+b.x, 
    .y=a.y+b.y
  };
}
static inline ivec3_t add_v3(ivec3_t a, ivec3_t b){
  return (ivec3_t){
    .x=a.x+b.x, 
    .y=a.y+b.y,
    .z=a.z+b.z
  };
}

static inline ivec2_t mul_sv2(int32_t s, ivec2_t v){
  return (ivec2_t){
    .x=v.x*s,
    .y=v.y*s
  };
}
static inline ivec3_t mul_sv3(int32_t s, ivec3_t v){
  return (ivec3_t){
    .x=v.x*s,
    .y=v.y*s,
    .z=v.z*s
  };
}

static inline ivec2_t div_sv2(int32_t s, ivec2_t v){
  return (ivec2_t){
    .x=v.x/s,
    .y=v.y/s
  };
}
static inline ivec3_t div_sv3(int32_t s, ivec3_t v){
  return (ivec3_t){
    .x=v.x/s,
    .y=v.y/s,
    .z=v.z/s
  };
}

static inline ivec2_t mul_mv2(imat2_t m, ivec2_t v){
  return (ivec2_t){
    .x=m.m[0]*v.x+m.m[1]*v.y, 
    .y=m.m[2]*v.x+m.m[3]*v.y
  };
}
static inline ivec3_t mul_mv3(imat3_t m, ivec3_t v){
  return (ivec3_t){
    .x=m.m[0]*v.x+m.m[1]*v.y+m.m[2]*v.z,
    .y=m.m[3]*v.x+m.m[4]*v.y+m.m[5]*v.z,
    .z=m.m[6]*v.x+m.m[7]*v.y+m.m[8]*v.z,
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
static inline imat3_t mul_m3(imat3_t a, imat3_t b){
  return (imat3_t){.m={
    a.m[0]*b.m[0]+a.m[1]*b.m[3]+a.m[2]*b.m[6],
    a.m[0]*b.m[1]+a.m[1]*b.m[4]+a.m[2]*b.m[7],
    a.m[0]*b.m[2]+a.m[1]*b.m[5]+a.m[2]*b.m[8],

    a.m[3]*b.m[0]+a.m[4]*b.m[3]+a.m[5]*b.m[6],
    a.m[3]*b.m[1]+a.m[4]*b.m[4]+a.m[5]*b.m[7],
    a.m[3]*b.m[2]+a.m[4]*b.m[5]+a.m[5]*b.m[8],

    a.m[6]*b.m[0]+a.m[7]*b.m[3]+a.m[8]*b.m[6],
    a.m[6]*b.m[1]+a.m[7]*b.m[4]+a.m[8]*b.m[7],
    a.m[6]*b.m[2]+a.m[7]*b.m[5]+a.m[8]*b.m[8],
  }};
}

static inline imat2_t scale_m2(int32_t s){
  return (imat2_t){.m={
    s,0,
    0,s
  }};
}
static inline imat2_t rot_m2(int16_t t){
  return (imat2_t){.m={
    fpcos(t), -fpsin(t),
    fpsin(t), fpcos(t)
  }};
}

static inline imat3_t rot_x_m3(int16_t t){
  return (imat3_t){.m={
    SIN_RANGE,  0,         0,
    0,          fpcos(t),  -fpsin(t),
    0,          fpsin(t),  fpcos(t)
  }};
}
static inline imat3_t rot_y_m3(int16_t t){
  return (imat3_t){.m={
    fpcos(t), 0,          -fpsin(t),
    0,        SIN_RANGE,  0,
    fpsin(t), 0,          fpcos(t)
  }};
}
static inline imat3_t rot_z_m3(int16_t t){
  return (imat3_t){.m={
    fpcos(t), -fpsin(t),  0,
    fpsin(t), fpcos(t),   0,
    0,        0,          SIN_RANGE
  }};
}
static inline imat3_t scale_m3(int32_t s){
  return (imat3_t){.m={
    s,0,0,
    0,s,0,
    0,0,s
  }};
}

/*
https://www.nullhardware.com/blog/fixed-point-sine-and-cosine-for-embedded-systems/
Implements the 5-order polynomial approximation to sin(x).
@param i   angle (with 2^15 units/circle)
@return    16 bit fixed point Sine value (4.12) (ie: +4096 = +1 & -4096 = -1)

The result is accurate to within +- 1 count. ie: +/-2.44e-4.
*/
int16_t fpsin(int16_t i)
{
    /* Convert (signed) input to a value between 0 and 8192. (8192 is pi/2, which is the region of the curve fit). */
    /* ------------------------------------------------------------------- */
    i <<= 1;
    uint8_t c = i<0; //set carry for output pos/neg

    if(i == (i|0x4000)) // flip input value to corresponding value in range [0..8192)
        i = (1<<15) - i;
    i = (i & 0x7FFF) >> 1;
    /* ------------------------------------------------------------------- */

    /* The following section implements the formula:
     = y * 2^-n * ( A1 - 2^(q-p)* y * 2^-n * y * 2^-n * [B1 - 2^-r * y * 2^-n * C1 * y]) * 2^(a-q)
    Where the constants are defined as follows:
    */
    enum {A1=3370945099UL, B1=2746362156UL, C1=292421UL};
    enum {n=13, p=32, q=31, r=3, a=12};

    uint32_t y = (C1*((uint32_t)i))>>n;
    y = B1 - (((uint32_t)i*y)>>r);
    y = (uint32_t)i * (y>>n);
    y = (uint32_t)i * (y>>n);
    y = A1 - (y>>(p-q));
    y = (uint32_t)i * (y>>n);
    y = (y+(1UL<<(q-a-1)))>>(q-a); // Rounding

    return c ? -y : y;
}

#endif // INCLUDE_LINALG_H
