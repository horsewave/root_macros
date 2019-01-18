/*
 *Discription: enOFOV the image to the default image size for BrainPET(256*256*153)
Editor: Bo Ma
Time: 2018.08.22
 * */

void Image_merge()
{


  //input with scatter correction
  //string base_folder="/data/PET/mr_pet_temp/Ma/software/data/random_OFOV/Random-OFOV-activity-C11-2016-09-08/use_dead_scat_correction/XB1BN286N-BI-01/img/";
  //string base_folder="/data/PET/mr_pet_temp/Ma/software/data/random_OFOV/Random-OFOV-activity-C11-2016-09-08/use_dead_scat_correction/XB1BN286N-BI-02/img/";
  //string base_folder="/data/PET/mr_pet_temp/Ma/software/data/random_OFOV/Random-OFOV-activity-C11-2016-09-08/use_dead_scat_correction/XB1BN286N-BI-03/img/";
  //string base_folder="/data/PET/mr_pet_temp/Ma/software/data/random_OFOV/Random-OFOV-activity-C11-2016-09-08/use_dead_scat_correction/XB1BN286N-BI-04/img/";
  //string base_folder="/data/PET/mr_pet_temp/Ma/software/data/random_OFOV/Random-OFOV-activity-C11-2016-09-08/use_dead_scat_correction/XB1BN286N-BI-05/img/";
  //string base_folder="/data/PET/mr_pet_temp/Ma/software/data/random_OFOV/Random-OFOV-activity-C11-2016-09-08/use_dead_scat_correction/XB1BN286N-BI-06/img/";
  //string base_folder="/data/PET/mr_pet_temp/Ma/software/data/random_OFOV/Random-OFOV-activity-C11-2016-09-08/use_dead_scat_correction/XB1BN286N-BI-07/img/";
  string base_folder="/data/PET/mr_pet_temp/Ma/software/data/random_OFOV/Random-OFOV-activity-C11-2016-09-08/use_dead_scat_correction/XB1BN286N-BI-08/img/";

  string patient_name_input="range_0-1223_reco_W3_na.dat";
  string patient_name_output="range_0-1223_reco_W3_na_3_ofov.dat";
  
  string input_image_att="/data/PET/mr_pet_temp/Ma/software/data/random_OFOV/Random-OFOV-activity-C11-2016-09-08/common_files/atten/XB1BN286N_AttenMap_HeadOnly_3_fov.i";
  //1: merge two Fov

  string input_image_IFOV= base_folder +patient_name_input;
  string input_image_OFOV=base_folder +patient_name_input;

  string output_image=base_folder +"merged_2_fov.data";
  
  //Image_merge_Ge_phantom( input_image_IFOV,input_image_OFOV, output_image);
  
  //2: merge to three fov
  input_image_OFOV=output_image;
  output_image=base_folder +"./merged_3_fov.data";

  //Image_merge_Ge_phantom_tri_fov(input_image_IFOV,input_image_OFOV, output_image);

  //3:set the emission value of the Ge phantom

  //string input_image_emi=output_image;
  output_image=base_folder+patient_name_output;
  string input_image_emi=output_image;
  
  output_image=output_image;

  float threshold_att=0.090;
  //float threshold_att=0;

  Set_img_value_Ge_phantom(input_image_emi,input_image_att,output_image,threshold_att);


  //4: set the emission value of the c11 phantom
  input_image_emi=output_image;
  output_image=output_image;

  //float C11_activity=361.6;//frame_01
  //float C11_activity=180.8;//frame_02
  //float C11_activity=90.4;//frame_03
  //float C11_activity=45.2;//frame_04
  //float C11_activity=22.6;//frame_05
  //float C11_activity=11.3;//frame_06
  //float C11_activity=5.6;//frame_07
  float C11_activity=2.8;//frame_08
  Set_img_value_C11_phantom(input_image_emi,input_image_att,output_image,C11_activity);


  gApplication->Terminate();




}
void Image_merge( string input_image_IFOV,string input_image_OFOV, string output_image)
{



  int dimx_IFOV=256;
  int dimy_IFOV=256;
  int dimz_IFOV=153;

  int dimx_OFOV=256;
  int dimy_OFOV=256;
  int dimz_OFOV=153;

  int dimx_merge=256;
  int dimy_merge=256;
  int dimz_merge=306;


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
  Read_image(input_image_OFOV,  image_OFOV, dimx_OFOV, dimy_OFOV, dimz_OFOV);



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


  Set_gap_value(image_merge,dimx_merge,dimy_merge,dimz_merge);


  Save_image(output_image, image_merge, dimx_merge, dimy_merge,dimz_merge );

  
  delete [] image_OFOV;
  image_OFOV=NULL;
  delete [] image_IFOV;
  image_IFOV=NULL;
  delete [] image_merge;
  image_merge=NULL;



}



