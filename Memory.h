#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <stdlib.h>
class Memory
{
	public:
		struct tagMemoryNode
		{
			int 	fd;
			off_t 	size;
			void*	pData;

			void reset()
			{
				if ( pData == NULL ) {
					free(pData);
				}
				pData 	= NULL;
				size 	= 0;
				fd 		= -1;
			}
		};
	public:
		Memory() {}
		~Memory() {}

		static Memory* getInstance();

		const tagMemoryNode* 	createMemory(off_t size);	
		void					deleteMemort();

	private:
		static Memory*	s_Instance;
		tagMemoryNode 	m_memory;
};
#endif
