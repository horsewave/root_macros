#!/bin/bash
# run two processes in the background and wait for them to finish



PATIENT_NAME="XB1BN286N-BI-09"
INPUT_FOLDER="/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/Random-OFOV-activity-C11-2016-09-08/"$PATIENT_NAME"/"
FRAME_TIME="0-1222"

# if you want to pass integer GPU_NUM, you should omit the ""
#root './Lor_to_sino.C("'$BASE_FOLDER'","'$BINARY_FILE_PATH_1'")'
root './Image_reconstruction_Simens.C("'$PATIENT_NAME'","'$INPUT_FOLDER'","'$FRAME_TIME'")'
  
 wait
sleep 10




#PATIENT_NAME="XB1BN286N-BI-01"
#INPUT_FOLDER="/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/Random-OFOV-activity-C11-2016-09-08/"$PATIENT_NAME"/"
#FRAME_TIME="0-1222"

## if you want to pass integer GPU_NUM, you should omit the ""
##root './Lor_to_sino.C("'$BASE_FOLDER'","'$BINARY_FILE_PATH_1'")'
#root './Image_reconstruction_Simens.C("'$PATIENT_NAME'","'$INPUT_FOLDER'","'$FRAME_TIME'")'
  
 #wait
#sleep 10


