#ifndef _TUCUBE_IDUMMY_H
#define _TUCUBE_IDUMMY_H

#include <tucube/tucube_Module.h>

#define TUCUBE_IDUMMY_FUNCTIONS \
int tucube_IDummy_service(struct tucube_Module* module)

#define TUCUBE_IDUMMY_FUNCTION_POINTERS \
int (*tucube_IDummy_service)(struct tucube_Module*)

#define TUCUBE_IDUMMY_DLSYM(module, localModuleType) \
TUCUBE_MODULE_DLSYM(module, localModuleType, tucube_IDummy_service)

#endif
