
//examples of ROOT TFormulas class
void formula_plots()

{

TGraph *g1=new TGraph();

for(int i=0;i<1000;i++)
{
  //g1->SetPoint(i,i,(i*i-4*i+7));
  g1->SetPoint(i,gRandom->Gaus(5,1),gRandom->Gaus(0,2));
}

//g1->SetPoint(0,1,1);
//g1->SetPoint(1,3,3);

g1->SetMarkerStyle(21);
g1->SetLineColor(kRed);
g1->SetTitle("Data plot with TGraph;GW;ms");
//g1->Draw();
//g1->Draw("AP");
//g1->Draw("AL");
//g1->Draw("APL");
g1->Draw("ACP");


//formula_plot_1();
  

}


void formula_plot_1()
{
  TCanvas* c1=new TCanvas("c1","my canvas",1500,500);

  c1->Divide(3,1);

  c1->cd(1); 

  TF1* f=new TF1("f","cos(x)",-2,2);
  f->SetLineColor(kBlue+3);
  f->SetTitle("My Function cos;X Axis Title;Y (miles)");

  f->Draw();

  c1->cd(2);
    TF1* f1=new TF1("f1","sin(x)",-2,2);

  f1->SetLineColor(kRed+3);
  f1->SetTitle("My Function sin;X Axis Title;Y (miles)");

  //draw on the pre canvas
  //f1->Draw("SAME");
  f1->Draw();

  c1->cd(3);
    TF2* f2=new TF2("f1","10*cos(x)*sin(y)",-3,3,0,10);

    f2->SetTitle("My Function sin;X Axis Title;Y (miles)");

  //draw on the pre canvas
  //f1->Draw("SAME");
  f2->Draw();

}

void Graph_from_file()
{
  TGraph *g1=new TGraph("./test.txt");

g1->SetMarkerStyle(21);
g1->SetLineColor(kRed);
g1->SetTitle("Data plot with TGraph;GW;ms");
//g1->Draw();
//g1->Draw("AP");
//g1->Draw("AL");
g1->Draw("APL");


//formula_plot_1();

}
