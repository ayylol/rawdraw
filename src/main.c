#include <stdint.h>
#include <stdio.h>
#include <stdlib.h> // for rand

#define WIDTH 300
#define HEIGHT 200

uint32_t buff[WIDTH*HEIGHT];
void save_ppm(uint32_t *buffer, uint32_t width, uint32_t height);

int main(void){
  for (uint32_t i=0; i < WIDTH*HEIGHT; i++){
    buff[i]=rand();
  }
  save_ppm(buff, WIDTH, HEIGHT);
  return 0;
}

void save_ppm(uint32_t *buffer, uint32_t width, uint32_t height){
  printf("P3\n%d %d\n255\n", width, height);
  for (uint32_t i=0; i < width*height; i++){
    uint8_t r=(buffer[i]>>8*2)&0xFF;
    uint8_t g=(buffer[i]>>8*1)&0xFF;
    uint8_t b=(buffer[i]>>8*0)&0xFF;
    printf("%d %d %d\n", r, g, b); 
  }
}
