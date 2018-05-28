#ifndef _VINBERO_IDUMMY_H
#define _VINBERO_IDUMMY_H

#include <vinbero_common/vinbero_common_Module.h>

#define VINBERO_IDUMMY_FUNCTIONS \
int vinbero_IDUMMY_service(struct vinbero_common_Module* module)

#define VINBERO_IDUMMY_FUNCTION_POINTERS \
int (*vinbero_IDUMMY_service)(struct vinbero_common_Module*)

struct vinbero_IDUMMY_Interface {
    VINBERO_IDUMMY_FUNCTION_POINTERS;
};

#define VINBERO_IDUMMY_DLSYM(interface, dlHandle, ret) \
do { \
    VINBERO_COMMON_MODULE_DLSYM(interface, dlHandle, vinbero_IDUMMY_service, ret); \
    if(*(ret) < 0) break; \
} while(0)


#endif
