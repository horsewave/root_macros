#include <stdlib.h>
#include <iomanip> 
using namespace std;
void img_voi_ave_std_calc_304()
{
  /* //photoelectric_hits*/
  //const int run_num=10;
  //mean=TMath::Mean(run_num,a_float_photoelectric_hits);
  //std_s=TMath::RMS(run_num,a_float_photoelectric_hits);
  //cout<<"mean_photoelectric_hits is: "<<mean<<endl;
  //cout<<"std_photoelectric_hits is: "<<std_s<<endl;

  string path_base_folder = "/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/XB1BN304N-BI/30min-data_0-30-set_insert_size/img/" ;
  string path_img_data =path_base_folder + "mcs/recon_use_mcs_true_only/recon_img_norm_with_coil.i" ;
  //string path_img_data =path_base_folder + "sss/recon_img_with_detec_s2_i64_norm_old.i";
  string path_img_voi= path_base_folder + "voi_img.i" ;

  //string saved_data_path = path_base_folder +"mcs_ave_std.txt" ;
  string saved_data_path = path_base_folder +"mcs_true_ave_std.txt" ;

  int dimx=256;
  int dimy=256;
  int dimz=153;
  int nVoxels= dimx* dimy* dimz;

  float* img_data=new float[nVoxels];
  float* img_voi=new float[nVoxels];

  memset(img_data, 0, sizeof(float)*nVoxels);
  memset(img_voi, 0, sizeof(float)*nVoxels);

  Read_image(path_img_data, img_data);
  Read_image(path_img_voi, img_voi);

  const int voi_num_per_slice=12;
  //const int voi_num_per_slice=1;

  float array_voi_val[voi_num_per_slice]={0};

  float voi_val=1.0;

  // 1.0: PTFE; 2.0: air; 3.0: water; 4-12: background
  for (int i=0;i<voi_num_per_slice;i++)
  {

    array_voi_val[i]=voi_val;

    voi_val+=1;
    cout << "voxel values for voi: " <<i<<"  is : "<< array_voi_val[i]<<endl;
  }

  int planes_num_voi=0;
  int z_start[voi_num_per_slice] ={0};
  int z_end [voi_num_per_slice] ={0};
  int pix_num_per_voi[voi_num_per_slice] ={0};

  for(int i=0;i<voi_num_per_slice;i++)
  {

    Get_voi_info(img_voi, array_voi_val[i],  planes_num_voi, z_start[i],z_end[i],pix_num_per_voi[i]);

  }


  const int voi_plane_num = planes_num_voi;


  float array_voi_res[voi_num_per_slice][voi_plane_num] ={0};

  // 1: get the sum for each ROI of each voi

  for (int i=0;i<voi_num_per_slice;i++)
  {

    Get_sum_data_voi( img_data, img_voi, array_voi_val[i],  array_voi_res[i], z_start[i], z_end[i]);

  }
  cout<<" get the sum data voi finished"<<endl;

  // 2: get the average value for each ROI of each voi
  for (int i=0;i<voi_num_per_slice;i++)
  {

    Get_ave_per_roi(array_voi_res[i],pix_num_per_voi[i],voi_plane_num );

  }
  cout<<" get the ave data voi finished"<<endl;

 
  
  double array_voi_ave[voi_num_per_slice] = {0};
  double array_voi_std[voi_num_per_slice] = {0};

  for (int i=0;i<voi_num_per_slice;i++)
  {

    Get_ave_std_array( array_voi_res[i], array_voi_ave[i],array_voi_std[i], voi_plane_num);

  }
  cout<<" get the std ave finished"<<endl;
  

 // 3:merge backgroun;
  int bg_per_plane=9;
  const int merge_plane_num=bg_per_plane * voi_plane_num;

  float merge_bg_voi[merge_plane_num] = {0};
  int bg_start=3;

  for(int i=bg_start; i<voi_num_per_slice;i++)
  {
    for(int j=0;j<voi_plane_num;j++)
    {

    int i_merge=(i-bg_start)*voi_plane_num+j;
    merge_bg_voi[i_merge]=array_voi_res[i][j];
    }
  }

  double merged_bg_ave = 0;
  double merged_bg_std = 0;

 Get_ave_std_array( merge_bg_voi, merged_bg_ave,merged_bg_std, merge_plane_num);


 cout<<" merged_ave is: "<< merged_bg_ave <<endl;
 cout<<" merged_std is: "<< merged_bg_std <<endl;

 double array_voi_merged_ave[voi_num_per_slice] = {0};
 double array_voi_merged_std[voi_num_per_slice] = {0};


 
 for(int i=0;i<voi_num_per_slice;i++)
 {
   array_voi_merged_ave[i]=merged_bg_ave;
   array_voi_merged_std[i]=merged_bg_std;

 }

  int rod_end=3;

 for(int i=0;i<rod_end;i++)
 {
   Get_error_propagation_dived(array_voi_merged_std[i], array_voi_ave[i],array_voi_std[i],merged_bg_ave, merged_bg_std);

 }



  // 3: save data

save_data(saved_data_path, array_voi_val, array_voi_ave, array_voi_std, array_voi_merged_ave, array_voi_merged_std,  pix_num_per_voi, array_voi_res, voi_num_per_slice,voi_plane_num);




  if(img_data!=NULL)
  {
    delete [] img_data;
    img_data=NULL;

  }




  if(img_voi!=NULL)
  {
    delete [] img_voi;
    img_voi=NULL;

  }

  gApplication->Terminate();

}




