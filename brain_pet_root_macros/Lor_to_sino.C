
/*
 * Description: This macro is used to convert the LOR data to sino data 
 *
 *
 * Input: 
 * Procedure:
 *
            ////--------1: merge two lor data----------------------------------- 
            /// --------2: convert lor data to sino data without norm----------------------------
            ////--------3: norm correction to all the simu sinos------------------------
            ////--------4: scaling the simu scatter---------------------------------
 
 * Functions:
 *
 *         1: void Lor_to_sino_run(string base_folder_lor,string patient_name, string path_sino_measured_prompt,string path_sino_measured_random,string path_sino_measured_norm,int gpu_num=2, bool is_float_lor=true)
 *         2: double get_total_events_measured_data(string path_sino_measured_prompt,string path_sino_measured_random,string path_sino_measured_norm)
 *         3: void save_sino_simu_true_plus_scatter(string path_sino_simu_true,string path_sino_simu_scatter,string path_saved_sino_simu_true_plus_scatter)
 *         4: void norm_correct_simu_sino(string path_input_sino,string path_norm_simu_sino,string path_saved_norm_sino)
 *         5: double get_total_events_simu_data(string path_sino_simu_true_plus_scatter)
 *         6: void save_sino_simu_true_plus_scatter(string path_sino_simu_true,string path_sino_simu_scatter,string path_saved_sino_simu_true_plus_scatter)
 *         7: void norm_correct_simu_sino(string path_input_sino,string path_norm_simu_sino,string path_saved_norm_sino)
 *         8: int convert_lor_to_sino(string path_lor,string path_saved_sino_data,bool is_float_lor=true)
 *         9: void Sino_scale_whole( string path_sino_input, string saved_sino_scaled,float scale_factor)
 *
 *
 * Editor: Bo Ma
 *
 * Date: 2018.12.07
 *
 */





//void Lor_to_sino(string base_folder_lor,string patient_name, int gpu_num )
//void Lor_to_sino(string base_folder_lor,string patient_name,int gpu_num)
void Lor_to_sino()
{


  //main_1();
  main_2();


  
   gApplication->Terminate();

}


/*
 *  convert LOR data to sino data and  scatter sino scaling for GPU-MCS 
 */
void main_1()
{
   string base_folder = "/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/XB1BN305N-BI/XB1BN305N-BI-01/";
  string base_folder_lor = base_folder +"scatterMCS/";

  string patient_name="XB1BN305N-BI-01";


  string path_sino_measured_prompt = base_folder +"sinos/range_0-1800_prompt.fs";
  string path_sino_measured_random = base_folder +"sinos/range_0-1800_vrr.fs";
  string path_sino_measured_norm = base_folder +"norm/PSNorm-170720_STD_norm.fs";

  int gpu_num=2;


    cout<< "gpu_num is : "<< gpu_num <<endl;
    Lor_to_sino_run(base_folder_lor,patient_name, path_sino_measured_prompt,path_sino_measured_random,path_sino_measured_norm,gpu_num );

}

/*
 * Just convert the LOR to sino and do nothing else
 *
 */

void main_2()
{
    // saved sino data
  string input_folder = "/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/cylinder_phantom_norm_file_mcs/scatterMCS/" ;
  
  string path_lor_data = input_folder + "norm_acq_gpuSimu_0_true.flor";
  string path_saved_sino = input_folder + "norm_acq_gpuSimu_0_true.fs";
  convert_lor_to_sino(path_lor_data,path_saved_sino);

  
}



