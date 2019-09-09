/*
*    The scenario: 
*    We have example1.root to store the money of brother A,B,C.
*    suppose we create another tree that stores the money for
*    the parents, Mom and Dad.
*
*    We can use TTree::AddFriend method 
*    to attach the extra data to existed data.
*    
*
*******************************************************************/
{
    //------------------------
    // STEP 1.
    // to create the example1.root if not exists.
    // in example1.root, 
    // we have data that represent the money for brother A,B,C.

    gROOT->ProcessLine(".L demo1.C");
    example1();


    TFile f("example1.root");
    TTree* tBrother;  f.GetObject("t",tBrother);



    //------------------------
    // STEP 2.
    // to create the another tree,
    // for he money for Mom and Dad.
    Int_t money_Mom;
    Int_t money_Dad;

    TTree* tParent = new TTree("tParent"," a friend");
    tParent->Branch("mM", &money_Mom, "money_Mom/I");
    tParent->Branch("mD", &money_Dad, "money_Dad/I");

    money_Mom = 500; money_Dad = 600;  tParent->Fill();
    money_Mom = 100; money_Dad = 600;  tParent->Fill();
    money_Mom = 100; money_Dad = 700;  tParent->Fill();
    money_Mom = 100; money_Dad = 800;  tParent->Fill();
    money_Mom = 100; money_Dad = 900;  tParent->Fill();
    
    TFile f2("example1_friend.root","recreate");
    tParent->Write();
    f2.Close();

  
    
    //------------------------
    // STEP 3.
    // To use AddFriend method

    
    tBrother->AddFriend("tParent","example1_friend.root")
    /*
    | parameter1: the name of the tree in friend (or called key).
    | parameter2: the root filename.
    |                    
    */


    /*
    |   To access the variables in friend's tree: 
    |                    
    |   we have to use:
    |   tParent.money_Mom  (tree_name.leaf_name)
    */
    tBrother->Scan("money_A:tParent.money_Mom:tParent.money_Dad");
    tBrother->Draw("tParent.money_Dad");
    /*
    |   note: 
    |   tBrother tree only has 3 entries
    |   so we can only have 3 entries from friend's tree.   
    */

}
