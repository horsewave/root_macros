void lesson_11_root_tree_analysis_TScan()
{
  part_4();
  }


void part_3()

{
  //--------------part 3 Scan---------------
  TFile* f=new TFile("staff.root");

  TTree* t;
  f->GetObject("T",t);


  //show only the first 8 field
  t->Scan();

  //show all the fields
  t->Scan("*");

  // about the number of entries on print
  t->SetScanField(3);
  t->Scan("*");

  // about the column width
  t->Scan("*","","colsize=20");
  t->Scan("*","","col=5:10:20");

  //about the precision
  t->Scan("*","","colsize=15 precision=4");
  t->Scan("*","","col=5.0F:10.0F:15.0F");//
  t->Scan("*","","col=5.1F:10.2F:15.3F");//5.1F: where 5 is the width, and 1 is the precision. F is the data format. 


  //about the range
  t->Scan("*","","col=5.0F:10.0F:15.0F",10,0);// 10 entries, start from index =0
  t->Scan("*","",10,5);// 10 entries, start from index =5


  // about math or invoking function
  //

  t->Scan("sqrt(staff.sta)","",10,5);// 10 entries, start from index =5
  t->Scan("log(staff.sta)","",10,5);// 10 entries, start from index =5
  //
  //
  //
  //
  //f->Print();
  //f->ls();


  f->Close();


}




/*
 *  special variables for array 
 *  entry$, Entries$, Length$, Iteration$
 *
 */
void part_4()
{
  // Let's create some data first
  TFile* f2 =new TFile("simple_array.root","recreate");

  TTree* t=new TTree("t","array");

  vector<Int_t> x;
  /*
   *  Think of it is a class from vector,
   *  with Int_t type specified in the template.
   */


  t->Branch("x",&x);

  x.push_back(10);
  x.push_back(13);
  x.push_back(14);
  x.push_back(11);

  t->Fill();
  x.clear();
  /*
   *  push_back(): to add elements;
   *  clear(): to clear all elenments;
   *  size() : to return number of elements;
   *  at(): get the element
   *  []
   *
   */

  x.push_back(110);
  x.push_back(132);
  x.push_back(144);
  x.push_back(115);

  t->Fill();
  x.clear();

  f2->Print();
  t->Scan();
  
  t->Write();

  t->Scan("x");
  t->Scan("x:Entry$:Entries$");
  /*
   *  Entry$ : Represents the current run number;
   *  Entries$ : Represents the total run number;
   *
   */

  t->Scan("x:Length$:Entry$");
  t->Scan("x:Length$:Iteration$");
  /*
   *  Length$: return the size of x, e.g: how many elements of the array.;
   *  Iteration$: return the current index.
   *
   */

  t->Scan("x:x[Iteration$+1]:x[Iteration$+2]");
  t->Scan("x:x[0]:x[Length$(x)-1]");

  /*
   *  x[i] : x[i+1] :x[i+2]
   *  x[i] : x[0] :x[end]
   *
   */


  f2->Close();




}