// the main function to run 
void Lor_to_sino_run(string base_folder_lor,string patient_name, string path_sino_measured_prompt,string path_sino_measured_random,string path_sino_measured_norm,int gpu_num=2, bool is_float_lor=true)
{


  bool is_ofov_info=false;

  string part_name_gpu="gpuSimu_0";

  string part_name_gpu_0=part_name_gpu;
  string part_name_gpu_1="gpuSimu_1";

  string part_name_true="true";
  string part_name_scatter="scatter";
  string part_name_scatter_ofov="scatter_ofov";
  string part_name_true_plus_scatter="true_plus_scatter";
 string part_name_merged="merged";

  string part_name_normed="normed";
  string part_name_scaled="scaled";

  string suffix_lor=".flor";
  string suffix_sino=".fs";


  string path_sino_simu_norm="/data/PET/mr_pet_temp/Ma/software/data/gpupet/commonFiles/norm_files/BrainPET_cylinder_norm_12cm_gpu_wdwellcorr.fs";


  ////---------1: merge two lor data----------------------------------- 

   if(gpu_num==2)
  {

    
  cout<<"------------1: start merge two lor data--------------"<<endl;
  
  string path_lor_true_0=base_folder_lor+ patient_name+ "_"+ part_name_gpu_0 + "_"+ part_name_true +suffix_lor;
  string path_lor_scatter_0=base_folder_lor+ patient_name+ "_"+ part_name_gpu_0 + "_"+ part_name_scatter +suffix_lor; 
  string path_lor_ofov_scatter_0=base_folder_lor+ patient_name+ "_"+ part_name_gpu_0 + "_"+ part_name_scatter_ofov +suffix_lor;

  string path_lor_true_1=base_folder_lor+ patient_name+ "_"+ part_name_gpu_1 + "_"+ part_name_true +suffix_lor;
  string path_lor_scatter_1=base_folder_lor+ patient_name+ "_"+ part_name_gpu_1 + "_"+ part_name_scatter +suffix_lor; 
  string path_lor_ofov_scatter_1=base_folder_lor+ patient_name+ "_"+ part_name_gpu_1 + "_"+ part_name_scatter_ofov +suffix_lor;


  string merged_lor_true = base_folder_lor+ part_name_merged +"_" + part_name_true +suffix_lor;
  string merged_lor_scatter = base_folder_lor+ part_name_merged +"_" + part_name_scatter+suffix_lor; 
  string merged_lor_ofov_scatter = base_folder_lor+ part_name_merged +"_" + part_name_scatter_ofov+suffix_lor ;


  Merge_lordata(path_lor_true_0,path_lor_true_1,merged_lor_true);
  Merge_lordata(path_lor_scatter_0,path_lor_scatter_1,merged_lor_scatter);
   if(is_ofov_info)
  {
  Merge_lordata(path_lor_ofov_scatter_0,path_lor_ofov_scatter_1,merged_lor_ofov_scatter);

  }


 cout<<"------------1: finish merge two lor data--------------"<<endl;


    

  }



  // --------2: convert lor data to sino data without norm----------------------------
  
  cout<<"------------2: start convert lor data to sino data without norm--------------"<<endl;

  if(gpu_num==2)
  {
    // //lor data
  string path_lor_true =merged_lor_true ;
  string path_lor_scatter = merged_lor_scatter ; 
  string path_lor_ofov_scatter =  merged_lor_ofov_scatter;


  }
  else if(gpu_num==1)
  {
    // //lor data
  string path_lor_true=base_folder_lor+ patient_name+ "_"+ part_name_gpu + "_"+ part_name_true +suffix_lor;

  string path_lor_scatter=base_folder_lor+ patient_name+ "_"+ part_name_gpu + "_"+ part_name_scatter +suffix_lor; 
  string path_lor_ofov_scatter=base_folder_lor+ patient_name+ "_"+ part_name_gpu + "_"+ part_name_scatter_ofov +suffix_lor;

  }


    // saved sino data
  string path_saved_sino_true=base_folder_lor+ part_name_true +suffix_sino;
  string path_saved_sino_scatter=base_folder_lor+ part_name_scatter +suffix_sino;

  string path_saved_sino_ofov_scatter=base_folder_lor+ part_name_scatter_ofov +suffix_sino;

  string path_saved_sino_true_plus_scatter=base_folder_lor+ part_name_true_plus_scatter +suffix_sino;

  cout<< path_saved_sino_true_plus_scatter <<endl;

  //  2.1 true lor without norm
  string path_lor_data=path_lor_true;
  string path_saved_sino=path_saved_sino_true;
  convert_lor_to_sino(path_lor_data,path_saved_sino);

   //  2.2 scatter lor without norm
  path_lor_data=path_lor_scatter;
  path_saved_sino=path_saved_sino_scatter;
  convert_lor_to_sino(path_lor_data,path_saved_sino);

  //  2.3 ofov lor without norm
  path_lor_data=path_lor_ofov_scatter;
  path_saved_sino=path_saved_sino_ofov_scatter;
  
  if(is_ofov_info)
  {

  convert_lor_to_sino(path_lor_data,path_saved_sino);
  }

  //2.4 get the true+ scatter sino
  save_sino_simu_true_plus_scatter(path_saved_sino_true,path_saved_sino_scatter,path_saved_sino_true_plus_scatter);

  cout<<"------------2: finish convert lor data to sino data without norm--------------"<<endl;



  ////--------------3: norm correction to all the simu sinos------------------------
  cout<<"------------3: start norm correction to all the simu sinos--------------"<<endl;

  string path_saved_sino_normed_true = base_folder_lor+ part_name_normed+ "_" + part_name_true +suffix_sino;
  string path_saved_sino_normed_scatter=base_folder_lor+part_name_normed+ "_" +  part_name_scatter +suffix_sino;

  string path_saved_sino_normed_ofov_scatter=base_folder_lor+part_name_normed+ "_" +  part_name_scatter_ofov +suffix_sino;
  string path_saved_sino_normed_true_plus_scatter=base_folder_lor+ part_name_normed+ "_" +  part_name_true_plus_scatter +suffix_sino;
  ;

  cout<< path_saved_sino_normed_true_plus_scatter<<endl;


  // 3.1 true
  norm_correct_simu_sino( path_saved_sino_true, path_sino_simu_norm, path_saved_sino_normed_true);

  // 3.2 scatter
  norm_correct_simu_sino( path_saved_sino_scatter, path_sino_simu_norm, path_saved_sino_normed_scatter);

  // 3.3 ofov scatter
   if(is_ofov_info)
  {

  norm_correct_simu_sino( path_saved_sino_ofov_scatter, path_sino_simu_norm, path_saved_sino_normed_ofov_scatter);
  }



  // 3.4 true_plus_scatter
  norm_correct_simu_sino( path_saved_sino_true_plus_scatter, path_sino_simu_norm, path_saved_sino_normed_true_plus_scatter);


  cout<<"------------3: finish norm correction to all the simu sinos--------------"<<endl;


  ////--------------4: scaling the simu scatter---------------------------------


  cout<<"------------4: start to scaling the simu scatter--------------"<<endl;
 
  // 4.1 get the total number of measured data; with random and norm correction
  string base_folder_measure="/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/XB1BN305N-BI/XB1BN305N-BI-01/";
  //string path_sino_measured_prompt = base_folder_measure +"sinos/range_0-1800_prompt.fs";
  //string path_sino_measured_random = base_folder_measure +"sinos/range_0-1800_vrr.fs";
  string path_sino_measured_norm = base_folder_measure +"norm/PSNorm-170720_STD_norm.fs";

  double total_events_measured_data = get_total_events_measured_data(path_sino_measured_prompt,path_sino_measured_random,path_sino_measured_norm);

  cout<<"total measured data is: "<<  total_events_measured_data<<endl;


  //// 4.2 get the total number of simu data; with norm correction,
  //// and save the true+scatter

  double total_events_simu_data= get_total_events_simu_data( path_saved_sino_normed_true_plus_scatter);

  cout<<"total measured data is: "<<  total_events_simu_data<<endl;


  //// 4.3 calculate the scaling factor

  float  scaling_factor=total_events_measured_data/total_events_simu_data;
  // 4.4 save the scaled scatter sino
  string path_saved_sino_scaled_scatter =base_folder_lor+part_name_normed+ "_" +part_name_scaled +"_" + part_name_scatter +suffix_sino;

  Sino_scale_whole( path_saved_sino_scatter, path_saved_sino_scaled_scatter,scaling_factor);

  cout<<"------------4: finish scaling the simu scatter--------------"<<endl;

  cout<<"------------all finished! You can reconstruct now!--------------"<<endl;


}






