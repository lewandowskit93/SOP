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
	static const int8_t storage_size = 256; //zmienna odpowiadaj�ca za rozmiar pami�ci
	static const int16_t storage [storage_size]; //w�a�ciwa pamie� fizyczna, 256*16=4096 bajt�w pami�ci

};

}
}



#endif // _SOP_MEMORY_STORAGE_H_