void Get_ave_std_array( float* array_data, double& array_voi_ave, double& array_voi_std,const int array_size)
{


  //const int run_num=10;


  array_voi_ave = TMath::Mean(array_size,array_data);
  array_voi_std = TMath::RMS(array_size,array_data);

  cout<<" ave is: " << array_voi_ave<<endl;
  cout<<" std is: " << array_voi_std<<endl;

}

void Get_ave_per_roi(float* array_voi,int pix_num_per_voi,int array_size )
{
  for(int i=0;i<array_size;i++)
  {
    if(pix_num_per_voi!=0)
    {
      array_voi[i]/=pix_num_per_voi;

    }
    else
    {
      cout<<" ERROR: the pixel number for the ROI is ZERO!"<<endl;
      exit(EXIT_FAILURE);
    }
  }

}




const int voi_num_per_slice=12;
const int voi_plane_num=90; 
/*
 * array_voi_val contains the defined value of the voi;
 * array_voi_res: is the calculated results for each voi;
 *
 *
 */
//void save_data(string saved_data_path, float *array_voi_val,float* array_voi_ave, float* array_voi_std,int* pix_num_per_voi,float array_voi_res[][voi_plane_num], int voi_num_per_slice,int voi_plane_num) 
void save_data(string saved_data_path, float *array_voi_val,double* array_voi_ave, double* array_voi_std, double* array_voi_merge_ave, double* array_voi_merge_std, int* pix_num_per_voi,float array_voi_res[][voi_plane_num], int voi_num_per_slice,int voi_plane_num) 

