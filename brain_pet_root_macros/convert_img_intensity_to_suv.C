/*
 * Description: This macro is used to convert the voxel intensity to activity concentration of the image.
 *              The total activity is acquired from the reconstructed image with the format of ecat(*.v).
 *              The image which is based on the sss is always needed to calculate the suv convert factor;
 *              Then you can use this suv_factor to convert the input image to intensity image.
 *
 *
 * Input: 
 * Procedure:
 *
 *
 * Functions:
 *          1: void convert_intensity_to_suv(string path_input_img,string path_image_sss,string path_output_img,int dimx=256,int dimy=256,int dimz=153, float spacing=1.25)
 *          2: void Read_image(string file_path,float* imageData,int voxel_num)
 *          3: void Write_image(string file_path,float* imageData,int voxel_num,bool is_float=true)
 *          4: float Remove_image_noise(float* input_image, float* output_image,int dimx=256,int dimy=256,int dimz=153)
 *          5: void SUV_convert(float* input_image,float SUV_factor,int z_min=20,int z_max=149,int dimx=256,int dimy=256,int dimz=153)
 *          6: float Get_max_image(float* input_image,int dimx=256,int dimy=256,int dimz=153)
 *          7: float Get_min_image(float* input_image,int dimx=256,int dimy=256,int dimz=153)
 *          8:
 *          9:
 *

 *
 *       
 *
 *
 * Editor: Bo Ma
 *
 * Date: 2018.12.07
 *
 */


void convert_img_intensity_to_suv()
{
  string base_folder = "/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/XB1BN305N-BI/XB1BN305N-BI-01/img/mcs/";
  string path_input_img = base_folder + "real_2_fov_mcs.i" ;
  string path_output_img =  base_folder +"real_2_fov_mcs_suv_test.i" ;
  string path_sss_img =  base_folder +"real_2_fov_mcs.i" ;
  
  cout<<"output is: "<< path_output_img<<endl;
  //in bq
  float total_activity=9746000;//FE2BP034F


convert_intensity_to_suv(path_input_img,path_sss_img,path_output_img);


}

void convert_intensity_to_suv(string path_input_img,string path_image_sss,string path_output_img,int dimx=256,int dimy=256,int dimz=153, float spacing=1.25)
{

  int nVoxels = dimx*dimy*dimz;

  float *image_data_input=new float[nVoxels];
  memset(image_data_input,0,sizeof(float)*nVoxels);
  float *image_data_sss=new float[nVoxels];
  memset(image_data_sss,0,sizeof(float)*nVoxels);

//------------------1: read image whose voxel value is reconstructed intensity--------------------
  //read data from disk to memory
  Read_image(path_input_img, image_data_input,nVoxels);
  Read_image(path_image_sss, image_data_sss,nVoxels);


  float *suv_data_input=new float[nVoxels];
  memset(suv_data_input,0,sizeof(float)*nVoxels);
  float *suv_data_sss=new float[nVoxels];
  memset(suv_data_sss,0,sizeof(float)*nVoxels);

  //in bq
  float total_activity=9746000;//FE2BP034F

  float total_intensity_input= Remove_image_noise(image_data_input,suv_data_input);
  float total_intensity_sss= Remove_image_noise(image_data_sss,suv_data_sss);

  float SUV_factor=total_activity/total_intensity_sss;

  //
  float voxel_volume=spacing*spacing*spacing;
  SUV_factor=SUV_factor/voxel_volume;

  SUV_convert(suv_data_input,SUV_factor);

  //--------save image---------
  Write_image(path_output_img, suv_data_input,nVoxels);



if(image_data_input!=NULL)
{
  delete [] image_data_input;
  image_data_input=NULL;

}

if(image_data_sss!=NULL)
{
  delete [] image_data_sss;
  image_data_sss=NULL;
}

if(suv_data_input!=NULL)
{
  delete [] suv_data_input;
  suv_data_input=NULL;
}


if(suv_data_sss!=NULL)
{
  delete [] suv_data_sss;
  suv_data_sss=NULL;
}




}

//read image from the disk to the memory
void Read_image(string file_path,float* imageData,int voxel_num)
{
  ifstream fin;
	fin.open(file_path.c_str());

	fin.read((char *)imageData, (voxel_num* sizeof(float)));
	fin.close();

}


