#ifndef _VINBERO_IDUMMY_H
#define _VINBERO_IDUMMY_H

#include <vinbero/vinbero_Module.h>

#define VINBERO_IDUMMY_FUNCTIONS \
int vinbero_IDummy_service(struct vinbero_Module* module)

#define VINBERO_IDUMMY_FUNCTION_POINTERS \
int (*vinbero_IDummy_service)(struct vinbero_Module*)

#define VINBERO_IDUMMY_DLSYM(module, localModuleType) \
VINBERO_MODULE_DLSYM(module, localModuleType, vinbero_IDummy_service)

#endif
