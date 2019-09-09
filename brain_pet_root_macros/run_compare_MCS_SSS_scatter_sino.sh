#!/bin/bash
# run two processes in the background and wait for them to finish


BASE_FOLDER="/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/XB1BN305N-BI/XB1BN305N-BI-01/"

FRAME_TIME="0-1800"
PART_NAME_MCS="scatterMCS/normed_scaled_scatter.fs"
PART_NAME_SSS="scatterSSS/range_"$FRAME_TIME"_scal_scat.fs"
PART_NAME_ROOT_FILE="root_file/compare_MCS_SSS_scatter_sino.root"


PATH_SINO_MCS=$BASE_FOLDER$PART_NAME_MCS
PATH_SINO_SSS=$BASE_FOLDER$PART_NAME_SSS
PATH_SAVED_ROOT=$BASE_FOLDER$PART_NAME_ROOT_FILE

# if you want to pass integer GPU_NUM, you should omit the ""
root './compare_MCS_SSS_scatter_sino.C("'$PATH_SINO_MCS'","'$PATH_SINO_SSS'","'$PATH_SAVED_ROOT'")'
  
 wait
sleep 10




