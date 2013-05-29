#ifndef MEMORYMNGR_H
#define MEMORYMNGR_H

// structs to store the memory info
struct memoryInfo;
struct memoryInfoList;

int setLineNumbers(const char* file, size_t line);

// the memory manager class;

class MemoryMngr {
private:
    memoryInfoList* created;
public:
    void init(void);
    void printStats();
    void addMemoryInfo(bool allocation, bool array, void* address, unsigned long size);
    void printSeparator(int length = 40);
    void printTitle(const char* title);
    void printMemoryInfo(memoryInfo* memInfo, unsigned long currentAllocationSize);
    void printLine(const char* file, size_t line);
};

void atExit();

MemoryMngr* initMemoryMngr();

#endif // MEMORYMNGR_H
