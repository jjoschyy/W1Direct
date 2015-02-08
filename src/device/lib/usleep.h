#ifdef _WIN32
	static inline void usleep(int count){}
#else
	#include <unistd.h>
#endif
