#ifndef _SOP_USERS_ENCRYPTORS_H_
#define _SOP_USERS_ENCRYPTORS_H_

#include <string>
#include <stdint.h>

namespace sop
{
  namespace users
  {
    class Encryptor
    {
      public:
        Encryptor();
        std::string encrypt(const std::string & data) const;
        virtual ~Encryptor();
      protected:
        virtual void doEncrypt(const std::string & data, std::string * encrypted_data) const = 0;
      private:
    };

    class CaesarEncryptor : public Encryptor
    {
      public:
        CaesarEncryptor(uint8_t shift);
        ~CaesarEncryptor();
      protected:
        uint8_t _shift;
        virtual void doEncrypt(const std::string & data, std::string * encrypted_data) const;
        char encryptSign(char sign) const;
      private:
        CaesarEncryptor();
    };
  }
}

#endif