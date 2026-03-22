#ifndef __PLUGIN_DEFINITIONS__
#define __PLUGIN_DEFINITIONS__

#include <lua.h>

#ifdef __cplusplus
extern "C" {
#endif

enum Event {
    ON_INPUT_START,
    ON_INPUT_ADD,
    ON_INPUT_END,
    ON_VARIABLE_RELOAD,
};

int plugin_setup(lua_State* L);
int plugin_destruct(lua_State* L);

typedef int (*Actor)(lua_State* L);
void add_hook(enum Event event, Actor);

#ifdef __cplusplus
}
#endif

#endif
