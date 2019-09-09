#include<iostream>
#include<fstream>
#include<string>
#include <stdio.h>

#include "./file_operations.h"

using namespace std;

int Get_total_singles(string file_path,vector<float> &v_float)
{
  ////string file_path="./mats.data";

  string find_str_start="Number of Singles";

  size_t shift_pos=find_str_start.size();
  shift_pos+=1;

  string find_str_end=",";

  FileOperator *my_file=new FileOperator;

  my_file->Set_file_name(file_path);

  //my_file->Extract_string();
  int v_size=my_file->Extract_float(find_str_start,find_str_end, shift_pos,v_float);

      
      if(my_file!=NULL)
      {
      delete my_file;
      my_file=NULL;
      }

      return v_size;


}

int Get_scatter_singles(string file_path,vector<float> &v_float)
{
  ////string file_path="./mats.data";

  string find_str_start="Number of scattered";

  size_t shift_pos=find_str_start.size();
  shift_pos+=1;

  string find_str_end=",";

  FileOperator *my_file=new FileOperator;

  my_file->Set_file_name(file_path);

  //my_file->Extract_string();
  int v_size=my_file->Extract_float(find_str_start,find_str_end, shift_pos,v_float);

      
      if(my_file!=NULL)
      {
      delete my_file;
      my_file=NULL;
      }

      return v_size;


}

int Get_scatter_hits(string file_path,vector<float> &v_float)
{
  ////string file_path="./mats.data";

  string find_str_start="Number of compton in Crystal";

  size_t shift_pos=find_str_start.size();
  shift_pos+=1;

  string find_str_end=",";

  FileOperator *my_file=new FileOperator;

  my_file->Set_file_name(file_path);

  //my_file->Extract_string();
  int v_size=my_file->Extract_float(find_str_start,find_str_end, shift_pos,v_float);

      
      if(my_file!=NULL)
      {
      delete my_file;
      my_file=NULL;
      }

      return v_size;


}


int Get_photoelectric_hits(string file_path,vector<float> &v_float)
{
  ////string file_path="./mats.data";

  string find_str_start="Number of photoelectric in crystal";

  size_t shift_pos=find_str_start.size();
  shift_pos+=1;

  string find_str_end=",";

  FileOperator *my_file=new FileOperator;

  my_file->Set_file_name(file_path);

  //my_file->Extract_string();
  int v_size=my_file->Extract_float(find_str_start,find_str_end, shift_pos,v_float);

      
      if(my_file!=NULL)
      {
      delete my_file;
      my_file=NULL;
      }

      return v_size;


}
int Get_true_coincidences(string file_path,vector<float> &v_float)
{
  ////string file_path="./mats.data";

  string find_str_start="Trues";

  size_t shift_pos=find_str_start.size();
  shift_pos+=1;

  string find_str_end=",";

  FileOperator *my_file=new FileOperator;

  my_file->Set_file_name(file_path);

  //my_file->Extract_string();
  int v_size=my_file->Extract_float(find_str_start,find_str_end, shift_pos,v_float);

      
      if(my_file!=NULL)
      {
      delete my_file;
      my_file=NULL;
      }

      return v_size;


}


int main( int argc, char *argv[ ] )
{
  
  string file_path="./results.txt";
  
  vector<float> v_float;
  //int v_size=Get_total_singles(file_path,v_float);
  //int v_size=Get_scatter_singles(file_path,v_float);
  //int v_size=Get_scatter_hits(file_path,v_float);
  //int v_size=Get_photoelectric_hits(file_path,v_float);
  int v_size=Get_true_coincidences(file_path,v_float);
    
    cout<<"vector size is: "<<v_size<<endl;
      for(int i=0;i<v_size;i++)
      {
      cout<<"vector "<<i<<"is: "<<v_float[i]<<endl;
      }



      return 1;


      }
