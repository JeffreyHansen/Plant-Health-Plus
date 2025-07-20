Package: libiconv:x64-windows@1.18#1

**Host Environment**

- Host: x64-windows
- Compiler: MSVC 19.44.35213.0
-    vcpkg-tool version: 2025-07-16-d6c019e723df46cb8c36360e4174b111455567d3
    vcpkg-scripts version: b91c3336ae 2025-07-19 (19 hours ago)

**To Reproduce**

`vcpkg install `

**Failure logs**

```
Downloading libiconv-1.18.tar.gz, trying https://ftp.gnu.org/gnu/libiconv/libiconv-1.18.tar.gz
Successfully downloaded libiconv-1.18.tar.gz
-- Cleaning sources at C:/Users/Tal/Desktop/Plant Health +/Plant-Health-Plus/PlantHealthPlus/vcpkg/buildtrees/libiconv/src/v1.18-8006e03beb.clean. Use --editable to skip cleaning for the packages you specify.
-- Extracting source C:/Users/Tal/Desktop/Plant Health +/Plant-Health-Plus/PlantHealthPlus/vcpkg/downloads/libiconv-1.18.tar.gz
-- Applying patch 0002-Config-for-MSVC.patch
-- Applying patch 0003-Add-export.patch
-- Applying patch 0004-ModuleFileName.patch
-- Using source at C:/Users/Tal/Desktop/Plant Health +/Plant-Health-Plus/PlantHealthPlus/vcpkg/buildtrees/libiconv/src/v1.18-8006e03beb.clean
-- Found external ninja('1.12.1').
-- Getting CMake variables for x64-windows
-- Warning: Paths with embedded space may be handled incorrectly by configure:
   C:/Users/Tal/Desktop/Plant Health +/Plant-Health-Plus/PlantHealthPlus/vcpkg/packages/libiconv_x64-windows
   C:/Users/Tal/Desktop/Plant Health +/Plant-Health-Plus/PlantHealthPlus/vcpkg_installed/x64-windows
   Please move the path to one without whitespaces!
Downloading msys2-autoconf-wrapper-20240607-1-any.pkg.tar.zst, trying https://mirror.msys2.org/msys/x86_64/autoconf-wrapper-20240607-1-any.pkg.tar.zst
Successfully downloaded msys2-autoconf-wrapper-20240607-1-any.pkg.tar.zst
Downloading msys2-automake-wrapper-20240607-1-any.pkg.tar.zst, trying https://mirror.msys2.org/msys/x86_64/automake-wrapper-20240607-1-any.pkg.tar.zst
Successfully downloaded msys2-automake-wrapper-20240607-1-any.pkg.tar.zst
Downloading msys2-binutils-2.44-1-x86_64.pkg.tar.zst, trying https://mirror.msys2.org/msys/x86_64/binutils-2.44-1-x86_64.pkg.tar.zst
Successfully downloaded msys2-binutils-2.44-1-x86_64.pkg.tar.zst
Downloading msys2-libtool-2.5.4-1-x86_64.pkg.tar.zst, trying https://mirror.msys2.org/msys/x86_64/libtool-2.5.4-1-x86_64.pkg.tar.zst
Successfully downloaded msys2-libtool-2.5.4-1-x86_64.pkg.tar.zst
Downloading msys2-make-4.4.1-2-x86_64.pkg.tar.zst, trying https://mirror.msys2.org/msys/x86_64/make-4.4.1-2-x86_64.pkg.tar.zst
Successfully downloaded msys2-make-4.4.1-2-x86_64.pkg.tar.zst
Downloading msys2-pkgconf-2.4.3-1-x86_64.pkg.tar.zst, trying https://mirror.msys2.org/msys/x86_64/pkgconf-2.4.3-1-x86_64.pkg.tar.zst
Successfully downloaded msys2-pkgconf-2.4.3-1-x86_64.pkg.tar.zst
Downloading msys2-which-2.23-4-x86_64.pkg.tar.zst, trying https://mirror.msys2.org/msys/x86_64/which-2.23-4-x86_64.pkg.tar.zst
Successfully downloaded msys2-which-2.23-4-x86_64.pkg.tar.zst
Downloading msys2-bash-5.2.037-2-x86_64.pkg.tar.zst, trying https://mirror.msys2.org/msys/x86_64/bash-5.2.037-2-x86_64.pkg.tar.zst
Successfully downloaded msys2-bash-5.2.037-2-x86_64.pkg.tar.zst
Downloading msys2-coreutils-8.32-5-x86_64.pkg.tar.zst, trying https://mirror.msys2.org/msys/x86_64/coreutils-8.32-5-x86_64.pkg.tar.zst
Successfully downloaded msys2-coreutils-8.32-5-x86_64.pkg.tar.zst
Downloading msys2-file-5.46-2-x86_64.pkg.tar.zst, trying https://mirror.msys2.org/msys/x86_64/file-5.46-2-x86_64.pkg.tar.zst
Successfully downloaded msys2-file-5.46-2-x86_64.pkg.tar.zst
Downloading msys2-gawk-5.3.2-1-x86_64.pkg.tar.zst, trying https://mirror.msys2.org/msys/x86_64/gawk-5.3.2-1-x86_64.pkg.tar.zst
Successfully downloaded msys2-gawk-5.3.2-1-x86_64.pkg.tar.zst
Downloading msys2-grep-1~3.0-7-x86_64.pkg.tar.zst, trying https://mirror.msys2.org/msys/x86_64/grep-1~3.0-7-x86_64.pkg.tar.zst
Successfully downloaded msys2-grep-1~3.0-7-x86_64.pkg.tar.zst
Downloading msys2-gzip-1.14-1-x86_64.pkg.tar.zst, trying https://mirror.msys2.org/msys/x86_64/gzip-1.14-1-x86_64.pkg.tar.zst
Successfully downloaded msys2-gzip-1.14-1-x86_64.pkg.tar.zst
Downloading msys2-diffutils-3.12-1-x86_64.pkg.tar.zst, trying https://mirror.msys2.org/msys/x86_64/diffutils-3.12-1-x86_64.pkg.tar.zst
Successfully downloaded msys2-diffutils-3.12-1-x86_64.pkg.tar.zst
Downloading msys2-sed-4.9-1-x86_64.pkg.tar.zst, trying https://mirror.msys2.org/msys/x86_64/sed-4.9-1-x86_64.pkg.tar.zst
Successfully downloaded msys2-sed-4.9-1-x86_64.pkg.tar.zst
Downloading msys2-msys2-runtime-3.6.2-2-x86_64.pkg.tar.zst, trying https://mirror.msys2.org/msys/x86_64/msys2-runtime-3.6.2-2-x86_64.pkg.tar.zst
Successfully downloaded msys2-msys2-runtime-3.6.2-2-x86_64.pkg.tar.zst
Downloading msys2-autoconf2.72-2.72-3-any.pkg.tar.zst, trying https://mirror.msys2.org/msys/x86_64/autoconf2.72-2.72-3-any.pkg.tar.zst
Successfully downloaded msys2-autoconf2.72-2.72-3-any.pkg.tar.zst
Downloading msys2-automake1.16-1.16.5-1-any.pkg.tar.zst, trying https://mirror.msys2.org/msys/x86_64/automake1.16-1.16.5-1-any.pkg.tar.zst
Successfully downloaded msys2-automake1.16-1.16.5-1-any.pkg.tar.zst
Downloading msys2-automake1.17-1.17-1-any.pkg.tar.zst, trying https://mirror.msys2.org/msys/x86_64/automake1.17-1.17-1-any.pkg.tar.zst
Successfully downloaded msys2-automake1.17-1.17-1-any.pkg.tar.zst
Downloading msys2-libiconv-1.18-1-x86_64.pkg.tar.zst, trying https://mirror.msys2.org/msys/x86_64/libiconv-1.18-1-x86_64.pkg.tar.zst
Successfully downloaded msys2-libiconv-1.18-1-x86_64.pkg.tar.zst
Downloading msys2-libintl-0.22.5-1-x86_64.pkg.tar.zst, trying https://mirror.msys2.org/msys/x86_64/libintl-0.22.5-1-x86_64.pkg.tar.zst
Successfully downloaded msys2-libintl-0.22.5-1-x86_64.pkg.tar.zst
Downloading msys2-zlib-1.3.1-1-x86_64.pkg.tar.zst, trying https://mirror.msys2.org/msys/x86_64/zlib-1.3.1-1-x86_64.pkg.tar.zst
Successfully downloaded msys2-zlib-1.3.1-1-x86_64.pkg.tar.zst
Downloading msys2-findutils-4.10.0-2-x86_64.pkg.tar.zst, trying https://mirror.msys2.org/msys/x86_64/findutils-4.10.0-2-x86_64.pkg.tar.zst
Successfully downloaded msys2-findutils-4.10.0-2-x86_64.pkg.tar.zst
Downloading msys2-tar-1.35-2-x86_64.pkg.tar.zst, trying https://mirror.msys2.org/msys/x86_64/tar-1.35-2-x86_64.pkg.tar.zst
Successfully downloaded msys2-tar-1.35-2-x86_64.pkg.tar.zst
Downloading msys2-gmp-6.3.0-1-x86_64.pkg.tar.zst, trying https://mirror.msys2.org/msys/x86_64/gmp-6.3.0-1-x86_64.pkg.tar.zst
Successfully downloaded msys2-gmp-6.3.0-1-x86_64.pkg.tar.zst
Downloading msys2-gcc-libs-13.3.0-1-x86_64.pkg.tar.zst, trying https://mirror.msys2.org/msys/x86_64/gcc-libs-13.3.0-1-x86_64.pkg.tar.zst
Successfully downloaded msys2-gcc-libs-13.3.0-1-x86_64.pkg.tar.zst
Downloading msys2-libbz2-1.0.8-4-x86_64.pkg.tar.zst, trying https://mirror.msys2.org/msys/x86_64/libbz2-1.0.8-4-x86_64.pkg.tar.zst
Successfully downloaded msys2-libbz2-1.0.8-4-x86_64.pkg.tar.zst
Downloading msys2-liblzma-5.8.1-1-x86_64.pkg.tar.zst, trying https://mirror.msys2.org/msys/x86_64/liblzma-5.8.1-1-x86_64.pkg.tar.zst
Successfully downloaded msys2-liblzma-5.8.1-1-x86_64.pkg.tar.zst
Downloading msys2-libzstd-1.5.7-1-x86_64.pkg.tar.zst, trying https://mirror.msys2.org/msys/x86_64/libzstd-1.5.7-1-x86_64.pkg.tar.zst
Successfully downloaded msys2-libzstd-1.5.7-1-x86_64.pkg.tar.zst
Downloading msys2-libreadline-8.2.013-1-x86_64.pkg.tar.zst, trying https://mirror.msys2.org/msys/x86_64/libreadline-8.2.013-1-x86_64.pkg.tar.zst
Successfully downloaded msys2-libreadline-8.2.013-1-x86_64.pkg.tar.zst
Downloading msys2-mpfr-4.2.2-1-x86_64.pkg.tar.zst, trying https://mirror.msys2.org/msys/x86_64/mpfr-4.2.2-1-x86_64.pkg.tar.zst
Successfully downloaded msys2-mpfr-4.2.2-1-x86_64.pkg.tar.zst
Downloading msys2-libpcre-8.45-5-x86_64.pkg.tar.zst, trying https://mirror.msys2.org/msys/x86_64/libpcre-8.45-5-x86_64.pkg.tar.zst
Successfully downloaded msys2-libpcre-8.45-5-x86_64.pkg.tar.zst
Downloading msys2-m4-1.4.19-2-x86_64.pkg.tar.zst, trying https://mirror.msys2.org/msys/x86_64/m4-1.4.19-2-x86_64.pkg.tar.zst
Successfully downloaded msys2-m4-1.4.19-2-x86_64.pkg.tar.zst
Downloading msys2-perl-5.38.4-2-x86_64.pkg.tar.zst, trying https://mirror.msys2.org/msys/x86_64/perl-5.38.4-2-x86_64.pkg.tar.zst
Successfully downloaded msys2-perl-5.38.4-2-x86_64.pkg.tar.zst
Downloading msys2-ncurses-6.5.20240831-2-x86_64.pkg.tar.zst, trying https://mirror.msys2.org/msys/x86_64/ncurses-6.5.20240831-2-x86_64.pkg.tar.zst
Successfully downloaded msys2-ncurses-6.5.20240831-2-x86_64.pkg.tar.zst
Downloading msys2-libxcrypt-4.4.38-1-x86_64.pkg.tar.zst, trying https://mirror.msys2.org/msys/x86_64/libxcrypt-4.4.38-1-x86_64.pkg.tar.zst
Successfully downloaded msys2-libxcrypt-4.4.38-1-x86_64.pkg.tar.zst
-- Applying patch C:/Users/Tal/Desktop/Plant Health +/Plant-Health-Plus/PlantHealthPlus/vcpkg/scripts/cmake/compile_wrapper_consider_clang-cl.patch
-- Using msys root at C:/Users/Tal/Desktop/Plant Health +/Plant-Health-Plus/PlantHealthPlus/vcpkg/downloads/tools/msys2/7c55719f9ca5acf6
Downloading msys2-mingw-w64-x86_64-pkgconf-1~2.4.3-1-any.pkg.tar.zst, trying https://mirror.msys2.org/mingw/mingw64/mingw-w64-x86_64-pkgconf-1~2.4.3-1-any.pkg.tar.zst
Successfully downloaded msys2-mingw-w64-x86_64-pkgconf-1~2.4.3-1-any.pkg.tar.zst
-- Using cached msys2-msys2-runtime-3.6.2-2-x86_64.pkg.tar.zst
-- Using msys root at C:/Users/Tal/Desktop/Plant Health +/Plant-Health-Plus/PlantHealthPlus/vcpkg/downloads/tools/msys2/9272adbcaf19caef
-- Configuring x64-windows-dbg
CMake Error at scripts/cmake/vcpkg_execute_required_process.cmake:127 (message):
    Command failed: "C:/Users/Tal/Desktop/Plant Health +/Plant-Health-Plus/PlantHealthPlus/vcpkg/downloads/tools/msys2/7c55719f9ca5acf6/usr/bin/bash.exe" --noprofile --norc --debug -c "V=1 CPP='compile cl.exe -E' CC='compile cl.exe' CC_FOR_BUILD='compile cl.exe' CPP_FOR_BUILD='compile cl.exe -E' CXX_FOR_BUILD='compile cl.exe' CXX='compile cl.exe' RC='windres-rc rc.exe' WINDRES='windres-rc rc.exe' AR='ar-lib lib.exe' LD='link.exe -verbose' RANLIB=':' STRIP=':' NM='dumpbin.exe -symbols -headers' DLLTOOL='link.exe -verbose -dll' CCAS=':' AS=':' ./../src/v1.18-8006e03beb.clean/configure --build=x86_64-pc-mingw32 \"--enable-extra-encodings\" \"--without-libiconv-prefix\" \"--without-libintl-prefix\" \"--enable-relocatable\" \"ac_cv_prog_ac_ct_STRIP=:\" \"gl_cv_double_slash_root=yes\" \"ac_cv_func_memmove=yes\" \"--disable-silent-rules\" \"--verbose\" \"--enable-shared\" \"--disable-static\" \"--prefix=/C/Users/Tal/Desktop/Plant Health +/Plant-Health-Plus/PlantHealthPlus/vcpkg_installed/x64-windows/debug\" \"--bindir=\\${prefix}/../tools/libiconv/debug/bin\" \"--sbindir=\\${prefix}/../tools/libiconv/debug/sbin\" \"--libdir=\\${prefix}/lib\" \"--includedir=\\${prefix}/../include\" \"--datarootdir=\\${prefix}/share/libiconv\""
    Working Directory: C:/Users/Tal/Desktop/Plant Health +/Plant-Health-Plus/PlantHealthPlus/vcpkg/buildtrees/libiconv/x64-windows-dbg
    Error code: 77
    See logs for more information:
      C:\Users\Tal\Desktop\Plant Health +\Plant-Health-Plus\PlantHealthPlus\vcpkg\buildtrees\libiconv\config-x64-windows-dbg-config.log
      C:\Users\Tal\Desktop\Plant Health +\Plant-Health-Plus\PlantHealthPlus\vcpkg\buildtrees\libiconv\config-x64-windows-dbg-out.log
      C:\Users\Tal\Desktop\Plant Health +\Plant-Health-Plus\PlantHealthPlus\vcpkg\buildtrees\libiconv\config-x64-windows-dbg-err.log

Call Stack (most recent call first):
  scripts/cmake/vcpkg_configure_make.cmake:866 (vcpkg_execute_required_process)
  ports/libiconv/portfile.cmake:42 (vcpkg_configure_make)
  scripts/ports.cmake:206 (include)



```

