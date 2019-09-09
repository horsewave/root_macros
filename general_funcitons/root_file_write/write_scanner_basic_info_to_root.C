// function: write all the basic information of the scanner to the root file,
// just in case it is needed,the information includes:
//
//
//
// Editor: Bo Ma
// Time: 2018.11.22

void write_scanner_basic_info_to_root()
{

  string path_lor_data="/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/XB1BN305N-BI/XB1BN305N-BI-01/scatterMCS/real_phantom_simu_run_num_5000/XB1BN305N-BI-01_gpuSimu_1_true.flor";

  string path_saved_root_file="./my_root_file.root";

  const int ring_num=153;
  sum_ring_events=new float[ring_num];
  memset(sum_ring_events,0,sizeof(float)*ring_num);




  get_information(path_lor_data,sum_ring_events);




  if(sum_ring_events!=NULL)
  {
    delete [] sum_ring_events;
    sum_ring_events=NULL;
  }


}


void write_array_to_root(string path_root_file,string root_mem_name,int* array_data,int array_size)
{
  //TFile* f=new TFile(path_root_file.c_str(),"recreate");
  TFile* f=new TFile(path_root_file.c_str(),"update");

  TVector* v=new TVector(array_size);

  for(int i=0;i<array_size;i++)
  {
    //set value to the vector
    v(i) =array_data[i];
  }

  //4: write to the file.
  v->Write(root_mem_name.c_str(),TObject::kOverwrite);//// save only the new version of the tree

  f->Close();


  v->Print();

  f->Close();

  delete f;
  f=NULL;

  delete v;
  v=NULL;


}


void write_array_to_root(string path_root_file,string root_mem_name,short* array_data,int array_size)
{
  //TFile* f=new TFile(path_root_file.c_str(),"recreate");
  TFile* f=new TFile(path_root_file.c_str(),"update");

  TVector* v=new TVector(array_size);

  for(int i=0;i<array_size;i++)
  {
    //set value to the vector
    v(i) =array_data[i];
  }

  //4: write to the file.
  v->Write(root_mem_name.c_str(),TObject::kOverwrite);//// save only the new version of the tree

  f->Close();


  v->Print();

  f->Close();

  delete f;
  f=NULL;

  delete v;
  v=NULL;


}




void write_2d_matrix_to_root(string path_root_file,string root_mem_name,TMatrix* mat,int row,int column )
{
  //string path_root_file="./my_root_file.root";

  TFile* f=new TFile(path_root_file.c_str(),"update");


  //mat->Print();

  /* //TObjString* ts=new TObjString("The total axile distibution of the LOR data");*/

  //4: write to the file.
  mat->Write(root_mem_name.c_str(),TObject::kOverwrite);//// save only the new version of the tree
  f->Close();
  delete f;
  f=NULL;
}




