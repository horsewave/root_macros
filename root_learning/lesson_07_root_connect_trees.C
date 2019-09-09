/*
 *  The scenario:
 *  We have example1.root to store the money of brother A,B,C.
 *  suppose we create another tree that sores the money for the 
 *  parents, Mom and Dad.
 *
 *  We can use TTree::AddFriend method
 *  to attach the extra data to existed data.
 *
 * 
 *
 */


void lesson_07_root_connect_trees()
{
  /*
   *  STEP 1:
   *  to create theexample1.root if not exists 
   *  in example1.root
   *  we have data that represent the money for brother A,B,C
   *
   *
   */

  //use other root macros
  gROOT->ProcessLine(".L ./lesson_02_root_write_tree.C");
  example_1();

  TFile f("example_1.root");

  TTree* tBrother;
  f.GetObject("t",tBrother);

   /*
   *  STEP 2:
   *  to create another tree
   *  for the money for Mom and Dad
   *
   */

  Int_t money_Mom;
  Int_t money_Dad;

  TTree* tParent=new TTree("tParent","a friend");
  tParent->Branch("mM",&money_Mom,"money_Mom/I");
  tParent->Branch("mD",&money_Dad,"money_Dad/I");

  money_Mom=500;money_Dad=600;tParent->Fill();
  money_Mom=500;money_Dad=600;tParent->Fill();
  money_Mom=500;money_Dad=600;tParent->Fill();
  money_Mom=500;money_Dad=600;tParent->Fill();
  money_Mom=500;money_Dad=600;tParent->Fill();

  TFile f2("example1_friend.root","recreate");
  tParent->Write();
  f2.Close();

 /*
   *  STEP 3:
   *  use AddFriend method
   */

  tBrother->AddFriend("tParent","example1_friend.root");

 /*
   *  parameter1: the name of the tree in friend or called key
   * parameter2: the root filename.
   */
  
  
  /*
   *  to access the variables in friend's tree'
   * we have to use tParent.money_Mom (tree_name.leaf_name)
   *
   */

  tBrother->Scan("money_A:tParent.money_Mom");
  tBrother->Draw("tParent.money_Mom");

  /*
   *  note:
   *  tBrother tree only has 3 entries,
   *  so we can only have 3 entries form friend's tree''
   */




 
 
f.Close();
}
