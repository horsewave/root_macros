#include "TROOT.h"    // to use gROOT point
#include "TObject.h" 
#include "TFile.h"
#include "TTree.h"



// read a tree.
void example6(){

    TFile f("example1.root");
    /*
    |   use f->ls(), to see what is inside the file.
    |   there is a tree, called "t"
    */

    
    TTree* t;  f.GetObject("t",t);
    /*
    |   This line is not always necessary,
    |   but if you want to open another root file,
    |   you should set a pointer to reference the obj
    |   otherwise in memory, only the most recently root file exist. 
    */


    /*
    |   We have 3 branches, 
    |   and each branch has one variable(leaf in ROOT terminology)
    |   review: 
    |   the writing out setting:
    |
    |   Int_t money_A
    |   t.Branch("mA", &money_A, "money_A/I");
    |   branch name: "mA"
    |   leaf   name: "money_A", type = Int_t
    */
   

    Int_t AA, BB, CC;
    t->SetBranchAddress("mA",&AA);
    t->SetBranchAddress("mB",&BB);
    t->SetBranchAddress("mC",&CC);
    /*
    |   When reading an existed tree,
    |   we declare variables first,
    |   and then link the variables to the data in tree,
    |   via the "SetBranchAddress" method.
    |
    |   
    |   If you know how the data is writen out,
    |   It is very easy to set, i.g.
    |   Int_t money_A;
    |   t->Branch          ("mA", &money_A, "money_A/I"); //write out
    |   t->SetBranchAddress("mA", &money_A);              //read in
    |
    */


    t->GetEntry(0);
    cout << AA << "\t" << BB << "\t" << CC << endl;

    t->GetEntry(1);
    cout << AA << "\t" << BB << "\t" << CC << endl;

    t->GetEntry(2);
    cout << AA << "\t" << BB << "\t" << CC << endl;


    cout << t->GetEntries() << endl;
    /*
    |   The GetEntries() method 
    |   return the total num of entries in a tree.
    |   
    |   It is very useful, when you want to design a loop.
    |
    */

    Int_t nn = t->GetEntries();

    for(Int_t i=0; i<nn; i++) { 

        t->GetEntry(i); 
        cout << AA << "\t" << BB << "\t" << CC << endl; 
    }


}




// to read in an array
void example7(){

    TFile f("example2.root");
    TTree* t;  f.GetObject("t",t);

     /*
    |   review: 
    |   the writing out setting:
    |
    |   Int_t A_account[2] = {0};
    |   Int_t B_account[3] = {0};
    |   t.Branch("A", A_account, "bankfromA[2]/I");
    |   t.Branch("B", B_account, "bankfromB[3]/I");
    */

    Int_t A_account[2] = {0};
    Int_t B_account[3] = {0};

    t->SetBranchAddress("A", A_account);
    t->SetBranchAddress("B", B_account);

    t->GetEntry(0);
    cout << A_account[0] <<"\n" << A_account[1] << endl;
    cout << B_account[0] <<"\n" \
         << B_account[1] << "\n"\
         << B_account[2] <<endl;

}



// to read in an array with varying size
void example8(){

    TFile f("example3.root");
    TTree* t;  f.GetObject("t",t);

     /*
    |   review: 
    |   the writing out setting:
    |
    |   Int_t energy[n_maxSize] = {0};
    |   Int_t n;
    |
    |   t.Branch("Size", &n, "n/I");
    |   t.Branch("Energy", energy, "particle_enery[n]/I");
    */

    Int_t energy[100];
    Int_t n;

    t->SetBranchAddress("Size", &n);
    t->SetBranchAddress("Energy", energy);

    Int_t nn = t->GetEntries();

    for(Int_t i=0; i<nn; i++) { 
        
        for(Int_t ii=0; ii<100; ii++) { energy[ii] = 0; }

        t->GetEntry(i); 

        cout <<"num = "<< n <<"\n"\
             << energy[0]   <<"\n"\
             << energy[1]   <<"\n"\
             << energy[2]   << endl;
        cout << "-----------------" << endl;
    }

    t->GetEntry(2);
    cout << n <<"\t"<< energy[0] <<"\t"\
                    << energy[1] <<"\t"\
                    << energy[2] << endl;

    t->GetEntry(0);
    cout << n <<"\t"<< energy[0] <<"\t"\
              << energy[1] <<"\t"\
              << energy[2] << endl;
    
    t->GetEntry(1);
    cout << n <<"\t"<< energy[0] <<"\t"\
                    << energy[1] <<"\t"\
                    << energy[2] << endl;

}





// to read in a "struct",
// use the TBanch class
void example9(){

    TFile f("example4.root");
    TTree* t;  f.GetObject("t",t);


    /*
    |   review: 
    |   the writing out setting:
    |
    |   t.Branch("Energy",  &sParticle.energy,  "p_energy/F");
    |   t.Branch("Position", sParticle.position,"pos[3]/F");
    |
    |   or
    |   t.Branch("Particle_info", &sParticle, "e/F:pos[3]/F")
    |   
    */

    // create by t.Branch("Particle_info", &sParticle, "e/F:pos[3]/F")

    struct particle_STRUCT { 
        Float_t energy; 
        Float_t position[3]; 
    };

    particle_STRUCT sParticle;

    /*
    |   
    |   Two steps.
    |   1. Get Branch, 
    |   2. Set the address to the "particle_STRUCT" type variables.
    |
    |   
    */
    TBranch* tb = t->GetBranch("Particle_info");
    tb->SetAddress(&sParticle);
    
    t->GetEntry(0);

    cout << sParticle.energy << "\n" << sParticle.position[0] << "\n" << sParticle.position[1] << "\n" << sParticle.position[2] << endl;




    /*

    //  t.Branch("Energy",  &sParticle.energy,  "p_energy/F");
    //  t.Branch("Position", sParticle.position,"pos[3]/F");
        
        Float_t energy; 
        Float_t position[3]; 
        
        t->SetBranchAddress("Energy", &energy);
        t->SetBranchAddress("Position", position);

    */


}






// to read in an "obj" from "particle_CLASS" class
void example10(){

    // to load the class definition.
    gSystem->Load("particle_CLASS_h.so");
    

    TFile f("example5.root");
    TTree* t;  f.GetObject("t",t);


    // to create an obj to receive.
    particle_CLASS* sParticle = new particle_CLASS(); 

    // to link
    t->SetBranchAddress("Particle_info",  &sParticle );

    t->GetEntry(0);
    cout << sParticle->Get_energy() << endl;
    cout << sParticle->Get_position()[0] << endl;
    cout << sParticle->Get_position()[1] << endl;
    cout << sParticle->Get_position()[2] << endl;


}
