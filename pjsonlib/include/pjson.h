#include <iostream>
#include <vector>
#include <map>
#include <string>

//==[Interface]============================================================
class pjson {
  public:
    enum jsonType : int64_t {
      jsonNull,
      jsonString,
      jsonNumberInt,
      jsonNumberFloat,
      jsonBoolean,
      jsonArray, //[ ] array
      jsonObject, // { ... } map
    };

    pjson(); // Default Constructor
    ~pjson(); // Destructor
    pjson(const pjson& aFrom); // Copy Constructor
    pjson(pjson&& aFrom); // Move constructor
    pjson& operator=(const pjson& aFrom); // Copy assignment
    pjson& operator=(pjson&& a); // Move assignment


    //Assignment overload
    pjson& operator=(const std::string& aString);
    pjson& operator=(const char* aCString);
    pjson& operator=(const int aInt);
    pjson& operator=(const float aFloat);
    pjson& operator=(const bool aBool);
    pjson& operator[] (int index);
    pjson& operator[] (const char* aSkey);

    jsonType getType() const;
    std::string toString() const;
    void copyFrom(const pjson& aFrom);

    void reset();
    void resetTo(jsonType aeType);

  private:
    void resetIfneeded(jsonType aeType);
  private:
    typedef std::vector<pjson*> PJSONARRAY;
    typedef std::map<std::string, pjson*> PJSONMAP;

    jsonType _eType = jsonType::jsonNull;
    union {
      void* _pValueRaw = nullptr;
      PJSONMAP* _pValueMap;
      PJSONARRAY* _pValueArray;
      int* _pValueInt;
      float* _pValueFloat;
      bool* _pValueBool;
      std::string* _pValueString;
      /* data */
    };
};
//========================================================================
