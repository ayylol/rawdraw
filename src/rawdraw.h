#ifndef INCLUDE_RAWDRAW_H_
#define INCLUDE_RAWDRAW_H_

#include <stdlib.h>

// CONSTANTS AND TYPES
typedef struct {
  uint32_t* buffer;
  uint16_t w, h;
} image_t;

typedef struct {
  uint16_t x, y;
} point_t;

typedef uint32_t color_t;

#define BLACK 0x00000000
#define WHITE 0x00FFFFFF
#define RED   0x00FF0000
#define GREEN 0x0000FF00
#define BLUE  0x000000FF

// PRIVATE FUNCTION DECLARATIONS
static inline uint32_t _rawdraw_get_i(image_t img, uint16_t x, uint16_t y);
static inline void _rawdraw_swap_points(point_t* p1, point_t* p2);

// INTERFACE
void rawdraw_rect(image_t img, point_t p1, point_t p2, color_t col) {
  p2.x=p2.x<img.w ? p2.x : img.w;
  p2.y=p2.y<img.h ? p2.y : img.h;
  for (uint16_t x=p1.x; x<p2.x; x++){
    for (uint16_t y=p1.y; y<p2.y; y++){
      img.buffer[_rawdraw_get_i(img, x, y)]=col;
    }
  }
}

void rawdraw_fill(image_t img, color_t col) {
  rawdraw_rect(img, (point_t){0,0}, (point_t){img.w,img.h}, col);
}

void rawdraw_line(image_t img, point_t p1, point_t p2, color_t col){
  bool low=true;
  if (abs((int32_t)p2.y - p1.y) < abs((int32_t)p2.x - p1.x)){
    if (p1.x > p2.x){ _rawdraw_swap_points(&p1, &p2); }
  }
  else {
    low=false;
    if (p1.y > p2.y){ _rawdraw_swap_points(&p1, &p2); }
  }
  int32_t d_axis1=p2.y-p1.y;
  int32_t d_axis2=p2.x-p1.x;
  int16_t di=1;
  if (d_axis1<0){
    di=-1;
    d_axis1=-d_axis1;
  }
  int32_t D=2*d_axis1-d_axis2;
  int16_t j = low ? p1.y : p1.x;
  int16_t start = low ? p1.x : p1.y;
  int16_t end = low ? p2.x : p2.y;
  for (int i=start; i<end; i++){
      int16_t x = low ? i : j;
      int16_t y = low ? j : i;
      img.buffer[_rawdraw_get_i(img, x, y)] = col;
      if (D>0){
        j=j+di;
        D+=2*(d_axis1-d_axis2);
      } else {
        D+=2*d_axis1;
      }
  }
}

// PRIVATE FUNCTION IMPLEMENTATIONS
static inline uint32_t _rawdraw_get_i(image_t img, uint16_t x, uint16_t y){
  return x+img.w*y; 
}

static inline void _rawdraw_swap_points(point_t* p1, point_t* p2){
  point_t temp_p=*p1;
  *p1=*p2;
  *p2=temp_p;
}

#endif // INCLUDE_RAWDRAW_H_
