#include "pjson.h"

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
