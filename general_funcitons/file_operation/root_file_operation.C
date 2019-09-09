void root_file_operation()
{

  gROOT->Reset();

 gSystem->Load("./file_operations_cpp.so");

 string file_path="./results.txt";
 


const int run_num=10;


  
  //get photoelectric hits
  string find_str_start="Number of photoelectric in crystal";
  float a_float_photoelectric_hits[run_num]={0};
  int v_size=Get_data(file_path,find_str_start,a_float_photoelectric_hits,run_num);

   
  //get scatter hits
  find_str_start="Number of compton in Crystal";
  float a_float_scatter_hits[run_num]={0};
  v_size=Get_data(file_path,find_str_start,a_float_scatter_hits,run_num);

  //get total singles
  find_str_start="Number of Singles";
  float a_float_total_singles[run_num]={0};
  v_size=Get_data(file_path,find_str_start,a_float_total_singles,run_num);

 
  //get scatter singles
  find_str_start="Number of scattered";
  float a_float_scatter_singles[run_num]={0};
  v_size=Get_data(file_path,find_str_start,a_float_scatter_singles,run_num);

 
  //get true coincidences
  find_str_start="Trues";
  float a_float_true_coincidences[run_num]={0};
  v_size=Get_data(file_path,find_str_start,a_float_true_coincidences,run_num);

  //get scatter coincidences
  find_str_start="therein scattered";
  float a_float_scatter_coincidences[run_num]={0};
  v_size=Get_data(file_path,find_str_start,a_float_scatter_coincidences,run_num);

  //get random coincidences
  find_str_start="Randoms";
  float a_float_random_coincidences[run_num]={0};
  v_size=Get_data(file_path,find_str_start,a_float_random_coincidences,run_num);

  float mean;
  float std_s;
  int verbos=0;
if(verbos==0)
{

  //photoelectric_hits
  mean=TMath::Mean(run_num,a_float_photoelectric_hits);
  std_s=TMath::RMS(run_num,a_float_photoelectric_hits);
  cout<<"mean_photoelectric_hits is: "<<mean<<endl;
  cout<<"std_photoelectric_hits is: "<<std_s<<endl;

  //scatter_hits
  mean=TMath::Mean(run_num,a_float_scatter_hits);
  std_s=TMath::RMS(run_num,a_float_scatter_hits);
  cout<<"mean_scatter_hits is: "<<mean<<endl;
  cout<<"std_scatter_hits is: "<<std_s<<endl;

  //total_singles
  mean=TMath::Mean(run_num,a_float_total_singles);
  std_s=TMath::RMS(run_num,a_float_total_singles);
  cout<<"mean_total_singles is: "<<mean<<endl;
  cout<<"std_total_singles is: "<<std_s<<endl;

 //scatter_singles
  mean=TMath::Mean(run_num,a_float_scatter_singles);
  std_s=TMath::RMS(run_num,a_float_scatter_singles);
  cout<<"mean_scatter_singles is: "<<mean<<endl;
  cout<<"std_scatter_singles is: "<<std_s<<endl;

  //true_coincidences
  mean=TMath::Mean(run_num,a_float_true_coincidences);
  std_s=TMath::RMS(run_num,a_float_true_coincidences);
  cout<<"mean_true_coincidencesis: "<<mean<<endl;
  cout<<"std_true_coincidences is: "<<std_s<<endl;


//scatter_coincidences
  mean=TMath::Mean(run_num,a_float_scatter_coincidences);
  std_s=TMath::RMS(run_num,a_float_scatter_coincidences);
  cout<<"mean_scatter_coincidences is: "<<mean<<endl;
  cout<<"std_scatter_coincidences is: "<<std_s<<endl;

//random_coincidences
  mean=TMath::Mean(run_num,a_float_random_coincidences);
  std_s=TMath::RMS(run_num,a_float_random_coincidences);
  cout<<"mean_random_coincidences is: "<<mean<<endl;
  cout<<"std_random_coincidences is: "<<std_s<<endl;


}



    


}

int Get_data(string file_path,string find_str_start, float* arr_float,const int array_size)
{
  ////string file_path="./mats.data";

  //string find_str_start="Number of Singles";

  size_t shift_pos=find_str_start.size();
  shift_pos+=1;

  string find_str_end=",";

  FileOperator *my_file=new FileOperator;

  my_file->Set_file_name(file_path);
 vector<float> v_float;

  int v_size=my_file->Extract_float(find_str_start,find_str_end, shift_pos,v_float);

 if(v_size!=array_size)
 {
   cout<<"Error, the vector size is not equal to the array size!!!!"<<"vector size: "<<v_size<<"  array size: "<<array_size<<endl;
   return 0;
 }

 else
 {
   for(int i=0;i<v_size;i++)
   {
     arr_float[i]=v_float[i];
   }
 }

      
      if(my_file!=NULL)
      {
      delete my_file;
      my_file=NULL;
      }

      return v_size;


}


/*void Get_gpu_1(const int run_num, float* full_data, float * gpu_data_1)*/
//{
  //for(int i=0;i<run_num;i++)
  //{

  //}
/*}*/
