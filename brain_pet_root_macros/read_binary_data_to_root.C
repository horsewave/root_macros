/*
 * Description: stored the scatter coincedences from the binary file to root file for further processing.
 *               The binary file is acquired from the GPU-MCS,which is sorted with  different data types:
 *               int,int, short int,short int, char.
 *
 *                This macros is only suitable for the binary file with ofov information in it.
 *
 * Input: 
 * Functions:
// ----------------1: create the tree and read binary data-------------------------------
read_binary_data_to_root_tree(binary_file_path,saved_root_file_path,tree_tag,tree_title);

// ----------------2: if there are more binary data to read in the same tree, then add binary data-------------------------

//add_binary_data_to_existing_tree(binary_file_path,saved_root_file_path,tree_tag);


// ----------------4: draw the tree and save the canvas-------------------------
//tree_draw(saved_root_file_name,tree_tag);


// ----------------5: an alternative:save binary data to ntuple-------------------------
 *  read_binary_data_to_root_tntuple( binary_file_path,saved_root_file_name,ntuple_name,ntuple_title,var_list);
 *
 * Editor: Bo Ma
 *
 * Date: 2018.12.07
 *
 */

#include<stdio.h>
#include<iostream.h>
#include<list>
//#include<vector>

using namespace std;



// An unsigned char can store 1 Bytes (8bits) of data (0-255)
typedef unsigned char BYTE;

// define coincedences relating to the GPU-MCS data.
typedef struct {
  int icry1;
  int icry2;
  //short int icry1_ofov_scatter_z_pos;
  //short int icry2_ofov_scatter_z_pos;

  // type=1: true events,no scatter;
  // type=2: ofov source, scatter occur
  // type=3: ifov source,scatter occur in ofov,then back to be detected
  // type=4: normal scatter,e.g: source ifov, scatter ifov. 
  // type=5: random events
  char type;
  double time1;
  double time2;
} Coincidence;


//void read_binary_data_to_root(string binary_file_path,string saved_root_file_path)
void read_binary_data_to_root()
{



  string base_folder="/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/phantom_gate_mcs_tof_comp/";

  string binary_file_path= base_folder + "scatterMCS/cylinder_gpuSimu_1.bin"; 

  string saved_root_file_path=base_folder+"root_file/tof_mcs.root";

  string tree_tag="Coincidences";
  string tree_title="coincidences from GPU MCS";



  // ----------------1: create the tree and read binary data-------------------------------
  //read_binary_data_to_root_tree(binary_file_path,saved_root_file_path,tree_tag,tree_title);
  read_binary_data_to_root_tree_tof(binary_file_path,saved_root_file_path,tree_tag,tree_title);

  // ----------------2: if there are more binary data to read in the same tree, then add binary data-------------------------

  //binary_file_path= base_folder + "scatterMCS/XB1BN305N-BI-01_gpuSimu_1.bin"; 
  //add_binary_data_to_existing_tree(binary_file_path,saved_root_file_path,tree_tag);


  // ----------------3: draw the tree and save the canvas-------------------------
  //tree_draw(saved_root_file_path,tree_tag);


  // ----------------4: save all the branches to vector for further processing.------------------------

//save_all_branch_to_vector(saved_root_file_path);





  // ----------------5: an alternative:save binary data to ntuple-------------------------
  /* ntuple

     string ntuple_name="Coincidence_ntuple";
     string ntuple_title="gpu_mcs_coincidence_data";
     string var_list="cry1:cry2:cry1_ofov_scatter_z_pos:cry2_ofov_scatter_z_pos:coin_type";

     string binary_file_path = "./cylinder_phantom_3_fov_20_cm_gpuSimu_0.bin"; 
  //
  //
  // complicated way to read the data;
  //read_binary_data_to_root_complicated_method()

  // easier way using TNtuple
  //read_binary_data_to_root_tntuple( binary_file_path,saved_root_file_name,ntuple_name,ntuple_title,var_list);
  //
  //

*/
  gApplication->Terminate();

}



