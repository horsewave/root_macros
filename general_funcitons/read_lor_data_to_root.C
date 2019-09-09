/***
 *
 * Get the axial distribution of the events in the lor data and save it to root file.
 *
 * ***get_axial_ring_counts.C*
 * Function: get the  events sum of all the crystals in one transaxile ring;
 * Editor: Bo Ma;
 *
 * Date:2017.01.10.
 * Version:1.0
 *
 * Discription: this version uses more memory: cause I have to new two 3 arrays with the size of the LOR file.

 *Class needed in PRESTO:
 *
 *1:Utilities
1:class BrainPET_DataAccess:
1):
 *
 *2:class BrainPET_RingData: public BrainPETbasics
 *
 *3:class BrainPET_LORfileData3:
 *
 *4:class VirtualGeometry: public VirtualGeometry_ABC;
 *
 * 5: class SinogramGrid:public SinogramGrid_ABC
 *
 * 6: class VirtualSinogram:
 *
 *2:class BrainPETbasics_ABC:
 *
 *
 *
 *
 *Data Procedure:
 *  1:Load LOR data: class BrainPET_LORfileData->BrainPET_LORfileData(int n_modul_pairs0, int n_modulpair_size0, bool data_float0=true);or

 *2:Get the data pointer for each module pair: float* BrainPET_LORfileData::get_modul_pair_data_float(int imodul_pair);
 Here I can get all the lor data belonging to the respective module pair: mpair_data[imodul_pair][0-mpair_size];

 *3:according to the module pair look up table , get the responding two modules forming the module pair: void MRPET_dump::mpair_lookup(int &amod, int &bmod, int index);   


 *4:according to the module pair, get the crystal index for the lors in the mpair, it is a look up table for each lor in the module pair :void BrainPETbasics_ABC::get_crystal_combis_for_modulpair(int impair, int* ic1, int *ic2)

 *5:according to the index of the total crystal, the get the responding number and ring number:  crystal_pos_typII BrainPETbasics_ABC::get_crystal_address_typII(int index);

 *6: calculate!!!   *


 *get_axial_ring_counts_single_file.C
 * Function: get the  events sum of all the crystals in one transaxile ring;
 * Editor: Bo Ma;
 *
 * Date:2017.01.15.
 * Version:2.0
 *
 * Discription: this version uses less memory: cause I have to new two 3 arrays with the size of the LOR file. one time only on lor file can be processed



 * get_axial_ring_counts_multi_file.C
 * Function: get the  events sum of all the crystals in one transaxile ring;
 * Editor: Bo Ma;
 *
 * Date:2017.01.16.
 * Version:3.0
 *
 * Discription: this version uses less memory: cause I have to new two 3 arrays with the size of the LOR file.



 *
 * ***/


//void read_lor_data_to_root(Int_t frame_index, Int_t slice_index)
void read_lor_data_to_root()
{
  string path_lor_data="/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/XB1BN305N-BI/XB1BN305N-BI-01/scatterMCS/real_phantom_simu_run_num_5000/XB1BN305N-BI-01_gpuSimu_1_true.flor";
  
  string path_saved_root_file="./my_root_file.root";

  bool is_float=true;

  get_lor_axile_event_distribution(path_lor_data,path_saved_root_file,is_float);


}



void get_axial_ring_counts_multi_file_prompt(Int_t frame_index, Int_t slice_index)
{
  TString lor_data_path_base="/data/mr_pet_temp/Ma/software/src/gate/gate_results/random_ofov_ma/lor_data/prompt/";
  TString saved_data_path_base="/data/mr_pet_temp/Ma/software/src/gate/gate_results/random_ofov_ma/lor_data/lor_axile_distribution/";

  TString basename_lor = "random_ofov_gate_run_frame";  
  TString basename_result = "event_count_per_ring_frame";  
  TString str_slice = "_slice";
  TString ext_lor= "_prompt.flor";
  TString ext_result= "_prompt.txt";





  std::ostringstream os_frame_num;
  os_frame_num<< frame_index; 
  TString str_frame_num(os_frame_num.str());




  std::ostringstream os_slice_num;
  os_slice_num<< slice_index; 
  TString str_slice_num(os_slice_num.str());

  TString lor_path_T=lor_data_path_base+basename_lor+str_frame_num+ str_slice + str_slice_num + ext_lor;

  TString results_path_T=saved_data_path_base+basename_result+str_frame_num+ str_slice + str_slice_num + ext_result;

  string lor_path=lor_path_T;
  string result_path=results_path_T;

  cout<<"lor path is :"<<lor_path<<endl;
  cout<<"results path is :"<<result_path<<endl;




  bool is_float=true;

  get_lor_axile_event_distribution(lor_path,result_path,is_float);

  string rm_command="rm "+lor_path;
  cout<<"rm command is : "<<rm_command<<endl;
  gSystem->Exec(rm_command.c_str());
  cout<<"start to exit"<<endl;
  exit(1);

}//end of void get_axial_ring_counts_multi_file()





