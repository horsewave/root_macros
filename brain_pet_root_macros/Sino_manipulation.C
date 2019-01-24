/***Sino_manipulation.C*
 ** Function: Operations with Sinograms,including random,normlization,scatter, attenuation corrections, sinogram scale(slice by slice, whole sinograms and by axial directions)
 * 1: void Sino_scale_slice_by_slice(string saved_sino_random_norm_cor_prompt, string path_sino_true_and_scatter_mcs,string path_sino_scatter_no_scale_mcs,string saved_sino_scatter_slice_scale_mcs)
 * 2: void Sino_scale_by_z_position(string saved_sino_random_norm_cor_prompt, string path_sino_true_and_scatter_mcs,string path_sino_scatter_no_scale_mcs,string saved_sino_scatter_z_scale_mcs)
 * 3: float_vec_t Get_axile_events_sino(string sino_data_path,bool is_float)
 * 4: string Float_to_string(float input_float,int precision_num)
 * 5: string IntToString (int a)
 * 6: void Cor_random( string path_sino_prompt,string path_sino_rand,string saved_sino_rand_cor)
 * 7: void Cor_norm( string path_sino_input, string path_sino_norm,string saved_sino_norm_cor)
 * 8: void Cor_att( string path_sino_input, string path_sino_att,string saved_sino_att_cor)
 * 9: void Cor_scatter( string path_sino_input, string path_sino_scatter,string saved_sino_scatter_cor)
 * 10: void Sino_scale_whole( string path_sino_input, string saved_sino_scaled,float scale_factor)
 * 11: void Sino_info()
 * 12: void Get_Sino_counts_per_plane(string sino_data_path,double* sino_plane_counts, bool is_short=false)
 * 13: void Get_Sino_counts_per_z(string sino_data_path,double* sino_z_counts,bool is_short=false)
 * 14: void array_divid(double* numerator, double* denominator, float* array_result, int array_size)
 * 15: void Sino_scale_slice_by_slice(string path_sino_input,string path_sino_saved,float* scale_factors,bool is_short=false)
 * 17: void Sino_get_z_position(string path_sino_input,bool is_short=false)
 * 19:  void Add_sino( string path_sino_input_1, string path_sino_input_2,string saved_sino)
 * 20: void create_sino_with_defined_value( string path_saved_sino, float bin_value)
 * 21: void set_norm_sino_gaps(string path_norm_sino_with_gap,string path_sino_temp, string path_saved_sino_without_gap,float gap_value)
 * 22: void sino_divid(string path_sino_numerator , string path_sino_denominator, string path_saved_sino)
 * 23: void set_sino_gaps(string path_sino_no_gap,string path_sino_with_gap, string path_saved_sino_without_gap,float gap_value=0)
 *
 * 24: void set_sino_value_except_gaps(string path_sino_with_gap, string path_saved_sino,float set_value=1)
 * 25: void get_gaps_sino(string path_sino_with_gap, string path_saved_sino,float set_value=1)
 * void remove_gap_sino_bad_lor(string path_sino_gap_ori,string path_saved_sino_without_gap,float sino_value=2)
 * void normlized_sino( string input_sino,string saved_sino)









 * Editor: Bo Ma;
 * 

 ** Date:2018.03.25.
 * ****/


#include <vector>

typedef std::vector<int> int_vec_t; 
typedef std::vector<std::vector<int>> int_vec_t_2d; 
typedef std::vector<float> float_vec_t; 

void Sino_manipulation()
{

main_1();

  //main_3() ;

  gApplication->Terminate();


}	



/*
 *  merge two sinos
 */
//void main_1( string sub_folder_1,string sub_folder_2,string sub_folder_merge)
void main_1( )
{

  //diff_scatter_methods()
  //

  string input_folder = "/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/sphere_phantom_simu/1_fov/scatterMCS/";

  string name_sino_0="true.fs";
  string name_sino_1="true.fs";
  string name_sino_merge="true.fs";

  string sub_folder_1 = "merged_with_detector_modual/" ;
  string sub_folder_2 = "run_5000_with_detector_modual_4/" ;
  string sub_folder_merge = "merged_with_detector_modual/" ;

  string path_input_sino_1 = input_folder + sub_folder_1+ name_sino_0; 
  string path_input_sino_2 = input_folder +  sub_folder_2+ name_sino_1; 
  string path_saved_sino = input_folder +  sub_folder_merge+ name_sino_merge; 
  Add_sino( path_input_sino_1, path_input_sino_2,path_saved_sino);


  name_sino_0="scatter.fs";
  name_sino_1="scatter.fs";
  name_sino_merge="scatter.fs";

  path_input_sino_1 = input_folder + sub_folder_1+ name_sino_0; 
  path_input_sino_2 = input_folder +  sub_folder_2+ name_sino_1; 
  path_saved_sino = input_folder +  sub_folder_merge+ name_sino_merge; 
  //cout<<"path: "<< path_input_sino_1<<endl;
  Add_sino( path_input_sino_1, path_input_sino_2,path_saved_sino);


}	



void main_3()
{
   string base_folder_input = "/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/sphere_phantom_simu/1_fov/scatterMCS/run_5000_with_detector_modual/";
   string base_folder_refer = "/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/sphere_phantom_simu/1_fov/scatterMCS/merged_with_detector_modual/";


  string path_sino_input_true= base_folder_input +"true.fs";
  string path_sino_input_scatter = base_folder_input +"scatter.fs";
  string path_sino_input_norm = "/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/cylinder_phantom_norm_file_mcs/norm_no_detection_modual/norm_sino_320_192_no_detection_modual.fs";
  string path_sino_input_scatter_normed = base_folder_input +"scatter_normed.fs";
  string path_sino_input_true_plus_scatter = base_folder_input +"true_plus_scatter.fs";
  string path_sino_input_true_plus_scatter_normed = base_folder_input +"true_plus_scatter_normed.fs";



  string path_sino_refer_true = base_folder_refer +"true.fs";
  string path_sino_refer_scatter = base_folder_refer +"scatter.fs";
  string path_sino_refer_norm ="/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/cylinder_phantom_norm_file_mcs/norm_with_detection_modual/norm_sino_320_192_with_detection_modual.fs";

  string path_sino_refer_scatter_normed = base_folder_refer +"scatter_normed.fs";
  string path_sino_refer_true_plus_scatter= base_folder_refer +"true_plus_scatter.fs";
  string path_sino_refer_true_plus_scatter_normed= base_folder_refer +"true_plus_scatter_normed.fs";

  string path_sino_scaled_scatter = base_folder_input +"scatter_scaled.fs";
  string path_sino_scaled_scatter_normed = base_folder_input +"scatter_scaled_normed.fs";


  cout << " path_sino_input_true : "<< path_sino_input_true <<endl;
  cout << " path_sino_input_scatter: "<< path_sino_input_scatter <<endl;
  cout << " path_sino_input_true_plus_scatter: "<< path_sino_input_true_plus_scatter <<endl;
  cout << " path_sino_input_true_plus_scatter_normed: "<< path_sino_input_true_plus_scatter_normed <<endl;
  
  cout << " path_sino_refer_true: "<< path_sino_refer_true <<endl;
  cout << " path_sino_refer_scatter: "<< path_sino_refer_scatter <<endl;
  cout << " path_sino_refer_true_plus_scatter: "<< path_sino_refer_true_plus_scatter <<endl;
  cout << " path_sino_refer_true_plus_scatter_normed: "<< path_sino_refer_true_plus_scatter_normed <<endl;
  cout << " path_sino_scaled_scatter: "<<  path_sino_scaled_scatter<<endl;


//Add_sino( path_sino_input_scatter, path_sino_input_true,path_sino_input_true_plus_scatter);
//Add_sino( path_sino_refer_scatter, path_sino_refer_true,path_sino_refer_true_plus_scatter);



//Cor_norm( path_sino_input_true_plus_scatter, path_sino_input_norm,path_sino_input_true_plus_scatter_normed);
//Cor_norm( path_sino_refer_true_plus_scatter, path_sino_refer_norm,path_sino_refer_true_plus_scatter_normed);

//Cor_norm( path_sino_input_scatter, path_sino_input_norm,path_sino_input_scatter_normed);
//Cor_norm( path_sino_refer_scatter, path_sino_refer_norm,path_sino_refer_scatter_normed);



  double total_events_input = get_total_events_simu_data( path_sino_input_true_plus_scatter_normed);
  double total_events_refer = get_total_events_simu_data( path_sino_refer_true_plus_scatter_normed);

  cout<<"total_events_input is: "<<  total_events_input<<endl;
  cout<<"total_events_refer is: "<<  total_events_refer<<endl;


  //// 4.3 calculate the scaling factor

  float  scaling_factor=total_events_refer/total_events_input;

  cout <<" scaling factor is:  " <<scaling_factor <<endl;
  // 4.4 save the scaled scatter sino

  Sino_scale_whole( path_sino_input_scatter_normed, path_sino_scaled_scatter_normed,scaling_factor);

  cout<<"------------4: finish scaling the simu scatter--------------"<<endl;

  cout<<"------------all finished! You can reconstruct now!--------------"<<endl;


}