// save all the branches to vector for further processing.
void save_all_branch_to_vector(string root_file_name)
{
   //string root_file_name="/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/XB1BN305N-BI/XB1BN305N-BI-01/root_file/coincidence_root_file.root";

  string tree_tag="coincidence_scatter";

  int z_number=460;

  string branch_name="cry1_ofov_scatter_z_pos";
  string saved_vector_name="vector_" + branch_name+"_type_2" ;
  short int coin_type = 2; 

  save_branch_to_vector(root_file_name,tree_tag,branch_name, saved_vector_name, z_number,coin_type );


  branch_name="cry1_ofov_scatter_z_pos";
  saved_vector_name="vector_" + branch_name +"_type_3";
  coin_type = 3; 

  save_branch_to_vector(root_file_name,tree_tag,branch_name, saved_vector_name, z_number,coin_type );

  branch_name="cry2_ofov_scatter_z_pos";
  saved_vector_name="vector_" + branch_name +"_type_3";
  coin_type = 3; 

  save_branch_to_vector(root_file_name,tree_tag,branch_name, saved_vector_name, z_number,coin_type );


  branch_name="cry1_ofov_scatter_z_pos";
  saved_vector_name="vector_" + branch_name +"_type_4";
  coin_type = 4; 

  save_branch_to_vector(root_file_name,tree_tag,branch_name, saved_vector_name, z_number,coin_type );




}



void save_branch_to_vector(string root_file_name,string tree_tag,string branch_name, string saved_vector_name, int vector_size,short int coin_type )
{

  TFile *root_file=new TFile(root_file_name.c_str(), "update");

  TTree *t= (TTree*)root_file->Get(tree_tag.c_str());

  TVector* my_vector = new TVector(vector_size) ;

  short int cry_ofov_scatter_z_pos;

  // type=1: true events,no scatter;
  // type=2: ofov source, scatter occur
  // type=3: ifov source,scatter occur in ofov,then back to be detected
  // type=4: normal scatter,e.g: source ifov, scatter ifov. 
  // type=5: random events
  short int coincidence_type;

  t->SetBranchAddress(branch_name.c_str(),&cry_ofov_scatter_z_pos);
  t->SetBranchAddress("coincidence_type",&coincidence_type);

  Int_t nentries = t->GetEntries();

  for (Int_t i=0; i<nentries;i++) 
  {
    t->GetEntry(i);	

    if(coincidence_type==coin_type)
    {

      my_vector(cry_ofov_scatter_z_pos)++;

    }


  }

  my_vector->Print();
  // write the vector to file
  my_vector->Write(saved_vector_name.c_str(),TObject::kOverwrite);


  root_file->Close();


}



void read_binary_data_to_root_tree_tof(string binary_file_path,string saved_root_file_name,string tree_tag,string tree_title)
{

  Coincidence coin;


  string branch_tag_cry1="cry1";
  string leaf_list_cry1="cry1/I";

  string branch_tag_cry2="cry2";
  string leaf_list_cry2="cry2/I";

  string branch_tag_type="coincidence_type";
  string leaf_list_type="coincedence_type/S";

  string branch_tag_time1="time1";
  string leaf_list_time1="time1/D";
 
  string branch_tag_time2="time2";
  string leaf_list_time2="time2/D";


  // char can not store correctly in branch, so need to change it to short int 
  // but when automatically convert char to  short int ,there maybe problems ,sometimes
  // the value is wrong. so it is better to convert the char to int.
  short int branch_value_type;
  //int branch_value_type;

  TTree* t=new TTree(tree_tag.c_str(),tree_title.c_str());
  t->Branch(branch_tag_cry1.c_str(),&coin.icry1,leaf_list_cry1.c_str());
  t->Branch(branch_tag_cry2.c_str(),&coin.icry2,leaf_list_cry2.c_str());
  t->Branch(branch_tag_type.c_str(),&branch_value_type,leaf_list_type.c_str());
  t->Branch(branch_tag_time1.c_str(),&coin.time1,leaf_list_time1.c_str());
  t->Branch(branch_tag_time2.c_str(),&coin.time2,leaf_list_time2.c_str());


  //string binary_file_path = "./cylinder_phantom_3_fov_20_cm_gpuSimu_0.bin"; 
  fstream file;
  file.open(binary_file_path.c_str(), ios::in | ios::out | ios::binary);
  //file.open(filePath, fstream::in | fstream::out | fstream::binary);

  if (file.fail())
  {
    cout << "ERROR: Cannot open the file..." << endl;
    return 0;
  }

  int count_i=0;
  while(!file.eof())
  {


    //read orderingly to the structure
    file.read((char* )(&coin.icry1), sizeof(int));
    file.read((char* )(&coin.icry2), sizeof(int));
    file.read((char* )(&coin.type), sizeof(char));
    file.read((char* )(&coin.time1), sizeof(double));
    file.read((char* )(&coin.time2), sizeof(double));

    branch_value_type=(short)coin.type;

    //cout<< setw(15) << coin.icry1
    //<< setw(15)  << coin.icry2
    //<< setw(15)  << coin.icry1_ofov_scatter_z_pos
    //<< setw(15)  << coin.icry2_ofov_scatter_z_pos
    //<< setw(15)  << (int)coin.type
    /*<< endl;*/

    // save only scatter events, without true and random

      t->Fill();
    count_i++;
    cout<<"count:"<<count_i<<"  time1: "<<coin.time1<<"  time2: "<<coin.time2<<" time diff: "<<(coin.time1-coin.time2)<<endl;

    


  }

  file.close();


  TFile* f=new TFile(saved_root_file_name.c_str(),"update");
  //TFile* f=new TFile(saved_root_file_name.c_str());

  t->Write();//// save only the new version of the tree
  //t->Write("",TObject::kOverwrite);
  //t->Write("",TObject::kSingleKey);

  //t->Write("",TObject::kWriteDelete );

  f->Close();


  delete f;
  f=NULL;

  delete t;
  t=NULL;

}





