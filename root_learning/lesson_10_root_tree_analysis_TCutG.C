/*
 *  In this tutorial,
 *  we will explore the TCut, TcutG,
 *  and some usefull skills for Draw() and Scan() method
 *
 *
 */

void lesson_10_root_tree_analysis_TCutG()
{
  //-----------part 2: Draw with TCutG-----------------
  
 
  TCanvas* c2=new TCanvas("c2","my 2D histo",500,500);

  c2->Divide(1,2);
  
  TH2* hxy=new TH1();
  t->Draw("x:y>>hxy(2000,0,1500,2000,0,12000)","","colz");
 
  /*
   *  x-axis =value of x
   *  y-axis =value of y
   *  color =counts(frequency)
   *  x-axis range =0 to 1500
   *  y-axis range =0 to 12000
   *  option: colz, surf, lego, etc.....
   *
   */

   /*
   *   to make a graphic cut: steps:
   *   1) at menu bar "viwe"->"Toolbar",
   *   2) at toobar,select the icon of scissors,
   *   3)use mouse left click to draw a region,
   *   4) use double click to finish the region,
   *   5) select the outline, and right click -->"setName",
   *   6)give a name ,,e.g.mycut.
   *
   */

 
  t->Draw("x:y>>hxy(2000,0,1500,2000,0,12000)","mycut","colz");
  t->Draw("x:y>>hxy(2000,0,1500,2000,0,12000)","!mycut","colz");


  c2.cd(1);
  t->Draw("money","mycut","");

  c2.cd(2);
  t->Draw("money","","");

  TFile g("cut.root","recreate");
  mycut->Write();
  g.Close();

/*
   *  save the mycut to file,then you can reuse it again.
   *  you can also modify it.
   *
   */

  TCanvas* c3=new TCanvas("c2","cut test",500);
  mycut->Draw();

  mycut->Print();

  /*
   *  for refining the region,
   *
   *  you can use Print(), and use SetPoint() method,
   *
   *  e.g. mycut->SetPoint(14,100,1000);
   *
   */

 

}
