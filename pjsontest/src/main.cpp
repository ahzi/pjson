#include <iostream>
#include "pjson.h"

int main() {
 
  
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

  //pjson& operator=(const std::vector<const char*>& aValueArray);
  std::vector<const char*> sCharVec = { "c1", "c2" };
  oB["charArray"] = sCharVec;

  //pjson& operator=(const std::vector<int>& aValueArray);
  std::vector<int> iVec = {7,8};
  oB["ints"] = iVec;

  //pjson& operator=(const std::vector<float>& aValueArray);
  std::vector<float> fVec = {1.1,2.1};
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
  oB["ints"] += 9;

  //pjson& operator+=(const float aValue);
  oB["floats"] += 3.3f;

  //pjson& operator+=(const bool aValue);
  oB["bools"] += true;

  //pjson& operator+=(const std::vector<std::string>& aValueArray);
  std::vector<std::string> sVec2 = {"four","five"};
  oB["strings"] += sVec2;

  //pjson& operator+=(const std::vector<char*>& aValueArray);
  {
    char* sCharTest1 = new char[10]; strcpy(sCharTest1,"c4");
    char* sCharTest2 = new char[10]; strcpy(sCharTest2,"c5");
    std::vector<char*> sCharVec2 = {sCharTest1, sCharTest2 };
    oB["charArray"] += sCharVec2;
    delete [] sCharTest1;
    delete [] sCharTest2;
  }

  //pjson& operator+=(const std::vector<int>& aValueArray);
  std::vector<int> iVec2 = {10,11};
  oB["ints"] += iVec2;

  //pjson& operator+=(const std::vector<float>& aValueArray);
  std::vector<float> fVec2 = {4.1,5.1};
  oB["floats"] += fVec2;

  //pjson& operator+=(const std::vector<bool>& aValueArray);
  std::vector<bool> bVec2 = {false, true};
  oB["bools"] += bVec2;

  //map of maps
  oB["Cats"]["Cat1"] = "meow";
  oB["Cats"]["Cat2"] = true;
  oB["Cats"]["Cat3"] = 37;
  oB["Cats"]["Cat4"] = 3.7f;
  oB["Cats"]["Cat5"];
  oB["Cats"]["Cat6"] = std::vector<float> {1.1f, 2.2f, 3.3f};
  oB["Cats"]["Cat7"]["dog1"] = 1;
  oB["Cats"]["Cat7"]["dog2"] = "bark";

  //Fetaure Test
  {
    std::cout<<std::endl<<"----------------------------------";
    std::cout<<std::endl<<"Feature Test :"<<std::endl;
    std::string sSrc = oB.toString(true);
    std::cout<<sSrc;
  }

  //Load from String Test
  {
    std::cout<<std::endl<<"----------------------------------";
    std::cout<<std::endl<<"Load from String Test :"<<std::endl;
    std::string sSrc = oB.toString();
    pjson* pResult = pjson::CreateFromString(sSrc);
    std::string sTest = (pResult) ? pResult->toString() : "";
    if(0==sTest.compare(sSrc)) {
      std::cout<<"PASS";
    } else {
      std::cout<<"FAIL";
    }
    delete pResult;
  }
  //Load from String Test
  {
    std::cout<<std::endl<<"----------------------------------";
    std::cout<<std::endl<<"Load from Formatted String Test :"<<std::endl;
    std::string sSrc1 = oB.toString(true);
    std::string sSrc2 = oB.toString(false);
    pjson* pResult = pjson::CreateFromString(sSrc1);
    std::string sTest1 = (pResult) ? pResult->toString(true) : "";
    std::string sTest2 = (pResult) ? pResult->toString() : "";
    if(0==sTest1.compare(sSrc1) && 0==sTest2.compare(sSrc2)) {
      std::cout<<"PASS";
    } else {
      std::cout<<"FAIL";
    }
    delete pResult;
  }

  //Formatting Test
  {
    std::cout<<std::endl<<"----------------------------------";
    std::cout<<std::endl<<"Formatting Test :"<<std::endl;
    pjson oA;
    oA["Level-1-A"] = "L1-A";
    oA["Level-1-B"] = "L1-B";
    oA["Level-1-C"]["Level-2-A"] = "L2-A";
    oA["Level-1-C"]["Level-2-B"] = "L2-B";
    oA["Level-1-C"]["Level-2-C"] = std::vector<int>({1,2,3,4,5,6});
    oA["Level-1-C"]["Level-2-D"][0] = std::vector<float>({1.0,2.0,3.0,4.0,5.0,6.0});
    oA["Level-1-C"]["Level-2-D"][1] = std::vector<const char*>({"one", "two","three"});
    oA["Level-1-D"] = "L1-D";

    std::cout<<oA.toString(true);
  }

  //Array Float Iterator
  {
    std::cout<<std::endl<<"----------------------------------";
    std::cout<<std::endl<<"Array Float Iterator Test :"<<std::endl;
    pjson::PJSONARRAY* pArray = oB["floats"].getArray();
    if(pArray) {
      std::cout<<"Array : ";
      bool bfirst = true;
      for(auto iPjons : *pArray) {
        if(!bfirst) {
          std::cout<<" , ";
        }
        std::cout<<iPjons->getFloat();
        bfirst = false;
      }
    }

  }

  //Array Int Iterator
  {
    std::cout<<std::endl<<"----------------------------------";
    std::cout<<std::endl<<"Array Int Iterator Test :"<<std::endl;
    pjson::PJSONARRAY* pArray = oB["ints"].getArray();
    if(pArray) {
      std::cout<<"Array : ";
      bool bfirst = true;
      for(auto iPjons : *pArray) {
        if(!bfirst) {
          std::cout<<" , ";
        }
        std::cout<<iPjons->getInt();
        bfirst = false;
      }
    }
  }

  //Array String Iterator
  {
    std::cout<<std::endl<<"----------------------------------";
    std::cout<<std::endl<<"Array String Iterator Test :"<<std::endl;
    pjson::PJSONARRAY* pArray = oB["strings"].getArray();
    if(pArray) {
      std::cout<<"Array : ";
      bool bfirst = true;
      for(auto iPjons : *pArray) {
        if(!bfirst) {
          std::cout<<" , ";
        }
        std::cout<<iPjons->getString();
        bfirst = false;
      }
    }
  }
  
  //Array Bool Iterator
  {
    std::cout<<std::endl<<"----------------------------------";
    std::cout<<std::endl<<"Array Bool Iterator Test :"<<std::endl;
    pjson::PJSONARRAY* pArray = oB["bools"].getArray();
    if(pArray) {
      std::cout<<"Array : ";
      bool bfirst = true;
      for(auto iPjons : *pArray) {
        if(!bfirst) {
          std::cout<<" , ";
        }
        std::cout<<iPjons->getBool()<<" "<<iPjons->toString();
        bfirst = false;
      }
    }
  }
  
    //Map Iterator Test
    {
      std::cout<<std::endl<<"----------------------------------";
      std::cout<<std::endl<<"Map Iterator Test :"<<std::endl;
      pjson::PJSONMAP* pMap = oB["Cats"].getMap();
      if(pMap) {
        std::cout<<"{";
        for(auto itr : *pMap) {
          std::cout<<std::endl<<" "<<itr.first<<" : ";

          switch (itr.second->getType()) {
            case pjson::jsonType::jsonNull:         std::cout<<"|Null  | "; break;
            case pjson::jsonType::jsonString:       std::cout<<"|String| "; break;
            case pjson::jsonType::jsonNumberInt:    std::cout<<"|Int   | "; break;
            case pjson::jsonType::jsonNumberFloat:  std::cout<<"|Float | "; break;
            case pjson::jsonType::jsonBoolean:      std::cout<<"|Bool  | "; break;
            case pjson::jsonType::jsonArray:        std::cout<<"|Array | "; break;
            case pjson::jsonType::jsonMap:          std::cout<<"|Map   | "; break;
            default: std::cout<<"|**ERROR**|"; break;
          }
          std::cout<<itr.second->toString();
        }
        std::cout<<std::endl<<"}";
      }
    }
    
  std::cout<<std::endl;
  return 0;
}