{

  ofstream of;
  //of.open(path_saved_data.c_str(),std::ofstream::out | std::ofstream::app);
  of.open(saved_data_path.c_str(),std::ofstream::out);

  of<<setw(9)<<"voi index"<< setw(3)<<" " ;

  for(int j=0;j<voi_num_per_slice;j++)
  {
    of<< setw(10) <<array_voi_val[j];
  }

  of<< endl;
  of<<setw(9)<<"average:"<< setw(3)<<" " ;

  for(int j=0;j<voi_num_per_slice;j++)
  {
    of<< setw(10) <<array_voi_ave[j];
    //of<< setw(10) <<"0.1";
  }
  of<< endl;

  of<<setw(9)<<"std: "<< setw(3)<<" " ;
  for(int j=0;j<voi_num_per_slice;j++)
  {
    of<< setw(10) <<array_voi_std[j];
    //of<< setw(10) <<"0.1";
  }
  of<< endl;


  of<< endl;
  of<<setw(9)<<"merg_ave:"<< setw(3)<<" " ;

  for(int j=0;j<voi_num_per_slice;j++)
  {
    of<< setw(10) <<array_voi_merge_ave[j];
    //of<< setw(10) <<"0.1";
  }
  of<< endl;

  of<<setw(9)<<"merg_std:"<< setw(3)<<" " ;
  for(int j=0;j<voi_num_per_slice;j++)
  {
    of<< setw(10) <<array_voi_merge_std[j];
    //of<< setw(10) <<"0.1";
  }
  of<< endl;




  of<<setw(9)<<"pix_num: "<< setw(3)<<" " ;
  for(int j=0;j<voi_num_per_slice;j++)
  {
    of<< setw(10) <<pix_num_per_voi[j];
  }


  of<< endl;



  for (int i=0;i<voi_plane_num;i++)
  {
    of<<endl;


    of<<"plane i: "<< setw(3)<< i;
    for(int j=0;j<voi_num_per_slice;j++)
    {
      of<< setw(10) <<array_voi_res[j][i];
    }

  }

  of<<endl;

  of.close();


}





void Read_image(string path_img, float* img_data, int dimx=256, int dimy=256, int dimz=153)
{

  int wordlength=sizeof(float);	
  //read original image to memory
  ifstream fin;
  fin.open(path_img.c_str());

  if(fin.good()){
    cout<<"Reading emission file from "<<path_img.c_str()<<endl;
    fin.read((char *)img_data, (dimx* dimy* dimz* wordlength));
    fin.close();

  }
  else
  {
    cout<<"Error opening emission file "<<path_img.c_str()<<endl;	
    exit (EXIT_FAILURE);
    fin.close();
  }



}

/*
 *  Get the average value of a given voi in the image.
 *  img_data: the recon image.
 *  img_voi: this is the image which has different vois, each voi has the same voxel value to identify each other
 *  voi_val: the voxel value of a given voi;
 *  voi_ave: the average of the image data in the given voi;
 *  voi_num: how many ROIs for each voi. eg: how many planes for each voi.
 *
 *
 *
 */

void Get_sum_data_voi(float* img_data,float* img_voi, float voi_val, float* array_voi_total, int z_start, int z_end,int dimx=256, int dimy=256, int dimz=153)
{


  int voi_index=0;

  /*  cout<<"voi_val: " << voi_val<< endl;*/
  //cout<<"z start: " << z_start<< endl;
  /*cout<<"z end: " << z_end<< endl;*/

  for(int z =z_start; z <=z_end;z++){

    int z_index_temp=z*dimy*dimx;
    float sum_voi_per_slice=0;

    for(int y = 0; y < dimy ; y++){

      int y_index_temp = y*dimx;

      for(int x = 0; x < dimx; x++){
        int img_index =z_index_temp+y_index_temp+x;
        if(img_voi[img_index]==voi_val)
        {
          sum_voi_per_slice+= img_data[img_index];

        }

      }

    }

    array_voi_total[voi_index]=sum_voi_per_slice;
    //cout<<"plane: " <<z<< "sum: "<< sum_voi_per_slice<<endl;
    voi_index++;


  }





}