void read_binary_data_to_root_tree(string binary_file_path,string saved_root_file_name,string tree_tag,string tree_title)
{

  Coincidence coin;


  string branch_tag_cry1="cry1";
  string leaf_list_cry1="cry1/I";

  string branch_tag_cry2="cry2";
  string leaf_list_cry2="cry2/I";

  string branch_tag_cry1_ofov_scatter_z_pos="cry1_ofov_scatter_z_pos";
  string leaf_list_cry1_ofov_scatter_z_pos="cry1_ofov_scatter_z_pos/S";

  string branch_tag_cry2_ofov_scatter_z_pos="cry2_ofov_scatter_z_pos";
  string leaf_list_cry2_ofov_scatter_z_pos="cry2_ofov_scatter_z_pos/S";

  string branch_tag_type="coincidence_type";
  string leaf_list_type="coincedence_type/S";


  // char can not store correctly in branch, so need to change it to short int 
  // but when automatically convert char to  short int ,there maybe problems ,sometimes
  // the value is wrong. so it is better to convert the char to int.
  short int branch_value_type;
  //int branch_value_type;

  TTree* t=new TTree(tree_tag.c_str(),tree_title.c_str());
  t->Branch(branch_tag_cry1.c_str(),&coin.icry1,leaf_list_cry1.c_str());
  t->Branch(branch_tag_cry2.c_str(),&coin.icry2,leaf_list_cry2.c_str());
  t->Branch(branch_tag_cry1_ofov_scatter_z_pos.c_str(),&coin.icry1_ofov_scatter_z_pos,leaf_list_cry1_ofov_scatter_z_pos.c_str());
  t->Branch(branch_tag_cry2_ofov_scatter_z_pos.c_str(),&coin.icry2_ofov_scatter_z_pos,leaf_list_cry2_ofov_scatter_z_pos.c_str());
  t->Branch(branch_tag_type.c_str(),&branch_value_type,leaf_list_type.c_str());


  //string binary_file_path = "./cylinder_phantom_3_fov_20_cm_gpuSimu_0.bin"; 
  fstream file;
  file.open(binary_file_path.c_str(), ios::in | ios::out | ios::binary);
  //file.open(filePath, fstream::in | fstream::out | fstream::binary);

  if (file.fail())
  {
    cout << "ERROR: Cannot open the file..." << endl;
    return 0;
  }

  while(!file.eof())
  {


    //read orderingly to the structure
    file.read((char* )(&coin.icry1), sizeof(int));
    file.read((char* )(&coin.icry2), sizeof(int));
    file.read((char* )(&coin.icry1_ofov_scatter_z_pos), sizeof(short int));
    file.read((char* )(&coin.icry2_ofov_scatter_z_pos), sizeof(short int));
    file.read((char* )(&coin.type), sizeof(char));

    branch_value_type=(short)coin.type;

    //cout<< setw(15) << coin.icry1
    //<< setw(15)  << coin.icry2
    //<< setw(15)  << coin.icry1_ofov_scatter_z_pos
    //<< setw(15)  << coin.icry2_ofov_scatter_z_pos
    //<< setw(15)  << (int)coin.type
    /*<< endl;*/

    // save only scatter events, without true and random
    if(branch_value_type>1 && branch_value_type<5)
    {

      t->Fill();

    }

  }

  file.close();


  TFile* f=new TFile(saved_root_file_name.c_str(),"update");

  t->Write();//// save only the new version of the tree
  //t->Write("",TObject::kOverwrite);
  //t->Write("",TObject::kSingleKey);

  //t->Write("",TObject::kWriteDelete );

  f->Close();


  delete f;
  f=NULL;

  delete t;
  t=NULL;

}







