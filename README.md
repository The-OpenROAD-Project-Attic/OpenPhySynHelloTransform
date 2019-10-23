# PhyKnight Hello Transform

A simple template for building Physical Synthesis transforms for [PhyKnight](https://github.com/The-OpenROAD-Project/PhyKnight)

## Building

Build by making a build directory (i.e. `build/`), run `cmake ..` in that directory, and then use `make` to build the desired target.

Example:

```bash
> mkdir build && cd build
> export PHY_HOME_PATH=<PhyKnight Source Code Path>
> export PHY_LIB_PATH=<PhyKnight Built Library Directory>
> export OPENDB_HOME_PATH=<OpenDB Source Code Directory>
> export OPENDB_LIB_PATH=<OpenDB Built Library Directory>
> cmake ..
> make
> make install # Or sudo make install
```

## Usage

```bash
> ./Phy
> transform hello_transform net1
> write_def out.def
```

Make sure to set `PHY_HOME_PATH` to PhyKnight source code path, `PHY_LIB_PATH` to the directory containing the built PhyKnight library file, `OPENDB_HOME_PATH` to [OpenDB](https://github.com/The-OpenROAD-Project/OpenDB) include path, `OPENDB_LIB_PATH` the directory containing the built [OpenDB](https://github.com/The-OpenROAD-Project/OpenDB) library files.