void Image_merge_tri_fov( string input_image_IFOV,string input_image_OFOV, string output_image)
{



  int dimx_IFOV=256;
  int dimy_IFOV=256;
  int dimz_IFOV=153;

  int dimx_OFOV=256;
  int dimy_OFOV=256;
  int dimz_OFOV=306;

  int dimx_merge=256;
  int dimy_merge=256;
  int dimz_merge=459;


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
  Read_image(input_image_OFOV,  image_OFOV, dimx_OFOV, dimy_OFOV, dimz_OFOV);



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


  Set_gap_value(image_merge,dimx_merge,dimy_merge,dimz_merge);

 Save_image(output_image, image_merge, dimx_merge, dimy_merge,dimz_merge );

  delete [] image_OFOV;
  image_OFOV=NULL;
  delete [] image_IFOV;
  image_IFOV=NULL;
  delete [] image_merge;
  image_merge=NULL;



}




void Image_merge_Ge_phantom( string input_image_IFOV,string input_image_OFOV, string output_image)
{



  int dimx_IFOV=256;
  int dimy_IFOV=256;
  int dimz_IFOV=153;

  int dimx_OFOV=256;
  int dimy_OFOV=256;
  int dimz_OFOV=153;

  int dimx_merge=256;
  int dimy_merge=256;
  int dimz_merge=306;


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
  Read_image(input_image_OFOV,  image_OFOV, dimx_OFOV, dimy_OFOV, dimz_OFOV);

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


  Set_gap_value(image_merge,dimx_merge,dimy_merge,dimz_merge);

  Save_image(output_image, image_merge, dimx_merge, dimy_merge,dimz_merge );


  
  delete [] image_OFOV;
  image_OFOV=NULL;
  delete [] image_IFOV;
  image_IFOV=NULL;
  delete [] image_merge;
  image_merge=NULL;



}







void Image_merge_Ge_phantom_tri_fov( string input_image_IFOV,string input_image_OFOV, string output_image)
{



  int dimx_IFOV=256;
  int dimy_IFOV=256;
  int dimz_IFOV=153;

  int dimx_OFOV=256;
  int dimy_OFOV=256;
  int dimz_OFOV=306;

  int dimx_merge=256;
  int dimy_merge=256;
  int dimz_merge=459;


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
  Read_image(input_image_OFOV,  image_OFOV, dimx_OFOV, dimy_OFOV, dimz_OFOV);


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


  Set_gap_value(image_merge,dimx_merge,dimy_merge,dimz_merge);


   Save_image(output_image, image_merge, dimx_merge, dimy_merge,dimz_merge );

  delete [] image_OFOV;
  image_OFOV=NULL;
  delete [] image_IFOV;
  image_IFOV=NULL;
  delete [] image_merge;
  image_merge=NULL;



}







