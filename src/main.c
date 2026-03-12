#include <stdint.h>
#include <stdio.h>
#include <stdlib.h> // for rand
#include <string.h>

#define WIDTH 1200
#define HEIGHT 800
uint32_t buff[WIDTH*HEIGHT];

char file_name[64] = "out.ppm";

int32_t save_ppm(char* file_name, uint32_t *buffer, 
                  uint32_t width, uint32_t height);

int32_t main(int argc, char* argv[]){
  if (argc == 2){
    strcpy(file_name, argv[1]); 
  }
  for (uint32_t i=0; i < WIDTH*HEIGHT; i++){
    buff[i]=rand();
  }
  save_ppm(file_name, buff, WIDTH, HEIGHT);
  return 0;
}

int32_t save_ppm(char* file_name, uint32_t *buffer,
                  uint32_t width, uint32_t height){
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
