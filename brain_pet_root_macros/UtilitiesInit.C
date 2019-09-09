{
    //---init basic utilities variables
    string setupfile="";
    string presto_env_setupfile="";
    if (hostname=="petcuda01")
    {
	//setupfile="/local/data1/sys/PRESTO_LOCAL/macros/GlobalSetup_petcuda01_local_v1.txt"; 
	//presto_env_setupfile="/data/BrainPET_Sys/global/DataBase/DBv3/presto/setup/PrestoEnvironment_Setup_v1.txt";
	setupfile="/home/mabo/software/src/PRESTO/setup_files/GlobalSetup_petpc20and21_local_v1.txt";
  presto_env_setupfile="/home/mabo/software/src/PRESTO/setup_files/files_needed/DBv3/presto/setup/PrestoEnvironment_Setup_v1.txt";
    }
    else
    {
	//setupfile="/data/BrainPET_Sys/global/DataBase/DBv3/setup/GlobalSetup_petpc20and21_local_v1.txt";
	//presto_env_setupfile="/data/BrainPET_Sys/global/DataBase/DBv3/presto/setup/PrestoEnvironment_Setup_v1.txt";
	setupfile="/home/mabo/software/src/PRESTO/setup_files/GlobalSetup_petpc20and21_local_v1.txt";
  presto_env_setupfile="/home/mabo/software/src/PRESTO/setup_files/files_needed/DBv3/presto/setup/PrestoEnvironment_Setup_v1.txt";
    }
    
    GlobalSetup* global_setup     = NULL;
    Utilities* utils              = NULL; 
    SiemensTools* stools          = NULL; 
    PrestoEnvironment* presto_env = NULL;


    //---init & dump Utilities toolbox
    Utilities_init();  

   
}
