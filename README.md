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
```

run this outside this shell (inside build directory)
```
$ export PATH=$PWD:$PATH
```
The executables are (partially tested):

* GenerateCrystalMapJPET
* ConvertToMUPET
* GenerateBlurredMUPET

Usage examples:
```
$ GenerateCrystalMapJPET -p parameters_file.par -o crystal_map_file.txt
$ ConvertToMUPET -i input_file -p parameters_file.par -o output_file.clm.safir
$ GenerateBlurredMUPET -i input_file [-p parameters_file.par]
```
Please, refer to [SAFIR example](https://github.com/UCL/STIR/tree/master/examples/SAFIR-listmode-virtual-scanner "example") for further instructions.

For the latter executable **GenerateBlurredMUPET**, it is optional to set geometry file with -p key. If not given, the script would try to generate .par file by parsing the very name of the input (for example, "D85_1lay_L020_4mm_10_00_07"). But even if the name could not be parsed, the program would search for the default parameters file "JPETGeometry.par" as the last possible option.

Consequently, **GenerateBlurredMUPET** would add new files, corresponding to three photomultipliers that reflect different smearing of the data, in both listmode and SAFIR formats.

**Note!** At the moment, the "smeared" listmode data comprise only pairs of Cartesian coordinates with times of hits, cutting out the rest (energy, type of coincidence etc).

**Note!** Smearing hit positions along axial (Z) direction would probably leave some events outside the scanner. By default, such events are thrown away, reducing their total number. Alternatively, hit positions could be coerced to the edges of the scanner (setting *coerceEdges* to *true* in JPETHit::calibrate() and JPETEvent::calibrate() functions).
