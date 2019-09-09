#!/bin/bash
# run two processes in the background and wait for them to finish


#BASE_FOLDER="/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/cylinder_phantom_3_fov_20_cm/scatterMCS/run_num_5000/"

#BINARY_FILE_PATH=$BASE_FOLDER"cylinder_phantom_3_fov_20_cm_gpuSimu_0.bin"
#SAVED_ROOT_FILE_PATH=$BASE_FOLDER"my_root_file.root"

#root './read_binary_data_to_root.C("'$BINARY_FILE_PATH'","'$SAVED_ROOT_FILE_PATH'")'
  
 #wait
#sleep 100



#BASE_FOLDER="/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/Random-OFOV-activity-C11-2016-09-08/XB1BN286N-BI-01/scatterMCS/simu_num_5000_new_1/"

#BINARY_FILE_PATH_1=$BASE_FOLDER"XB1BN286N-BI-01_gpuSimu_0.bin"
#BINARY_FILE_PATH_2=$BASE_FOLDER"XB1BN286N-BI-01_gpuSimu_1.bin"
#SAVED_ROOT_FILE_PATH=$BASE_FOLDER"my_root_file.root"

#root './read_binary_data_to_root.C("'$BINARY_FILE_PATH_1'","'$BINARY_FILE_PATH_2'","'$SAVED_ROOT_FILE_PATH'")'
  
 #wait
#sleep 10





BASE_FOLDER="/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/Random-OFOV-activity-C11-2016-09-08/XB1BN286N-BI-09/scatterMCS/"

BINARY_FILE_PATH_1=$BASE_FOLDER"XB1BN286N-BI-09_gpuSimu_0.bin"
BINARY_FILE_PATH_2=$BASE_FOLDER"XB1BN286N-BI-09_gpuSimu_1.bin"
SAVED_ROOT_FILE_PATH=$BASE_FOLDER"my_root_file.root"

root './read_binary_data_to_root.C("'$BINARY_FILE_PATH_1'","'$BINARY_FILE_PATH_2'","'$SAVED_ROOT_FILE_PATH'")'
  
 wait
sleep 10