// get the total number of measured data
// (prompt -random)*norm
// return value: total number
double get_total_events_measured_data(string path_sino_measured_prompt,string path_sino_measured_random,string path_sino_measured_norm)
{

  MRPET_dump *dump = new MRPET_dump();	

  ///---measurement data, correction of random and norm//////////////////////////////////////////////////

  BrainPET_Sinograms* measure_sino=new BrainPET_Sinograms(dump,"prompt");

  // return value:total value of( prompt -random)
  Corr_rand_norm( path_sino_measured_prompt,path_sino_measured_random, path_sino_measured_norm, measure_sino);

  //double 	get_n_Events ()
  double total_events= measure_sino->get_n_Events();


  delete dump;
  dump=NULL;

  delete measure_sino;
  measure_sino=NULL;


  return total_events ;



}


void save_sino_simu_true_plus_scatter(string path_sino_simu_true,string path_sino_simu_scatter,string path_saved_sino_simu_true_plus_scatter)
{

  MRPET_dump *dump = new MRPET_dump();	

  ///---measurement data, correction of random and norm//////////////////////////////////////////////////

  bool is_short_int=false;
  BrainPET_Sinograms* simu_true_sino=new BrainPET_Sinograms(dump,"simu_true");
  BrainPET_Sinograms* simu_scatter_sino=new BrainPET_Sinograms(dump,"simu_scatter");


  simu_true_sino->read_flatSinogram(path_sino_simu_true,is_short_int);
  simu_scatter_sino->read_flatSinogram(path_sino_simu_scatter,is_short_int);

  // add true and scatter
  //add_sinogram (BrainPET_Sinograms *sinogram)
  simu_true_sino->add_sinogram(simu_scatter_sino);

  //save true+scatter sinogram
  simu_true_sino->write_flat_sinograms(path_saved_sino_simu_true_plus_scatter,is_short_int);

  delete dump;
  dump=NULL;

  delete simu_true_sino;
  simu_true_sino=NULL;

  delete simu_scatter_sino;
  simu_scatter_sino=NULL;







}

