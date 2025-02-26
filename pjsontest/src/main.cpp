#include "pjson.h"

int main() {
  pjson oA;
  oA["my gosh"][3]["nest"] = "deep";
  oA["my gosh"][0]["test"] = "love";
  oA["Hello"] = "world";
  oA["my gosh"][1][3] = 3.33f;
  oA["my gosh"][1][2] = 10;
  
  pjson oB;

  //pjson& operator[] (const std::string& aString);
  std::string sMeow = "meow1";
  oB[sMeow] = true;
  // pjson& operator[] (const char* aSkey);
  oB["meow2"] = true;
  //pjson& operator[] (int index);
  oB["meow3"][0] = "Yay0";
  oB["meow3"][1] = "Yay1";

  //pjson& operator=(const std::string& aString);
  std::string sCat = "cat1";
  oB["Cat1"] = sCat;

  //pjson& operator=(const char* aCString);
  oB["Cat2"] = "cat2";

  //pjson& operator=(const int aInt);
  oB["Dog1"] = 1;
  //pjson& operator=(const float aFloat);
  oB["DogFloat"] = 3.7f;
  //pjson& operator=(const bool aBool);
  oB["DogBool"] = true;

  //pjson& operator=(const std::vector<std::string>& aValueArray);
  std::vector<std::string> sVec = {"one", "two"};
  oB["strings"] = sVec;

  //pjson& operator=(const std::vector<char*>& aValueArray);
  std::vector<char*> sCharVec = { "c1", "c2" };
  oB["charArray"] = sCharVec;

  //pjson& operator=(const std::vector<int>& aValueArray);
  std::vector<int> iVec = {1,2};
  oB["ints"] = iVec;

  //pjson& operator=(const std::vector<float>& aValueArray);
  std::vector<float> fVec = {1.0,2.0};
  oB["floats"] = fVec;

  //pjson& operator=(const std::vector<bool>& aValueArray);
  std::vector<bool> bVec = {true,false};
  oB["bools"] = bVec;

  //pjson& operator+=(const std::string& aValue);
  std::string sThree = "three";
  oB["strings"] += sThree;

  //pjson& operator+=(const char* aValue);
  oB["charArray"] += "c3";

  //pjson& operator+=(const int aValue);
  oB["ints"] += 3;

  //pjson& operator+=(const float aValue);
  oB["floats"] += 3.0f;

  //pjson& operator+=(const bool aValue);
  oB["bools"] += true;

  //pjson& operator+=(const std::vector<std::string>& aValueArray);
  std::vector<std::string> sVec2 = {"four","five"};
  oB["strings"] += sVec2;

  //pjson& operator+=(const std::vector<char*>& aValueArray);
  std::vector<char*> sCharVec2 = {"c4", "c5" };
  oB["charArray"] += sCharVec2;

  //pjson& operator+=(const std::vector<int>& aValueArray);
  std::vector<int> iVec2 = {4,5};
  oB["ints"] += iVec2;

  //pjson& operator+=(const std::vector<float>& aValueArray);
  std::vector<float> fVec2 = {4.0,5.0};
  oB["floats"] += fVec2;

  //pjson& operator+=(const std::vector<bool>& aValueArray);
  std::vector<bool> bVec2 = {false, true};
  oB["bools"] += bVec2;


  std::string sSrc = oB.toString(8,0);
  std::string sSrcUnformated = oB.toString();
  std::cout<<std::endl<<"Praveen\'s JSON !";
  std::cout<<std::endl<<"Source : "<<sSrc;
  std::cout<<std::endl<<"Source UnFormatted : "<<sSrcUnformated;

  size_t iStart =0;
  size_t iEnd =sSrc.length();
  pjson* pResult = nullptr;
  pjson::CreateFromString(sSrc.c_str(), iStart, iEnd, pResult);
  std::string sTest = pResult->toString();
  delete pResult;
  std::cout<<std::endl<<"Test   : "<<sTest;

  std::cout<<std::endl;
  return 0;
}