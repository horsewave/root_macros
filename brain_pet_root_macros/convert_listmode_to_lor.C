/*
 * convert_listmode_to_lor: Read listmod data and save them to lor data;
 *
 *
 *
 */
#include<stdio.h>
#include<iostream.h>
#include<list>
//#include<vector>

using namespace std;



// An unsigned char can store 1 Bytes (8bits) of data (0-255)
typedef unsigned char BYTE;

// define coincedences relating to the GPU-MCS data.
typedef struct {
  int icry1;
  int icry2;
  short int icry1_ofov_scatter_z_pos;
  short int icry2_ofov_scatter_z_pos;

  // type=1: true events,no scatter;
  // type=2: ofov source, scatter occur
  // type=3: ifov source,scatter occur in ofov,then back to be detected
  // type=4: normal scatter,e.g: source ifov, scatter ifov. 
  // type=5: random events
  char type;
} Coincidence;


void convert_listmode_to_lor()
{

  int lor_size = 304*864*864;
  float * lorval_true=new float[lor_size];
  memset(lorval_true,0,sizeof(float)*lor_size);

  /*for (int i=0;i<lor_size;i++)*/
  //{
  //lorval_true[i]=1;
  /*}*/

  //string path_listmode = "/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/cylinder_phantom_norm_file_mcs/scatterMCS/run_num_5000/norm_acq_gpuSimu_0.bin";
  //string file_name = "/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/cylinder_phantom_norm_file_mcs/scatterMCS/run_num_5000/true.flor";

   string path_listmode = "/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/cylinder_phantom_norm_file_mcs/scatterMCS/run_num_5000/norm_acq_gpuSimu_0.bin";
  string file_name = "/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/cylinder_phantom_norm_file_mcs/scatterMCS/run_num_5000/true.fs";


save_coincidences_lor(path_listmode, lorval_true);

  Write_lor_file(lorval_true,file_name,lor_size);
  gApplication->Terminate();



}

/*
 *
 * Generate LOR data by default value
 *
 */
void main_1()
{
  int lor_size = 304*864*864;
  float * lorval_true=new float[lor_size];
  memset(lorval_true,0,sizeof(float)*lor_size);

  /*for (int i=0;i<lor_size;i++)*/
  //{
  //lorval_true[i]=1;
  /*}*/

  string file_name = "./true.flor";

  save_coincidences_lor(lorval_true);

  Write_lor_file(lorval_true,file_name,lor_size);

}

void main_2()
{

}


void Write_lor_file(float *lorval_true,string file_name,int data_length )
{

  string fpath_lor_true=file_name+"_true.flor";
  FILE *f1=NULL;

  f1=NULL;
  f1=fopen(fpath_lor_true.c_str(),"wb");
  fwrite(lorval_true,sizeof(float), data_length,f1);
  fclose(f1);

}


void save_coincidences_lor(string path_listmode,float* lortrue)
{

  Coincidence coin;

  fstream file;
  file.open(path_listmode.c_str(), ios::in | ios::out | ios::binary);
  //file.open(filePath, fstream::in | fstream::out | fstream::binary);

  if (file.fail())
  {
    cout << "ERROR: Cannot open the file..." << endl;
    return ;
  }



  if(lortrue!=NULL)
  {
     int  blocksize=144;
      int casssize=blocksize*6;
      int ncrystals=casssize*32;
      int Nhead_EPM=32;
      int h_pair= Nhead_EPM*Nhead_EPM;
      ////memory not free-Bo
      short* hplookup=new short[h_pair];
      memset(hplookup,0,sizeof(short)*h_pair);

      create_hplookup(hplookup);      

    while(!file.eof())
    {

      //read orderingly to the structure
      file.read((char* )(&coin.icry1), sizeof(int));
      file.read((char* )(&coin.icry2), sizeof(int));
      file.read((char* )(&coin.type), sizeof(char));

      int icry1 = coin.icry1;
      int icry2 = coin.icry2;

      if(coin.type==1)
      {

      int addr;

      int icass1 = icry1/casssize;
      int i1  = icry1 % casssize;
      int iblock1 = i1/ blocksize;
      int i2 = i1 % blocksize;
      int ic_ax = i2 / 12; 
      int ic_ta = i2 % 12;
      int ic1=143-(ic_ta*12+ic_ax);    
      int icass2 = icry2/casssize;
      i1  = icry2 % casssize;
      int iblock2 = i1/ blocksize;
      i2 = i1 % blocksize;
      ic_ax = i2 / 12; 
      ic_ta = i2 % 12;
      int ic2=143-(ic_ta*12+ic_ax);    

      addr=get_LOR_flat_address(hplookup,icass1,iblock1,ic1,icass2,iblock2,ic2);
      if(addr >0 && addr < 304*864*864)
        lortrue[addr]++;				

    }

      }
      

if(hplookup!=NULL)
  {
    delete [] hplookup;
    hplookup=NULL;
  }

  }

  




  file.close();







}


