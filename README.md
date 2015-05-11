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

This should build an executable showing a recorder dialog, where you can 
control your telecinema.
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

