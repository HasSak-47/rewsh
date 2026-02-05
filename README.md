# Luall
This is an interactive Linux shell implemented in C with embedded Lua support, designed to provide a flexible and extensible command-line interface.
It uses Lua as its scripting language, and provides a minimal API to interact with the OS and the interaction loop, which also serves as a built in commands.

The shell allows modifications to its behavior at runtime including prompts, command parsing, and more.

## Features
- Built-in shell commands.
- Lua integration for scripting and automation.
- Environment variable management.
- Configurable initialization scripts.
- Configurable at runtime

## Demonstrations
![luall](https://github.com/user-attachments/assets/14467c20-61e2-4202-a509-b09e3e581fdb)

## How to use
### dependencies
To build and run this project, you will need:
- **Lua** 5.4
- **gcc**
- **make**
- **Rust toolchain** (rustc + cargo)
- **cbindgen**
- **clang** (for Rust build dependencies)

### building
- Clone the project `git clone -b stable https://github.com/HasSak-47/cshell.git`
- to build all: `make all`
- to run: `make run` or `./luall` after building

## Shortcomings 
### UX
- No syntax highlighting
- There is no tab completion
- Ctrl+c doesn't skip to the next line
- Basic prompt editing; no multiline or advanced line editing
- History is minimal (no persistent history file)

### Utils
- No file globing *
- No job control (fg/bg)
- Pipes exist at the Lua API level, but there is no shell syntax for pipelines/redirection yet
- Limited built-ins and minimal error messages
- No completion for env vars, paths, or commands
- Testing is kinda shit (sparse and mostly manual)

## Technical Details
### Architecture
- Core shell loop and terminal handling live in C (see `src/main.c`).
- Lua runs as an embedded scripting runtime; the shell drives a single `lua_State` and exposes a `Luall` table for API + state.
- CLI argument parsing is implemented in Rust and exported as a C ABI (`src/lib.rs`) and linked into the C binary.

### Lua initialization and config
- `init.lua` is the required blueprint loaded during `lua_setup`.
- `config.lua` is the user configuration file (loaded from the configured `CONFIG_PATH`).
- Runtime values like cwd/user/host/error/debug are pushed into `Luall.vars` each prompt cycle.

### Hot-loaded modules
- Files in `hot/` are compiled into shared objects under `units/` and linked into `units/bundle.so`.
- The shell `dlopen`s `bundle.so` and resolves `handle_input`, `lua_setup`, and `lua_cleanup`.
- A reload rebuilds and re-loads the bundle when the `reload` flag is set.

### Build pipeline
- `make all` builds C objects, the Rust `cdylib` (`libcshell.so`), and generates `include/bindgen.h` via `cbindgen`.
- The Rust library is copied into `build/` and linked into the final `luall` binary.

### Foot guns
Luall is the bridge between the interaction loop and the configuration, and there are not many guards to guaranty it's integrity and structure and the user has full control over it changes to it may crash the whole shell :D

More ways to shoot yourself in the foot:
- You can override core functions/tables in Lua at runtime and the C side will happily call into the void.
- Hot reload recompiles and re-loads the bundle; stale state, missing symbols, or bad pointers = instant sadness.
- `init.lua` is assumed to exist and be valid; syntax errors will stop the shell from starting.
- Raw terminal mode is enabled; if the process crashes you might need to `reset` your terminal.
- The API is thin and fast, not safe; invalid userdata/types can crash or corrupt state.
