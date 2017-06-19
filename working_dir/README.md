##In order to run:

1) Copy data files to the folder 'INPUT_DATA'
2) Run script 
>./one_click_reconstruction.sh
to create projection data and reconstruct it using FBP3DRP algorithm (all files in INPUT_DATA folder).
_OR_  
Run script
>./get_files.sh
if you only one to convert input data files into file format readable by STIR and SAFIR module. 
3) Output files can be found in the 'OUTPUT_DATA' directory.

##You should first create executables (with path added to $PATH):

* GenerateCrystalMapJPET
* ConvertToMUPET
* GenerateBlurredMUPET

You can do easy installation by typing
>source install.sh
in the main directory of this programme.
