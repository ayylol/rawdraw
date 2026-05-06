#ifndef INCLUDE_XWRAPPER_H_
#define INCLUDE_XWRAPPER_H_

#include <X11/Xlib.h>
#include <X11/Xutil.h>

typedef struct {
  Display *display;
  Window window;
  GC gc;
  Pixmap pm;
  XImage *image;
  bool image_binded;
  int w,h;
} XWrapper;
typedef struct {
  bool exit;
} Events;
void X11Init(XWrapper *x_ctx);
void X11Destroy(XWrapper *x_ctx);
void X11BindBuffer(XWrapper *x_ctx, void* buffer, int w, int h);
void X11DrawFrame(XWrapper *x_ctx);
void X11PollEvents(XWrapper *x_ctx, Events *events);

#endif //INCLUDE_XWRAPPER_H_
