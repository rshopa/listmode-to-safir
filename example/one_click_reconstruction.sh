#!/bin/bash
#AUTHOR="RAFAL MASELEK"
#EMAIL="rafal.maselek@ncbj.gov.pl"

#Roman Shopa's script
./get_files.sh

echo $'\n======================'
echo $'--- PERFORMING ONE CLICK RECONSTRUCTION ---'
echo $'Current directory: \n'$PWD
INPUT_LIST=$(ls -t -U | grep "^SAFIR_D")
echo $INPUT_LIST
for FOLDER in $INPUT_LIST
do
 cd $FOLDER
 cd ./PM
  echo "CREATING PROJ DATA IN $PWD"
  lm_to_projdata lm_to_projdata.par
  echo "PERFORMING RECONSTRUCTION USING FBP3DRP in $PWD"  
  #could  be also somehow done without copying
  cp ../../FBP3DRP.par .
  FBP3DRP ./FBP3DRP.par
  cd ..
 cd ./SI
  echo "CREATING PROJ DATA IN $PWD"
  lm_to_projdata lm_to_projdata.par
  echo "PERFORMING RECONSTRUCTION USING FBP3DRP in $PWD"  
  cp ../../FBP3DRP.par .
  FBP3DRP ./FBP3DRP.par
  cd ..
 cd ./WL
  echo "CREATING PROJ DATA IN $PWD"
  lm_to_projdata lm_to_projdata.par
  echo "PERFORMING RECONSTRUCTION USING FBP3DRP in $PWD"  
  cp ../../FBP3DRP.par .
  FBP3DRP ./FBP3DRP.par
 cd ../../
done

echo $'\n:::::: RECONSTRUCTION ENDED ::::::'

