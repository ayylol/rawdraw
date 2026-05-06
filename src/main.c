#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "rawdraw.h"
#include "xwrapper.h"

#define DRAW_TICK 0.5
#define WIDTH 1420
#define HEIGHT 840
uint32_t buffer[WIDTH*HEIGHT];
char file_name[64] = "out.ppm";

int32_t save_ppm(char* file_name, uint32_t *buffer, 
                  uint32_t width, uint32_t height);
void draw_frame(image_t img);

int32_t main(int argc, char* argv[]) {
  if (argc == 2){
    strcpy(file_name, argv[1]); 
  }
  image_t img = {.buffer=buffer, .w=WIDTH, .h=HEIGHT};

  XWrapper x_ctx;
  X11Init(&x_ctx);
  X11BindBuffer(&x_ctx, img.buffer, WIDTH, HEIGHT);

  clock_t t=clock();
  double draw_t=DRAW_TICK;
  Events events;
  while (true) {
    clock_t curr_t=clock();
    double delta_t=(double)(curr_t-t)/CLOCKS_PER_SEC;
    t=curr_t;
    draw_t+=delta_t;
    if (draw_t >= DRAW_TICK){
      draw_frame(img);
      draw_t=0.0;
    }
    X11DrawFrame(&x_ctx);
    X11PollEvents(&x_ctx, &events);
    if (events.exit) break;
  }
  X11Destroy(&x_ctx);
  return 0;
}

void draw_frame(image_t img){
  rawdraw_fill(img, BLACK);
  point_t tri[3];
  for (int i=0; i<4; i++){
    for (int j=0; j<3; j++){
      int32_t x = rand()%img.w;
      int32_t y = rand()%img.h;
      tri[j]=(point_t){.x=x,.y=y};
    }
    rawdraw_tri(img, tri[0], tri[1], tri[2], colors[rand()%8]);
  }
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
