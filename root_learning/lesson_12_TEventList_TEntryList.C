/*
 *  In this tutorial,
 *  we will explore the TEventList, TEntryList
 *
 */

void lesson_12_TEventList_TEntryList()
{
 
  /*
   * review: setup selection
   *
   *   t->Draw("money");
   *   t->Draw("money","money<2500");
   *
   *   t->Scan("money","","col=15.0F",10,0);
   *   t->Scan("money","money<3000","col=15.0F",10,0);
   */
   
  /*   The problem is:
   *   the selection may take long time, especially when 
   *   you have a complicated selection rule.
   *   and each time, Draw(),Scan() will take long time.
   *
   *   we can make a list that store the sorted data,
   *   and then just plot the list,
   *   hence we just need to sort once.
   *
   */

  /*
   *  you can use:
   *  TEventList, TEventList 
   *  their operations are very similar
   *  let's start from TEventList
   *
   *
   */


  event_list();



}


//TEventList
//
void event_list()
{
   TFile* f=new TFile("./staff.root");

  TTree* t;
  f->GetObject("T",t);



  //t->SetScanField(3);
  t->Print();
  //t->ls();
  //t->Scan();


  //t->Draw(">>myEvent");
  //the defaut type of myEvent is TEventList
  t->Draw(">>myEvent","money<2500");
  /*
   *   using Draw() method to store selectedd data,
   *   to generate the TEventList
   *   >> redirect to TEventList obj "myEvent",
   *   >>+ redirect to the TEventList obj "myEvent" in append mode
   *
   *   you can specify the range as well.
   *   t->Draw(">>myEvent","money<3000","",100,0);
   *
   *
   */

  TEventList* myEvent;
  gDirectory->GetObject("myEvent",myEvent);

  t->SetEventList(myEvent);
  /*
   *  use TTree::SetEventList() method,
   *  to restrict the tree within the selection rule,
   *  from the "myEvent"
   *
   *
   */

  t->Draw("money");
  t->Scan("money","","col=15.0F",10,0);

  for(Int_t i=0;i<10;i++)
  {
    cout<<i<<"\t"<<myEvent->GetEntry(i)<<endl;
  }
  /*
   *  TEventList::GetEntry() is different form 
   *  TTree:GetEntry(), it only return the index with which
   *  the entries satisfy the selection rules.
   *
   */

  //looping
  Int_t n_list=myEvent->GetN();//total run in myEvent
  Float_t money;
  t->SetBranchAddress("money",&money);

  Long64_t index_good;
  /*
   *  in case we have a long run
   *  we set up Long64_t type,
   *  below we show 10 runs ,for full run, use i<n_list;
   *
   */

  for(Int_t i=0;i<n_list;i++)
  {
    index_good = myEvent->GetEntry(i);
    t->GetEntry(index_good);

    cout<<"money = "<<fixed<<setprecision(0)<<money<<endl;

    t->SetEventList(0);//to reset.
  }






  f->Close();


}


//TEntryList :use considerably less memory for storage 
void entry_list()
{

   TFile* f=new TFile("./staff.root");

  TTree* t;
  f->GetObject("T",t);

  t->Draw(">>myEntry","money<2500","entrylist");
  /*
   * using Draw() method, and add "entrylist" flag.
   */

  TEntryList* myEntry;
  gDirectory->GetObject("myEntry",myEntry);

  t->SetEntryList(myEntry);

  t->Draw("money");
  t->Draw("money","","col=15.0F",10,0);

  for(Int_t i=0;i<10;i++)
  {
    cout<<i<<"\t"<<myEntry<<GetEntry(i)<<endl;
  }

  //looping
  Int_t n_list=myEntry->GetN();
  for(Int_t i=0;i<n_list;i++)
  {
    int index_good=myEntry->GetEntry();
    t->GetEntry(index_good);
    cout<<"money = "<<fixed<<setprecision(0)<<money<<endl;
  }
  /*
   *  or Next() method,
   *  it is quicker than TEntryList::GetEntry() method;
   */

  int index_good=myEntry->GetEntry(0);// setup the starting point

  for(Int_t i=0;i<n_list;i++)
  {
    t->GetEntry(index_good);
    index_good=myEntry->Next();
    cout<<"money = "<<fixed<<setprecision(0)<<money<<endl;
  }

  TFile g("list.root","recreate");
  myEntry->Write();
  myEvent->Write();
  g->Close();

  f->Close();

}
