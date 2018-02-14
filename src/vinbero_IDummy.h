#ifndef _VINBERO_IDUMMY_H
#define _VINBERO_IDUMMY_H

#include <vinbero/vinbero_Module.h>

#define VINBERO_IDUMMY_FUNCTIONS \
int vinbero_IDummy_service(struct vinbero_Module* module)

#define VINBERO_IDUMMY_FUNCTION_POINTERS \
int (*vinbero_IDummy_service)(struct vinbero_Module*)

struct vinbero_IDummy_Interface {
    VINBERO_IDUMMY_FUNCTION_POINTERS;
};

#define VINBERO_IDUMMY_DLSYM(interface, childModule, errorVariable)                      \
do {                                                                                        \
    VINBERO_MODULE_DLSYM(interface, childModule, vinbero_IDummy_service, errorVariable); \
    if(*errorVariable == 1) break;                                                          \
} while(0)


#endif
