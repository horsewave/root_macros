{
//  gROOT->ProcessLine(".x /data/mr_pet_temp/Caldeira/LSO/rootlogon_rev340_root53405.C");
// gROOT->ProcessLine(".x /data/mr_pet_temp/Caldeira/LSO/rootlogon_rev379_root53405.C");
gROOT->ProcessLine(".x /data/mr_pet_temp/Caldeira/LSO/rootlogon_rev383_root53405.C");

/*  
  //---init basic utilities
  string setup_fname="/data/BrainPET_Sys/User/Caldeira/simOne2One/macros/GlobalSetup_One2OneMap.txt";
  GlobalSetup* global_setup=new GlobalSetup(setup_fname);
  GlobalSetup* Setup=global_setup;
  cout << Setup->utilities_mapping_shortcut_refpos << endl;
  Setup->utilities_mapping_shortcut_refpos="/data/BrainPET_Sys/Software/TemplateProcData/version1/presto/mapping/DefaultBrainPET_refpos.MappingShortcut.dat";
  cout << Setup->utilities_mapping_shortcut_refpos << endl;
  cout << Setup->userlist_file << endl;
  Utilities* utils=new Utilities(*global_setup);
  SiemensTools* stools=new SiemensTools(utils);

  //---init of cluster handling
  string fname_userlist="/data/BrainPET_Sys/global/ClusterAccess/admin/Cluster_UserList.dat";


  cout <<" Problem here ? " << endl;
  Utils_Cluster64* clu64=new Utils_Cluster64(fname_userlist);
  cout <<" Problem here ? " << endl;

  clu64->info_cluster_setup(); 

  cout << " " << endl;
  cout << " " << endl;
  cout << "instance access: GlobalSetup     : global_setup" << endl;
  cout << "instance access: Utils_Cluster64 : clu64" << endl;
  cout << "instance access: Utilities       : utils" << endl;

  int nclients=4;
  clu64->init_clients(nclients);

  //---init of SysMatrix Handling
  Utils_SysMatrix* mat_tool=new Utils_SysMatrix(*clu64);
  mat_tool->version_utils_sysmat();

  gROOT->ProcessLine(".x /data/BrainPET_Sys/Software/ProcTools/projects/caldeira/macros/definitions.C");
 
*/


}
