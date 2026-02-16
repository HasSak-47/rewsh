SRC_DIR := src

OBJ_DIR := .ignore/build

SRCS := $(wildcard $(SRC_DIR)/*.c)

OUT_RUST_LIB := $(OBJ_DIR)/libcshell.so 
SRC_RUST_LIB := target/release/libcshell.so

OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o,$(SRCS)) $(OUT_RUST_LIB)

OUT := luall

C := gcc
CFLAGS := -g -shared -I include -c -Wall -Werror

LDFLAGS := -o $(OUT) -export-dynamic -llua

build: $(OUT)

$(OUT): $(OBJS)
	$(C) $(OBJS) $(LDFLAGS)

$(OUT_RUST_LIB): Cargo.toml
	cargo build --release
	cp $(SRC_RUST_LIB) $(OUT_RUST_LIB)
	cbindgen -c ./cbindgen.toml --output include/bindgen.h

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(C) $(CFLAGS) $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

run: clean build
	./$(OUT)

test: CFLAGS += -DLY_TEST
test: clean build
	./$(OUT)

clean:
	rm -f $(OBJS)

clean_all: clean

valgrind: shell
	valgrind ./$(OUT)


.PHONY: build run test clean cmds source $(OUT_RUST_LIB)
