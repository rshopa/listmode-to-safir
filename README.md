# Converting list mode of events to STIR (interfile) sinograms

This implementation concerns data, simulated in [GATE](http://www.opengatecollaboration.org/) for the cylindrical scanner in the framework of [Jagiellonian Positron Emission Tomography (J-PET)](http://koza.if.uj.edu.pl/pet/ "Home page") project.

#### Prerequisites
* [STIR 3.0](http://stir.sourceforge.net/ "STIR homepage") (for registered users) or [developers’ version](https://github.com/UCL/STIR)
* [SAFIR](http://stir.sourceforge.net/MIC2015UsersMeeting/STIR_UM2015_Fischer_SAFIRInputFileFormat.pdf "PowerPoint Presentation"), build with STIR 3.0 (but already integrated with developers’ version, so no need to install).


#### Simple build:
```
$ cd /directory-with-repository-files/src/
$ mkdir -p build
$ cd build
$ cmake ../
$ make
$ export PATH=$PWD:$PATH
```
This will compile two bash executables:
* **GenerateCrystalMapJPET** – generates file for mapping Cartesian coordinates to IDs of rings and detectors according to scanner geometry.
* **ConvertToMUPET** – converts ASCII list mode data from GATE simulation into MUPET binary format (with big endian order).

#### Example of usage 
(requires **JPETGeometry.par** – file containing J-PET geometry parameters.):
```
$ cd ../../input
$ GenerateCrystalMapJPET -p JPETGeometry.par -o ../crystal_map_jpet.txt
$ ConvertToMUPET -i PSF_384strips_x10_y0_z0 -p JPETGeometry.par -o ../output_x10y0z0.clm.safir
```
