

void lesson_02_root_write_tree()
{
  //example_1();
  example_2();
  //example_3();

}

//file a tree with varialble
void example_1()
{

  //---1:create a tree 
  TTree t("t","a simple tree");
  //key=t;
  //title=a simple tree;
  //
  //---2: create variable;
  Int_t money_A, money_B,money_C;//set three tree branch.

  //----3: link the variables to the tree;
  //use Branch method, to set up a branch.
  //
  //Branch("key for the branch",address of variable,leaf/its datatype(I means int_t))
  //I:32 bit singed integer;
  //i:32 bit unsighned integer;
  //F:32 bit floating point;
  //D:64 bit floating point;
  t.Branch("mA",&money_A,"money_A/I");
  t.Branch("mB",&money_A,"money_B/I");
  t.Branch("mC",&money_A,"money_C/I");

  //4: file the tree
  //make an entry
  money_A=10;
  money_B=50;
  money_C=40;
  t.Fill();
//make more entries;
money_A=20;
  money_B=40;
  money_C=70;
  t.Fill();

  money_A=90;
  money_B=60;
  money_C=40;
  t.Fill();


  //t.ls();
  t.Print();
  
  //t.Scan();
//---5: write the tree to the root file
TFile* f=new TFile("example_1.root","recreate");
t.Write();
f->Close();

  
}


//file a tree with array;
void example_2()
{
  TTree t("t","a tree");

  /*
   *suppose :2 brothers ,A and B
   *A:have 2 bank accounts;
   *B: have 3;
   */
  Int_t A_account[2]={0 };
  Int_t B_account[3]={0 };
  /*
   *create a branch, and a leaf to strore the array;
   *a branch "A",linked to array A_account,
   *with one leaf "bank",data type "integer(I)"
   *
   *
   */

   int array_size=2;
std::ostringstream os_int_to_str;
    os_int_to_str<<array_size; 
    string str_array_size(os_int_to_str.str());

    string leaf_list="array_data["+str_array_size+"]/I";


  t.Branch("A",A_account,leaf_list.c_str());
  //t.Branch("A",A_account,"bank[2]/I");
  t.Branch("B",B_account,"bank[3]/I");

  //file the tree;
  A_account[0]=10;
  A_account[1]=20;

  B_account[0]=20;
  B_account[1]=30;
  B_account[2]=80;

  t.Fill();

  //file the tree;
  A_account[0]=100;
  A_account[1]=200;

  B_account[0]=510;
  B_account[1]=302;
  B_account[2]=800;

  t.Fill();


  t.Print();
  t.Scan();

   //t.Scan();
//---5: write the tree to the root file
TFile* f=new TFile("example_1.root","recreate");
t.Write();
f->Close();



}


//filling a tree with an array with varialble size

void example_3()
{
  TTree t("t","a Tree");

  /*
   *Declare an array, and set up the size;
   */

  const Int_t n_maxSize=100;
  Int_t energy[n_maxSize]={0};

  Int_t n;
  
  /*
   *! set up a branch to store "n"first;
   *then create a branch "energy",
   *and a leaf to store the array with 
   *its size linking to variable "n"
   *
   */

  t.Branch("Size",&n,"n/I");
  //n inside[n] is related to brance Size.
  t.Branch("Energy",energy,"particle_energy[n]/I");

  n=1;
  energy[0]=9;
  t.Fill();

  
  n=2;
  energy[0]=1;
  energy[1]=1;
  t.Fill();

  n=3;
  energy[0]=1;
  energy[1]=111;
  energy[2]=11;
  t.Fill();

  //t.Scan("n:particle_energy","","colsize=20");
  t.Scan("Size:Energy","","colsize=20");

  TFile f("example3.root","recreate");
  t.Write();
  f.Close();
}
