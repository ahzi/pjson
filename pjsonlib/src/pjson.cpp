#include "pjson.h"

//-----------------------------------------------------------------
/*
int stricmp(const char *p1, const char *p2){
  register unsigned char *s1 = (unsigned char *) p1;
  register unsigned char *s2 = (unsigned char *) p2;
  unsigned char c1, c2;
  do {
      c1 = (unsigned char) toupper((int)*s1++);
      c2 = (unsigned char) toupper((int)*s2++);
      if (c1 == '\0') {
        return c1 - c2;
      }
  } while (c1 == c2);
  return c1 - c2;  
}
*/
//-----------------------------------------------------------------
pjson::pjson()
: _eType(jsonType::jsonNull)
, _pValueRaw(nullptr)
{

}
//-----------------------------------------------------------------
pjson::~pjson() {
  reset();
}
//-----------------------------------------------------------------
pjson::pjson(const pjson& aFrom)
: _eType(jsonType::jsonNull)
, _pValueRaw(nullptr)
{ 
  copyFrom(aFrom);
}
//-----------------------------------------------------------------
// Move constructor
pjson::pjson(pjson&& aFrom) {
  _eType = aFrom._eType;
  _pValueRaw = aFrom._pValueRaw;
  aFrom._pValueRaw = nullptr;
  aFrom._eType = jsonType::jsonNull;
}
//-----------------------------------------------------------------
// Move assignment
pjson& pjson::operator=(pjson&& aFrom) {
if (&aFrom == this)
return *this;

  reset();
  _eType = aFrom._eType;
  _pValueRaw = aFrom._pValueRaw;

  aFrom._eType = jsonType::jsonNull;
  aFrom._pValueRaw = nullptr;

return *this;
}
//-----------------------------------------------------------------
// Copy assignment
pjson& pjson::operator=(const pjson& aFrom) {
  if (&aFrom == this)
    return *this;

  //reset(); // CopyFrom will do a reset anyways
  copyFrom(aFrom);
  return *this;
}
//-----------------------------------------------------------------
pjson::jsonType pjson::getType() const { 
  return _eType;
};
//-----------------------------------------------------------------
void pjson::reset() {
  resetTo(jsonType::jsonNull);
}
//-----------------------------------------------------------------
void pjson::resetIfneeded(jsonType aeType) {
  if(_eType != aeType) {
    resetTo(aeType);
  }
}
//-----------------------------------------------------------------
void pjson::resetTo(pjson::jsonType aeType) {
  switch(_eType) {
    case jsonType::jsonNull:         { _pValueRaw = nullptr; break; }
    case jsonType::jsonString:       { delete _pValueString;  break; }
    case jsonType::jsonNumberInt:    { delete _pValueInt;     break; }
    case jsonType::jsonNumberFloat:  { delete _pValueFloat;   break; }
    case jsonType::jsonBoolean:      { delete _pValueBool;    break; }
    case jsonType::jsonArray:  {
      for(pjson* pj : *_pValueArray) {
        delete pj;
      }
      delete _pValueArray;
      break;
    }
    case jsonType::jsonObject: {
      for (const auto& kv : *_pValueMap) {
          delete kv.second;
      }
      delete _pValueMap;
      break;
    }      
  }//end switch
  _pValueRaw = nullptr;

  switch(aeType) {
    case jsonType::jsonNull:         { /* _pValueRaw = nullptr; */ break; }
    case jsonType::jsonString:       { _pValueString = new std::string; break; }
    case jsonType::jsonNumberInt:    { _pValueInt = new int; break; }
    case jsonType::jsonNumberFloat:  { _pValueFloat = new float; break; }
    case jsonType::jsonBoolean:      { _pValueBool = new bool; break; }
    case jsonType::jsonArray:        { _pValueArray = new PJSONARRAY; break; }
    case jsonType::jsonObject:       { _pValueMap = new PJSONMAP; break; }
  } //end switch
  _eType = aeType;
}
//-----------------------------------------------------------------
void pjson::copyFrom(const pjson& aFrom) {
  resetTo(aFrom.getType());

  switch(_eType) {
    case jsonType::jsonNull:         { /* _pValueRaw = nullptr; */ break; }
    case jsonType::jsonString:       { *_pValueString = *(aFrom._pValueString); break; }
    case jsonType::jsonNumberInt:    { *_pValueInt = *(aFrom._pValueInt); break; }
    case jsonType::jsonNumberFloat:  { *_pValueFloat = *(aFrom._pValueFloat); break; }
    case jsonType::jsonBoolean:      { *_pValueBool = *(aFrom._pValueBool); break; }
    case jsonType::jsonArray:        { 
      for (auto it : *(aFrom._pValueArray)) {
        pjson* pObj = new pjson();
        pObj->copyFrom(*it);
        _pValueArray->push_back(pObj);
      }
      break;
    }
    case jsonType::jsonObject:       { 
      for (auto const& it : *(aFrom._pValueMap)) {
        pjson* pObj = new pjson();
        pObj->copyFrom(*(it.second));
        (*_pValueMap)[it.first] = pObj;
      }
      break;
    }
  } //end switch
}
//-----------------------------------------------------------------
//std::string pjson::toString() const {
std::string pjson::toString(int a_iTablen/* = -1 */, int a_iStartDepth/* = 0*/) const {
  std::string out;
  std::string spaces;
  if(a_iTablen>0) {
    spaces = std::string(a_iTablen * a_iStartDepth, ' ');
  }
  switch(_eType) {
    case jsonType::jsonNull:         { out += "null"; break; }
    case jsonType::jsonString:       { out += "\""; out += *_pValueString; out += "\""; break; }
    case jsonType::jsonNumberInt:    { out += std::to_string(*_pValueInt); break; }
    case jsonType::jsonNumberFloat:  { out += std::to_string(*_pValueFloat); break; }
    case jsonType::jsonBoolean:      { out += (*_pValueBool)?"true":"false"; break; }
    case jsonType::jsonArray:  {
      out += "[";
      if(a_iTablen>0) {
        out += '\n';
      }
      bool bFirstElement = true;
      for (auto it = _pValueArray->begin(); it != _pValueArray->end(); it++) {
        if(!bFirstElement) {
          out += ",";
          if(a_iTablen>0) {
            out += '\n';
          }
        }

        if(a_iTablen>0) {
          out += spaces;
        }
        out += " ";
        out += (*it)->toString(a_iTablen, a_iStartDepth+1);
        out += " ";
        bFirstElement = false;
      }
      if(bFirstElement) {
          out += " ";
      }
      if(a_iTablen>0) {
        out += '\n';
        out += spaces;
      }
      out += "]";
      break;
    }
    case jsonType::jsonObject: {
      out = "{";
      if(a_iTablen>0) {
        out += '\n';
      }
      bool bFirstElement = true;
      for (auto it = _pValueMap->begin(); it != _pValueMap->end(); it++) {
        if(!bFirstElement) {
          out += ",";
          if(a_iTablen>0) {
            out += '\n';
          } 
        }
        if(a_iTablen>0) {
          out += spaces;
        }
        out += " \"";
        out += it->first;
        out += "\" : ";
        out += it->second->toString(a_iTablen, a_iStartDepth+1);
        out += " ";
        bFirstElement = false;
    }
      if(bFirstElement) {
          out += " ";
      }
      if(a_iTablen>0) {
        out += '\n';
        out += spaces;
      }
      out += "}";
      break;
    }      
  }//end switch

  return out;
}
//-----------------------------------------------------------------
pjson& pjson::operator=(const std::string& aString) {
  resetIfneeded(jsonType::jsonString);
  *_pValueString = aString;
  return *this;
}
//-----------------------------------------------------------------
pjson& pjson::operator=(const char* aCString) {
  resetIfneeded(jsonType::jsonString);
  *_pValueString = aCString;
  return *this;
}
//-----------------------------------------------------------------
pjson& pjson::operator=(const int aInt) {
  resetIfneeded(jsonType::jsonNumberInt);
  *_pValueInt = aInt;
  return *this;
}
//-----------------------------------------------------------------
pjson& pjson::operator=(const float aFloat) {
  resetIfneeded(jsonType::jsonNumberFloat);
  *_pValueFloat = aFloat;
  return *this;
}
//-----------------------------------------------------------------
pjson& pjson::operator=(const bool aBool) {
  resetIfneeded(jsonType::jsonBoolean);
  *_pValueBool = aBool;
  return *this;
}
//-----------------------------------------------------------------
#define PJSON_VALUE_ARRAY_SET_ITERATOR      \
          resetTo(jsonType::jsonArray);     \
          for(auto i : aValueArray){        \
            pjson* pTemp = new pjson();     \
            *pTemp = i;                     \
            _pValueArray->push_back(pTemp); \
          }                                 \
          return *this;
