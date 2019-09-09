void hsimple()
{
  string file_name="test_ntuple.root";
  TFile *f=new TFile(file_name.c_str(),"update");

  TNtuple *ntuple = new TNtuple("ntuple","Demo ntuple","px:py:pz:value");

  for(int z=0;z<72;z++){
    for(int y=0;y<128;y++){
      for(int x=0;x<128;x++){
        int value=x*y*z;

       ntuple->Fill(x,y,z,value);
      }
    }
  }



ntuple->Write();

}


