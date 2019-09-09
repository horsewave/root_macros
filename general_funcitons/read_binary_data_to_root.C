/*
 *  read binary file with different data types:
 * int,int, short int,short int, char
 *
 * Editor: Bo Ma
 *
 *
 */

#include<stdio.h>
#include<iostream.h>
#include<list>
//#include<vector>

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


void read_binary_data_to_root()
{
  list<int> list_cry1;
  list<int> list_cry2;
  list<short int> list_cry1_ofov_scatter_z_pos ;
  list<short int> list_cry2_ofov_scatter_z_pos;
  list<char> list_type;

  const int list_size=read_binary_data_to_list(list_cry1,list_cry2, list_cry1_ofov_scatter_z_pos ,list_cry2_ofov_scatter_z_pos,list_type);
  cout<<"list size is: "<<list_size<<endl;


  string path_root_file="./my_root_file.root";

  string tree_tag="coincidence";
  string tree_name="coincedence from GPU MCS";

  create_root_file(path_root_file,tree_tag, tree_name );


  string branch_tag="cry1";
  string leaf_list="cry1/I";

  write_list_to_root(path_root_file, list_cry1,tree_tag,tree_name,branch_tag,leaf_list);

  //t->DropBranchFromCache("*",kTRUE);
  //t->Refresh();
  string branch_tag="cry2";
  string leaf_list="cry2/I";
  add_branch_to_tree(path_root_file,list_cry2,tree_tag,tree_name,branch_tag,leaf_list);

  string branch_tag="cry1_ofov_scatter_z_pos";
  string leaf_list="cry1_ofov_scatter_z_pos/S";
  add_branch_to_tree(path_root_file,list_cry1_ofov_scatter_z_pos,tree_tag,tree_name,branch_tag,leaf_list);

  string branch_tag="cry2_ofov_scatter_z_pos";
  string leaf_list="cry2_ofov_scatter_z_pos/S";
  add_branch_to_tree(path_root_file,list_cry2_ofov_scatter_z_pos,tree_tag,tree_name,branch_tag,leaf_list);

  string branch_tag="coincidence_type";
  string leaf_list="coincedence_type/I";
  //string leaf_list="coincedence_type/C";//I don't know how to  store the Charactor data.
  //But if you don't set leaf_list, and using string, it can show the charactor
  add_branch_to_tree(path_root_file,list_type,tree_tag,tree_name,branch_tag,leaf_list);
  //add_string_branch(path_root_file,list_type,tree_tag,tree_name,branch_tag,leaf_list);




}


void create_root_file(string path_root_file,string tree_tag, string tree_name )
{

  //TTree* t=new TTree("coincidence","coincedence from GPU MCS");
  TTree* t=new TTree(tree_tag.c_str(),tree_name.c_str());

  ////---5: write the tree to the root file
  TFile* f=new TFile(path_root_file.c_str(),"recreate");
  t->Write();//// save only the new version of the tree
  //f->Close();

  delete t;
  t=NULL;

  delete f;
  f=NULL;




}

// write a branch of the tree, this is the first branch. IF you want
// add other branches, use add_branch_to_tree()
void write_list_to_root(string path_root_file,list<int> &data_list,string tree_tag,string tree_name,string branch_tag,string leaf_list)
{

  TFile* f=new TFile(path_root_file.c_str(),"update");

  int branch_value;

  TTree * t=(TTree*)f->Get(tree_tag.c_str());

  TBranch* branch=t->Branch(branch_tag.c_str(),&branch_value,leaf_list.c_str());

  for(list<int>::iterator it = data_list.begin();
      it != data_list.end(); it++)
  {

    branch_value=*it;
    //branch->Fill();
    t->Fill();
  }

  //t->AddBranchToCache(branch,kTRUE);
  //t->AddBranchToCache(branch,kFALSE);

  t->Print();
  //t->Scan();

  ////---5: write the tree to the root file
  t->Write("",TObject::kOverwrite);//// save only the new version of the tree
  //t->Write();//// save only the new version of the tree
  f->Close();

  delete f;
  f=NULL;

}

// 
void add_branch_to_tree(string path_root_file,list<int> &data_list,string tree_tag,string tree_name,string branch_tag,string leaf_list)
{

  TFile* f=new TFile(path_root_file.c_str(),"update");

  int branch_value;

  TTree * t=(TTree*)f->Get(tree_tag.c_str());

  TBranch* branch=t->Branch(branch_tag.c_str(),&branch_value,leaf_list.c_str());

  for(list<int>::iterator it = data_list.begin();
      it != data_list.end(); it++)
  {

    branch_value=*it;
    branch->Fill();
    //t->Fill();
  }

  //t->AddBranchToCache(branch,kTRUE);
  //t->AddBranchToCache(branch,kFALSE);

  t->Print();
  //t->Scan();

  ////---5: write the tree to the root file
  t->Write("",TObject::kOverwrite);//// save only the new version of the tree
  //t->Write();//// save only the new version of the tree
  f->Close();

  delete f;
  f=NULL;

}



