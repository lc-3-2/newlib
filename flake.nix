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

      # Convenience variable to reference the LLVM package
      llvm-lc-3-2-pkg = llvm-lc-3-2.packages.${system}.llvm-lc-3-2;

      # Ensure that the compiler is present at runtime. We need it to use the
      # library, after all.
      propagatedBuildInputs = [
        llvm-lc-3-2-pkg
      ];
      nativeBuildInputs = [
        pkgs.texinfo
      ];

      # What flags we'll use during cross-compilation
      cflags-for-lc-3-2 = builtins.concatStringsSep " " [
        "--target=lc_3.2-none"
        "-g"
      ];

    in {

      packages = rec {
        default = newlib-lc-3-2;

        newlib-lc-3-2 = stdenv.mkDerivation {
          inherit name propagatedBuildInputs nativeBuildInputs;

          src = self;

          configurePhase = ''
            runHook preConfigure

            CC_FOR_TARGET="${llvm-lc-3-2-pkg}/bin/clang" \
            CXX_FOR_TARGET="${llvm-lc-3-2-pkg}/bin/clang++" \
            AS_FOR_TARGET="${llvm-lc-3-2-pkg}/bin/llvm-mc" \
            LD_FOR_TARGET="${llvm-lc-3-2-pkg}/bin/ld.lld" \
            AR_FOR_TARGET="${llvm-lc-3-2-pkg}/bin/llvm-ar" \
            RANLIB_FOR_TARGET="${llvm-lc-3-2-pkg}/bin/llvm-ranlib" \
            STRIP_FOR_TARGET="${llvm-lc-3-2-pkg}/bin/llvm-strip" \
            OBJCOPY_FOR_TARGET="${llvm-lc-3-2-pkg}/bin/llvm-objcopy" \
            OBJDUMP_FOR_TARGET="${llvm-lc-3-2-pkg}/bin/llvm-objdump" \
            READELF_FOR_TARGET="${llvm-lc-3-2-pkg}/bin/llvm-readelf" \
            NM_FOR_TARGET="${llvm-lc-3-2-pkg}/bin/llvm-nm" \
            DLLTOOL_FOR_TARGET="${llvm-lc-3-2-pkg}/bin/llvm-dlltool" \
            LIPO_FOR_TARGET="${llvm-lc-3-2-pkg}/bin/llvm-lipo" \
            WINDRES_FOR_TARGET="${llvm-lc-3-2-pkg}/bin/llvm-windres" \
            CFLAGS_FOR_TARGET="${cflags-for-lc-3-2}" \
            ../configure \
              --prefix=$out --host=${system} --target=lc_3.2-none \
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
            echo "$out/lc_3.2-none/"
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
