/**
 * @file 	libs/base/fix_cpp.cpp
 * @brief 	C++ compatibility definitions.
 */
//###############################################
#include <base/fix_cpp.h>
//###############################################
int __cxa_guard_acquire(__guard* g) {
	return !*(char*)(g);
}

void __cxa_guard_release (__guard* g) {
	*(char*)g = 1;
}

void __cxa_guard_abort (__guard* g) {

}

//###############################################
void __cxa_deleted_virtual(void) {
	abort();
}

void __cxa_pure_virtual(void) {
	abort();
}

//###############################################
void* operator new[](size_t size) {
	return malloc(size);
}

void* operator new(size_t size) {
	return malloc(size);
}

void operator delete[](void* ptr) {
	free(ptr);
}

void operator delete(void* ptr) {
	free(ptr);
}


