#include "rawdraw.h"
#include <assert.h>
#include <string.h> // TODO: REMEMBER TO REMOVE THIS SHIT !!!!
#include <stdint.h>
#include <ncurses.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PIXEL_WIDTH 152*2
#define PIXEL_HEIGHT 43*4
uint32_t g_pixel_buffer[PIXEL_WIDTH*PIXEL_HEIGHT];
canvas_t g_canvas = {g_pixel_buffer, PIXEL_WIDTH, PIXEL_HEIGHT};

void init_ncurses();
void init_color_rgb(int32_t id, uint32_t rgb);
void destroy_ncurses();
void drain_events();
void draw_frame(canvas_t canvas);
void present(canvas_t canvas);
void save_ppm(uint32_t *buffer, uint32_t width, uint32_t height);
void load_ppm(uint32_t *buffer, const char* path);

int32_t main(int argc, char* argv[]) {
  //draw_frame(g_canvas);
  rawdraw_fill(g_canvas, g_color_palette[16]);
  //load_ppm(g_canvas.buffer, "./doom.ppm");
  //save_ppm(g_canvas.buffer, g_canvas.w, g_canvas.h);
  init_ncurses();
  int32_t count=0;
  while (1) {
    draw_frame(g_canvas);
    present(g_canvas);
    refresh();
    usleep(300*16000);
  }
  destroy_ncurses();
}

void init_ncurses(){
  setlocale(LC_CTYPE, "C.utf8");
  //setlocale(LC_ALL, "");
  initscr();
  cbreak();
  keypad(stdscr, true);
  noecho();
  nodelay(stdscr, TRUE);
  keypad(stdscr, TRUE);
  set_escdelay(0);
  curs_set(0);

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
  for (int i=0; i<3; i++){
    for (int j=0; j<3; j++){
      int32_t x = rand()%canvas.w;
      int32_t y = rand()%canvas.h;
      tri[j]=(point_t){.x=x,.y=y};
    }
    rawdraw_tri(canvas, tri[0], tri[1], tri[2], g_color_palette[rand()%216+16]);
  }
  /*
  point_t tri[3] = {
    (point_t){canvas.w/2,25},
    (point_t){3*canvas.w/4,canvas.h-25},
    (point_t){canvas.w/4,canvas.h-25},
  };
  rawdraw_tri(canvas, tri[0], tri[1], tri[2], g_color_palette[154]);
  */
}

