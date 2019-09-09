/*
 * TTree:MakeCode and
 * TTree:MakeClass help us to get the skeleton of a tree.
 *
 * But they are very approriate when the tree structure is complicated. e.g a class has private variable that is an obj form another class.
 */


void lesson_08_root_MakeCode_MakeClass()
{
  /*
   *  STEP 1:
   *  create the example_1.root if not exists.
   *
   *
   */

  //use other root macros
  gROOT->ProcessLine(".L ./lesson_02_root_write_tree.C");
  example_1();

  TFile f("example_1.root");

  TTree* t;
  f.GetObject("t",t);

   /*
   *  STEP 2:
   * use MakeCode and MakeClass method. 
   *
   */


  t->MakeCode("code_form_example1.C");
  t->MakeClass("class_form_example1.C");



  /*  parameter: output filename 
   *  if you don't setup the output filename,
   *  the default filename will be tree_name.C'a
   *
   *
   *  .L class_form_example1;
   *  class_form_example1* t=new class_form_example1;
   *  t->show(0);
   */
  
  
 /*  
  *   when you use a TChain obj,
  *   you can only use MakeCode
  *
  *
   */
 

 
 
f.Close();
}
