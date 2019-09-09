
void main_set_value()
{
  //
  int dimx=256;
  int dimy=256;
  int dimz=153;

  string input_image="/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/sphere_phantom_simu/1_fov_305/att.i";

  string output_image="/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/sphere_phantom_simu/1_fov_305/att_fwd.i";

  int wordlength=sizeof(float);	
  int nVoxels = dimx*dimy*dimz;

  float *input_data=new float[nVoxels];
  memset(input_data, 0, wordlength*nVoxels);

  //read original image to memory
  ifstream fin;
  fin.open(input_image.c_str());

  if(fin.good()){
    cout<<"Reading emission file from "<<input_image.c_str()<<endl;
    //of<<"Reading emission file from "<<input_image.c_str()<<endl;
    fin.read((char *)input_data, (nVoxels* wordlength));
    fin.close();
  }
  else
  {
    cout<<"Error opening emission file "<<input_image.c_str()<<endl;	
    fin.close();
  }


  float air=0;
  float water=0.096;
  float pmma=0.08;
  float glass=0.12;

  for(int z = 0; z < dimz; z++){
    for(int y = 0; y < dimy; y++){
      for(int x = 0; x < dimx; x++){
        
        float pix_value = input_data[z*dimx*dimy + y*dimx + x];


        if(0.06>pix_value&&pix_value>0.04)
        {
          pix_value=pmma;
          cout<< "pmma :" << pix_value <<endl;
          
        }
        else if(pix_value>=0.08&&pix_value<=0.095)
        {
          //cout<< "water:" <<pix_value<<endl;
          pix_value=water;
        }
        
        input_data[z*dimx*dimy + y*dimx + x]=pix_value;	
      }
    }
  }
  //write imge to disk
  ofstream fout;
  fout.open(output_image.c_str());
  fout.write((char*) input_data, (dimx * dimy * dimz * wordlength));
  fout.close();

  //delete occupied stack
  delete [] input_data;
  input_data=NULL;

 
}
