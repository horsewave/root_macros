void read_sino()
{
  string path_sino = "/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/cylinder_phantom_norm_file_mcs/test.fs" ;

   int dimx=256;
  int dimy=192;
  int dimz=1399;
  int nVoxels= dimx* dimy* dimz;

  float* sino_data=new float[nVoxels];
  memset(sino_data, 0, sizeof(float)*nVoxels);

  read_sino_flat_ma(path_sino, sino_data, dimx, dimy,  dimz);

  double* radial_sino=new double[dimx];
  memset(radial_sino, 0, sizeof(double)*dimx);
 

 get_sino_radial_pro( sino_data, radial_sino);
  for(int i=0;i<dimx;i++)
  {
    cout<<radial_sino[i]<<endl;

  }


  if(sino_data!=NULL)
  {
    delete [] sino_data;
    sino_data=NULL;

  }

 if(radial_sino!=NULL)
  {
    delete [] radial_sino;
    radial_sino=NULL;

  }


gApplication->Terminate();


}

void read_sino_flat_ma(string path_sino, float* sino_data, int dimx=256, int dimy=192, int dimz=1399)
{
  int wordlength=sizeof(float);	
  //read original image to memory
  ifstream fin;
  fin.open(path_sino.c_str());

  cout<< "path image: "<< path_sino<<endl;
  if(fin.good()){
    cout<<"Reading emission file from "<<path_sino.c_str()<<endl;
    fin.read((char *)sino_data, (dimx* dimy* dimz* wordlength));
    fin.close();

  }
  else
  {
    cout<<"Error opening emission file "<<path_sino.c_str()<<endl;	
    exit (EXIT_FAILURE);
    fin.close();
  }




}

void get_sino_radial_pro(float* sino_data, double* radial_sino,int dimx=256, int dimy=192, int dimz=1399)
{
   
  for(int z=0;z<dimz;z++)
  {
    int temp_z_index=z*dimy*dimx;
    for (int y=0;y<dimy;y++)
    {
      int temp_y_index=y*dimx;
      for(int x=0;x<dimx;x++)
      {
        int i_sino=temp_z_index+temp_y_index+x;
       radial_sino[x] += sino_data[i_sino];
      }


    }
  }

}
