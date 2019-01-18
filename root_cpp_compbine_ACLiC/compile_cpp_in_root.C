{
  /************************************************************
   * 1: How to compile a c++ file
   *********************************************************/
  string aclic_compile=".L ./utilslib.cpp+";
  gROOT->ProcessLine(aclic_compile.c_str());

  // 1): This is the default one. You can also set your own setups like following

               //options: (1): "+":The + option generates the shared library and names it by taking the name of the file “filename” but replacing thedot before the extension by an underscore and by adding the shared library extension for the current platform. Forexample on most platforms, hsimple.cxx will generate hsimple_cxx.so.
               //string aclic_compile=".L ./utilslib.cpp+";

              //(2):"++"To ensure that the shared library is rebuilt you can use the ++ syntax:
                    //string aclic_compile=".L ./utilslib.cpp++";
              //(3): ++g: will compile MyScript.C with debug symbols; usually this means giving the -g option to compiler.
                    //string aclic_compile=".L ./utilslib.cpp++g";
  
                 //(4):++O: will compile MyScript.C with optimizations; usually this means giving the -O option to compiler
  
    //(2:set the default compile option -g or -O

      //gSystem->SetAclicMode(TSystem::kDebug);
      //gSystem->SetAclicMode(TSystem::kOpt);
   
    //(3: set Include folder 
   /*string inc_path="./";*/
   /*string include_command="-I"+inc_path;*/
   
  //add the include path
   //gSystem->AddIncludePath(include_command.c_str());;

    //4): Link a library 
        //This is especially useful for static libraries.
        //gSystem->AddLinkedLibs("-L/my/path -lanylib");

          //For shared ones you can also simply load them before trying to compile the script:
          //gSystem->Load("mydir/mylib");


/************************************************************
   * 2: How to use the classes and library you have difined see root_linkdef.h
   *********************************************************/
 
  }