void Set_gap_value(float * img_data,int dim_x,int dim_y,int dim_z)
{

  //int cover_z_start=149;
  int cover_z_start=145;
  int cover_z_end=174;

  int half_z=dim_z/2;
  int gap_z=cover_z_end-cover_z_start+1;

  int z_ave_num=10;

  int z_head_gap=0;
  int z_feet_gap=gap_z-z_head_gap;


  int voxel_per_slice=dim_x*dim_y;


  float *head_ave=new float[voxel_per_slice];
  memset(head_ave, 0, sizeof(float)*voxel_per_slice);

  float *foot_ave=new float[voxel_per_slice];
  memset(foot_ave, 0, sizeof(float)*voxel_per_slice);



  //get the sum of the head side, sum the value of z_ave_num slices;

  for(int z =(cover_z_start-z_ave_num); z <cover_z_start;z++){

    int z_index_temp=z*dim_y*dim_x; 

    for(int y = 0; y < dim_y ; y++){

      int y_index_temp = y*dim_x;


      for(int x = 0; x < dim_x; x++){
        int img_merge_index =z_index_temp+y_index_temp+x;

        int slice_index=y_index_temp+x;
        head_ave[slice_index]+=img_data[img_merge_index];


      }

    }
  }

  //get the sum of the foot side, sum the value of z_ave_num slices;

  for(int z =cover_z_end; z <(cover_z_end+z_ave_num);z++){

    int z_index_temp=z*dim_y*dim_x; 

    for(int y = 0; y < dim_y ; y++){

      int y_index_temp = y*dim_x;


      for(int x = 0; x < dim_x; x++){
        int img_merge_index =z_index_temp+y_index_temp+x;

        int slice_index=y_index_temp+x;
        foot_ave[slice_index]+=img_data[img_merge_index];


      }

    }
  }


  //average the value

  for(int y = 0; y < dim_y ; y++){

    int y_index_temp = y*dim_x;


    for(int x = 0; x < dim_x; x++){

      int slice_index=y_index_temp+x;
      foot_ave[slice_index]=foot_ave[slice_index]/z_ave_num;
      head_ave[slice_index]=head_ave[slice_index]/z_ave_num;

    }

  }





  //set the head end;

  for(int z =cover_z_start; z <(cover_z_start+z_head_gap);z++){

    int z_index_temp=z*dim_y*dim_x; 

    for(int y = 0; y < dim_y ; y++){

      int y_index_temp = y*dim_x;


      for(int x = 0; x < dim_x; x++){
        int img_merge_index =z_index_temp+y_index_temp+x;

        int slice_index=y_index_temp+x;
        img_data[img_merge_index]=head_ave[slice_index];

      }

    }
  }


  //set the foot end;

  for(int z =(cover_z_end-z_feet_gap+1); z <=cover_z_end;z++){

    int z_index_temp=z*dim_y*dim_x; 

    for(int y = 0; y < dim_y ; y++){

      int y_index_temp = y*dim_x;


      for(int x = 0; x < dim_x; x++){
        int img_merge_index =z_index_temp+y_index_temp+x;

        int slice_index=y_index_temp+x;
        img_data[img_merge_index]=foot_ave[slice_index];

      }

    }
  }





  delete [] foot_ave;
  foot_ave=NULL;
  delete [] head_ave;
  head_ave=NULL;



}


void Set_img_value_Ge_phantom(string input_image_emi,string input_image_att,string output_image, float threshold_att)
{

  int dimx=256;
  int dimy=256;
  int dimz=459;

  /*float threshold_att=0.09;*/
  //int z_position_start=145;
  /*int z_position_end=219;*/

  int nVoxels= dimx* dimy* dimz;


  float *image_data_emi=new float[nVoxels];
  memset(image_data_emi, 0, sizeof(float)*nVoxels);



  float *image_data_att=new float[nVoxels];
  memset(image_data_att, 0, sizeof(float)*nVoxels);


  Read_image(input_image_emi,  image_data_emi, dimx, dimy, dimz);
  Read_image(input_image_att,  image_data_att, dimx, dimy, dimz);

  int z_position_start=0;
  int z_position_end=14;
  //int z_position_end=211;

  float set_value=0;

  Set_img_value(image_data_emi,image_data_att, dimx,dimy, dimz,set_value,threshold_att,z_position_start,z_position_end);

  z_position_start=14;
  z_position_end=187;


  int z_ifov_start=30;
  int z_ifov_end=130;

  //float set_value=Get_ave_value(image_data_emi,image_data_att,threshold_att, dimx,dimy, dimz,z_ifov_start,z_ifov_end);
  int sum_voxel_num_Ge=Get_sum_effect_voxel(image_data_emi,image_data_att,threshold_att, dimx,dimy, dimz,z_position_start,z_position_end);

  float Ge_activity=32400000.0;//32.4MBq

  //float set_value=Get_ave_value(image_data_emi,image_data_att,threshold_att, dimx,dimy, dimz,z_ifov_start,z_ifov_end);
  float set_value=Ge_activity/sum_voxel_num_Ge;

  cout<<"ge voxel is: "<<sum_voxel_num_Ge<<"voxel value is: "<<set_value<<endl;
  //set water att:0.3
  //float set_value=0.096;
  Set_img_value(image_data_emi,image_data_att, dimx,dimy, dimz,set_value,threshold_att,z_position_start,z_position_end);
  
  z_position_start=187;
  z_position_end=211;


  set_value=0;
  Set_img_value(image_data_emi,image_data_att, dimx,dimy, dimz,set_value,threshold_att,z_position_start,z_position_end);



    //z_position_start=371;
  //z_position_end=387;


  //set_value=0.096;
  ////Set_img_value(image_data_emi,image_data_att, dimx,dimy, dimz,set_value,threshold_att,z_position_start,z_position_end);

   //z_position_start=387;
  //z_position_end=459;


  /*set_value=0;*/
  //Set_img_value(image_data_emi,image_data_att, dimx,dimy, dimz,set_value,threshold_att,z_position_start,z_position_end);





  Save_image(output_image,  image_data_emi, dimx, dimy, dimz); 



  delete [] image_data_emi;
  image_data_emi=NULL;


  delete [] image_data_att;
  image_data_att=NULL;




}





