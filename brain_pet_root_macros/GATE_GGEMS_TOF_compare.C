/*
 * Compare the time of flight between GATE and GGEMS using a cube phantom with a point soure in the middle of it.1.25mm.
 *
 *
 */


void GATE_GGEMS_TOF_compare(){

  gROOT->Reset();

      TString full_path=basename_root +str_frame_num+ str_slice + str_slice_num + ext;
      Root_data_analize(full_path,log_path);

}	




 void Root_data_analize(TString full_path,string log_path)
 //void Root_data_analize(TString full_path,string log_path,VirtualGeometry* vgeom)
{


  //cout<<"step0"<<endl;
  std::ofstream of;
  //cout<<"step1"<<endl;
  of.open(log_path.c_str());
  //of.open("test.txt");

  //cout<<"step2"<<endl;
  //--- adding coincidence chains
  /* TChain chain("Coincidences");*/
    //chain.Add(full_path);
    //chain.Add(basename +str_frame_num+ str_slice + str_slice_num + ext);


  //--- This line for single ROOT file
   //TFile f("BrainPET_Plane_source.root");
   TFile *root_file=new TFile(full_path);
  
//   //---define data tree for evaluation
   TTree *Coincidences = (TTree*)gDirectory->Get("Coincidences");


  //---Declaration of leaves types - TTree Coincidences  
  Int_t           RayleighCrystal1;
  Int_t           RayleighCrystal2;
  Int_t           RayleighPhantom1;
  Int_t           RayleighPhantom2;
  Char_t          RayleighVolName1[40];
  Char_t          RayleighVolName2[40];
  Float_t         axialPos;
  Char_t          comptVolName1[40];
  Char_t          comptVolName2[40];
  Int_t           compton1;
  Int_t           compton2;
  Int_t           crystalID1;
  Int_t           crystalID2;
  Int_t           comptonPhantom1;
  Int_t           comptonPhantom2;
  Float_t         energy1;
  Float_t         energy2;   
  Int_t           eventID1;
  Int_t           eventID2;
  Float_t         globalPosX1;
  Float_t         globalPosX2;
  Float_t         globalPosY1;
  Float_t         globalPosY2;      
  Float_t         globalPosZ1;
  Float_t         globalPosZ2;
  Int_t           layerID1;
  Int_t           layerID2;
  Int_t           moduleID1;
  Int_t           moduleID2;
  Float_t         rotationAngle;
  Int_t           rsectorID1;
  Int_t           rsectorID2;
  Int_t           runID;
  Float_t         sinogramS;
  Float_t         sinogramTheta;
  Int_t           sourceID1;
  Int_t           sourceID2;
  Float_t         sourcePosX1;
  Float_t         sourcePosX2;
  Float_t         sourcePosY1;
  Float_t         sourcePosY2;
  Float_t         sourcePosZ1;
  Float_t         sourcePosZ2;
  Int_t           submoduleID1;
  Int_t           submoduleID2;
  Double_t         time1;
  Double_t         time2;
  
  //---extra variables   
  Float_t         zmin,zmax,z;

  cout<<"step4"<<endl;
  
  //---Set branch addresses - TTree Coincidences
  //---to setup data reading
  Coincidences->SetBranchAddress("RayleighCrystal1",&RayleighCrystal1);
  Coincidences->SetBranchAddress("RayleighCrystal2",&RayleighCrystal2);
  Coincidences->SetBranchAddress("RayleighPhantom1",&RayleighPhantom1);
  Coincidences->SetBranchAddress("RayleighPhantom2",&RayleighPhantom2);
  Coincidences->SetBranchAddress("RayleighVolName1",&RayleighVolName1);
  Coincidences->SetBranchAddress("RayleighVolName2",&RayleighVolName2);
  Coincidences->SetBranchAddress("axialPos",&axialPos);
  Coincidences->SetBranchAddress("comptVolName1",&comptVolName1);
  Coincidences->SetBranchAddress("comptVolName2",&comptVolName2);
  Coincidences->SetBranchAddress("comptonCrystal1",&compton1);
  Coincidences->SetBranchAddress("comptonCrystal2",&compton2);
  Coincidences->SetBranchAddress("crystalID1",&crystalID1);
  Coincidences->SetBranchAddress("crystalID2",&crystalID2);
  Coincidences->SetBranchAddress("comptonPhantom1",&comptonPhantom1);
  Coincidences->SetBranchAddress("comptonPhantom2",&comptonPhantom2);
  Coincidences->SetBranchAddress("energy1",&energy1);
  Coincidences->SetBranchAddress("energy2",&energy2);   
  Coincidences->SetBranchAddress("eventID1",&eventID1);
  Coincidences->SetBranchAddress("eventID2",&eventID2);
  Coincidences->SetBranchAddress("globalPosX1",&globalPosX1);
  Coincidences->SetBranchAddress("globalPosX2",&globalPosX2);
  Coincidences->SetBranchAddress("globalPosY1",&globalPosY1);
  Coincidences->SetBranchAddress("globalPosY2",&globalPosY2);      
  Coincidences->SetBranchAddress("globalPosZ1",&globalPosZ1);
  Coincidences->SetBranchAddress("globalPosZ2",&globalPosZ2);
  Coincidences->SetBranchAddress("layerID1",&layerID1);
  Coincidences->SetBranchAddress("layerID2",&layerID2);
  Coincidences->SetBranchAddress("moduleID1",&moduleID1);
  Coincidences->SetBranchAddress("moduleID2",&moduleID2);
  Coincidences->SetBranchAddress("rotationAngle",&rotationAngle);
  Coincidences->SetBranchAddress("rsectorID1",&rsectorID1);
  Coincidences->SetBranchAddress("rsectorID2",&rsectorID2);
  Coincidences->SetBranchAddress("runID",&runID);
  Coincidences->SetBranchAddress("sinogramS",&sinogramS);
  Coincidences->SetBranchAddress("sinogramTheta",&sinogramTheta);
  Coincidences->SetBranchAddress("sourceID1",&sourceID1);
  Coincidences->SetBranchAddress("sourceID2",&sourceID2);
  Coincidences->SetBranchAddress("sourcePosX1",&sourcePosX1);
  Coincidences->SetBranchAddress("sourcePosX2",&sourcePosX2);
  Coincidences->SetBranchAddress("sourcePosY1",&sourcePosY1);
  Coincidences->SetBranchAddress("sourcePosY2",&sourcePosY2);
  Coincidences->SetBranchAddress("sourcePosZ1",&sourcePosZ1);
  Coincidences->SetBranchAddress("sourcePosZ2",&sourcePosZ2);
  Coincidences->SetBranchAddress("submoduleID1",&submoduleID1);
  Coincidences->SetBranchAddress("submoduleID2",&submoduleID2);
  Coincidences->SetBranchAddress("time1",&time1);
  Coincidences->SetBranchAddress("time2",&time2);
   
  
  

  Int_t nbytes = 0, nbytesdelay = 0, nrandom = 0, nscatter = 0, ntrue = 0;  


  Int_t nentries = Coincidences->GetEntries();

  //---Loop for each event in the TTree Coincidences

  int nout=0;
  int nfilled=0;
  for (Int_t i=0; i<nentries;i++) 
    {
      nbytes += Coincidences->GetEntry(i);	 
      if (i <= 10) 
	{
	  cout << " entry: " << i << endl;
	  cout << " submoduleID1: " << submoduleID1 << " rsectorID1: " << rsectorID1 << " crystalID1: " << crystalID1 << endl;
	  cout << " submoduleID2: " << submoduleID2 << " rsectorID2: " << rsectorID2 << " crystalID2: " << crystalID2 << endl;
	}
      if (eventID1 != eventID2) 
	{ 
	  //---Random coincidence
	  ++nrandom;
	}

      else 
  {  
    //---True coincidence
   /* int Icrystal1, Iring1;*/
    //decode_address(rsectorID1, submoduleID1, crystalID1, Icrystal1, Iring1);

    //int Icrystal2, Iring2;
    //decode_address(rsectorID2, submoduleID2, crystalID2, Icrystal2, Iring2);

   /* if (i<10)*/
      //{
        //cout << "Ir1=" << Iring1 << "   IC1=" << Icrystal1 << endl;
        //cout << "Ir2=" << Iring2 << "   IC2=" << Icrystal2 << endl;
      /*}*/
    bool out_of_bounds;
    double weight=1.0;
    //vgeom->add_LOR_weight(Iring1, Icrystal1, Iring2, Icrystal2, weight, out_of_bounds);
    if (out_of_bounds)
      {
        //cout << "event out  of bounds i=" << i << endl;
        nout++;
      }
    else
      {
        nfilled++;
      }
    

    if (comptonPhantom1 == 0 && comptonPhantom2 == 0 &&
        RayleighPhantom1 == 0 && RayleighPhantom2 == 0) 
      {  
        //---true unscattered coincidence
        ntrue++;
      } 
    else 
      { //---true scattered coincidence
        nscatter++;
      }
  } 

    }

  cout << endl << endl;
  cout << " *********************************************************************************** " << endl;
  cout << " *                                                                                 * " << endl;
  cout << " *   B r a i n P E T  S i m u l a t i o n   A n a l y s i s                        * " << endl;
  cout << " *                                                                                 * " << endl;
  cout << " *********************************************************************************** " << endl;
  cout << endl << endl;
  cout << "out of bounds events=" << nout << endl;
  cout << "filled        events=" << nfilled << endl;
  cout << " There are " << nbytes << " bytes read" << endl;     
  cout << " There are " << nentries << " entries" << endl;
  cout << " There are " << ntrue << " true unscattered coincidences" << endl;
  cout << " There are " << nrandom << " random coincidences" << endl;
  cout << " There are " << nscatter << " scattered coincidences" << endl;
  cout << "  ==> there are " << nentries << " coincidences (true, scattered, and random)" << endl;   
  cout << "  ==> global scatter fraction = " << (float)nscatter/(float)(nentries-nrandom) << endl;
  cout << " *********************************************************************************** " << endl;
  cout << endl << endl;

  of<< " *********************************************************************************** " << endl;
  of<< " *                                                                                 * " << endl;
  of<< " *   B r a i n P E T  S i m u l a t i o n   A n a l y s i s                        * " << endl;
  of<< " *                                                                                 * " << endl;
  of<< " *********************************************************************************** " << endl;
  of<< endl << endl;
  of<< "out of bounds events=" << nout << endl;
  of<< "filled        events=" << nfilled << endl;
  of<< " There are " << nbytes << " bytes read" << endl;     
  of << " There are " << nentries << " entries" << endl;
  of<< " There are " << ntrue << " true unscattered coincidences" << endl;
  of<< " There are " << nrandom << " random coincidences" << endl;
  of<< " There are " << nscatter << " scattered coincidences" << endl;
  of<< "  ==> there are " << nentries << " coincidences (true, scattered, and random)" << endl;   
  of<< "  ==> global scatter fraction = " << (float)nscatter/(float)(nentries-nrandom) << endl;
  of<< " *********************************************************************************** " << endl;
  of<<endl<<endl;
  of.close();
 if(root_file!=NULL)
  {
    delete root_file;
    root_file=NULL;
  }


}



