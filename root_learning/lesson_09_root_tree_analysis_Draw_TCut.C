/*
 *  In this tutorial,
 *  we will explore the TCut, TcutG,
 *  and some usefull skills for Draw() and Scan() method
 *
 *
 */

void lesson09_root_tree_analysis_Draw_TCut()
{
  //-----------part 1: Draw-----------------
  
  TFile* f=new TFile("example_1.root");

  TTree* t;
  t->GetObject("t",t);

  TCanvas* c1=new TCanvas("c1","my histo",500,800);
  c1->Divide(1,2);
  /*
   *  set a Canvas with the title "my histo"
   *  width =500 px,
   *  height =800 px.
   *  divede to sub_pad ,1 col and 2 row.
   */

  c1->cd(1);
  t->Draw("money");
 
  c1->cd(2);
  t->Draw("money","money<2000");

 /*
   * whole data, and partial data from the selection.
   * x axis:money value,
   * y axis: counts(frequency)
   */

  c1->cd(1);
  t->Draw("log(money)");
  t->Draw("TMath::Log(money)");
  t->Draw("sqrt(money)");
 /*
   * you can use math,
   * e.g. you can call math function, and get the return value.
   */

 


  c1->cd(1);
  t->Draw("money","money>1000 && money <2000");

  c1->cd(2);
  t->Draw("money","money<1000 || money >2000");

 /*
   * logic operator
   * &&, || etc to combine the selection.
   *
   */


  /*
   *  using TCut obj
   *  to make the selection input easier.
   *
   */

  TCut cut1="money<1000";
  TCut cut2="money>2000";

 c1->cd(1);
  t->Draw("money");

  c1->cd(2);
  t->Draw("money", cut1 ||cut2 );




  TH1* h1=new TH1();
  TH1* h2=new TH1();
  // "goff" option, not draw  right now
  t->Draw("money>>h1(2000,0,4000)", "","goff");
  t->Draw("money>>h2(2000,0,4000)", cut1 ||cut2,"goff" );


 c1->cd(1);
 h1->Draw();

  c1->cd(2);
 h2->Draw();

/*
   * redirect to histogram
   * num of bin =2000
   * x-axils from 0-4000
   * 
   * In this way, it is convenient histo to files.
   */



}
