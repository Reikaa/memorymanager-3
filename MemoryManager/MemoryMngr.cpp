#include <stdlib.h>
#include <new>
#include <iostream>
#include "MemoryMngr.h"
#include "Header.h"

//const char* __file__;
//size_t __line__ ;


#if defined(DEBUG) || defined(_DEBUG) || defined(__debug)
MemoryMngr* memoryMngr = initMemoryMngr();
#endif

MemoryMngr* initMemoryMngr(){
    MemoryMngr* memoryMngr= (MemoryMngr*)malloc(sizeof(MemoryMngr));
    memoryMngr->init();
    atexit(atExit);
    return memoryMngr;
}

// ### print utils ###
void printSeparator(int length = 40){
    std::cout.fill('-');
    std::cout.width(length);
    std::cout << "" << std::endl;
}

// ### at exit ###

void atExit(){
    printSeparator();
    std::cout << "Memory Statistics" << std::endl;
    printSeparator();
    std::cout << "\n";
    memoryMngr->printStats();
}

// ### MemoryManager ###

void MemoryMngr::init() {
    created = (memoryInfoList*)malloc(sizeof(memoryInfoList));
    deleted = (memoryInfoList*)malloc(sizeof(memoryInfoList));
}

void MemoryMngr::addCreated(const char* name, bool array, void* address, unsigned long size) {
    created->add(true, array,address, size, __file__, __line__);
}

void MemoryMngr::addDeleted(const char* name, bool array, void* address, unsigned long size){
    created->add(false, array,address, size, __file__, __line__);
}

void MemoryMngr::printStats(){
    unsigned long maxUsedMemory = 0;
    // print all created
    memoryInfo* memInfo = created->first;
    unsigned long currentAllocationSize = 0;
    //    memoryInfo* otherMemoryInfo = created->first;
    std::printf("#  %-10s %-10s %-12s %10s \n","operator", "size", "address", "memory");
    printSeparator();
    while (memInfo != 0) {
        std::string operatorName = (std::string)(memInfo->isAllocation ? "new" : "delete") + (std::string)(memInfo->isArray ? "[]" : "");
        if(!memInfo->isAllocation) {
            memoryInfo* allocation = created->first;
            int index = -1;
            bool wrongDelete = false;
            while (allocation != 0 && index < 0) {
                if(allocation->isAllocation && allocation->address == memInfo->address){
                    index = allocation->number;
                    wrongDelete = allocation->isArray != memInfo->isArray;
                    allocation->isDeallocated = true;
                } else {
                    allocation = allocation->next;
                }
            }
            if(index > -1){ // should always be the case
                currentAllocationSize -= allocation->size;
                std::printf("%-2d %-19s %-10p %10lu", index,operatorName.c_str(), memInfo->address, currentAllocationSize );
                if(wrongDelete){
                    std::cout << " *** wrong delete ***";
                }
                created->remove(allocation);
            } else {
                std::cout << "not found";
            }
        } else {
            currentAllocationSize += memInfo->size;
            std::printf("%-2d %-10s %8lu %10p %10lu %10s:%zu", memInfo->number,operatorName.c_str(),memInfo->size, memInfo->address, currentAllocationSize, memInfo->file, memInfo->line);
        }
        if(currentAllocationSize > maxUsedMemory){
            maxUsedMemory = currentAllocationSize;
        }
        std::cout << std::endl;
        memoryInfo* old = memInfo;
        memInfo = memInfo->next;
        if(!old->isAllocation){
            created->remove(old);
        }
    }
    
    std::printf("\nMemory Leaks\n");
    printSeparator();
    unsigned long leakSize = 0;
    int numberOfLeaks =0;
    memInfo = created->first;
    while (memInfo != 0) {
        std::string operatorName = (std::string)(memInfo->isAllocation ? "new" : "delete") + (std::string)(memInfo->isArray ? "[]" : "");
        std::printf("%-2d %-10s %8lu %10p \n", memInfo->number,operatorName.c_str(),memInfo->size, memInfo->address);
        
        numberOfLeaks++;
        leakSize += memInfo->size;
        
        memoryInfo* old = memInfo;
        memInfo = memInfo->next;
        created->remove(old);
        
    }
    
    std::printf("\nMemory Usage\n");
    printSeparator();
    std::printf("%-20s %d\n", "No. of leaks:", numberOfLeaks);
    std::printf("%-20s %lu\n", "Max used memory:", maxUsedMemory);
    std::printf("%-20s %lu\n", "Leaked memory:", leakSize);
}

// ### Override new ###

void* operator new(size_t size) throw(std::bad_alloc){
    void* p = malloc(size);
    if (!p) throw std::bad_alloc();
    memoryMngr->addCreated("new", false, p, size);
    return p;
}

void* operator new(size_t size, const std::nothrow_t& nothrow_value) noexcept {
    void* p = malloc(size);
    memoryMngr->addCreated("new", false, p, size);
    return p;
}

void* operator new[](size_t size) throw(std::bad_alloc){
    void* p = malloc(size);
    if (!p) throw std::bad_alloc();
    memoryMngr->addCreated("new[]", true, p, size);
    return p;
}
void* operator new[](size_t size, const std::nothrow_t& nothrow_value) noexcept {
    void* p = malloc(size);
    memoryMngr->addCreated("new[]", true, p, size);
    return p;
}

// ### Override delete ###

void operator delete(void *p) throw(){
    memoryMngr->addDeleted("delete", false,p, sizeof(&p));
    free(p);
}
void operator delete(void *p, const std::nothrow_t& nothrow_value){
    memoryMngr->addDeleted("delete", false, p, sizeof(&p));
    free(p);
}
void operator delete[](void *p) throw(){
    memoryMngr->addDeleted("delete[]", true, p, sizeof(&p));
    free(p);
}

void operator delete[](void *p, const std::nothrow_t& nothrow_value){
    memoryMngr->addDeleted("delete[]", true, p, sizeof(&p));
    free(p);
}

