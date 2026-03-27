#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "rawdraw.h"

#define WIDTH 174
#define HEIGHT 44
uint32_t buffer[WIDTH*HEIGHT];

color_t colors[8] = {BLACK,WHITE,RED,GREEN,BLUE,MAGENTA,YELLOW,CYAN};

char file_name[64] = "out.ppm";

int32_t save_ppm(char* file_name, uint32_t *buffer, 
                  uint32_t width, uint32_t height);

void init_ncurses();
void draw_frame(image_t img);

int32_t main(int argc, char* argv[]) {
  if (argc == 2){
    strcpy(file_name, argv[1]); 
  }
  image_t img = {.buffer=buffer, .w=WIDTH, .h=HEIGHT};

  char ch=' ';
  init_ncurses();
  while( ch != 'q' ) {
      draw_frame(img);
      for (int32_t i=0; i < img.w; i++){
        for (int32_t j=0; j < img.h; j++){
          ch='#';
          int8_t col_pair=1;
          switch(img.buffer[rawdraw_get_i(img, i, j)]){
            case WHITE:   col_pair=1; break;
            case RED:     col_pair=2; break;
            case GREEN:   col_pair=3; break;
            case BLUE:    col_pair=4; break;
            case MAGENTA: col_pair=5; break;
            case YELLOW:  col_pair=6; break;
            case CYAN:    col_pair=7; break;
            case BLACK:   ch=' ';     break;
          };
          attron(COLOR_PAIR(col_pair));
          mvaddch(j,i, ch|A_BOLD);
        }
      }
      refresh();
      save_ppm(file_name, img.buffer, img.w, img.h);
      ch = getch();
  }
  endwin();

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

void init_ncurses(){
  initscr();
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
