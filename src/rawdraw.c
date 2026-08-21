#include "rawdraw.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


// PRIVATE FUNCTION IMPLEMENTATIONS
static inline void rawdraw_swap_points(point_t* p1, point_t* p2){
  point_t temp_p=*p1;
  *p1=*p2;
  *p2=temp_p;
}

// PUBLIC FUNCTION IMPLEMENTATIONS
void rawdraw_rect(image_t img, point_t p1, point_t p2, color_t col) {
  // TODO: this bound checking probably needs more thought put into it,
  // but at least it doesn't seg fault now
  p2.x=p2.x<img.w ? p2.x : img.w-1;
  p2.y=p2.y<img.h ? p2.y : img.h-1;
  p1.x=p1.x>=0 ? p1.x : 0;
  p1.y=p1.y>=0 ? p1.y : 0;
  for (int32_t x=p1.x; x<=p2.x; x++){
    for (int32_t y=p1.y; y<=p2.y; y++){
      img.buffer[rawdraw_get_i(img, x, y)]=col;
    }
  }
}

void rawdraw_fill(image_t img, color_t col) {
  rawdraw_rect(img, (point_t){0,0}, (point_t){img.w-1,img.h-1}, col);
}

void rawdraw_point(image_t img, point_t p, int32_t dim, color_t col){
  point_t p1=p;
  p1.x-=(dim-1)/2;
  p1.y-=(dim-1)/2;
  point_t p2=p;
  p2.x+=(dim-1)/2 + (dim%2 ? 0 : 1);
  p2.y+=(dim-1)/2 + (dim%2 ? 0 : 1);
  rawdraw_rect(img,p1,p2,col);
}

bool is_left_of(point_t p1, point_t p2, point_t p3){
  point_t v1=(point_t){-(p2.y-p1.y),p2.x-p1.x};
  point_t v2=(point_t){p3.x-p1.x,p3.y-p1.y};
  // Take dot product
  int32_t dot = v1.x*v2.x+v1.y*v2.y;
  return dot>=0;
}

// Source - https://stackoverflow.com/a/3437484
// Posted by David Titarenco, modified by community. See post 'Timeline' for change history
// Retrieved 2026-05-10, License - CC BY-SA 4.0
 #define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })
 #define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })
 #define clamp(a,l,u) \
   min(max(a,l),u)    \

void rawdraw_tri(image_t img, point_t p1, point_t p2, point_t p3, color_t col){
  point_t lb = {clamp(min(min(p1.x,p2.x),p3.x),0,img.w-1),clamp(min(min(p1.y,p2.y),p3.y),0,img.h-1)};
  point_t ub = {clamp(max(max(p1.x,p2.x),p3.x),0,img.w-1),clamp(max(max(p1.y,p2.y),p3.y),0,img.h-1)};
  for (int32_t x=lb.x; x<ub.x; x++){
    for (int32_t y=lb.y; y<ub.y; y++){
      point_t p4 = (point_t){x,y};
      bool inside=
        (is_left_of(p1,p2,p4)&&is_left_of(p2,p3,p4)&&is_left_of(p3,p1,p4));
        //|| !(is_left_of(p1,p2,p4)||is_left_of(p2,p3,p4)||is_left_of(p3,p1,p4));
      if (inside){ img.buffer[rawdraw_get_i(img, x, y)]=col; }
    }
  }
}

// TODO: ADD BOUNDS CHECKING!!!!!!!!!!!
void rawdraw_line(image_t img, point_t p1, point_t p2, color_t col){
  bool low=true;
  if (abs(p2.y - p1.y) < abs(p2.x - p1.x)){
    if (p1.x > p2.x){ rawdraw_swap_points(&p1, &p2); }
  }
  else {
    low=false;
    if (p1.y > p2.y){ rawdraw_swap_points(&p1, &p2); }
  }
  int32_t d_axis1=p2.y-p1.y;
  int32_t d_axis2=p2.x-p1.x;
  if (!low) {
    int32_t temp=d_axis1;
    d_axis1=d_axis2;
    d_axis2=temp;
  }
  int8_t di=1;
  if (d_axis1<0){
    di=-1;
    d_axis1=-d_axis1;
  }
  int32_t j = low ? p1.y : p1.x;
  int32_t start = low ? p1.x : p1.y;
  int32_t end = low ? p2.x : p2.y;
  int32_t D=2*d_axis1-d_axis2;
  for (int i=start; i<end; i++){
      int32_t x = low ? i : j;
      int32_t y = low ? j : i;
      img.buffer[rawdraw_get_i(img, x, y)] = col;
      if (D>0){
        j=j+di;
        D+=2*(d_axis1-d_axis2);
      } else {
        D+=2*d_axis1;
      }
  }
}

