#!/bin/bash
# run two processes in the background and wait for them to finish



#BASE_FOLDER="/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/Random-OFOV-activity-C11-2016-09-08/XB1BN286N-BI-01/scatterMCS/simu_num_5000_new_1/"

#BINARY_FILE_PATH_1="XB1BN286N-BI-01"
#GPU_NUM="1"

## if you want to pass integer GPU_NUM, you should omit the ""
##root './Lor_to_sino.C("'$BASE_FOLDER'","'$BINARY_FILE_PATH_1'")'
#root './Lor_to_sino.C("'$BASE_FOLDER'","'$BINARY_FILE_PATH_1'",'$GPU_NUM')'
  
 #wait
#sleep 10


#BASE_FOLDER="/data/PET/mr_pet_temp/Ma/software/data/gpupet/patients/FET/FE2BP034F-BI/scatterMCS/photonPair48E9/"

#BINARY_FILE_PATH_1="FE2BP034F-BI_0-3000"
#GPU_NUM="2"

## if you want to pass integer GPU_NUM, you should omit the ""
##root './Lor_to_sino.C("'$BASE_FOLDER'","'$BINARY_FILE_PATH_1'")'
#root './Lor_to_sino.C("'$BASE_FOLDER'","'$BINARY_FILE_PATH_1'",'$GPU_NUM')'
  
 #wait
#sleep 10




BASE_FOLDER="/data/PET/mr_pet_temp/Ma/software/data/gpupet/patients/FET/FE2BP034F-BI/scatterMCS/photonPair48E9/"

BINARY_FILE_PATH_1="FE2BP034F-BI_0-3000"
GPU_NUM="2"

# if you want to pass integer GPU_NUM, you should omit the ""
#root './Lor_to_sino.C("'$BASE_FOLDER'","'$BINARY_FILE_PATH_1'")'
root './Lor_to_sino.C("'$BASE_FOLDER'","'$BINARY_FILE_PATH_1'",'$GPU_NUM')'
  
 wait
sleep 10


