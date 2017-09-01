#ifndef __MEMORY_H__
#define __MEMORY_H__

class Memory
{
	public:
		Memory();
		~Memory();

		
		
		static Memory* getInstance();

	private:
		static Memory *s_Instance;
};


#endif
