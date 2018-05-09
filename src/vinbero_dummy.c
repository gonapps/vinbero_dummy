#include <dlfcn.h>
#include <err.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <vinbero_log.h>
#include <vinbero/vinbero_Module.h>
#include <vinbero/vinbero_IModule.h>
#include <vinbero/vinbero_IBasic.h>
#include <libgenc/genc_Tree.h>
#include "vinbero_IDummy.h"

struct vinbero_dummy_LocalModule {
    const char* message;
    int interval;
};

VINBERO_IMODULE_FUNCTIONS;
VINBERO_IBASIC_FUNCTIONS;
VINBERO_IDUMMY_FUNCTIONS;

int vinbero_IModule_init(struct vinbero_Module* module, struct vinbero_Config* config, void* args[]) {
    VINBERO_LOG_TRACE("in vinbero_IModule_init(...)");
    module->name = "vinbero_Dummy";
    module->version = "0.0.1";
    module->localModule.pointer = malloc(1 * sizeof(struct vinbero_dummy_LocalModule));

    struct vinbero_dummy_LocalModule* localModule = module->localModule.pointer;
    VINBERO_CONFIG_GET(config, module, "vinbero_dummy.message", string, &(localModule->message), "I HAVE NOTHING TO SAY");
    VINBERO_CONFIG_GET(config, module, "vinbero_dummy.interval", integer, &(localModule->interval), 1);

    return 0;
}

int vinbero_IModule_rInit(struct vinbero_Module* module, struct vinbero_Config* config, void* args[]) {
    VINBERO_LOG_TRACE("in vinbero_IModule_rInit(...)");
    return 0;
}

int vinbero_IBasic_service(struct vinbero_Module* module, void* args[]) {
    struct vinbero_dummy_LocalModule* localModule = module->localModule.pointer;
    struct vinbero_Module* parentModule = GENC_TREE_NODE_GET_PARENT(module);
    while(true) {
        VINBERO_LOG_INFO("Module message: %s", localModule->message);
        VINBERO_LOG_INFO("ID of my parent module is %s", parentModule->id); 
        GENC_TREE_NODE_FOR_EACH_CHILD(module, index) {
            struct vinbero_Module* childModule = &GENC_TREE_NODE_GET_CHILD(module, index);
            struct vinbero_IDummy_Interface interface;
            int errorVariable;
            VINBERO_IDUMMY_DLSYM(&interface, &childModule->dlHandle, &errorVariable);
            if(errorVariable == 1)
                return -1;
            interface.vinbero_IDummy_service(childModule);
        }
        sleep(localModule->interval);
    }
    return 0;
}

int vinbero_IDummy_service(struct vinbero_Module* module) {
    struct vinbero_dummy_LocalModule* localModule = module->localModule.pointer;
    struct vinbero_Module* parentModule = GENC_TREE_NODE_GET_PARENT(module);
    VINBERO_LOG_INFO("Module message: %s", localModule->message);
    VINBERO_LOG_INFO("ID of my parent module is %s", parentModule->id); 
    GENC_TREE_NODE_FOR_EACH_CHILD(module, index) {
        struct vinbero_Module* childModule = &GENC_TREE_NODE_GET_CHILD(module, index);
        struct vinbero_IDummy_Interface interface;
        int errorVariable;
        VINBERO_IDUMMY_DLSYM(&interface, &childModule->dlHandle, &errorVariable);
        interface.vinbero_IDummy_service(childModule);
    }
    return 0;
}

int vinbero_IModule_destroy(struct vinbero_Module* module) {
    VINBERO_LOG_TRACE("in vinbero_IModule_destroy(...)");
    return 0;
}

int vinbero_IModule_rDestroy(struct vinbero_Module* module) {
    VINBERO_LOG_TRACE("in vinbero_IModule_rDestroy(...)");
    struct vinbero_dummy_LocalModule* localModule = module->localModule.pointer;
    free(module->localModule.pointer);
    if(module->tlModuleKey != NULL)
        free(module->tlModuleKey);
//    dlclose(module->dl_handle);
    return 0;
}
