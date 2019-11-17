# OpenPhySyn Hello Transform

A simple template for building Physical Synthesis transforms for [OpenPhySyn](https://github.com/The-OpenROAD-Project/OpenPhySyn)

## Building

Build by making a build directory (i.e. `build/`), run `cmake ..` in that directory, and then use `make` to build the desired target.

Example:

```bash
> mkdir build && cd build
> export PSN_HOME_PATH=<OpenPhySyn Source Code Path>
> export PSN_LIB_PATH=<OpenPhySyn Built Library Directory>
> export OPENDB_HOME_PATH=<OpenDB Source Code Directory>
> export OPENDB_LIB_PATH=<OpenDB Built Library Directory>
> export OPENDB_STA_PATH=<OpenSTA Source Code Directory>
> cmake ..
> make
> make install # Or sudo make install
```

## Usage

```bash
> ./Psn
> # Add random wire
> transform hello_transform net1
> write_def out.def
```

Make sure to set `PSN_HOME_PATH` to OpenPhySyn source code path, `PSN_LIB_PATH` to the directory containing the built OpenPhySyn library file, `OPENDB_HOME_PATH` to [OpenDB](https://github.com/The-OpenROAD-Project/OpenDB) include path, `OPENDB_LIB_PATH` the directory containing the built [OpenDB](https://github.com/The-OpenROAD-Project/OpenDB) library files.
