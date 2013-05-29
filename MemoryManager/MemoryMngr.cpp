#include <stdlib.h>
#include <new>
#include <iostream>
#include "MemoryMngr.h"

#define MEMORY_MANAGER
#include "LineInfo.h"

struct memoryInfo
{
    int number;
    bool isAllocation = false;
    bool isArray = false;
    bool isDeallocated = false;
    const char* file;
    size_t line ;
    void* address;
    unsigned long size;
    memoryInfo* previous = 0;
    memoryInfo* next = 0;
};

struct memoryInfoList
{
    int size = 0;
    int index = 0;
    memoryInfo* first = 0;
    memoryInfo* last = 0;
    void add(bool isAllocation, bool array, void* address, unsigned long size, const char* file,size_t line) {
        memoryInfo* info = (memoryInfo*)malloc(sizeof(memoryInfo));
        info->number = ++index;
        info->isAllocation = isAllocation;
        info->isArray = array;
        info->address = address;
        info->size=size;
        info->next = 0;
        info->file = file;
        info->line = line;
        info->previous = last;
        if(last != 0){
            last->next = info;
        } else {
            first = info;
        }
        last = info;
        size++;
    }
    memoryInfo* remove(memoryInfo* element, bool returnPrevious = false){
        memoryInfo* returnValue = returnPrevious ? element->previous : element->next;
        if(element->previous == 0 && element->next == 0){
            first = last = 0;
        } else if(element->previous == 0){
            first = element->next;
            element->next->previous = 0;
        } else if(element->next == 0){
            last = element->previous;
            element->previous->next = 0;
        } else {
            element->previous->next = element->next;
            element->next->previous = element->previous;
        }
        free(element);
        size--;
        return returnValue;
    }
};

const char* current_file_;
size_t current_line_ ;


int setLineNumbers(const char* file, size_t line){
   current_line_ = line;
   current_file_ = file;
}

#if defined(DEBUG) || defined(_DEBUG) || defined(__debug)
MemoryMngr* memoryMngr = initMemoryMngr();
#endif

MemoryMngr* initMemoryMngr(){
    MemoryMngr* memoryMngr= (MemoryMngr*)malloc(sizeof(MemoryMngr));
    memoryMngr->init();
    atexit(atExit);
    return memoryMngr;
}

// ### at exit ###
void atExit(){
    memoryMngr->printStats();
}

// ### MemoryManager ###

void MemoryMngr::init() {
    created = (memoryInfoList*)malloc(sizeof(memoryInfoList));
}

void MemoryMngr::addMemoryInfo(bool allocation, bool array, void* address, unsigned long size) {
//    created->add(allocation, array,address, size, __file__, __line__);
        created->add(allocation, array,address, size,current_file_,current_line_);
}

void MemoryMngr::printStats(){
    printTitle("Memory Statistics");
    unsigned long maxUsedMemory = 0;
    memoryInfo* memInfo = created->first;
    unsigned long currentAllocationSize = 0;
    std::printf("#  %-10s %6s %6s %10s \n","operator", "size", "address", "memory");
    printSeparator();
    while (memInfo != 0) {
        if(memInfo->isAllocation) {
            currentAllocationSize += memInfo->size;
            printMemoryInfo(memInfo, currentAllocationSize);
        } else {
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
                printMemoryInfo(memInfo, currentAllocationSize);
                if(wrongDelete){
                    std::printf(" *** wrong delete ***");
                    printLine(memInfo->file, memInfo->line );
                }
                created->remove(allocation);
            } else {
                std::cout << "not found";
            }
        }
        // compute max used memory
        if(currentAllocationSize > maxUsedMemory){
            maxUsedMemory = currentAllocationSize;
        }
        std::cout << std::endl;
        // only delete if it is not an allocation, we need them to match with the deallocation to check for leaks.
        memInfo = memInfo->isAllocation ? memInfo->next : created->remove(memInfo);
    }
    
    std::printf("\nMemory Leaks\n");
    printSeparator();
    unsigned long leakSize = 0;
    int numberOfLeaks =0;
    memInfo = created->first;
    while (memInfo != 0) {
        numberOfLeaks++;
        leakSize += memInfo->size;
        printMemoryInfo(memInfo, leakSize);
        printLine(memInfo->file, memInfo->line);
        std::cout << std::endl;
        memInfo = created->remove(memInfo);
    }
    
    std::printf("\nMemory Usage\n");
    printSeparator();
    std::printf("%-20s %d\n", "No. of leaks:", numberOfLeaks);
    std::printf("%-20s %lu\n", "Max used memory:", maxUsedMemory);
    std::printf("%-20s %lu\n", "Leaked memory:", leakSize);
}

void MemoryMngr::printSeparator(int length){
    std::cout.fill('-');
    std::cout.width(length);
    std::cout << "" << std::endl;
}

void MemoryMngr::printTitle(const char* title){
    printSeparator();
    std::cout << title << std::endl;
    printSeparator();
    std::cout << "\n";
}

void MemoryMngr::printMemoryInfo(memoryInfo* memInfo, unsigned long currentAllocationSize){
    std::string operatorName = (std::string)(memInfo->isAllocation ? "new" : "delete") + (std::string)(memInfo->isArray ? "[]" : "");
    std::printf("%-2d %-10s %6lu %10p %6lu", memInfo->number,operatorName.c_str(),memInfo->size, memInfo->address, currentAllocationSize);
}

void MemoryMngr::printLine(const char* file, size_t line){
    std::printf(" %s:%zu", file, line);
}


// ### Override new ###

void* operator new(size_t size) throw(std::bad_alloc){
    void* p = malloc(size);
    if (!p) throw std::bad_alloc();
    memoryMngr->addMemoryInfo(true,false, p, size);
    return p;
}

void* operator new(size_t size, const std::nothrow_t& nothrow_value) noexcept {
    void* p = malloc(size);
    memoryMngr->addMemoryInfo(true,false, p, size);
    return p;
}

void* operator new[](size_t size) throw(std::bad_alloc){
    void* p = malloc(size);
    if (!p) throw std::bad_alloc();
    memoryMngr->addMemoryInfo(true,true, p, size);
    return p;
}
void* operator new[](size_t size, const std::nothrow_t& nothrow_value) noexcept {
    void* p = malloc(size);
    memoryMngr->addMemoryInfo(true,true, p, size);
    return p;
}

// ### Override delete ###

void operator delete(void *p) throw(){
    memoryMngr->addMemoryInfo(false , false,p, sizeof(&p));
    free(p);
}
void operator delete(void *p, const std::nothrow_t& nothrow_value){
    memoryMngr->addMemoryInfo(false, false, p, sizeof(&p));
    free(p);
}
void operator delete[](void *p) throw(){
    memoryMngr->addMemoryInfo(false, true, p, sizeof(&p));
    free(p);
}

void operator delete[](void *p, const std::nothrow_t& nothrow_value){
    memoryMngr->addMemoryInfo(false, true, p, sizeof(&p));
    free(p);
}