void Set_img_value_C11_phantom(string input_image_emi,string input_image_att,string output_image,float C11_activity)
{

  int dimx=256;
  int dimy=256;
  int dimz=459;

  /*float threshold_att=0.09;*/
  //int z_position_start=145;
  /*int z_position_end=219;*/

  int nVoxels= dimx* dimy* dimz;


  float *image_data_emi=new float[nVoxels];
  memset(image_data_emi, 0, sizeof(float)*nVoxels);



  float *image_data_att=new float[nVoxels];
  memset(image_data_att, 0, sizeof(float)*nVoxels);


  Read_image(input_image_emi,  image_data_emi, dimx, dimy, dimz);
  Read_image(input_image_att,  image_data_att, dimx, dimy, dimz);



  int z_position_start=15;
  int z_position_end=187;

  float threshold_att=0.09;

  float ave_value_Ge=Get_ave_value(image_data_emi,image_data_att,threshold_att, dimx,dimy, dimz,z_position_start,z_position_end);
  int sum_voxel_num_Ge=Get_sum_effect_voxel(image_data_emi,image_data_att,threshold_att, dimx,dimy, dimz,z_position_start,z_position_end);

  cout<<"the activity of Ge phantom is: "<<ave_value_Ge*sum_voxel_num_Ge<<endl;

  z_position_start=211;
  //z_position_start=153;
  z_position_end=371;

  int sum_voxel_num_C11=Get_sum_effect_voxel(image_data_emi,image_data_att,threshold_att, dimx,dimy, dimz,z_position_start,z_position_end);

  float Ge_activity=32.4;//MBq
  //float C11_activity=360;

  float scale_factor=C11_activity/Ge_activity;


  float set_value=(scale_factor*ave_value_Ge*sum_voxel_num_Ge)/sum_voxel_num_C11;

  Set_img_value(image_data_emi,image_data_att, dimx,dimy, dimz,set_value,threshold_att,z_position_start,z_position_end);

  float ave_value_C11=Get_ave_value(image_data_emi,image_data_att,threshold_att, dimx,dimy, dimz,z_position_start,z_position_end);
  sum_voxel_num_C11=Get_sum_effect_voxel(image_data_emi,image_data_att,threshold_att, dimx,dimy, dimz,z_position_start,z_position_end);

  cout<<"the activity of C11 phantom is: "<<ave_value_C11*sum_voxel_num_C11<<endl;



  z_position_start=371;
  z_position_end=459;

  set_value=0;

  Set_img_value(image_data_emi,image_data_att, dimx,dimy, dimz,set_value,threshold_att,z_position_start,z_position_end);

  Save_image(output_image,  image_data_emi, dimx, dimy, dimz); 



  delete [] image_data_emi;
  image_data_emi=NULL;


  delete [] image_data_att;
  image_data_att=NULL;




}









