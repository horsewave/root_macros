{
  //-----------------------------------------------------------------------------------------
  //---collection of data processing & reconstruction methods
  //---Version V3.01 (C) J.Scheins
  //-----------------------------------------------------------------------------------------
  cout << "##############################################################################" << endl;
  cout << "### ROOT   version 5.34.36 " << endl;
  cout << "### PRESTO devel version  (23-june-2016)" << endl;
  cout << "##############################################################################" << endl;

  //-----------------------------------------------------------------------------------------
  //---all relevant C++ libraries for processing etc.
  //-----------------------------------------------------------------------------------------
  

  //---detect environment
  string hostname        = gSystem->HostName();
  string user            = gSystem->Getenv("USER");
  string rootsys         = gSystem->Getenv("ROOTSYS");
  //string presto_project  = gSystem->Getenv("PRESTO_PROJECT");
  //string presto_project  = "/data/mr_pet_temp/Ma/PRESTO";
  string presto_project  = "/home/mabo/software/src/PRESTO";
  //string presto_project  = "/home/mabo/software/src/PRESTO/PRESTO";
  cout << "----> rootlogon.C" << endl;
  cout << "----> HOST           = " << hostname << endl;
  cout << "----> USER           = " << user << endl;
  cout << "----> ROOTSYS        = " << rootsys << endl;
  cout << "----> PRESTO_PROJECT = " << presto_project << endl;
  
  //---path of C++ processing software PRESTO  
  string presto_bin_path="";
  string presto_lib_path="";
  presto_bin_path=presto_project+"/bin";
  presto_lib_path=presto_project+"/libs";
  cout << "----> PRESTO software lib path: " << presto_lib_path << endl;

  //shell setups for root 
  gSystem->Exec("source /home/mabo/software/src/PRESTO/macros/variables_root.sh");

  //-----------------------------------------------------------------------------------------
  //---load relevant software libraries
  //-----------------------------------------------------------------------------------------
  int nmoduls=15;
  string modul_name[15];
  modul_name[0] ="utils";
  modul_name[1] ="BrainPET_data";
  modul_name[2] ="ECAT7";
  modul_name[3] ="geometry";
  modul_name[4] ="geometryV2";
  modul_name[5] ="reco_speedup";
  modul_name[6] ="efficiency";
  modul_name[7] ="interface2root";
  modul_name[8] ="dkey_maker";
  modul_name[9] ="emission_sim";
  modul_name[10]="BrainPET_reco";
  modul_name[11]="eval_tools";
  modul_name[12]="BrainPET_DataBase";
  modul_name[13]="motion_corr";
  modul_name[14]="BrainPET_presto_proc";
  for (int i=0; i<nmoduls; i++)
    {
      string libname="lib_"+modul_name[i]+"_ROOT.so";
      string libfile=presto_lib_path+"/"+libname;
      cout << "----> load library:  " << libname << endl;
      //gSystem.Load(libfile.c_str());
      gSystem->Load(libfile.c_str());
    }
  
  gSystem.Load("libGpad.so");
  //gSystem->Load("libGpad.so");
  puts("  ");
  cout << "Load finished"<< endl;


  //-----------------------------------------------------------------------------------------
  //---styles
  //-----------------------------------------------------------------------------------------
  TStyle* fStyle=new TStyle("mystyle","");
  fStyle->SetOptTitle(1);
  fStyle->SetTextFont(42);
  fStyle->SetTextSize(0.055);
  fStyle->SetLabelFont(42,"X");
  fStyle->SetLabelFont(42,"Y");
  fStyle->SetLabelFont(42,"Z");
  fStyle->SetLabelOffset(0.01,"X");
  fStyle->SetLabelOffset(0.01,"Y");
  fStyle->SetNdivisions(508,"X");
  fStyle->SetNdivisions(508,"Y");
  fStyle->SetFrameBorderMode(0);
  fStyle->SetCanvasBorderMode(0);
  fStyle->SetPadBorderMode(0);
  fStyle->SetOptStat(0);
  fStyle->SetCanvasColor(10);
  fStyle->SetPadLeftMargin(0.1);
  fStyle->SetPadBottomMargin(0.10);
  //fStyle->SetLabelSize(0.025,"X");  //---23/11/2012
  //fStyle->SetLabelSize(0.025,"Y");
  fStyle->SetLabelSize(0.040,"X");
  fStyle->SetLabelSize(0.040,"Y");

  //fStyle->SetTitleSize(0.028,"X");  //---23/11/2012
  //fStyle->SetTitleSize(0.028,"Y");
  //fStyle->SetTitleSize(0.028,"Z");
  fStyle->SetTitleSize(0.04,"X");
  fStyle->SetTitleSize(0.04,"Y");
  fStyle->SetTitleSize(0.04,"Z");
  fStyle->SetTitleOffset(0.9,"X");
  fStyle->SetTitleOffset(1.0,"Y");
  fStyle->SetCanvasColor(10);
  fStyle->SetFrameLineColor(10);
  fStyle->SetPadColor(10);
  fStyle->SetCanvasColor(10);
  fStyle->SetHistLineColor(10);
  fStyle->SetHistLineWidth(0.5);
  gStyle->SetOptStat(0);
  gStyle->SetPalette(1,0);
  gROOT->SetStyle("mystyle");
  gROOT->ForceStyle();


  //---Utilities toolbox init  
  #include "./Utilities.C"
  gROOT->ProcessLine(".x ./UtilitiesInit.C");







}
