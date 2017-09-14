#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <stdlib.h>
#include <boost/bitmap.hpp>

using namespace boost;
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
		typedef bimap<int, tagMemoryNode*> 	MemoryMap;
	public:
		Memory() {}
		~Memory() {}

		static Memory* 				getInstance();

		int 				 		createMemory(off_t size);	
		void						deleteMemory(int fd);

		const tagMemoryNode*		getMemory(int fd);

	private:
		static Memory*				s_Instance;
		MemoryMap 					m_mapMemory;
};
#endif
