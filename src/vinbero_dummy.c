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
#include <vinbero/vinbero_Module.h>
#include <vinbero/vinbero_IModule.h>
#include <vinbero/vinbero_ITlService.h>
#include <libgenc/genc_cast.h>
#include <libgenc/genc_Tree.h>
#include <gaio.h>

struct vinbero_dummy_Interface {
    VINBERO_ITLSERVICE_FUNCTION_POINTERS;
};

struct vinbero_dummy_LocalModule {
    const char* id;
    int interval;
    int requiresNext;
};

struct vinbero_dummy_TlModule {
    int integer;
};

VINBERO_IMODULE_FUNCTIONS;
VINBERO_ITLSERVICE_FUNCTIONS;

int vinbero_IModule_init(struct vinbero_Module* module, struct vinbero_Config* config, void* args[]) {
    warnx("%s: %u: %s", __FILE__, __LINE__, __FUNCTION__);
    module->localModule.pointer = malloc(1 * sizeof(struct vinbero_dummy_LocalModule));
    module->tlModuleKey = malloc(1 * sizeof(pthread_key_t));
    pthread_key_create(module->tlModuleKey, NULL);

    struct vinbero_dummy_LocalModule* localModule = module->localModule.pointer;
    VINBERO_CONFIG_GET(config, module->id, "vinbero_dummy.id", string, &(localModule->id), "ANONYMOUS");
    VINBERO_CONFIG_GET(config, module->id, "vinbero_dummy.interval", integer, &(localModule->interval), 1);
    VINBERO_CONFIG_GET(config, module->id, "vinbero_dummy.requiresNext", integer, &(localModule->requiresNext), 0);
/*
    struct vinbero_Module_Ids childModuleIds;
    GENC_ARRAY_LIST_INIT(&childModuleIds);
    VINBERO_MODULE_GET_CHILD_MODULE_IDS(config, module->id, &childModuleIds);
    if(GENC_ARRAY_LIST_SIZE(&childModuleIds) == 0)
        errx(EXIT_FAILURE, "%s: %u: vinbero_dummy requires next modules", __FILE__, __LINE__);
*/
    return 0;
}

int vinbero_IModule_tlInit(struct vinbero_Module* module, struct vinbero_Config* config, void* args[]) {
    warnx("%s: %u: %s", __FILE__, __LINE__, __FUNCTION__);
    struct vinbero_dummy_LocalModule* localModule = module->localModule.pointer;
    struct vinbero_dummy_TlModule* tlModule = malloc(1 * sizeof(struct vinbero_dummy_TlModule));
    pthread_setspecific(*module->tlModuleKey, tlModule);
    return 0;
}

int vinbero_ITlService_call(struct vinbero_Module* module, void* args[]) {
    struct vinbero_dummy_LocalModule* localModule = module->localModule.pointer;
    struct vinbero_dummy_TlModule* tlModule = pthread_getspecific(*module->tlModuleKey);    
    while(true) {
        warnx("ID: %s", localModule->id);
        sleep(localModule->interval);
    }
    return 0;
}

int vinbero_IModule_tlDestroy(struct vinbero_Module* module) {
struct vinbero_dummy_TlModule* localModule = module->localModule.pointer;
    warnx("%s: %u: %s", __FILE__, __LINE__, __FUNCTION__);
    struct vinbero_dummy_TlModule* tlModule = pthread_getspecific(*module->tlModuleKey);
    if(tlModule != NULL)
        free(tlModule);
    return 0;
}

int vinbero_IModule_destroy(struct vinbero_Module* module) {
struct vinbero_dummy_TlModule* localModule = module->localModule.pointer;
    warnx("%s: %u: %s", __FILE__, __LINE__, __FUNCTION__);
//    dlclose(module->dl_handle);
    pthread_key_delete(*module->tlModuleKey);
    free(module->tlModuleKey);
    free(module->localModule.pointer);
    free(module);
    return 0;
}
