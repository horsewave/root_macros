/*
 *Discription: combine the phantom and the coil. to the default image size for BrainPET(320*320*153)
Editor: Bo Ma
Time: 2018.08.22
 * */

void Image_merge_coil()
{
  //***305
 /* string path_base="/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/XB1BN305N-BI/atten/att_realistic/";*/
  //string head_name="XB1BN305N-BI-03_04/XB1BN305N-BI-03-04_2_fov_att_map.i";
  ////string head_name="XB1BN305N-BI-03_04/XB1BN305N-BI-03-04_3_fov_att_map.i";
  
  //string coil_name="att_map_merged_coil_320_320_306.data";
  ////string coil_name="att_map_merged_coil_320_320_459.data";
  
  //string output_name="XB1BN305N-BI-03_04/XB1BN305N-BI-03-04_2_fov_att_map_with_coil.i";
  /*//string output_name="XB1BN305N-BI-03_04/XB1BN305N-BI-03-04_3_fov_att_map_with_coil.i";*/
  
  /*//310*/
    //string path_base="/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/XB1BN310N-BI/atten/att_realistic/";
  ////string head_name="XB1BN310N-BI-03_04/XB1BN310N-BI-03-04_2_fov_att_map.i";
  //string head_name="XB1BN310N-BI-03_04/XB1BN310N-BI-03-04_3_fov_att_map.i";
 

  ////string coil_name="att_map_merged_coil_320_320_306.data";
  //string coil_name="att_map_merged_coil_320_320_459.data";
  
  ////string output_name="XB1BN310N-BI-03_04/XB1BN310N-BI-03-04_2_fov_att_map_with_coil.i";
  //string output_name="XB1BN310N-BI-03_04/XB1BN310N-BI-03-04_3_fov_att_map_with_coil.i";



  //emission image
 string path_base="/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/XB1BN305N-BI/XB1BN305N-BI-03/img/mcs/emi_img_realistic/";
  string head_name="XB1BN305N-BI-03_2_fov_emi_img_real.i";
  //string head_name="XB1BN305N-BI-03_04/XB1BN305N-BI-03-04_3_fov_att_map.i";
  
  //string coil_name="att_map_merged_coil_320_320_306.data";
  string coil_name="att_map_merged_coil_320_320_459.data";
  
  string output_name="XB1BN305N-BI-03_2_fov_emi_img_real_320_320_306.i";




  string path_head=path_base+head_name;
  string path_coil=path_base+coil_name;
  string path_output_image=path_base+output_name;

  //int dimz=306;
  int dimz=459;
  Image_merge_emission_img(path_head, path_coil,path_output_image,dimz);

}


void Image_merge(string path_head, string path_coil,string path_output_image,int dimz)
{


  int dimx_head=256;
  int dimy_head=256;
  int dimz_head=dimz;

  int dimx_coil=320;
  int dimy_coil=320;
  int dimz_coil=dimz;

  int nVoxels_head= dimx_head* dimy_head* dimz;
  int nVoxels_coil= dimx_coil* dimy_coil* dimz;

 //return;

  float *image_head=new float[nVoxels_head];
  memset(image_head, 0, sizeof(float)*nVoxels_head);

  float *image_coil=new float[nVoxels_coil];
  memset(image_coil, 0, sizeof(float)*nVoxels_coil);

  Read_image(path_head,  image_head, dimx_head, dimy_head, dimz);
  Read_image(path_coil,  image_coil, dimx_coil, dimy_coil, dimz);



    for(int x = (dimx_coil - dimx_head)/2; x < (dimx_coil + dimx_head)/2; x++){
      for(int y = (dimy_coil - dimy_head)/2; y < (dimy_coil + dimy_head)/2; y++){
        for(int z = (dimz_coil - dimz)/2; z < (dimz_coil + dimz)/2; z++){
          int index_coil=z*dimx_coil*dimy_coil + y*dimx_coil + x;
          int index_head=(z-(dimz_coil-dimz_head)/2)*dimx_head*dimy_head 
                         + (y-(dimy_coil-dimy_head)/2)*dimx_head
                         + x-(dimx_coil-dimx_head)/2;
          if(image_coil[index_coil]==0)
            image_coil[index_coil]= image_head[index_head];			
        }
      }
    }

Save_image(path_output_image,  image_coil, dimx_coil, dimy_coil, dimz);



  delete [] image_head;
  image_head=NULL;
  delete [] image_coil;
  image_coil=NULL;



}



