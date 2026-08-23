#ifndef INCLUDE_RAWDRAW_H_
#define INCLUDE_RAWDRAW_H_

#include <stdint.h>

// CONSTANTS AND TYPES
typedef struct {
  uint32_t* buffer;
  int32_t w, h;
} canvas_t;

typedef struct {
  int32_t x, y;
} point_t;

typedef uint32_t color_t;


// INTERFACE
void rawdraw_fill(canvas_t canvas, color_t col);
void rawdraw_rect(canvas_t canvas, point_t p1, point_t p2, color_t col);
void rawdraw_point(canvas_t canvas, point_t p, int32_t dim, color_t col);
void rawdraw_line(canvas_t canvas, point_t p1, point_t p2, color_t col);
void rawdraw_tri(canvas_t canvas, point_t p1, point_t p2, point_t p3, color_t col);
static inline uint32_t rawdraw_get_i(canvas_t canvas, int32_t x, int32_t y){
  return x+canvas.w*y;
}

#define COLOR_PALETTE_COUNT 256
extern color_t g_color_palette[COLOR_PALETTE_COUNT];

#endif // INCLUDE_RAWDRAW_H_

