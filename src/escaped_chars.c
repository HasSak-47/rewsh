#include "escaped_chars.h"
#include <stdlib.h>
#include <string.h>
#include "utils.h"

static void read_escape(
    struct EscapedChars* cs, const char* chrs, size_t i, const size_t len) {
    switch (chrs[i]) {
    // special cases
    case 'x': // hex
        return;
    case 'u': // unicode
        return;
    case '0': // octal
        return;
    default: {
        struct Character c = {.ty = ESCAPED_CHARACTER, .data = chrs[i]};
        vector_push((*cs), c);
    }
    }
}

struct EscapedChars read_n_escaped_chars(const char* chrs, const size_t len) {
    struct EscapedChars cs = {};

    for (size_t i = 0; i < len; ++i) {
        if (chrs[i] == '\\') {
            ++i;
            read_escape(&cs, chrs, i, len);
            continue;
        }
        struct Character c = {.ty = NORMAL_CHARACTER, .data = chrs[i]};
        vector_push(cs, c);
    }

    return cs;
}

char to_char(const struct Character c) {
    switch (c.ty) {
    case NORMAL_CHARACTER:
        return c.data;
    case ESCAPED_CHARACTER:
        switch (c.data) {
        case 'e':
            return '\e';
        default:
            return c.data;
        }
    default:
        return c.data;
        break;
    }
}

char* escaped_chars_to_cstring(const struct EscapedChars chars) {
    char* s = malloc((chars.len + 1) * sizeof(char));
    for (size_t i = 0; i < chars.len; ++i) {
        s[i] = to_char(chars.data[i]);
    }
    s[chars.len] = 0;

    return s;
}

struct EscapedChars escaped_chars_sub(
    struct EscapedChars v, const size_t beg, const size_t end) {
    struct EscapedChars name = {};
    for (size_t i = beg; i < end; ++i) {
        vector_push(name, v.data[i]);
    }

    return name;
}

bool escaped_chars_cmp(struct EscapedChars c, const char* str) {
    size_t len = strlen(str);
    size_t i   = 0;
    while (i < len && i < c.len) {
        if (str[i] != to_char(c.data[i]))
            return false;
        ++i;
    }

    return len == c.len;
}
