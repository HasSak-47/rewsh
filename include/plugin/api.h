
#ifndef __PLUGIN_API_H__
#define __PLUGIN_API_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <lua.h>
#include "definitions.h"

struct Plugin self;
void load();
void unload();

#ifdef __cplusplus
}
#endif

#endif
