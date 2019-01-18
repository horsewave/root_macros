{
  gSystem->Load("./utilslib_cpp.so");

  double t=get_time();
  cout<<"time is :"<<t<<endl;

  char local_time[50]={0};

 Get_sys_time(local_time,50);

 cout<<"time is: "<<local_time<<endl;
 

}
