
#include "./file_operations.h"


FileOperator::FileOperator(){

}

// Load materials from data base
void FileOperator::Set_file_name(string filename) {

  m_file_name_input=filename;

}

//
string FileOperator::Extract_string() {

  ifstream is_file(m_file_name_input.c_str());
  string line;
  string string_extract;

  while (is_file) {
    skip_comment(is_file);
    getline(is_file, line);

    if (is_file) 
    {
      string_extract = line.substr(0, line.find(":"));
      cout<<"string is: "<<string_extract<<endl;

      m_v_string.push_back(string_extract);

    } // if

  } // while


  return string_extract;
}

int FileOperator::Extract_float(string find_str_start,string find_str_end,size_t shift_pos,vector<float> &v_float)
{


  ifstream is_file(m_file_name_input.c_str());
  string line;
  float res;

  while (is_file) {
    skip_comment(is_file);
    getline(is_file, line);

    if (is_file) 
    {


      string line_temp=line;

      size_t find_str_pos=line_temp.find(find_str_start);
      //size_t shift_pos=find_str_start.size();

      if((find_str_pos!=string::npos))
      {
        //cout<<"find_str_pos is: "<<find_str_pos<<endl;

      line_temp = line_temp.substr(find_str_pos+shift_pos);

      cout<<"line is: "<<line_temp<<endl;

      std::string line_temp1 = line_temp.substr(0, line_temp.find(find_str_end));
      std::stringstream(line_temp1) >> res;

      cout<<"float is: "<<res<<endl;
       v_float.push_back(res);


      }

      
    } // if

  } // while

  return v_float.size();
}

int FileOperator::Extract_int(std::string line) {
  unsigned short int res;
  line = line.substr(line.find("n=")+2);
  line = line.substr(0, line.find(";"));
  //line = remove_white_space(line);
  std::stringstream(line) >> res;
  return res;
}


// Skip comment starting with "#"
void FileOperator::skip_comment(std::istream & is) {
  char c;
  char line[1024];
  if (is.eof()) return;
  is >> c;
  while (is && (c=='#')) {
    is.getline(line, 1024);
    is >> c;
    if (is.eof()) return;
  }
  is.unget();
}


// Remove all white space
string FileOperator::remove_white_space(string line) {
  
  string txt=line;
  //txt.erase(remove_if(txt.begin(), txt.end(), std::isspace), txt.end());
  //txt.erase(remove_if(txt.begin(), txt.end(), [](unsigned char x){return std::isspace(x);}), txt.end());
  return txt;
}