void get_lor_axile_event_distribution(string lor_path,string result_path,bool is_float)
{  

  int ring_num=get_ring_num();
  cout <<"ring number is :"<<ring_num<<endl;


  // sum of event counts in each ring
  if(is_float)
  {
    float *sum_ring_events;


    sum_ring_events=new float[ring_num];
    memset(sum_ring_events,0,sizeof(float)*ring_num);
    get_ring_sum_event_counts_float(lor_path,sum_ring_events);

    write_array_to_root(result_path, sum_ring_events,ring_num);
    //save_sum_ring_event_float(result_path,sum_ring_events,ring_num);

    cout<<"ring number is : "<<ring_num<<endl;

  }
  else
  {

    int *sum_ring_events;

    sum_ring_events=new int[ring_num];
    memset(sum_ring_events,0,sizeof(int)*ring_num);
    get_ring_sum_event_counts_short(lor_path,sum_ring_events);
write_array_to_root(result_path, sum_ring_events,ring_num); 
  //save_sum_ring_event_short(result_path,sum_ring_events,ring_num );

    cout<<"ring number is : "<<ring_num<<endl;
  }



  if(sum_ring_events!=NULL)
  {
    delete [] sum_ring_events;
    sum_ring_events=NULL;
  }

}

void write_array_to_root(string path_saved_root_file, float *array_data,int array_size)
//void write_array_to_root(string path_saved_root_file, int *array_data,int array_size)
{


  //string path_root_file="./my_root_file.root";
  string path_root_file=path_saved_root_file;

  string tree_tag="LOR_data";
  string tree_name="Axile distribution of lor events";

  //create_root_file(path_root_file,tree_tag, tree_name );

  //TTree* t=new TTree("coincidence","coincedence from GPU MCS");
  TTree* t=new TTree(tree_tag.c_str(),tree_name.c_str());

  std::ostringstream os_int_to_str;
  os_int_to_str<<array_size; 
  string str_array_size(os_int_to_str.str());

  string leaf_list="array_data["+str_array_size+"]/I";

  t->Branch("z_lor_events",array_data,leaf_list.c_str());


  t->Fill();

  ////---5: write the tree to the root file
  TFile* f=new TFile(path_root_file.c_str(),"update");
  t->Write("",TObject::kOverwrite);//// save only the new version of the tree

  //t->Write();//// save only the new version of the tree
  f->Close();





  delete t;
  t=NULL;

  delete f;
  f=NULL;





}



void create_root_file(string path_root_file,string tree_tag, string tree_name )
{

  //TTree* t=new TTree("coincidence","coincedence from GPU MCS");
  TTree* t=new TTree(tree_tag.c_str(),tree_name.c_str());

  ////---5: write the tree to the root file
  TFile* f=new TFile(path_root_file.c_str(),"recreate");
  t->Write();//// save only the new version of the tree
  //f->Close();

  delete t;
  t=NULL;

  delete f;
  f=NULL;




}







int get_ring_num()
{
  MRPET_dump *dump = new MRPET_dump();
  int ring_num=dump->get_mod_ny();

  if(dump!=NULL)
  {
    delete dump;
    dump=NULL;
  }

  return ring_num;

}

