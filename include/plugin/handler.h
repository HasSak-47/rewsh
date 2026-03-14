#ifndef __PLUGIN_HANDLER_HPP__
#define __PLUGIN_HANDLER_HPP__

#ifndef PLUGIN_FOLDER
#define PLUGIN_FOLDER "./plugins"
#endif

#include "../path.h"
#include "./definitions.h"

DefineVector(VectorPlugin, struct Plugin);

struct PluginSymbol {
    void* sym;
    struct Plugin plugin;
};

struct Hook {
    enum Event event;
    Actor actor;
};

DefineVector(VectorPluginSymbol, struct Plugin);
DefineVector(VectorHook, struct Hook);

struct PluginHandler {
    struct VectorHook hooks;
    struct PluginSymbol symbol;
};

void load_plugin(struct Path plugin_path);

#endif
