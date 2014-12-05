#ifndef _SOP_MEMORY_STORAGE_H_
#define _SOP_MEMORY_STORAGE_H_

#include <stdint.h>

namespace sop
{
namespace storage
{
class Storage
{
public:	

protected:

private:
	static const int8_t storage_size = 256; //zmienna odpowiadaj¹ca za rozmiar pamiêci
	static const int16_t storage [storage_size]; //w³aœciwa pamieæ fizyczna, 256*16=4096 bajtów pamiêci

};

}
}



#endif // _SOP_MEMORY_STORAGE_H_
