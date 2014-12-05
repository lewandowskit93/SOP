#ifndef _SOP_MEMORY_LOGICAL_MEMORY_H_
#define _SOP_MEMORY_LOGICAL_MEMORY_H_

#include <stdint.h>

namespace sop
{
namespace logical
{
struct page //struktura strony
{
  int8_t frame_number;//nr ramki do jakej przynale¿y stron
  int8_t process_id;//do jakiego procesu nale¿y strona
  char valid_invalid;//czy strona w pamiêci czy nie
};
class LogicalMemory
{
public:	

protected:

private:
	static const int8_t storage_size = 256; //zmienna odpowiadaj¹ca za rozmiar tabeli stron
	static const page page_table[storage_size];//tabela stron, rekordów tyle ile ramek
};

}
}



#endif // _SOP_MEMORY_LOGICAL_MEMORY_H_