void Image_merge_emission_img(string path_head, string path_coil,string path_output_image,int dimz)
{


  int dimx_head=256;
  int dimy_head=256;
  int dimz_head=dimz;

  int dimx_coil=320;
  int dimy_coil=320;
  int dimz_coil=dimz;

  int nVoxels_head= dimx_head* dimy_head* dimz;
  int nVoxels_coil= dimx_coil* dimy_coil* dimz;

 //return;

  float *image_head=new float[nVoxels_head];
  memset(image_head, 0, sizeof(float)*nVoxels_head);

  float *image_coil=new float[nVoxels_coil];
  memset(image_coil, 0, sizeof(float)*nVoxels_coil);

  Read_image(path_head,  image_head, dimx_head, dimy_head, dimz);
  //Read_image(path_coil,  image_coil, dimx_coil, dimy_coil, dimz);



    for(int x = (dimx_coil - dimx_head)/2; x < (dimx_coil + dimx_head)/2; x++){
      for(int y = (dimy_coil - dimy_head)/2; y < (dimy_coil + dimy_head)/2; y++){
        for(int z = (dimz_coil - dimz)/2; z < (dimz_coil + dimz)/2; z++){
          int index_coil=z*dimx_coil*dimy_coil + y*dimx_coil + x;
          int index_head=(z-(dimz_coil-dimz_head)/2)*dimx_head*dimy_head 
                         + (y-(dimy_coil-dimy_head)/2)*dimx_head
                         + x-(dimx_coil-dimx_head)/2;
          if(image_coil[index_coil]==0)
            image_coil[index_coil]= image_head[index_head];			
        }
      }
    }

Save_image(path_output_image,  image_coil, dimx_coil, dimy_coil, dimz);



  delete [] image_head;
  image_head=NULL;
  delete [] image_coil;
  image_coil=NULL;



}

void Image_create_with_coilv( string output_image,int dimz_merge)
{


  string input_image_IFOV="./ctJuelich_Komplett_rsl_segmented_320x153_nobed.i";

  //string output_image="./cubic_phantom_att_OFOV.i33";
  //string output_image="./merged_coil_320*320*459.data";

  int dimx_IFOV=320;
  int dimy_IFOV=320;
  int dimz_IFOV=153;

  int dimx_OFOV=320;
  int dimy_OFOV=320;
  int dimz_OFOV=dimz_merge-dimz_IFOV;

  int dimx_merge=320;
  int dimy_merge=320;
  //int dimz_merge=459;


  int nVoxels_IFOV = dimx_IFOV * dimy_IFOV * dimz_IFOV;

  int nVoxels_OFOV = dimx_OFOV*dimy_OFOV*dimz_OFOV;

  int nVoxels_merge = dimx_merge*dimy_merge*dimz_merge;


  //return;

  float *image_IFOV=new float[nVoxels_IFOV];
  memset(image_IFOV, 0, sizeof(float)*nVoxels_IFOV);

  float *image_OFOV=new float[nVoxels_OFOV];
  memset(image_OFOV, 0, sizeof(float)*nVoxels_OFOV);

  float *image_merge=new float[nVoxels_merge];
  memset(image_merge, 0, sizeof(float)*nVoxels_merge);


  Read_image(input_image_IFOV,  image_IFOV, dimx_IFOV, dimy_IFOV, dimz_IFOV);

  //add the head data to the OFOV data.
  for(int z =0 ; z <dimz_merge;z++){

    int z_index_temp=z*dimy_OFOV*dimx_OFOV; 

    for(int y = 0; y < dimy_OFOV ; y++){

      int y_index_temp = y*dimx_OFOV;


      for(int x = 0; x < dimx_OFOV; x++){
        int img_merge_index =z_index_temp+y_index_temp+x;


        if(z<dimz_IFOV)
        {
          image_merge[img_merge_index] = image_IFOV[img_merge_index];
        }
        else
        {
          int img_ofov_index=img_merge_index-(dimz_IFOV*dimy_IFOV*dimx_IFOV);
          image_merge[img_merge_index] = image_OFOV[img_ofov_index];



        }

      }
    }
  }


Save_image(output_image,  image_merge, dimx_merge, dimy_merge, dimz_merge);



  delete [] image_OFOV;
  image_OFOV=NULL;
  delete [] image_IFOV;
  image_IFOV=NULL;
  delete [] image_merge;
  image_merge=NULL;





}


void Read_image(string path_img, float* img_data, int dimx, int dimy, int dimz)
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
    fin.close();
  }



}


void Save_image(string path_saved_img, float* img_data, int dimx, int dimy, int dimz)
{



  int wordlength=sizeof(float);	

  ofstream fout;
  fout.open(path_saved_img.c_str());
  fout.write((char*)img_data, (dimx* dimy* dimz* wordlength));
  fout.close();
    cout<<"Image saved to : "<<path_saved_img.c_str()<<endl;



}

