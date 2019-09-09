
void histogram_study()
{

  //Creating_his();

  getrandomh();
}


void Creating_his()
{
  // using various constructors
  //
 TH2* h = new TH2D(
/* name */ "h2",
/* title */ "Hist with constant bin width",
/* X-dimension */ 100, 0.0, 4.0,
/* Y-dimension */ 200, -3.0, 1.5);


  TH1* h1 = new TH1I("h1", "h1 title", 100, 0.0, 4.0);
  TH2* h2 = new TH2F("h2", "h2 title", 40, 0.0, 2.0, 30, -1.5, 3.5);
  TH3* h3 = new TH3D("h3", "h3 title", 80, 0.0, 1.0, 100, -2.0, 2.0,
      50, 0.0, 3.0);
  // cloning a histogram
  TH1* hc = (TH1*)h1->Clone();
  // projecting histograms
  // the projections always contain double values !
  TH1* hx = h2->ProjectionX(); // ! TH1D, not TH1F
  TH1* hy = h2->ProjectionY(); // ! TH1D, not TH1F



Int_t bin = h3->GetBin(80, 100, 50);
  cout<<"the bin number is: "<<bin<<endl;
}

void Fill_hist()
{

   TH1* h1 = new TH1I("h1", "h1 title", 100, 0.0, 4.0);
  TH2* h2 = new TH2F("h2", "h2 title", 40, 0.0, 2.0, 30, -1.5, 3.5);
  TH3* h3 = new TH3D("h3", "h3 title", 80, 0.0, 1.0, 100, -2.0, 2.0,
      50, 0.0, 3.0);
  // cloning a histogram
  TH1* hc = (TH1*)h1->Clone();

  float x=1.5;
  float y=1.2;
  float z=1.5;
  float w=10.3

  h1->Fill(x);
h1->Fill(x,w); // with weight
h2->Fill(x,y);
h2->Fill(x,y,w);
h3->Fill(x,y,z);
h3->Fill(x,y,z,w);

float bin=1.2;
Double_t binContent = h1->GetBinContent(bin);
}


void getrandomh() 
{
TH1F *source = new TH1F("source","source hist",100,-3,3);

source->FillRandom("gaus",1000);

TH1F *final = new TH1F("final","final hist",100,-3,3);
// continued...
for (Int_t i=0;i<10000;i++) {
final->Fill(source->GetRandom());
//final->Fill(2);
}

//TH1F *final_scal=new TH1F("source","source hist",100,-3,3);

//final->Scale(2);


TCanvas *c1 = new TCanvas("c1","c1",800,1000);
c1->Divide(1,2);
c1->cd(1);
//final_scal->Draw();
source->Draw();

c1->cd(2);
final->Draw();
c1->cd();
}
