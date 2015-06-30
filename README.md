/////////////////////////// INTRODUCTION ///////////////////////////////////////

* KALISCOPE is a set of tools and plugins to create a professional pipeline for
  telecinema devices. A telecinema device is a device that convert silver films
  into numeric movies. The goal is to produce reliable sophisticated tools to
  help artists in their analog film projects by respecting the quality of their
  precious images.

* Done by Eloi du Bois

* LICENSE (except for what is inside the directory kaliscope_root/libraries, for each sublib, check its sub-licenses):
        GPLv3 (might change, but as the development is under progress I prefer staying in GPL)

* By using this library and its concepts and designs, you accept the content 
  of DISCLAIMER.txt

////////////////////////////////////////////////////////////////////////////////

## Presentation video

http://youtu.be/W9B1whn3qKo

## Main features

* The pipeline is OpenFX based, meaning that you can plug commercial plugins
  for stabilization, denoise, etc...
* Compatible with ximea camera thru an OpenFx plugin
* Multiplatform user interface (only qt GUI right now, but all is possible)
* Network client/server to remotely control the GUI
* Use of TuttleOfx, which is a very reliable framework for OpenFX plugins that
  comes with plugins such as denoiser, undistort, lut, crop, keyer, and many others.
* Many output formats are handled: DPX, TIF, Jpeg2000, ....

## Compilation

To compile, you will need Qt5, Boost, Fmod, ncurses, cdk and scons:

* QT5: http://www.qt.io/download/
* Boost: http://www.boost.org/users/download/
* NCURSES: http://ftp.gnu.org/pub/gnu/ncurses/
* CDK: http://invisible-island.net/cdk/

Please note that due to the intensive use of templates, you will need at least 1.6GB of memory.
This is important if you plan to compile this directly on the Raspberry PI.

First, clone the repository:

```
git clone https://github.com/edubois/kaliscope.git
cd kaliscope
git submodule update -i
```
This should bring tools/sconsProject and mvp-player submodules

Now, it's time to edit default.sconf according to your configuration.
In the default configuration, I made a parent directory 3rdParties where I put
my 3rd party libraries. To change your external libraries base dir, 
edit the variable extern in this file (default.sconf).

If you are using Mac, adapt the last lines according to your
XCode configuration.

If you are not using Mac, remove the lines after '# Mac only'


## Running

When you are ready, enter:

```scons mode=release```

This should build an executable kaliscope_qt showing a recorder dialog, where you can 
control your telecinema.

Edit init.ksh according to your configuration, then source it and execute the binary file.

You only have to connect your Hall Sensor to GPIO INPUT 1 of the ximea device.
The Hall sensor must be activated by a magnet each time a film frame has to be captured.
Connecting it to the GPIO1 will synchronize the triggering of the camera exposure afterwhat we can capture images.

* if you want to compile the ximea censor (camera) ofx plugin, type:
```scons mode=release XimeaCameraReader```

To start the network remote, use:

```kaliscope_qt --remote```

Please note that, if using a firewall, you must allow communication on port 11999

* You can also chose to run all the unittests:
```scons unittest```

OR some of them:

```scons unittest-kaliscope-core```


## Compilation and running of kalisync

Kalisync is a tool that is used to synchronize the telecinema capture device.
It has been designed for raspberry pi. The main idea is to watch for a 
synchronization signal on a GPIO input pin (18 in the following example).
When a signal is detected on the input pin, a network event is sent to another
powerful computer which will do the frame capture job.

To compile this tool, you will need to build kalisync on a raspberry pi with a recent g++ (>=4.8) (or by using a cross-compilation tool).

```cp default-rpi.sconf default.sconf```

```scons kalisync```

I personally had issues to compile this program on my raspberry pi2 because it needs more than 2GB of RAM.
I had to connect a USB solid hard drive and set up a ramdisk on it, then after 1 hour of compilation, I was able
to get a working program. Please note that if you set a swap on a flash disk (sdcard for example), you might break it.


## Compilation of the OFX plugins

### Color mask remover plugin

This plugin helps in removing colored masks that can appear on you films negatives (often an orange mask).

```scons colorMaskRemover```

### 

