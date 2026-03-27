#ifndef INCLUDE_RAWDRAW_H_
#define INCLUDE_RAWDRAW_H_

#include <stdint.h>

// CONSTANTS AND TYPES
typedef struct {
  uint32_t* buffer;
  int32_t w, h;
} image_t;

typedef struct {
  int32_t x, y;
} point_t;

typedef uint32_t color_t;

#define BLACK   0x00000000
#define WHITE   0x00FFFFFF
#define RED     0x00FF0000
#define GREEN   0x0000FF00
#define BLUE    0x000000FF
#define MAGENTA 0x00FF00FF
#define YELLOW  0x00FFFF00
#define CYAN    0x0000FFFF

// INTERFACE
void rawdraw_fill(image_t img, color_t col);
void rawdraw_rect(image_t img, point_t p1, point_t p2, color_t col);
void rawdraw_line(image_t img, point_t p1, point_t p2, color_t col);
void rawdraw_tri(image_t img, point_t p1, point_t p2, point_t p3, color_t col);
static inline uint32_t rawdraw_get_i(image_t img, int32_t x, int32_t y){
  return x+img.w*y;
}

#endif // INCLUDE_RAWDRAW_H_