void Set_img_value(float *image_data_emi,float *image_data_att,int dimx,int dimy, int dimz,float set_value,float threshold_att=0.09,int z_position_start,int z_position_end)
  //void Set_img_value(string input_image_emi,string input_image_att int dimx,int dimy, int dimz,float set_value,float threshold_att=0.09,int z_position_start,int z_position_end)
{



  int nVoxels= dimx* dimy* dimz;





  //float *image_data_emi=new float[nVoxels];
  //memset(image_data_emi, 0, sizeof(float)*nVoxels);



  //float *image_data_att=new float[nVoxels];
  //memset(image_data_att, 0, sizeof(float)*nVoxels);



  //Read_image(input_image_emi,  image_data_emi, dimx, dimy, dimz);
  //Read_image(input_image_att,  image_data_att, dimx, dimy, dimz);



  for(int z =z_position_start; z <z_position_end;z++){

    int z_index_temp=z*dimy*dimx; 
    //int z_index_temp_2=(z+8)*dimy*dimx; 

    for(int y = 0; y < dimy ; y++){

      int y_index_temp = y*dimx;


      for(int x = 0; x < dimx; x++){
        int img_index =z_index_temp+y_index_temp+x;
        //int img_index_2 =z_index_temp_2+y_index_temp+x;

        if(image_data_att[img_index]>threshold_att)
        {

          image_data_emi[img_index]=set_value;
          //image_data_emi[img_index]=image_data_emi[img_index_2];
          //image_data_emi[img_index]=0;

        }
        else
        {
          image_data_emi[img_index]=0;


        }



      }

    }
  }


  //delete [] image_data_emi;
  //image_data_emi=NULL;


  //delete [] image_data_att;
  //image_data_att=NULL;




}


float Get_ave_value(float* image_data_emi,float* image_data_att,float threshold_att, int dimx,int dimy, int dimz,int z_position_start,int z_position_end)
  //float Get_ave_value(string path_img_emi,string path_img_att,float threshold_att,float threshold_att, int dimx,int dimy, int dimz,int z_position_start,int z_position_end)

{


  //the average value to return;
  float ave_value=0;
  //sum of all the volues of the voxel
  float sum_value=0;
  //sum of the voxel numbers which is the source.
  int sum_vox_num_eff=0;
  //the threshhold value of the attenuation map to define the source range
  //float threshold_att;

  // slices to get the ave value
  int num_slice=z_position_end-z_position_start+1;




  for(int z =z_position_start; z <z_position_end;z++){

    int z_index_temp=z*dimy*dimx; 

    for(int y = 0; y < dimy ; y++){

      int y_index_temp = y*dimx;


      for(int x = 0; x < dimx; x++){
        int img_index =z_index_temp+y_index_temp+x;

        if(image_data_att[img_index]>threshold_att)
        {
          sum_vox_num_eff++;

          sum_value+=image_data_emi[img_index];

        }



      }

    }
  }

  ave_value=sum_value/sum_vox_num_eff;

  //delete [] image_data_emi;
  //image_data_emi=NULL;
  //delete [] image_data_att;
  //image_data_att=NULL;

  return ave_value;

}


int Get_sum_effect_voxel(float* image_data_emi,float* image_data_att,float threshold_att, int dimx,int dimy, int dimz,int z_position_start,int z_position_end)

{


  //the average value to return;
  float ave_value=0;
  //sum of all the volues of the voxel
  float sum_value=0;
  //sum of the voxel numbers which is the source.
  int sum_vox_num_eff=0;
  //the threshhold value of the attenuation map to define the source range
  //float threshold_att;

  // slices to get the ave value
  int num_slice=z_position_end-z_position_start+1;


  for(int z =z_position_start; z <z_position_end;z++){

    int z_index_temp=z*dimy*dimx; 

    for(int y = 0; y < dimy ; y++){

      int y_index_temp = y*dimx;


      for(int x = 0; x < dimx; x++){
        int img_index =z_index_temp+y_index_temp+x;

        if(image_data_att[img_index]>threshold_att)
        {
          sum_vox_num_eff++;

        }



      }

    }
  }



  return sum_vox_num_eff;

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


}

