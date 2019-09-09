/*
*    
*    TTree:MakeCode  method and
*    TTree:MakeClass method help us to get the skeleton of a tree.
*
*    But they are not very approriate when the tree structure is 
*    complicated. ex. a class has private variable that is an obj 
*    from another class.
*    
*
*******************************************************************/
{

    //------------------------
    // STEP 1.
    // to create the example1.root if not exists.
    
    gROOT->ProcessLine(".L demo1.C"); example1();


    TFile f("example1.root");
    TTree* t;  f.GetObject("t",t);


    //------------------------
    // STEP 2.
    // to use MakeCode method
    

    t->MakeCode("Code_from_example1.C");
    t->MakeClass("CLASS_from_example1");
    /*
    | parameter: output filename
    | if you don't setup the output filename 
    | the default filename will be tree_Name.C (t.C) 
    |
    | .L CLASS_from_example1.C;
    | CLASS_from_example1* t = new CLASS_from_example1
    | t->Show(0)
    | 
    */

     /*
    | 
    | when you use a TChain obj,
    | you can only use MakeCode.
    |
    */



}