void save_coincidences_lor(float* lortrue)
{




  int  blocksize=144;
  int casssize=blocksize*6;
  int ncrystals=casssize*32;
  int Nhead_EPM=32;
  int h_pair= Nhead_EPM*Nhead_EPM;
  int addr;
  ////memory not free-Bo
  short* hplookup=new short[h_pair];
  memset(hplookup,0,sizeof(short)*h_pair);

  create_hplookup(hplookup);      
  for(int icry1=0;icry1<ncrystals;icry1++)
  {
    for(int icry2= (icry1 + 1);icry2<ncrystals;icry2++)
    {


      if(lortrue!=NULL){
        int icass1 = icry1/casssize;
        int i1  = icry1 % casssize;
        int iblock1 = i1/ blocksize;
        int i2 = i1 % blocksize;
        int ic_ax = i2 / 12; 
        int ic_ta = i2 % 12;
        int ic1=143-(ic_ta*12+ic_ax);    
        int icass2 = icry2/casssize;
        i1  = icry2 % casssize;
        int iblock2 = i1/ blocksize;
        i2 = i1 % blocksize;
        ic_ax = i2 / 12; 
        ic_ta = i2 % 12;
        int ic2=143-(ic_ta*12+ic_ax);    

        addr=get_LOR_flat_address(hplookup,icass1,iblock1,ic1,icass2,iblock2,ic2);
        if(addr >0 && addr < 304*864*864)
          lortrue[addr]++;				
      }
    }

  }


  if(hplookup!=NULL)
  {
    delete [] hplookup;
    hplookup=NULL;
  }

}




void create_hplookup(short *hplookup){

  int hoff, a,b;
  int i, o, hb ,cntr;
  const int BrainPET_n_FOV = 19;     
  const int Nhead_EPM=32;    
  int n_mpairs;



  int h_pair_total = Nhead_EPM*(Nhead_EPM-1);
  short * hpair=new short[h_pair_total];
  memset(hpair,0,sizeof(short)*h_pair_total);


  hoff=(Nhead_EPM-BrainPET_n_FOV+1)/2;
  cntr=0;
  n_mpairs=Nhead_EPM*BrainPET_n_FOV/2;
  // check to be sure we don't exceed the maximum allowable (496=32*31/2);
  if (n_mpairs > 496){
    printf("Invalid head_fov..n_mpairs\n");
    exit(0);
  }
  if (BrainPET_n_FOV%2 == 0){
    printf("FOV should be odd number\n");
    exit(0);
  }
  for (i=0; i<Nhead_EPM; i++) {
    for (o=0; o<BrainPET_n_FOV; o++) {
      hb=i+o+hoff;
      if (hb < Nhead_EPM) {
        hpair[2*cntr]=i;
        hpair[2*cntr+1]=hb;
        cntr++;
      }
    }
  }
  if (cntr != n_mpairs){
    printf("init_lor_info(): You must have goofed up somewhere n_mpairs)\n");
    exit(0);
  }
  for (i=0; i<Nhead_EPM*Nhead_EPM; i++) 
    hplookup[i]=(short)0;
  for (i=0; i<n_mpairs; i++) {
    a=hpair[2*i];
    b=hpair[2*i+1];
    hplookup[32*b+a]=(short)(i+1);
    hplookup[32*a+b]=(short)(-i-1);
  }


  if(hpair!=NULL)
  {
    delete [] hpair;
    hpair=NULL;
  }


}


void get_LOR_flat_address(short*hplookup, int h1, int b1, int c1,int h2, int b2,int c2){

  int a1; //adress of crystal_1
  int a2; //adress of crystal_2
  int ax; //tmp for switch
  int addr; //total adress of lor in lor-array

  int hp=hplookup[h1*32+h2];

  if (hp == 0) {
    return -1;
  }

  a1=144*b1+c1;
  a2=144*b2+c2;
  if (hp < 0) {
    hp=-hp;
    ax=a1;
    a1=a2;
    a2=ax;
  }

  //--- calculation of adress:
  addr=(144*6*144*6)*(hp-1)+a2*144*6+a1;
  return addr;

}