void get_information(string lor_path,float *sum_ring_events)
//void get_information(string lor_path)
{


  //class including all the basic information of the MRPET.
  MRPET_dump *dump = new MRPET_dump();

  int n_mpairs=dump->get_n_mpairs();

  int max_ring_diff=dump->get_max_ringdiff();

  int mod_nx=dump->get_mod_nx();
  int mod_ny=dump->get_mod_ny();

  int mp_size=(mod_nx*mod_ny)*(mod_nx*mod_ny);

  int ring_num=mod_ny;

  //19*16=304
  cout<<"n_mpairs is : "<<n_mpairs <<endl;
  //67
  cout<<"max_ring_diff is : "<<max_ring_diff <<endl;
  //72
  cout<<"ring_num is : "<<ring_num <<endl;
  //12
  cout<<"mod_nx is : "<<mod_nx <<endl;
  //12*6=72
  cout<<"mod_ny is : "<< mod_ny<<endl;
  //(12*12*6)*(12*12*6)
  cout<<"mp_size is : "<<mp_size<<endl;

  const int const_n_mpairs=n_mpairs;

  //  For each module pair(i), it is the combination of 
  //  mpair_a[i] and mpair_b[i]; 
  int mpair_a[const_n_mpairs];
  int mpair_b[const_n_mpairs];

  for(int i=0;i<n_mpairs;i++)
  {
    dump->mpair_lookup(mpair_a[i],mpair_b[i],i);

    //cout<<"n_mpairs index is: "<<i<<"; modual a is: "<<mpair_a[i]<<"; modual b is: "<<mpair_b[i]<<endl;

  }


  //-----------------------------------------

  //1: save module pair lookup table,total number:304=19*16.
  string path_saved_root_file="./my_root_file.root";

  string root_mem_name="modual_pair_lookup_table_a";
  write_array_to_root(path_saved_root_file,root_mem_name, mpair_a,n_mpairs);

  root_mem_name="modual_pair_lookup_table_b";
  write_array_to_root(path_saved_root_file,root_mem_name, mpair_b,n_mpairs);

  //-----------------------------------------




  bool is_float=true;
  cout<<"start to new class BrainPET_LORfileData"<<endl;
  BrainPET_LORfileData *c_LOR_file_data=new BrainPET_LORfileData(n_mpairs,mp_size,is_float);

  cout<<"start to load data"<<endl;

  c_LOR_file_data->load_data(lor_path.c_str(),is_float);


  cout<<"finish to load data"<<endl;

  cout<<"get the module pair data:"<<endl;

  //---modul pair data pointers,the count for each lor 

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

  //-----------------------------------------

  //2: save  the crystal_ring_lookup table,e.g: the ring number for each crystal
  // crystal number: 12*12*6*32=27684
  path_saved_root_file="./my_root_file.root";

  root_mem_name="crystal_ring_lookup_table";

  write_array_to_root(path_saved_root_file,root_mem_name, crystal_ring_lookup,crystal_tot_num);


  //-----------------------------------------



  //for each mpair, the lookup table of :crystal index and lor index. 
  //There are n_mpairs lookup tables.
  int *mpair_lor_cry_lookup_1;
  int *mpair_lor_cry_lookup_2;

  //for each modual pair, there is a lor t0 crystal index loopuptable
  TMatrix* mat_1=new TMatrix(n_mpairs,mp_size);
  TMatrix* mat_2=new TMatrix(n_mpairs,mp_size);

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

      //copy the loopup table to TMatrix and save them to root file
      mat_1(i,j)=mpair_lor_cry_lookup_1[j];
      mat_2(i,j)=mpair_lor_cry_lookup_2[j];

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

  //-----------------------------------------

  // 3: save LOR to crystal index lookup table for each modual pair
  // it is a 2d matrix, the row is the number of modual pair:304
  // column is the LOR number for each modual pair: (12*12*6)*(12*12*6)
  path_saved_root_file="./my_root_file.root";

  root_mem_name="mpair_lor_cry_lookup_table_1";

  write_2d_matrix_to_root(path_saved_root_file, root_mem_name,mat_1,n_mpairs,mp_size );

  root_mem_name="mpair_lor_cry_lookup_table_2";

  write_2d_matrix_to_root(path_saved_root_file, root_mem_name,mat_2,n_mpairs,mp_size );

  //-----------------------------------------




  if(dump!=NULL)
  {
    delete dump;
    dump=NULL;
  }


  if(mpair_data!=NULL)
  {
    delete [] mpair_data;
    mpair_data=NULL;
  }


  if(c_LOR_file_data!=NULL)
  {
    delete c_LOR_file_data;
    c_LOR_file_data=NULL;
  }



if(mat_1!=NULL)
{
  delete mat_1;
  mat_1=NULL;
}




if(mat_2!=NULL)
{
  delete mat_2;
  mat_2=NULL;
}

}



void get_information_backup(string lor_path,float *sum_ring_events)
//void get_information(string lor_path)
{


  //class including all the basic information of the MRPET.
  MRPET_dump *dump = new MRPET_dump();

  int n_mpairs=dump->get_n_mpairs();

  int max_ring_diff=dump->get_max_ringdiff();

  int mod_nx=dump->get_mod_nx();
  int mod_ny=dump->get_mod_ny();

  int mp_size=(mod_nx*mod_ny)*(mod_nx*mod_ny);

  int ring_num=mod_ny;

  //19*16=304
  cout<<"n_mpairs is : "<<n_mpairs <<endl;
  //67
  cout<<"max_ring_diff is : "<<max_ring_diff <<endl;
  //72
  cout<<"ring_num is : "<<ring_num <<endl;
  //12
  cout<<"mod_nx is : "<<mod_nx <<endl;
  //12*6=72
  cout<<"mod_ny is : "<< mod_ny<<endl;

  //(12*12*6)*(12*12*6)
  cout<<"mp_size is : "<<mp_size<<endl;

  const int const_n_mpairs=n_mpairs;
  int mpair_a[const_n_mpairs];
  int mpair_b[const_n_mpairs];

  for(int i=0;i<n_mpairs;i++)
  {
    dump->mpair_lookup(mpair_a[i],mpair_b[i],i);

    //cout<<"n_mpairs index is: "<<i<<"; modual a is: "<<mpair_a[i]<<"; modual b is: "<<mpair_b[i]<<endl;

  }


  //-----------------------------------------

  string path_saved_root_file="./my_root_file.root";
  string root_mem_name="modual_pair_lookup_table_a";

  write_array_to_root(path_saved_root_file,root_mem_name, mpair_a,n_mpairs);



  //-----------------------------------------




  bool is_float=true;
  cout<<"start to new class BrainPET_LORfileData"<<endl;
  BrainPET_LORfileData *c_LOR_file_data=new BrainPET_LORfileData(n_mpairs,mp_size,is_float);

  cout<<"start to load data"<<endl;

  c_LOR_file_data->load_data(lor_path.c_str(),is_float);


  cout<<"finish to load data"<<endl;

  cout<<"get the module pair data:"<<endl;

  //---modul pair data pointers,the count for each lor 

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
