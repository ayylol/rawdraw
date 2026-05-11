#include "rawdraw.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


color_t colors[8] = {BLACK,WHITE,RED,GREEN,BLUE,MAGENTA,YELLOW,CYAN};
// PRIVATE FUNCTION IMPLEMENTATIONS
static inline void rawdraw_swap_points(point_t* p1, point_t* p2){
  point_t temp_p=*p1;
  *p1=*p2;
  *p2=temp_p;
}

// PUBLIC FUNCTION IMPLEMENTATIONS
void rawdraw_rect(image_t img, point_t p1, point_t p2, color_t col) {
  // TODO: this bound checking probably needs more thought put into it,
  // but at least it doesn't seg fault now
  p2.x=p2.x<img.w ? p2.x : img.w-1;
  p2.y=p2.y<img.h ? p2.y : img.h-1;
  p1.x=p1.x>=0 ? p1.x : 0;
  p1.y=p1.y>=0 ? p1.y : 0;
  for (int32_t x=p1.x; x<=p2.x; x++){
    for (int32_t y=p1.y; y<=p2.y; y++){
      img.buffer[rawdraw_get_i(img, x, y)]=col;
    }
  }
}

void rawdraw_fill(image_t img, color_t col) {
  rawdraw_rect(img, (point_t){0,0}, (point_t){img.w-1,img.h-1}, col);
}

void rawdraw_point(image_t img, point_t p, int32_t dim, color_t col){
  point_t p1=p;
  p1.x-=(dim-1)/2;
  p1.y-=(dim-1)/2;
  point_t p2=p;
  p2.x+=(dim-1)/2 + (dim%2 ? 0 : 1);
  p2.y+=(dim-1)/2 + (dim%2 ? 0 : 1);
  rawdraw_rect(img,p1,p2,col);
}

bool is_left_of(point_t p1, point_t p2, point_t p3){
  point_t v1=(point_t){-(p2.y-p1.y),p2.x-p1.x};
  point_t v2=(point_t){p3.x-p1.x,p3.y-p1.y};
  // Take dot product
  int32_t dot = v1.x*v2.x+v1.y*v2.y;
  return dot>0;
}

// Source - https://stackoverflow.com/a/3437484
// Posted by David Titarenco, modified by community. See post 'Timeline' for change history
// Retrieved 2026-05-10, License - CC BY-SA 4.0
 #define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })
 #define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

void rawdraw_tri(image_t img, point_t p1, point_t p2, point_t p3, color_t col){
  point_t lb = {min(min(p1.x,p2.x),p3.x),min(min(p1.y,p2.y),p3.y)};
  point_t ub = {max(max(p1.x,p2.x),p3.x),max(max(p1.y,p2.y),p3.y)};
  for (int32_t x=lb.x; x<ub.x; x++){
    for (int32_t y=lb.y; y<ub.y; y++){
      point_t p4 = (point_t){x,y};
      bool inside=
        (is_left_of(p1,p2,p4)&&is_left_of(p2,p3,p4)&&is_left_of(p3,p1,p4))
        || !(is_left_of(p1,p2,p4)||is_left_of(p2,p3,p4)||is_left_of(p3,p1,p4));
      if (inside){ img.buffer[rawdraw_get_i(img, x, y)]=col; }
      //img.buffer[rawdraw_get_i(img, x, y)]=inside ? col : WHITE;
    }
  }
}

// TODO: ADD BOUNDS CHECKING!!!!!!!!!!!
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

