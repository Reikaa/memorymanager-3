#ifndef MEMORYMNGR_H
#define MEMORYMNGR_H

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
    void remove(memoryInfo* element){
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
    }
};


class MemoryMngr {
private:
    memoryInfoList* created;
    memoryInfoList* deleted;
public:
    void init(void);
    void printStats();
    void addCreated(const char* name, bool array, void* address, unsigned long size);
    void addDeleted(const char* name, bool array, void* address, unsigned long size);
};

void atExit();
MemoryMngr* initMemoryMngr();
//MemoryMngr* memoryMngr;

#if defined(DEBUG) || defined(_DEBUG) || defined(__debug)
MemoryMngr* memoryMngr = initMemoryMngr();
#endif

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


#endif // MEMORYMNGR_H
