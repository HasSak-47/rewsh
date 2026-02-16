
#ifndef __PLUGIN_API_H__
#define __PLUGIN_API_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <lua.h>
#include "definitions.h"

typedef int (*Actor)(lua_State* L);

struct Plugin self;
void add_hook(enum Event event, Actor);
void load();
void unload();

#ifdef __cplusplus
}
#endif

#endif
