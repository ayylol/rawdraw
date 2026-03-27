#include "rawdraw.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


// PRIVATE FUNCTION IMPLEMENTATIONS
static inline void rawdraw_swap_points(point_t* p1, point_t* p2){
  point_t temp_p=*p1;
  *p1=*p2;
  *p2=temp_p;
}

// PUBLIC FUNCTION IMPLEMENTATIONS
void rawdraw_rect(image_t img, point_t p1, point_t p2, color_t col) {
  p2.x=p2.x<img.w ? p2.x : img.w;
  p2.y=p2.y<img.h ? p2.y : img.h;
  for (int32_t x=p1.x; x<=p2.x; x++){
    for (int32_t y=p1.y; y<=p2.y; y++){
      img.buffer[rawdraw_get_i(img, x, y)]=col;
    }
  }
}

void rawdraw_fill(image_t img, color_t col) {
  rawdraw_rect(img, (point_t){0,0}, (point_t){img.w-1,img.h-1}, col);
}

void rawdraw_tri(image_t img, point_t p1, point_t p2, point_t p3, color_t col){
  rawdraw_line(img, p1, p2, col);
  rawdraw_line(img, p1, p3, col);
  rawdraw_line(img, p3, p2, col);
}

void rawdraw_line(image_t img, point_t p1, point_t p2, color_t col){
  bool low=true;
  if (abs(p2.y - p1.y) < abs(p2.x - p1.x)){
    if (p1.x > p2.x){ rawdraw_swap_points(&p1, &p2); }
  }
  else {
    low=false;
    if (p1.y > p2.y){ rawdraw_swap_points(&p1, &p2); }
  }
  int32_t d_axis1=p2.y-p1.y;
  int32_t d_axis2=p2.x-p1.x;
  if (!low) {
    int32_t temp=d_axis1;
    d_axis1=d_axis2;
    d_axis2=temp;
  }
  int8_t di=1;
  if (d_axis1<0){
    di=-1;
    d_axis1=-d_axis1;
  }
  int32_t j = low ? p1.y : p1.x;
  int32_t start = low ? p1.x : p1.y;
  int32_t end = low ? p2.x : p2.y;
  int32_t D=2*d_axis1-d_axis2;
  for (int i=start; i<end; i++){
      int32_t x = low ? i : j;
      int32_t y = low ? j : i;
      img.buffer[rawdraw_get_i(img, x, y)] = col;
      if (D>0){
        j=j+di;
        D+=2*(d_axis1-d_axis2);
      } else {
        D+=2*d_axis1;
      }
  }
}

