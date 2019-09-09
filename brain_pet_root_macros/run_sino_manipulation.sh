#!/bin/bash
# run two processes in the background and wait for them to finish


SUB_FOLDER_1="run_num_5000/"
SUB_FOLDER_2="run_num_1000/"

SUB_FOLDER_MERGE="merge_6000/"

## if you want to pass integer GPU_NUM, you should omit the ""
#root './Sino_manipulation.C("'$SUB_FOLDER_1'","'$SUB_FOLDER_2'","'$SUB_FOLDER_MERGE'")'
  
 #wait
#sleep 10

SUB_FOLDER_1=$SUB_FOLDER_MERGE
SUB_FOLDER_2="run_num_1400/"

SUB_FOLDER_MERGE="merge_7400/"

# if you want to pass integer GPU_NUM, you should omit the ""
#root './Sino_manipulation.C("'$SUB_FOLDER_1'","'$SUB_FOLDER_2'","'$SUB_FOLDER_MERGE'")'

wait
sleep 10

SUB_FOLDER_1=$SUB_FOLDER_MERGE
SUB_FOLDER_2="run_num_1800/"

SUB_FOLDER_MERGE="merge_9200/"

# if you want to pass integer GPU_NUM, you should omit the ""
root './Sino_manipulation.C("'$SUB_FOLDER_1'","'$SUB_FOLDER_2'","'$SUB_FOLDER_MERGE'")'

wait
sleep 10

SUB_FOLDER_1=$SUB_FOLDER_MERGE
SUB_FOLDER_2="run_num_2200/"

SUB_FOLDER_MERGE="merge_11400/"

# if you want to pass integer GPU_NUM, you should omit the ""
root './Sino_manipulation.C("'$SUB_FOLDER_1'","'$SUB_FOLDER_2'","'$SUB_FOLDER_MERGE'")'

wait
sleep 10

SUB_FOLDER_1=$SUB_FOLDER_MERGE
SUB_FOLDER_2="run_num_2600/"

SUB_FOLDER_MERGE="merge_14000/"

# if you want to pass integer GPU_NUM, you should omit the ""
root './Sino_manipulation.C("'$SUB_FOLDER_1'","'$SUB_FOLDER_2'","'$SUB_FOLDER_MERGE'")'

wait
sleep 10


SUB_FOLDER_1=$SUB_FOLDER_MERGE
SUB_FOLDER_2="run_num_3000/"

SUB_FOLDER_MERGE="merge_17000/"

# if you want to pass integer GPU_NUM, you should omit the ""
root './Sino_manipulation.C("'$SUB_FOLDER_1'","'$SUB_FOLDER_2'","'$SUB_FOLDER_MERGE'")'

wait
sleep 10

SUB_FOLDER_1=$SUB_FOLDER_MERGE
SUB_FOLDER_2="run_num_3400/"

SUB_FOLDER_MERGE="merge_20400/"

# if you want to pass integer GPU_NUM, you should omit the ""
root './Sino_manipulation.C("'$SUB_FOLDER_1'","'$SUB_FOLDER_2'","'$SUB_FOLDER_MERGE'")'

wait
sleep 10


SUB_FOLDER_1=$SUB_FOLDER_MERGE
SUB_FOLDER_2="run_num_3800/"

SUB_FOLDER_MERGE="merge_24200/"

# if you want to pass integer GPU_NUM, you should omit the ""
root './Sino_manipulation.C("'$SUB_FOLDER_1'","'$SUB_FOLDER_2'","'$SUB_FOLDER_MERGE'")'

wait
sleep 10



SUB_FOLDER_1=$SUB_FOLDER_MERGE
SUB_FOLDER_2="run_num_4000/"

SUB_FOLDER_MERGE="merge_28200/"

# if you want to pass integer GPU_NUM, you should omit the ""
root './Sino_manipulation.C("'$SUB_FOLDER_1'","'$SUB_FOLDER_2'","'$SUB_FOLDER_MERGE'")'

wait
sleep 10


SUB_FOLDER_1=$SUB_FOLDER_MERGE
SUB_FOLDER_2="run_num_4200/"

SUB_FOLDER_MERGE="merge_32200/"

# if you want to pass integer GPU_NUM, you should omit the ""
root './Sino_manipulation.C("'$SUB_FOLDER_1'","'$SUB_FOLDER_2'","'$SUB_FOLDER_MERGE'")'

wait
sleep 10


SUB_FOLDER_1=$SUB_FOLDER_MERGE
SUB_FOLDER_2="run_num_4600/"

SUB_FOLDER_MERGE="merge_36800/"

# if you want to pass integer GPU_NUM, you should omit the ""
root './Sino_manipulation.C("'$SUB_FOLDER_1'","'$SUB_FOLDER_2'","'$SUB_FOLDER_MERGE'")'

wait
sleep 10






