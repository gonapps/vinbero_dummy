#include <dlfcn.h>
#include <err.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <vinbero/vinbero_Module.h>
#include <vinbero/vinbero_IModule.h>
#include <vinbero/vinbero_IBasic.h>
#include <libgenc/genc_Tree.h>
#include "vinbero_IDummy.h"

struct vinbero_dummy_Interface {
    VINBERO_IDUMMY_FUNCTION_POINTERS;
};

struct vinbero_dummy_LocalModule {
    const char* message;
    int interval;
};

VINBERO_IMODULE_FUNCTIONS;
VINBERO_IBASIC_FUNCTIONS;
VINBERO_IDUMMY_FUNCTIONS;

int vinbero_IModule_init(struct vinbero_Module* module, struct vinbero_Config* config, void* args[]) {
    warnx("%s: %u: %s", __FILE__, __LINE__, __FUNCTION__);
    module->name = "vinbero_Dummy";
    module->version = "0.0.1";
    module->localModule.pointer = malloc(1 * sizeof(struct vinbero_dummy_LocalModule));

    struct vinbero_dummy_LocalModule* localModule = module->localModule.pointer;
    VINBERO_CONFIG_GET(config, module, "vinbero_dummy.message", string, &(localModule->message), "I HAVE NOTHING TO SAY");
    VINBERO_CONFIG_GET(config, module, "vinbero_dummy.interval", integer, &(localModule->interval), 1);

    struct vinbero_Module_Ids childModuleIds;
    GENC_ARRAY_LIST_INIT(&childModuleIds);
    VINBERO_CONFIG_GET_CHILD_MODULE_IDS(config, module->id, &childModuleIds);
    GENC_TREE_NODE_FOR_EACH_CHILD(module, index) {
        struct vinbero_Module* childModule = &GENC_TREE_NODE_GET_CHILD(module, index);
        childModule->interface = malloc(1 * sizeof(struct vinbero_dummy_Interface));
        struct vinbero_dummy_Interface* moduleInterface = childModule->interface;
        if((moduleInterface->vinbero_IDummy_service = dlsym(childModule->dlHandle, "vinbero_IDummy_service")) == NULL) {
            warnx("%s: %u: Unable to find vinbero_IDummy_service()", __FILE__, __LINE__);
            return -1;
        }
    }
    return 0;
}

int vinbero_IBasic_service(struct vinbero_Module* module, void* args[]) {
    struct vinbero_dummy_LocalModule* localModule = module->localModule.pointer;
    struct vinbero_Module* parentModule = GENC_TREE_NODE_GET_PARENT(module);
    while(true) {
        warnx("Module message: %s", localModule->message);
        warnx("ID of my parent module is %s", parentModule->id); 
        GENC_TREE_NODE_FOR_EACH_CHILD(module, index) {
            struct vinbero_Module* childModule = &GENC_TREE_NODE_GET_CHILD(module, index);
            struct vinbero_dummy_Interface* moduleInterface = childModule->interface;
            moduleInterface->vinbero_IDummy_service(childModule);
        }
        sleep(localModule->interval);
    }
    return 0;
}

int vinbero_IDummy_service(struct vinbero_Module* module) {
    struct vinbero_dummy_LocalModule* localModule = module->localModule.pointer;
    struct vinbero_Module* parentModule = GENC_TREE_NODE_GET_PARENT(module);
    warnx("Module message: %s", localModule->message);
    warnx("ID of my parent module is %s", parentModule->id); 
    GENC_TREE_NODE_FOR_EACH_CHILD(module, index) {
        struct vinbero_Module* childModule = &GENC_TREE_NODE_GET_CHILD(module, index);
        struct vinbero_dummy_Interface* moduleInterface = childModule->interface;
        moduleInterface->vinbero_IDummy_service(childModule);
    }
    
    return 0;
}

int vinbero_IModule_destroy(struct vinbero_Module* module) {
struct vinbero_dummy_LocalModule* localModule = module->localModule.pointer;
    warnx("%s: %u: %s", __FILE__, __LINE__, __FUNCTION__);
    GENC_TREE_NODE_FOR_EACH_CHILD(module, index) {
        struct vinbero_Module* childModule = &GENC_TREE_NODE_GET_CHILD(module, index);
        free(childModule->interface);
    }
    free(module->localModule.pointer);
    if(module->tlModuleKey != NULL)
        free(module->tlModuleKey);
//    dlclose(module->dl_handle);
    return 0;
}