void Sino_scale_slice_by_slice(string saved_sino_random_norm_cor_prompt, string path_sino_true_and_scatter_mcs,string path_sino_scatter_no_scale_mcs,string saved_sino_scatter_slice_scale_mcs)
{

  int plane_num=1399;
  int z_num=153;
  double* sino_plane_counts_nume=new double[plane_num];
  double* sino_plane_counts_denume=new double[plane_num];

  memset(sino_plane_counts_nume,0,sizeof(double)*plane_num);
  memset(sino_plane_counts_denume,0,sizeof(double)*plane_num);

  float* scale_factors_plane=new float[plane_num];
  //std::fill_n(scale_factors, plane_num, 0.1);
  memset(scale_factors_plane,0,sizeof(float)*plane_num);


  Get_Sino_counts_per_plane(saved_sino_random_norm_cor_prompt, sino_plane_counts_nume);
  Get_Sino_counts_per_plane(path_sino_true_and_scatter_mcs, sino_plane_counts_denume);
  array_divid(sino_plane_counts_nume, sino_plane_counts_denume, scale_factors_plane, plane_num);

  for(int i=0;i<plane_num;i++)
  {
    cout<<"  "<<i<<"  : "<<scale_factors_plane[i]<<endl;
  }

  Sino_scale_slice_by_slice( path_sino_scatter_no_scale_mcs,saved_sino_scatter_slice_scale_mcs, scale_factors_plane);




  if(scale_factors_plane!=NULL)
  {
    delete [] scale_factors_plane ;
    scale_factors_plane=NULL;
  }


  if(sino_plane_counts_nume!=NULL)
  {
    delete [] sino_plane_counts_nume ;
    sino_plane_counts_nume=NULL;
  }


  if(sino_plane_counts_denume!=NULL)
  {
    delete [] sino_plane_counts_denume ;
    sino_plane_counts_denume=NULL;
  }



}




/*Function: Get the axial distribution of a sinogram;
 *Parameters: :the path of the sinogram;
 *            :the data format of the sinogram: only the prompt sino is not float;
 *Return value: a vector of the axial distribtuion of the sinogram;
 * */
float_vec_t Get_axile_events_sino(string sino_data_path,bool is_float)
{


  cout<< sino_data_path<<endl;


  //plane numbers of the sino data
  int plane_num;

  //event counts of every plane;
  float* array_events_per_plane;

  //event counts of every Z position.
  float* array_events_per_z;
  int* array_planes_per_z;

  //the corresponding axial postion of every plane.
  int* array_axial_position_per_plane;
  //---sinogram object sructure
  int max_z=0;


  //class including all the basic information of the MRPET.
  MRPET_dump *dump = new MRPET_dump();	


  ///---measurement data, correction of random and norm//////////////////////////////////////////////////
  BrainPET_Sinograms* whole_sinogram=new BrainPET_Sinograms(dump,"whole_sinogram"); 

  cout<<"start to read  sino data from: "<<sino_data_path<<endl;

  whole_sinogram->read_flatSinogram(sino_data_path, is_float);
  cout<<"end of reading  sino data from: "<<sino_data_path<<endl;

  //Get plane number of a sino.
  plane_num=whole_sinogram->get_n_planes();

  array_events_per_plane=new float[plane_num];

  memset(array_events_per_plane,0,sizeof(float)*plane_num);

  array_axial_position_per_plane=new int[plane_num];
  memset(array_axial_position_per_plane,0,sizeof(int)*plane_num);

  for(int j=0;j<plane_num;j++)
  {
    //single sino plane
    BrainPET_SinogramData* single_sinogram=whole_sinogram->get_plane(j);

    //event counts of a sino plane.
    array_events_per_plane[j]=single_sinogram->get_n_Events();

    //get the corresponding axial position of this sino plane.
    array_axial_position_per_plane[j]=single_sinogram->getAxialPosition();

    //get the max axile value.It is integer
    if(max_z<array_axial_position_per_plane[j])
    {
      max_z=array_axial_position_per_plane[j];
    }

  }

  max_z+=1;//number of the z
  cout<<"the maximum z is: "<<max_z<<endl;

  array_events_per_z= new float[max_z];

  memset(array_events_per_z,0,sizeof(float)*max_z);
  //memset(array_events_per_z_sss,0,sizeof(float)*max_z);


  array_planes_per_z= new int[max_z];
  memset(array_planes_per_z,0,sizeof(int)*max_z);

  //get the summery counts for the corresponding axial position.
  for(int j=0;j<plane_num;j++)
  {
    int z=array_axial_position_per_plane[j];
    array_events_per_z[z]+=array_events_per_plane[j];
    array_planes_per_z[z]++;

  }

  cout<<"***************************************************"<<endl;
  cout<<"The plane number is: "<<plane_num<<endl;
  cout<<"The  axile value is: "<<max_z<<endl;
  cout<<"***************************************************"<<endl;

  cout<<"***************************************************"<<endl;
  cout<<"plane number per z"<<endl;
  cout<<"***************************************************"<<endl;

  int z_max_planes=0;
  for(int j=0;j<max_z;j++)
  {
    int plane_number= array_planes_per_z[j];
    if(z_max_planes<plane_number)
    {
      z_max_planes=plane_number;

    }
    cout<<"plane numbers for each z:[ "<<j<<" ]= "<< array_planes_per_z[j]<<endl;
    cout<<"max plane numbers : "<<z_max_planes<<endl;
  }

  //scaling
  for(int j=0;j<max_z;j++)
  {
    int plane_number= array_planes_per_z[j];
    float norm_factor=(float)z_max_planes/plane_number;
    array_events_per_z[j]=norm_factor*array_events_per_z[j];

  }

  //copy the data to a vector to return
  float_vec_t vector_events_per_z(max_z);
  for(int j=0;j<max_z;j++)
  {
    vector_events_per_z[j]=array_events_per_z[j];

  }


  if(array_events_per_plane!=NULL)
  {
    delete [] array_events_per_plane;
    array_events_per_plane=NULL;

  }
  if(array_events_per_z!=NULL)
  {
    delete [] array_events_per_z;
    array_events_per_z=NULL;

  }
  if(array_planes_per_z!=NULL)
  {
    delete [] array_planes_per_z;
    array_planes_per_z=NULL;

  }

  if(array_axial_position_per_plane!=NULL)
  {
    delete [] array_axial_position_per_plane;
    array_axial_position_per_plane=NULL;

  }

  if(dump!=NULL)
  {
    delete dump;
    dump=NULL;
  }

  if(whole_sinogram!=NULL)
  {
    delete whole_sinogram;
    whole_sinogram=NULL;
  }


  return vector_events_per_z;


}


string Float_to_string(float input_float,int precision_num)
{
  stringstream s_stream;
  s_stream << fixed << setprecision(precision_num) << input_float;
  string s = s_stream.str();

  return s;
}

string IntToString (int a)
{
  ostringstream temp;
  temp<<a;
  return temp.str();
}


void Cor_random( string path_sino_prompt,string path_sino_rand,string saved_sino_rand_cor)
{ 

  //class including all the basic information of the MRPET.
  MRPET_dump *dump = new MRPET_dump();	

  ///---measurement data, correction of random and norm//////////////////////////////////////////////////
  BrainPET_Sinograms* sino_prompt=new BrainPET_Sinograms(dump,"prompt");  
  BrainPET_Sinograms* sino_random=new BrainPET_Sinograms(dump,"random");

  //bool is_int=true;
  bool is_int=false;
  sino_prompt->read_flatSinogram(path_sino_prompt,is_int);

  is_int=false;
  sino_random->read_flatSinogram(path_sino_rand,is_int);

  //...random correction
  sino_prompt->subtract_sinogram(sino_random);
  sino_prompt->write_flat_sinograms(saved_sino_rand_cor,is_int);

  cout<<"random correction finished!"<<endl;

  if(sino_prompt!=NULL)
  {
    delete sino_prompt;
    sino_prompt=NULL;

  }
  if(sino_random!=NULL)
  {
    delete sino_random;
    sino_random=NULL;

  }

  if(dump!=NULL)
  {
    delete dump;
    dump=NULL;

  }

}