void present(canvas_t canvas){
  int32_t terminal_width=canvas.w/2;
  int32_t terminal_height=canvas.h/4;
  assert(terminal_width*2==canvas.w);
  assert(terminal_height*4==canvas.h);
  for (int32_t x=0; x<terminal_width; x++){
    for (int32_t y=0; y<terminal_height; y++){
      // NOTE: !!! Ordered in the same manner as the unicode braille characters !!!
      uint32_t is_color0 = 0 != canvas.buffer[rawdraw_get_i(canvas, (x*2)+0, (y*4)+0)];
      uint32_t is_color1 = 0 != canvas.buffer[rawdraw_get_i(canvas, (x*2)+0, (y*4)+1)];
      uint32_t is_color2 = 0 != canvas.buffer[rawdraw_get_i(canvas, (x*2)+0, (y*4)+2)];
      uint32_t is_color3 = 0 != canvas.buffer[rawdraw_get_i(canvas, (x*2)+1, (y*4)+0)];
      uint32_t is_color4 = 0 != canvas.buffer[rawdraw_get_i(canvas, (x*2)+1, (y*4)+1)];
      uint32_t is_color5 = 0 != canvas.buffer[rawdraw_get_i(canvas, (x*2)+1, (y*4)+2)];
      uint32_t is_color6 = 0 != canvas.buffer[rawdraw_get_i(canvas, (x*2)+0, (y*4)+3)];
      uint32_t is_color7 = 0 != canvas.buffer[rawdraw_get_i(canvas, (x*2)+1, (y*4)+3)];
      // Get Byte Representing Bit Pattern
      uint8_t bit_pattern =
        is_color0 << 0 | is_color3 << 3 |
        is_color1 << 1 | is_color4 << 4 |
        is_color2 << 2 | is_color5 << 5 |
        is_color6 << 6 | is_color7 << 7;
      // Convert Bit Pattern to Appropriate UTF-8 Encoding
      uint8_t braille_char[4]={};
      braille_char[0] = 0xE2;
      braille_char[1] = 0xA0 | (bit_pattern >> 6);
      braille_char[2] = 0x80 | (bit_pattern & 0x3F);
      // Average Color
      uint32_t color_count = is_color0 + is_color1 + is_color2 + is_color3 + is_color4 + is_color5 + is_color6 + is_color7;
      if (color_count == 0) {
        mvaddch(y, x, ' ');
        continue;
      }

      uint32_t color0 = canvas.buffer[rawdraw_get_i(canvas, (x*2)+0, (y*4)+0)];
      uint32_t color1 = canvas.buffer[rawdraw_get_i(canvas, (x*2)+0, (y*4)+1)];
      uint32_t color2 = canvas.buffer[rawdraw_get_i(canvas, (x*2)+0, (y*4)+2)];
      uint32_t color3 = canvas.buffer[rawdraw_get_i(canvas, (x*2)+1, (y*4)+0)];
      uint32_t color4 = canvas.buffer[rawdraw_get_i(canvas, (x*2)+1, (y*4)+1)];
      uint32_t color5 = canvas.buffer[rawdraw_get_i(canvas, (x*2)+1, (y*4)+2)];
      uint32_t color6 = canvas.buffer[rawdraw_get_i(canvas, (x*2)+0, (y*4)+3)];
      uint32_t color7 = canvas.buffer[rawdraw_get_i(canvas, (x*2)+1, (y*4)+3)];

      uint32_t channel_averages[3] = {
        (rawdraw_channel_red(color0) + rawdraw_channel_red(color1) +
         rawdraw_channel_red(color2) + rawdraw_channel_red(color3) +
         rawdraw_channel_red(color4) + rawdraw_channel_red(color5) +
         rawdraw_channel_red(color6) + rawdraw_channel_red(color7)) / color_count,

        (rawdraw_channel_blue(color0) + rawdraw_channel_blue(color1) +
         rawdraw_channel_blue(color2) + rawdraw_channel_blue(color3) +
         rawdraw_channel_blue(color4) + rawdraw_channel_blue(color5) +
         rawdraw_channel_blue(color6) + rawdraw_channel_blue(color7)) / color_count,

        (rawdraw_channel_green(color0) + rawdraw_channel_green(color1) +
         rawdraw_channel_green(color2) + rawdraw_channel_green(color3) +
         rawdraw_channel_green(color4) + rawdraw_channel_green(color5) +
         rawdraw_channel_green(color6) + rawdraw_channel_green(color7)) / color_count
      };
      assert(channel_averages[0]<=0xFF);
      assert(channel_averages[1]<=0xFF);
      assert(channel_averages[2]<=0xFF);

      uint8_t channel_indexes[3];
      for (uint32_t i=0; i<3; i++){
        if      (channel_averages[i] < 0x19*1) { channel_indexes[i]=0; } 
        else if (channel_averages[i] < 0x19*3) { channel_indexes[i]=1; }
        else if (channel_averages[i] < 0x19*5) { channel_indexes[i]=2; }
        else if (channel_averages[i] < 0x19*7) { channel_indexes[i]=3; }
        else if (channel_averages[i] < 0x19*9) { channel_indexes[i]=4; }
        else                                   { channel_indexes[i]=5; }
      }
      assert(channel_indexes[0]<=5);
      assert(channel_indexes[1]<=5);
      assert(channel_indexes[2]<=5);
      uint32_t color_index=(channel_indexes[0]*6*6 + channel_indexes[1]*6 + channel_indexes[2])+16;
      
      attron(COLOR_PAIR(color_index));

      mvaddstr(y, x, (const char *)braille_char);
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

// NOTE: NOT DOING ANY CHECKING CUZ ITS JUST TO TEST
// JANKIEST PPM LOADER KNOWN TO MAN!
void load_ppm(uint32_t *buffer, const char* path){
  FILE* f = fopen(path, "r");
  if (f == NULL) {
    printf("The file could not open.");
    return;
  }
  char string_buffer[1024];
  fgets(string_buffer, 1024, f); // Get P3 out of the way
  fgets(string_buffer, 1024, f);
  int32_t x, y;
  sscanf(string_buffer, "%d %d\n", &x, &y);
  //printf("%d %d\n", x, y);
  int32_t counter=0;
  fgets(string_buffer, 1024, f); // Get Color Depth out of the way
  while (1) {
    fgets(string_buffer, 1024, f);
    //printf(string_buffer);
    char *ptr = strtok(string_buffer, " ");
    while (ptr != NULL){
      uint8_t color_channel;
      sscanf(ptr, "%hhd", &color_channel);
      uint8_t offset;
      if (counter % 3 == 0){ // RED
        offset=2;
      } else if (counter % 3 == 1){ // GREEN
        offset=1;
      } else if (counter % 3 == 2){ // BLUE
        offset=0;
      }
      buffer[counter/3] = buffer[counter/3] | color_channel<<(8*offset);
      counter++;
      ptr = strtok(NULL, " ");
    }
    if (counter == (x*y)*3) break;
  }

  fclose(f);
  
}

void save_ppm(uint32_t *buffer, uint32_t width, uint32_t height){
  printf("P3\n%d %d\n255\n", width, height);
  for (uint32_t i=0; i < width*height; i++){
    uint8_t r=rawdraw_channel_red(buffer[i]);
    uint8_t g=rawdraw_channel_green(buffer[i]);
    uint8_t b=rawdraw_channel_blue(buffer[i]);
    printf("%d %d %d\n", r, g, b); 
  }
}
