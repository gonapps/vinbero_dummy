#ifndef _VINBERO_INTERFACE_DUMMY_H
#define _VINBERO_INTERFACE_DUMMY_H

#include <vinbero_common/vinbero_common_Module.h>

#define VINBERO_INTERFACE_DUMMY_FUNCTIONS \
int vinbero_interface_DUMMY_service(struct vinbero_common_Module* module)

#define VINBERO_INTERFACE_DUMMY_FUNCTION_POINTERS \
int (*vinbero_interface_DUMMY_service)(struct vinbero_common_Module*)

struct vinbero_interface_DUMMY {
    VINBERO_INTERFACE_DUMMY_FUNCTION_POINTERS;
};

#define VINBERO_INTERFACE_DUMMY_DLSYM(interface, dlHandle, ret) \
do { \
    VINBERO_COMMON_MODULE_DLSYM(interface, dlHandle, vinbero_interface_DUMMY_service, ret); \
    if(*(ret) < 0) break; \
} while(0)


#endif