void Cor_norm( string path_sino_input, string path_sino_norm,string saved_sino_norm_cor)
{ 

  //class including all the basic information of the MRPET.
  MRPET_dump *dump = new MRPET_dump();	


  ///---measurement data, correction of random and norm//////////////////////////////////////////////////
  BrainPET_Sinograms* sino_input=new BrainPET_Sinograms(dump,"sino_input");  
  BrainPET_Sinograms* sino_norm=new BrainPET_Sinograms(dump,"norm");

  bool is_int=false;
  sino_input->read_flatSinogram(path_sino_input,is_int);
  sino_norm->read_flatSinogram(path_sino_norm,is_int);


  //...norm correction  
  sino_input->multiply_by_sinogram(sino_norm);
  cout<<"normalization correction finished!"<<endl;
  sino_input->write_flat_sinograms(saved_sino_norm_cor,is_int);


  if(sino_input!=NULL)
  {
    delete sino_input;
    sino_input=NULL;

  }

  if(sino_norm!=NULL)
  {
    delete sino_norm;
    sino_norm=NULL;

  }
  if(dump!=NULL)
  {
    delete dump;
    dump=NULL;

  }

}


void Cor_att( string path_sino_input, string path_sino_att,string saved_sino_att_cor)
{ 

  //class including all the basic information of the MRPET.
  MRPET_dump *dump = new MRPET_dump();	

  ///---measurement data, correction of random and att//////////////////////////////////////////////////
  BrainPET_Sinograms* sino_input=new BrainPET_Sinograms(dump,"sino_input");  
  BrainPET_Sinograms* sino_att=new BrainPET_Sinograms(dump,"att");

  bool is_int=false;
  sino_input->read_flatSinogram(path_sino_input,is_int);
  sino_att->read_flatSinogram(path_sino_att,is_int);


  //...att correction  
  sino_input->multiply_by_sinogram(sino_att);
  cout<<"attenuation correction finished!"<<endl;
  sino_input->write_flat_sinograms(saved_sino_att_cor,is_int);


  if(sino_input!=NULL)
  {
    delete sino_input;
    sino_input=NULL;

  }

  if(sino_att!=NULL)
  {
    delete sino_att;
    sino_att=NULL;

  }
  if(dump!=NULL)
  {
    delete dump;
    dump=NULL;

  }

}



void Cor_scatter( string path_sino_input, string path_sino_scatter,string saved_sino_scatter_cor)
{ 

  //class including all the basic information of the MRPET.
  MRPET_dump *dump = new MRPET_dump();	

  ///---measurement data, correction of random and scatter//////////////////////////////////////////////////
  BrainPET_Sinograms* sino_input=new BrainPET_Sinograms(dump,"sino_input");  
  BrainPET_Sinograms* sino_scatter=new BrainPET_Sinograms(dump,"scatter");

  bool is_int=false;
  sino_input->read_flatSinogram(path_sino_input,is_int);
  sino_scatter->read_flatSinogram(path_sino_scatter,is_int);

  //...scatter correction  
  sino_input->subtract_sinogram(sino_scatter);

  cout<<"random correction finished!"<<endl;

  sino_input->write_flat_sinograms(saved_sino_scatter_cor,is_int);


  if(sino_input!=NULL)
  {
    delete sino_input;
    sino_input=NULL;

  }

  if(sino_scatter!=NULL)
  {
    delete sino_scatter;
    sino_scatter=NULL;

  }
  if(dump!=NULL)
  {
    delete dump;
    dump=NULL;

  }

}


void Sino_scale_whole( string path_sino_input, string saved_sino_scaled,float scale_factor)
{ 

  //class including all the basic information of the MRPET.
  MRPET_dump *dump = new MRPET_dump();	

  ///---measurement data, correction of random and scatter//////////////////////////////////////////////////
  BrainPET_Sinograms* sino_input=new BrainPET_Sinograms(dump,"sino_input");  

  bool is_int=false;
  sino_input->read_flatSinogram(path_sino_input,is_int);

  //---scale scatter sinogram
  sino_input->scale_by_value(scale_factor); 
  cout<<"scatterenuation correction finished!"<<endl;
  sino_input->write_flat_sinograms(saved_sino_scaled,is_int);


  if(sino_input!=NULL)
  {
    delete sino_input;
    sino_input=NULL;

  }
  if(dump!=NULL)
  {
    delete dump;
    dump=NULL;

  }

}


//void Sino_info(string path_sino_input)
void Sino_info()
{ 

  //class including all the basic information of the MRPET.
  MRPET_dump *dump = new MRPET_dump();	

  ///---measurement data, correction of random and scatter//////////////////////////////////////////////////
  BrainPET_Sinograms* sino_input=new BrainPET_Sinograms(dump,"sino_input");  

  //sino_input->read_flatSinogram(path_sino_input,is_int);

  int n_planes=sino_input->get_n_planes(); 
  int n_views=sino_input->get_n_views(); 
  int n_projs=sino_input->get_n_projs(); 

  cout<<"Sino plane number is: "<<n_planes<<endl;
  cout<<"Sino view number is: "<<n_views<<endl;
  cout<<"Sino projection number is: "<<n_projs<<endl;



  if(sino_input!=NULL)
  {
    delete sino_input;
    sino_input=NULL;

  }
  if(dump!=NULL)
  {
    delete dump;
    dump=NULL;

  }

}

/**
 *sino_plane_counts: array to store the event counts per plane in the sino(size:1399)
 *is_short: flag of the sino data
 *
 *
 **/
void Get_Sino_counts_per_plane(string sino_data_path,double* sino_plane_counts, bool is_short=false)
{
  cout<< sino_data_path<<endl;


  //class including all the basic information of the MRPET.
  MRPET_dump *dump = new MRPET_dump();	

  ///---measurement data, correction of random and norm//////////////////////////////////////////////////
  BrainPET_Sinograms* whole_sinogram=new BrainPET_Sinograms(dump,"whole_sinogram"); 

  whole_sinogram->read_flatSinogram(sino_data_path, is_short);
  cout<<"end of reading  sino data from: "<<sino_data_path<<endl;

  //Get plane number of a sino.
  int plane_num=whole_sinogram->get_n_planes();

  for(int j=0;j<plane_num;j++)
  {
    //single sino plane
    BrainPET_SinogramData* single_sinogram=whole_sinogram->get_plane(j);

    //event counts of a sino plane.
    sino_plane_counts[j]=single_sinogram->get_n_Events();
  }

  if(dump!=NULL)
  {
    delete dump;
    dump=NULL;
  }

  if(whole_sinogram!=NULL)
  {
    delete whole_sinogram;
    whole_sinogram=NULL;
  }

  return ;

}




/**
 *sino_z_counts: array to store the event counts per plane in the z direction sino(size:153)
 *
 *
 **/
