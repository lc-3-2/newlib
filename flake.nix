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

      # Ensure that the compiler is present at runtime. We need it to use the
      # library, after all.
      propagatedBuildInputs = [
        llvm-lc-3-2.packages.${system}.llvm-lc-3-2
      ];

      nativeBuildInputs = [
        pkgs.texinfo
      ];

    in {

      packages = rec {
        default = newlib-lc-3-2;

        newlib-lc-3-2 = stdenv.mkDerivation {
          inherit name propagatedBuildInputs nativeBuildInputs;

          src = self;

          configurePhase = ''
            runHook preConfigure

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
            CFLAGS_FOR_TARGET="-ffunction-sections -fdata-sections" \
            ../configure \
              --prefix=$out --host=${system} --target=lc_3.2 \
              --enable-newlib-register-fini --disable-newlib-supplied-syscalls \
              --disable-multilib

            runHook postConfigure
          '';

          # Build in parallel
          enableParallelBuilding = true;

          # Build out of tree
          preConfigure = ''
            mkdir ./build/
            pushd ./build/
          '';
          postConfigure = ''
            popd
          '';
          makeFlags = ["-C" "./build/"];

          # Add an easy way to get the sysroot to pass to `clang`. This is
          # needed when we're running it in the shell.
          postInstall = ''
            mkdir $out/bin/

            cat <<EOF > $out/bin/lc32newlib-sysroot
            #!/bin/bash
            echo "$out/lc_3.2/"
            EOF

            chmod +x $out/bin/lc32newlib-sysroot
          '';
        };
      };

      devShells = rec {
        default = newlib-lc-3-2;

        newlib-lc-3-2 = pkgs.mkShell {
          inherit name propagatedBuildInputs nativeBuildInputs;

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
