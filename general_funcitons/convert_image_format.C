void convert_image_format()
{

  //
  int dimx=210;
  int dimy=210;
  int dimz=153;
  
  string input_image="/data/PET/mr_pet_temp/Ma/software/src/gate/gate_macros/Brain_phantom_ma/subject04_atten.i33";

  string output_image="/data/PET/mr_pet_temp/Ma/software/src/gate/gate_macros/Brain_phantom_ma/subject04_atten_int32.i33";

  int wordlength=sizeof(unsigned short);	
  int nVoxels = dimx*dimy*dimz;

  unsigned short *input_data=new unsigned short[nVoxels];
  memset(input_data, 0, sizeof(unsigned short)*nVoxels);

       //read original image to memory
  ifstream fin;
  fin.open(input_image.c_str());

  if(fin.good()){
      cout<<"Reading emission file from "<<input_image.c_str()<<endl;
      //of<<"Reading emission file from "<<input_image.c_str()<<endl;
      fin.read((char *)input_data, (dimx * dimy * dimz * wordlength));
      fin.close();
  }
  else
  {
    cout<<"Error opening emission file "<<input_image.c_str()<<endl;	
    fin.close();
  }
    

  wordlength=sizeof(int);	
  
  int *output_data=new int[nVoxels];
  memset(output_data, 0, sizeof(int)*nVoxels);


  int air=0;
  int water=1;
  int brain=3;
  int skull=4;

  for(int x = 0; x < dimx; x++){
    for(int y = 0; y < dimy; y++){
      for(int z = 0; z < dimz; z++){
        int value=input_data[z*dimx*dimy + y*dimx + x] = 0;
        
        int value_out=0;

        switch(value)
        {
          case 0:value_out=air;
                 break;
          case 1:value_out=water;
                 break;
          case 7:value_out=skull;
                 break;
          case 8:value_out=skull;
                 break;
          case 11:value_out=skull;
                  break;
          default:value_out=brain;
                  break;
        }
        //why?to reduce the time?
        output_data[z*dimx*dimy + y*dimx + x]=value_out;	
      }
    }
  }
  //write imge to disk
  ofstream fout;
  fout.open(output_image.c_str());
  fout.write((char*) output_data, (dimx * dimy * dimz * wordlength));
  fout.close();

  //delete occupied stack
  delete [] input_data;
  input_data=NULL;

  delete [] output_data;
  input_data=NULL;


}

