# SourceXtractor++ Demo Plugin

[SourceXtractor++](https://github.com/astrorama/sourcextractorplusplus)
supports externally built plugins, loading them at run-time.

This means anyone can develop their own plugin, add it to the pipeline,
and get their target measure - for instance - directly into the
output catalog.

This is a particular example of such a plugin, which can be used as an
example, or even as a baseline. You can just fork it, and modify it to
your needs.

The repository is organized following the standard
[Elements'](https://github.com/astrorama/Elements) layout:

* `CMakeLists.txt` Project-level cmake build script
* `Makefile` so you can type `make` directly. You could, of course,
    run `cmake` out of the source tree
* `doc` contains documentation about the project
* `Petrosian` is an Elements _module_. A single project
    could contain, for instance, the plugin itself, and a Python script
    that does something with the output. Those would be, ideally,
    two modules.
    - `doc` Module level documentation
    - `Petrosian` C++ headers
    - `src/lib` Library sources. You could have `src/program` if you had
        a binary. In this case, we do not have any.

At the source level, we have split the functionality into two different
properties: `PetrosianRadius`, and `PetrosianPhotometry`.
There is no reason not to have both in one single property, but this
organization helps understanding the code, and highlight how
one property can be, at the same time, part of the output and part
of the input of a different property.

Shared between both, we have `PetrosianPlugin`, which takes care of
registering the properties and their associated output columns; and
`PetrosianConfig`, which handles the configuration of this particular
measure.

For more details, we recommend navigating through the source code, as
each file is heavily commented.

## Building the plugin

For building the plugin you need, obviously, to install first
SourceXtractor++. You can do it via `rpms`, or compiling directly
from sources. Either way, Elements should be able to find the required
components.

If you follow the rpm way, you will need to install the development
packages:

```shell script
dnf install Elements-devel Alexandria-devel SourceXtractor-devel
```

For how to compile, or get the necessary rpms, please, go to
[SourceXtractor++](https://github.com/astrorama/sourcextractorplusplus)
and follow the instructions there.

Once you have the requirements installed, you can just type:

```shell script
make
ls build*/lib*/libPetrosian.*
```

The plugin will be on a directory called `build` (or starting with
`build`), inside the folder `lib64` (or `lib` depending on your platform).

### Distributing the plugin

If you have installed the dependencies using rpms, Elements can
easily generate an rpm with the compiled plugin. Just type:

```shell script
make rpm
```

**Note**: This repository includes a `.travis.yaml` file configured
to build the rpm.

## Using the plugin

SourceXtractor++ needs to be told where the plugins are located,
and which plugins to load. As of today, it will not load them
automatically. It can be done as follows:

From the source directory:

```shell script
plugin_name="libPetrosian"
plugin_full_path=$(readlink -f build*/lib*/${plugin_name}.*)
plugin_dir=$(dirname "${plugin_full_path}")
```

Let's check it works:

```shell script
sourcextractor++ --plugin-directory "${plugin_dir}" --plugin "${plugin_name}" --help
```

If everything went fine, you will be able to see the configuration
registered by this plugin:

```
Petrosian photometry options:
  --petrosian-eta arg (=0.20000000000000001)
                                        Fraction of the isophote over the 
                                        surface brightness For the Petrosian 
                                        radius
  --pretrosian-factor arg (=2)          Scale factor for Petrosian photometry
  --petrosian-minimum-radius arg (=3.5) Minimum radius for Petrosian photometry
  --check-image-petrosian arg           Check image for Petrosian apertures
```

Similarly, you can check the list of output properties:

```shell script
sourcextractor++ --plugin-directory "${plugin_dir}" --plugin "${plugin_name}" --list-output-properties
```

```
AperturePhotometry
AutoPhotometry
ExternalFlags
FlexibleModelFitting
GroupInfo
GroupStamp
IsophotalFlux
KronRadius
MoffatModelFitting
NDetectedPixels
PeakValue
PetrosianPhotometry <<
PetrosianRadius     <<
PixelBoundaries
PixelCentroid
ShapeParameters
SourceFlags
SourceIDs
WorldCentroid
```

Now, you can run `sourcextractor++` in your data in the usual way.
As long as you pass `--plugin-directory` and `--plugin`, you will
be able to ask for the corresponding properties as you would any other.

## Getting help

If you need any help, do not hesitate to open an *Issue* on this project.

## Disclaimers

* SourceXtractor major version is `0`. This means the API may change.
  We do not expect any major changes, but that does not mean a header file
  may change location, or one API call could change.
  
  In any case, your code will remain compatible with the version listed
  on the `CMakeLists.txt` file (if it is a released version!).

* This plugin implements the [Petrosian aperture](https://doi.org/10.1111/j.1365-2966.2004.08090.x).
  It has been cross-checked with SExtractor 2, but do not expect it to be flawless.
