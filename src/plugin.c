#include <stdio.h>
#include <stdlib.h>

#include <debug.h>
#include <ly_string.h>
#include <path.h>
#include <plugin/handler.h>
#include <state.h>

void add_hook(enum Event event, Actor actor) {
    struct PluginHandler* handler = &state.plugins;
    struct Hook hook              = {event, actor};

    vector_push(handler->hooks, hook)
}

void load_plugin(struct Path plugin_path) {
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

    char* in_path_str       = get_path_string(plugin_path);
    struct Path object_path = parse_path(CACHE_PATH);
    struct String name      = get_name(&plugin_path);
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
