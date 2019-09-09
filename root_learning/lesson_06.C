/*
*    In this tutorial, 
*    We create tiny.root, and tiny_1.root ....
*    and then use TChain obj to link them into a "single tree".
*
*******************************************************************/
{
    //------------------------
    // STEP 1.
    // to create a tree with small size
    /*
    |   The default value is 100 Gigabytes.
    */

    TFile* f = new TFile("tiny.root","recreate");

    TTree* t = new TTree("t","a simple Tree");

    t->SetMaxTreeSize(500); 
 

    Int_t money_A, money_B, money_C;




    t->Branch("mA", &money_A, "money_A/I");
    t->Branch("mB", &money_B, "money_B/I");
    t->Branch("mC", &money_C, "money_C/I");

    for(Int_t i=0; i< 100*500; i++) { 
        money_A = 10; 
        money_B = 30; 
        money_C = 60; 
        t->Fill(); 
    }
   
    t->Write();           
    gROOT->ProcessLine(".!ls -lthr");
    //f->Close();
    
    //------------------------
    // STEP 2.
    // to use TChain to concatenate several root files.

    
    TChain chain("t");   
    chain.Add("tiny.root")
    chain.Add("tiny_1.root")
    chain.Add("tiny_2.root")
    // In each root file, there is a tree "t".

    /*
    | We use the Add() method to connect the root files.    
    | TChain obj works like a tree. Try the following commands.
    |
    | chain.Print();
    | chain.Scan("money_A:money_B:money_C");
    | chain.Draw("money_A");  
    | chain.GetEntries()
    |                    
    */
    
    

    /*
    |  When retrieving the branch,
    |  we use the SetBranchAddress() method.
    |   ( using TBranch is not allowed.)
    */
    
    Int_t AA;
    chain.SetBranchAddress("mA", &AA);

    chain.GetEntry(0);
    cout << AA << endl;



}