color_t colors[256] = {
  X000_BLACK, X001_MAROON, X002_GREEN, X003_OLIVE, X004_NAVY, X005_PURPLE, X006_TEAL, X007_SILVER,
  X008_GREY, X009_RED, X010_LIME, X011_YELLOW, X012_BLUE, X013_FUCHSIA, X014_AQUA, X015_WHITE,
  x016_Grey0, x017_NavyBlue, x018_DarkBlue, x019_Blue3, x020_Blue3, x021_Blue1, x022_DarkGreen, x023_DeepSkyBlue4,
  x024_DeepSkyBlue4, x025_DeepSkyBlue4, x026_DodgerBlue3, x027_DodgerBlue2, x028_Green4, x029_SpringGreen4, x030_Turquoise4, x031_DeepSkyBlue3,
  x032_DeepSkyBlue3, x033_DodgerBlue1, x034_Green3, x035_SpringGreen3, x036_DarkCyan, x037_LightSeaGreen, x038_DeepSkyBlue2, x039_DeepSkyBlue1,
  x040_Green3, x041_SpringGreen3, x042_SpringGreen2, x043_Cyan3, x044_DarkTurquoise, x045_Turquoise2, x046_Green1, x047_SpringGreen2,
  x048_SpringGreen1, x049_MediumSpringGreen, x050_Cyan2, x051_Cyan1, x052_DarkRed, x053_DeepPink4, x054_Purple4, x055_Purple4,
  x056_Purple3, x057_BlueViolet, x058_Orange4, x059_Grey37, x060_MediumPurple4, x061_SlateBlue3, x062_SlateBlue3, x063_RoyalBlue1,
  x064_Chartreuse4, x065_DarkSeaGreen4, x066_PaleTurquoise4, x067_SteelBlue, x068_SteelBlue3, x069_CornflowerBlue, x070_Chartreuse3, x071_DarkSeaGreen4,
  x072_CadetBlue, x073_CadetBlue, x074_SkyBlue3, x075_SteelBlue1, x076_Chartreuse3, x077_PaleGreen3, x078_SeaGreen3, x079_Aquamarine3,
  x080_MediumTurquoise, x081_SteelBlue1, x082_Chartreuse2, x083_SeaGreen2, x084_SeaGreen1, x085_SeaGreen1, x086_Aquamarine1, x087_DarkSlateGray2,
  x088_DarkRed, x089_DeepPink4, x090_DarkMagenta, x091_DarkMagenta, x092_DarkViolet, x093_Purple, x094_Orange4, x095_LightPink4,
  x096_Plum4, x097_MediumPurple3, x098_MediumPurple3, x099_SlateBlue1, x100_Yellow4, x101_Wheat4, x102_Grey53, x103_LightSlateGrey,
  x104_MediumPurple, x105_LightSlateBlue, x106_Yellow4, x107_DarkOliveGreen3, x108_DarkSeaGreen, x109_LightSkyBlue3, x110_LightSkyBlue3, x111_SkyBlue2,
  x112_Chartreuse2, x113_DarkOliveGreen3, x114_PaleGreen3, x115_DarkSeaGreen3, x116_DarkSlateGray3, x117_SkyBlue1, x118_Chartreuse1, x119_LightGreen,
  x120_LightGreen, x121_PaleGreen1, x122_Aquamarine1, x123_DarkSlateGray1, x124_Red3, x125_DeepPink4, x126_MediumVioletRed, x127_Magenta3,
  x128_DarkViolet, x129_Purple, x130_DarkOrange3, x131_IndianRed, x132_HotPink3, x133_MediumOrchid3, x134_MediumOrchid, x135_MediumPurple2,
  x136_DarkGoldenrod, x137_LightSalmon3, x138_RosyBrown, x139_Grey63, x140_MediumPurple2, x141_MediumPurple1, x142_Gold3, x143_DarkKhaki,
  x144_NavajoWhite3, x145_Grey69, x146_LightSteelBlue3, x147_LightSteelBlue, x148_Yellow3, x149_DarkOliveGreen3, x150_DarkSeaGreen3, x151_DarkSeaGreen2,
  x152_LightCyan3, x153_LightSkyBlue1, x154_GreenYellow, x155_DarkOliveGreen2, x156_PaleGreen1, x157_DarkSeaGreen2, x158_DarkSeaGreen1, x159_PaleTurquoise1,
  x160_Red3, x161_DeepPink3, x162_DeepPink3, x163_Magenta3, x164_Magenta3, x165_Magenta2, x166_DarkOrange3, x167_IndianRed,
  x168_HotPink3, x169_HotPink2, x170_Orchid, x171_MediumOrchid1, x172_Orange3, x173_LightSalmon3, x174_LightPink3, x175_Pink3,
  x176_Plum3, x177_Violet, x178_Gold3, x179_LightGoldenrod3, x180_Tan, x181_MistyRose3, x182_Thistle3, x183_Plum2,
  x184_Yellow3, x185_Khaki3, x186_LightGoldenrod2, x187_LightYellow3, x188_Grey84, x189_LightSteelBlue1, x190_Yellow2, x191_DarkOliveGreen1,
  x192_DarkOliveGreen1, x193_DarkSeaGreen1, x194_Honeydew2, x195_LightCyan1, x196_Red1, x197_DeepPink2, x198_DeepPink1, x199_DeepPink1,
  x200_Magenta2, x201_Magenta1, x202_OrangeRed1, x203_IndianRed1, x204_IndianRed1, x205_HotPink, x206_HotPink, x207_MediumOrchid1,
  x208_DarkOrange, x209_Salmon1, x210_LightCoral, x211_PaleVioletRed1, x212_Orchid2, x213_Orchid1, x214_Orange1, x215_SandyBrown,
  x216_LightSalmon1, x217_LightPink1, x218_Pink1, x219_Plum1, x220_Gold1, x221_LightGoldenrod2, x222_LightGoldenrod2, x223_NavajoWhite1,
  x224_MistyRose1, x225_Thistle1, x226_Yellow1, x227_LightGoldenrod1, x228_Khaki1, x229_Wheat1, x230_Cornsilk1, x231_Grey100,
  x232_Grey3, x233_Grey7, x234_Grey11, x235_Grey15, x236_Grey19, x237_Grey23, x238_Grey27, x239_Grey30,
  x240_Grey35, x241_Grey39, x242_Grey42, x243_Grey46, x244_Grey50, x245_Grey54, x246_Grey58, x247_Grey62,
  x248_Grey66, x249_Grey70, x250_Grey74, x251_Grey78, x252_Grey82, x253_Grey85, x254_Grey89, x255_Grey93,
};
