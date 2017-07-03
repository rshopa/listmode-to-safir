#!/bin/bash
#AUTHOR="RAFAL MASELEK"
#EMAIL="rafal.maselek@ncbj.gov.pl"

#Roman Shopa's script
./get_files.sh

echo $'\n======================'
echo $'--- PERFORMING ONE CLICK RECONSTRUCTION ---'
echo $'Current directory: \n'$PWD
INPUT_LIST=$(ls -t -U | grep "^SAFIR_D")
echo $'\nInput list:'
echo $INPUT_LIST
echo $'======================'
for FOLDER in $INPUT_LIST
do
 cd $FOLDER
 cd ./PM
  echo $'\nCREATING PROJ DATA IN \n'$PWD
  lm_to_projdata lm_to_projdata.par
  echo $'\nPERFORMING RECONSTRUCTION USING FBP3DRP IN:\n'$PWD  
  #could  be also somehow done without copying
  cp ../../FBP3DRP.par .
  FBP3DRP ./FBP3DRP.par
  cd ..
 cd ./SI
  echo $'\nCREATING PROJ DATA IN \n'$PWD
  lm_to_projdata lm_to_projdata.par
  echo $'\nPERFORMING RECONSTRUCTION USING FBP3DRP IN:\n'$PWD  
  cp ../../FBP3DRP.par .
  FBP3DRP ./FBP3DRP.par
  cd ..
 cd ./WL
  echo $'\nCREATING PROJ DATA IN \n'$PWD
  lm_to_projdata lm_to_projdata.par
  echo $'\nPERFORMING RECONSTRUCTION USING FBP3DRP IN:\n'$PWD  
  cp ../../FBP3DRP.par .
  FBP3DRP ./FBP3DRP.par
 cd ../../
done

echo $'\n:::::: RECONSTRUCTION ENDED ::::::'

