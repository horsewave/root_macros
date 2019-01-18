{
  char name[10], title[20];

  TObjArray Hlist(0); // create an array of Histograms

  TH1F* h; // create a pointer to a histogram

  // make and fill 15 histograms and add them to the object array
  for (Int_t i = 0; i < 15; i++)
  {
    sprintf(name,"h%d",i);

    sprintf(title,"histo nr:%d",i);

    h = new TH1F(name,title,100,-4,4);

    Hlist.Add(h);

    h->FillRandom("gaus",1000);
  }
  // open a file and write the array to the file
  TFile f("demo.root","recreate");
  Hlist.Write();
  f.Close();
}