void get_ring_sum_event_counts_float(string lor_path,float *sum_ring_events)
{


  //class including all the basic information of the MRPET.
  MRPET_dump *dump = new MRPET_dump();

  int n_mpairs=dump->get_n_mpairs();

  int max_ring_diff=dump->get_max_ringdiff();

  int mod_nx=dump->get_mod_nx();
  int mod_ny=dump->get_mod_ny();

  int mp_size=(mod_nx*mod_ny)*(mod_nx*mod_ny);

  int ring_num=mod_ny;

  cout<<"n_mpairs is : "<<n_mpairs <<endl;
  cout<<"max_ring_diff is : "<<max_ring_diff <<endl;
  cout<<"ring_num is : "<<ring_num <<endl;
  cout<<"mod_nx is : "<<mod_nx <<endl;
  cout<<"mod_ny is : "<< mod_ny<<endl;
  cout<<"mp_size is : "<<mp_size<<endl;

  const int const_n_mpairs=n_mpairs;
  int mpair_a[const_n_mpairs];
  int mpair_b[const_n_mpairs];

  for(int i=0;i<n_mpairs;i++)
  {
    dump->mpair_lookup(mpair_a[i],mpair_b[i],i);

    //cout<<"n_mpairs index is: "<<i<<"; modual a is: "<<mpair_a[i]<<"; modual b is: "<<mpair_b[i]<<endl;

  }

  bool is_float=true;
  cout<<"start to new class BrainPET_LORfileData"<<endl;
  BrainPET_LORfileData *c_LOR_file_data=new BrainPET_LORfileData(n_mpairs,mp_size,is_float);

  cout<<"start to load data"<<endl;

  c_LOR_file_data->load_data(lor_path.c_str(),is_float);


  cout<<"finish to load data"<<endl;

  cout<<"get the module pair data:"<<endl;

  //---modul pair data pointers
  float** mpair_data;
  mpair_data=new float*[n_mpairs];

  for(int i=0;i<n_mpairs;i++)
  {
    mpair_data[i]=c_LOR_file_data->get_modul_pair_data_float(i);

  }
  cout<<"finish to get the module pair data:"<<endl;


  BrainPETbasics_ABC *c_PET_basics_ABC=new BrainPETbasics_ABC;

  int crystal_tot_num=c_PET_basics_ABC->number_of_crystals();
  cout<<"crystal number is: "<<crystal_tot_num<<endl;

  /*for(int i=0;i<20;i++)*/
  //{
  //cout<<setw(4)<<i<<setw(10)<<mpair_lor_cry_lookup_1[0][i]<<setw(10)<<mpair_lor_cry_lookup_2[0][i]<<endl;
  /*}*/
  //---struct:
  //---define unique crystal position in terms of unique transaxial crystal number [0..12x32=384[
  //---and ring number [0..72[ without gaps (!)
  struct crystal_pos_typII
  {
    short icrystal_on_ring;
    short iring;
  };


  short *crystal_ring_lookup=new short[crystal_tot_num];

  memset(crystal_ring_lookup,0,sizeof(short)*crystal_tot_num);

  for(int i=0;i<crystal_tot_num;i++)
  {
    crystal_pos_typII struct_crystal_ring=c_PET_basics_ABC->get_crystal_address_typII(i);


    crystal_ring_lookup[i]=struct_crystal_ring.iring;

    //cout<<"crystal index is: "<<i<<" ring index is: "<< crystal_ring_lookup[i]<<endl;

  }

  //look up table for the crystal index(0-12*12*6*32-1) and the mpair and the lor index(0-(12*12*6*32)*(12*12*6*32)-1) in each mpair;
  //there are two crystal for each lor, so there are two lookup table;

  int *mpair_lor_cry_lookup_1;
  int *mpair_lor_cry_lookup_2;

  for(int i=0;i<n_mpairs;i++)
  {
    mpair_lor_cry_lookup_1=new int[mp_size];
    memset(mpair_lor_cry_lookup_1,0,sizeof(int)*mp_size);
    mpair_lor_cry_lookup_2=new int[mp_size];
    memset(mpair_lor_cry_lookup_2,0,sizeof(int)*mp_size);
    //---get decoded crystal combinations for specific sequential block of modul pair
    //---arrays ic1 and ic2 must exist and have length mpsize!
    c_PET_basics_ABC->get_crystal_combis_for_modulpair(i, mpair_lor_cry_lookup_1, mpair_lor_cry_lookup_2);

    for(int j=0;j<mp_size;j++)
    {
      //get the crystal index according to the lor-crystal lookup table;
      int crystal_index_1=mpair_lor_cry_lookup_1[j];
      int crystal_index_2=mpair_lor_cry_lookup_2[j];

      //get the ring index according to the ring-crystal look up table
      short ring_index_crystal1=crystal_ring_lookup[crystal_index_1];
      short ring_index_crystal2=crystal_ring_lookup[crystal_index_2];

      //get the lor data 
      float lor_data=mpair_data[i][j];

      //add the events to the corresponding ring.
      sum_ring_events[ring_index_crystal1]+=lor_data;
      sum_ring_events[ring_index_crystal2]+=lor_data;

    }
    if(mpair_lor_cry_lookup_1!=NULL)
    {
      delete [] mpair_lor_cry_lookup_1;
      mpair_lor_cry_lookup_1=NULL;

    }
    if(mpair_lor_cry_lookup_2!=NULL)
    {

      delete [] mpair_lor_cry_lookup_2;
      mpair_lor_cry_lookup_2=NULL;
    }


  }


  if(mpair_data!=NULL)
  {
    delete [] mpair_data;
    mpair_data=NULL;
  }

  if(mpair_lor_cry_lookup_1!=NULL)
  {
    delete [] mpair_lor_cry_lookup_1;
    mpair_lor_cry_lookup_1=NULL;

  }
  if(mpair_lor_cry_lookup_2!=NULL)
  {

    delete [] mpair_lor_cry_lookup_2;
    mpair_lor_cry_lookup_2=NULL;
  }

  if(crystal_ring_lookup!=NULL)
  {
    delete [] crystal_ring_lookup;
    crystal_ring_lookup=NULL;
  }


  if(dump!=NULL)
  {
    delete dump;
    dump=NULL;
  }

  if(c_LOR_file_data!=NULL)
  {
    delete c_LOR_file_data;
    c_LOR_file_data=NULL;
  }

  if(c_PET_basics_ABC!=NULL)
  {
    delete c_PET_basics_ABC;
    c_PET_basics_ABC=NULL;
  }




}




