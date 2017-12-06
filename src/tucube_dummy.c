#include <dlfcn.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <tucube/tucube_Module.h>
#include <tucube/tucube_IModule.h>
#include <tucube/tucube_ICore.h>
#include <libgenc/genc_cast.h>
#include <libgenc/genc_Tree.h>

struct tucube_dummy_Interface {
    TUCUBE_ICORE_FUNCTION_POINTERS;
};

struct tucube_dummy_LocalModule {
    const char* message;
    int interval;
    int requiresNext;
};

TUCUBE_IMODULE_FUNCTIONS;
TUCUBE_ICORE_FUNCTIONS;

int tucube_IModule_init(struct tucube_Module* module, struct tucube_Config* config, void* args[]) {
    warnx("%s: %u: %s", __FILE__, __LINE__, __FUNCTION__);
    module->localModule.pointer = malloc(1 * sizeof(struct tucube_dummy_LocalModule));

    struct tucube_dummy_LocalModule* localModule = module->localModule.pointer;
    TUCUBE_CONFIG_GET(config, module->id, "tucube_dummy.message", string, &(localModule->message), "I HAVE NOTHING TO SAY");
    TUCUBE_CONFIG_GET(config, module->id, "tucube_dummy.interval", integer, &(localModule->interval), 1);
    TUCUBE_CONFIG_GET(config, module->id, "tucube_dummy.requiresNext", integer, &(localModule->requiresNext), 0);

    struct tucube_Module_Ids childModuleIds;
    GENC_ARRAY_LIST_INIT(&childModuleIds);
    TUCUBE_CONFIG_GET_CHILD_MODULE_IDS(config, module->id, &childModuleIds);
    if(localModule->requiresNext && GENC_ARRAY_LIST_SIZE(&childModuleIds) == 0)
        errx(EXIT_FAILURE, "%s: %u: tucube_dummy requires at least one next module", __FILE__, __LINE__);
    return 0;
}

int tucube_ICore_service(struct tucube_Module* module, void* args[]) {
    struct tucube_dummy_LocalModule* localModule = module->localModule.pointer;
    struct tucube_Module* parentModule = GENC_TREE_NODE_GET_PARENT(module);
    while(true) {
        warnx("Module message: %s", localModule->message);
        warnx("ID of my parent module is %s", parentModule->id); 
        sleep(localModule->interval);
    }
    return 0;
}

int tucube_IModule_destroy(struct tucube_Module* module) {
struct tucube_dummy_TlModule* localModule = module->localModule.pointer;
    warnx("%s: %u: %s", __FILE__, __LINE__, __FUNCTION__);
//    dlclose(module->dl_handle);
    free(module->tlModuleKey);
    free(module->localModule.pointer);
    free(module);
    return 0;
}
