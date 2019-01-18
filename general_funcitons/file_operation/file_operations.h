#ifndef FILEOPERATOR_H
#define FILEOPERATOR_H


#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>
#include <cctype>
#include<iostream>

using namespace std;


class FileOperator
{

  public:

    //reconstructor
    FileOperator();

    void Set_file_name(string filename);
    string Extract_string();
    int Extract_int(string line);
    int Extract_float(string find_str_start,string find_str_end,size_t shift_pos,vector<float> &v_float);

    void skip_comment(istream & is);

    // Remove all white space
    string remove_white_space(string txt); 
    
    //~FileOperator();


  private:

    string m_file_name_input;
    string m_file_name_output;
    vector<string> m_v_string;
    vector<float> m_v_float;
    vector<int> m_v_int;

};


#endif
