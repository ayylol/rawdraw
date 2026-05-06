#include "xwrapper.h"

#include <stdlib.h>
#include <stdio.h>

// TODO: gotta test how ergonomic this wrapper is I guess
void X11Init(XWrapper *x_ctx){
  char* name="rawdraw";
  char* class="FLOAT";

  x_ctx->display = XOpenDisplay(NULL);
  if (x_ctx->display == NULL) {
    fprintf(stderr, "Cannot open display\n");
    exit(1);
  }

  int screen_num = DefaultScreen(x_ctx->display);
  x_ctx->window = XCreateSimpleWindow(
      x_ctx->display, RootWindow(x_ctx->display, screen_num),
      240, 110, 1440, 860, 1, BlackPixel(x_ctx->display, screen_num),
      WhitePixel(x_ctx->display, screen_num));
  XSelectInput(x_ctx->display, x_ctx->window, ExposureMask | KeyPressMask);
  XMapWindow(x_ctx->display, x_ctx->window);

  XClassHint* classHint;
  XStoreName(x_ctx->display, x_ctx->window, name);
  classHint = XAllocClassHint();
  if (classHint) {
      classHint->res_name = name;
      classHint->res_class = class;
  }
  XSetClassHint(x_ctx->display, x_ctx->window, classHint);
  XFree(classHint);
  x_ctx->image_binded=false;
}
void X11Destroy(XWrapper *x_ctx){
  if (x_ctx->image_binded){
    XFreePixmap(x_ctx->display, x_ctx->pm);
    XFreeGC(x_ctx->display, x_ctx->gc);
    // XDestroyImage also frees the buffer which we don't know if we can free
    free(x_ctx->image);
  }
  XCloseDisplay(x_ctx->display);
}
void X11BindBuffer(XWrapper *x_ctx, void* buffer, int w, int h){
  x_ctx->w=w;
  x_ctx->h=h;
  x_ctx->gc = XCreateGC(x_ctx->display, x_ctx->window, 0, NULL);
  x_ctx->pm = XCreatePixmap(
      x_ctx->display, x_ctx->window, x_ctx->w, x_ctx->h, 24);
  x_ctx->image = XCreateImage(
      x_ctx->display, DefaultVisual(x_ctx->display, 0), 24, ZPixmap, 0, 
      (char*) buffer, x_ctx->w, x_ctx->h, 32, 0); 
  x_ctx->image_binded=true;
}
void X11DrawFrame(XWrapper *x_ctx){
  XPutImage(x_ctx->display, x_ctx->pm, x_ctx->gc, x_ctx->image, 0,0,0,0, 
      x_ctx->w, x_ctx->h);
  XCopyArea(x_ctx->display, x_ctx->pm, x_ctx->window, x_ctx->gc, 0, 0, 
      x_ctx->w, x_ctx->h, 10, 10);
}
void X11PollEvents(XWrapper *x_ctx, Events *events){
  events->exit=false;
  XEvent xEvent;
  XNextEvent(x_ctx->display, &xEvent);
  if (xEvent.type == KeyPress){
    if (XLookupKeysym(&xEvent.xkey, 0) == 0xff1b){ // ESC PRESSED
      events->exit=true;
    }
  }
}
