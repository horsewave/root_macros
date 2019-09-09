
/*
 *  In this tutorial,
 *  we will discuss some basic properties of 2D histogram obj.
 *
 */
void lesson_14_basic_2d_histogram()
{

  //------------------1: concept of global bin----------------------
  int binN = 100;

  int xmin = -2 ;
  int xmax = 2 ;
  
  int ymin = -2;
  int ymax = 2;

  TCanvas* c1 = new TCanvas("c1","2d histo demo", 500,0,500,800);
  c1->Divide(1,2);
  c1->cd(1);

  TH2C* h1 = new TH2C("h1","a 2d histogram",100,-2,2,100,-2,2);
  /**
   *  in 2d and 3D histograms, the binning can be redefined
   *  by a simpler 1D array.
   *  i.e: bin#1=(1,1), #2=(1,2),#3=(2,1),#4=(2,2,).
   *
   *  h1->GetBin(binx,biny,binz) will return the global bin
   *  and that is quite usefull for "for" loop.
   *  such as: h1->GetBinContent(global_bin) 
   *
   */

  //------------------2: fill a 2d histogram-----------------------
  for(int i=0;i<10000;i++)
  {
    h1->Fill(gRandom->Gaus(-0.5,0.1),gRandom->Gaus(-0.5,0.1));
    h1->Fill(gRandom->Gaus(0.2,0.2),gRandom->Gaus(1.4,0.2));
  }

  /*
   *  h1->Fill(x,y): add 1 to (x,y)
   *  h1->Fill(x,y,w): add w to (x,y)
   *
   *
   */

  h1->Draw("colz");

  //---------------3: projection------------------
  /*
   *  use the ProjectionX() and ProjectionY() method,
   *  to project a 2D histogram to a 1D histogram;
   *
   */
  c1->cd(2);
  h1->ProjectionX()->Draw();
  h1->ProjectionY()->Draw();

  /*
   *  if you want to have a "gate", 
   *  such as see the y projection,
   *  but limit the data from the x range 0 to 1, steps:
   *  1: click the canvas in h1;
   *  2: select "SetShowProjectiony", input "20"(bins);
   *  3: move the cursor to the range 0 to 1.
   *
   */

  /*
   *  to count the number in a certain area, steps:
   *  1: make a TCutG obj, and set name(e.g mycut);
   *  2: mycut->IntegralHist(h1)
   *
   */



}
