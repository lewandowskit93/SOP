#ifndef _SOP_STRING_CONVERTER_H_
#define _SOP_STRING_CONVERTER_H_

#include <string>
#include <sstream>

namespace sop
{
  class StringConverter
  {
    public:
      template<class T>
      static T convertStringTo(const std::string & value)
      {
        T conv_value=0;
        if(value!="")
        {
          std::stringstream ss;
          ss<<value;
          ss>>conv_value;
        }
        return conv_value;
      }

      template<> static bool convertStringTo(const std::string & value);

      template<class T>
      static std::string convertToString(const T & value)
      {
        std::stringstream ss;
        ss<<value;
        return ss.str();
      }

      template<> static std::string convertToString(const bool & value);

    private:
      StringConverter();
  };
}

template<> bool sop::StringConverter::convertStringTo(const std::string & value)
{
  if(value=="true" || value=="TRUE" || value == "T" || value=="t") return true;
  else return false;
}

template<> std::string sop::StringConverter::convertToString(const bool & value)
{
  if(value==true)return "TRUE";
  else return "FALSE";
}

#endif