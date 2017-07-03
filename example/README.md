# Preparing data (GATE list mode) to conversion

In order to utilize [SAFIR](http://stir.sourceforge.net/MIC2015UsersMeeting/STIR_UM2015_Fischer_SAFIRInputFileFormat.pdf "PowerPoint Presentation"), initial data require some additional steps of preprocessing. The shell script **get_files.sh** automatically creates all mandatory files, sorted out in directories, one for each initial GATE simulation.

## Prerequisites
#### SAFIR and STIR files
The list of files, required to proceed (all of them must be located in the same directory, where initial list mode data from GATE simulations are placed in.):

* *FBP3DRP.par* -- default 3D filtered reprojection parameters for STIR
* *get_files.sh* -- main shell script which automatically converts data to SAFIR format
* *listmode_input_JPET_SAFIR.par* -- [parameters for SAFIR](https://github.com/UCL/STIR/tree/master/examples/SAFIR-listmode-virtual-scanner "details")
* *lm_to_projdata.par* -- parameters for transformation from SAFIR into STIR interfile
* *template_sino_L020_4mm.hs* -- (here and below) temporary sinogram header
* *template_sino_L020_7mm.hs*
* *template_sino_L050_4mm.hs*
* *template_sino_L050_7mm.hs*
* *template_sino_L100_4mm.hs*
* *template_sino_L100_7mm.hs*

Temporary sinogram headers reflect scanner geometries and some of their parameters will be changed during transformation.

#### Building C++ executables
Execution of **get_files.sh** is possible only after the [build](https://github.com/JPETTomography/listmode-to-safir) of functions **GenerateCrystalMapJPET**, **ConvertToMUPET** and **GenerateBlurredMUPET** and after adding its (build directory) path to $PATH:
```
$ cd /build-of-source/
$ export PATH=$PWD:$PATH
```

## Algorithm description (executing *get_files.sh*)
**Note!** Information, displayed during conversion, sometimes uses carriage return. Hence it is better to add a filter in order to save the output in a log file ('*shell_output.log*'):
```
$ ./get_files.sh | tee >(grep -v $'\r' > "shell_output.log")
```

First of all, a separate directory will be created for each file with '*SAFIR_*' prefix prepended (for example, '*SAFIR_D85_1lay_L020_4mm_01_00_00_short/*' -- for '*D85_1lay_L020_4mm_01_00_00_short*').

Next, three different Gaussian blurs had to be added to the initial axial (Z) and time variables, corresponding to smearing (error) in various models of photomultipliers (PM). The notation below would mean "SI" -- for silicon photomultiplier, "PMT" -- standard tube Hamamatsu-like one, "WLS" -- for [wavelength shifter](http://iopscience.iop.org/article/10.1088/0031-9155/53/7/002 "one of the articles"). This procedure is implemented in **GenerateBlurredMUPET** command (look for description [here](https://github.com/JPETTomography/listmode-to-safir)).

For each smearing (i.e. blur or PM type), a new file will be composed in initial list mode format, prepending the corresponding prefix ('*PMT_*', '*SI_*', '*WLS_*').

**GenerateBlurredMUPET** will also create file with geometry parameters '*JPETGeometry.par*', generate crystal map '*crystal_map_jpet.txt*' (briefly explained in [SAFIR presentation](http://stir.sourceforge.net/MIC2015UsersMeeting/STIR_UM2015_Fischer_SAFIRInputFileFormat.pdf "PowerPoint Presentation")) and calculate averages (mean and median) for depth-of-interaction (DOI) during conversion ('*DOI_parameters.par*').

Next, a separate directory will be created for each PM (PMT, SI and WLS) with files of distinct parameters for SAFIR-Interfile conversion. '*JPETGeometry.par*', '*crystal_map_jpet.txt*' and '*DOI_parameters.par*' are common files (same for all cases) and will be placed in main directory. '***lm_to_projdata.par***' and '***FBP3DRP.par***', however, will be copied to every directory, for convenience.

Finally, the script will convert list mode data, smeared for three PMs, into MUPET binaries (SAFIR format) with extension '*.clm.safir*', which will be moved to corresponding directories. Since MUPET is substantially smaller than ASCII, list mode files will be deleted.

Please, refer to [SAFIR example](https://github.com/UCL/STIR/tree/master/examples/SAFIR-listmode-virtual-scanner "example in STIR repository") for exploring the further conversion procedure (SAFIR/MUPET to STIR/Interfile) and compulsory files needed for it. You might also check [instructions on J-PET Wiki](http://koza.if.uj.edu.pl/petwiki/index.php/STIR_%28Software_for_Tomographic_Image_Reconstruction%29#SAFIR_module "SAFIR usage"), if you have got an access.
