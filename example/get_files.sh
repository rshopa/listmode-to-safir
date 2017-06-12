echo $'\n======================'
echo $'--- PREPARING DATA ---'
echo $'Current directory: \n'$PWD
INPUT_LIST=$(ls -t -U | grep "D85")
# echo $INPUT_LIST

for x in $INPUT_LIST
do
  if echo $x | grep -q -P '^D85'
  then
    # -o - only part of line matching pattern -P
    WIDTH=$(echo $x | grep -o -P '.{0,1}mm')
    LENGTH=$(echo $x | grep -o -P 'L.{0,3}')

    # make directory and cd to it
    DIR=$'SAFIR_'$(echo $x)
    mkdir -p $DIR
    cd $DIR
    echo $'\n:::::::::::::::::::::::::::::::::::'
    echo $'CONVERTING TO SAFIR: '
    echo $x
    echo $':::::::::::::::::::::::::::::::::::'

    # This command is from my src written in c++
    # Must be added to PATH (export PATH=$PWD$'/build':$PATH)
    # It produces 3 files for different photomultipliers (PM),
    # with conversion to SAFIR binaty (.clm.safir)
    GenerateBlurredMUPET -i $'../'$x

    # list of files with PM
    PM_LIST=$(ls -t -U | grep -P "_[dD]")
    for pm in $PM_LIST
    do
      PM=$(echo $pm | awk '{print substr($0,0,3)}')
      # calculate Depth of interaction (DOI)
      if [[ $PM = 'SI_' ]]
      then
        PM=SI
        LINE=$(sed '5!d' DOI_parameters.par)
      else
        if [[ $PM = 'WLS' ]]
        then
          LINE=$(sed '7!d' DOI_parameters.par)
        else
          LINE=$(sed '3!d' DOI_parameters.par)
        fi
      fi
      DOI=$(echo $LINE | awk '{ print $3 }')
      echo $PM$' - '$DOI

      # Create directories for every PM and move files
      mkdir -p $PM
      mv $pm $PM

      # Copies corresponding template sinogram
      if [[ $WIDTH = '4mm' ]]
      then
        if [[ $LENGTH = 'L020' ]]
        then
          SINO_TEMP=template_sino_L020_4mm.hs
        else
          if [[ $LENGTH = 'L050' ]]
          then
            SINO_TEMP=template_sino_L050_4mm.hs
          else
            SINO_TEMP=template_sino_L100_4mm.hs
          fi
        fi
      else
        if [[ $LENGTH = 'L020' ]]
        then
          SINO_TEMP=template_sino_L020_7mm.hs
        else
          if [[ $LENGTH = 'L050' ]]
          then
            SINO_TEMP=template_sino_L050_7mm.hs
          else
            SINO_TEMP=template_sino_L100_7mm.hs
          fi
        fi
      fi

      cp $'../'$SINO_TEMP $PM$'/template_sino.hs'
      sed -i "31s/:=.*/:= $DOI/" $PM$'/template_sino.hs'
      touch $PM$'/template_sino.s'
      # # files of parameters
      cp $'../listmode_input_JPET_SAFIR.par' $PM
      cp $'../lm_to_projdata.par' $PM
      # cd $PM
      if echo $pm | grep -P 'clm.safir'
      then
        cd $PM
        sed -i "2s/:=.*/:= $pm/" listmode_input_JPET_SAFIR.par
        cd ..
      fi
    done

    cd ..
    echo $'\n:::::: CONVERSION ENDED ::::::'
  fi
done
# echo ${INPUT_LIST[]}