void add_binary_data_to_existing_tree(string binary_file_path,string saved_root_file_name,string tree_tag)
{

  Coincidence coin;

  TFile* f=new TFile(saved_root_file_name.c_str(),"update");
  if (!f) return;
  //TTree* t;
  //(TTree*)f->GetObject(tree_tag.c_str(),t);
  TTree* t=(TTree*)f->Get(tree_tag.c_str());

  string branch_tag_cry1="cry1";

  string branch_tag_cry2="cry2";

  string branch_tag_cry1_ofov_scatter_z_pos="cry1_ofov_scatter_z_pos";

  string branch_tag_cry2_ofov_scatter_z_pos="cry2_ofov_scatter_z_pos";

  string branch_tag_type="coincidence_type";


  // char can not store correctly in branch, so need to change it to short int 
  // but when automatically convert char to  short int ,there maybe problems ,sometimes
  // the value is wrong. so it is better to convert the char to int.
  short int branch_value_type;
  //int branch_value_type;

  t->SetBranchAddress(branch_tag_cry1.c_str(),&coin.icry1);
  t->SetBranchAddress(branch_tag_cry2.c_str(),&coin.icry2);
  t->SetBranchAddress(branch_tag_cry1_ofov_scatter_z_pos.c_str(),&coin.icry1_ofov_scatter_z_pos);
  t->SetBranchAddress(branch_tag_cry2_ofov_scatter_z_pos.c_str(),&coin.icry2_ofov_scatter_z_pos);
  t->SetBranchAddress(branch_tag_type.c_str(),&branch_value_type);


  //string binary_file_path = "./cylinder_phantom_3_fov_20_cm_gpuSimu_0.bin"; 

  fstream file;
  file.open(binary_file_path.c_str(), ios::in | ios::out | ios::binary);
  //file.open(filePath, fstream::in | fstream::out | fstream::binary);

  if (file.fail())
  {
    cout << "ERROR: Cannot open the file..." << endl;
    return 0;
  }

  while(!file.eof())
  {


    //read orderingly to the structure

    file.read((char* )(&coin.icry1), sizeof(int));
    file.read((char* )(&coin.icry2), sizeof(int));
    file.read((char* )(&coin.icry1_ofov_scatter_z_pos), sizeof(short int));
    file.read((char* )(&coin.icry2_ofov_scatter_z_pos), sizeof(short int));
    file.read((char* )(&coin.type), sizeof(char));

    branch_value_type=(short)coin.type;

    // only save scatter events,e.g:
    //branch_value_type=2,3,or 4
    if(branch_value_type>1 && branch_value_type<5)
    {

      t->Fill();

    }

  }

  file.close();

  //t->Write();//// save only the new version of the tree
  //t->Write("",TObject::kOverwrite);
  //t->Write("",TObject::kSingleKey);
  t->Write("",TObject::kWriteDelete );

  f->Close();

  delete f;
  f=NULL;

}



