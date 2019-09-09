{

  //1:open root file, it is , Big Endian byte ordering
  TFile f("demo.root");

  if (f.IsZombie()) 
  {
    cout << "Error opening file" << endl;
    exit(-1);
  }

  //2:show it in windos
  //TBrowser browser;
  //
  //3: show all the file information.
  f.Map();

  //show all the TKeys;
  f.GetListOfKeys()->Print();

  //Get one key
  TH1F *h9 = (TH1F*)f.Get("h9");

  f->ls();
  
//list objects in memory
  gDirectory->ls("-m");
  //list objects on disk
  gDirectory->ls("-d")

    //save file to disk.
    f->Write()
    //hpx->Write("newName")
    //hpx->Write("",TObject::kOverwrite)


  f.Close();
}

