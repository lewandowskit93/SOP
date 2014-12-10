#include ".\sop\users\encryptors.h"

sop::users::Encryptor::Encryptor()
{
}

std::string sop::users::Encryptor::encrypt(const std::string & data) const
{
  std::string encrypted_data;
  this->doEncrypt(data,&encrypted_data);
  return encrypted_data;
}

sop::users::Encryptor::~Encryptor()
{
}

sop::users::CaesarEncryptor::CaesarEncryptor():
  _shift(0)
{
}

sop::users::CaesarEncryptor::CaesarEncryptor(uint8_t shift):
  _shift(shift)
{
}

void sop::users::CaesarEncryptor::doEncrypt(const std::string & data, std::string *encrypted_data) const
{
  encrypted_data->clear();
  encrypted_data->resize(data.size(),' ');
  for(std::string::size_type i = 0; i<data.size();++i)
  {
    (*encrypted_data)[i]=encryptSign(data[i]);
  }
}

char sop::users::CaesarEncryptor::encryptSign(char sign) const
{
  if((sign>='a' && sign<='z'))
  {
    uint8_t offset = sign-'a';
    offset=(offset+_shift)%('z'-'a'+1);
    return 'a'+offset;
  }
  else if((sign>='A' && sign<='Z'))
  {
    uint8_t offset = sign-'A';
    offset=(offset+_shift)%('Z'-'A'+1);
    return 'A'+offset;
  }
  else return sign;
}

sop::users::CaesarEncryptor::~CaesarEncryptor()
{
}