void norm_correct_simu_sino(string path_input_sino,string path_norm_simu_sino,string path_saved_norm_sino)
{
  MRPET_dump *dump = new MRPET_dump();	

  ///---measurement data, correction of random and norm//////////////////////////////////////////////////

  bool is_short_int=false;
  BrainPET_Sinograms* input_simu_sino=new BrainPET_Sinograms(dump,"simu_input");
  BrainPET_Sinograms* norm_sino_mcs=new BrainPET_Sinograms(dump,"simunorm");


  input_simu_sino->read_flatSinogram(path_input_sino,is_short_int);
  norm_sino_mcs->read_flatSinogram(path_norm_simu_sino,is_short_int);

  input_simu_sino->multiply_by_sinogram(norm_sino_mcs);

  //save true+scatter sinogram
  input_simu_sino->write_flat_sinograms(path_saved_norm_sino,is_short_int);


  delete dump;
  dump=NULL;

  delete input_simu_sino;
  input_simu_sino=NULL;

  delete norm_sino_mcs;
  norm_sino_mcs=NULL;




}





// get the total number of measured data
// (prompt -random)*norm
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


//random and norm correction
//output: s0:corrected prompt sinogram.
int Corr_rand_norm(string prompt_sino,string random_sino, string norm_sino, BrainPET_Sinograms *s0)
{

  MRPET_dump *dump = new MRPET_dump();	
  ///---measurement data, correction of random and norm//////////////////////////////////////////////////
  BrainPET_Sinograms* s1=new BrainPET_Sinograms(dump,"random");

  bool is_short_int=true;
  s0->read_flatSinogram(prompt_sino,is_short_int);

  is_short_int=false;
  s1->read_flatSinogram(random_sino,is_short_int);

  //...random correction
  s0->subtract_sinogram(s1);
  int measevents=(int)s0->get_n_Events();

  //...norm correction
  s1->read_flatSinogram(norm_sino,is_short_int);	
  s0->multiply_by_sinogram(s1);

  delete s1;
  s1=NULL;



  return measevents;

}

