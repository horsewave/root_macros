#include "TROOT.h"//to use gROOT point
#include "TObject.h"
#include "TFile.h"
#include "TTree.h"



void lesson_04_root_read_tree()
{
  example_6();

}

//read a tree
void example_6()
{
  TFile f("./tree_file.root");
  f.ls(); 
  /*
   *  use f->ls() to see what is inside the file;
   *  there is a tree, called "t"
   *
   */

  TTree* t;
  f.GetObject("t",t);
  t->Print();

  /*
   *  This line is not always necessary,
   *  but if you want ot open another root file,
   *  you should set a pointer to reference the obj
   *  otherwise in memory, only the most recently root file exist.
   *
   */


  /*
   * we have 3 brancehs,
   * and each branch has one variable(leaf in ROOT terminoloty)
   * review:
   * the writting out setting:
   * Int_t money_A
   * t.Branch("mA",&money_A,"money_A/I");
   * branch name:"mA"
   * leaf name : "money_A",type=Int_t
   *
   */

  Int_t AA, BB,CC;
  t->SetBranchAddress("mA",&AA);
  t->SetBranchAddress("mB",&BB);
  t->SetBranchAddress("mC",&CC);
  /*
   *  When reading an existed tree, 
   *  we declare variables first,
   *  and then link the variables to the data in tree,
   *  via the "SetBranchAddress" function
   *
   *
   * if you know how the data is writen out,
   * it is very easy to set, i.g.
   * Int_t money_A
   * t->branch("mA",&money_A,"money_A/I");//write out
   *
   * t->SetBranchAddress("mA",&money_A); //read in
   *
   *
   */

  t->GetEntry(0);
  cout<<AA<<"\t"<<BB<<"\t"<<CC<<endl;

t-t->GetEntry(1);
  cout<<AA<<"\t"<<BB<<"\t"<<CC<<endl;

t->GetEntry(2);
  cout<<AA<<"\t"<<BB<<"\t"<<CC<<endl;



  cout<<t->GetEntries()<<endl;
  /*
   * The Getentries() method
   * returns the total num of entries in a tree.
   *
   * it is very usefull, when you want ot design a loop
   *
   */ 


  Int_t nn=t->GetEntries();
  for(Int_t i=0;i<nn;i++)
  {
    t->GetEntry(i);
  cout<<AA<<"\t"<<BB<<"\t"<<CC<<endl;

  }





  f.Close();
}

// to read in an array
void example7()
{
  TFile f("test2.root");

  TTree* t;
  f.GetObject("t",t);

  /*
   *  review:
   *  the writing out setting:
   * Int_t A_account[2]={0};
   * Int_t B_account[3]={0};
   * t.Branch("A", A_account,"bankfromA[2]/I")
   * t.Branch("B", B_account,"bankfromB[3]/I")
   *
   *
   */

  Int_t A_account[2]={0};
  Int_t B_account[3]={0};

  t->SetBranchAddress("A",A_account);
  t->SetBranchAddress("B",B_account);
}

// to read in an array with variable size
void example8()
{
  TFile f("example_3.root");

  TTree* t;
  f.GetObject("t",t);


  /*
   *   Review:
   *   the writing out setting:
   *   Int_t energy[n_maxSize]={n};
   *   Int_t n;
   *   t.Branch("Size",&n,"n/I");
   *   t.Branch("Energy",energy,"particle_energy[i]/I");
   *
   *
   */


  Int_t energy[100];
  Int_t n;
  t->SetBranchAddress("Size",&n);
  t->SetBranchAddress("Energy",&energy);
}
