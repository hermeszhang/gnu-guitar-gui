GNU Guitar GUI
==============

[![Travis Badge](https://travis-ci.org/gnu-guitar/gui.svg?branch=master)](https://travis-ci.org/gnu-guitar/gui)
[![Build status](https://ci.appveyor.com/api/projects/status/daqojo4qeng5e9kn?svg=true)](https://ci.appveyor.com/project/tholberton/gui)

The graphical interface of the GNU Guitar project.

This project is part of a larger project (see [this repo](https://github.com/gnu-guitar/gnu-guitar)).

You may build this project by itself. However, it will only be useful for
testing the user interface. Building this project by itself will not allow you
to stream audio.

### Building

The easiest way to build the project is to use the [build scripts](https://github.com/gnu-guitar/gnu-guitar).

To build the project by itself, make sure you have the Qt libraries installed.

Once the Qt libraries are installed, you can enter the build directory and do
this:

```
mkdir build && cd build
cmake ..
cmake --build .
```

You can run the program like this:

```
src/gnu-guitar-gui
```

Do NOT install the program. It's not going to be very useful on your system by itself.
