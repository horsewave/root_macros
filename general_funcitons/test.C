/*
 *  read binary file with different data types:
 * int,int, short int,short int, char
 *
 */

#include<stdio.h>
#include<iostream>
#include<fstream>
#include<list>
//#include<vector>
#include<iomanip>

using namespace std;



// An unsigned char can store 1 Bytes (8bits) of data (0-255)
typedef unsigned char BYTE;

typedef struct {
  int icry1;
  int icry2;
  short int icry1_ofov_scatter_z_pos;
  short int icry2_ofov_scatter_z_pos;
  char type;
} Coincidence;

void test()
{


  Coincidence coin;

  int struct_size=(int)sizeof(coin);

  const char *filePath = "./XB1BN286N-BI-01_gpuSimu_0.bin"; 


  fstream file;
  file.open(filePath, ios::in | ios::out | ios::binary);
  //file.open(filePath, fstream::in | fstream::out | fstream::binary);

  if (file.fail())
  {
    cout << "ERROR: Cannot open the file..." << endl;
    return 0;
  }




  while(!file.eof())
  {

     //Reading the next record
    //file.read(reinterpret_cast<char *>(&coin.icry1), sizeof(coin.icry1));
    //file.read(reinterpret_cast<char *>(&coin.icry2), sizeof(coin.icry2));
    //file.read(reinterpret_cast<char *>(&coin.icry1_ofov_scatter_z_pos), sizeof(coin.icry1_ofov_scatter_z_pos));
    //file.read(reinterpret_cast<char *>(&coin.icry2_ofov_scatter_z_pos), sizeof(coin.icry2_ofov_scatter_z_pos));
    //file.read(reinterpret_cast<char *>(&coin.type), sizeof(coin.type));

   /* file.read((char* )(&coin.icry1), sizeof(coin.icry1));*/
    //file.read((char* )(&coin.icry2), sizeof(coin.icry2));
    //file.read((char* )(&coin.icry1_ofov_scatter_z_pos), sizeof(coin.icry1_ofov_scatter_z_pos));
    //file.read((char* )(&coin.icry2_ofov_scatter_z_pos), sizeof(coin.icry2_ofov_scatter_z_pos));
    //file.read((char* )(&coin.type), sizeof(coin.type));



    file.read((char* )(&coin.icry1), sizeof(int));
    file.read((char* )(&coin.icry2), sizeof(int));
    file.read((char* )(&coin.icry1_ofov_scatter_z_pos), sizeof(short int));
    file.read((char* )(&coin.icry2_ofov_scatter_z_pos), sizeof(short int));
    file.read((char* )(&coin.type), sizeof(char));




    cout<< setw(15) << coin.icry1
      << setw(15)  << coin.icry2
      << setw(15)  << coin.icry1_ofov_scatter_z_pos
      << setw(15)  << coin.icry2_ofov_scatter_z_pos
      << setw(15)  << (int)coin.type
      << endl;


  }



  file.close();







}

// Get the size of a file
long get_file_size(FILE *file)
{

  long size;

  fseek (file, 0, SEEK_END);   // non-portable
  size=ftell (file);
  printf ("Size of myfile.txt: %ld bytes.\n",size);
  return size;


}



studymc = new TTree(“studymc”,“TTree from StudyMC.cpp”);
studymc->Branch(“nTfired”, &nTfired, “nTfired/I”);
studymc->Branch(“Tfired”, &Tfired, “Tfired[nTfired]/C”);

//with in the corresponding header :
int nTfired;
Char_t Tfired[200];








TClonesArray* caTfired=new TClonesArray(“TObjString”);

studymc = new TTree(“studymc”,“TTree from StudyMC.cpp”);
studymc->Branch(“nTfired”, &caTfired);

for (Int_t entry=0; entry<…; entry++) {
Int_t nFired=…; // set the number of fired triggers
for (Int_t iFired=0; iFired<nFired <iFired++) {
// get the iFired-th trigger name
const char* tName=framework->get_triggernam(iFired);
new ((*caTfired)[iFired]) TObjString(tName);
}
studymc->Fill();
caTfired->Clear();
}[/code]



std::vector<std::string> vecTfired;
std::vector<std::string>* pVecTfired=&vecTFired;
studymc = new TTree("studymc","TTree from StudyMC.cpp");
studymc->Branch("nTfired", &pVecTfired)
