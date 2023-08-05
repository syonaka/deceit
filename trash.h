#pragma once
#include <iostream>

inline std::string gen_random_str(const int len) {
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";
	std::string tmp_s;
	tmp_s.reserve(len);

	for (int i = 0; i < len; ++i) {
		tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
	}

	return tmp_s;
}

inline int gen_random_int(const int len) {
	static const char alphanum[] = "0123456789";
	std::string tmp_s;
	tmp_s.reserve(len);

	for (int i = 0; i < len; ++i) {
		tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
	}

	return std::stoi(tmp_s);
}

template<typename t>
__forceinline static t CallVFunction(void* ppClass, int index)
{
	unsigned long long* pVTable = *(unsigned long long**)ppClass;
	unsigned long long dwAddress = pVTable[index];
	return (t)(dwAddress);
}

namespace trash
{
	inline float RenderWidth = 0;
	inline float RenderHeight = 0;
}