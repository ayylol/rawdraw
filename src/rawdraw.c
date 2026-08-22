#include "rawdraw.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


// PRIVATE FUNCTION IMPLEMENTATIONS
static inline void rawdraw_swap_points(point_t* p1, point_t* p2){
  point_t temp_p=*p1; *p1=*p2; *p2=temp_p;
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
  return dot>=0;
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
 #define clamp(a,l,u) \
   min(max(a,l),u)    \

void rawdraw_tri(image_t img, point_t p1, point_t p2, point_t p3, color_t col){
  point_t lb = {clamp(min(min(p1.x,p2.x),p3.x),0,img.w-1),clamp(min(min(p1.y,p2.y),p3.y),0,img.h-1)};
  point_t ub = {clamp(max(max(p1.x,p2.x),p3.x),0,img.w-1),clamp(max(max(p1.y,p2.y),p3.y),0,img.h-1)};
  for (int32_t x=lb.x; x<ub.x; x++){
    for (int32_t y=lb.y; y<ub.y; y++){
      point_t p4 = (point_t){x,y};
      bool inside=
        (is_left_of(p1,p2,p4)&&is_left_of(p2,p3,p4)&&is_left_of(p3,p1,p4));
        //|| !(is_left_of(p1,p2,p4)||is_left_of(p2,p3,p4)||is_left_of(p3,p1,p4));
      if (inside){ img.buffer[rawdraw_get_i(img, x, y)]=col; }
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

uint32_t colors[NUM_COLORS] =
{
  // xterm default colors.
  0x000000, // COLOR_BLACK
  0xcd0000, // COLOR_BLUE
  0x00cd00, // COLOR_GREEN
  0xcdcd00, // COLOR_CYAN
  0x0000cd, // COLOR_RED
  0xcd00cd, // COLOR_MAGENTA
  0x00cdcd, // COLOR_YELLOW
  0xe5e5e5, // COLOR_WHITE
  0x4d4d4d, // HI COLOR_BLACK
  0xff0000, // HI COLOR_BLUE
  0x00ff00, // HI COLOR_GREEN
  0xffff00, // HI COLOR_CYAN
  0x0000ff, // HI COLOR_RED
  0xff00ff, // HI COLOR_MAGENTA
  0x00ffff, // HI COLOR_YELLOW
  0xffffff, // HI COLOR_WHITE

  // Netscape palette
  0x000000, 0x000033, 0x000066, 0x000099, 0x0000CC, 0x0000FF,
  0x003300, 0x003333, 0x003366, 0x003399, 0x0033CC, 0x0033FF,
  0x006600, 0x006633, 0x006666, 0x006699, 0x0066CC, 0x0066FF,
  0x009900, 0x009933, 0x009966, 0x009999, 0x0099CC, 0x0099FF,
  0x00CC00, 0x00CC33, 0x00CC66, 0x00CC99, 0x00CCCC, 0x00CCFF,
  0x00FF00, 0x00FF33, 0x00FF66, 0x00FF99, 0x00FFCC, 0x00FFFF,
  0x330000, 0x330033, 0x330066, 0x330099, 0x3300CC, 0x3300FF,
  0x333300, 0x333333, 0x333366, 0x333399, 0x3333CC, 0x3333FF,
  0x336600, 0x336633, 0x336666, 0x336699, 0x3366CC, 0x3366FF,
  0x339900, 0x339933, 0x339966, 0x339999, 0x3399CC, 0x3399FF,
  0x33CC00, 0x33CC33, 0x33CC66, 0x33CC99, 0x33CCCC, 0x33CCFF,
  0x33FF00, 0x33FF33, 0x33FF66, 0x33FF99, 0x33FFCC, 0x33FFFF,
  0x660000, 0x660033, 0x660066, 0x660099, 0x6600CC, 0x6600FF,
  0x663300, 0x663333, 0x663366, 0x663399, 0x6633CC, 0x6633FF,
  0x666600, 0x666633, 0x666666, 0x666699, 0x6666CC, 0x6666FF,
  0x669900, 0x669933, 0x669966, 0x669999, 0x6699CC, 0x6699FF,
  0x66CC00, 0x66CC33, 0x66CC66, 0x66CC99, 0x66CCCC, 0x66CCFF,
  0x66FF00, 0x66FF33, 0x66FF66, 0x66FF99, 0x66FFCC, 0x66FFFF,
  0x990000, 0x990033, 0x990066, 0x990099, 0x9900CC, 0x9900FF,
  0x993300, 0x993333, 0x993366, 0x993399, 0x9933CC, 0x9933FF,
  0x996600, 0x996633, 0x996666, 0x996699, 0x9966CC, 0x9966FF,
  0x999900, 0x999933, 0x999966, 0x999999, 0x9999CC, 0x9999FF,
  0x99CC00, 0x99CC33, 0x99CC66, 0x99CC99, 0x99CCCC, 0x99CCFF,
  0x99FF00, 0x99FF33, 0x99FF66, 0x99FF99, 0x99FFCC, 0x99FFFF,
  0xCC0000, 0xCC0033, 0xCC0066, 0xCC0099, 0xCC00CC, 0xCC00FF,
  0xCC3300, 0xCC3333, 0xCC3366, 0xCC3399, 0xCC33CC, 0xCC33FF,
  0xCC6600, 0xCC6633, 0xCC6666, 0xCC6699, 0xCC66CC, 0xCC66FF,
  0xCC9900, 0xCC9933, 0xCC9966, 0xCC9999, 0xCC99CC, 0xCC99FF,
  0xCCCC00, 0xCCCC33, 0xCCCC66, 0xCCCC99, 0xCCCCCC, 0xCCCCFF,
  0xCCFF00, 0xCCFF33, 0xCCFF66, 0xCCFF99, 0xCCFFCC, 0xCCFFFF,
  0xFF0000, 0xFF0033, 0xFF0066, 0xFF0099, 0xFF00CC, 0xFF00FF,
  0xFF3300, 0xFF3333, 0xFF3366, 0xFF3399, 0xFF33CC, 0xFF33FF,
  0xFF6600, 0xFF6633, 0xFF6666, 0xFF6699, 0xFF66CC, 0xFF66FF,
  0xFF9900, 0xFF9933, 0xFF9966, 0xFF9999, 0xFF99CC, 0xFF99FF,
  0xFFCC00, 0xFFCC33, 0xFFCC66, 0xFFCC99, 0xFFCCCC, 0xFFCCFF,
  0xFFFF00, 0xFFFF33, 0xFFFF66, 0xFFFF99, 0xFFFFCC, 0xFFFFFF,

  // Shades of gray
  0x080808, 0x121212, 0x1C1C1C, 0x262626,
  0x303030, 0x3A3A3A, 0x444444, 0x4E4E4E,
  0x585858, 0x626262, 0x6C6C6C, 0x767676,
  0x808080, 0x8A8A8A, 0x949494, 0x9E9E9E,
  0xA8A8A8, 0xB2B2B2, 0xBCBCBC, 0xC6C6C6,
  0xD0D0D0, 0xDADADA, 0xE4E4E4, 0xEEEEEE,

};
