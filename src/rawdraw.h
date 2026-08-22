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


// INTERFACE
void rawdraw_fill(image_t img, color_t col);
void rawdraw_rect(image_t img, point_t p1, point_t p2, color_t col);
void rawdraw_point(image_t img, point_t p, int32_t dim, color_t col);
void rawdraw_line(image_t img, point_t p1, point_t p2, color_t col);
void rawdraw_tri(image_t img, point_t p1, point_t p2, point_t p3, color_t col);
static inline uint32_t rawdraw_get_i(image_t img, int32_t x, int32_t y){
  return x+img.w*y;
}

#define NUM_COLORS 256
extern color_t colors[NUM_COLORS];

#endif // INCLUDE_RAWDRAW_H_

