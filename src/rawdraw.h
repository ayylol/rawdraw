#ifndef INCLUDE_RAWDRAW_H_
#define INCLUDE_RAWDRAW_H_

// CONSTANTS AND TYPES
typedef struct {
  uint32_t* buffer;
  uint32_t w, h;
} image_t;

typedef struct {
  uint32_t x, y;
} point_t;

typedef uint32_t color_t;

#define BLACK 0x00000000
#define WHITE 0x00FFFFFF
#define RED   0x00FF0000
#define GREEN 0x0000FF00
#define BLUE  0x000000FF

// PRIVATE FUNCTIONS
static inline uint32_t _get_i(image_t img, uint32_t x, uint32_t y){
  return x+img.w*y; 
}

// INTERFACE
void rawdraw_fill(image_t img, color_t col) {
  for (uint32_t x = 0; x < img.w; x++){
    for (uint32_t y = 0; y < img.h; y++){
      img.buffer[_get_i(img, x, y)] = col;
    }
  }
}

void rawdraw_rect(image_t img, point_t p1, point_t p2, color_t col) {
  p2.x = p2.x < img.w ? p2.x : img.w;
  p2.y = p2.y < img.h ? p2.y : img.h;
  for (uint32_t x = p1.x; x < p2.x; x++){
    for (uint32_t y = p1.y; y < p2.y; y++){
      img.buffer[_get_i(img, x, y)] = col;
    }
  }
}

#endif // INCLUDE_RAWDRAW_H_