//---simulation data/////////////////////////////////////////////////////////////////////////////////
//convert listmode to lor data
//int Merge_lordata(BrainPET_LORfileData* lor_true,  BrainPET_LORfileData* lor_scat, BrainPET_LORfileData* lor_true_without_scat, int nr_of_gpus,string simufolder, string basename_simu)
int Merge_lordata(string path_lor_data_1,string path_lor_data_2,string path_saved_merged_lor,bool is_float_lor=true)
{
  // merge all the lor data to one.


  // class to manage lor data
  //  	BrainPET_LORfileData (int n_modul_pairs0, int n_modulpair_size0, bool data_float0=true)
  BrainPET_LORfileData* lor0 = new BrainPET_LORfileData(304, 864*864, is_float_lor);
  BrainPET_LORfileData* lor1 = new BrainPET_LORfileData(304, 864*864, is_float_lor);

  //BrainPET_LORfileData::load_data	(	char *filename, bool expect_floats = false)	
  lor0->load_data(path_lor_data_1.c_str(), is_float_lor);	
  cout<<"the total num single: "<< (int)lor0->get_count_sum()<<endl;
  lor1->load_data(path_lor_data_2.c_str(), is_float_lor);


  //here ,lor0=lor0+lor1: true+scatter events
  lor0->add_data_from(lor1);

  if(lor0->get_count_sum()==0)
    cout<<"Simulation on GPU device "<< i<< " failed"<<endl;

  int total_lor_counts=(int)lor0->get_count_sum();
  cout<<"the total num merge: "<< (int)lor0->get_count_sum()<<endl;

  //	store_as_LORfileData_float (char *fname, bool overwrite=false)
  lor0->store_as_LORfileData_float(path_saved_merged_lor.c_str());

  delete lor0 ;
  lor0=NULL;

  delete lor1 ;
  lor1=NULL;



  // end of "for" circle

  return total_lor_counts;

}


//return value: the total counts of LOR data
int convert_lor_to_sino(string path_lor,string path_saved_sino_data,bool is_float_lor=true)
{


  MRPET_dump *dump = new MRPET_dump();	

  cout<< " Create Class of  BrainPET_LORfileData for LOR files of lor_true_and_scat, lor_scat, lor_true_without_scat"<< endl;

  BrainPET_LORfileData* lor_data = new BrainPET_LORfileData(304, 864*864, is_float_lor);

  //BrainPET_LORfileData::load_data	(	char *filename, bool expect_floats = false)	
  lor_data->load_data(path_lor.c_str(), is_float_lor);	

  if(lor_data->get_count_sum()==0)
    cout<<"Simulation on GPU device "<< i<< " failed"<<endl;

  int total_lor_counts=(int)lor_data->get_count_sum();

  //converter used to convert lor data to sinogram
  BrainPET_QuickConverter* converter=new BrainPET_QuickConverter("/data/BrainPET_Sys/Software/TemplateProcData/version1/lookups/LookupTable.BrainPET_QuickConverter.dat");

  //...procedure to convert a lor to sinogram
  int nelements;

  // stack for lor data: s1:true +scatter 
  float* flor;
  flor=lor_data->get_data_pointer_float(nelements);


  BrainPET_Sinograms* sino_data=new BrainPET_Sinograms(dump,"simu_sino");
  // stack for sino data
  sino_data->resetAllSinograms();
  int nfloats;
  float* fsino;

  fsino=sino_data->get_handle_flat_float(nfloats);

  //converting
  //void 	convert_LOR2sino (float *lordata, int nlordata, float *sinodata, int nsinodata, bool dwell_correction)
  converter->convert_LOR2sino(flor,nelements,fsino,nfloats,true);

  // save sino data
  bool is_short_int=false;
  sino_data->write_flat_sinograms(path_saved_sino_data,false);

  /*string cmd_del_bin=".! rm "+simufolder+"*.bin";*/
  //cout<<"delet .bin file: "<<cmd_del_bin<< endl;
  /*gROOT->ProcessLine(cmd_del_bin.c_str());*/

  delete dump;
  dump=NULL;


  delete converter ;
  converter=NULL;


  delete sino_data;
  sino_data=NULL;

  delete lor_data ;
  lor_data=NULL;

  return total_lor_counts;

}

void Sino_scale_whole( string path_sino_input, string saved_sino_scaled,float scale_factor)
{ 

  //class including all the basic information of the MRPET.
  MRPET_dump *dump = new MRPET_dump();	

  ///---measurement data, correction of random and scatter//////////////////////////////////////////////////
  BrainPET_Sinograms* sino_input=new BrainPET_Sinograms(dump,"sino_input");  

  bool is_short_int=false;
  sino_input->read_flatSinogram(path_sino_input,is_short_int);


  //---scale scatter sinogram
  sino_input->scale_by_value(scale_factor); 
  cout<<"scatterenuation correction finished!"<<endl;
  sino_input->write_flat_sinograms(saved_sino_scaled,is_short_int);


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


