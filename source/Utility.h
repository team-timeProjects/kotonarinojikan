#ifndef _UTILITY_H_
#define _UTILITY_H_

template<typename T>
inline void SafeDelete(T*& ptr)
{
	if (ptr != nullptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}

#endif