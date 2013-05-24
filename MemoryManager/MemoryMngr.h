#ifndef MEMORYMNGR_H
#define MEMORYMNGR_H


struct memoryInfo
{
    int number;
    bool isAllocation = false;
    bool isArray = false;
    bool isDeallocated = false;
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
    void add(bool isAllocation, bool array, void* address, unsigned long size) {
        memoryInfo* info = (memoryInfo*)malloc(sizeof(memoryInfo));
        info->number = ++index;
        info->isAllocation = isAllocation;
        info->isArray = array;
        info->address = address;
        info->size=size;
        info->next = 0;
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

#endif // MEMORYMNGR_H