void Get_Sino_counts_per_z(string sino_data_path,double* sino_z_counts,bool is_short=false)
{
  cout<< sino_data_path<<endl;


  //plane numbers of the sino data
  int plane_num;

  //event counts of every plane;
  double* array_events_per_plane;

  //event counts of every Z position.
  double* array_events_per_z;
  int* array_planes_per_z;

  //the corresponding axial postion of every plane.
  int* array_axial_position_per_plane;
  //---sinogram object sructure
  int max_z=0;


  //class including all the basic information of the MRPET.
  MRPET_dump *dump = new MRPET_dump();	


  ///---measurement data, correction of random and norm//////////////////////////////////////////////////
  BrainPET_Sinograms* whole_sinogram=new BrainPET_Sinograms(dump,"whole_sinogram"); 

  cout<<"start to read  sino data from: "<<sino_data_path<<endl;

  whole_sinogram->read_flatSinogram(sino_data_path, is_short);
  cout<<"end of reading  sino data from: "<<sino_data_path<<endl;

  //Get plane number of a sino.
  plane_num=whole_sinogram->get_n_planes();

  array_events_per_plane=new double[plane_num];

  memset(array_events_per_plane,0,sizeof(double)*plane_num);

  array_axial_position_per_plane=new int[plane_num];
  memset(array_axial_position_per_plane,0,sizeof(int)*plane_num);

  for(int j=0;j<plane_num;j++)
  {
    //single sino plane
    BrainPET_SinogramData* single_sinogram=whole_sinogram->get_plane(j);

    //event counts of a sino plane.
    array_events_per_plane[j]=single_sinogram->get_n_Events();

    //get the corresponding axial position of this sino plane.
    array_axial_position_per_plane[j]=single_sinogram->getAxialPosition();

    //get the max axile value.It is integer
    if(max_z<array_axial_position_per_plane[j])
    {
      max_z=array_axial_position_per_plane[j];
    }

  }

  max_z+=1;//number of the z
  cout<<"the maximum z is: "<<max_z<<endl;

  array_events_per_z= new double[max_z];

  memset(array_events_per_z,0,sizeof(double)*max_z);
  //memset(array_events_per_z_sss,0,sizeof(float)*max_z);


  array_planes_per_z= new int[max_z];
  memset(array_planes_per_z,0,sizeof(int)*max_z);

  //get the summery counts for the corresponding axial position.
  for(int j=0;j<plane_num;j++)
  {
    int z=array_axial_position_per_plane[j];
    array_events_per_z[z]+=array_events_per_plane[j];
    array_planes_per_z[z]++;

  }

  cout<<"***************************************************"<<endl;
  cout<<"The plane number is: "<<plane_num<<endl;
  cout<<"The  axile value is: "<<max_z<<endl;
  cout<<"***************************************************"<<endl;

  cout<<"***************************************************"<<endl;
  cout<<"plane number per z"<<endl;
  cout<<"***************************************************"<<endl;


  //copy
  for(int j=0;j<max_z;j++)
  {
    sino_z_counts[j]=array_events_per_z[j];
  }


  if(array_events_per_plane!=NULL)
  {
    delete [] array_events_per_plane;
    array_events_per_plane=NULL;

  }
  if(array_events_per_z!=NULL)
  {
    delete [] array_events_per_z;
    array_events_per_z=NULL;

  }
  if(array_planes_per_z!=NULL)
  {
    delete [] array_planes_per_z;
    array_planes_per_z=NULL;

  }

  if(array_axial_position_per_plane!=NULL)
  {
    delete [] array_axial_position_per_plane;
    array_axial_position_per_plane=NULL;

  }

  if(dump!=NULL)
  {
    delete dump;
    dump=NULL;
  }

  if(whole_sinogram!=NULL)
  {
    delete whole_sinogram;
    whole_sinogram=NULL;
  }


  return ;


}



void array_divid(double* numerator, double* denominator, float* array_result, int array_size)
{
  for(int i=0;i<array_size;i++)
  {
    if(denominator[i]==0)
    {
      cout<<"error! the denominator is zero! Break!"<<endl;
      return;
    }
    array_result[i]=numerator[i]/denominator[i];
  }
}



float Get_Sino_scale_factor_whole(string path_sino_measured, string path_sino_simulated)
{

}

void Sino_scale_slice_by_slice(string path_sino_input,string path_sino_saved,float* scale_factors,bool is_short=false)
{
  cout<< path_sino_input<<endl;
  cout<< path_sino_saved<<endl;


  //plane numbers of the sino data
  int plane_num=1399;
  int view_num=256;
  int projection_num=192;

  //event counts of every plane;


  //class including all the basic information of the MRPET.
  MRPET_dump *dump = new MRPET_dump();	


  ///---measurement data, correction of random and norm//////////////////////////////////////////////////
  BrainPET_Sinograms* whole_sinogram=new BrainPET_Sinograms(dump,"whole_sinogram"); 

  cout<<"start to read  sino data from: "<<path_sino_input<<endl;
  bool is_short=false;
  whole_sinogram->read_flatSinogram(path_sino_input, is_short);
  cout<<"end of reading  sino data from: "<<path_sino_input<<endl;

  //Get plane number of a sino.
  plane_num=whole_sinogram->get_n_planes();

  for(int i_plane=0;i_plane<plane_num;i_plane++)
  {
    //single sino plane
    whole_sinogram->get_plane(i_plane)->scale_by_value(scale_factors[i_plane]);
    //whole_sinogram->get_plane(i_plane)->scale_by_value(0.1);

  }
  ;
  whole_sinogram->write_flat_sinograms(path_sino_saved,is_short);




  if(dump!=NULL)
  {
    delete dump;
    dump=NULL;
  }

  if(whole_sinogram!=NULL)
  {
    delete whole_sinogram;
    whole_sinogram=NULL;
  }


  return ;



}

//scale the sino by Z position. For all the single sinograms which belong to the same axial position, they share the same scaling factor.
//scale_factors: array which has 153 elements.
void Sino_scale_by_z_position(string path_sino_input,string path_sino_saved,float* scale_factors,bool is_short=false)
{
  cout<< path_sino_input<<endl;
  cout<< path_sino_saved<<endl;


  //plane numbers of the sino data
  int plane_num=1399;
  int view_num=256;
  int projection_num=192;

  //event counts of every plane;


  //class including all the basic information of the MRPET.
  MRPET_dump *dump = new MRPET_dump();	


  ///---measurement data, correction of random and norm//////////////////////////////////////////////////
  BrainPET_Sinograms* whole_sinogram=new BrainPET_Sinograms(dump,"whole_sinogram"); 

  cout<<"start to read  sino data from: "<<path_sino_input<<endl;
  bool is_short=false;
  whole_sinogram->read_flatSinogram(path_sino_input, is_short);
  cout<<"end of reading  sino data from: "<<path_sino_input<<endl;

  //Get plane number of a sino.
  plane_num=whole_sinogram->get_n_planes();

  for(int i_plane=0;i_plane<plane_num;i_plane++)
  {

    z_position=whole_sinogram->get_plane(i_plane)->getAxialPosition();
    //cout<<"  "<<i_plane<<" :  "<<z_position<<endl;

    //single sino plane
    whole_sinogram->get_plane(i_plane)->scale_by_value(scale_factors[z_position]);
    //whole_sinogram->get_plane(i_plane)->scale_by_value(0.1);

  }
  whole_sinogram->write_flat_sinograms(path_sino_saved,is_short);




  if(dump!=NULL)
  {
    delete dump;
    dump=NULL;
  }

  if(whole_sinogram!=NULL)
  {
    delete whole_sinogram;
    whole_sinogram=NULL;
  }


  return ;



}

void Sino_get_z_position(string path_sino_input,bool is_short=false)
{
  cout<< path_sino_input<<endl;


  //plane numbers of the sino data
  int plane_num=1399;

  //event counts of every plane;


  //class including all the basic information of the MRPET.
  MRPET_dump *dump = new MRPET_dump();	


  ///---measurement data, correction of random and norm//////////////////////////////////////////////////
  BrainPET_Sinograms* whole_sinogram=new BrainPET_Sinograms(dump,"whole_sinogram"); 

  cout<<"start to read  sino data from: "<<path_sino_input<<endl;
  bool is_short=false;
  whole_sinogram->read_flatSinogram(path_sino_input, is_short);
  cout<<"end of reading  sino data from: "<<path_sino_input<<endl;

  //Get plane number of a sino.
  plane_num=whole_sinogram->get_n_planes();

  int max_z=153;

  int* array_planes_per_z= new int[max_z];
  memset(array_planes_per_z,0,sizeof(int)*max_z);

  int_vec_t_2d z_plane_maxtrix(max_z);


  for(int i_plane=0;i_plane<plane_num;i_plane++)
  {

    z_position=whole_sinogram->get_plane(i_plane)->getAxialPosition();
    cout<<"  "<<i_plane<<" :  "<<z_position<<endl;

    z_plane_maxtrix[z_position].push_back(i_plane);

  }

  for(int i=0;i<max_z;i++)
  {
    int vec_size= z_plane_maxtrix[i].size();
    cout<<"  "<<i<<"  :  ";
    for(int j=0;j<vec_size;j++)
    {
      cout<<z_plane_maxtrix[i].at(j)<<"  ";;
    }

    cout<<endl;


  }

  for(int i=0;i<max_z;i++)
  {
    int vec_size= z_plane_maxtrix[i].size();


    cout<<"  "<<i<<"  :  "<<vec_size<<endl;;




  }


  if(dump!=NULL)
  {
    delete dump;
    dump=NULL;
  }

  if(whole_sinogram!=NULL)
  {
    delete whole_sinogram;
    whole_sinogram=NULL;
  }

  if(array_planes_per_z!=NULL)
  {
    delete [] array_planes_per_z;
    array_planes_per_z=NULL;
  }


  return ;



}








//---convert z and segment to planenumber
int seg_z_to_plane_number(int seg, int z)
{

}
//---get segment info for plane number using LUTs
void get_seg_info(int planeNumber, int &segment, int &z_value, 
    float &min_angle, float &max_angle, float &mean_angle)
{

}




