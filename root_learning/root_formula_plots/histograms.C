void histograms()
{

  TH1D *h1=new TH1D("h1","first Histogram;x;counts",10,0,10);

  //h1->Fill(2);

for(int i=0;i<1000;i++)
{
h1->Fill(gRandom->Gaus(5,2));
}

h1->SetMinimum(0);
h1->SetFillColor(kBlue-7);

//h1->SetStats(0);
h1->SetStats(1);
gStyle->SetOptStat(1111111111);
 

h1->Draw();

}