void get_ring_sum_event_counts_short(string lor_path,int *sum_ring_events)
{



  //class including all the basic information of the MRPET.
  MRPET_dump *dump = new MRPET_dump();

  int n_mpairs=dump->get_n_mpairs();

  int max_ring_diff=dump->get_max_ringdiff();

  int mod_nx=dump->get_mod_nx();
  int mod_ny=dump->get_mod_ny();

  int mp_size=(mod_nx*mod_ny)*(mod_nx*mod_ny);

  int ring_num=mod_ny;

  cout<<"n_mpairs is : "<<n_mpairs <<endl;
  cout<<"max_ring_diff is : "<<max_ring_diff <<endl;
  cout<<"ring_num is : "<<ring_num <<endl;
  cout<<"mod_nx is : "<<mod_nx <<endl;
  cout<<"mod_ny is : "<< mod_ny<<endl;
  cout<<"mp_size is : "<<mp_size<<endl;

  const int const_n_mpairs=n_mpairs;
  int mpair_a[const_n_mpairs];
  int mpair_b[const_n_mpairs];

  for(int i=0;i<n_mpairs;i++)
  {
    dump->mpair_lookup(mpair_a[i],mpair_b[i],i);

    //cout<<"n_mpairs index is: "<<i<<"; modual a is: "<<mpair_a[i]<<"; modual b is: "<<mpair_b[i]<<endl;

  }

  bool is_float=false;
  cout<<"start to new class BrainPET_LORfileData"<<endl;
  BrainPET_LORfileData *c_LOR_file_data=new BrainPET_LORfileData(n_mpairs,mp_size,is_float);

  cout<<"start to load data"<<endl;

  c_LOR_file_data->load_data(lor_path.c_str(),is_float);


  cout<<"finish to load data"<<endl;

  cout<<"get the module pair data:"<<endl;

  //---modul pair data pointers
  short** mpair_data;
  mpair_data=new short*[n_mpairs];

  for(int i=0;i<n_mpairs;i++)
  {
    mpair_data[i]=c_LOR_file_data->get_modul_pair_data_short(i);

  }
  cout<<"finish to get the module pair data:"<<endl;


  BrainPETbasics_ABC *c_PET_basics_ABC=new BrainPETbasics_ABC;

  int crystal_tot_num=c_PET_basics_ABC->number_of_crystals();
  cout<<"crystal number is: "<<crystal_tot_num<<endl;

  /*for(int i=0;i<20;i++)*/
  //{
  //cout<<setw(4)<<i<<setw(10)<<mpair_lor_cry_lookup_1[0][i]<<setw(10)<<mpair_lor_cry_lookup_2[0][i]<<endl;
  /*}*/
  //---struct:
  //---define unique crystal position in terms of unique transaxial crystal number [0..12x32=384[
  //---and ring number [0..72[ without gaps (!)
  struct crystal_pos_typII
  {
    short icrystal_on_ring;
    short iring;
  };


  short *crystal_ring_lookup=new short[crystal_tot_num];

  memset(crystal_ring_lookup,0,sizeof(short)*crystal_tot_num);

  for(int i=0;i<crystal_tot_num;i++)
  {
    crystal_pos_typII struct_crystal_ring=c_PET_basics_ABC->get_crystal_address_typII(i);


    crystal_ring_lookup[i]=struct_crystal_ring.iring;

    //cout<<"crystal index is: "<<i<<" ring index is: "<< crystal_ring_lookup[i]<<endl;

  }

  //look up table for the crystal index(0-12*12*6*32-1) and the mpair and the lor index(0-(12*12*6*32)*(12*12*6*32)-1) in each mpair;
  //there are two crystal for each lor, so there are two lookup table;

  int *mpair_lor_cry_lookup_1;
  int *mpair_lor_cry_lookup_2;

  for(int i=0;i<n_mpairs;i++)
  {
    mpair_lor_cry_lookup_1=new int[mp_size];
    memset(mpair_lor_cry_lookup_1,0,sizeof(int)*mp_size);
    mpair_lor_cry_lookup_2=new int[mp_size];
    memset(mpair_lor_cry_lookup_2,0,sizeof(int)*mp_size);
    //---get decoded crystal combinations for specific sequential block of modul pair
    //---arrays ic1 and ic2 must exist and have length mpsize!
    c_PET_basics_ABC->get_crystal_combis_for_modulpair(i, mpair_lor_cry_lookup_1, mpair_lor_cry_lookup_2);

    for(int j=0;j<mp_size;j++)
    {
      //get the crystal index according to the lor-crystal lookup table;
      int crystal_index_1=mpair_lor_cry_lookup_1[j];
      int crystal_index_2=mpair_lor_cry_lookup_2[j];

      //get the ring index according to the ring-crystal look up table
      short ring_index_crystal1=crystal_ring_lookup[crystal_index_1];
      short ring_index_crystal2=crystal_ring_lookup[crystal_index_2];

      //get the lor data 
      short lor_data=mpair_data[i][j];

      //add the events to the corresponding ring.
      sum_ring_events[ring_index_crystal1]+=(int)lor_data;
      sum_ring_events[ring_index_crystal2]+=(int)lor_data;

    }
    if(mpair_lor_cry_lookup_1!=NULL)
    {
      delete [] mpair_lor_cry_lookup_1;
      mpair_lor_cry_lookup_1=NULL;

    }
    if(mpair_lor_cry_lookup_2!=NULL)
    {

      delete [] mpair_lor_cry_lookup_2;
      mpair_lor_cry_lookup_2=NULL;
    }


  }


  if(mpair_data!=NULL)
  {
    delete [] mpair_data;
    mpair_data=NULL;
  }

  if(mpair_lor_cry_lookup_1!=NULL)
  {
    delete [] mpair_lor_cry_lookup_1;
    mpair_lor_cry_lookup_1=NULL;

  }
  if(mpair_lor_cry_lookup_2!=NULL)
  {

    delete [] mpair_lor_cry_lookup_2;
    mpair_lor_cry_lookup_2=NULL;
  }

  if(crystal_ring_lookup!=NULL)
  {
    delete [] crystal_ring_lookup;
    crystal_ring_lookup=NULL;
  }



  if(dump!=NULL)
  {
    delete dump;
    dump=NULL;
  }

  if(c_LOR_file_data!=NULL)
  {
    delete c_LOR_file_data;
    c_LOR_file_data=NULL;
  }

  if(c_PET_basics_ABC!=NULL)
  {
    delete c_PET_basics_ABC;
    c_PET_basics_ABC=NULL;
  }



}



