
#include <lauxlib.h>
#include <lua.h>

#include <debug.h>
#include <path.h>
#include <state.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pwd.h>
#include <sys/wait.h>
#include <unistd.h>
#include "ly_string.h"
#include "utils.h"

struct ShellState state = {};

/**
 * sets the state of the shell
 */
void init_shell_variables() {
    state.running   = true;
    state.vars.host = malloc(256);
    gethostname(state.vars.host, 256);

    uid_t uid = getuid();
    // no getpwuid_r cuz it is ez and I (hopefully) just need a single thread
    struct passwd* p     = getpwuid(uid);
    state.vars.user.name = strdup(p->pw_name);
    state.vars.user.home = parse_path(p->pw_dir);

    const char* _cwd = getenv("PWD");
    state.vars.cwd   = parse_path(_cwd);
}

void init_shell_config() {
    state.config.init    = parse_path(INIT_PATH);
    state.config.plugins = parse_path(PLUGIN_PATH);
    state.config.config  = parse_path(CONFIG_PATH);

    expand_path(&state.config.init, &state.vars.cwd);
    expand_path(&state.config.plugins, &state.vars.cwd);
    expand_path(&state.config.config, &state.vars.cwd);
}

/**
 * sets the state of the shell
 */
void init_shell_state() {
    state.L = luaL_newstate();

    init_shell_variables();
    init_shell_config();

    struct VectorPath plugins_paths = get_childs(&state.config.plugins);

    // NOTE: upgrade to just link a c compiler into the shell instead of just
    // using system lmao
    for (size_t i = 0; i < plugins_paths.len; ++i) {
    }
}

/**
 * updates state where it is needed
 */
void get_current_state() {}

/**
 * cleanins the shell state
 */
void end_shell_state() {
    free(state.vars.host);

    free(state.vars.user.name);

    destruct_path(&state.vars.cwd);
    destruct_path(&state.vars.user.home);
    destruct_path(&state.config.init);
    destruct_path(&state.config.plugins);
    destruct_path(&state.config.config);

    lua_close(state.L);
}
