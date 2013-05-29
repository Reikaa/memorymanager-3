//
//  Header.h
//  MemoryManager
//
//  Created by Robin Wyss on 24.05.13.
//  Copyright (c) 2013 Robin Wyss. All rights reserved.
//
#ifndef GLOBAL_H // header guards
#define GLOBAL_H

#include "MemoryMngr.h"

#ifndef MEMORY_MANAGER

// replace new and delete to first update the line number information.

// the trick here is the && 0 which ensures that the expression is evaluated to false an the new is taken.
#define new setLineNumbers(__FILE__,__LINE__) && 0 ? NULL : new

// delete is replaces by two statement, the prepare and then the delete is appended,
#define delete setLineNumbers(__FILE__,__LINE__);delete

#endif

#endif
#include <stdlib.h>