<details><summary>C:\Users\Tal\Desktop\Plant Health +\Plant-Health-Plus\PlantHealthPlus\vcpkg\buildtrees\libiconv\config-x64-windows-dbg-err.log</summary>

```
configure: error: in '/c/Users/Tal/Desktop/Plant Health +/Plant-Health-Plus/PlantHealthPlus/vcpkg/buildtrees/libiconv/x64-windows-dbg':
configure: error: C compiler cannot create executables
See 'config.log' for more details
```
</details>

<details><summary>C:\Users\Tal\Desktop\Plant Health +\Plant-Health-Plus\PlantHealthPlus\vcpkg\buildtrees\libiconv\config-x64-windows-dbg-out.log</summary>

```
checking for a BSD-compatible install... /usr/bin/install -c
checking whether sleep supports fractional seconds... yes
checking filesystem timestamp resolution... 0.01
checking whether build environment is sane... yes
checking for a race-free mkdir -p... /usr/bin/mkdir -p
checking for gawk... gawk
checking whether make sets $(MAKE)... yes
checking whether make supports nested variables... yes
checking xargs -n works... yes
checking whether make sets $(MAKE)... (cached) yes
checking for gcc... compile cl.exe
checking whether the C compiler works... no
```
</details>

<details><summary>C:\Users\Tal\Desktop\Plant Health +\Plant-Health-Plus\PlantHealthPlus\vcpkg\buildtrees\libiconv\config-x64-windows-dbg-config.log</summary>

