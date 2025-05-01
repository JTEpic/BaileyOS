# BaileyOS


Made by JTEpic

OS project started september 2024


## Introduction

BaileyOS is an Open Source, multi-platform operating system. It provides a simple, platform-independent API for creating windows, contexts and surfaces, reading input, handling events, etc.

BaileyOS natively supports Linux, macOS and Windows and other Unix-like systems. On Linux (both Wayland and X11 are supported.) currently X11 is supported.

BaileyOS is licensed under the (ENTER LICENSE) license.

You can download the latest development or stable release as source.

The documentation is available online and is included in all source archives. See the release notes for new features, caveats and deprecations in the latest release. For more details see the version history.

The master branch is the stable integration branch and should always compile and run on all supported platforms, although details of newly added features may change until they have been included in a release. New features and many bug fixes live in other branches until they are stable enough to merge.

(BaileyOS exists because of the contributions of many people around the world, whether by reporting bugs, providing community support, adding features, reviewing or testing code, debugging, proofreading docs, suggesting features or fixing bugs.)


## Compiling BaileyOS

BaileyOS is written primarily in Assembly/C++. BaileyOS itself requires only the headers and libraries for your OS and window system.

BaileyOS supports compilation on Windows with Visual C++ 2013 and later, MinGW and MinGW-w64, on macOS with Clang and on Linux and other Unix-like systems with GCC and Clang. It will likely compile in other environments as well, but this is not regularly tested.

See the compilation guide for more information about how to compile BaileyOS yourself.


## Using BaileyOS

See the documentation for tutorials, guides and the API reference.


## Contributing to BaileyOS

See the contribution guide for more information.


## System requirements

BaileyOS supports Windows XP and later and macOS 10.8 and later. Linux and other Unix-like systems running the X Window System are supported even without a desktop environment or modern extensions, although some features require a running window or clipboard manager.

See the compatibility guide in the documentation for more information.


## Dependencies

BaileyOS itself needs the folowing headers and libraries for your OS and window system.

    nasm for compiling assembly
    i686-elf (Binutils + GCC) for cross compiling C
    qemu-desktop for emulation


## Reporting bugs

Bugs are reported to our [issue tracker](https://github.com/JTEpic/BaileyOS/issues). Please check the contribution guide for information on what to include when reporting a bug.


## Contact

On the GitHub page you can find the latest version of BaileyOS, as well as news, documentation and other information about the project.

If you have a bug to report, a patch to submit or a feature you'd like to request, please file it in the [issue tracker](https://github.com/JTEpic/BaileyOS/issues) on GitHub.

Finally, if you're interested in helping out with the development of BaileyOS or porting it to your favorite platform, join us on GitHub.
