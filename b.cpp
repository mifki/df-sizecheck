// g++ -dynamiclib -undefined suppress -flat_namespace -arch x86_64 -arch i386 b.cpp -o b.dylib
// DYLD_LIBRARY_PATH="./hack:./libs:./hack/libs" DYLD_FRAMEWORK_PATH="./hack:./libs:./hack/libs" DYLD_INSERT_LIBRARIES=./b.dylib:./hack/libdfhack.dylib ./dwarfort.exe 


#include <stdio.h>
#include <stdlib.h>

#include <memory>

using namespace std;

void* operator new  ( std::size_t n, const std::nothrow_t& tag)
{
    void *addr = malloc(n+16);
    bzero(addr, n+16);
    *(size_t*)addr = n;
    *(uint32_t*)((char*)addr+8) = 0x11223344;

    return (char*)addr+16;
}

void * operator new(std::size_t n) throw(std::bad_alloc)
{
    void *addr = malloc(n+16);
    bzero(addr, n+16);
    *(size_t*)addr = n;
    *(uint32_t*)((char*)addr+8) = 0x11223344;
    
    return (char*)addr+16;
}

void  operator delete  ( void* ptr   )
{
    return;
}

