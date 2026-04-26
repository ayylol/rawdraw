#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "rawdraw.h"

#define DRAW_TICK 0.5
#define WIDTH 1420
#define HEIGHT 840
uint32_t buffer[WIDTH*HEIGHT];

color_t colors[8] = {BLACK,WHITE,RED,GREEN,BLUE,MAGENTA,YELLOW,CYAN};

char file_name[64] = "out.ppm";

int32_t save_ppm(char* file_name, uint32_t *buffer, 
                  uint32_t width, uint32_t height);

void draw_frame(image_t img);

int32_t main(int argc, char* argv[]) {
  if (argc == 2){
    strcpy(file_name, argv[1]); 
  }
  image_t img = {.buffer=buffer, .w=WIDTH, .h=HEIGHT};

  Display *display;
  Window window;
  XEvent event;
  int screen_num;

  display = XOpenDisplay(NULL);
  if (display == NULL) {
    fprintf(stderr, "Cannot open display\n");
    exit(1);
  }

  screen_num = DefaultScreen(display);
  window = XCreateSimpleWindow(display, RootWindow(display, screen_num), 240, 110, 1440, 860, 1,
                         BlackPixel(display, screen_num), WhitePixel(display, screen_num));
  XSelectInput(display, window, ExposureMask | KeyPressMask);
  XMapWindow(display, window);

  XClassHint* classHint;
  XStoreName(display, window, "rawdraw");
  classHint = XAllocClassHint();
  if (classHint) {
      classHint->res_name = "rawdraw";
      classHint->res_class = "FLOAT";
  }
  XSetClassHint(display, window, classHint);
  XFree(classHint);

  GC gc = XCreateGC(display, window, 0, NULL);
  Pixmap pm = XCreatePixmap(display, window, WIDTH, HEIGHT, 24);
  XImage *image = XCreateImage(display, DefaultVisual(display, 0), 24, ZPixmap, 0, (char*) img.buffer, WIDTH, HEIGHT, 32, 0); 

  clock_t t;
  double draw_t=DRAW_TICK;
  while (true) {
    clock_t curr_t=clock();
    double delta_t=(double)(curr_t-t)/CLOCKS_PER_SEC;
    t=curr_t;
    draw_t+=delta_t;

    if (draw_t >= DRAW_TICK){
      draw_frame(img);
      draw_t=0.0;
    }
    XPutImage(display, pm, gc, image, 0,0,0,0, WIDTH, HEIGHT);
    XCopyArea(display, pm, window, gc, 0, 0, WIDTH, HEIGHT, 10, 10);

    XNextEvent(display, &event);
    if (event.type == KeyPress){
      if (XLookupKeysym(&event.xkey, 0) == 0xff1b){ // ESC PRESSED
        break;
      }
    }
  }
  XCloseDisplay(display);

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