void Get_voi_info(float* img_voi, float voi_val, int& voi_num, int& z_start,int& z_end,int& pix_num_per_voi,int dimx=256, int dimy=256, int dimz=153)
{


  int temp_voi_num=0;
  int temp_pix_num_per_voi=0;
  int temp_z_start=200;
  int temp_z_end=0;

  cout<< " the defined voi volue is: " << voi_val <<endl;

  for(int z =0; z <dimz;z++){

    int z_index_temp=z*dimy*dimx; 
    bool is_voi=false;

    for(int y = 0; y < dimy ; y++){

      int y_index_temp = y*dimx;

      for(int x = 0; x < dimx; x++){
        int img_index =z_index_temp+y_index_temp+x;

        if(img_voi[img_index]==voi_val)
        {

          cout<<" find one voi plane: z= "<<z<<endl;
          is_voi=true;
          break;

        }

      }
      if(is_voi==true)
      {
        break;
      }

    }

    if(is_voi == true)
    {
      temp_voi_num++;
      if(temp_z_start>z)
      {
        temp_z_start=z;
      }
      if(temp_z_end<z)
      {
        temp_z_end=z;
      }


    }
  }


  int z_index_temp=temp_z_start*dimy*dimx; 
  for(int y = 0; y < dimy ; y++){

    int y_index_temp = y*dimx;

    for(int x = 0; x < dimx; x++){
      int img_index =z_index_temp+y_index_temp+x;

      if(img_voi[img_index]==voi_val)
      {
        temp_pix_num_per_voi++;

      }

    }
  }




  pix_num_per_voi=temp_pix_num_per_voi;
  voi_num = temp_voi_num;
  z_start = temp_z_start;
  z_end =  temp_z_end;

  cout<< "voi: "<<voi_val << ": Total voi planes : "<<voi_num <<endl;
  cout<< "voi: "<<voi_val << ": voi planes start: " << z_start <<endl;
  cout<< "voi: "<<voi_val << ": voi planes ends: " << z_end<<endl;
  cout<< "voi: "<<voi_val << ": pixel num per voi: " << pix_num_per_voi<<endl;




}

void write_vector_to_root(string path_root_file, float* array_data, string vec_name, int array_size)
{
  //string path_root_file="./my_root_file.root";


  TFile* f=new TFile(path_root_file.c_str(),"update");



  //1: declare a TVector with is the child of TObject.
  //int array_size=153;
  TVector* v=new TVector(array_size);

  for(int i=0;i<array_size;i++)
  {
    //set value to the vector
    v(i) =array_data[i];
  }



  //TObjString* ts=new TObjString("The total axile distibution of the LOR data");

  //4: write to the file.
  v->Write(vec_name.c_str(),TObject::kOverwrite);//// save only the new version of the tree
  f->Close();
  v->Print();

  //delete ts;
  //ts=NULL;
  delete f;
  f=NULL;

  delete v;
  v=NULL;

}





void write_2d_array_to_root(string path_root_file, TMatrix* mat, string mat_name, int row, int column)
{
  string path_root_file="./my_root_file.root";


  TFile* f=new TFile(path_root_file.c_str(),"update");



  //1: declare a TVector with is the child of TObject.
  int column=153;
  int row=5;
  TMatrix* mat=new TMatrix(row,column);

  for(int i=0;i<row;i++)
  {
    for(int j=0;j<column;j++)
    {

      mat(i,j)=i*j;

    }

  }
  mat->Print();
  /* //TObjString* ts=new TObjString("The total axile distibution of the LOR data");*/

  //4: write to the file.
  mat->Write("my_2d_matrix",TObject::kOverwrite);//// save only the new version of the tree
  f->Close();


  delete f;
  f=NULL;


  delete mat;
  mat=NULL;




}


void Get_error_propagation_dived(double& std_result, double ave_num,double std_num,double ave_deno, double std_deno)
{

  //double sq_num=TMath::Mean(run_num,a_float_photoelectric_hits);
  if(ave_num==0 ||ave_deno==0)
  {
    cout<<"ERROR: the denominator is zero!!"<<endl;
    exit(EXIT_FAILURE);
  }
  double std_to_ave_num=std_num/ave_num; 
  double std_to_ave_deno=std_num/ave_deno;

 //double sq_std_result= TMath::Sq(std_to_ave_num)+TMath::Sq(std_to_ave_deno);
 double sq_std_result= std_to_ave_num * std_to_ave_num + std_to_ave_num * std_to_ave_deno;
 std_result=TMath::Sqrt(sq_std_result);


}
