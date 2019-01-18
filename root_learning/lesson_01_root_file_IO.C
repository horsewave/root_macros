//lesson 1:CERN ROOT tutorial for beginners -- 1. File I/O (TFile Class)
//
void lesson_01_root_file_IO()
{
  example_1();
  //example_2();
  //example_3();
  

}

void one()
{
  //1:-----create a pointer that can reference to TFile class----
  TFile* f=new TFile("./demo.root");
  f->close();

  //2:---create instance from TFile class-------
  TFile f("./demo.root","recreate")//to overwrite;
  TFile f("./demo.root")//to read;
    TFile f("./demo.root","update")//to append;
    f.Close();
// 3:----
    f.ls()//to list what is in this TFile object"f";

    //tip: use tab to auto complete the  input;
    //tip:Root file is independent of .root file extension.

}

//example 1: basic read and write a root file;
void example_1()
{
  TFile* f=new TFile("./test.root","recreate");

  f->ls();

  TH1F* h0=new TH1F("h0","histogram0",100,-10,10);
  TH1F* h1=new TH1F("h1","histogram1",100,-10,10);

  h0->FillRandom("gaus",100);
  h1->FillRandom("gaus",100);

  h0->Write();
  h1->Write();

  f->ls();

  //h0->Draw();
  h0->FillRandom("gaus",200);

  h0->Write();
  //h0->Draw();
  f->Close();

}

//example 2: more functions

void example_2()
{
  /*TFile::mkdir("char");//to mkdir;*/
  //TFile::cd();//to change dir;
  //TFile::delete();//to delete element or dir;
  //TFile::ls("-m");//to see objects in memory;
  //gDirectory::pwd();//to show the current path;
  //
  //
  TFile* f1=new TFile("test1.root","recreate");
  TFile* f2=new TFile("test2.root","recreate");

  gDirectory->pwd();

  f1->cd();
  f1->ls();

  f1->mkdir("new_dir");

  f1->ls();
  f1->cd("new_dir");

  new TBrowser


  f1->Close();
  f2->Close();
  
}
 
//root file provide sequential access and direct access
void example_3()
{
  TFile* f=new TFile("demo.root");

  //print all the keys
  f->GetListOfKeys()->Print();
  f->ls();

}
