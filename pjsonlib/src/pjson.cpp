#include "pjson.h"


//-----------------------------------------------------------------
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
std::string pjson::toString() const {
  std::string out;
  switch(_eType) {
    case jsonType::jsonNull:         { out = "null"; break; }
    case jsonType::jsonString:       { out = "\""; out += *_pValueString; out += "\""; break; }
    case jsonType::jsonNumberInt:    { out = std::to_string(*_pValueInt); break; }
    case jsonType::jsonNumberFloat:  { out = std::to_string(*_pValueFloat); break; }
    case jsonType::jsonBoolean:      { out = (*_pValueBool)?"true":"false"; break; }
    case jsonType::jsonArray:  {
      out = "[";
      bool bFirstElement = true;
      for (auto it = _pValueArray->begin(); it != _pValueArray->end(); it++) {
        if(!bFirstElement) {
          out += ",";
        }
        out += " ";
        out += (*it)->toString();
        out += " ";
        bFirstElement = false;
      }
      if(bFirstElement) {
          out += " ";
      }
      out += "]";
      break;
    }
    case jsonType::jsonObject: {
      out = "{";
      bool bFirstElement = true;
      for (auto it = _pValueMap->begin(); it != _pValueMap->end(); it++) {
        if(!bFirstElement) {
          out += ",";
        }
        out += " \"";
        out += it->first;
        out += "\" : ";
        out += it->second->toString();
        out += " ";
        bFirstElement = false;
      }
      if(bFirstElement) {
          out += " ";
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
pjson* pjson::CreateFromString(const char* aSrc, size_t& a_iStart,size_t a_iEnd, pjson* a_pParent) {
  //1. Scan for fundametal type
  jsonType eType = jsonType::jsonError;
  while (a_iStart<a_iEnd) {
    char aChar = tolower(aSrc[a_iStart]);
    if('{' == aChar) {
      eType = jsonType::jsonObject;
    } else if('[' == aChar) {
      eType = jsonType::jsonArray;
    } else if('\"' == aChar) {
      eType = jsonType::jsonString;
    } else if('n' == aChar) {
      eType = jsonType::jsonNull;
    } else if('t' == aChar || 'f' == aChar) {
      eType = jsonType::jsonBoolean;
    } else if('.' == aChar|| '-' == aChar || ('0' <= aChar && '9' >= aChar)) {
      eType = jsonType::jsonNumberInt;
      for(int i = a_iStart; i<a_iEnd;++i) {
        if(aSrc[i] == '.' || tolower(aSrc[i]) == 'e' || aSrc[i] == '+') {
          eType = jsonType::jsonNumberFloat;
          break;
        } else if('-' == aChar || ('0' <= aChar && '9' >= aChar)) {
          continue;
        } else {
          break;
        }
      }
    } else if(' ' == aChar || '\t' == aChar || '\0' == aChar || '\n' == aChar) {
      //ignore
      ++a_iStart;
    } else {
      break; // Invalid Json
    }
  } // end while

  //2. Create Type;
  if(eType == jsonType::jsonError) {
    return nullptr;
  }

  pjson* pResult = nullptr;
  switch (eType) {
    case jsonType::jsonNull: {
      if((a_iEnd - a_iStart) >= 4 && 0==stricmp(aSrc+a_iStart,"null")) {
        pResult = new pjson();
        bool bSet = true;
        *pResult = bSet;
        a_iStart+=4;
      }
      break; 
    }
    case jsonType::jsonString: {
      int i = a_iStart + 1;
      while(i<a_iEnd) {
        if('\\' == aSrc[i]) {
          ++i;
        } else if('\"' == aSrc[i]) {
          break;
        }
        ++i;
      }
      if(i<a_iEnd) {
        pResult = new pjson();
        *pResult = std::string(aSrc+a_iStart+1, i - a_iStart -1);
        a_iStart = i + 1;
      }
      break;
    }
    case jsonType::jsonNumberInt:    {
      int i = a_iStart+1; // ignore the first '-' or digit
      while(i<a_iEnd) {
        if(aSrc[i] >= '0' && aSrc[i] <= '9') {
          ++i;
        } else {
          break;
        }
      }
      if(i<a_iEnd) {
        std::string sTemp = std::string(aSrc+a_iStart, i-a_iStart);
        pResult = new pjson();
        *pResult = std::stoi(sTemp);
        a_iStart = i;
      }
      break;
    }
    case jsonType::jsonNumberFloat: {
      int i = a_iStart+1; // ignore the first '-' or digit
      while(i<a_iEnd) {
        if('.' == aSrc[i] || '+' == aSrc[i] || 'e' == tolower(aSrc[i]) || (aSrc[i] >= '0' && aSrc[i] <= '9')) {
          ++i;
        } else {
          break;
        }
      }
      if(i<a_iEnd) {
        std::string sTemp = std::string(aSrc+a_iStart, i-a_iStart);
        pResult = new pjson();
        *pResult = std::stof(sTemp);
        a_iStart = i;
      }
      break;
    }
    case jsonType::jsonBoolean: {
      if((a_iEnd - a_iStart) >= 4 && 0==stricmp(aSrc+a_iStart,"true")) {
        pResult = new pjson();
        bool bSet = true;
        *pResult = bSet;
        a_iStart+=4;
      } else if((a_iEnd - a_iStart) >= 5 && 0==stricmp(aSrc+a_iStart,"false")) {
        pResult = new pjson();
        bool bSet = false;
        *pResult = bSet;
        a_iStart+=5;
      }   
      break; 
    }
    case jsonType::jsonArray: {
      pResult = new pjson();
      pResult->resetTo(jsonType::jsonArray);
      int iCount = 0, iCommaCount = 0;
      bool bValid = false;
      ++a_iStart; // ignore first char "["
      while(a_iStart<a_iEnd) {
        char aChar = aSrc[a_iStart++];
        if(' ' == aChar || '\t' == aChar || '\0' == aChar || '\n' == aChar) {
          //ignore
        } else if(']' == aChar) {
          bValid = true;
          break;
        } else if(',' == aChar) {
          ++iCommaCount;
          if(iCommaCount > iCount) {
            pjson& pTemp = pResult[iCount++]; 
          }
        } else {
           pResult[iCount++] = CreateFromString(aSrc, a_iStart,a_iEnd, pResult); 
        }
      }
      if(!bValid) {
        delete pResult;
        pResult = nullptr;
      }
      break;
    }
    case jsonType::jsonObject:       { 
      pResult = new pjson();
      pResult->resetTo(jsonType::jsonObject);
      int iCount = 0;
      bool bValid = false;
      ++a_iStart; // ignore first char "{"
      while(a_iStart<a_iEnd) {
        char aChar = aSrc[a_iStart++];
        if(' ' == aChar || '\t' == aChar || '\0' == aChar || '\n' == aChar) {
          //ignore
        } else if('}' == aChar) {
          bValid = true;
          break;
        } else if('\"' == aChar) {
          ++iCommaCount;
          if(iCommaCount > iCount) {
            pjson& pTemp = pResult[iCount++]; 
          }
        } else {
           pResult[iCount++] = CreateFromString(aSrc, a_iStart,a_iEnd, pResult); 
        }
      }
      if(!bValid) {
        delete pResult;
        pResult = nullptr;
      }
      break;
    }
  } //end switch

  if(!pResult) {
    a_iStart = a_iEnd;
  }

  return pResult;
} //end of function
