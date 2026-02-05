
#include <lauxlib.h>
#include <lua.h>

#include <debug.h>
#include <path.h>
#include <state.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pwd.h>
#include <unistd.h>

/**
 * sets the state of the shell
 */
void init_shell_state() {
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

    state.config.init    = parse_path(INIT_PATH);
    state.config.plugins = parse_path(HOT_PATH);
    state.config.config  = parse_path(INIT_PATH);

    expand_path(&state.config.init, &state.vars.cwd);
    expand_path(&state.config.plugins, &state.vars.cwd);
    expand_path(&state.config.config, &state.vars.cwd);
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

    debug_printf("deleting cwd\n");
    destruct_path(&state.vars.cwd);
    debug_printf("deleting home\n");
    destruct_path(&state.vars.user.home);
    debug_printf("deleting init\n");
    destruct_path(&state.config.init);
    debug_printf("deleting plugins\n");
    destruct_path(&state.config.plugins);
    debug_printf("deleting config\n");
    destruct_path(&state.config.config);
}
