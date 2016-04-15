#pragma once

#include <cassert>

template <class T> class Singleton {
protected:
	static T *pInstance;
	Singleton() = default;
	Singleton(const Singleton&) = delete;
	Singleton(const Singleton&&) = delete;
	Singleton &operator=(const Singleton) = delete;
public:
	static T *getInstance() {
		static bool Initialized = false;
		if (!Initialized) {
			pInstance = nullptr;
			while (!pInstance)
				pInstance = new T();
		}
		assert(pInstance && "Cannot allocate memory for singleton instance!");
		Initialized = true;
		return pInstance;
	}

	~Singleton() {
		if (pInstance)
			delete pInstance;
		pInstance = nullptr;
	}
};

template <class T> T *Singleton<T>::pInstance = nullptr;