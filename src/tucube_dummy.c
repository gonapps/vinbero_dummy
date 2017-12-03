#include <dlfcn.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <tucube/tucube_Module.h>
#include <tucube/tucube_IModule.h>
#include <tucube/tucube_ITlService.h>
#include <libgenc/genc_cast.h>
#include <libgenc/genc_Tree.h>
#include <gaio.h>

struct tucube_dummy_Interface {
    TUCUBE_ITLSERVICE_FUNCTION_POINTERS;
};

struct tucube_dummy_LocalModule {
    const char* id;
    int interval;
    int requiresNext;
};

struct tucube_dummy_TlModule {
    int integer;
};

TUCUBE_IMODULE_FUNCTIONS;
TUCUBE_ITLSERVICE_FUNCTIONS;

int tucube_IModule_init(struct tucube_Module* module, struct tucube_Config* config, void* args[]) {
    warnx("%s: %u: %s", __FILE__, __LINE__, __FUNCTION__);
    module->localModule.pointer = malloc(1 * sizeof(struct tucube_dummy_LocalModule));
    module->tlModuleKey = malloc(1 * sizeof(pthread_key_t));
    pthread_key_create(module->tlModuleKey, NULL);

    struct tucube_dummy_LocalModule* localModule = module->localModule.pointer;
    TUCUBE_CONFIG_GET(config, module->name, "tucube_dummy.id", string, &(localModule->id), "ANONYMOUS");
    TUCUBE_CONFIG_GET(config, module->name, "tucube_dummy.interval", integer, &(localModule->interval), 1);
    TUCUBE_CONFIG_GET(config, module->name, "tucube_dummy.requiresNext", integer, &(localModule->requiresNext), 0);
/*
    struct tucube_Module_Names childModuleNames;
    GENC_ARRAY_LIST_INIT(&childModuleNames);
    TUCUBE_MODULE_GET_CHILD_MODULE_NAMES(config, module->name, &childModuleNames);
    if(GENC_ARRAY_LIST_SIZE(&childModuleNames) == 0)
        errx(EXIT_FAILURE, "%s: %u: tucube_dummy requires next modules", __FILE__, __LINE__);
*/
    return 0;
}

int tucube_IModule_tlInit(struct tucube_Module* module, struct tucube_Config* config, void* args[]) {
    warnx("%s: %u: %s", __FILE__, __LINE__, __FUNCTION__);
    struct tucube_dummy_LocalModule* localModule = module->localModule.pointer;
    struct tucube_dummy_TlModule* tlModule = malloc(1 * sizeof(struct tucube_dummy_TlModule));
    pthread_setspecific(*module->tlModuleKey, tlModule);
    return 0;
}

int tucube_ITlService_call(struct tucube_Module* module, void* args[]) {
    struct tucube_dummy_LocalModule* localModule = module->localModule.pointer;
    struct tucube_dummy_TlModule* tlModule = pthread_getspecific(*module->tlModuleKey);    
    while(true) {
        warnx("ID: %s", localModule->id);
        sleep(localModule->interval);
    }
    return 0;
}

int tucube_IModule_tlDestroy(struct tucube_Module* module) {
struct tucube_dummy_TlModule* localModule = module->localModule.pointer;
    warnx("%s: %u: %s", __FILE__, __LINE__, __FUNCTION__);
    struct tucube_dummy_TlModule* tlModule = pthread_getspecific(*module->tlModuleKey);
    if(tlModule != NULL)
        free(tlModule);
    return 0;
}

int tucube_IModule_destroy(struct tucube_Module* module) {
struct tucube_dummy_TlModule* localModule = module->localModule.pointer;
    warnx("%s: %u: %s", __FILE__, __LINE__, __FUNCTION__);
//    dlclose(module->dl_handle);
    pthread_key_delete(*module->tlModuleKey);
    free(module->tlModuleKey);
    free(module->localModule.pointer);
    free(module);
    return 0;
}
