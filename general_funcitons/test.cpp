#include<iostream>
#include<fstream>
#include<iomanip>

using namespace std;


typedef struct {
  int icry1;
  int icry2;
  short int icry1_ofov_scatter_z_pos;
  short int icry2_ofov_scatter_z_pos;
  char type;
} Coincidence;




int main()
{

  // 
  Coincidence coin;

  int struct_size=(int)sizeof(coin);

  const char *filePath = "./XB1BN286N-BI-01_gpuSimu_0.bin"; 


  fstream file;
  
  //open file 
  file.open(filePath, ios::in | ios::out | ios::binary);
  //or
  //file.open(filePath, fstream::in | fstream::out | fstream::binary);

  if (file.fail())
  {
    cout << "ERROR: Cannot open the file..." << endl;
    return 0;
  }


  while(!file.eof())
  {

     //(1)Reading the next record
    /*file.read(reinterpret_cast<char *>(&coin.icry1), sizeof(coin.icry1));*/
    //file.read(reinterpret_cast<char *>(&coin.icry2), sizeof(coin.icry2));
    //file.read(reinterpret_cast<char *>(&coin.icry1_ofov_scatter_z_pos), sizeof(coin.icry1_ofov_scatter_z_pos));
    //file.read(reinterpret_cast<char *>(&coin.icry2_ofov_scatter_z_pos), sizeof(coin.icry2_ofov_scatter_z_pos));
    /*file.read(reinterpret_cast<char *>(&coin.type), sizeof(coin.type));*/

    //(2) or
    file.read((char* )(&coin.icry1), sizeof(coin.icry1));
    file.read((char* )(&coin.icry2), sizeof(coin.icry2));
    file.read((char* )(&coin.icry1_ofov_scatter_z_pos), sizeof(coin.icry1_ofov_scatter_z_pos));
    file.read((char* )(&coin.icry2_ofov_scatter_z_pos), sizeof(coin.icry2_ofov_scatter_z_pos));
    file.read((char* )(&coin.type), sizeof(coin.type));

    
    // (3) this is the only way for ROOT. (1) AND (2)
    // do not work for ROOT.BUT work for g++
    file.read((char* )(&coin.icry1), sizeof(int));
    file.read((char* )(&coin.icry2), sizeof(int));
    file.read((char* )(&coin.icry1_ofov_scatter_z_pos), sizeof(short int));
    file.read((char* )(&coin.icry2_ofov_scatter_z_pos), sizeof(short int));
    file.read((char* )(&coin.type), sizeof(char));

    // in root ,left is not working.
    cout << left 
      << setw(15) << coin.icry1
      << setw(15)  << coin.icry2
      << setw(15)  << coin.icry1_ofov_scatter_z_pos
      << setw(15)  << coin.icry2_ofov_scatter_z_pos
      << setw(15)  << (int)coin.type
      << endl;


  }

  file.close();

  return 0;

}