//--- initialise segments lookup tables:
void Sino_3D_seginfo(int nViews = 192, int nProjs = 256,
    int maxRingdiff_x = 67, int span_x = 9,
    float crystalRadius = 18.796,
    int verbose_x = 0)
{

  int nviews=; 
  int nprojs=;
  int maxRingdiff=;
  int span=;
  int nplanes=;

  //---modul pairs:
  int n_mpairs;
  int n_mods;
  int nheads;
  int ncrystals; 
  int ncrystals_per_ring;
  int mod_nx;
  int mod_ny;
  int mod_fov=19;
  int mod_nrings;
  float crystal_radius;
  double plane_sep;

  //---segment information
  int maxseg=; // 
  int nsegs=; // number of segments (always odd)
  int nrings=77; // number of detector rings
  float d_tan_theta; // tilt angle of segments
  int *segz0;
  int *segnz;
  int *segzoff;
  int *segnums;

  //---bad head combination
  int bad_hp;

  //---contains cristal positions:
  point3D_typ *crystalPositions;

  //---binsize in mm
  float binsize=0.125;

  //---lookuptables
  int *segments_lut;
  int *z_val_lut;
  float *min_angle_lut;
  float *mean_angle_lut;
  float *max_angle_lut;




  int np, sp2, segnzs, segnum; //##z0,
  int i; //## j, rsq, ix, iy; 

  if(span == 0){
    cout << "ERROR: span = 0!" << endl;
    exit(0);
  }

  maxseg=maxRingdiff/span;
  nsegs=2*maxseg+1;
  np=2*nrings-1; // number of planes (2n+1 with interleaving)
  sp2=(span+1)/2; // min ring difference (5 for span=9)

  segz0  = new int[nsegs]; // minRD for segment[i]
  segnz  = new int[nsegs]; // number of elements in segment[i]
  segzoff= new int[nsegs]; // 
  segnums= new int[nsegs]; // segment index
  segnzs=0;
  nplanes=0;

  // for every segment
  for (i=0; i < nsegs; i++){
    segnum=(1-2*(i%2))*(i+1)/2;
    segnums[i]=segnum;
    if (i==0) 
      segz0[0]=0;
    else 
      segz0[i]=sp2+span*((i-1)/2);
    segnz[i]=np-2*segz0[i];
    segnzs+=segnz[i];
    if (i==0) 
      segzoff[0]=0;
    else 
      segzoff[i] = segzoff[i-1] + segnz[i-1];
    nplanes += segnz[i];
  }
  if(verbose>0){
    cout << "nPlanes: " << nplanes << endl;
  }
  // tilt angle of segments:
  d_tan_theta = span*plane_sep/crystal_radius;
  double tan0=0.0;



  if(verbose>0){
    cout << "-----------------------------------------------------------" << endl;
    cout << "MRPET_dump::init_seginfo:  Info: " << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "maxseg=" << maxseg << endl;
    cout << "nsegs =" << nsegs << endl;
    cout << "d_tan_theta=" << d_tan_theta  << endl;
    //##int nsino=0;
    for (i=0; i< nsegs; i++)
    {
      printf("[i]=%2d  segnums=%3d   segnz=%3d   segz0=%3d  segzoff=%3d \n",
          i,segnums[i], segnz[i],segz0[i],segzoff[i]);  
    }

    cout  << "Nsinos=" << nplanes << endl;
    cout << "-----------------------------------------------------------" << endl;


    cout << "Segment Angles:  "<< endl;
    cout << "span=" << span << endl;
    cout << "plane_sep=" << plane_sep << endl;
    cout << "crystal_radius=" << crystal_radius << endl;
    cout << "d_tan_theta = span*plane_sep / crystal_radius = " << d_tan_theta << endl;
  }
  double RAD2DEG=180.0/M_PI;
  for (i=0; i<=maxseg; i++)
  {
    if(verbose>0){
      printf("[%2d]:  %6.3f degree \n",i, RAD2DEG*atan(tan0));
    }
    tan0+=d_tan_theta;
  }


  double TAN_THETA=0.0;
  int segment0 = (int)floor(0.0/d_tan_theta+0.5); 
  if(verbose>0){
    cout << "-----------------------------------------------------------" << endl;
    cout << "segment0=" << segment0 << endl;
  }
  int is=0;
  double seg_angle_bound[10];
  seg_angle_bound[0]=0.0;
  for (i=0; (is<maxseg+2); i++)
  {
    int segment1 = (int)floor(TAN_THETA/d_tan_theta+0.5); 
    if (segment1>segment0)
    {
      is++;
      seg_angle_bound[is]=atan(TAN_THETA)*RAD2DEG;
      if(verbose>0){
        cout << is << "#: segment angle bound=" << seg_angle_bound[is] << endl;
      }
      segment0=segment1;
    }
    TAN_THETA+=0.025;
  }
  if(verbose>0){
    cout << "-----------------------------------------------------------" << endl;
    cout << "SegmentAngles: " << endl;
    for (i=1; i<=maxseg; i++)
    {
      printf("Seg %d    Angle1= %6.3f deg    Angle2= %6.3f deg      AngleMEAN= %6.3f deg\n",
          i,seg_angle_bound[i], seg_angle_bound[i+1], 
          0.5*(seg_angle_bound[i]+seg_angle_bound[i+1]));
    }
    cout << "-----------------------------------------------------------" << endl;
  }

  //generate segment LUTs:
  //--------------------------------------------------------------------------
  segments_lut   =new int[nplanes];
  z_val_lut   =new int[nplanes];
  min_angle_lut  =new float[nplanes];
  mean_angle_lut =new float[nplanes];
  max_angle_lut  =new float[nplanes];


  int plane;  
  for(i = 0; i < nsegs-1; i ++){
    for(plane = segzoff[i]; plane < segzoff[i+1]; plane++){
      //segment:
      segments_lut[plane] = segnums[i];
      //angles:
      if(segnums[i]==0){
        min_angle_lut[plane]  = 0;
        mean_angle_lut[plane] = 0;
        max_angle_lut[plane]  = 0;
      }else{
        if(i%2==0){
          min_angle_lut[plane]  = seg_angle_bound[(int)(i/2)];
          mean_angle_lut[plane] = 0.5*(seg_angle_bound[(int)(i/2)]+seg_angle_bound[(int)(i/2)+1]);
          max_angle_lut[plane]  = seg_angle_bound[(int)(i/2)+1];
        }else{
          min_angle_lut[plane]  = seg_angle_bound[(int)(i/2)+1];
          mean_angle_lut[plane] = 0.5*(seg_angle_bound[(int)(i/2)+1]+seg_angle_bound[(int)(i/2)+2]);
          max_angle_lut[plane]  = seg_angle_bound[(int)(i/2)+2];
        }
      }
      z_val_lut[plane] = (plane - segzoff[i] + segz0[i]);  
    }
  }    


  for(int plane = segzoff[i]; plane < nplanes; plane++){
    //segment:
    segments_lut[plane] = segnums[i];
    //angles:
    if(segnums[i]==0){
      min_angle_lut[plane]  = 0;
      mean_angle_lut[plane] = 0;
      max_angle_lut[plane]  = 0;
    }else{
      if(i%2==0){
        min_angle_lut[plane]  = seg_angle_bound[(int)(i/2)];
        mean_angle_lut[plane] = 0.5*(seg_angle_bound[(int)(i/2)]+seg_angle_bound[(int)(i/2)+1]);
        max_angle_lut[plane]  = seg_angle_bound[(int)(i/2)+1];
      }else{
        min_angle_lut[plane]  = seg_angle_bound[(int)(i/2)+1];
        mean_angle_lut[plane] = 0.5*(seg_angle_bound[(int)(i/2)+1]+seg_angle_bound[(int)(i/2)+2]);
        max_angle_lut[plane]  = seg_angle_bound[(int)(i/2)+2];
      }
    }
    z_val_lut[plane] = (plane - segzoff[i] + segz0[i]);  
  }
  //--------------------------------------------------------------------------


  //--------------------------------------------------------------------------
  //---create instanve of SinoSegmentStructure which collects 
  //---all axial compression properties
  SinoSegmentStructure* s=new SinoSegmentStructure;
  s->nviews=nviews;
  s->nradial=nprojs;
  s->nrings=nrings;
  s->nplanes_segment_0=(nrings*2)-1;
  s->ispan=span;
  s->imax_ring_diff=maxRingdiff;
  s->plane_separation=plane_sep/100.0;  //[from cm to m]
  s->nsinos=nplanes;

  s->set_number_of_segments(nsegs);
  for (i=1; i<nsegs; i++)
  {
    int is=abs(segnums[i]);

    double mean=0.5*(seg_angle_bound[is]+seg_angle_bound[is+1]);
    s->set_segment_info(i, segnums[i], segnz[i], segz0[i],
        seg_angle_bound[is]/RAD2DEG, seg_angle_bound[is+1]/RAD2DEG, 
        mean/RAD2DEG);                          
  }

  //---set also segment 0 infos
  s->set_segment_info(0,0,s->nplanes_segment_0, 0, 0.0, 0.0, 0.0);


  if (segment_struct!=NULL)
  {
    cout << "ERROR: MRPET_dump::init_seginfo: ";
    cout << "already defined; (overwrite not supported)" << endl;
    exit(0);
  }
  segment_struct=s;

  //---set sinogram segment identifier lookup
  s->set_sino_identifiers();
  //--------------------------------------------------------------------------

  compute_crystal_positions();


}


