#pragma once

#include <cassert>

template <class T> class Singleton {
public:
	static T *getInstance() {
		static T *pInstance = nullptr;
		while (!pInstance)
			pInstance = new T();
		assert(pInstance && "Cannot allocate memory for singleton instance!");
		return pInstance;
	}
};
