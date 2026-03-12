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

void _rawdraw_line_low(image_t img, point_t p1, point_t p2, color_t col);
void _rawdraw_line_high(image_t img, point_t p1, point_t p2, color_t col);

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
  if (abs((int32_t)p2.y - p1.y) < abs((int32_t)p2.x - p1.x))
    if (p1.x > p2.x){
      _rawdraw_line_low(img, (point_t){p2.x, p2.y}, (point_t){p1.x, p1.y}, col);
    } else {
      _rawdraw_line_low(img, (point_t){p1.x, p1.y}, (point_t){p2.x, p2.y}, col);
    }
  else {
    if (p1.y > p2.y){
      _rawdraw_line_high(img, (point_t){p2.x, p2.y}, (point_t){p1.x, p1.y}, col);
    } else { 
      _rawdraw_line_high(img, (point_t){p1.x, p1.y}, (point_t){p2.x, p2.y}, col);
    }
  }
}

// PRIVATE FUNCTION IMPLEMENTATIONS
static inline uint32_t _rawdraw_get_i(image_t img, uint16_t x, uint16_t y){
  return x+img.w*y; 
}

void _rawdraw_line_low(image_t img, point_t p1, point_t p2, color_t col){
  int32_t dx=p2.x-p1.x;
  int32_t dy=p2.y-p1.y;
  int16_t yi=1;
  if (dy<0){
    yi=-1;
    dy=-dy;
  }
  int32_t D=2*dy-dx;
  int16_t y=p1.y;
  for (int x=p1.x; x<p2.x; x++){
      img.buffer[_rawdraw_get_i(img, x, y)] = col;
      if (D>0){
        y=y+yi;
        D+=2*(dy-dx);
      } else {
        D+=2*dy;
      }
  }
}
void _rawdraw_line_high(image_t img, point_t p1, point_t p2, color_t col){
  int32_t dx=p2.x-p1.x;
  int32_t dy=p2.y-p1.y;
  int16_t xi=1;
  if (dx<0){
    xi=-1;
    dx=-dx;
  }
  int32_t D=2*dx-dy;
  int16_t x=p1.x;
  for (int y=p1.y; x<p2.y; y++){
      img.buffer[_rawdraw_get_i(img, x, y)] = col;
      if (D>0){
        x=x+xi;
        D+=2*(dx-dy);
      } else {
        D+=2*dx;
      }
  }
}

#endif // INCLUDE_RAWDRAW_H_