```
This file contains any messages produced by compilers while
running configure, to aid debugging if configure makes a mistake.

It was created by libiconv configure 1.18, which was
generated by GNU Autoconf 2.72.  Invocation command line was

  $ ./../src/v1.18-8006e03beb.clean/configure --build=x86_64-pc-mingw32 --enable-extra-encodings --without-libiconv-prefix --without-libintl-prefix --enable-relocatable ac_cv_prog_ac_ct_STRIP=: gl_cv_double_slash_root=yes ac_cv_func_memmove=yes --disable-silent-rules --verbose --enable-shared --disable-static '--prefix=/C/Users/Tal/Desktop/Plant Health +/Plant-Health-Plus/PlantHealthPlus/vcpkg_installed/x64-windows/debug' '--bindir=${prefix}/../tools/libiconv/debug/bin' '--sbindir=${prefix}/../tools/libiconv/debug/sbin' '--libdir=${prefix}/lib' '--includedir=${prefix}/../include' '--datarootdir=${prefix}/share/libiconv'

## --------- ##
## Platform. ##
## --------- ##

hostname = DESKTOP-FEK8KLL
uname -m = x86_64
uname -r = 3.6.2-a9cfe9f5.x86_64
uname -s = MSYS_NT-10.0-19045
uname -v = 2025-06-03 09:09 UTC

/usr/bin/uname -p = unknown
/bin/uname -X     = unknown

/bin/arch              = x86_64
/usr/bin/arch -k       = unknown
/usr/convex/getsysinfo = unknown
/usr/bin/hostinfo      = unknown
/bin/machine           = unknown
/usr/bin/oslevel       = unknown
/bin/universe          = unknown

PATH: /c/Program Files (x86)/Windows Kits/10/bin/10.0.26100.0/x64/
PATH: /c/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.44.35207/bin/Hostx64/x64/
PATH: /c/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.44.35207/bin/Hostx64/x64/
PATH: /c/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.44.35207/bin/HostX64/x64/
PATH: /c/Program Files/Microsoft Visual Studio/2022/Community/Common7/IDE/VC/VCPackages/
PATH: /c/Program Files/Microsoft Visual Studio/2022/Community/Common7/IDE/CommonExtensions/Microsoft/TestWindow/
PATH: /c/Program Files/Microsoft Visual Studio/2022/Community/Common7/IDE/CommonExtensions/Microsoft/TeamFoundation/Team Explorer/
PATH: /c/Program Files/Microsoft Visual Studio/2022/Community/MSBuild/Current/bin/Roslyn/
PATH: /c/Program Files/Microsoft Visual Studio/2022/Community/Team Tools/DiagnosticsHub/Collector/
PATH: /c/Program Files (x86)/Windows Kits/10/bin/10.0.26100.0/x64/
PATH: /c/Program Files (x86)/Windows Kits/10/bin/x64/
PATH: /c/Program Files/Microsoft Visual Studio/2022/Community/MSBuild/Current/Bin/amd64/
PATH: /c/Windows/Microsoft.NET/Framework64/v4.0.30319/
PATH: /c/Program Files/Microsoft Visual Studio/2022/Community/Common7/IDE/
PATH: /c/Program Files/Microsoft Visual Studio/2022/Community/Common7/Tools/
PATH: /c/Program Files/PowerShell/7/
PATH: /c/Users/Tal/Desktop/Plant Health +/Plant-Health-Plus/PlantHealthPlus/vcpkg/scripts/buildsystems/make_wrapper/
PATH: /usr/share/automake-1.16/
PATH: /usr/bin/
PATH: /c/Windows/system32/
PATH: /c/Windows/
PATH: /c/Windows/system32/Wbem/
PATH: /c/Windows/system32/WindowsPowerShell/v1.0/
PATH: /c/Windows/system32/OpenSSH/
PATH: /c/Program Files/Microsoft Visual Studio/2022/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/
PATH: /c/Program Files/Microsoft Visual Studio/2022/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/Ninja/
PATH: /c/Program Files/Microsoft Visual Studio/2022/Community/Common7/IDE/VC/Linux/bin/ConnectionManagerExe/
PATH: /c/Program Files/Microsoft Visual Studio/2022/Community/VC/vcpkg/
PATH: /c/Program Files/Microsoft Visual Studio/2022/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/Ninja/
PATH: /c/Users/Tal/Desktop/Plant Health +/Plant-Health-Plus/PlantHealthPlus/vcpkg/downloads/tools/msys2/9272adbcaf19caef/mingw64/bin/


## ----------- ##
## Core tests. ##
## ----------- ##

configure:3907: looking for aux files: config.rpath ltmain.sh config.guess config.sub compile missing install-sh
configure:3920:  trying ./../src/v1.18-8006e03beb.clean/build-aux/
configure:3949:   ./../src/v1.18-8006e03beb.clean/build-aux/config.rpath found
configure:3949:   ./../src/v1.18-8006e03beb.clean/build-aux/ltmain.sh found
configure:3949:   ./../src/v1.18-8006e03beb.clean/build-aux/config.guess found
configure:3949:   ./../src/v1.18-8006e03beb.clean/build-aux/config.sub found
configure:3949:   ./../src/v1.18-8006e03beb.clean/build-aux/compile found
configure:3949:   ./../src/v1.18-8006e03beb.clean/build-aux/missing found
configure:3931:   ./../src/v1.18-8006e03beb.clean/build-aux/install-sh found
configure:4081: checking for a BSD-compatible install
configure:4155: result: /usr/bin/install -c
configure:4166: checking whether sleep supports fractional seconds
configure:4182: result: yes
configure:4185: checking filesystem timestamp resolution
configure:4320: result: 0.01
configure:4325: checking whether build environment is sane
configure:4366: result: yes
configure:4537: checking for a race-free mkdir -p
configure:4580: result: /usr/bin/mkdir -p
configure:4587: checking for gawk
configure:4608: found /usr/bin/gawk
configure:4620: result: gawk
configure:4631: checking whether make sets $(MAKE)
configure:4655: result: yes
configure:4681: checking whether make supports nested variables
configure:4700: result: yes
configure:4714: checking xargs -n works
configure:4730: result: yes
configure:4830: checking whether make sets $(MAKE)
configure:4854: result: yes
configure:4928: checking for gcc
configure:4961: result: compile cl.exe
configure:5320: checking for C compiler version
configure:5329: compile cl.exe --version >&5
Microsoft (R) C/C++ Optimizing Compiler Version 19.44.35213 for x64
Copyright (C) Microsoft Corporation.  All rights reserved.

cl : Command line warning D9002 : ignoring unknown option '--version'
cl : Command line error D8003 : missing source filename
configure:5340: $? = 2
configure:5329: compile cl.exe -v >&5
Microsoft (R) C/C++ Optimizing Compiler Version 19.44.35213 for x64
Copyright (C) Microsoft Corporation.  All rights reserved.

cl : Command line warning D9002 : ignoring unknown option '-v'
cl : Command line error D8003 : missing source filename
configure:5340: $? = 2
configure:5329: compile cl.exe -V >&5
Microsoft (R) C/C++ Optimizing Compiler Version 19.44.35213 for x64
Copyright (C) Microsoft Corporation.  All rights reserved.

cl : Command line error D8004 : '/V' requires an argument
configure:5340: $? = 2
configure:5329: compile cl.exe -qversion >&5
Microsoft (R) C/C++ Optimizing Compiler Version 19.44.35213 for x64
Copyright (C) Microsoft Corporation.  All rights reserved.

cl : Command line warning D9002 : ignoring unknown option '-qversion'
cl : Command line error D8003 : missing source filename
configure:5340: $? = 2
configure:5329: compile cl.exe -version >&5
Microsoft (R) C/C++ Optimizing Compiler Version 19.44.35213 for x64
Copyright (C) Microsoft Corporation.  All rights reserved.

cl : Command line warning D9002 : ignoring unknown option '-version'
cl : Command line error D8003 : missing source filename
configure:5340: $? = 2
configure:5360: checking whether the C compiler works
configure:5382: compile cl.exe -Xcompiler -nologo -Xcompiler -utf-8 -Xcompiler -MP -Xcompiler -MDd -Xcompiler -Z7 -Xcompiler -Ob0 -Xcompiler -Od -Xcompiler -RTC1 -DWIN32 -D_WINDOWS -Xlinker -Xlinker -Xlinker -LIBPATH:C:/Users/Tal/Desktop/Plant\ Health\ +/Plant-Health-Plus/PlantHealthPlus/vcpkg_installed/x64-windows/debug/lib -Xlinker -Xlinker -Xlinker -machine:x64 -Xlinker -Xlinker -Xlinker -nologo -Xlinker -Xlinker -Xlinker -debug -Xlinker -Xlinker -Xlinker -INCREMENTAL conftest.c -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -lcomdlg32 -ladvapi32 >&5
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9024 : unrecognized source file type 'Health\', object file assumed
cl : Command line warning D9024 : unrecognized source file type '+C:/Users/Tal/Desktop/Plant Health +/Plant-Health-Plus/PlantHealthPlus/vcpkg/downloads/tools/msys2/7c55719f9ca5acf6/Plant-Health-Plus/PlantHealthPlus/vcpkg_installed/x64-windows/debug/lib', object file assumed
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9024 : unrecognized source file type 'Health\', object file assumed
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9024 : unrecognized source file type '+C:/Users/Tal/Desktop/Plant Health +/Plant-Health-Plus/PlantHealthPlus/vcpkg/downloads/tools/msys2/7c55719f9ca5acf6/Plant-Health-Plus/PlantHealthPlus/vcpkg_installed/x64-windows/debug/lib', object file assumed
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9024 : unrecognized source file type 'Health\', object file assumed
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
conftest.c
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9024 : unrecognized source file type 'Health\', object file assumed
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9024 : unrecognized source file type '+C:/Users/Tal/Desktop/Plant Health +/Plant-Health-Plus/PlantHealthPlus/vcpkg/downloads/tools/msys2/7c55719f9ca5acf6/Plant-Health-Plus/PlantHealthPlus/vcpkg_installed/x64-windows/debug/lib', object file assumed
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9024 : unrecognized source file type '+C:/Users/Tal/Desktop/Plant Health +/Plant-Health-Plus/PlantHealthPlus/vcpkg/downloads/tools/msys2/7c55719f9ca5acf6/Plant-Health-Plus/PlantHealthPlus/vcpkg_installed/x64-windows/debug/lib', object file assumed
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9024 : unrecognized source file type 'Health\', object file assumed
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9024 : unrecognized source file type 'Health\', object file assumed
cl : Command line warning D9002 : ignoring unknown option '-Xcompiler'
cl : Command line warning D9024 : unrecognized source file type 'Health\', object file assumed
cl : Command line warning D9024 : unrecognized source file type 'Health\', object file assumed
cl : Command line warning D9024 : unrecognized source file type 'Health\', object file assumed
cl : Command line warning D9024 : unrecognized source file type 'Health\', object file assumed
cl : Command line warning D9024 : unrecognized source file type '+C:/Users/Tal/Desktop/Plant Health +/Plant-Health-Plus/PlantHealthPlus/vcpkg/downloads/tools/msys2/7c55719f9ca5acf6/Plant-Health-Plus/PlantHealthPlus/vcpkg_installed/x64-windows/debug/lib', object file assumed
cl : Command line warning D9024 : unrecognized source file type 'Health\', object file assumed
cl : Command line warning D9024 : unrecognized source file type 'Health\', object file assumed
cl : Command line warning D9024 : unrecognized source file type '+C:/Users/Tal/Desktop/Plant Health +/Plant-Health-Plus/PlantHealthPlus/vcpkg/downloads/tools/msys2/7c55719f9ca5acf6/Plant-Health-Plus/PlantHealthPlus/vcpkg_installed/x64-windows/debug/lib', object file assumed
cl : Command line warning D9024 : unrecognized source file type 'Health\', object file assumed
cl : Command line warning D9024 : unrecognized source file type '+C:/Users/Tal/Desktop/Plant Health +/Plant-Health-Plus/PlantHealthPlus/vcpkg/downloads/tools/msys2/7c55719f9ca5acf6/Plant-Health-Plus/PlantHealthPlus/vcpkg_installed/x64-windows/debug/lib', object file assumed
cl : Command line warning D9024 : unrecognized source file type '+C:/Users/Tal/Desktop/Plant Health +/Plant-Health-Plus/PlantHealthPlus/vcpkg/downloads/tools/msys2/7c55719f9ca5acf6/Plant-Health-Plus/PlantHealthPlus/vcpkg_installed/x64-windows/debug/lib', object file assumed
cl : Command line warning D9024 : unrecognized source file type '+C:/Users/Tal/Desktop/Plant Health +/Plant-Health-Plus/PlantHealthPlus/vcpkg/downloads/tools/msys2/7c55719f9ca5acf6/Plant-Health-Plus/PlantHealthPlus/vcpkg_installed/x64-windows/debug/lib', object file assumed
cl : Command line warning D9024 : unrecognized source file type '+C:/Users/Tal/Desktop/Plant Health +/Plant-Health-Plus/PlantHealthPlus/vcpkg/downloads/tools/msys2/7c55719f9ca5acf6/Plant-Health-Plus/PlantHealthPlus/vcpkg_installed/x64-windows/debug/lib', object file assumed
cl : Command line warning D9024 : unrecognized source file type '+C:/Users/Tal/Desktop/Plant Health +/Plant-Health-Plus/PlantHealthPlus/vcpkg/downloads/tools/msys2/7c55719f9ca5acf6/Plant-Health-Plus/PlantHealthPlus/vcpkg_installed/x64-windows/debug/lib', object file assumed
cl : Command line warning D9024 : unrecognized source file type '+C:/Users/Tal/Desktop/Plant Health +/Plant-Health-Plus/PlantHealthPlus/vcpkg/downloads/tools/msys2/7c55719f9ca5acf6/Plant-Health-Plus/PlantHealthPlus/vcpkg_installed/x64-windows/debug/lib', object file assumed
cl : Command line warning D9024 : unrecognized source file type '+C:/Users/Tal/Desktop/Plant Health +/Plant-Health-Plus/PlantHealthPlus/vcpkg/downloads/tools/msys2/7c55719f9ca5acf6/Plant-Health-Plus/PlantHealthPlus/vcpkg_installed/x64-windows/debug/lib', object file assumed
LINK : warning LNK4044: unrecognized option '/Xlinker'; ignored
LINK : warning LNK4044: unrecognized option '/Xlinker'; ignored
LINK : warning LNK4044: unrecognized option '/Xlinker'; ignored
LINK : warning LNK4044: unrecognized option '/Xlinker'; ignored
LINK : warning LNK4044: unrecognized option '/Xlinker'; ignored
LINK : fatal error LNK1104: cannot open file 'Health\.obj'
configure:5386: $? = 2
configure:5427: result: no
configure: failed program was:
| /* confdefs.h */
| #define PACKAGE_NAME "libiconv"
| #define PACKAGE_TARNAME "libiconv"
| #define PACKAGE_VERSION "1.18"
| #define PACKAGE_STRING "libiconv 1.18"
| #define PACKAGE_BUGREPORT ""
| #define PACKAGE_URL ""
| #define PACKAGE "libiconv"
| #define VERSION "1.18"
| /* end confdefs.h.  */
| 
| int
| main (void)
| {
| 
|   ;
|   return 0;
| }
configure:5432: error: in '/c/Users/Tal/Desktop/Plant Health +/Plant-Health-Plus/PlantHealthPlus/vcpkg/buildtrees/libiconv/x64-windows-dbg':
configure:5434: error: C compiler cannot create executables
See 'config.log' for more details

## ---------------- ##
## Cache variables. ##
## ---------------- ##

ac_cv_env_CC_set=set
ac_cv_env_CC_value='compile cl.exe'
ac_cv_env_CFLAGS_set=set
ac_cv_env_CFLAGS_value='-Xcompiler -nologo -Xcompiler -utf-8 -Xcompiler -MP -Xcompiler -MDd -Xcompiler -Z7 -Xcompiler -Ob0 -Xcompiler -Od -Xcompiler -RTC1'
ac_cv_env_CPPFLAGS_set=set
ac_cv_env_CPPFLAGS_value='-DWIN32 -D_WINDOWS'
ac_cv_env_CPP_set=set
ac_cv_env_CPP_value='compile cl.exe -E'
ac_cv_env_LDFLAGS_set=set
ac_cv_env_LDFLAGS_value='-Xlinker -Xlinker -Xlinker -LIBPATH:C:/Users/Tal/Desktop/Plant\ Health\ +/Plant-Health-Plus/PlantHealthPlus/vcpkg_installed/x64-windows/debug/lib -Xlinker -Xlinker -Xlinker -machine:x64 -Xlinker -Xlinker -Xlinker -nologo -Xlinker -Xlinker -Xlinker -debug -Xlinker -Xlinker -Xlinker -INCREMENTAL'
ac_cv_env_LIBS_set=set
ac_cv_env_LIBS_value='-lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -lcomdlg32 -ladvapi32'
ac_cv_env_LT_SYS_LIBRARY_PATH_set=
ac_cv_env_LT_SYS_LIBRARY_PATH_value=
ac_cv_env_build_alias_set=set
ac_cv_env_build_alias_value=x86_64-pc-mingw32
ac_cv_env_host_alias_set=
ac_cv_env_host_alias_value=
ac_cv_env_target_alias_set=
ac_cv_env_target_alias_value=
ac_cv_func_memmove=yes
ac_cv_path_install='/usr/bin/install -c'
ac_cv_path_mkdir=/usr/bin/mkdir
ac_cv_prog_AWK=gawk
ac_cv_prog_ac_ct_CC='compile cl.exe'
ac_cv_prog_ac_ct_STRIP=:
ac_cv_prog_make_make_set=yes
am_cv_filesystem_timestamp_resolution=0.01
am_cv_make_support_nested_variables=yes
am_cv_sleep_fractional_seconds=yes
am_cv_xargs_n_works=yes
gl_cv_double_slash_root=yes

## ----------------- ##
## Output variables. ##
## ----------------- ##

ACLOCAL='${SHELL} '\''/c/Users/Tal/Desktop/Plant Health +/Plant-Health-Plus/PlantHealthPlus/vcpkg/buildtrees/libiconv/src/v1.18-8006e03beb.clean/build-aux/missing'\'' aclocal-1.17'
ALLOCA=''
ALLOCA_H=''
AMDEPBACKSLASH=''
AMDEP_FALSE=''
AMDEP_TRUE=''
AMTAR='$${TAR-tar}'
AM_BACKSLASH='\'
AM_DEFAULT_V=''
AM_DEFAULT_VERBOSITY='1'
AM_V=''
APPLE_UNIVERSAL_BUILD=''
AR='ar-lib lib.exe'
ARFLAGS=''
AS=':'
ASM_SYMBOL_PREFIX=''
ASSERT_H=''
AUTOCONF='${SHELL} '\''/c/Users/Tal/Desktop/Plant Health +/Plant-Health-Plus/PlantHealthPlus/vcpkg/buildtrees/libiconv/src/v1.18-8006e03beb.clean/build-aux/missing'\'' autoconf'
AUTOHEADER='${SHELL} '\''/c/Users/Tal/Desktop/Plant Health +/Plant-Health-Plus/PlantHealthPlus/vcpkg/buildtrees/libiconv/src/v1.18-8006e03beb.clean/build-aux/missing'\'' autoheader'
AUTOMAKE='${SHELL} '\''/c/Users/Tal/Desktop/Plant Health +/Plant-Health-Plus/PlantHealthPlus/vcpkg/buildtrees/libiconv/src/v1.18-8006e03beb.clean/build-aux/missing'\'' automake-1.17'
AWK='gawk'
BITSIZEOF_PTRDIFF_T=''
BITSIZEOF_SIG_ATOMIC_T=''
BITSIZEOF_SIZE_T=''
BITSIZEOF_WCHAR_T=''
BITSIZEOF_WINT_T=''
BROKEN_WCHAR_H=''
CAN_PRINT_STACK_TRACE=''
CC='compile cl.exe'
CCDEPMODE=''
CFLAGS='-Xcompiler -nologo -Xcompiler -utf-8 -Xcompiler -MP -Xcompiler -MDd -Xcompiler -Z7 -Xcompiler -Ob0 -Xcompiler -Od -Xcompiler -RTC1'
CFLAG_VISIBILITY=''
CP=''
CPP='compile cl.exe -E'
CPPFLAGS='-DWIN32 -D_WINDOWS'
CSCOPE='cscope'
CTAGS='ctags'
CYGPATH_W='cygpath -w'
DEFS=''
DEPDIR=''
DLLTOOL='link.exe -verbose -dll'
DLL_VARIABLE=''
DSYMUTIL=''
DUMPBIN=''
ECHO_C=''
ECHO_N='-n'
ECHO_T=''
EGREP=''
EILSEQ=''
EMULTIHOP_HIDDEN=''
EMULTIHOP_VALUE=''
ENOLINK_HIDDEN=''
ENOLINK_VALUE=''
EOVERFLOW_HIDDEN=''
EOVERFLOW_VALUE=''
ERRNO_H=''
ETAGS='etags'
EXEEXT=''
FGREP=''
FILECMD=''
GETTEXT_MACRO_VERSION=''
GL_CFLAG_ALLOW_WARNINGS=''
GL_CFLAG_GNULIB_WARNINGS=''
GL_COND_LIBTOOL_FALSE=''
GL_COND_LIBTOOL_TRUE=''
GL_COND_OBJ_CANONICALIZE_LGPL_FALSE=''
GL_COND_OBJ_CANONICALIZE_LGPL_TRUE=''
GL_COND_OBJ_CLOSE_FALSE=''
GL_COND_OBJ_CLOSE_TRUE=''
GL_COND_OBJ_DUP2_FALSE=''
GL_COND_OBJ_DUP2_TRUE=''
GL_COND_OBJ_ERROR_FALSE=''
GL_COND_OBJ_ERROR_TRUE=''
GL_COND_OBJ_FCNTL_FALSE=''
GL_COND_OBJ_FCNTL_TRUE=''
GL_COND_OBJ_FREE_FALSE=''
GL_COND_OBJ_FREE_TRUE=''
GL_COND_OBJ_FSTAT_FALSE=''
GL_COND_OBJ_FSTAT_TRUE=''
GL_COND_OBJ_GETDTABLESIZE_FALSE=''
GL_COND_OBJ_GETDTABLESIZE_TRUE=''
GL_COND_OBJ_GETPROGNAME_FALSE=''
GL_COND_OBJ_GETPROGNAME_TRUE=''
GL_COND_OBJ_MEMPCPY_FALSE=''
GL_COND_OBJ_MEMPCPY_TRUE=''
GL_COND_OBJ_MSVC_INVAL_FALSE=''
GL_COND_OBJ_MSVC_INVAL_TRUE=''
GL_COND_OBJ_MSVC_NOTHROW_FALSE=''
GL_COND_OBJ_MSVC_NOTHROW_TRUE=''
GL_COND_OBJ_OPEN_FALSE=''
GL_COND_OBJ_OPEN_TRUE=''
GL_COND_OBJ_PROGRELOC_FALSE=''
GL_COND_OBJ_PROGRELOC_TRUE=''
GL_COND_OBJ_RAISE_FALSE=''
GL_COND_OBJ_RAISE_TRUE=''
GL_COND_OBJ_RAWMEMCHR_FALSE=''
GL_COND_OBJ_RAWMEMCHR_TRUE=''
GL_COND_OBJ_READLINK_FALSE=''
GL_COND_OBJ_READLINK_TRUE=''
GL_COND_OBJ_READ_FALSE=''
GL_COND_OBJ_READ_TRUE=''
GL_COND_OBJ_REALLOC_POSIX_FALSE=''
GL_COND_OBJ_REALLOC_POSIX_TRUE=''
GL_COND_OBJ_SIGPROCMASK_FALSE=''
GL_COND_OBJ_SIGPROCMASK_TRUE=''
GL_COND_OBJ_STAT_FALSE=''
GL_COND_OBJ_STAT_TRUE=''
GL_COND_OBJ_STDIO_READ_FALSE=''
GL_COND_OBJ_STDIO_READ_TRUE=''
GL_COND_OBJ_STDIO_WRITE_FALSE=''
GL_COND_OBJ_STDIO_WRITE_TRUE=''
GL_COND_OBJ_STRERROR_FALSE=''
GL_COND_OBJ_STRERROR_OVERRIDE_FALSE=''
GL_COND_OBJ_STRERROR_OVERRIDE_TRUE=''
GL_COND_OBJ_STRERROR_TRUE=''
GL_GENERATE_ALLOCA_H_FALSE=''
GL_GENERATE_ALLOCA_H_TRUE=''
GL_GENERATE_ASSERT_H_FALSE=''
GL_GENERATE_ASSERT_H_TRUE=''
GL_GENERATE_ERRNO_H_FALSE=''
GL_GENERATE_ERRNO_H_TRUE=''
GL_GENERATE_LIMITS_H_FALSE=''
GL_GENERATE_LIMITS_H_TRUE=''
GL_GENERATE_STDCKDINT_H_FALSE=''
GL_GENERATE_STDCKDINT_H_TRUE=''
GL_GENERATE_STDDEF_H_FALSE=''
GL_GENERATE_STDDEF_H_TRUE=''
GL_GENERATE_STDINT_H_FALSE=''
GL_GENERATE_STDINT_H_TRUE=''
GL_GNULIB_ABORT_DEBUG=''
GL_GNULIB_ACCESS=''
GL_GNULIB_ALIGNED_ALLOC=''
GL_GNULIB_ATOLL=''
GL_GNULIB_BTOWC=''
GL_GNULIB_CALLOC_GNU=''
GL_GNULIB_CALLOC_POSIX=''
GL_GNULIB_CANONICALIZE_FILE_NAME=''
GL_GNULIB_CHDIR=''
GL_GNULIB_CHMOD=''
GL_GNULIB_CHOWN=''
GL_GNULIB_CLOSE=''
GL_GNULIB_COPY_FILE_RANGE=''
GL_GNULIB_CREAT=''
GL_GNULIB_CTIME=''
GL_GNULIB_DPRINTF=''
GL_GNULIB_DUP2=''
GL_GNULIB_DUP3=''
GL_GNULIB_DUP=''
GL_GNULIB_DZPRINTF=''
GL_GNULIB_ENVIRON=''
GL_GNULIB_EUIDACCESS=''
GL_GNULIB_EXECL=''
GL_GNULIB_EXECLE=''
GL_GNULIB_EXECLP=''
GL_GNULIB_EXECV=''
GL_GNULIB_EXECVE=''
GL_GNULIB_EXECVP=''
GL_GNULIB_EXECVPE=''
GL_GNULIB_EXPLICIT_BZERO=''
GL_GNULIB_FACCESSAT=''
GL_GNULIB_FCHDIR=''
GL_GNULIB_FCHMODAT=''
GL_GNULIB_FCHOWNAT=''
GL_GNULIB_FCLOSE=''
GL_GNULIB_FCNTL=''
GL_GNULIB_FDATASYNC=''
GL_GNULIB_FDOPEN=''
GL_GNULIB_FFLUSH=''
GL_GNULIB_FFSL=''
GL_GNULIB_FFSLL=''
GL_GNULIB_FGETC=''
GL_GNULIB_FGETS=''
GL_GNULIB_FOPEN=''
GL_GNULIB_FOPEN_GNU=''
GL_GNULIB_FPRINTF=''
GL_GNULIB_FPRINTF_POSIX=''
GL_GNULIB_FPURGE=''
GL_GNULIB_FPUTC=''
GL_GNULIB_FPUTS=''
GL_GNULIB_FREAD=''
GL_GNULIB_FREE_POSIX=''
GL_GNULIB_FREOPEN=''
GL_GNULIB_FSCANF=''
GL_GNULIB_FSEEK=''
GL_GNULIB_FSEEKO=''
GL_GNULIB_FSTAT=''
GL_GNULIB_FSTATAT=''
GL_GNULIB_FSYNC=''
GL_GNULIB_FTELL=''
GL_GNULIB_FTELLO=''
GL_GNULIB_FTRUNCATE=''
GL_GNULIB_FUTIMENS=''
GL_GNULIB_FWRITE=''
GL_GNULIB_FZPRINTF=''
GL_GNULIB_GETC=''
GL_GNULIB_GETCHAR=''
GL_GNULIB_GETCWD=''
GL_GNULIB_GETDELIM=''
GL_GNULIB_GETDOMAINNAME=''
GL_GNULIB_GETDTABLESIZE=''
GL_GNULIB_GETENTROPY=''
GL_GNULIB_GETGROUPS=''
GL_GNULIB_GETHOSTNAME=''
GL_GNULIB_GETLINE=''
GL_GNULIB_GETLOADAVG=''
GL_GNULIB_GETLOGIN=''
GL_GNULIB_GETLOGIN_R=''
GL_GNULIB_GETOPT_POSIX=''
GL_GNULIB_GETPAGESIZE=''
GL_GNULIB_GETPASS=''
GL_GNULIB_GETPASS_GNU=''
GL_GNULIB_GETPROGNAME=''
GL_GNULIB_GETSUBOPT=''
GL_GNULIB_GETUMASK=''
GL_GNULIB_GETUSERSHELL=''
GL_GNULIB_GRANTPT=''
GL_GNULIB_GROUP_MEMBER=''
GL_GNULIB_IMAXABS=''
GL_GNULIB_IMAXDIV=''
GL_GNULIB_ISATTY=''
GL_GNULIB_LCHMOD=''
GL_GNULIB_LCHOWN=''
GL_GNULIB_LINK=''
GL_GNULIB_LINKAT=''
GL_GNULIB_LOCALTIME=''
GL_GNULIB_LSEEK=''
GL_GNULIB_LSTAT=''
GL_GNULIB_MALLOC_GNU=''
GL_GNULIB_MALLOC_POSIX=''
GL_GNULIB_MBRLEN=''
GL_GNULIB_MBRTOWC=''
GL_GNULIB_MBSCASECMP=''
GL_GNULIB_MBSCASESTR=''
GL_GNULIB_MBSCHR=''
GL_GNULIB_MBSCSPN=''
GL_GNULIB_MBSINIT=''
GL_GNULIB_MBSLEN=''
GL_GNULIB_MBSNCASECMP=''
GL_GNULIB_MBSNLEN=''
GL_GNULIB_MBSNRTOWCS=''
GL_GNULIB_MBSPBRK=''
GL_GNULIB_MBSPCASECMP=''
GL_GNULIB_MBSRCHR=''
GL_GNULIB_MBSRTOWCS=''
GL_GNULIB_MBSSEP=''
GL_GNULIB_MBSSPN=''
GL_GNULIB_MBSSTR=''
GL_GNULIB_MBSTOK_R=''
GL_GNULIB_MBSTOWCS=''
GL_GNULIB_MBSZERO=''
GL_GNULIB_MBTOWC=''
GL_GNULIB_MDA_ACCESS=''
GL_GNULIB_MDA_CHDIR=''
GL_GNULIB_MDA_CHMOD=''
GL_GNULIB_MDA_CLOSE=''
GL_GNULIB_MDA_CREAT=''
GL_GNULIB_MDA_DUP2=''
GL_GNULIB_MDA_DUP=''
GL_GNULIB_MDA_ECVT=''
GL_GNULIB_MDA_EXECL=''
GL_GNULIB_MDA_EXECLE=''
GL_GNULIB_MDA_EXECLP=''
GL_GNULIB_MDA_EXECV=''
GL_GNULIB_MDA_EXECVE=''
GL_GNULIB_MDA_EXECVP=''
GL_GNULIB_MDA_EXECVPE=''
GL_GNULIB_MDA_FCLOSEALL=''
GL_GNULIB_MDA_FCVT=''
GL_GNULIB_MDA_FDOPEN=''
GL_GNULIB_MDA_FILENO=''
GL_GNULIB_MDA_GCVT=''
GL_GNULIB_MDA_GETCWD=''
GL_GNULIB_MDA_GETPID=''
GL_GNULIB_MDA_GETW=''
GL_GNULIB_MDA_ISATTY=''
GL_GNULIB_MDA_LSEEK=''
GL_GNULIB_MDA_MEMCCPY=''
GL_GNULIB_MDA_MKDIR=''
GL_GNULIB_MDA_MKTEMP=''
GL_GNULIB_MDA_OPEN=''
GL_GNULIB_MDA_PUTENV=''
GL_GNULIB_MDA_PUTW=''
GL_GNULIB_MDA_READ=''
GL_GNULIB_MDA_RMDIR=''
GL_GNULIB_MDA_STRDUP=''
GL_GNULIB_MDA_SWAB=''
GL_GNULIB_MDA_TEMPNAM=''
GL_GNULIB_MDA_TZSET=''
GL_GNULIB_MDA_UMASK=''
GL_GNULIB_MDA_UNLINK=''
GL_GNULIB_MDA_WCSDUP=''
GL_GNULIB_MDA_WRITE=''
GL_GNULIB_MEMCHR=''
GL_GNULIB_MEMMEM=''
GL_GNULIB_MEMPCPY=''
GL_GNULIB_MEMRCHR=''
GL_GNULIB_MEMSET_EXPLICIT=''
GL_GNULIB_MKDIR=''
GL_GNULIB_MKDIRAT=''
GL_GNULIB_MKDTEMP=''
GL_GNULIB_MKFIFO=''
GL_GNULIB_MKFIFOAT=''
GL_GNULIB_MKNOD=''
GL_GNULIB_MKNODAT=''
GL_GNULIB_MKOSTEMP=''
GL_GNULIB_MKOSTEMPS=''
GL_GNULIB_MKSTEMP=''
GL_GNULIB_MKSTEMPS=''
GL_GNULIB_MKTIME=''
GL_GNULIB_NANOSLEEP=''
GL_GNULIB_NONBLOCKING=''
GL_GNULIB_OBSTACK_PRINTF=''
GL_GNULIB_OBSTACK_PRINTF_POSIX=''
GL_GNULIB_OBSTACK_ZPRINTF=''
GL_GNULIB_OPEN=''
GL_GNULIB_OPENAT=''
GL_GNULIB_OVERRIDES_STRUCT_STAT=''
GL_GNULIB_PCLOSE=''
GL_GNULIB_PERROR=''
GL_GNULIB_PIPE2=''
GL_GNULIB_PIPE=''
GL_GNULIB_POPEN=''
GL_GNULIB_POSIX_MEMALIGN=''
GL_GNULIB_POSIX_OPENPT=''
GL_GNULIB_PREAD=''
GL_GNULIB_PRINTF=''
GL_GNULIB_PRINTF_POSIX=''
GL_GNULIB_PTHREAD_SIGMASK=''
GL_GNULIB_PTSNAME=''
GL_GNULIB_PTSNAME_R=''
GL_GNULIB_PUTC=''
GL_GNULIB_PUTCHAR=''
GL_GNULIB_PUTENV=''
GL_GNULIB_PUTS=''
GL_GNULIB_PWRITE=''
GL_GNULIB_QSORT_R=''
GL_GNULIB_RAISE=''
GL_GNULIB_RAND=''
GL_GNULIB_RANDOM=''
GL_GNULIB_RANDOM_R=''
GL_GNULIB_RAWMEMCHR=''
GL_GNULIB_READ=''
GL_GNULIB_READLINK=''
GL_GNULIB_READLINKAT=''
GL_GNULIB_REALLOCARRAY=''
GL_GNULIB_REALLOC_POSIX=''
GL_GNULIB_REALPATH=''
GL_GNULIB_REMOVE=''
GL_GNULIB_RENAME=''
GL_GNULIB_RENAMEAT=''
GL_GNULIB_RMDIR=''
GL_GNULIB_RPMATCH=''
GL_GNULIB_SCANF=''
GL_GNULIB_SECURE_GETENV=''
GL_GNULIB_SETENV=''
GL_GNULIB_SETHOSTNAME=''
GL_GNULIB_SIG2STR=''
GL_GNULIB_SIGABBREV_NP=''
GL_GNULIB_SIGACTION=''
GL_GNULIB_SIGDESCR_NP=''
GL_GNULIB_SIGNAL_H_SIGPIPE=''
GL_GNULIB_SIGPROCMASK=''
GL_GNULIB_SLEEP=''
GL_GNULIB_SNPRINTF=''
GL_GNULIB_SNZPRINTF=''
GL_GNULIB_SPRINTF_POSIX=''
GL_GNULIB_STACK_TRACE=''
GL_GNULIB_STAT=''
GL_GNULIB_STDIO_H_NONBLOCKING=''
GL_GNULIB_STDIO_H_SIGPIPE=''
GL_GNULIB_STPCPY=''
GL_GNULIB_STPNCPY=''
GL_GNULIB_STRCASESTR=''
GL_GNULIB_STRCHRNUL=''
GL_GNULIB_STRDUP=''
GL_GNULIB_STRERROR=''
GL_GNULIB_STRERRORNAME_NP=''
GL_GNULIB_STRERROR_R=''
GL_GNULIB_STRFTIME=''
GL_GNULIB_STRNCAT=''
GL_GNULIB_STRNDUP=''
GL_GNULIB_STRNLEN=''
GL_GNULIB_STRPBRK=''
GL_GNULIB_STRPTIME=''
GL_GNULIB_STRSEP=''
GL_GNULIB_STRSIGNAL=''
GL_GNULIB_STRSTR=''
GL_GNULIB_STRTOD=''
GL_GNULIB_STRTOF=''
GL_GNULIB_STRTOIMAX=''
GL_GNULIB_STRTOK_R=''
GL_GNULIB_STRTOL=''
GL_GNULIB_STRTOLD=''
GL_GNULIB_STRTOLL=''
GL_GNULIB_STRTOUL=''
GL_GNULIB_STRTOULL=''
GL_GNULIB_STRTOUMAX=''
GL_GNULIB_STRVERSCMP=''
GL_GNULIB_SYMLINK=''
GL_GNULIB_SYMLINKAT=''
GL_GNULIB_SYSTEM_POSIX=''
GL_GNULIB_SZPRINTF=''
GL_GNULIB_TIME=''
GL_GNULIB_TIMEGM=''
GL_GNULIB_TIMESPEC_GET=''
GL_GNULIB_TIMESPEC_GETRES=''
GL_GNULIB_TIME_R=''
GL_GNULIB_TIME_RZ=''
GL_GNULIB_TMPFILE=''
GL_GNULIB_TRUNCATE=''
GL_GNULIB_TTYNAME_R=''
GL_GNULIB_TZNAME=''
GL_GNULIB_TZSET=''
GL_GNULIB_UNISTD_H_GETOPT=''
GL_GNULIB_UNISTD_H_NONBLOCKING=''
GL_GNULIB_UNISTD_H_SIGPIPE=''
GL_GNULIB_UNLINK=''
GL_GNULIB_UNLINKAT=''
GL_GNULIB_UNLOCKPT=''
GL_GNULIB_UNSETENV=''
GL_GNULIB_USLEEP=''
GL_GNULIB_UTIMENSAT=''
GL_GNULIB_VASPRINTF=''
GL_GNULIB_VASZPRINTF=''
GL_GNULIB_VDPRINTF=''
GL_GNULIB_VDZPRINTF=''
GL_GNULIB_VFPRINTF=''
GL_GNULIB_VFPRINTF_POSIX=''
GL_GNULIB_VFSCANF=''
GL_GNULIB_VFZPRINTF=''
GL_GNULIB_VPRINTF=''
GL_GNULIB_VPRINTF_POSIX=''
GL_GNULIB_VSCANF=''
GL_GNULIB_VSNPRINTF=''
GL_GNULIB_VSNZPRINTF=''
GL_GNULIB_VSPRINTF_POSIX=''
GL_GNULIB_VSZPRINTF=''
GL_GNULIB_VZPRINTF=''
GL_GNULIB_WCPCPY=''
GL_GNULIB_WCPNCPY=''
GL_GNULIB_WCRTOMB=''
GL_GNULIB_WCSCASECMP=''
GL_GNULIB_WCSCAT=''
GL_GNULIB_WCSCHR=''
GL_GNULIB_WCSCMP=''
GL_GNULIB_WCSCOLL=''
GL_GNULIB_WCSCPY=''
GL_GNULIB_WCSCSPN=''
GL_GNULIB_WCSDUP=''
GL_GNULIB_WCSFTIME=''
GL_GNULIB_WCSLEN=''
GL_GNULIB_WCSNCASECMP=''
GL_GNULIB_WCSNCAT=''
GL_GNULIB_WCSNCMP=''
GL_GNULIB_WCSNCPY=''
GL_GNULIB_WCSNLEN=''
GL_GNULIB_WCSNRTOMBS=''
GL_GNULIB_WCSPBRK=''
GL_GNULIB_WCSRCHR=''
GL_GNULIB_WCSRTOMBS=''
GL_GNULIB_WCSSPN=''
GL_GNULIB_WCSSTR=''
GL_GNULIB_WCSTOK=''
GL_GNULIB_WCSWIDTH=''
GL_GNULIB_WCSXFRM=''
GL_GNULIB_WCTOB=''
GL_GNULIB_WCTOMB=''
GL_GNULIB_WCWIDTH=''
GL_GNULIB_WGETCWD=''
GL_GNULIB_WMEMCHR=''
GL_GNULIB_WMEMCMP=''
GL_GNULIB_WMEMCPY=''
GL_GNULIB_WMEMMOVE=''
GL_GNULIB_WMEMPCPY=''
GL_GNULIB_WMEMSET=''
GL_GNULIB_WRITE=''
GL_GNULIB_ZPRINTF=''
GL_GNULIB__EXIT=''
GMSGFMT=''
GMSGFMT_015=''
GNULIBHEADERS_OVERRIDE_WINT_T=''
GNULIB_LOCALEDIR=''
GNULIB_LOCALEDIR_c=''
GNULIB_LOCALEDIR_c_make=''
GREP=''
HAVE_ALIGNED_ALLOC=''
HAVE_ATOLL=''
HAVE_BTOWC=''
HAVE_C99_STDINT_H=''
HAVE_CANONICALIZE_FILE_NAME=''
HAVE_CHOWN=''
HAVE_COPY_FILE_RANGE=''
HAVE_CRTDEFS_H=''
HAVE_DECL_ECVT=''
HAVE_DECL_ENVIRON=''
HAVE_DECL_EXECVPE=''
HAVE_DECL_FCHDIR=''
HAVE_DECL_FCLOSEALL=''
HAVE_DECL_FCVT=''
HAVE_DECL_FDATASYNC=''
HAVE_DECL_FPURGE=''
HAVE_DECL_FSEEKO=''
HAVE_DECL_FTELLO=''
HAVE_DECL_GCVT=''
HAVE_DECL_GETDELIM=''
HAVE_DECL_GETDOMAINNAME=''
HAVE_DECL_GETLINE=''
HAVE_DECL_GETLOADAVG=''
HAVE_DECL_GETLOGIN=''
HAVE_DECL_GETLOGIN_R=''
HAVE_DECL_GETPAGESIZE=''
HAVE_DECL_GETUSERSHELL=''
HAVE_DECL_GETW=''
HAVE_DECL_IMAXABS=''
HAVE_DECL_IMAXDIV=''
HAVE_DECL_INITSTATE=''
HAVE_DECL_LOCALTIME_R=''
HAVE_DECL_MEMMEM=''
HAVE_DECL_MEMRCHR=''
HAVE_DECL_OBSTACK_PRINTF=''
HAVE_DECL_PROGRAM_INVOCATION_NAME=''
HAVE_DECL_PUTW=''
HAVE_DECL_SETENV=''
HAVE_DECL_SETHOSTNAME=''
HAVE_DECL_SETSTATE=''
HAVE_DECL_SNPRINTF=''
HAVE_DECL_STRDUP=''
HAVE_DECL_STRERROR_R=''
HAVE_DECL_STRNDUP=''
HAVE_DECL_STRNLEN=''
HAVE_DECL_STRSIGNAL=''
HAVE_DECL_STRTOIMAX=''
HAVE_DECL_STRTOK_R=''
HAVE_DECL_STRTOUMAX=''
HAVE_DECL_TRUNCATE=''
HAVE_DECL_TTYNAME_R=''
HAVE_DECL_UNSETENV=''
HAVE_DECL_VSNPRINTF=''
HAVE_DECL_WCSDUP=''
HAVE_DECL_WCTOB=''
HAVE_DECL_WCWIDTH=''
HAVE_DPRINTF=''
HAVE_DUP3=''
HAVE_ERROR=''
HAVE_ERROR_AT_LINE=''
HAVE_ERROR_H=''
HAVE_EUIDACCESS=''
HAVE_EXECVPE=''
HAVE_EXPLICIT_BZERO=''
HAVE_FACCESSAT=''
HAVE_FCHDIR=''
HAVE_FCHMODAT=''
HAVE_FCHOWNAT=''
HAVE_FCNTL=''
HAVE_FDATASYNC=''
HAVE_FEATURES_H=''
HAVE_FFSL=''
HAVE_FFSLL=''
HAVE_FSEEKO=''
HAVE_FSTATAT=''
HAVE_FSYNC=''
HAVE_FTELLO=''
HAVE_FTRUNCATE=''
HAVE_FUTIMENS=''
HAVE_GETDTABLESIZE=''
HAVE_GETENTROPY=''
HAVE_GETGROUPS=''
HAVE_GETHOSTNAME=''
HAVE_GETLOGIN=''
HAVE_GETPAGESIZE=''
HAVE_GETPASS=''
HAVE_GETPROGNAME=''
HAVE_GETSUBOPT=''
HAVE_GETUMASK=''
HAVE_GRANTPT=''
HAVE_GROUP_MEMBER=''
HAVE_IMAXABS=''
HAVE_IMAXDIV=''
HAVE_IMAXDIV_T=''
HAVE_INITSTATE=''
HAVE_INTTYPES_H=''
HAVE_LCHMOD=''
HAVE_LCHOWN=''
HAVE_LINK=''
HAVE_LINKAT=''
HAVE_LSTAT=''
HAVE_MAX_ALIGN_T=''
HAVE_MBRLEN=''
HAVE_MBRTOWC=''
HAVE_MBSINIT=''
HAVE_MBSLEN=''
HAVE_MBSNRTOWCS=''
HAVE_MBSRTOWCS=''
HAVE_MBTOWC=''
HAVE_MEMPCPY=''
HAVE_MEMSET_EXPLICIT=''
HAVE_MKDIRAT=''
HAVE_MKDTEMP=''
HAVE_MKFIFO=''
HAVE_MKFIFOAT=''
HAVE_MKNOD=''
HAVE_MKNODAT=''
HAVE_MKOSTEMP=''
HAVE_MKOSTEMPS=''
HAVE_MKSTEMP=''
HAVE_MKSTEMPS=''
HAVE_MSVC_INVALID_PARAMETER_HANDLER=''
HAVE_NANOSLEEP=''
HAVE_OFF64_T=''
HAVE_OPENAT=''
HAVE_OS_H=''
HAVE_PCLOSE=''
HAVE_PIPE2=''
HAVE_PIPE=''
HAVE_POPEN=''
HAVE_POSIX_MEMALIGN=''
HAVE_POSIX_OPENPT=''
HAVE_POSIX_SIGNALBLOCKING=''
HAVE_PREAD=''
HAVE_PTHREAD_SIGMASK=''
HAVE_PTSNAME=''
HAVE_PTSNAME_R=''
HAVE_PWRITE=''
HAVE_QSORT_R=''
HAVE_RAISE=''
HAVE_RANDOM=''
HAVE_RANDOM_H=''
HAVE_RANDOM_R=''
HAVE_RAWMEMCHR=''
HAVE_READLINK=''
HAVE_READLINKAT=''
HAVE_REALLOCARRAY=''
HAVE_REALPATH=''
HAVE_RENAMEAT=''
HAVE_RPMATCH=''
HAVE_SECURE_GETENV=''
HAVE_SETENV=''
HAVE_SETHOSTNAME=''
HAVE_SETSTATE=''
HAVE_SIG2STR=''
HAVE_SIGABBREV_NP=''
HAVE_SIGACTION=''
HAVE_SIGDESCR_NP=''
HAVE_SIGHANDLER_T=''
HAVE_SIGINFO_T=''
HAVE_SIGNED_SIG_ATOMIC_T=''
HAVE_SIGNED_WCHAR_T=''
HAVE_SIGNED_WINT_T=''
HAVE_SIGSET_T=''
HAVE_SLEEP=''
HAVE_STDINT_H=''
HAVE_STPCPY=''
HAVE_STPNCPY=''
HAVE_STR2SIG=''
HAVE_STRCASESTR=''
HAVE_STRCHRNUL=''
HAVE_STRERRORNAME_NP=''
HAVE_STRPBRK=''
HAVE_STRPTIME=''
HAVE_STRSEP=''
HAVE_STRTOD=''
HAVE_STRTOF=''
HAVE_STRTOL=''
HAVE_STRTOLD=''
HAVE_STRTOLL=''
HAVE_STRTOUL=''
HAVE_STRTOULL=''
HAVE_STRUCT_RANDOM_DATA=''
HAVE_STRUCT_SIGACTION_SA_SIGACTION=''
HAVE_STRVERSCMP=''
HAVE_SYMLINK=''
HAVE_SYMLINKAT=''
HAVE_SYS_BITYPES_H=''
HAVE_SYS_INTTYPES_H=''
HAVE_SYS_LOADAVG_H=''
HAVE_SYS_PARAM_H=''
HAVE_SYS_TYPES_H=''
HAVE_TIMEGM=''
HAVE_TIMESPEC_GET=''
HAVE_TIMESPEC_GETRES=''
HAVE_TIMEZONE_T=''
HAVE_TYPE_VOLATILE_SIG_ATOMIC_T=''
HAVE_TZALLOC=''
HAVE_UNISTD_H=''
HAVE_UNISTRING_WOE32DLL_H=''
HAVE_UNLINKAT=''
HAVE_UNLOCKPT=''
HAVE_USLEEP=''
HAVE_UTIMENSAT=''
HAVE_VASPRINTF=''
HAVE_VDPRINTF=''
HAVE_VISIBILITY=''
HAVE_WCHAR_H=''
HAVE_WCPCPY=''
HAVE_WCPNCPY=''
HAVE_WCRTOMB=''
HAVE_WCSCASECMP=''
HAVE_WCSCAT=''
HAVE_WCSCHR=''
HAVE_WCSCMP=''
HAVE_WCSCOLL=''
HAVE_WCSCPY=''
HAVE_WCSCSPN=''
HAVE_WCSDUP=''
HAVE_WCSFTIME=''
HAVE_WCSLEN=''
HAVE_WCSNCASECMP=''
HAVE_WCSNCAT=''
HAVE_WCSNCMP=''
HAVE_WCSNCPY=''
HAVE_WCSNLEN=''
HAVE_WCSNRTOMBS=''
HAVE_WCSPBRK=''
HAVE_WCSRCHR=''
HAVE_WCSRTOMBS=''
HAVE_WCSSPN=''
HAVE_WCSSTR=''
HAVE_WCSTOK=''
HAVE_WCSWIDTH=''
HAVE_WCSXFRM=''
HAVE_WINSOCK2_H=''
HAVE_WINT_T=''
HAVE_WMEMCHR=''
HAVE_WMEMCMP=''
HAVE_WMEMCPY=''
HAVE_WMEMMOVE=''
HAVE_WMEMPCPY=''
HAVE_WMEMSET=''
HAVE__EXIT=''
ICONV_CONST=''
INCLUDE_NEXT=''
INCLUDE_NEXT_AS_FIRST_DIRECTIVE=''
INSTALL_DATA='${INSTALL} -m 644'
INSTALL_PROGRAM='${INSTALL}'
INSTALL_PROGRAM_ENV=''
INSTALL_SCRIPT='${INSTALL}'
INSTALL_STRIP_PROGRAM='$(install_sh) -c -s'
INT32_MAX_LT_INTMAX_MAX=''
INT64_MAX_EQ_LONG_MAX=''
INTLLIBS=''
INTL_MACOSX_LIBS=''
LD='link.exe -verbose'
LDFLAGS='-Xlinker -Xlinker -Xlinker -LIBPATH:C:/Users/Tal/Desktop/Plant\ Health\ +/Plant-Health-Plus/PlantHealthPlus/vcpkg_installed/x64-windows/debug/lib -Xlinker -Xlinker -Xlinker -machine:x64 -Xlinker -Xlinker -Xlinker -nologo -Xlinker -Xlinker -Xlinker -debug -Xlinker -Xlinker -Xlinker -INCREMENTAL'
LIBICONV=''
LIBICRT_LIBDEPS=''
LIBICRT_LTLIBDEPS=''
LIBINTL=''
LIBOBJS=''
LIBS='-lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -lcomdlg32 -ladvapi32'
LIBTOOL=''
LIBUNISTRING_COMPILE_UNIWIDTH_WIDTH_FALSE=''
LIBUNISTRING_COMPILE_UNIWIDTH_WIDTH_TRUE=''
LIBUNISTRING_UNITYPES_H=''
LIBUNISTRING_UNIWIDTH_H=''
LIMITS_H=''
LIPO=''
LN=''
LN_S=''
LOCALE_EN_UTF8=''
LTLIBICONV=''
LTLIBINTL=''
LTLIBOBJS=''
LT_SYS_LIBRARY_PATH=''
MAKEINFO='${SHELL} '\''/c/Users/Tal/Desktop/Plant Health +/Plant-Health-Plus/PlantHealthPlus/vcpkg/buildtrees/libiconv/src/v1.18-8006e03beb.clean/build-aux/missing'\'' makeinfo'
MANIFEST_TOOL=''
MKDIR_P='/usr/bin/mkdir -p'
MSGFMT=''
MSGMERGE=''
MSGMERGE_FOR_MSGFMT_OPTION=''
NEXT_ASSERT_H=''
NEXT_AS_FIRST_DIRECTIVE_ASSERT_H=''
NEXT_AS_FIRST_DIRECTIVE_ERRNO_H=''
NEXT_AS_FIRST_DIRECTIVE_ERROR_H=''
NEXT_AS_FIRST_DIRECTIVE_FCNTL_H=''
NEXT_AS_FIRST_DIRECTIVE_INTTYPES_H=''
NEXT_AS_FIRST_DIRECTIVE_LIMITS_H=''
NEXT_AS_FIRST_DIRECTIVE_SIGNAL_H=''
NEXT_AS_FIRST_DIRECTIVE_STDDEF_H=''
NEXT_AS_FIRST_DIRECTIVE_STDINT_H=''
NEXT_AS_FIRST_DIRECTIVE_STDIO_H=''
NEXT_AS_FIRST_DIRECTIVE_STDLIB_H=''
NEXT_AS_FIRST_DIRECTIVE_STRING_H=''
NEXT_AS_FIRST_DIRECTIVE_SYS_STAT_H=''
NEXT_AS_FIRST_DIRECTIVE_SYS_TYPES_H=''
NEXT_AS_FIRST_DIRECTIVE_TIME_H=''
NEXT_AS_FIRST_DIRECTIVE_UNISTD_H=''
NEXT_AS_FIRST_DIRECTIVE_WCHAR_H=''
NEXT_ERRNO_H=''
NEXT_ERROR_H=''
NEXT_FCNTL_H=''
NEXT_INTTYPES_H=''
NEXT_LIMITS_H=''
NEXT_SIGNAL_H=''
NEXT_STDDEF_H=''
NEXT_STDINT_H=''
NEXT_STDIO_H=''
NEXT_STDLIB_H=''
NEXT_STRING_H=''
NEXT_SYS_STAT_H=''
NEXT_SYS_TYPES_H=''
NEXT_TIME_H=''
NEXT_UNISTD_H=''
NEXT_WCHAR_H=''
NM='dumpbin.exe -symbols -headers'
NMEDIT=''
NULLPTR_T_NEEDS_STDDEF=''
OBJDUMP=''
OBJEXT=''
OTOOL64=''
OTOOL=''
PACKAGE='libiconv'
PACKAGE_BUGREPORT=''
PACKAGE_NAME='libiconv'
PACKAGE_STRING='libiconv 1.18'
PACKAGE_TARNAME='libiconv'
PACKAGE_URL=''
PACKAGE_VERSION='1.18'
PATH_SEPARATOR=':'
POSUB=''
PRAGMA_COLUMNS=''
PRAGMA_SYSTEM_HEADER=''
PRIPTR_PREFIX=''
PTHREAD_H_DEFINES_STRUCT_TIMESPEC=''
PTRDIFF_T_SUFFIX=''
RANLIB=':'
RC='windres-rc rc.exe'
RELOCATABLE=''
RELOCATABLE_BUILD_DIR=''
RELOCATABLE_CONFIG_H_DIR=''
RELOCATABLE_LDFLAGS=''
RELOCATABLE_LIBRARY_PATH=''
RELOCATABLE_SRC_DIR=''
RELOCATABLE_STRIP=''
RELOCATABLE_VIA_LD_FALSE=''
RELOCATABLE_VIA_LD_TRUE=''
RELOCATABLE_VIA_WRAPPER_FALSE=''
RELOCATABLE_VIA_WRAPPER_TRUE=''
REPLACE_ABORT=''
REPLACE_ACCESS=''
REPLACE_ALIGNED_ALLOC=''
REPLACE_BTOWC=''
REPLACE_CALLOC_FOR_CALLOC_GNU=''
REPLACE_CALLOC_FOR_CALLOC_POSIX=''
REPLACE_CANONICALIZE_FILE_NAME=''
REPLACE_CHMOD=''
REPLACE_CHOWN=''
REPLACE_CLOSE=''
REPLACE_COPY_FILE_RANGE=''
REPLACE_CREAT=''
REPLACE_CTIME=''
REPLACE_DPRINTF=''
REPLACE_DUP2=''
REPLACE_DUP3=''
REPLACE_DUP=''
REPLACE_ERROR=''
REPLACE_ERROR_AT_LINE=''
REPLACE_EXECL=''
REPLACE_EXECLE=''
REPLACE_EXECLP=''
REPLACE_EXECV=''
REPLACE_EXECVE=''
REPLACE_EXECVP=''
REPLACE_EXECVPE=''
REPLACE_FACCESSAT=''
REPLACE_FCHDIR=''
REPLACE_FCHMODAT=''
REPLACE_FCHOWNAT=''
REPLACE_FCLOSE=''
REPLACE_FCNTL=''
REPLACE_FDATASYNC=''
REPLACE_FDOPEN=''
REPLACE_FFLUSH=''
REPLACE_FFSLL=''
REPLACE_FOPEN=''
REPLACE_FOPEN_FOR_FOPEN_GNU=''
REPLACE_FPRINTF=''
REPLACE_FPURGE=''
REPLACE_FREE=''
REPLACE_FREOPEN=''
REPLACE_FSEEK=''
REPLACE_FSEEKO=''
REPLACE_FSTAT=''
REPLACE_FSTATAT=''
REPLACE_FTELL=''
REPLACE_FTELLO=''
REPLACE_FTRUNCATE=''
REPLACE_FUTIMENS=''
REPLACE_GETCWD=''
REPLACE_GETDELIM=''
REPLACE_GETDOMAINNAME=''
REPLACE_GETDTABLESIZE=''
REPLACE_GETENTROPY=''
REPLACE_GETGROUPS=''
REPLACE_GETLINE=''
REPLACE_GETLOADAVG=''
REPLACE_GETLOGIN_R=''
REPLACE_GETPAGESIZE=''
REPLACE_GETPASS=''
REPLACE_GETPASS_FOR_GETPASS_GNU=''
REPLACE_GETPROGNAME=''
REPLACE_GETSUBOPT=''
REPLACE_GETUSERSHELL=''
REPLACE_GMTIME=''
REPLACE_IMAXABS=''
REPLACE_IMAXDIV=''
REPLACE_INITSTATE=''
REPLACE_ISATTY=''
REPLACE_LCHOWN=''
REPLACE_LINK=''
REPLACE_LINKAT=''
REPLACE_LOCALTIME=''
REPLACE_LOCALTIME_R=''
REPLACE_LOCALTIME_RZ=''
REPLACE_LSEEK=''
REPLACE_LSTAT=''
REPLACE_MALLOC_FOR_MALLOC_GNU=''
REPLACE_MALLOC_FOR_MALLOC_POSIX=''
REPLACE_MBRLEN=''
REPLACE_MBRTOWC=''
REPLACE_MBSINIT=''
REPLACE_MBSNRTOWCS=''
REPLACE_MBSRTOWCS=''
REPLACE_MBSTATE_T=''
REPLACE_MBSTOWCS=''
REPLACE_MBTOWC=''
REPLACE_MB_CUR_MAX=''
REPLACE_MEMCHR=''
REPLACE_MEMMEM=''
REPLACE_MEMPCPY=''
REPLACE_MEMSET_EXPLICIT=''
REPLACE_MKDIR=''
REPLACE_MKFIFO=''
REPLACE_MKFIFOAT=''
REPLACE_MKNOD=''
REPLACE_MKNODAT=''
REPLACE_MKOSTEMP=''
REPLACE_MKOSTEMPS=''
REPLACE_MKSTEMP=''
REPLACE_MKTIME=''
REPLACE_MKTIME_Z=''
REPLACE_NANOSLEEP=''
REPLACE_NULL=''
REPLACE_OBSTACK_PRINTF=''
REPLACE_OPEN=''
REPLACE_OPENAT=''
REPLACE_PERROR=''
REPLACE_PIPE2=''
REPLACE_POPEN=''
REPLACE_POSIX_MEMALIGN=''
REPLACE_POSIX_OPENPT=''
REPLACE_PREAD=''
REPLACE_PRINTF=''
REPLACE_PTHREAD_SIGMASK=''
REPLACE_PTSNAME=''
REPLACE_PTSNAME_R=''
REPLACE_PUTENV=''
REPLACE_PWRITE=''
REPLACE_QSORT_R=''
REPLACE_RAISE=''
REPLACE_RAND=''
REPLACE_RANDOM=''
REPLACE_RANDOM_R=''
REPLACE_READ=''
REPLACE_READLINK=''
REPLACE_READLINKAT=''
REPLACE_REALLOCARRAY=''
REPLACE_REALLOC_FOR_REALLOC_POSIX=''
REPLACE_REALPATH=''
REPLACE_REMOVE=''
REPLACE_RENAME=''
REPLACE_RENAMEAT=''
REPLACE_RMDIR=''
REPLACE_SETENV=''
REPLACE_SETHOSTNAME=''
REPLACE_SETSTATE=''
REPLACE_SLEEP=''
REPLACE_SNPRINTF=''
REPLACE_SPRINTF=''
REPLACE_STAT=''
REPLACE_STDIO_READ_FUNCS=''
REPLACE_STDIO_WRITE_FUNCS=''
REPLACE_STPCPY=''
REPLACE_STPNCPY=''
REPLACE_STRCASESTR=''
REPLACE_STRCHRNUL=''
REPLACE_STRDUP=''
REPLACE_STRERROR=''
REPLACE_STRERRORNAME_NP=''
REPLACE_STRERROR_R=''
REPLACE_STRFTIME=''
REPLACE_STRNCAT=''
REPLACE_STRNDUP=''
REPLACE_STRNLEN=''
REPLACE_STRSIGNAL=''
REPLACE_STRSTR=''
REPLACE_STRTOD=''
REPLACE_STRTOF=''
REPLACE_STRTOIMAX=''
REPLACE_STRTOK_R=''
REPLACE_STRTOL=''
REPLACE_STRTOLD=''
REPLACE_STRTOLL=''
REPLACE_STRTOUL=''
REPLACE_STRTOULL=''
REPLACE_STRTOUMAX=''
REPLACE_STRVERSCMP=''
REPLACE_SYMLINK=''
REPLACE_SYMLINKAT=''
REPLACE_TIME=''
REPLACE_TIMEGM=''
REPLACE_TIMESPEC_GET=''
REPLACE_TIMESPEC_GETRES=''
REPLACE_TMPFILE=''
REPLACE_TRUNCATE=''
REPLACE_TTYNAME_R=''
REPLACE_TZSET=''
REPLACE_UNLINK=''
REPLACE_UNLINKAT=''
REPLACE_UNSETENV=''
REPLACE_USLEEP=''
REPLACE_UTIMENSAT=''
REPLACE_VASPRINTF=''
REPLACE_VDPRINTF=''
REPLACE_VFPRINTF=''
REPLACE_VPRINTF=''
REPLACE_VSNPRINTF=''
REPLACE_VSPRINTF=''
REPLACE_WCRTOMB=''
REPLACE_WCSCMP=''
REPLACE_WCSFTIME=''
REPLACE_WCSNCAT=''
REPLACE_WCSNCMP=''
REPLACE_WCSNRTOMBS=''
REPLACE_WCSRTOMBS=''
REPLACE_WCSSTR=''
REPLACE_WCSTOK=''
REPLACE_WCSWIDTH=''
REPLACE_WCTOB=''
REPLACE_WCTOMB=''
REPLACE_WCWIDTH=''
REPLACE_WMEMCMP=''
REPLACE_WMEMPCPY=''
REPLACE_WRITE=''
REPLACE__EXIT=''
SED=''
SET_MAKE=''
SHELL='/bin/sh'
SIG_ATOMIC_T_SUFFIX=''
SIZE_T_SUFFIX=''
SRCLIBOBJS=''
STDCKDINT_H=''
STDDEF_H=''
STDDEF_NOT_IDEMPOTENT=''
STDINT_H=''
STRIP=':'
SYS_TIME_H_DEFINES_STRUCT_TIMESPEC=''
TIME_H_DEFINES_STRUCT_TIMESPEC=''
TIME_H_DEFINES_TIME_UTC=''
UINT32_MAX_LT_UINTMAX_MAX=''
UINT64_MAX_EQ_ULONG_MAX=''
UNDEFINE_STRTOK_R=''
UNISTD_H_DEFINES_STRUCT_TIMESPEC=''
UNISTD_H_HAVE_SYS_RANDOM_H=''
UNISTD_H_HAVE_WINSOCK2_H=''
UNISTD_H_HAVE_WINSOCK2_H_AND_USE_SOCKETS=''
USE_EXTRA_ENCODINGS=''
USE_MBSTATE_T=''
USE_NLS=''
VERSION='1.18'
WCHAR_T_SUFFIX=''
WINDOWS_64_BIT_OFF_T=''
WINDOWS_64_BIT_ST_SIZE=''
WINDOWS_STAT_INODES=''
WINDOWS_STAT_TIMESPEC=''
WINDRES='windres-rc rc.exe'
WINT_T_SUFFIX=''
WOE32=''
WOE32DLL=''
XGETTEXT=''
XGETTEXT_015=''
XGETTEXT_EXTRA_OPTIONS=''
ac_ct_AR=''
ac_ct_CC='compile cl.exe'
ac_ct_DUMPBIN=''
am__EXEEXT_FALSE=''
am__EXEEXT_TRUE=''
am__fastdepCC_FALSE=''
am__fastdepCC_TRUE=''
am__include=''
am__isrc=' -I$(srcdir)'
am__leading_dot='.'
am__nodep=''
am__quote=''
am__rm_f_notfound=''
am__tar='$${TAR-tar} chof - "$$tardir"'
am__untar='$${TAR-tar} xf -'
am__xargs_n='xargs -n'
bindir='${prefix}/../tools/libiconv/debug/bin'
build='x86_64-pc-mingw32'
build_alias='x86_64-pc-mingw32'
build_cpu=''
build_os=''
build_vendor=''
datadir='${datarootdir}'
datarootdir='${prefix}/share/libiconv'
docdir='${datarootdir}/doc/${PACKAGE_TARNAME}'
dvidir='${docdir}'
exec_prefix='NONE'
gl_LIBOBJDEPS=''
gl_LIBOBJS=''
gl_LTLIBOBJS=''
gltests_LIBOBJDEPS=''
gltests_LIBOBJS=''
gltests_LTLIBOBJS=''
gltests_WITNESS=''
host=''
host_alias=''
host_cpu=''
host_os=''
host_vendor=''
htmldir='${docdir}'
includedir='${prefix}/../include'
infodir='${datarootdir}/info'
install_sh='${SHELL} '\''/c/Users/Tal/Desktop/Plant Health +/Plant-Health-Plus/PlantHealthPlus/vcpkg/buildtrees/libiconv/src/v1.18-8006e03beb.clean/build-aux/install-sh'\'''
libdir='${prefix}/lib'
libexecdir='${exec_prefix}/libexec'
localedir='${datarootdir}/locale'
localedir_c=''
localedir_c_make=''
localstatedir='${prefix}/var'
mandir='${datarootdir}/man'
mkdir_p='$(MKDIR_P)'
oldincludedir='/usr/include'
pdfdir='${docdir}'
prefix='/C/Users/Tal/Desktop/Plant Health +/Plant-Health-Plus/PlantHealthPlus/vcpkg_installed/x64-windows/debug'
program_transform_name='s,x,x,'
psdir='${docdir}'
reloc_final_prefix_c=''
reloc_final_prefix_c_make=''
runstatedir='${localstatedir}/run'
sbindir='${prefix}/../tools/libiconv/debug/sbin'
sharedstatedir='${prefix}/com'
subdirs=''
sysconfdir='${prefix}/etc'
target_alias=''

## ----------- ##
## confdefs.h. ##
## ----------- ##

/* confdefs.h */
#define PACKAGE_NAME "libiconv"
#define PACKAGE_TARNAME "libiconv"
#define PACKAGE_VERSION "1.18"
#define PACKAGE_STRING "libiconv 1.18"
#define PACKAGE_BUGREPORT ""
#define PACKAGE_URL ""
#define PACKAGE "libiconv"
#define VERSION "1.18"

configure: exit 77
```
</details>

**Additional context**

<details><summary>vcpkg.json</summary>

```
{
  "name": "plant-health-plus",
  "version": "0.2.0",
  "dependencies": [
    "curl",
    "libxml2"
  ]
}

```
</details>
