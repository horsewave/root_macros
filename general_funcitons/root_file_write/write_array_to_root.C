
void write_array_to_root()
{

  //create_root();
  //write_vector_to_tree();
  //get_vector_from_tree();
  
  //write_vector_to_root();
  //get_vector_from_root();
//write_2d_matrix_to_root();
get_2d_matrix_from_root();
}

void create_root()
{
  string path_root_file="./my_root_file.root";


  //TFile* f=new TFile(path_root_file.c_str(),"recreate");
  TFile* f=new TFile(path_root_file.c_str(),"update");


  string tree_tag="LOR_data";
  string tree_name="Axile distribution of lor events";



  int array_data[153];
  int array_size=153;

  for(int i=0;i<array_size;i++)
  {
    array_data[i]=i*2;

    cout<<array_data[i]<<endl;
  }

  //TTree* t=(TTree*)f->Get(tree_tag.c_str());
  TTree* t=new TTree(tree_tag.c_str(),tree_name.c_str());

  std::ostringstream os_int_to_str;
  os_int_to_str<<array_size; 
  string str_array_size(os_int_to_str.str());


  string leaf_list="array_data["+str_array_size+"]/I";

  t->Branch("z_lor_events",array_data,leaf_list.c_str());

  t->Fill(); 
  t->Write("",TObject::kOverwrite);;//// save only the new version of the tree
  //f->Close();

  delete t;
  t=NULL;

  delete f;
  f=NULL;

 


}

void write_vector_to_tree()
{
  string path_root_file="./my_root_file.root";


  TFile* f=new TFile(path_root_file.c_str(),"update");


  string tree_tag="LOR_data";
  string tree_name="Axile distribution of lor events";


  //1: declare a TVector with is the child of TObject.
  int array_size=153;
  TVector* v=new TVector(153);

  for(int i=0;i<array_size;i++)
  {
    //set value to the vector
    v(i) =i;
  }


  //2: Get your tree
  TTree* t=(TTree*)f->Get(tree_tag.c_str());

  TObjString* ts=new TObjString("The total axile distibution of the LOR data");

  //3: add to the tree
  t->GetUserInfo()->Add(ts);
  t->GetUserInfo()->Add(v);
  //4: write to the file.
  t->Write("",TObject::kOverwrite);//// save only the new version of the tree
  cout<<"write"<<endl;
  f->Close();

//delete ts;
  //ts=NULL;
  delete f;
  f=NULL;

  //delete v;
  //v=NULL;


}


void write_vector_to_root()
{
  string path_root_file="./my_root_file.root";


  TFile* f=new TFile(path_root_file.c_str(),"update");



  //1: declare a TVector with is the child of TObject.
  int array_size=153;
  TVector* v=new TVector(153);

  for(int i=0;i<array_size;i++)
  {
    //set value to the vector
    v(i) =i;
  }



  //TObjString* ts=new TObjString("The total axile distibution of the LOR data");
 
  //4: write to the file.
  v->Write("my_vector",TObject::kOverwrite);//// save only the new version of the tree
  cout<<"write"<<endl;
  f->Close();


  v->Print();

//delete ts;
  //ts=NULL;
  delete f;
  f=NULL;

  delete v;
  v=NULL;


}



void get_vector_from_tree()
{

  //1: open root file
  string path_root_file="./my_root_file.root";

  TFile* f=new TFile(path_root_file.c_str());

  string tree_tag="LOR_data";
  string tree_name="Axile distribution of lor events";

  //2: Get the tree 
  TTree* t=(TTree*)f->Get(tree_tag.c_str());

  //3: Define a vector to get the info
  TVector* v;
  v=(TVector*)(t->GetUserInfo()->FindObject("TVectorT<float>"));
  t->GetUserInfo()->Print();

  //v->Draw();

  //v->Print();


  f->Close();


}



void get_vector_from_root()
{

  //1: open root file
  string path_root_file="./my_root_file.root";

  TFile* f=new TFile(path_root_file.c_str());


  //3: Define a vector to get the info
  TVector* v=(TVector*)f->Get("my_vector");

  //v->Draw();

  v->Print();


  f->Close();


}


void write_2d_matrix_to_root()
{
  string path_root_file="./my_root_file.root";


  TFile* f=new TFile(path_root_file.c_str(),"update");



  //1: declare a TVector with is the child of TObject.
  int column=153;
  int row=5;
  TMatrix* mat=new TMatrix(row,column);

  for(int i=0;i<row;i++)
  {
    for(int j=0;j<column;j++)
    {

      mat(i,j)=i*j;

    }

  }
     
mat->Print();

 /* //TObjString* ts=new TObjString("The total axile distibution of the LOR data");*/
 
  //4: write to the file.
  mat->Write("my_2d_matrix",TObject::kOverwrite);//// save only the new version of the tree
  f->Close();


  delete f;
  f=NULL;


  delete mat;
  mat=NULL;

   


}


void write_2d_matrix_to_root(string path_root_file,string root_mem_name,TMatrix* mat,int row,int column )
{
  //string path_root_file="./my_root_file.root";

  TFile* f=new TFile(path_root_file.c_str(),"update");


  //mat->Print();

  /* //TObjString* ts=new TObjString("The total axile distibution of the LOR data");*/

  //4: write to the file.
  mat->Write(root_mem_name.c_str(),TObject::kOverwrite);//// save only the new version of the tree
  f->Close();
  delete f;
  f=NULL;
}






void get_2d_matrix_from_root( string path_root_file,string matrix_name)
{

  //1: open root file
  //string path_root_file="./my_root_file.root";

  TFile* f=new TFile(path_root_file.c_str());


  //3: Define a vector to get the info
  //TMatrix* mat=(TMatrix*)f->Get("my_2d_matrix");
  TMatrix* mat=(TMatrix*)f->Get(matrix_name.c_str());


  mat->Print();


  f->Close();


}


void test()
{
  int array_data[153];
  int array_size=153;
  TVector* v=new TVector(153);

  for(int i=0;i<array_size;i++)
  {
    array_data[i]=i*2;
    v(i) =array_data[i];

    //cout<<v(i)<<endl;
  }

  v->Print();
  //v->SetName("hello");
  cout<< v->GetName()<<endl;


}