void Save_sino_axile_counts(float* array_axial_count, string saved_path_data int arry_size)
{
  ofstream of;
  of.open(saved_path_data.c_str(),std::ofstream::out);

  of<<setw(20)<<"ring index"<<setw(20)<<"Event counts"<<"\r"<<endl;

  /*int arry_size=vector_events_per_z_mcs.size();*/

  for(int i=0;i<=arry_size;i++)
  {
    of<<setw(20)<<i<<setw(20)<<array_axial_count[i]<<"\r"<<endl;

  }

  of.close();


}

/*
 * merge two sinograms
 *
 */
void Add_sino( string path_sino_input_1, string path_sino_input_2,string saved_sino)
{ 

  //class including all the basic information of the MRPET.
  MRPET_dump *dump = new MRPET_dump();	

  ///---measurement data, correction of random and scatter//////////////////////////////////////////////////
  BrainPET_Sinograms* sino_input_1=new BrainPET_Sinograms(dump,"sino_input");  
  BrainPET_Sinograms* sino_input_2=new BrainPET_Sinograms(dump,"scatter");

  bool is_int=false;
  sino_input_1->read_flatSinogram(path_sino_input_1,is_int);
  sino_input_2->read_flatSinogram(path_sino_input_2,is_int);

  cout<<" the total events of sino_1 is: " << sino_input_1->get_n_Events()<< endl;
  cout<<" the total events of sino_2 is: " << sino_input_2->get_n_Events()<< endl;


  //...scatter correction  
  sino_input_1->add_sinogram(sino_input_2);

  cout<<" the total events of merged sino is: " << sino_input_1->get_n_Events()<< endl;

  sino_input_1->write_flat_sinograms(saved_sino,is_int);


  if(sino_input_1!=NULL)
  {
    delete sino_input_1;
    sino_input_1=NULL;

  }

  if(sino_input_2!=NULL)
  {
    delete sino_input_2;
    sino_input_2=NULL;

  }
  if(dump!=NULL)
  {
    delete dump;
    dump=NULL;

  }

}


/*
 * create sinograms with defined value for all the bins 
 *
 */
void create_sino_with_defined_value( string path_saved_sino, float bin_value)
{ 

  //class including all the basic information of the MRPET.
  MRPET_dump *dump = new MRPET_dump();	

  bool is_int = false ;
  ///---measurement data, correction of random and scatter//////////////////////////////////////////////////
  BrainPET_Sinograms* sinos=new BrainPET_Sinograms(dump,"sino_input");  

  sinos->setAllSinogramBins(bin_value);

  cout<<" the total counts of sino is: " << sinos->get_n_Events() <<endl;

  sinos->write_flat_sinograms(path_saved_sino,is_int);


  if(sinos!=NULL)
  {
    delete sinos;
    sinos=NULL;

  }

  if(dump!=NULL)
  {
    delete dump;
    dump=NULL;

  }

}


/*
 * path_norm_sino_with_gap: norm sino with gaps which is 0;
 * path_sino_temp : a sino file which has no gaps,all the gaps are filled a defined value.
 * if the value of a point in norm_sino ==0,&& sino_temp is !=0, then this point is in the gap,
 * you can set the value you defined.
 *
 */

void set_norm_sino_gaps(string path_norm_sino_with_gap,string path_sino_gaps, string path_saved_sino_without_gap,float gap_value=0)
{

  //class including all the basic information of the MRPET.
  MRPET_dump *dump = new MRPET_dump();	

  ///---measurement data, correction of random and scatter//////////////////////////////////////////////////
  BrainPET_Sinograms* sino_input_1=new BrainPET_Sinograms(dump,"sino_norm");  
  BrainPET_Sinograms* sino_input_2=new BrainPET_Sinograms(dump,"temp");

  bool is_int=false;
  sino_input_1->read_flatSinogram(path_norm_sino_with_gap,is_int);
  sino_input_2->read_flatSinogram(path_sino_gaps,is_int);

  cout<<" the total events of sino_1 is: " << sino_input_1->get_n_Events()<< endl;
  cout<<" the total events of sino_2 is: " << sino_input_2->get_n_Events()<< endl;

  int n_planes=sino_input_1->get_n_planes(); 
  int n_views=sino_input_1->get_n_views(); 
  int n_projs=sino_input_1->get_n_projs(); 

  for (int i_plane=0;i_plane<n_planes;i_plane++)
  {
    for(int i_view=0;i_view<n_views;i_view++)
    {
      for(int i_proj=0;i_proj<n_projs;i_proj++)
      {

        int addr = i_plane*n_views*n_projs + i_view*n_projs + i_proj;
        //get the address in the sino
        //dump->proj_to_addr(i_view, i_proj, i_plane, addr);

        float sino_value_2=sino_input_2->get_event_from_address ( addr);

        //if(sino_value_2>0 && sino_value_1 == 0)
        if(sino_value_2>0)
        {
          sino_input_1->set_event_at_address (addr, gap_value);
        }

      }
    }
  }


  cout<<" the total events of merged sino is: " << sino_input_1->get_n_Events()<< endl;

  sino_input_1->write_flat_sinograms(path_saved_sino_without_gap,is_int);


  if(sino_input_1!=NULL)
  {
    delete sino_input_1;
    sino_input_1=NULL;

  }

  if(sino_input_2!=NULL)
  {
    delete sino_input_2;
    sino_input_2=NULL;

  }
  if(dump!=NULL)
  {
    delete dump;
  }






}

/*
 *  There are bad points in the gap sinos, which is taken into gap while they are lors.
 *  So this function is used to find these points and set them to 0 while the gaps are 1.
 *  methods: check the value of the upper and down plane of the same point, if any of the two points is not gap,
 *  then this point is not gap either.
 *
 */

void remove_gap_sino_bad_lor(string path_sino_gap_ori,string path_saved_sino_without_gap,float sino_value=2)
{

  //class including all the basic information of the MRPET.
  MRPET_dump *dump = new MRPET_dump();	

  ///---measurement data, correction of random and scatter//////////////////////////////////////////////////
  BrainPET_Sinograms* sino_input_1=new BrainPET_Sinograms(dump,"sino_norm");  

  bool is_int=false;
  sino_input_1->read_flatSinogram(path_sino_gap_ori,is_int);

  cout<<" the total events of sino_1 is: " << sino_input_1->get_n_Events()<< endl;

  int n_planes=sino_input_1->get_n_planes(); 
  int n_views=sino_input_1->get_n_views(); 
  int n_projs=sino_input_1->get_n_projs(); 

  for (int i_plane=0;i_plane<n_planes;i_plane++)
  {
    for(int i_view=0;i_view<n_views;i_view++)
    {
      for(int i_proj=0;i_proj<n_projs;i_proj++)
      {

        int addr = i_plane*n_views*n_projs + i_view*n_projs + i_proj;
        //get the address in the sino
        //dump->proj_to_addr(i_view, i_proj, i_plane, addr);

        float sino_value_1=sino_input_1->get_event_from_address ( addr);

        if(sino_value_1>0)
        {

          int addr_1 = (i_plane-1)*n_views*n_projs + i_view*n_projs + i_proj;
          int addr_2 = (i_plane+1)*n_views*n_projs + i_view*n_projs + i_proj;

          float sino_value_down = 1;
          float sino_value_up=1;

          if(i_plane>0 && i_plane<(n_planes-1))
          {
            sino_value_down = sino_input_1->get_event_from_address(addr_1);
            sino_value_up = sino_input_1->get_event_from_address(addr_2);

          }
          else if(i_plane==0)
          {
            sino_value_up=sino_input_1->get_event_from_address ( addr_2);


          }
          else if(i_plane==(n_planes-1))
          {
            sino_value_down = sino_input_1->get_event_from_address(addr_1);


          }


          if(sino_value_down==0 || sino_value_up == 0)
          {

            sino_input_1->set_event_at_address (addr, sino_value);

          }
        }

      }
    }
  }


  cout<<" the total events of merged sino is: " << sino_input_1->get_n_Events()<< endl;

  sino_input_1->write_flat_sinograms(path_saved_sino_without_gap,is_int);


  if(sino_input_1!=NULL)
  {
    delete sino_input_1;
    sino_input_1=NULL;

  }

  if(dump!=NULL)
  {
    delete dump;
  }






}




