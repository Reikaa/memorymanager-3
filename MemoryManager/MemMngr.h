#ifndef MEMORYMNGR_H
#define MEMORYMNGR_H

// structs to store the memory info
struct memoryInfo;
struct memoryInfoList;


// replace new and delete to first update the line number information.

// the trick here is the && 0 which ensures that the expression is evaluated to false an the new is taken.
#define new setLineNumbers(__FILE__,__LINE__) && 0 ? NULL : new

// delete is replaces by two statement, the prepare and then the delete is appended,
#define delete setLineNumbers(__FILE__,__LINE__);delete

// this method returns an int because otherwise it can not be used in the expression which replaces new, since new returns an int.
// there doesn't have to be a return value, since it is never used (the macro ensures that the value from the correct new is returned)
int setLineNumbers(const char* file, size_t line);

// the memory manager class;

class MemoryMngr {
private:
    memoryInfoList* infoList;
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
