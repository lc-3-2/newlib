# Newlib for LC-3.2

This repository houses a version of Newlib that was modified so that `libgloss`
has support for LC-3.2 targets.

## Building

### Build Environment
For ease of use, this repository has a Nix development shell with the required
packages. In addition to the normal build environment, the project requires
Autoconf 2.69 and Automake 1.15.1 - those specific versions are required. It
also requires [lc-3-2/llvm](https://github.com/lc-3-2/llvm), ideally on the
`PATH`.

### Build Instructions

To build this project, check out the repository and navigate into it. Create a
build subdirectory and change into it too. Configure with
```bash
$ CC_FOR_TARGET="clang" \
  CXX_FOR_TARGET="clang++" \
  AS_FOR_TARGET="llvm-mc" \
  LD_FOR_TARGET="ld.lld" \
  AR_FOR_TARGET="llvm-ar" \
  RANLIB_FOR_TARGET="llvm-ranlib" \
  STRIP_FOR_TARGET="llvm-strip" \
  OBJCOPY_FOR_TARGET="llvm-objcopy" \
  OBJDUMP_FOR_TARGET="llvm-objdump" \
  READELF_FOR_TARGET="llvm-readelf" \
  NM_FOR_TARGET="llvm-nm" \
  DLLTOOL_FOR_TARGET="llvm-dlltool" \
  LIPO_FOR_TARGET="llvm-lipo" \
  WINDRES_FOR_TARGET="llvm-windres" \
  CFLAGS_FOR_TARGET="--target=lc_3.2-none -g -ffunction-sections -fdata-sections -Wno-unused-command-line-argument -Wno-unknown-pragmas -Wno-deprecated-non-prototype" \
  ../configure \
    --prefix=${NEWLIB_INSTALL_DIR} --host=x86_64-linux --target=lc_3.2-none \
    --enable-newlib-register-fini --disable-newlib-supplied-syscalls \
    --enable-newlib-io-long-long --enable-newlib-io-long-double \
    --disable-multilib
```
Make sure to set `--prefix` to the installation directory. Also make sure all
the `*_FOR_TARGET` variables point to the programs from
[lc-3-2/llvm](https://github.com/lc-3-2/llvm) - use the full path if the
programs are not on the `PATH`.

With configuration done, build and install with
```bash
$ make
$ make install
```
You can build in parallel if you want to.

### Modifying Build Instructions

Autoconf and Automake are not needed for the normal build process. However, if
the macroed build instructions (`configure.ac` or `Makefile.am`) are changed,
the real build instructions (`configure` or `Makefile`) need to be regenerated.
To do this, first change into the appropriate subproject directory - usually
`libgloss` or `newlib`. Then run
```bash
$ autoreconf
```

Make sure to check in the changes made to the real build instructions, as is
tradition with Autotools-based projects.

## Usage

Once this project is installed, it can be used with
[lc-3-2/llvm](https://github.com/lc-3-2/llvm). To do so, supply the appropriate
`--sysroot` to the compiler. Usually, this is
`${NEWLIB_INSTALL_DIR}/lc_3.2-none`.

Additionally, you must choose a "backend" to compile your code with. Different
backends assume different hardware and thus provide different capabilities. The
backends are:
* `Trap` assumes the standard traps (`GETC`, `OUT`, `HALT`) are implemented. It
  provides console IO, but it does not provide entropy, time, or file system
  facilities.
* `Sim` assumes the environment provided by
  [lc-3-2/lc32sim](https://github.com/lc-3-2/lc32sim). In particular, it assumes
  the standard traps are implemented, and that some MMIO devices exist at
  particular addresses.

Once you choose a backend `${BACKEND}`, supply the argument
`-Tlc32${BACKEND}.ld` during compilation. This linker script is provided with
the library and will thus be found in the system root.

## Contributing
To contribute, please file an issue or a pull request on this GitHub.
Alternatively, email the project maintainers directly.

### Organization

Most of the new code was written in `libgloss/lc32/`. It is organized in terms
of backends and capabilities. Each backend, in the sense explained above, have
their own linker script and their own entry in `Makefile.inc`. If you want to
add a new backend, add its linker script and its macroed build instructions,
then run `autoreconf`.

Each backend uses some capabilities. The capabilities are:
* `Trap` assumes the standard traps (`GETC`, `OUT`, `HALT`) are implemented.
* `Rngmmio` assumes a memory-mapped register that gives random numbers.
* `Clockmmio` assumes a memory-mapped clock as implemented in the simulator.

The special value `x` signifies no capabilities. The code is organized in terms
of the capabilities it uses, and the backends specify which of those files they
want to include via `Makefile.inc`.
