
void write_array_to_root()
{

  create_root();
  write_vector();
  get_vector();

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

  delete v;
  v=NULL;



}

void write_vector()
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


void get_vector()
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