//-----------------------------------------------------------------
pjson& pjson::operator=(const std::vector<char*>& aValueArray) {
  PJSON_VALUE_ARRAY_SET_ITERATOR
}
//-----------------------------------------------------------------
pjson& pjson::operator=(const std::vector<int>& aValueArray) {
  PJSON_VALUE_ARRAY_SET_ITERATOR
}
//-----------------------------------------------------------------
pjson& pjson::operator=(const std::vector<float>& aValueArray) {
  PJSON_VALUE_ARRAY_SET_ITERATOR
}
//-----------------------------------------------------------------
pjson& pjson::operator=(const std::vector<bool>& aValueArray) {
  PJSON_VALUE_ARRAY_SET_ITERATOR
}
//-----------------------------------------------------------------
pjson& pjson::operator=(const std::vector<std::string>& aValueArray) {
  PJSON_VALUE_ARRAY_SET_ITERATOR
}
//-----------------------------------------------------------------
#define PJSON_VALUE_ARRAY_APPEND_ITERATOR       \
          resetIfneeded(jsonType::jsonArray);   \
          pjson* pTemp = new pjson();           \
          *pTemp = aValue;                      \
          _pValueArray->push_back(pTemp);       \
          return *this;
//-----------------------------------------------------------------
pjson& pjson::operator+=(const std::string& aValue) {
  PJSON_VALUE_ARRAY_APPEND_ITERATOR
}
//-----------------------------------------------------------------
pjson& pjson::operator+=(const char* aValue) {
  PJSON_VALUE_ARRAY_APPEND_ITERATOR
}
//-----------------------------------------------------------------
pjson& pjson::operator+=(const int aValue) {
  PJSON_VALUE_ARRAY_APPEND_ITERATOR
}
//-----------------------------------------------------------------
pjson& pjson::operator+=(const float aValue) {
  PJSON_VALUE_ARRAY_APPEND_ITERATOR
}
//-----------------------------------------------------------------
pjson& pjson::operator+=(const bool aValue) {
  PJSON_VALUE_ARRAY_APPEND_ITERATOR
}
//-----------------------------------------------------------------
#define PJSON_VALUE_ARRAY_APPEND_ARRAY            \
          resetIfneeded(jsonType::jsonArray);     \
          for(auto i : aValueArray){              \
            pjson* pTemp = new pjson();           \
            *pTemp = i;                           \
            _pValueArray->push_back(pTemp);       \
          }                                       \
          return *this;
