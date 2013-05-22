#ifndef MEMORYMNGR_H
#define MEMORYMNGR_H

struct memoryInfo
{
    int number;
    const char* name;
    void* address;
    unsigned long size;
    memoryInfo* previous = 0;
    memoryInfo* next = 0;
};

struct memoryInfoList
{
    int count = 0;
    memoryInfo* list = 0;
    void add(const char* name, void* address, unsigned long size) {
        memoryInfo* info = (memoryInfo*)malloc(sizeof(memoryInfo));
        info->number = ++count;
        info->name = name;
        info->address = address;
        info->size=size;
        info->next = 0;
        info->previous = list;
        if(list != 0){
            list->next = info;
        }
        list = info;
    }
    void revert(){
        memoryInfo* element = list;
        // revert the list
        while (element != 0 && element->previous != 0) {
            element = element->previous;
        }
    }
};



class MemoryMngr{
private:
    int count;
    memoryInfo* created;
    memoryInfo* deleted;
public:
    int getCount();
    void init(void);
    void incrementCount();
    void decrementCount();
    void printStats();
    void addCreated(const char* name, void* address, unsigned long size);
    void addDeleted(const char* name, void* address);
};

void atExit();
MemoryMngr* initMemoryMngr();

//void* operator new(size_t size) throw (std::bad_alloc);
//void* operator new(size_t size, const std::nothrow_t& nothrow_value) noexcept;
//void* operator new[](size_t size) throw (std::bad_alloc);
//void* operator new[](size_t size, const std::nothrow_t& nothrow_value) noexcept;

#endif // MEMORYMNGR_H
