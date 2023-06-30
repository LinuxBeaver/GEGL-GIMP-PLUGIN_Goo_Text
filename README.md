# GEGL Goo on Text
Gimp filter where GEGL puts goo on your text. It works great with my other text styling filters.

If you want to compile you must compile gegl:goo-text, gegl:ZZwind and gegl:bevel. ZZWind and Bevel are components of GEGL Goo Text and GEGL Goo Text will not work without them.
GEGL Bevel is a bonus filter that comes with goo-text and a few other of my filters. If you already have gegl:bevel in GEGL Plugins you do not need to recompile it unless you want to update to the latest version which is recommended.  


![image preview](previewgoo.png )
![image preview](final.png )

## OS specific location to put GEGL Filter binaries 

Windows
 C:\\Users\<YOUR NAME>\AppData\Local\gegl-0.4\plug-ins
 
 Linux 
 /home/(USERNAME)/.local/share/gegl-0.4/plug-ins
 
 Linux (Flatpak)
 /home/(USERNAME)/.var/app/org.gimp.GIMP/data/gegl-0.4/plug-ins

 ### Linux

To compile and install you will need the GEGL header files (`libgegl-dev` on
Debian based distributions or `gegl` on Arch Linux) and meson (`meson` on
most distributions).

```bash
meson setup --buildtype=release build
ninja -C build

```

If you have an older version of gegl you may need to copy to `~/.local/share/gegl-0.3/plug-ins`
instead (on Ubuntu 18.04 for example).



### Windows

The easiest way to compile this project on Windows is by using msys2.  Download
and install it from here: https://www.msys2.org/

Open a msys2 terminal with `C:\msys64\mingw64.exe`.  Run the following to
install required build dependencies:

```bash
pacman --noconfirm -S base-devel mingw-w64-x86_64-toolchain mingw-w64-x86_64-meson mingw-w64-x86_64-gegl
```

Then build the same way you would on Linux:

```bash
meson setup --buildtype=release build
ninja -C build
```


## More Previews of this based Gimp Plugin

![image preview](newgoo.png )

![image](https://github.com/LinuxBeaver/gegl-goo-on-text/assets/78667207/4471522e-7c42-444e-8ec9-e7e8190c6a3d)