// if stored to tntuple, the file is smaller then 
// save it to tree. So it is better to save it to tntuple.
void read_binary_data_to_root_tntuple( string binary_file_path,string saved_root_file_name,string ntuple_name,string ntuple_title,string var_list)
//void read_binary_data_to_root_tntuple()
{

  TNtuple *ntuple = new TNtuple(ntuple_name.c_str(),ntuple_title.c_str(),var_list.c_str());

  Coincidence coin;

  int struct_size=(int)sizeof(coin);

  //const char *binary_file_path = "./cylinder_phantom_3_fov_20_cm_gpuSimu_0.bin"; 

  fstream file;
  file.open(binary_file_path.c_str(), ios::in | ios::out | ios::binary);
  //file.open(filePath, fstream::in | fstream::out | fstream::binary);

  if (file.fail())
  {
    cout << "ERROR: Cannot open the file..." << endl;
    return 0;
  }

  while(!file.eof())
  {


    //read orderingly to the structure

    file.read((char* )(&coin.icry1), sizeof(int));
    file.read((char* )(&coin.icry2), sizeof(int));
    file.read((char* )(&coin.icry1_ofov_scatter_z_pos), sizeof(short int));
    file.read((char* )(&coin.icry2_ofov_scatter_z_pos), sizeof(short int));
    file.read((char* )(&coin.type), sizeof(char));


    ntuple->Fill(coin.icry1,coin.icry2,coin.icry1_ofov_scatter_z_pos,coin.icry2_ofov_scatter_z_pos,coin.type);

    /*cout<< setw(15) << coin.icry1*/
    //<< setw(15)  << coin.icry2
    //<< setw(15)  << coin.icry1_ofov_scatter_z_pos
    //<< setw(15)  << coin.icry2_ofov_scatter_z_pos
    //<< setw(15)  << (int)coin.type
    /*<< endl;*/

  }

  file.close();

  TFile *f = new TFile(saved_root_file_name.c_str(),"update"); 
  if (!f) return;
  //ntuple->Write();
  ntuple->Write("",TObject::kOverwrite);
  f->Close();


}