//read image from the disk to the memory
void Write_image(string file_path,float* imageData,int voxel_num,bool is_float=true)
{
    int wordlength;

  if(is_float==true)
  {
    wordlength=sizeof(float);	

  }

    else
    {
      cout<<"saved image should be float ,but it is not.What happened???"<<endl;
      return;
    }
 //write imge to disk
  ofstream fout;
  fout.open(file_path.c_str());
  fout.write((char*) imageData, (voxel_num* wordlength));
  fout.close();
 
}


//----remove the noise of the input image, set the voxels below the threshold as 0
//-----threshold should be set in the function
//-----return: the sum of the image intensity.
float Remove_image_noise(float* input_image, float* output_image,int dimx=256,int dimy=256,int dimz=153)
{
  float Maximu_after_scale=256.0;

  float threshold_val=5.0;
  //float threshold_val=10.0;
  //truncate the original data with the head and tail which is really noisy
  int z_min=20;
  int z_max=149;

  float max_value=Get_max_image(input_image);
  float min_value=Get_min_image(input_image);;

  float max_minus_min=max_value-min_value;


  for(int z=z_min;z<z_max;z++)
  {
    for(int y = 0; y < dimy; y++)
    {

      for(int x= 0; x< dimx; x++)
      {
        float val= input_image[z*dimx*dimy + y*dimx + x];
        val=((val-min_value)/max_minus_min)*Maximu_after_scale;
        if(val<threshold_val)
        {
          val=0;
        }
        output_image[z*dimx*dimy + y*dimx + x]=val;
      }

    }	
  }


  float total_value_before=0.0;
   for(int z=z_min;z<z_max;z++)
  {
    for(int y = 0; y < dimy; y++)
    {

      for(int x= 0; x< dimx; x++)
      {
        float val= output_image[z*dimx*dimy + y*dimx + x];
        
        if(val>0)
        {
          output_image[z*dimx*dimy + y*dimx + x]=input_image[z*dimx*dimy + y*dimx + x];
          total_value_before+= output_image[z*dimx*dimy + y*dimx + x];
        }
      }

    }	
  }


  return total_value_before;
}


/* function name:SUV_convert(float* input_image, float* output_image,float total_activity)
 * Parametors: total_activity: the total activity in kBq
 *
 *
 * */
void SUV_convert(float* input_image,float SUV_factor,int z_min=20,int z_max=149,int dimx=256,int dimy=256,int dimz=153)
{
  //truncate the original data with the head and tail which is really noisy
  //int z_min=20;
  //int z_max=149;

 for(int z=z_min;z<z_max;z++)
 {
   for(int y = 0; y < dimy; y++)
   {

     for(int x= 0; x< dimx; x++)
     {
       float val= input_image[z*dimx*dimy + y*dimx + x];
       input_image[z*dimx*dimy + y*dimx + x]=val*SUV_factor;
     }

   }	
 }

}




float Get_max_image(float* input_image,int dimx=256,int dimy=256,int dimz=153)
{
  int z_min=20;
  int z_max=149;

  float max_value=0.0;
  //float min_value=10000.0;
  for(int z=z_min;z<z_max;z++)
  {
    for(int y = 0; y < dimy; y++)
    {

      for(int x= 0; x< dimx; x++)
      {
        float val= input_image[z*dimx*dimy + y*dimx + x];
        if(max_value<val)
        {
          max_value=val;
        }
        /*if(min_value>val)*/
        //{
        //min_value=val;
        /*}*/

      }

    }	
  }


  return max_value;

}


float Get_min_image(float* input_image,int dimx=256,int dimy=256,int dimz=153)
{
  int z_min=20;
  int z_max=149;

  //float max_value=0.0;
  float min_value=1000000.0;
  for(int z=z_min;z<z_max;z++)
  {
    for(int y = 0; y < dimy; y++)
    {

      for(int x= 0; x< dimx; x++)
      {
        float val= input_image[z*dimx*dimy + y*dimx + x];
        /*if(max_value<val)*/
        //{
        //max_value=val;
        /*}*/
        if(min_value>val)
        {
          min_value=val;
        }

      }

    }	
  }


  return min_value;

}


