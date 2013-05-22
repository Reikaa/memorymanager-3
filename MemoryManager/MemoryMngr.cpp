#include <stdlib.h>
#include <new>
#include <iostream>
#include "MemoryMngr.h"

#if defined(DEBUG) || defined(_DEBUG) || defined(__debug)
MemoryMngr* memoryMngr = initMemoryMngr();
#endif

void atExit(){
    memoryMngr->printStats();
    std::cout << memoryMngr->getCount() << std::endl;
    std::cout << std::endl;
}

MemoryMngr* initMemoryMngr(){
     MemoryMngr* memoryMngr= (MemoryMngr*)malloc(sizeof(MemoryMngr));
     memoryMngr->init();
     atexit(atExit);
     return memoryMngr;
}

// ### MemoryManager ###

void MemoryMngr::init() {
    count = 0;
    created = 0;
    deleted = 0;
}

void MemoryMngr::addCreated(const char* name, void* address, unsigned long size) {
    memoryInfo* info = (memoryInfo*)malloc(sizeof(memoryInfo));
    info->number = ++count;
    info->name = name;
    info->address = address;
    info->size=size;
    info->next = 0;
    info->previous = created;
    if(created != 0){
        created->next = info;
    }
    created = info;
}

void MemoryMngr::addDeleted(const char* name, void* address){
    memoryInfo* info = (memoryInfo*)malloc(sizeof(memoryInfo));
    info->name = name;
    info->address = address;
    info->previous = deleted;
    info->next = 0;
    if(deleted != 0){
        deleted->next = info;
    }
    deleted = info;
}

void MemoryMngr::incrementCount(){
    //count++;
}

void MemoryMngr::decrementCount(){
    //count--;
}

int MemoryMngr::getCount(){
    return count;
}

void MemoryMngr::printStats(){
    memoryInfo* createdInfo = created;
    // revert the list
    while (createdInfo != 0 && createdInfo->previous != 0) {
        createdInfo = createdInfo->previous;
    }
    // print all created
    std::printf("    %-10s %-10s %-10s \n","operator", "size", "address");
    while (createdInfo != 0) {
        std::printf("#%-2d %-10s %-10lu %-10p \n", createdInfo->number,createdInfo->name,createdInfo->size, createdInfo->address);
        memoryInfo* old = createdInfo;
        createdInfo = createdInfo->next;
        free(old);
    }
    
    memoryInfo* leaks = (memoryInfo*)malloc(sizeof(memoryInfo));
    
    memoryInfo* deletedInfo = deleted;
    // revert the list
    while (deletedInfo != 0 && deletedInfo->previous != 0) {
        deletedInfo = deletedInfo->previous;
    }
    // print all created
    std::printf("    %-10s %-10s %-10s \n","operator", "size", "address");
    while (deletedInfo != 0) {
        std::printf("#%-2d %-20s %-10p \n", deletedInfo->number,deletedInfo->name, deletedInfo->address);
        memoryInfo* old = deletedInfo;
        deletedInfo = deletedInfo->next;
        free(old);
    }
}

// ### Override new ###

void* operator new(size_t size) throw(std::bad_alloc){
    memoryMngr->incrementCount();
    void* p = malloc(size);
    if (!p) throw std::bad_alloc();
    memoryMngr->addCreated("new", p, size);
    return p;
}

void* operator new(size_t size, const std::nothrow_t& nothrow_value) noexcept {
    memoryMngr->incrementCount();
    void* p = malloc(size);
    memoryMngr->addCreated("new", p, size);
    return p;
}

void* operator new[](size_t size) throw(std::bad_alloc){
    memoryMngr->incrementCount();
    void* p = malloc(size);
    if (!p) throw std::bad_alloc();
    memoryMngr->addCreated("new[]", p, size);
    return p;
}
void* operator new[](size_t size, const std::nothrow_t& nothrow_value) noexcept {
    memoryMngr->incrementCount();
    void* p = malloc(size);
    memoryMngr->addCreated("new[]", p, size);
    return p;
}

// ### Override delete ###

void operator delete(void *p) throw(){
    memoryMngr->decrementCount();
    memoryMngr->addDeleted("delete", p);
    free(p);
}
void operator delete(void *p, const std::nothrow_t& nothrow_value){
    memoryMngr->decrementCount();
    memoryMngr->addDeleted("delete", p);
    free(p);
}
void operator delete[](void *p) throw(){
    memoryMngr->decrementCount();
    memoryMngr->addDeleted("delete[]", p);
    free(p);
}

void operator delete[](void *p, const std::nothrow_t& nothrow_value){
    memoryMngr->decrementCount();
    memoryMngr->addDeleted("delete[]", p);
    free(p);
}
