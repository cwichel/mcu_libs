/**
 * @code 	#include <base/fix_cpp.h> @endcode
 * @file 	libs/base/fix_cpp.h
 * @brief	C++ compatibility definitions.
 */
//###############################################
#ifndef _BASE_FIX_CPP_H_
#define _BASE_FIX_CPP_H_
//###############################################
extern "C" {
#include <stdlib.h>
}

//###############################################
__extension__ typedef int __guard __attribute__((mode (__DI__)));

//###############################################
extern "C" int __cxa_guard_acquire(__guard*);
extern "C" void __cxa_guard_release (__guard*);
extern "C" void __cxa_guard_abort (__guard*);

//###############################################
extern "C" void __cxa_deleted_virtual(void);
extern "C" void __cxa_pure_virtual(void);

//###############################################
void* operator new[](size_t size);
void* operator new(size_t size);

void operator delete[](void* ptr);
void operator delete(void* ptr);

//###############################################
#endif /* _BASE_FIX_CPP_H_ */
//###############################################
