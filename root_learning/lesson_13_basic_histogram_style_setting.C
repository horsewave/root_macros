/*
 *  In this tutorial,
 *  we will discuss some basic properties of 1D histogram obj.
 *  Fill(), Draw(),Clone(), Reset(), and SetXXXX
 *
 */
void lesson_13_basic_histogram_style_setting()
{
  //---------------------- 1: 1D histogram basic usage, class----------------------
 int binN = 100;
 int xmin = 0;
 int xmax =100;

 TCanvas* c1=new TCanvas("c1","histo demo",500,600,500,800);//500,600: coordinate;500,800:pixel size
 c1->Divide(1,2);
 c1->cd(1);

 TH1C* h1 = new TH1C("h1","char:127 to -128", binN,xmin,xmax);
 h1->Draw();//all empty now.

 /*
  *  Fill()
  *  histogram is designed to show the counts(frequency),
  *  e.g. 6th bin starts with 5 end with 6;
  *  Fill(5.2) will add the 6th bin a cound, now 1 count;
  *  Fill(5.5) will add the 6th bin a cound, now 2 count;
  */

 for(int i=0;i<20;i++)
 {
   h1->Fill(0.1);
   h1->Fill(5.1);
   h1->Fill(-0.1);//under the range, no effect
   h1->Fill(10.1);
   h1->Fill(120);//over the range,no effect.
 }
 h1->Draw();
 //gPad->Update();

 /*
  *  note: if you really want "120" filled in the histogram,
  *  which ranges from 0 to 100, you can set:
  *
  *  h1->SetBit(TH1::kCanRebin); !! set before filling.
  *
  *  the range will automatically extend, but the bin size will
  *  be changed.
  *
  *
  */

 /*
  *  over the char upper limit 127,
  *  it will only display 127 in counts
  *
  */
 for(int i=0;i<500;i++)
 {
   h1->Fill(0.2);
   h1->Fill(10.2);
 }

 h1->Draw();

 //------------------------2: clone a histogram--------------------

 c1->cd(2);

 TH1C* h1_backup = (TH1C*) h1->Clone();
 h1_backup->SetTitle("cloned histo");
 h1_backup->Draw();
 /*
  *  use Clone() method,
  *  then convert its return stuff to a TH1C pointer.
  *
  *  now any modification in h1 won't affect h1_backup.
  *
  */

 //------------------3: LineColor, LineWidth, LineStyle, FillColor------------
 h1->SetLineColor(kGreen);
 h1->SetLineWidth(2);
 h1->SetFillColor(kRed);
 //https://root.cern.ch/root/html/TAttLine.html#L3
 
 //-----------------4: OptStat--------------
 gStyle->SetOptStat(kFALSE);// disable stat box



 //-----------------5: axis Label----------------------
 h1->GetXaxis()->SetTitle("this is x axis");
 h1->GetYaxis()->SetTitle("this is Y axis");


 //-----------------6: Axis range(only for zooming in)
 
 h1->GetXaxis()->SetRangeUser(0,200); // not working
 h1->GetXaxis()->SetRangeUser(0,20); // bin 0 to 20 


 h1->GetXaxis()->SetRange(1,10); // 


 //------------------7: log scale----------------
 
 c1->cd(1);
 gPad->SetLogy(kTRUE);// this is good for large numbers
 //gPad->SetLogy(kFALSE);// this is good for large numbers
 
 


 //------------------8: max and min value-------------------
 h1->SetMaximum(100);
 h1->SetMinimum(0);
 h1->SetMaximum();// use default setting
 h1->SetMinimum();// use default setting


 //------------------9: save to ps, jpg,pdf,eps or others
 c1->Print("test.jpg");
 //if print only one his, 
 c1->cd(1);
 gPad->Print("test.jpg");

 // Note: in GUI-->minu bar-->view-->>editor, 
 // you can find more drawing options.

 //gApplication->Terminate();



}
