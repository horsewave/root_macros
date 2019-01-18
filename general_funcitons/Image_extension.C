/*
 *Discription: enlarge the image to the default image size for BrainPET(256*256*153)
Editor: Bo Ma
Time: 2018.08.22
 * */

void Image_extension()
{

  string input_image_small="./cubic_phantom_small.i33";
  string input_image_large="./cubic_phantom_large.i33";

  //string output_image="./cubic_phantom_att_large.i33";
  string output_image="./cubic_phantom_source_large.i33";

  int wordlength=sizeof(float);	


  int dimx_small=100;
  int dimy_small=100;
  int dimz_small=100;

  int dimx_large=256;
  int dimy_large=256;
  int dimz_large=153;


  int nVoxels_small = dimx_small*dimy_small*dimz_small;

  int nVoxels_large = dimx_large*dimy_large*dimz_large;

  float *image_small=new float[nVoxels_small];
  memset(image_small, 0, sizeof(float)*nVoxels_small);

  float *image_large=new float[nVoxels_large];
  memset(image_large, 0, sizeof(float)*nVoxels_large);

  //read original image to memory
  ifstream fin;
  fin.open(input_image_small.c_str());

  if(fin.good()){
    cout<<"Reading emission file from "<<input_image_small.c_str()<<endl;
    //of<<"Reading emission file from "<<input_image.c_str()<<endl;
    fin.read((char *)image_small, (dimx_small * dimy_small * dimz_small * wordlength));
    fin.close();
  }
  else
  {
    cout<<"Error opening emission file "<<input_image_small.c_str()<<endl;	
    fin.close();
  }


  fin.open(input_image_large.c_str());

  if(fin.good()){
    cout<<"Reading emission file from "<<input_image_large.c_str()<<endl;
    //of<<"Reading emission file from "<<input_image.c_str()<<endl;
    fin.read((char *)image_large, (dimx_large* dimy_large* dimz_large* wordlength));
    fin.close();
  }
  else
  {
    cout<<"Error opening emission file "<<input_image_large.c_str()<<endl;	
    fin.close();
  }





  //add the head data to the large data.
  for(int x = (dimx_large - dimx_small)/2; x < (dimx_large + dimx_small)/2; x++){
    for(int y = (dimy_large - dimy_small)/2; y < (dimy_large + dimy_small)/2; y++){
      for(int z = (dimz_large - dimz_small)/2; z < (dimz_large + dimz_small)/2; z++){
        if(image_large[z*dimx_large*dimy_large + y*dimx_large + x]==0)
        {
          image_large[z*dimx_large*dimy_large + y*dimx_large + x]
            = image_small[(z-(dimz_large-dimz_small)/2)*dimx_small*dimy_small 
            + (y-(dimy_large-dimy_small)/2)*dimx_small
            + x-(dimx_large-dimx_small)/2];	

        }

      }
    }
  }




  ofstream fout;
  fout.open(output_image.c_str());
  fout.write((char*)image_large, (dimx_large * dimy_large * dimz_large * wordlength));
  fout.close();

  delete [] image_large;
  image_large=NULL;
  delete [] image_small;
  image_small=NULL;


}
