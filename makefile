SRC_DIR := src
UNI_DIR := hot

OBJ_DIR := build
SHR_DIR := units

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
SHLIB_EXT := dylib
else
SHLIB_EXT := so
endif

RUST_TARGET ?=
CC ?= cc

LUA_CFLAGS := $(shell pkg-config --cflags lua5.4 2>/dev/null || pkg-config --cflags lua-5.4)
LUA_LIBS := $(shell pkg-config --libs lua5.4 2>/dev/null || pkg-config --libs lua-5.4)

ifeq ($(strip $(RUST_TARGET)),)
RUST_RELEASE_DIR := target/release
else
RUST_RELEASE_DIR := target/$(RUST_TARGET)/release
endif

SRCS := $(wildcard $(SRC_DIR)/*.c)
UNTS := $(wildcard $(UNI_DIR)/*.c)

OUT_RUST_LIB := $(OBJ_DIR)/libcshell.$(SHLIB_EXT)
SRC_RUST_LIB := $(RUST_RELEASE_DIR)/libcshell.$(SHLIB_EXT)

OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o,$(SRCS)) $(OUT_RUST_LIB)
SHRD := $(patsubst $(UNI_DIR)/%.c, $(SHR_DIR)/%.$(SHLIB_EXT),$(UNTS))

OUT := luall
BUNDLE := $(SHR_DIR)/bundle.$(SHLIB_EXT)

CFLAGS := -g -I include -Wall $(LUA_CFLAGS) -DBUNDLE_EXT=\"$(SHLIB_EXT)\"
OBJ_CFLAGS := $(CFLAGS) -fPIC -c
UFLAGS := $(CFLAGS) -fPIC -shared

LDFLAGS := -o $(OUT) -export-dynamic $(LUA_LIBS)

all: shell hot

shell: $(OUT)

$(OUT): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS)

$(OUT_RUST_LIB): Cargo.toml
	cargo build --release $(if $(RUST_TARGET),--target $(RUST_TARGET))
	cp $(SRC_RUST_LIB) $(OUT_RUST_LIB)
	cbindgen -c ./cbindgen.toml --output include/bindgen.h

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(OBJ_CFLAGS) $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

run : shell
	./$(OUT)

hot: $(BUNDLE)

$(BUNDLE): $(SHRD)
	$(CC) -fPIC -shared $(SHRD) -o $(BUNDLE)


$(SHR_DIR)/%.$(SHLIB_EXT): $(UNI_DIR)/%.c | $(SHR_DIR)
	$(CC) $(UFLAGS) $< -o $@

$(SHR_DIR):
	mkdir -p $(SHR_DIR)

clean:
	rm -f $(OBJS)

clean_units:
	rm -f $(BUNDLE)
	rm -f $(SHRD)

clean_all: clean clean_units

valgrind: shell
	valgrind ./$(OUT)


.PHONY: all clean cmds source $(OUT_RUST_LIB)
