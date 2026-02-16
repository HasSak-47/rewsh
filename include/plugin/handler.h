#ifndef __PLUGIN_HANDLER_HPP__
#define __PLUGIN_HANDLER_HPP__

#ifndef PLUGIN_FOLDER
#define PLUGIN_FOLDER "./plugins"
#endif

#include "../ly_string.h"
#include "./definitions.h"

DefineVector(VectorPlugin, struct Plugin);

struct PluginHandler {
    void* sym;
    struct Plugin plugin;
};

DefineVector(VectorPluginHandler, struct Plugin);

void load_plugins();

#endif
