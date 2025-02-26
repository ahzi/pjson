#include <iostream>
#include <vector>
#include <unordered_map>
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
    pjson& operator[] (const std::string& aString);
    pjson& operator[] (const char* aSkey);
    pjson& operator[] (int index);

    //Assignment overload
    pjson& operator=(const std::string& aString);
    pjson& operator=(const char* aCString);
    pjson& operator=(const int aInt);
    pjson& operator=(const float aFloat);
    pjson& operator=(const bool aBool);

    pjson& operator=(const std::vector<std::string>& aValueArray);
    pjson& operator=(const std::vector<char*>& aValueArray);
    pjson& operator=(const std::vector<int>& aValueArray);
    pjson& operator=(const std::vector<float>& aValueArray);
    pjson& operator=(const std::vector<bool>& aValueArray);

    pjson& operator+=(const std::string& aValue);
    pjson& operator+=(const char* aValue);
    pjson& operator+=(const int aValue);
    pjson& operator+=(const float aValue);
    pjson& operator+=(const bool aValue);

    pjson& operator+=(const std::vector<std::string>& aValueArray);
    pjson& operator+=(const std::vector<char*>& aValueArray);
    pjson& operator+=(const std::vector<int>& aValueArray);
    pjson& operator+=(const std::vector<float>& aValueArray);
    pjson& operator+=(const std::vector<bool>& aValueArray);

    jsonType getType() const;
    std::string toString(int a_iTablen = -1, int a_iStartDepth=0) const;
    void copyFrom(const pjson& aFrom);

    void reset();
    void resetTo(jsonType aeType);

    static bool CreateFromString(const char* aSrc, size_t& a_iStart, size_t a_iEnd, pjson*& a_rResult);
  private:
    void resetIfneeded(jsonType aeType);

    static bool ScanPastColon(const char* aSrc, size_t& a_iStart,const size_t a_iEnd);
    static bool ExtractString(const char* aSrc, size_t& a_iStart,const size_t a_iEnd, std::string& aStrResult);
    static bool ScanString(const char* aSrc, size_t& a_iStart,const size_t a_iEnd, pjson*& a_rStrResult);
    static bool ScanBool(const char* aSrc, size_t& a_iStart,const size_t a_iEnd, pjson*& a_rBoolResult);
    static bool ScanToNext(const char* aSrc, size_t& a_iStart,const size_t a_iEnd, char& a_rResult);
    static bool ScanNull(const char* aSrc, size_t& a_iStart,const size_t a_iEnd, pjson*& a_rNUllResult);
    static bool ScanNumber(const char* aSrc, size_t& a_iStart, const size_t a_iEnd, pjson*& a_rNumResult);
    static bool ScanArray(const char* aSrc, size_t& a_iStart, const size_t a_iEnd, pjson*& a_rAResult);
    static bool ScanObject(const char* aSrc, size_t& a_iStart, const size_t a_iEnd, pjson*& a_rAResult);

  private:
    typedef std::vector<pjson*> PJSONARRAY;
    typedef std::unordered_map<std::string, pjson*> PJSONMAP;

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
