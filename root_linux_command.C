/*
 *  Description: run linux shell command in root macros
 *  
 *  Editor: Bo Ma
 *  Date: 3018.12.06
 */


void root_linux_command()
{
  // ------1: run root file in root .
  gROOT->ProcessLine(".x /data/Weirich/root_macros/rootlogon_devel.C");

  // ------2: make directory
  string my_folder="/data/Ma";
  char command_buffer[1024];
  sprintf(command_buffer, ".! mkdir %s", my_folder.c_str()); 
  gROOT->ProcessLine(command_buffer);



}
