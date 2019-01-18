void fit_data()
{

//Define data
TGraph *g1=new TGraph();

for(int i=0;i<10;i++)
{
  //g1->SetPoint(i,i,(i*i-4*i+7));
  float x=0.5*i;
  float y=4*x+2+gRandom->Gaus(0,1);
  //g1->SetPoint(i,gRandom->Gaus(5,1),gRandom->Gaus(0,2));
  g1->SetPoint(i,x,y);
}

//define fit function
//TF1 *f=new TF1("f","pol2",10);
TF1 *f=new TF1("f","[0]*x+[1]",0,10);
f->SetParNames("slope","offset");
f->SetParameters(2,1);
g1->Fit(f);


g1->SetMarkerStyle(21);
g1->SetLineColor(kRed);
g1->SetTitle("Data plot with TGraph;GW;ms");
g1->Draw("ACP");


}
