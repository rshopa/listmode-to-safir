# Converting list mode of events to STIR (interfile) sinograms

This implementation concerns data, simulated in [GATE](http://www.opengatecollaboration.org/) for the cylindrical scanner in the framework of [Jagiellonian Positron Emission Tomography (J-PET)](http://koza.if.uj.edu.pl/pet/ "Home page") project.

#### Prerequisites
* [STIR 3.0](http://stir.sourceforge.net/ "STIR homepage") (for registered users) or [developers’ version](https://github.com/UCL/STIR)
* [SAFIR](http://stir.sourceforge.net/MIC2015UsersMeeting/STIR_UM2015_Fischer_SAFIRInputFileFormat.pdf "PowerPoint Presentation"), build with STIR 3.0 (but already integrated with developers’ version, so no need to install).


### Easy installation:
```
source install.sh
```

#### Step-by-step build (use if easy install fails):
```
$ cd /directory-with-repository-files/src/
$ mkdir -p build
$ cd build
$ cmake ../
$ make
```

run this outside this shell (inside src directory)
```
$ export PATH=$PWD:$PATH
```
The executables are (not tested yet):

* GenerateCrystalMapJPET
* ConvertToMUPET
* GenerateBlurredMUPET

The latter should presumably create parameters file from the very name of the datafile, adding three different new files for three photomultipliers (different smearing of the data).
