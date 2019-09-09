/*
 *  In this lesson,
 *  We create tiny.root, and tiny_1.root..... 
 *  and then use TChain obj to link them into a "single tree"
 *
 */


void lesson_06_root_tchain()
{
  /*
   *  STEP 1:
   *  to create a tree with small size
   *  
   *  the default value is 100 Gigabytes
   *
   */

  TFile* f=new TFile("./tiny_root/tiny.root","recreate");
  TTree* t=new TTree("t","a simple Tree");

  /**
   *  the max entry size is 500, if it is bigger than 500 entries, it will create another root file.
   *  But it has to have the first container,which is tiny.root.
   *
   * */
  t->SetMaxTreeSize(500);

  Int_t money_A;
  Int_t money_B;
  Int_t money_C;

  t->Branch("mA",&money_A,"money_A/I");
  t->Branch("mB",&money_B,"money_B/I");
  t->Branch("mC",&money_C,"money_C/I");

  for(Int_t i=0;i<100*500;i++)
  {
    money_A=10;
    money_B=30;
    money_C=60;
    
    t->Fill();
  }

  t->Write();

  gROOT->ProcessLine(".!ls -lthr ./tiny_root/");

  /*f->ls();*/

    /*f->Close();//not necessary anymore */


  /****
   *
   *  STEP 2:
   *  to use TChain to concatenate several root files
   */

  TChain chain("t");
  chain.Add("./tiny_root/tiny.root");
  chain.Add("./tiny_root/tiny_1.root");
  chain.Add("./tiny_root/tiny_2.root");
  //in each root file, there is a tree "t".

  /*
   *  We use the Add() method to connect the root files.
   *  TChain obj works like a tree. try the following command.
   *  chain.Print();
   *  chain.Scan("money_A:money_B:money_C","","",start,end);
   *  chain.Draw("money_A");
   *  chain.GetEntries();
   *
   */

}