void Create_LOR_data(string saved_lor_path,VirtualGeometry* vgeom)
{
   //---create LOR data

  BrainPET_RingData* rdata;
  rdata=new BrainPET_RingData;

  int nblocks, nblock_size;

  rdata->get_sequential_data_sizes(nblocks, nblock_size);
  bool floats=true;
  BrainPET_LORfileData* lordata=new BrainPET_LORfileData(nblocks, nblock_size,floats);
  
  rdata->fill_BrainPET_LORfileData(vgeom,lordata);
  
  //--- store LOR file

  bool overwrite=true;
  lordata->store_as_LORfileData_float(saved_lor_path.c_str(), overwrite);

  if(rdata!=NULL)
  {
    delete rdata;
    rdata=NULL;
  }
  if(lordata!=NULL)
  {
    delete lordata;
    lordata=NULL;
  }


}

//-------------------------------------------------------------------------------------------
//---calculate ring and crystal ID from GATE coordinates
//-------------------------------------------------------------------------------------------
void decode_address(int rsector, int submodul, int crystal, int &Icrystal, int &Iring)
{
  int iaxial,itransaxial;
  get_crystal_address(crystal,iaxial,itransaxial);
  
  Iring=submodul*12+iaxial;
  Icrystal=rsector*12+itransaxial;
}


//-------------------------------------------------------------------------------------------
//---convert index=[0..143] to axial and transaxial contributions 
//-------------------------------------------------------------------------------------------
void get_crystal_address(int idx, int &iaxial, int &itransaxial)
{
  iaxial=idx / 12;
  itransaxial=idx % 12;
}
