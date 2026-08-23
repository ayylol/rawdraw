#include "rawdraw.h"
#include <stdint.h>
#include <ncurses.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PIXEL_WIDTH 284
#define PIXEL_HEIGHT 160
uint32_t g_pixel_buffer[PIXEL_WIDTH*PIXEL_HEIGHT];
canvas_t g_canvas = {g_pixel_buffer, PIXEL_WIDTH, PIXEL_HEIGHT};

void init_ncurses();
void init_color_rgb(int32_t id, uint32_t rgb);
void destroy_ncurses();
void drain_events();
void draw_frame(canvas_t canvas);
void present(canvas_t canvas);

int32_t main(int argc, char* argv[]) {
  init_ncurses();
  int32_t count=0;
  while (1) {
    draw_frame(g_canvas);
    present(g_canvas);
    refresh();
    usleep(20*16000);
  }
  destroy_ncurses();
}

void init_ncurses(){
  setlocale(LC_CTYPE, "C.utf8");
  initscr();
  cbreak();
  keypad(stdscr, true);
  noecho();
  nodelay(stdscr, TRUE);
  keypad(stdscr, TRUE);
  set_escdelay(0);

  // For mouse movement
  // https://gist.github.com/sylt/93d3f7b77e7f3a881603
  mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
  printf("\033[?1003h\n");

  fflush(stdout);
  mouseinterval(0);

  start_color();
  for (int32_t i=16; i<COLOR_PALETTE_COUNT; i++){
    init_color_rgb(i, g_color_palette[i]);
  }
  clear();
}

void draw_frame(canvas_t canvas){
  rawdraw_fill(canvas, g_color_palette[16]);
  point_t tri[3];
  for (int i=0; i<1; i++){
    for (int j=0; j<3; j++){
      int32_t x = rand()%canvas.w;
      int32_t y = rand()%canvas.h;
      tri[j]=(point_t){.x=x,.y=y};
    }
    rawdraw_tri(canvas, tri[0], tri[1], tri[2], g_color_palette[154]);
  }
}

void present(canvas_t canvas){
  int32_t terminal_width=canvas.w/2;
  int32_t terminal_height=canvas.h/4;
  // TODO: ASSERT THE SIZE IS FINE
  for (int32_t x=0; x<terminal_width; x++){
    for (int32_t y=0; y<terminal_height; y++){
      uint32_t is_color0 = 0 != canvas.buffer[rawdraw_get_i(canvas, (x*2)+0, (y*4)+0)];
      uint32_t is_color1 = 0 != canvas.buffer[rawdraw_get_i(canvas, (x*2)+1, (y*4)+0)];
      uint32_t is_color2 = 0 != canvas.buffer[rawdraw_get_i(canvas, (x*2)+0, (y*4)+1)];
      uint32_t is_color3 = 0 != canvas.buffer[rawdraw_get_i(canvas, (x*2)+1, (y*4)+1)];
      uint32_t is_color4 = 0 != canvas.buffer[rawdraw_get_i(canvas, (x*2)+0, (y*4)+2)];
      uint32_t is_color5 = 0 != canvas.buffer[rawdraw_get_i(canvas, (x*2)+1, (y*4)+2)];
      uint32_t is_color6 = 0 != canvas.buffer[rawdraw_get_i(canvas, (x*2)+0, (y*4)+3)];
      uint32_t is_color7 = 0 != canvas.buffer[rawdraw_get_i(canvas, (x*2)+1, (y*4)+3)];
      if (is_color0 || is_color1 || is_color2 || is_color3 || is_color4 || is_color5 || is_color6 || is_color7) {
          attron(COLOR_PAIR(154));
          uint8_t braille_char[3]={0xE2, 0xA3, 0xBF};
          mvaddstr(y, x, (const char *)braille_char);
      } else {
          attron(COLOR_PAIR(17));
          uint8_t braille_char[3]={0xE2, 0xA0, 0x81};
          mvaddstr(y, x, (const char *)braille_char);
      }
    }
  }
}

void
init_color_rgb(int32_t id, uint32_t rgb) {
  uint32_t b = ((rgb & 0xff) * 1000 ) / 255;
  uint32_t g = (((rgb >> 8) & 0xff) * 1000 ) / 255;
  uint32_t r = (((rgb >> 16) & 0xff) * 1000 ) / 255;

  init_color(id, r, g, b);
  init_pair(id, id, COLOR_BLACK);
}

void destroy_ncurses(){
  printf("\033[?1003l\n"); // Disable mouse movement events, as l = low
  endwin();
}
