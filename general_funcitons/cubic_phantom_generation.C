void cubic_phantom_generation()
{

  cubic_phantom_generation_float();
}







void cubic_phantom_generation_float()
{

  //
  int dimx=100;
  int dimy=100;
  int dimz=100;
 
/* int dimx=256;*/
  //int dimy=256;
  //int dimz=153;

  //string output_image="./cubic_phantom_att.i33";
  string output_image="./cubic_phantom_source.i33";

  int wordlength=sizeof(float);	
  int nVoxels = dimx*dimy*dimz;

   
  float* output_data=new float[nVoxels];
  memset(output_data, 0, sizeof(float)*nVoxels);

//total activity(Bq)
  float activity=1000000;
  float suv=activity/nVoxels;
  float voxel_value=1;
  //float voxel_value=0;

  for(int x = 0; x < dimx; x++){
    for(int y = 0; y < dimy; y++){
      for(int z = 0; z < dimz; z++){
                //why?to reduce the time?
        output_data[z*dimx*dimy + y*dimx + x]=voxel_value;	
      }
    }
  }
  //write imge to disk
  ofstream fout;
  fout.open(output_image.c_str());
  fout.write((char*) output_data, (dimx * dimy * dimz * wordlength));
  fout.close();

 
  delete [] output_data;
  output_data=NULL;


}


















void cubic_phantom_generation_unsigned_short()
{

  //
  int dimx=100;
  int dimy=100;
  int dimz=100;
 
 
  string output_image="./cubic_phantom_att.i33";
  //string output_image="./cubic_phantom_source.i33";

  int wordlength=sizeof(unsigned short);	
  int nVoxels = dimx*dimy*dimz;

   
  unsigned short* output_data=new unsigned short[nVoxels];
  memset(output_data, 0, sizeof(unsigned short)*nVoxels);

//total activity(Bq)
  float activity=1000000;
  float suv=activity/nVoxels;
  unsigned short voxel_value=1;

  for(int x = 0; x < dimx; x++){
    for(int y = 0; y < dimy; y++){
      for(int z = 0; z < dimz; z++){
                //why?to reduce the time?
        output_data[z*dimx*dimy + y*dimx + x]=voxel_value;	
      }
    }
  }
  //write imge to disk
  ofstream fout;
  fout.open(output_image.c_str());
  fout.write((char*) output_data, (dimx * dimy * dimz * wordlength));
  fout.close();

 
  delete [] output_data;
  output_data=NULL;


}

