
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
        struct String cc   = string_from_cstr("gcc");
        const char* ARGS[] = {
            "-g",
            "-shared",
            "-I",
            "include",
            "-fPIC",
            "-Wall",
            "-Werror",
        };

        char* in_path_str       = get_path_string(plugins_paths.data[i]);
        struct Path object_path = parse_path(CACHE_PATH);
        struct String name      = get_name(&plugins_paths.data[i]);
        if (name.data == NULL) {
            printf("malformed path %s\n", in_path_str);
            exit(-1);
        }
        push_name_string(&object_path, name);

        // WARN: leaky interface
        struct String* inner =
            &object_path._inner.data[object_path._inner.len - 1].name;
        inner->data[inner->len - 1] = 'o';

        char* out_path_str = get_path_string(object_path);

        for (size_t i = 0; i < (sizeof(ARGS) / sizeof(ARGS[i])); ++i) {
            vector_push(cc, ' ');
            string_concat_cstr(&cc, ARGS[i]);
        }

        vector_push(cc, ' ');
        string_concat_cstr(&cc, in_path_str);
        vector_push(cc, ' ');
        string_concat_cstr(&cc, "-o");
        vector_push(cc, ' ');
        string_concat_cstr(&cc, out_path_str);

        char* cc_str = string_to_cstring(cc);
        // printf("running: %s\n", cc_str);

        int err_code = system(cc_str);

        if (err_code != 0) {
            printf("failed to load plugin: %s\n", in_path_str);
            exit(-1);
        }

        debug_printf("loaded plugin %s\n", in_path_str);

        free(cc_str);
        free(in_path_str);
        free(out_path_str);
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
