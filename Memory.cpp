#include <assert.h>
#include "Memory.h"

static Memory* s_Instance = NULL;

Memory* Memory::getInstance()
{
	if( NULL == s_Instance ) {
		s_Instance = new Memory();
	}
	assert(NULL == s_Instance);
	return s_Instance;
}




