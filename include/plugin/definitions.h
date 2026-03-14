#ifndef __PLUGIN_DEFINITIONS__
#define __PLUGIN_DEFINITIONS__

#include <lua.h>

#ifdef __cplusplus
extern "C" {
#endif

struct TargetVersion {
    unsigned major;
    unsigned minor;
    unsigned patch;
};

struct Plugin {
    struct TargetVersion versions;
    char* name;
};

enum Event {
    ON_INPUT_START,
    ON_INPUT_ADD,
    ON_INPUT_END,
    ON_VARIABLE_RELOAD,
};

typedef int (*Actor)(lua_State* L);
void add_hook(enum Event event, Actor);

#ifdef __cplusplus
}
#endif

#endif