void save_sum_ring_event_float(string saved_file_path, float *sum_ring_events,int ring_num  )
{
  ofstream of;
  of.open(saved_file_path.c_str());

  of<<setw(20)<<"ring index"<<setw(20)<<"Event counts"<<"\r"<<endl;

  int sum_events=0;
  float max_event_counts=0; 
  for(int i=0;i<ring_num;i++)
  {
    if(max_event_counts<sum_ring_events[i]) 
    {
      max_event_counts=sum_ring_events[i];

    }

    of<<setw(20)<<i<<setw(20)<<sum_ring_events[i]<<"\r"<<endl;
    sum_events+=sum_ring_events[i];
  }

  of<<endl<<endl;
  of<<"The total events are : "<<sum_events<<endl;
  of.close();
  cout<<"The total events are : "<<sum_events<<endl;


}

void save_sum_ring_event_short(string saved_file_path, int *sum_ring_events,int ring_num  )
{
  ofstream of;
  of.open(saved_file_path.c_str());
  //of.open(saved_file_path.c_str(),std::ofstream::out);

  of<<setw(20)<<"ring index"<<setw(20)<<"Event counts"<<"\r"<<endl;

  int sum_events=0; 
  int max_event_counts=0; 
  for(int i=0;i<ring_num;i++)
  {
    if(max_event_counts<sum_ring_events[i]) 
    {
      max_event_counts=sum_ring_events[i];

    }

    of<<setw(20)<<i<<setw(20)<<sum_ring_events[i]<<"\r"<<endl;
    sum_events+=sum_ring_events[i];
  }

  of<<endl<<sum_events<<endl;
  of<<"The total events are : "<<sum_events<<endl;
  of.close();
  cout<<"The total events are : "<<sum_events<<endl;



}

