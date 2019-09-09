#include "TROOT.h"//to use gROOT point
#include "TObject.h"
#include "TFile.h"
#include "TTree.h"



void lesson_05_root_read_tree()
{
  example_9();

}

//to read in a struct
//use the TBranch class.
void example_9()
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
   *  Review:
   *  the writing out setting:
   *
   *  t.Branch("Energy",&sParticle.energy,"p_energy/F");
   *  t.Branch("Position",&sParticle.position,"pos[3]/F");
   *
   *  or:
   *  t.Branch("Particle_info",&sParticle,"e/F:pos[3]/F");
   *
   */

  //create by  t.Branch("Particle_info",&sParticle,"e/F:pos[3]/F");



  struct particle_STRUCT{
    Float_t energy;
    Float_t position[3];
  };

  particle_STRUCT sParticle;
   
  /*
   *  Two steps:
   *  1. Get Branch,
   *  2. Set the address to the "particle_STRUCT" type variables
   *
   */
 

  TBranch* tb=t->GetBranch("Particle_info");
  tb->SetAddress(&sParticle);


 
  Int_t nn=t->GetEntries();

  for(Int_t i=0;i<nn;i++)
  {
    t->GetEntry(i);
  cout<<sParticle.energy<<endl;
  cout<<sParticle.Position[0]<<endl;

  }





  f.Close();
}

// to read in an "obj" from "particle_class" class 
void example10()
{
  TFile f("test2.root");

  TTree* t;
  f.GetObject("t",t);

  /*
   *  to load the class definition:
   *  gSystem->Load("particle_CLASS.so");
   *
   */
gSystem->Load("particle_CLASS.so");


//to create an obj to receive.
particle_CLASS* sParticle=new particle_CLASS();

//to link


  t->SetBranchAddress("Particle_info",&sParticle);

  Int_t nn=t->GetEntries();

  for(Int_t i=0;i<nn;i++)
  {
    t->GetEntry(i);
  cout<<sParticle->Get_energy()<<endl;
  cout<<sParticle->Get_Position()[0]<<endl;

  }


}


