{
  description = "Newlib for LC-3.2 Development and Build Environment";

  inputs = {
    nixpkgs-23-05.url = "github:NixOS/nixpkgs/release-23.05";
    flake-utils.url = "github:numtide/flake-utils";

    # We have to use SSH here since the repository is private
    llvm-lc-3-2.url = "git+ssh://git@github.com/lc-3-2/llvm.git";
  };

  outputs = { self, nixpkgs-23-05, flake-utils, llvm-lc-3-2 }@inputs :
    flake-utils.lib.eachSystem ["x86_64-linux"] (system:
    let
      name = "newlib-lc-3.2-dev";
      pkgs = nixpkgs-23-05.legacyPackages.${system};
      inherit (pkgs) stdenv;

      nativeBuildInputs = [
        pkgs.texinfo

        # Usually, we would add custom packages with an overlay, but that adds a
        # lot of complexity. Thus, we fetch the package manually.
        llvm-lc-3-2.packages.${system}.lc-3-2
      ];

    in {

      packages = rec {
        default = newlib-lc-3-2;

        newlib-lc-3-2 = stdenv.mkDerivation {
          inherit name nativeBuildInputs;

          src = self;

          configurePhase = ''
            mkdir ./build/
            pushd ./build/

            CC_FOR_TARGET="clang" \
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
            CFLAGS_FOR_TARGET="-nostdlib -D__IEEE_LITTLE_ENDIAN -v" \
            ../configure \
              --host=x86_64-pc-linux-gnu --target=lc_3.2 \
              --prefix=$out \
              --disable-multilib

            popd
          '';

          buildPhase = "make -C ./build/";
          installPhase = "make -C ./build/ install";
        };
      };

      devShells = rec {
        default = newlib-lc-3-2;

        newlib-lc-3-2 = pkgs.mkShell {
          inherit name nativeBuildInputs;

          # Add autotools in case we need to modify and regenerate. We don't
          # mark this as a nativeBuildInput for the package since this is only
          # done when we push changes.
          buildInputs = [
            pkgs.autoconf269
            pkgs.automake115x
          ];

          shellHook = ''
            export PS1="(${name}) [\u@\h \W]\$ "
          '';
        };
      };
    });
}
