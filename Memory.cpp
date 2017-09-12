#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include "Memory.h"

Memory* Memory::s_Instance = NULL;

static const char* DEFAULT_FILE_PATH 		= "/opt/wayland_buffer/";
static const char* DEFAULT_FILE_TEMPLATE 	= "share-buffer-XXXXXX";

Memory* Memory::getInstance()
{
	if( NULL == s_Instance ) {
		s_Instance = (Memory*)malloc(sizeof(Memory));
	}
	assert(NULL != s_Instance);
	return s_Instance;
}

const Memory::tagMemoryNode* Memory::createMemory(off_t size)
{
	char* name = (char *)malloc(strlen(DEFAULT_FILE_PATH) + strlen(DEFAULT_FILE_TEMPLATE));

	strncpy(name, DEFAULT_FILE_PATH, strlen(DEFAULT_FILE_PATH));
	strncpy(name + strlen(DEFAULT_FILE_PATH), DEFAULT_FILE_TEMPLATE, strlen(DEFAULT_FILE_TEMPLATE));
	
	printf("name: %s\n", name);
	int fd = mkostemp(name, O_CLOEXEC | O_CREAT );
	perror("mkostemp");
	assert ( fd > 0 );	
	unlink( name );
	free(name);

	assert( ftruncate(fd, size) == 0 );

	void* pData = mmap(NULL, size, PROT_WRITE | PROT_WRITE, MAP_SHARED, fd, 0);

	assert ( pData != MAP_FAILED );
	
	m_memory.fd 	= fd;
	m_memory.size 	= size;
	m_memory.pData 	= pData;

	return &m_memory;
}

void Memory::deleteMemort()
{
	m_memory.reset();
}