//-----------------------------------------------------------------
pjson& pjson::operator+=(const std::vector<int>& aValueArray) {
  PJSON_VALUE_ARRAY_APPEND_ARRAY
}
//-----------------------------------------------------------------
pjson& pjson::operator+=(const std::vector<char*>& aValueArray) {
  PJSON_VALUE_ARRAY_APPEND_ARRAY
}
//-----------------------------------------------------------------
pjson& pjson::operator+=(const std::vector<float>& aValueArray) {
  PJSON_VALUE_ARRAY_APPEND_ARRAY
}
//-----------------------------------------------------------------
pjson& pjson::operator+=(const std::vector<bool>& aValueArray) {
  PJSON_VALUE_ARRAY_APPEND_ARRAY
}
//-----------------------------------------------------------------
pjson& pjson::operator+=(const std::vector<std::string>& aValueArray) {
  PJSON_VALUE_ARRAY_APPEND_ARRAY
}
//-----------------------------------------------------------------
pjson& pjson::operator[] (int index) {
  resetIfneeded(jsonType::jsonArray);
  int iSize = static_cast<int>(_pValueArray->size());
  if(index < 0) {
    if(iSize > 0) {
      index = (iSize - (index % iSize)) % iSize;
    } else {
      index = 0;
    }
  } 

  if(index >= iSize) {
    int iAdd = 1 + index - iSize;
    for(int i=iAdd;i--;) {
      _pValueArray->push_back(new pjson());
    }
  }

  return *(*_pValueArray)[static_cast<size_t>(index)];
}
//-----------------------------------------------------------------
pjson& pjson::operator[] (const std::string& aString) {
  const char* cStr = aString.c_str();
  resetIfneeded(jsonType::jsonObject);
  PJSONMAP::iterator it = _pValueMap->find(cStr);
  if(it != _pValueMap->end()) {
    return *((*_pValueMap)[cStr]);
  } 
  pjson* pNew = new pjson();
  (*_pValueMap)[cStr] = pNew;
  return *pNew;
}
//-----------------------------------------------------------------
pjson& pjson::operator[] (const char* aSkey) {
  resetIfneeded(jsonType::jsonObject);
  PJSONMAP::iterator it = _pValueMap->find(aSkey);
  if(it != _pValueMap->end()) {
    return *((*_pValueMap)[aSkey]);
  } 
  pjson* pNew = new pjson();
  (*_pValueMap)[aSkey] = pNew;
  return *pNew;
}
//-----------------------------------------------------------------
/*static*/
bool pjson::CreateFromString(const char* aSrc, size_t& a_iStart,size_t a_iEnd, pjson*& a_rResult) {
  //1. Scan for fundametal type
  char aChar;
  while (ScanToNext(aSrc, a_iStart, a_iEnd, aChar)) {
    aChar = tolower(aChar);
    if('\"' == aChar) { 
      return ScanString(aSrc, a_iStart, a_iEnd, a_rResult); 
    } 
    else if('n' == aChar) {
      return ScanNull(aSrc, a_iStart, a_iEnd, a_rResult); 
    }
    else if('t' == aChar || 'f' == aChar) { 
      return ScanBool(aSrc, a_iStart, a_iEnd, a_rResult); 
    }
    else if('+' == aChar || '-' == aChar || '.' == aChar || ('0' <= aChar && '9' >= aChar)) {
      return ScanNumber(aSrc, a_iStart, a_iEnd, a_rResult);
    } else if('{' == aChar) {
      return ScanObject(aSrc, a_iStart, a_iEnd, a_rResult);
    } else if('[' == aChar) {
      return ScanArray(aSrc, a_iStart, a_iEnd, a_rResult);
    } else {
      //unknown
      break;
    }
  } // end while

  return false;
}
//-----------------------------------------------------------------
/*static*/
bool pjson::ScanBool(const char* aSrc, size_t& a_iStart,const size_t a_iEnd, pjson*& a_rBoolResult) {
  if((a_iEnd - a_iStart) >= 4 
    && 't' == tolower(aSrc[a_iStart])
    && 'r' == tolower(aSrc[a_iStart+1])
    && 'u' == tolower(aSrc[a_iStart+2])
    && 'e' == tolower(aSrc[a_iStart+3])) {
    a_rBoolResult = new pjson();
    *a_rBoolResult = bool(true);
    a_iStart+=4;
    return true;
  }
  if((a_iEnd - a_iStart) >= 5 
    && 'f' == tolower(aSrc[a_iStart])
    && 'a' == tolower(aSrc[a_iStart+1])
    && 'l' == tolower(aSrc[a_iStart+2])
    && 's' == tolower(aSrc[a_iStart+3])
    && 'e' == tolower(aSrc[a_iStart+4])) {
    a_rBoolResult = new pjson();
    *a_rBoolResult = bool(false);
    a_iStart+=5;
    return true;
  }
  return false;
}
//-----------------------------------------------------------------
/*static*/
bool pjson::ExtractString(const char* aSrc, size_t& a_iStart,const size_t a_iEnd, std::string& aStrResult) {
  if('\"' != aSrc[a_iStart]){
    return false;
  }
  ++a_iStart;
  int iEnd = a_iStart;
  int iStart = a_iStart;
  while(a_iStart<a_iEnd) {
    char c = aSrc[a_iStart++];
    if('\"' == c) {
      iEnd = a_iStart - 1;
      break;
    } else if('\\' == c) {
      ++a_iStart;
    }
  }
  if(iStart<iEnd && iStart >=0 && iEnd >=0) {
    aStrResult = std::string(aSrc+iStart, iEnd-iStart);
    return true;
  }
  return false;
}
//-----------------------------------------------------------------
/*static*/
bool pjson::ScanString(const char* aSrc, size_t& a_iStart,const size_t a_iEnd, pjson*& a_rStrResult) {
  std::string str;
  if(ExtractString(aSrc, a_iStart, a_iEnd, str)) {
    a_rStrResult = new pjson();
    *a_rStrResult = str;
    return true;
  }
  return false;
}
//-----------------------------------------------------------------
/*static*/
bool pjson::ScanPastColon(const char* aSrc, size_t& a_iStart,const size_t a_iEnd) {
  while(a_iStart<a_iEnd) {
    char c = aSrc[a_iStart++];
    if(':' == c) {
      return true;
    } else if(' ' == c || '\t' == c || '\0' == c || '\n' == c) {
      //ignore
    } else {
      break;    
    } 
  }

  return false;
}
//-----------------------------------------------------------------
/*static*/
bool pjson::ScanToNext(const char* aSrc, size_t& a_iStart,const size_t a_iEnd, char& a_rResult) {
  while(a_iStart<a_iEnd) {
    a_rResult = aSrc[a_iStart];
    if(' ' == a_rResult || '\t' == a_rResult || '\0' == a_rResult || '\n' == a_rResult) {
      ++a_iStart;
    } else {
      return true;
    } 
  }

  return false;
}
//-----------------------------------------------------------------
/*static*/
bool pjson::ScanNull(const char* aSrc, size_t& a_iStart,const size_t a_iEnd, pjson*& a_rNUllResult) {
  if((a_iEnd - a_iStart) >= 4 
     && 'n' == tolower(aSrc[a_iStart])
     && 'u' == tolower(aSrc[a_iStart+1])
     && 'l' == tolower(aSrc[a_iStart+2])
     && 'l' == tolower(aSrc[a_iStart+3])
    ) {
    a_rNUllResult = new pjson();
    a_iStart+=4;
    return true;
  }
  return false; 
}
//-----------------------------------------------------------------
/*static*/
bool pjson::ScanNumber(const char* aSrc, size_t& a_iStart, const size_t a_iEnd, pjson*& a_rNumResult) {
  bool bFloat = false;
  int iEnd = a_iStart;
  enum NumberSection : int {
    NumberSectionSign = 0,
    NumberSectionDigit,
    NumberSectionDot,
    NumberSectionDecimal,
    NumberSectionExponential,
    NumberSectionExponentialSign,
    NumberSectionExponentialDigit,
    NumberEnd
  };
  int eSec = NumberSectionSign;

  for(int i = a_iStart; i<a_iEnd && eSec!=NumberEnd; ) {
    switch ((NumberSection)eSec) {
      case NumberSectionSign: {
        if('+' == aSrc[i] || '-' == aSrc[i]) {
          iEnd = ++i;
        } 
        ++eSec;
        break;
      }
      case NumberSectionDigit: {
        if('0' <= aSrc[i] && '9' >= aSrc[i]) {
          iEnd = ++i;
        } else {
          ++eSec;
        }
        break;
      }
      case NumberSectionDot: {
        if('.' == aSrc[i]) {
          iEnd = ++i;
          bFloat = true;
        } 
        ++eSec;
        break;
      }
      case NumberSectionDecimal: {
        if('0' <= aSrc[i] && '9' >= aSrc[i]) {
          iEnd = ++i;
          bFloat = true;
        } else {
          ++eSec;
        }
        break;
      }
      case NumberSectionExponential: {
        if('e' == tolower(aSrc[i])) {
          iEnd = ++i;
          bFloat = true;
        } 
        ++eSec;
        break;
      }
      case NumberSectionExponentialSign: {
        if('+' == aSrc[i] || '-' == aSrc[i]) {
          iEnd = ++i;
          bFloat = true;
        } 
        ++eSec;
        break;
      }
      case NumberSectionExponentialDigit: {
        if('0' <= aSrc[i] && '9' >= aSrc[i]) {
          iEnd = ++i;
          bFloat = true;
        } else {
          ++eSec;
        }
        break;
      }
      case NumberEnd: {
        break;
      }
    } // end switch
  } //end for

  if(iEnd > a_iStart) {
    std::string sTemp = std::string(aSrc+a_iStart, iEnd-a_iStart);
    a_rNumResult = new pjson();
    if(bFloat) {
      *a_rNumResult = std::stof(sTemp);
    } else {
      *a_rNumResult = std::stoi(sTemp);
    }
    a_iStart = iEnd;
    return true; 
  }
  return false; 
}
//-----------------------------------------------------------------
/*static*/
bool pjson::ScanArray(const char* aSrc, size_t& a_iStart, const size_t a_iEnd, pjson*& a_rAResult) {
  a_rAResult = new pjson();
  a_rAResult->resetTo(jsonType::jsonArray);
  bool bValid = false;
  ++a_iStart; // ignore first char "["
  char aChar;
  while(ScanToNext(aSrc, a_iStart, a_iEnd, aChar)) {
    if(']' == aChar) {
      ++a_iStart;
      bValid = true;
      break;
    } else if(',' == aChar) {
      ++a_iStart; //ignore commas
    } else {
      pjson* pTemp = nullptr;
      if(CreateFromString(aSrc, a_iStart,a_iEnd, pTemp)) {
        a_rAResult->_pValueArray->push_back(pTemp);
      } else {
        break;
      }
    }
  }
  if(!bValid) {
    delete a_rAResult;
    a_rAResult = nullptr;
  }

  return bValid;
}
//-----------------------------------------------------------------
/*static*/
bool pjson::ScanObject(const char* aSrc, size_t& a_iStart, const size_t a_iEnd, pjson*& a_rAResult) {
  a_rAResult = new pjson();
  a_rAResult->resetTo(jsonType::jsonObject);
  bool bValid = false;
  ++a_iStart; // ignore first char "{"
  char aChar;
  while(ScanToNext(aSrc, a_iStart, a_iEnd, aChar)) {
    if('}' == aChar) {
      ++a_iStart;
      bValid = true;
      break;
    } else if(',' == aChar) {
      ++a_iStart; // ignore commas
    } else {
      pjson* pVal = nullptr;
      std::string mkey;
      if(ExtractString(aSrc, a_iStart, a_iEnd, mkey)
          && ScanPastColon(aSrc, a_iStart, a_iEnd)
          && CreateFromString(aSrc, a_iStart, a_iEnd, pVal)) {
        //success
          (*(a_rAResult->_pValueMap))[mkey.c_str()] = pVal;
      } else {
        delete a_rAResult;
        a_rAResult = nullptr;
        bValid = false;
        break;
      }
    }
  }
  return bValid;
}
//-----------------------------------------------------------------
