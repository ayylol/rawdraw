#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rawdraw.h"

#define WIDTH 400
#define HEIGHT 300
uint32_t buffer[WIDTH*HEIGHT];

char file_name[64] = "out.ppm";

int32_t save_ppm(char* file_name, uint32_t *buffer, 
                  uint32_t width, uint32_t height);

int32_t main(int argc, char* argv[]) {
  if (argc == 2){
    strcpy(file_name, argv[1]); 
  }
  image_t img = {.buffer=buffer, .w=WIDTH, .h=HEIGHT};
  rawdraw_fill(img, BLACK);
  /*
  point_t m = {.x=WIDTH/2, .y=HEIGHT/2};
  point_t p1 = {.x=30, .y=20};
  point_t p2 = {.x=10, .y=120};
  rawdraw_line(img, m, (point_t){WIDTH, HEIGHT}, RED);
  rawdraw_line(img, m, (point_t){WIDTH, 0}, GREEN);
  rawdraw_line(img, m, (point_t){0, HEIGHT}, BLUE);
  rawdraw_line(img, m, (point_t){0, 0}, WHITE);
  rawdraw_tri(img, m, p1, p2, RED);
  rawdraw_rect(img, m, m, GREEN);
  rawdraw_rect(img, p1, p1, BLUE);
  rawdraw_rect(img, p2, p2, WHITE);
  */
  point_t tri[3];
  for (int i=0; i<10; i++){
    for (int j=0; j<3; j++){
      int32_t x = rand()%img.w;
      int32_t y = rand()%img.h;
      tri[j]=(point_t){.x=x,.y=y};
    }
    rawdraw_tri(img, tri[0], tri[1], tri[2], rand());
  }
  save_ppm(file_name, img.buffer, img.w, img.h);
  return 0;
}

int32_t save_ppm(char* file_name, uint32_t *buffer,
                  uint32_t width, uint32_t height) {
  FILE* output_file = fopen(file_name, "w");
  if (output_file == NULL) {
    printf("ERROR OPENING %s\n", file_name);
    return 1;
  }
  fprintf(output_file, "P3\n%d %d\n255\n", width, height);
  for (uint32_t i=0; i < width*height; i++){
    uint8_t r=(buffer[i]>>8*2)&0xFF;
    uint8_t g=(buffer[i]>>8*1)&0xFF;
    uint8_t b=(buffer[i]>>8*0)&0xFF;
    fprintf(output_file, "%d %d %d\n", r, g, b);
  }
  fclose(output_file);
  return 0;
}
