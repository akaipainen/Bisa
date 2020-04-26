# Bisa

Bisa is an analysis framework developed to commission RPC detectors for the BIS-78 upgrade to the ATLAS Muon Spectrometer at CERN. This repository contains a command-line-tool (CLT) which produces histogram plots as PDFs.

## Features
-----------
- Supports MacOS and Linux
- Fast analysis (Completes a 10 min data run in under 20 seconds)

## Getting Started (Mac/Linux)
------------------

Clone this repository. Make sure to include the ```--recursive``` flag, otherwise git will not clone the submodules.
```sh
$ git clone --recursive https://github.com/akaipainen/Bisa.git
```

Build the code using Premake5. Replace ```<ROOT_INSTALL_DIR>``` with your local ROOT installation directory.
```sh
$ cd Bisa
$ ./vendor/bin/premake/premake_mac --rootsys=<ROOT_INSTALL_DIR> gmake
$ make config=Dist Commissioning
```

Run the application on a datafile. The resulting output will be saved in the ```output/``` folder.
```sh
$ ./bin/Dist-macosx-x86_64/Commissioning/Commissioning data/config.json
```

## Using Bisa
-----------
You are encouraged to fork this repository and modify the task specific code in Commissioning.

### Project organization
This project is split up into two components: a library (Bisa) and a command-line application (Commissioning). Bisa hosts the code necessary to run any analysis task. Commissioning hosts the code specific to the commissioning task, so this is where the histograms and cuts are defined.

#### Bisa
The custom types of interest are `Hit`, `HitCollection`, `Feature`, and `FeatureCollection`. 
- A `Hit` is defined as a single dataword from the readout electronics 
- A `HitCollection` consists of any number of hits from a single event
    - The master `HitCollection` contains all hits in an event
- A `Feature` is defined as a collection of hits (`HitCollection`) with some shared property
- A `FeatureCollection` consists of all features of a given type in an event

For example, I have create a `Feature` called `SpaceCluster`. I create a `SpaceClusterSelector` to select the hits that belong in space clusters. In this case, I will group all adjacent hits into space clusters, then return all space clusters as a `FeatureCollection`. 

#### Commissioning
Bisa was made with the intent of creating a modular framework which could be applied to various tasks within the commissioning process of BIS-78. The Commissioning project defines the specfic features and histograms to be analyzed. Currently, the Commissioning tool creates cuts on 5 different features: adjacent hits, first hits on strips, noise bursts, space clusters, and time clusters. The selector code for these features can be found in the repo under `Commissioning/src/Features`.

#### Config
All chamber configuration settings are handled in the config.json file. This defines the mapping from TDCs to RPCs, the cable mapping from TDC channel to RPC strip, and other run-time constants such as the run duration. See the config file for more details.