// Draw and save the tree histogram
// Promble: 1: I can not set the name of the pad;
//  2: when saving the pad, it is not shown in the root file.
void tree_draw(string saved_root_file_name,string tree_tag)
{


  TFile* f=new TFile(saved_root_file_name.c_str(),"update");
  if (!f) return;
  //TTree* t;
  //(TTree*)f->GetObject(tree_tag.c_str(),t);
  TTree* t=(TTree*)f->Get(tree_tag.c_str());

  string branch_tag_cry1="cry1";

  string branch_tag_cry2="cry2";

  string branch_tag_cry1_ofov_scatter_z_pos="cry1_ofov_scatter_z_pos";

  string branch_tag_cry2_ofov_scatter_z_pos="cry2_ofov_scatter_z_pos";

  string branch_tag_type="coincidence_type";


  string canvas_name ="canvas_ofov_scatter_events" ;
  string canvas_title = "Axial distribution of OFOV scattering events ";

  TCanvas *c1 = new TCanvas(canvas_name.c_str(),canvas_title.c_str(),200,10,700,780);

  string pad1_name= "pad_ofov_source";
  string pad1_title= " source in OFOV, detected by the scanner ";

  string pad2_name= "pad_ifov_source_ofov_scattered";
  string pad2_title= " Photon generated in FOV, but scatter happens OFOV";
  string pad3_name= "pad_detected_scatter_ifov";
  string pad3_title= " scatter events detected in the scanner  ";
  string pad4_name= "pad_all_scatter";
  string pad4_title= " all scatters comparison ";



  TPad *pad1 = new TPad(pad1_name.c_str(),pad1_title.c_str(),0.02,0.52,0.48,0.98,21);
  TPad *pad2 = new TPad(pad2_name.c_str(),pad2_title.c_str(),0.52,0.52,0.98,0.98,21);
  TPad *pad3 = new TPad(pad3_name.c_str(),pad3_title.c_str(),0.02,0.02,0.48,0.48,21);
  TPad *pad4 = new TPad(pad4_name.c_str(),pad4_title.c_str(),0.52,0.02,0.98,0.48,1);
  pad1->Draw();
  pad2->Draw();
  pad3->Draw();
  pad4->Draw();
  //
  // Change default style for the statistics box
  gStyle->SetStatW(0.30);
  gStyle->SetStatH(0.20);
  gStyle->SetStatColor(42);



  // ----------------------1: draw OFOV source scatter-----------------------
  pad1->cd();
  pad1->SetGrid();
  pad1->SetLogy();
  pad1->GetFrame()->SetFillColor(15);
  pad1->SetTitle(pad1_title.c_str());
  t->SetLineColor(2);
  t->SetLineWidth(4);
  //1001: solid
  t->SetFillStyle(1001);
  //t->SetFillColor(45);

  string str_cut=branch_tag_type+"==2";
  TCut cut1=str_cut.c_str();
  t->Draw(branch_tag_cry1_ofov_scatter_z_pos.c_str(),cut1,"C*");

  t->SetLineColor(4);
  t->SetLineWidth(2);
  // caution: there should not be spaces in the draw option,
  // it is very wierd. if you set "  same",it will do nothing, 
  // you have to remove the space "same"
  t->Draw(branch_tag_cry2_ofov_scatter_z_pos.c_str(),cut1,"same C*");

  pad1->RedrawAxis();


  // ----------------------2: draw IFOV source OFOV scatter-----------------------
   pad2->cd();
  pad2->SetGrid();
  pad2->SetLogy();
  pad2->GetFrame()->SetFillColor(32);
  t->SetLineColor(2);
  t->SetLineWidth(4);
  //1001: solid
  t->SetFillStyle(1001);
  //t->SetFillColor(45);

  str_cut=branch_tag_type+"==3";
  cut1=str_cut.c_str();
  //t->Draw(branch_tag_cry1_ofov_scatter_z_pos.c_str(),cut1,"same");
  t->Draw(branch_tag_cry1_ofov_scatter_z_pos.c_str(),cut1,"C*");

  t->SetLineColor(4);
  t->SetLineWidth(4);

  t->Draw(branch_tag_cry2_ofov_scatter_z_pos.c_str(),cut1,"same C*");

  pad2->RedrawAxis();



  // ----------------------3: draw detected scatter IFOV-----------------------
  pad3->cd();
  pad3->SetGrid();
  pad3->SetLogy();
  pad3->GetFrame()->SetFillColor(32);
  t->SetLineColor(2);
  t->SetLineWidth(4);
  //1001: solid
  t->SetFillStyle(1001);
  //t->SetFillColor(45);

  str_cut=branch_tag_type+"==4";
  cut1=str_cut.c_str();
  //t->Draw(branch_tag_cry1_ofov_scatter_z_pos.c_str(),cut1,"same");
  t->Draw(branch_tag_cry1_ofov_scatter_z_pos.c_str(),cut1,"C*");

   t->SetLineColor(4);
  t->SetLineWidth(4);

  t->Draw(branch_tag_cry2_ofov_scatter_z_pos.c_str(),cut1,"same C*");

  pad3->RedrawAxis();


  // ----------------------4: draw all -----------------------
  pad4->cd();
  pad4->SetGrid();
  pad4->SetLogy();
  pad4->GetFrame()->SetFillColor(32);
  t->SetLineColor(2);
  t->SetLineWidth(4);
  //1001: solid
  t->SetFillStyle(1001);
  //t->SetFillColor(45);

  //str_cut=branch_tag_type+"==3";
  str_cut=branch_tag_type+"==2";
  cut1=str_cut.c_str();

  t->Draw(branch_tag_cry1_ofov_scatter_z_pos.c_str(),cut1,"C*");

t->SetLineColor(4);
  t->SetLineWidth(4);

str_cut=branch_tag_type+"==3";
  cut1=str_cut.c_str();
t->Draw(branch_tag_cry1_ofov_scatter_z_pos.c_str(),cut1,"same C*");


t->SetLineColor(6);
  t->SetLineWidth(4);

str_cut=branch_tag_type+"==4";
  cut1=str_cut.c_str();
t->Draw(branch_tag_cry1_ofov_scatter_z_pos.c_str(),cut1,"same C*");


  pad4->RedrawAxis();




  c1->cd();
  c1->Update();
  gStyle->SetStatColor(2);


  //Save to root file
 /* pad1->Write("",TObject::kOverwrite);*/
  //pad2->Write("",TObject::kOverwrite);
  //pad3->Write("",TObject::kOverwrite);
  //pad4->Write("",TObject::kOverwrite);
c1->Write("",TObject::kOverwrite);
//f->Write("",TObject::kOverwrite);


  f->Close();

}




