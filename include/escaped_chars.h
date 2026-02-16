#ifndef __STRING_H__
#define __STRING_H__

#include <stdbool.h>
#include "utils.h"

enum CharacterType {
    ESCAPED_CHARACTER,
    ESCAPED_UNI,
    ESCAPED_HEX,
    ESCAPED_OCT,
    NORMAL_CHARACTER,
};

struct Character {
    enum CharacterType ty;
    int data;
};

char to_char(const struct Character c);

DefineVector(EscapedChars, struct Character);

struct EscapedChars read_n_escaped_chars(const char* chrs, const size_t len);
struct EscapedChars read_n_escaped_chars_deter(
    const char* chrs, char deter, const size_t len);
char* escaped_chars_to_cstring(const struct EscapedChars chars);
bool escaped_chars_cmp(struct EscapedChars c, const char* str);

struct EscapedChars escaped_chars_sub(
    struct EscapedChars v, const size_t beg, const size_t end);

#endif // !__STRING_H__
