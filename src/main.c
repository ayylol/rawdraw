#include "rawdraw.h"
#include <stdint.h>
#include <ncurses.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void init_ncurses();
void init_color_rgb(int32_t id, uint32_t rgb);
void destroy_ncurses();

int32_t main(int argc, char* argv[]) {
  init_ncurses();
  uint8_t braille_char[3]={0xe2, 0xa3, 0xbf};
  int32_t count=0;
  while (1) {
    for (int32_t i=0; i<NUM_COLORS; i++){
      attron(COLOR_PAIR(i));
      mvaddstr(i/32, i%32, (const char *)braille_char);
    }
    refresh();
    usleep(400000);
  }
  destroy_ncurses();
}

void init_ncurses(){
  if (!setlocale(LC_ALL, "C.utf8")){
    printf("LOCALE COULD NOT BE SET\n");
    exit(1);
  }
  initscr();
  setlocale(LC_CTYPE, "C.utf8");
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
  for (int32_t i=0; i<NUM_COLORS; i++){
    init_color_rgb(i, colors[i]);
  }
  clear();
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
