#include "util.h"

int map(int x, int in_min, int in_max, int out_min, int out_max){
  int range = (in_max - in_min);
  float y = x - in_min;
  int result =  ((out_max - out_min) * y/range) + out_min;
  if(result > out_max){
    return out_max;
  }
  if(result < out_min){
    return out_min;
  }
  return result;

}