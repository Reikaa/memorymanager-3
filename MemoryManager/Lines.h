//
//  Lines.h
//  MemoryManager
//
//  Created by Robin Wyss on 28.05.13.
//  Copyright (c) 2013 Robin Wyss. All rights reserved.
//

#ifndef MemoryManager_Lines_h
#define MemoryManager_Lines_h

const char* __file__ = "unknown";
size_t __line__ = 0;

int PrepareNew(const char* file, size_t line) {
    __file__ =  file;
    __line__ = line;
}

//#define new (__file__=__FILE__,__line__=__LINE__) & 0 ? NULL : new
#define new PrepareNew(__FILE__,__LINE__) & 0 ? NULL : new
#define delete PrepareNew(__FILE__,__LINE__);delete


#endif