//using the exiting data to generate a 2d histogram
float GenerateHistogram(TH1F* pHistgram, int *array_data)
{
  int dimx=pHistgram->GetNbinsX();

  cout<<"test dimX:"<<dimx<<endl;


  for(int x = 0; x < dimx; x++)
  {
    int val = array_data[x];
    pHistgram->SetBinContent(x+1,val);
    //cout<<val<<endl;

  }
  //to get the max value of the Histogram.
  return pHistgram->GetMaximum();	


}




//using the exiting data to generate a 2d histogram
float GenerateHistogram(TH1F* pHistgram, float *array_data)
{
  int dimx=pHistgram->GetNbinsX();

  cout<<"test dimX:"<<dimx<<endl;


  for(int x = 0; x < dimx; x++)
  {
    float val = array_data[x];
    pHistgram->SetBinContent(x+1,val);
    //cout<<val<<endl;

  }
  //to get the max value of the Histogram.
  return pHistgram->GetMaximum();	


}


//draw the hisgram
void DrawHistgram(TCanvas* can_hist,TH1F* pHistgram, int subImage_indext)
{
  can_hist->cd(subImage_indext);

  pHistgram->GetXaxis()->SetTitle("Axile Positon");
  pHistgram->GetYaxis()->SetTitle("Event Counts");

  pHistgram->Draw("l*h");

  can_hist->Update();

  //palette = (TPaletteAxis*)pHistgram->GetListOfFunctions()->FindObject("palette");

  //palette->SetX1NDC(0.9);
  //palette->SetX2NDC(0.93);
  //palette->SetY1NDC(0.1);
  //palette->SetY2NDC(0.9);

  can_hist->Modified();
  can_hist->Update();


}












//
