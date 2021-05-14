## DCPU16

This is an assembler and an emulator for the DCPU16, a fictional computer with accompanying hardware made for the cancelled game 0x10c.

I wrote this mostly as a means to get more experience with C++ and related tools.

### Building

Get the dependencies:

```sh
git submodule update --init --recursive
```

Generate the makefiles:

```sh
mkdir build
cd build
cmake ..
```

Build the project:

```sh
make
```

### Testing

Follow the build instructions and then run either `make test` or `./build/tests`.