void add_branch_to_tree(string path_root_file,list<short int> &data_list,string tree_tag,string tree_name,string branch_tag,string leaf_list)
{
  TFile* f=new TFile(path_root_file.c_str(),"update");

  short int branch_value;

  TTree * t=(TTree*)f->Get(tree_tag.c_str());

  TBranch* branch=t->Branch(branch_tag.c_str(),&branch_value,leaf_list.c_str());

  for(list<short int>::iterator it = data_list.begin();
      it != data_list.end(); it++)
  {

    branch_value=*it;
    branch->Fill();
    //t->Fill();
  }


  t->Print();

  ////---5: write the tree to the root file
  t->Write("",TObject::kOverwrite);//// save only the new version of the tree
  f->Close();

  delete f;
  f=NULL;

}


void add_branch_to_tree(string path_root_file,list<char> &data_list,string tree_tag,string tree_name,string branch_tag,string leaf_list)
{
  TFile* f=new TFile(path_root_file.c_str(),"update");

  int branch_value;

  TTree * t=(TTree*)f->Get(tree_tag.c_str());

  TBranch* branch=t->Branch(branch_tag.c_str(),&branch_value,leaf_list.c_str());
  //TBranch* branch=t->Branch(branch_tag.c_str(),&branch_value,leaf_list.c_str());

  for(list<char>::iterator it = data_list.begin();
      it != data_list.end(); it++)
  {

    branch_value=(int)*it;
    
    branch->Fill();
    //t->Fill();
    //cout<<branch_value<<endl;

  }

  t->Print();
  //t->Scan();

  ////---5: write the tree to the root file
  t->Write("",TObject::kOverwrite);//// save only the new version of the tree
  f->Close();

  delete f;
  f=NULL;
}





void add_string_branch(string path_root_file,list<char> &data_list,string tree_tag,string tree_name,string branch_tag,string leaf_list)
{
  TFile* f=new TFile(path_root_file.c_str(),"update");

  
  //char* branch_value;
  string branch_value;

  TTree * t=(TTree*)f->Get(tree_tag.c_str());

  TBranch* branch=t->Branch(branch_tag.c_str(),&branch_value);
  //TBranch* branch=t->Branch(branch_tag.c_str(),&branch_value,leaf_list.c_str());

  for(list<char>::iterator it = data_list.begin();
      it != data_list.end(); it++)
  {

    if((int)*it==1)
    {
      branch_value="true";

    }
    else
    {
      branch_value="scatter";
    }
    
    branch->Fill();
    //t->Fill();
    //cout<<branch_value<<endl;
  }

  t->Print();
  //t->Scan();

  ////---5: write the tree to the root file
  t->Write("",TObject::kOverwrite);//// save only the new version of the tree
  f->Close();

  delete f;
  f=NULL;
}





int read_binary_data_to_list( list<int> &list_cry1,list<int> &list_cry2,list<short int> &list_cry1_ofov_scatter_z_pos , list<short int> &list_cry2_ofov_scatter_z_pos,list<char> &list_type)
{


  /*list<int> list_cry1;*/
  //list<int> list_cry2;
  //list<short int> list_cry1_ofov_scatter_z_pos ;
  //list<short int> list_cry1_ofov_scatter_z_pos;
  /*list<char> list_type;*/

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


    //read orderingly to the structure

    file.read((char* )(&coin.icry1), sizeof(int));
    file.read((char* )(&coin.icry2), sizeof(int));
    file.read((char* )(&coin.icry1_ofov_scatter_z_pos), sizeof(short int));
    file.read((char* )(&coin.icry2_ofov_scatter_z_pos), sizeof(short int));
    file.read((char* )(&coin.type), sizeof(char));

    // put the data to the list
    list_cry1.push_back(coin.icry1);
    list_cry2.push_back(coin.icry2);
    list_cry1_ofov_scatter_z_pos.push_back(coin.icry1_ofov_scatter_z_pos) ;
    list_cry2_ofov_scatter_z_pos.push_back(coin.icry2_ofov_scatter_z_pos);
    list_type.push_back(coin.type);



    cout<< setw(15) << coin.icry1
      << setw(15)  << coin.icry2
      << setw(15)  << coin.icry1_ofov_scatter_z_pos
      << setw(15)  << coin.icry2_ofov_scatter_z_pos
      << setw(15)  << (int)coin.type
      << endl;


  }

  file.close();

  return list_cry1.size();




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

