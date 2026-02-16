#ifndef __STATE_H__
#define __STATE_H__

#include "utils.h"

#ifndef INIT_PATH
#define INIT_PATH "./init.lua"
#endif

#ifndef PLUGIN_PATH
#define PLUGIN_PATH "./plugins"
#endif

#ifndef CACHE_PATH
#define CACHE_PATH "./.ignore/cache"
#endif

#ifndef CONFIG_PATH
#define CONFIG_PATH "./config.lua"
#endif
#include <lua.h>
#include <stdbool.h>

#include "path.h"
#include "plugin/handler.h"

// Luall.vars
struct User {
    char* name;
    struct Path home;
};

struct Vars {
    struct User user;
    struct Path cwd;
    char* host;
    int error;
    bool debug;
};

struct Config {
    struct Path config;
    struct Path init;
    struct Path plugins;
};

struct ShellState {
    struct Vars vars;
    struct Config config;
    struct VectorPluginHandler plugins;
    bool running;
    bool reload;
    lua_State* L;
};

extern struct ShellState state;

void init_shell_state();
void end_shell_state();

void get_current_state();
void update_current_state();

void set_raw_mode();
void unset_raw_mode();

#endif