/*
 * set the gaps of the sino with defined value, the value of the  other points are not changed.
 *
 *
 */
void set_sino_gaps(string path_sino_no_gap,string path_sino_with_gap, string path_saved_sino_without_gap,float gap_value=0)
{

  //class including all the basic information of the MRPET.
  MRPET_dump *dump = new MRPET_dump();	

  ///---measurement data, correction of random and scatter//////////////////////////////////////////////////
  BrainPET_Sinograms* sino_input_1=new BrainPET_Sinograms(dump,"sino_norm");  
  BrainPET_Sinograms* sino_input_2=new BrainPET_Sinograms(dump,"temp");

  bool is_int=false;
  sino_input_1->read_flatSinogram(path_sino_no_gap,is_int);
  sino_input_2->read_flatSinogram(path_sino_with_gap,is_int);

  cout<<" the total events of sino_1 is: " << sino_input_1->get_n_Events()<< endl;
  cout<<" the total events of sino_2 is: " << sino_input_2->get_n_Events()<< endl;

  int n_planes=sino_input_1->get_n_planes(); 
  int n_views=sino_input_1->get_n_views(); 
  int n_projs=sino_input_1->get_n_projs(); 

  for (int i_plane=0;i_plane<n_planes;i_plane++)
  {
    for(int i_view=0;i_view<n_views;i_view++)
    {
      for(int i_proj=0;i_proj<n_projs;i_proj++)
      {

        int addr = i_plane*n_views*n_projs + i_view*n_projs + i_proj;
        //get the address in the sino
        //dump->proj_to_addr(i_view, i_proj, i_plane, addr);

        float sino_value_1=sino_input_1->get_event_from_address ( addr);
        float sino_value_2=sino_input_2->get_event_from_address ( addr);

        if(sino_value_1>0 && sino_value_2 == 0)
        {

          int addr_1 = (i_plane-1)*n_views*n_projs + i_view*n_projs + i_proj;
          int addr_2 = (i_plane+1)*n_views*n_projs + i_view*n_projs + i_proj;

          float sino_value_down = 0;
          float sino_value_up=0;

          if(i_plane>0 && i_plane<(n_planes-1))
          {
            sino_value_down = sino_input_2->get_event_from_address ( addr_1);
            sino_value_up=sino_input_2->get_event_from_address ( addr_2);

          }
          else if(i_plane==0)
          {
            sino_value_up=sino_input_2->get_event_from_address ( addr_2);


          }
          else if(i_plane==(n_planes-1))
          {
            sino_value_down = sino_input_2->get_event_from_address ( addr_1);


          }


          if(sino_value_down==0 && sino_value_up == 0)
          {

            sino_input_1->set_event_at_address (addr, gap_value);

          }
        }

      }
    }
  }


  cout<<" the total events of merged sino is: " << sino_input_1->get_n_Events()<< endl;

  sino_input_1->write_flat_sinograms(path_saved_sino_without_gap,is_int);


  if(sino_input_1!=NULL)
  {
    delete sino_input_1;
    sino_input_1=NULL;

  }

  if(sino_input_2!=NULL)
  {
    delete sino_input_2;
    sino_input_2=NULL;

  }
  if(dump!=NULL)
  {
    delete dump;
  }






}


/*
 * set the points of the sino to the defined value except the gaps
 */
void set_sino_value_except_gaps(string path_sino_with_gap, string path_saved_sino,float set_value=1)
{

  //class including all the basic information of the MRPET.
  MRPET_dump *dump = new MRPET_dump();	

  ///---measurement data, correction of random and scatter//////////////////////////////////////////////////
  BrainPET_Sinograms* sino_input_1=new BrainPET_Sinograms(dump,"sino_norm");  

  bool is_int=false;
  sino_input_1->read_flatSinogram(path_sino_with_gap,is_int);

  cout<<" the total events of sino_1 is: " << sino_input_1->get_n_Events()<< endl;

  int n_planes=sino_input_1->get_n_planes(); 
  int n_views=sino_input_1->get_n_views(); 
  int n_projs=sino_input_1->get_n_projs(); 

  for (int i_plane=0;i_plane<n_planes;i_plane++)
  {
    for(int i_view=0;i_view<n_views;i_view++)
    {
      for(int i_proj=0;i_proj<n_projs;i_proj++)
      {

        int addr = i_plane*n_views*n_projs + i_view*n_projs + i_proj;
        //get the address in the sino
        //dump->proj_to_addr(i_view, i_proj, i_plane, addr);

        float sino_value_1=sino_input_1->get_event_from_address ( addr);

        // all the points except the gaps
        if(sino_value_1>0)
        {

          sino_input_1->set_event_at_address (addr, set_value);

        }

      }
    }
  }


  cout<<" the total events of merged sino is: " << sino_input_1->get_n_Events()<< endl;

  sino_input_1->write_flat_sinograms(path_saved_sino,is_int);


  if(sino_input_1!=NULL)
  {
    delete sino_input_1;
    sino_input_1=NULL;

  }


  if(dump!=NULL)
  {
    delete dump;
  }






}









void sino_divid_presto(string path_sino_numerator , string path_sino_denominator, string path_saved_sino)
{


  //class including all the basic information of the MRPET.
  MRPET_dump *dump = new MRPET_dump();	

  ///---measurement data, correction of random and scatter//////////////////////////////////////////////////
  BrainPET_Sinograms* sino_numerator=new BrainPET_Sinograms(dump,"sino_norm");  
  BrainPET_Sinograms* sino_denominator=new BrainPET_Sinograms(dump,"temp");
  BrainPET_Sinograms* sino_result=new BrainPET_Sinograms(dump,"temp");

  bool is_int=false;
  sino_numerator->read_flatSinogram(path_sino_numerator,is_int);
  sino_denominator->read_flatSinogram(path_sino_denominator,is_int);

  cout<<" the total events of sino_1 is: " << sino_numerator->get_n_Events()<< endl;
  cout<<" the total events of sino_2 is: " << sino_denominator->get_n_Events()<< endl;


  //sino_numerator->multiply_by_sinogram(sino_denominator);
  //sino_numerator->divide_by_sinogram(sino_denominator);
  cout<<" the total events of sino_1 is: " << sino_numerator->get_n_Events()<< endl;
  sino_numerator->divide_by_sinogram(sino_denominator);
  sino_numerator->write_flat_sinograms(path_saved_sino,is_int);


  if(sino_numerator!=NULL)
  {
    delete sino_numerator;
    sino_numerator=NULL;

  }

  if(sino_denominator!=NULL)
  {
    delete sino_denominator;
    sino_denominator=NULL;

  }
  if(dump!=NULL)
  {
    delete dump;
  }





}


