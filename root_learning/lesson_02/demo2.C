#include "TROOT.h"    // to use gROOT point
#include "TObject.h" 
#include "TFile.h"
#include "TTree.h"
#include "TSystem.h"




// .L demo2.C


//  to store an object from "particle_CLASS" in a tree
void example5()
{

    TTree t("t","a simple Tree with class");


   	/*
	|   define a class, and compile it to .so file
    |
	|	.x filename++
	|   
	|   i.g. you have myClass.h; type in .x myClass.h++
    |   then load the .so file.
    |
	*/

    gSystem->Load("particle_CLASS_h.so");
    particle_CLASS* sParticle = new particle_CLASS(); 

    t.Branch("Particle_info",  &sParticle );
   	/*
	|   "Branch name", "pointer's address"
    |   
	|	note: try 
	|   int x = 8; int* xptr = &x;
	|   and check, x, &x, xptr, &xptr, and *xptr
    |   
    |   or "Branch name", "class name", "ptr's address"
	*/

    sParticle->Set_energy(11);
    sParticle->Set_position(1.1, 2.2, 3.3);
    t.Fill();    

    sParticle->Set_energy(99.);
    sParticle->Set_position(191., 291., 391.);
    t.Fill();  


   	/*
	|   new TBrower, and check the tree structure
    |
	|	t.GetListOfLeaves()->Print()
	|   
	|   t.Scan("energy:position[0]:position[1]:position[2]")
    |
	*/

    //-----Save to file------//
    TFile f("example5.root","recreate");  
    t.Write();
    f.Close();

}


