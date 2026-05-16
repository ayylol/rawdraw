#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "linalg.h"
#include "rawdraw.h"
#include "xwrapper.h"

#define DRAW_TICK 0.0167
#define WIDTH 1420
#define HEIGHT 840
uint32_t buffer[WIDTH*HEIGHT];
char file_name[64] = "out.ppm";

int32_t save_ppm(char* file_name, uint32_t *buffer, 
                  uint32_t width, uint32_t height);
void draw_frame(image_t img);

static inline ivec2_t to_ivec2(point_t a){ return (ivec2_t){.x=a.x,.y=a.y}; }
static inline point_t to_point(ivec2_t a){ return (point_t){.x=a.x,.y=a.y}; }

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

int s=0;
int s1=100;
bool growing=true;
void draw_frame(image_t img){
  rawdraw_fill(img, BLACK);
  ivec3_t quad[6*6]={
    // FRONT
    {.x=-1,.y=-1,.z=1}, {.x=1,.y=-1,.z=1}, {.x=1,.y=1,.z=1},  
    {.x=-1,.y=-1,.z=1}, {.x=1,.y=1,.z=1},  {.x=-1,.y=1,.z=1}, 
    // BACK
    {.x=-1,.y=-1,.z=-1},  {.x=1,.y=1,.z=-1},  {.x=1,.y=-1,.z=-1}, 
    {.x=-1,.y=-1,.z=-1},  {.x=-1,.y=1,.z=-1}, {.x=1,.y=1,.z=-1},
    // LEFT
    {.x=-1,.y=-1,.z=-1},   {.x=-1,.y=1,.z=1}, {.x=-1,.y=1,.z=-1}, 
    {.x=-1,.y=-1,.z=-1},    {.x=-1,.y=-1,.z=1}, {.x=-1,.y=1,.z=1},
    // RIGHT
    {.x=1,.y=-1,.z=1},  {.x=1,.y=1,.z=-1},  {.x=1,.y=1,.z=1},
    {.x=1,.y=-1,.z=1},  {.x=1,.y=-1,.z=-1},  {.x=1,.y=1,.z=-1},
  };
  ivec3_t center= {.x=img.w/2,.y=img.h/2,.z=0};
  ivec3_t camera= {.x=0,.y=0,.z=-150};
  (void) camera;
  imat3_t rot=rot_y_m3(s);
  //imat3_t scale=scale_m3(s1);
  for (int i=0; i<6*4;i++){
    //quad[i]=//add_v3(div_sv3(SIN_RANGE, mul_mv3(model,quad[i])), center);
    //quad[i]=mul_mv3(scale,quad[i]);
    quad[i]=mul_sv3(s1,quad[i]);
    quad[i]=div_sv3(SIN_RANGE, mul_mv3(rot,quad[i]));
    quad[i]=add_v3(quad[i],camera);
    quad[i]=mul_sv3(8192*2,quad[i]);
    quad[i]=div_sv3(quad[i].z/(4*8192),quad[i]);
    quad[i]=div_sv3(8192*2,quad[i]);
    quad[i]=mul_sv3(20,quad[i]);
    quad[i]=add_v3(quad[i],center);
    //quad[i]=mul_sv3(2048,quad[i]);
    //quad[i]=div_sv3(2048,quad[i]);
    //quad[i]=mul_mv3(scale,quad[i]);

    //quad[i]=mul_sv3(quad[i].z,quad[i]);
    //quad[i]=div_sv3(quad[i].z,quad[i]);
  }
  for (int i=0;i<23;i+=3){
    rawdraw_tri(img, to_point(v3_to_v2(quad[i])),   to_point(v3_to_v2(quad[i+1])),  to_point(v3_to_v2(quad[i+2])),colors[2+(i/6)%6]);
  }
  s+=i16_PI/300;
  /*
  if (growing){
    s1+=1;
  }else{
    s1-=1;
  }
  if (s1>250 || s1<10) growing=!growing;
  */
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
