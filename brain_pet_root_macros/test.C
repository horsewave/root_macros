void test()
{

  TVector* v=new TVector(100);
  for(int i=0;i<100;i++)
  {
    v(i)=i;
  }


  v->Print();
  cout<<"vector size is: "<<v->GetNrows()<<endl;

}
