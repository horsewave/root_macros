#include "TROOT.h"    // to use gROOT point
#include "TObject.h" 
#include "TFile.h"
#include "TTree.h"


// .L demo1.C


void example1()
{


    TTree t("t","a simple Tree");
    // key = t
	// title = a simple tree

	
	/*
	|	suppose we have 3 brothers, A, B, and C
	|	each time the mother will distribute $100 to them,
	|	Let use: 
	|	money_A for brother A
	|	money_B for brother B
	|	money_C for brother C	
	*/
	
	Int_t money_A, money_B, money_C;


	// now, link variables "money_A", "money_B", money_C
	/*
	|	
	|	3 branchs, and each branch has 1 leaf.
	|	b1->
	|	b2->
	|	b3->
    |
	|	or 1 branch, 3 leaves. ( discuss latter)
	|	b->lv1
	|	 ->lv2
	|	 ->lv3
	*/


	/* 
	|	use Branch method, to set up a branch.
    |  (key for the branch, address of variable, leaf and its type)  	 |
	*/
	
	// a branch "mA", linked to variable money_A, with one leaf.
    t.Branch("mA", &money_A, "money_A/I");
    t.Branch("mB", &money_B, "money_B/I");
    t.Branch("mC", &money_C, "money_C/I");

    money_A = 10;   money_B = 30;   money_C = 60;   t.Fill();
    money_A = 20;   money_B = 30;   money_C = 50;   t.Fill();
    money_A = 50;   money_B = 35;   money_C = 15;   t.Fill();


	//-----Save to file------//
	TFile f("example1.root","recreate");   
	t.Write();
	f.Close();


    /*
    |   I: 	32 bit signed integer 
    |   i: 	32 bit unsigned integer 
    |   F: 	32 bit floating point 
    |   D: 	64 bit floating point 
    */

	
}



// example 2 : filling a tree with an array

void example2()
{


    TTree t("t","a Tree");

	/*
	|	suppose: 2 brothers, A and B
	|	Brother A have 2 bank accounts.
	|	Brother B have 3 bank accounts.	
    */
	
	Int_t A_account[2] = {0};
    Int_t B_account[3] = {0};



	
	/*
    |   create a branch, and a leaf to store the array
    |   a branch "A", linked to arrary A_account, 
    |   with one leaf "bank" .
    */
    t.Branch("A", A_account, "bankfromA[2]/I");.
    t.Branch("B", B_account, "bankfromB[3]/I");

    
    A_account[0] = 10;  
    A_account[1] = 110;  

    B_account[0] = 9;  
    B_account[1] = 99; 
    B_account[2] = 999; 
    t.Fill();


    A_account[0] =  20;  
    A_account[1] = 120;  
 
    B_account[0] = 7;  
    B_account[1] = 77; 
    B_account[2] = 777; 
    t.Fill();

	//-----Save to file------//
	TFile f("example2.root","recreate");   
	t.Write();
	f.Close();
	
}







// example 3 : filling a tree with an array with variable size
void example3()
{
    
    TTree t("t","a Tree");

	/*
	|	Declare an array, and set up the size.
    */
	
    Int_t n_maxSize = 100;

    Int_t energy[n_maxSize] = {0};

    Int_t n;
    
    t.Branch("Size", &n, "n/I");
    t.Branch("Energy", energy, "particle_enery[n]/I");
	/*
    |   ! set up a branch to store "n" first
    |
    |   then create a branch "Energy", 
    |   and a leaf to store the array 
    |   with its size linking to variable "n"
    |
    |   
    */

    n=1;
    energy[0]=9;
    t.Fill();

    n=2;
    energy[0]=11;
    energy[1]=111;
    t.Fill();

    n=3;
    energy[0]=3;
    energy[1]=33;
    energy[2]=333;
    t.Fill();

    /*
    |   read the result by following command:
    |   t.Scan("n:particle_enery","","colsize=20")
    */

	//-----Save to file------//
	TFile f("example3.root","recreate");   
	t.Write();
	f.Close();
	
}


    




//  to store in a the "struct" data type.
void example4()


{

	/*
	|	you can set up multiple leaves in a branch,
	|   
	|    "struct" or "class", 
	|   here I demonstrate the "struct" first 
    |	But it is recommended to use "class"
    |
	*/

    struct particle_STRUCT { 
        Float_t energy; 
        Float_t position[3]; 
    };


    particle_STRUCT sParticle;

    TTree t("t","a simple Tree with struct");

    t.Branch("Energy",  &sParticle.energy,  "p_energy/F");
    t.Branch("Position", sParticle.position,"pos[3]/F");

	/*
	|	use "." operator to access the element inside struct
	|   
	|   the rules of setup the leaf and branach are the same.
    |
    |   or 
    |   t.Branch("Particle_info", &sParticle, "e/F:pos[3]/F")
    |   t->Scan("e:pos")
	*/
    
    sParticle.energy = 11.;
    sParticle.position[0] = 7.7;
    sParticle.position[1] = 2.2;
    sParticle.position[2] = 3.3;
    t.Fill();    

    sParticle.energy = 99.;
    sParticle.position[0] = 191.;
    sParticle.position[1] = 291.;
    sParticle.position[2] = 391.;
    t.Fill();
    //-----Save to file------//
    TFile f("example4.root","recreate");  
    t.Write();
    f.Close();

}





