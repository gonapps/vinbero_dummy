#include <dlfcn.h>
#include <err.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <vinbero_common/vinbero_common_Call.h>
#include <vinbero_common/vinbero_common_Config.h>
#include <vinbero_common/vinbero_common_Log.h>
#include <vinbero_common/vinbero_common_Module.h>
#include <vinbero/vinbero_Interface_MODULE.h>
#include <vinbero/vinbero_Interface_BASIC.h>
#include <libgenc/genc_Tree.h>
#include "vinbero_Interface_DUMMY.h"

struct vinbero_dummy_LocalModule {
    const char* message;
    int interval;
};

VINBERO_INTERFACE_MODULE_FUNCTIONS;
VINBERO_INTERFACE_BASIC_FUNCTIONS;
VINBERO_INTERFACE_DUMMY_FUNCTIONS;

int vinbero_Interface_MODULE_init(struct vinbero_common_Module* module, struct vinbero_common_Config* config, void* args[]) {
    VINBERO_COMMON_LOG_TRACE2();
    module->name = "vinbero_Dummy";
    module->version = "0.0.1";
    module->childrenRequired = false;
    module->localModule.pointer = malloc(1 * sizeof(struct vinbero_dummy_LocalModule));

    struct vinbero_dummy_LocalModule* localModule = module->localModule.pointer;
    vinbero_common_Config_getString(config, module, "vinbero_dummy.message", &(localModule->message), "I HAVE NOTHING TO SAY");
    vinbero_common_Config_getInt(config, module, "vinbero_dummy.interval", &(localModule->interval), 1);

    return 0;
}

int vinbero_Interface_MODULE_rInit(struct vinbero_common_Module* module, struct vinbero_common_Config* config, void* args[]) {
    VINBERO_COMMON_LOG_TRACE2();
    return 0;
}

int vinbero_Interface_BASIC_service(struct vinbero_common_Module* module, void* args[]) {
    VINBERO_COMMON_LOG_TRACE2();
    int ret;
    struct vinbero_dummy_LocalModule* localModule = module->localModule.pointer;
    struct vinbero_common_Module* parentModule = GENC_TREE_NODE_GET_PARENT(module);
    while(true) {
        VINBERO_COMMON_LOG_DEBUG("Module message: %s", localModule->message);
        VINBERO_COMMON_LOG_DEBUG("ID of my parent module is %s", parentModule->id); 
        GENC_TREE_NODE_FOR_EACH_CHILD(module, index) {
            struct vinbero_common_Module* childModule = &GENC_TREE_NODE_GET_CHILD(module, index);
            VINBERO_COMMON_CALL(DUMMY, service, childModule, &ret, childModule);
            if(ret < 0)
                return ret;
        }
        sleep(localModule->interval);
    }
    return 0;
}

int vinbero_Interface_DUMMY_service(struct vinbero_common_Module* module) {
    VINBERO_COMMON_LOG_TRACE2();
    int ret;
    struct vinbero_dummy_LocalModule* localModule = module->localModule.pointer;
    struct vinbero_common_Module* parentModule = GENC_TREE_NODE_GET_PARENT(module);
    VINBERO_COMMON_LOG_DEBUG("Module message: %s", localModule->message);
    VINBERO_COMMON_LOG_DEBUG("ID of my parent module is %s", parentModule->id); 
    GENC_TREE_NODE_FOR_EACH_CHILD(module, index) {
        struct vinbero_common_Module* childModule = &GENC_TREE_NODE_GET_CHILD(module, index);
        VINBERO_COMMON_CALL(DUMMY, service, childModule, &ret, childModule);
        if(ret < 0)
            return ret;
    }
    return 0;
}

int vinbero_Interface_MODULE_destroy(struct vinbero_common_Module* module) {
    VINBERO_COMMON_LOG_TRACE2();
    return 0;
}

int vinbero_Interface_MODULE_rDestroy(struct vinbero_common_Module* module) {
    VINBERO_COMMON_LOG_TRACE2();
    struct vinbero_dummy_LocalModule* localModule = module->localModule.pointer;
    free(module->localModule.pointer);
    if(module->tlModuleKey != NULL)
        free(module->tlModuleKey);
//    dlclose(module->dl_handle);
    return 0;
}
