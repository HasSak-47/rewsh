#include <plugin/definitions.h>
#include <stdio.h>

int plugin_setup(lua_State* L) {
    printf("hello world!");
    return 0;
}
