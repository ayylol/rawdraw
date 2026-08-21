#include <locale.h>
#include <ncurses.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "linalg.h"
#include "rawdraw.h"

#define DRAW_TICK 0.0167
#define WIDTH 100
#define HEIGHT 50
uint32_t buffer[WIDTH*HEIGHT];
char file_name[64] = "out.ppm";

void init_ncurses();
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
  clock_t t=clock();
  double draw_t=DRAW_TICK;
  
  init_ncurses();
  uint8_t ch = ' ';
  while( ch != 'q' ) {
    clock_t curr_t=clock();
    double delta_t=(double)(curr_t-t)/CLOCKS_PER_SEC;
    t=curr_t;
    draw_t+=delta_t;
    if (draw_t >= DRAW_TICK){
      draw_frame(img);
      draw_t=0.0;
    }
    for (int32_t i=0; i < img.w; i++){
      for (int32_t j=0; j < img.h; j++){
        ch='#';
        //ch=L'\u28FF';
        int8_t col_pair=1;
        switch(img.buffer[rawdraw_get_i(img, i, j)]){
          case X000_BLACK:  ch=' ';     break;
          case X001_MAROON: col_pair=1; break;
          case X002_GREEN:  col_pair=2; break;
          case X003_OLIVE:  col_pair=3; break;
          case X004_NAVY:   col_pair=4; break;
          case X006_TEAL:   col_pair=5; break;
          case X007_SILVER: col_pair=6; break;
          case X008_GREY:   col_pair=7; break;
        };
        attron(COLOR_PAIR(col_pair));
        if (ch == ' ') {
          mvaddch(j, i, ch);
        } else {
          mvaddstr(j, i, "\xE2\xA3\xBF");
        }
      }
    }
    refresh();
    save_ppm(file_name, img.buffer, img.w, img.h);
    ch = getch();
  }
  /*
  while (true) {
    clock_t curr_t=clock();
    double delta_t=(double)(curr_t-t)/CLOCKS_PER_SEC;
    t=curr_t;
    draw_t+=delta_t;
    if (draw_t >= DRAW_TICK){
      draw_frame(img);
      draw_t=0.0;
    }
    if (events.exit) break;
  }
  */
  return 0;
}

void init_ncurses(){
  initscr();
  setlocale(LC_ALL, "C.utf8");
  start_color();
  cbreak();
  keypad(stdscr, true);
  noecho();
  curs_set(0);
  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_RED, COLOR_BLACK);
  init_pair(3, COLOR_GREEN, COLOR_BLACK);
  init_pair(4, COLOR_BLUE, COLOR_BLACK);
  init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(6, COLOR_YELLOW, COLOR_BLACK);
  init_pair(7, COLOR_CYAN, COLOR_BLACK);

}

int s=0;
int s1=100;
bool growing=true;
void draw_frame(image_t img){
  rawdraw_fill(img, X000_BLACK);
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
    rawdraw_tri(img, to_point(v3_to_v2(quad[i])),   to_point(v3_to_v2(quad[i+1])),  to_point(v3_to_v2(quad[i+2])),X007_SILVER);
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
