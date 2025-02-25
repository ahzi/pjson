#include "pjson.h"

int main() {
  std::cout<<"\nHello World !\n";
  pjson oA;
  /*pjson oB;
  pjson oC;
  oA["hello"] = "World";
  oC = 3.3f;
  oB[0] = oC;
  oB[5] = 5;
  oA["my gosh"] = oB;
  oA["my my"] = true;
  oA["my gosh"][0] = 0;
  oA["my gosh"][1] = 1;*/
  oA["my gosh"][1]["nest"] = "deep";
  oA["my gosh"][0]["test"] = "love";
  oA["Hello"] = "world";
  oA["my gosh"][2][3][3][3] = 3.33f;
  std::cout<<std::endl<<oA.toString()<<std::endl;

  return 0;
}