void sino_divid(string path_sino_numerator , string path_sino_denominator, string path_saved_sino)
{


  //class including all the basic information of the MRPET.
  MRPET_dump *dump = new MRPET_dump();	

  ///---measurement data, correction of random and scatter//////////////////////////////////////////////////
  BrainPET_Sinograms* sino_numerator=new BrainPET_Sinograms(dump,"sino_norm");  
  BrainPET_Sinograms* sino_denominator=new BrainPET_Sinograms(dump,"temp");
  BrainPET_Sinograms* sino_result=new BrainPET_Sinograms(dump,"temp");

  bool is_int=false;
  sino_numerator->read_flatSinogram(path_sino_numerator,is_int);
  sino_denominator->read_flatSinogram(path_sino_denominator,is_int);

  cout<<" the total events of sino_1 is: " << sino_numerator->get_n_Events()<< endl;
  cout<<" the total events of sino_2 is: " << sino_denominator->get_n_Events()<< endl;



  int n_planes=sino_numerator->get_n_planes(); 
  int n_views=sino_numerator->get_n_views(); 
  int n_projs=sino_numerator->get_n_projs(); 

  for (int i_plane=0;i_plane<n_planes;i_plane++)
  {
    for(int i_view=0;i_view<n_views;i_view++)
    {
      for(int i_proj=0;i_proj<n_projs;i_proj++)
      {

        int addr = i_plane*n_views*n_projs + i_view*n_projs + i_proj;
        //get the address in the sino
        //dump->proj_to_addr(i_view, i_proj, i_plane, addr);

        float sino_value_numerator=sino_numerator->get_event_from_address ( addr);
        float sino_value_denominator=sino_denominator->get_event_from_address ( addr);

        float value_temp;

        if(sino_value_denominator>0)
        {
          value_temp = sino_value_numerator/sino_value_denominator ;
        }
        else
        {
          value_temp = 0;
        }


        sino_numerator->set_event_at_address (addr, value_temp);

      }
    }
  }



  sino_numerator->write_flat_sinograms(path_saved_sino,is_int);


  if(sino_numerator!=NULL)
  {
    delete sino_numerator;
    sino_numerator=NULL;

  }

  if(sino_denominator!=NULL)
  {
    delete sino_denominator;
    sino_denominator=NULL;

  }
  if(dump!=NULL)
  {
    delete dump;
  }





}

/*
 *  Generate a sino whose gaps has a defined value(1), while the rest is 0
 *
 */

void get_gaps_sino(string path_sino_with_gap, string path_saved_sino,float set_value=1)

{

  //class including all the basic information of the MRPET.
  MRPET_dump *dump = new MRPET_dump();	

  ///---measurement data, correction of random and scatter//////////////////////////////////////////////////
  BrainPET_Sinograms* sino_input_1=new BrainPET_Sinograms(dump,"sino_norm");  

  bool is_int=false;
  sino_input_1->read_flatSinogram(path_sino_with_gap,is_int);

  cout<<" the total events of sino_1 is: " << sino_input_1->get_n_Events()<< endl;

  int n_planes=sino_input_1->get_n_planes(); 
  int n_views=sino_input_1->get_n_views(); 
  int n_projs=sino_input_1->get_n_projs(); 

  for (int i_plane=0;i_plane<n_planes;i_plane++)
  {
    for(int i_view=0;i_view<n_views;i_view++)
    {
      for(int i_proj=0;i_proj<n_projs;i_proj++)
      {

        int addr = i_plane*n_views*n_projs + i_view*n_projs + i_proj;
        //get the address in the sino
        //dump->proj_to_addr(i_view, i_proj, i_plane, addr);

        float sino_value_1=sino_input_1->get_event_from_address ( addr);

        // all the points except the gaps
        if(sino_value_1>0)
        {
          sino_input_1->set_event_at_address (addr, 0);
        }
        else if(sino_value_1==0)
        {
          sino_input_1->set_event_at_address (addr, set_value);

        }

      }
    }
  }


  cout<<" the total events of merged sino is: " << sino_input_1->get_n_Events()<< endl;

  sino_input_1->write_flat_sinograms(path_saved_sino,is_int);


  if(sino_input_1!=NULL)
  {
    delete sino_input_1;
    sino_input_1=NULL;

  }


  if(dump!=NULL)
  {
    delete dump;
  }






}

/*
 *  normaliztion the sino with the averaged value of 1
 *
 */
void normlized_sino( string input_sino,string saved_sino)
{




  //class including all the basic information of the MRPET.
  MRPET_dump *dump = new MRPET_dump();	

  ///---measurement data, correction of random and scatter//////////////////////////////////////////////////
  BrainPET_Sinograms* sino_input=new BrainPET_Sinograms(dump,"sino_norm");  

  bool is_int=false;
  sino_input->read_flatSinogram(input_sino,is_int);

  cout<<" the total events of sino_1 is: " << sino_input->get_n_Events()<< endl;

  double total_events = sino_input->get_n_Events();

  int n_planes=sino_input->get_n_planes(); 
  int n_views=sino_input->get_n_views(); 
  int n_projs=sino_input->get_n_projs(); 

  int total_bins_except_gaps = 0 ;




  float ave_value= total_events/total_bins;

  float inverse_ave_value=1/ave_value;

  sino_input->scale_by_value(inverse_ave_value);

  cout<<" the total events of sino_1 is: " << sino_input->get_n_Events()<< endl;
  sino_input->write_flat_sinograms(saved_sino,is_int);


  if(sino_input!=NULL)
  {
    delete sino_input;
    sino_input=NULL;

  }

  if(dump!=NULL)
  {
    delete dump;
  }


}



// return value: total number
double get_total_events_simu_data(string path_sino_simu_true_plus_scatter)
{

  MRPET_dump *dump = new MRPET_dump();	

  ///---measurement data, correction of random and norm//////////////////////////////////////////////////

  bool is_short_int=false;
  BrainPET_Sinograms* simu_true_plus_scatter=new BrainPET_Sinograms(dump,"simu_true_plus_scatter");


  simu_true_plus_scatter->read_flatSinogram(path_sino_simu_true_plus_scatter,is_short_int);

  //double 	get_n_Events ()
  double total_events= simu_true_plus_scatter->get_n_Events();

  delete dump;
  dump=NULL;

  delete simu_true_plus_scatter;
  simu_true_plus_scatter=NULL;



  return total_events ;

}











void backup()
{


  int plane_num=1399;
  int z_num=153;
  double* sino_plane_counts_nume=new double[plane_num];
  double* sino_plane_counts_denume=new double[plane_num];

  memset(sino_plane_counts_nume,0,sizeof(double)*plane_num);
  memset(sino_plane_counts_denume,0,sizeof(double)*plane_num);

  double* sino_z_counts_nume=new double[z_num];
  double* sino_z_counts_denume=new double[z_num];

  memset(sino_z_counts_nume,0,sizeof(double)*z_num);
  memset(sino_z_counts_denume,0,sizeof(double)*z_num);


  float* scale_factors_plane=new float[plane_num];
  float* scale_factors_plane=new float[plane_num];
  //std::fill_n(scale_factors, plane_num, 0.1);
  memset(scale_factors_plane,0,sizeof(float)*plane_num);

  float* scale_factors_z=new float[z_num];
  float* scale_factors_z=new float[z_num];
  //std::fill_n(scale_factors, plane_num, 0.1);
  memset(scale_factors_z,0,sizeof(float)*z_num);

  Get_Sino_counts_per_plane(saved_sino_random_norm_cor_prompt, sino_plane_counts_nume);
  Get_Sino_counts_per_plane(path_sino_true_and_scatter_mcs, sino_plane_counts_denume);
  array_divid(sino_plane_counts_nume, sino_plane_counts_denume, scale_factors_plane, plane_num);

  for(int i=0;i<plane_num;i++)
  {
    cout<<"  "<<i<<"  : "<<scale_factors_plane[i]<<endl;
  }

  Sino_scale_slice_by_slice( path_sino_scatter_no_scale_mcs,saved_sino_scatter_slice_scale_mcs, scale_factors_plane);



  Get_Sino_counts_per_z(saved_sino_random_norm_cor_prompt, sino_z_counts_nume);
  Get_Sino_counts_per_z(path_sino_true_and_scatter_mcs, sino_z_counts_denume);
  array_divid(sino_z_counts_nume, sino_z_counts_denume, scale_factors_z, z_num);


  Sino_scale_by_z_position(path_sino_scatter_no_scale_mcs,saved_sino_scatter_z_scale_mcs, scale_factors_z);
  for(int i=0;i<z_num;i++)
  {
    cout<<"  "<<i<<"  : "<<scale_factors_z[i]<<endl;
  }



  if(scale_factors_plane!=NULL)
  {
    delete [] scale_factors_plane ;
    scale_factors_plane=NULL;
  }

  if(scale_factors_z!=NULL)
  {
    delete [] scale_factors_z ;
    scale_factors_z=NULL;
  }

  if(sino_plane_counts_nume!=NULL)
  {
    delete [] sino_plane_counts_nume ;
    sino_plane_counts_nume=NULL;
  }


  if(sino_plane_counts_denume!=NULL)
  {
    delete [] sino_plane_counts_denume ;
    sino_plane_counts_denume=NULL;
  }

  if(sino_z_counts_nume!=NULL)
  {
    delete [] sino_z_counts_nume ;
    sino_z_counts_nume=NULL;
  }

  if(sino_z_counts_denume!=NULL)
  {
    delete [] sino_z_counts_denume ;
    sino_z_counts_denume=NULL;
  }


}
