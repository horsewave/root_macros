#include <vector>

typedef std::vector<int> int_vec_t; 
typedef std::vector<float> float_vec_t; 




void write_array_to_root()
{

  //create_root();
  //write_vector_to_tree();
  //get_vector_from_tree();

  //write_vector_to_root();
  //

  int z_ifov=153;
  int z_3_fov=460;
  float* v_ifov_scatter_sino=new float[z_ifov];
  float* v_ofov_source_scatter_sino=new float[z_3_fov];
  float* v_ifov_source_ofov_scatter_sino=new float[z_3_fov];

 memset(v_ifov_scatter_sino,0,sizeof(float)*z_ifov);
 memset(v_ofov_source_scatter_sino,0,sizeof(float)*z_3_fov);
 memset(v_ifov_source_ofov_scatter_sino,0,sizeof(float)*z_3_fov);


  string root_file_scatter="/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/cylinder_phantom_3_fov_20_cm/root_file/compare_MCS_SSS_scatter_sino.root";
  string root_file_coin="/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/cylinder_phantom_3_fov_20_cm/root_file/coincidence_root_file.root";


  string vector_name_ifov_scat="vector_events_per_z_mcs_ofov_scatter";
  string vector_name_ofov_source_scat="vector_events_per_z_mcs_ofov_source";
  string vector_name_ifov_source_ofov_scat="vector_events_per_z_mcs_ifov_source_ofov_scatter";

  get_vector_from_root(root_file_scatter ,vector_name_ifov_scat, v_ifov_scatter_sino);
  get_vector_from_root(root_file_coin ,vector_name_ofov_source_scat, v_ofov_source_scatter_sino);
  get_vector_from_root(root_file_coin ,vector_name_ifov_source_ofov_scat, v_ifov_source_ofov_scatter_sino);


  //resize it:
  int final_size=220;

float* ifov_scatter_sino=new float[final_size];
  float* ofov_source_scatter_sino=new float[final_size];
  float* ifov_source_ofov_scatter_sino=new float[final_size];

 memset(ifov_scatter_sino,0,sizeof(float)*final_size);
 memset(ofov_source_scatter_sino,0,sizeof(float)*final_size);
 memset(ifov_source_ofov_scatter_sino,0,sizeof(float)*final_size);

 for(int i=0;i<final_size;i++)
 {
   ifov_scatter_sino[i]=v_ifov_scatter_sino[i];
   ofov_source_scatter_sino[i]=v_ofov_source_scatter_sino[i];
   ifov_source_ofov_scatter_sino[i]=v_ifov_source_ofov_scatter_sino[i];

 }



Generate_save_mcs_sss_sino_plot(ifov_scatter_sino,  ofov_source_scatter_sino, ifov_source_ofov_scatter_sino,final_size,root_file_scatter);






  //get_vector_from_root();
  //write_2d_matrix_to_root();

  //get_2d_matrix_from_root();
  //
  delete [] v_ifov_scatter_sino;
  v_ifov_scatter_sino=NULL;

  delete [] v_ofov_source_scatter_sino;
  v_ofov_source_scatter_sino=NULL;

  delete [] v_ifov_source_ofov_scatter_sino;
  v_ifov_source_ofov_scatter_sino=NULL;


   delete [] ifov_scatter_sino;
    ifov_scatter_sino=NULL;

  delete [] ofov_source_scatter_sino;
  ofov_source_scatter_sino=NULL;

  delete [] ifov_source_ofov_scatter_sino;
  ifov_source_ofov_scatter_sino=NULL;



 
  gApplication->Terminate();


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



void get_vector_from_root(string path_root_file ,string vector_name,float* float_array)
{

  //1: open root file
  //string path_root_file="./my_root_file.root";

  TFile* f=new TFile(path_root_file.c_str());


  //3: Define a vector to get the info
  //TVector* v1=(TVector*)f->Get("my_vector");
  TVector* v1=(TVector*)f->Get(vector_name.c_str());

  //v->Draw();

  //v->Print();
  //
int array_size=v1->GetNrows();

cout<<"size is: "<<array_size<<endl;
for(int i=0;i<array_size;i++)
{
  float_array[i]=v1(i);
  //cout<<float_array[i]<<endl;
}


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









void Generate_save_mcs_sss_sino_plot(float*array_events_ifov_scat, float* array_events_ofov_source,float* array_events_ifov_source,int array_size,string saved_data_path)
{

  

  string canvas_name="canvas_scat_sino_z_events_ofov_scatter";
    //string canvas_name="canvas_scat_sino_z_events_MCS_SSS_compare_scaled_by_z_plane";
  string canvas_title="Scatter Sinogram Comparison for MCS and SSS _all_scatter_to_ofov_scatter";


  //string canvas_name="canvas_scat_sino_z_events_MCS_SSS_compare_scaled_slice_by_slice";
  //string canvas_title="Scatter Sinogram Comparison for MCS and SSS scaled_slice_by_slice";
  
  //string canvas_name="canvas_scat_sino_z_events_MCS_SSS_compare";
  //string canvas_title="Scatter Sinogram Comparison for MCS and SSS";

  TCanvas* can_graph=new TCanvas(canvas_name.c_str(),canvas_title.c_str(), 1600, 1000);
  can_graph->SetGrid();

  TGraph *gr_ifov_scatter = new TGraph (array_size);
  TGraph *gr_ofov_source = new TGraph (array_size);
  TGraph *gr_ifov_source_ofov_scat = new TGraph (array_size);


  GenerateGraphData(gr_ifov_scatter,array_events_ifov_scat);
  GenerateGraphData(gr_ofov_source,array_events_ofov_source);
  GenerateGraphData(gr_ifov_source_ofov_scat,array_events_ifov_source);

  Color_t color_1=kBlack;
  Color_t mcs_color=kBlue;
  Color_t sss_color=kRed;
  GraphAttributeSet(gr_ifov_scatter,kBlack);
  GraphAttributeSet(gr_ofov_source,kBlue);
  GraphAttributeSet(gr_ifov_source_ofov_scat,kRed);


  TMultiGraph *mg = new TMultiGraph();
  TLegend *legend = new TLegend(0.10,0.80,0.3,0.90);


  MultiGraphAttributeSet(mg,legend, gr_ifov_scatter,gr_ofov_source,gr_ifov_source_ofov_scat);


  //string string_total_counts_mcs = get_string_total_counts(array_events_per_z_mcs,array_size);
  //string string_total_counts_sss= get_string_total_counts( array_events_per_z_sss,array_size);
  //string string_difference = get_string_count_diff( array_events_per_z_mcs, array_events_per_z_sss,array_size);
  
  //int z_start=50;
  //int z_end=120;
  //string string_max_differ_mcs_sss = get_string_count_max_diff(array_events_per_z_mcs,array_events_per_z_sss,array_size,z_start,z_end);


  //string mcs_text="MCS counts: "+string_total_counts_mcs;
  //string sss_text="SSS counts: "+string_total_counts_sss;
  //string difference_text="Difference: "+string_difference+"%";

  //string string_max_sino="50-120";
  /*string max_difference_text="Differ("+string_max_sino+"): "+string_max_differ_mcs_sss+"%";*/



  //TPaveText *pt = new TPaveText(.1,.6,.3,.8,"brNDC");
 /* Gernerate_pave_text(pt, mcs_text.c_str(),mcs_color);*/
  //Gernerate_pave_text(pt, sss_text.c_str(),sss_color);
  //Gernerate_pave_text(pt, difference_text.c_str(),kBlack);
  /*//Gernerate_pave_text(pt, max_difference_text.c_str(),kBlack);*/

  gPad->Update();
  gPad->Modified();

  //save_image(can_graph,saved_data_path);

  save_canvas_to_root_file(can_graph, saved_data_path);
  //save_canvas_to_root_file(can_graph, saved_data_path,gr_ofov_source,gr_ifov_source_ofov_scat,mg);

 gSystem->ProcessEvents();

 if(legend!=NULL)
 {
   delete legend;
   legend=NULL;

 }


if(gr_ifov_source_ofov_scat!=NULL)
 {
   delete gr_ifov_source_ofov_scat;
   gr_ifov_source_ofov_scat=NULL;
 }

if(gr_ofov_source!=NULL)
 {
   delete gr_ofov_source;
   gr_ofov_source=NULL;

 }

if(mg!=NULL)
 {
   delete mg;
   mg=NULL;

 }

if(can_graph!=NULL)
 {
   delete can_graph;
   can_graph=NULL;

 }


  
}

void Gernerate_pave_text(TPaveText *pt,char* insert_text,Color_t text_color )
{
   //TPaveText *pt = new TPaveText(.05,.1,.25,.3);
   //TText *t1 = pt->AddText("F(t) = #sum_{i=-#infty}^{#infty}A(i)cos#[]{#frac{i}{t+i}}");
   //pt->AddLine(.0,.5,1.,.5);
   TText *t1 = pt->AddText(insert_text);
   t1->SetTextColor(text_color);
   pt->Draw();
   pt->SetFillColor(0);
   

}



void save_image( TCanvas* can, string saved_img_path)
{
   //this function is very important, otherwise,the picture can not be saved;
  gSystem->ProcessEvents();

  TImage *img = TImage::Create();

  /*string img_hist_name="./pictures/mcs_sss_hist_zSlice_"+IntToString(z_slice)+".png";*/
  cout<<"saved image path is :"<<saved_img_path<<endl;

  //img->FromPad(c, 10, 10, 300, 200);
  img->FromPad(can);

  img->WriteImage(saved_img_path.c_str());
  
  gSystem->ProcessEvents();
  
  delete img;


}
 


float_vec_t Get_axile_events_sino(string sino_data_path,bool is_float)
{

  
  cout<< sino_data_path<<endl;


  //plane numbers of the sino data
  int plane_num;

  //event counts of every plane;
  float* array_events_per_plane;

  //event counts of every Z position.
  float* array_events_per_z;
  int* array_planes_per_z;

  //the corresponding axial postion of every plane.
  int* array_axial_position_per_plane;
  //---sinogram object sructure
  int max_z=0;


  //class including all the basic information of the MRPET.
  MRPET_dump *dump = new MRPET_dump();	

  
  ///---measurement data, correction of random and norm//////////////////////////////////////////////////
  BrainPET_Sinograms* whole_sinogram=new BrainPET_Sinograms(dump,"whole_sinogram"); 

  cout<<"start to read  sino data from: "<<sino_data_path<<endl;

  whole_sinogram->read_flatSinogram(sino_data_path, is_float);
  cout<<"end of reading  sino data from: "<<sino_data_path<<endl;

  //Get plane number of a sino.
  plane_num=whole_sinogram->get_n_planes();

  array_events_per_plane=new float[plane_num];

  memset(array_events_per_plane,0,sizeof(float)*plane_num);

  array_axial_position_per_plane=new int[plane_num];
  memset(array_axial_position_per_plane,0,sizeof(int)*plane_num);

  for(int j=0;j<plane_num;j++)
  {
    //single sino plane
    BrainPET_SinogramData* single_sinogram=whole_sinogram->get_plane(j);

    //event counts of a sino plane.
    array_events_per_plane[j]=single_sinogram->get_n_Events();

    //get the corresponding axial position of this sino plane.
    array_axial_position_per_plane[j]=single_sinogram->getAxialPosition();

    //get the max axile value.It is integer
    if(max_z<array_axial_position_per_plane[j])
    {
      max_z=array_axial_position_per_plane[j];
    }

  }

  max_z+=1;//number of the z
  cout<<"the maximum z is: "<<max_z<<endl;

  array_events_per_z= new float[max_z];
 
  memset(array_events_per_z,0,sizeof(float)*max_z);
  //memset(array_events_per_z_sss,0,sizeof(float)*max_z);


  array_planes_per_z= new int[max_z];
  memset(array_planes_per_z,0,sizeof(int)*max_z);

  //get the summery counts for the corresponding axial position.
  for(int j=0;j<plane_num;j++)
  {
    int z=array_axial_position_per_plane[j];
    array_events_per_z[z]+=array_events_per_plane[j];
    array_planes_per_z[z]++;

  }

  cout<<"***************************************************"<<endl;
  cout<<"The plane number is: "<<plane_num<<endl;
  cout<<"The  axile value is: "<<max_z<<endl;
  cout<<"***************************************************"<<endl;

  cout<<"***************************************************"<<endl;
  cout<<"plane number per z"<<endl;
  cout<<"***************************************************"<<endl;

  int z_max_planes=0;
  for(int j=0;j<max_z;j++)
  {
    int plane_number= array_planes_per_z[j];
    if(z_max_planes<plane_number)
    {
      z_max_planes=plane_number;

    }
    cout<<"plane numbers for each z:[ "<<j<<" ]= "<< array_planes_per_z[j]<<endl;
    cout<<"max plane numbers : "<<z_max_planes<<endl;
  }

  //scaling
  for(int j=0;j<max_z;j++)
  {
    int plane_number= array_planes_per_z[j];
    float norm_factor=(float)z_max_planes/plane_number;
    //cout<<"event numbers for each z:[ "<<j<<" ]= "<<array_events_per_z[j]<<endl;
    array_events_per_z[j]=norm_factor*array_events_per_z[j];
    cout<<"event numbers for each z:[ "<<j<<" ]= "<<array_events_per_z[j]<<endl;

  }

//copy the data to a vector to return
float_vec_t vector_events_per_z(max_z);
 for(int j=0;j<max_z;j++)
  {
    vector_events_per_z[j]=array_events_per_z[j];

  }

  
  if(array_events_per_plane!=NULL)
  {
    delete [] array_events_per_plane;
    array_events_per_plane=NULL;

  }
  if(array_events_per_z!=NULL)
  {
    delete [] array_events_per_z;
    array_events_per_z=NULL;

  }
  if(array_planes_per_z!=NULL)
  {
    delete [] array_planes_per_z;
    array_planes_per_z=NULL;

  }

  if(array_axial_position_per_plane!=NULL)
  {
    delete [] array_axial_position_per_plane;
    array_axial_position_per_plane=NULL;

  }

  if(dump!=NULL)
  {
    delete dump;
    dump=NULL;
  }

if(whole_sinogram!=NULL)
  {
    delete whole_sinogram;
    whole_sinogram=NULL;
  }
  return vector_events_per_z;


}






//generate a plot using one slice of the image
void GenerateGraphData(TGraph *myGraph,float* imageData)
{
  int max_z=myGraph->GetN();
  cout<<"Graph N:"<<max_z<<endl;
  
	for(int x = 0;  x< max_z; x++)
  {
    //average values of y+-2,to smooth the plot.
    float event_count=imageData[x]; 

		myGraph->SetPoint(x,(double)x,(double)event_count);
  }

}

//set the plot attributes
void GraphAttributeSet(TGraph *myGraph,Color_t lcolor)
{
  myGraph->SetLineColor(lcolor);
	myGraph->SetLineWidth(6);


}

//put multi graph into on MultiGraph.
//position: to describe the y and z value of the image.
void MultiGraphAttributeSet(TMultiGraph *mg,TLegend *legend, TGraph *gr_ifov_scatter,TGraph *gr_ofov_source,TGraph *gr_ifov_source_ofov_scat)
{
  mg->Add(gr_ifov_scatter);	
  mg->Add(gr_ofov_source);	
  mg->Add(gr_ifov_source_ofov_scat);


  mg->Draw("AC");		

  mg->GetXaxis()->SetTitle("Axile sinograme number");
  mg->GetYaxis()->SetTitle("Event counts");
  mg->GetXaxis()->CenterTitle();
  mg->GetYaxis()->CenterTitle();	
  mg->Draw("AC");

  //****draw legend 
  legend->AddEntry(gr_ifov_scatter,"gr_ifov_scatter", "l");
  legend->AddEntry(gr_ofov_source,"gr_ofov_source", "l");
  //legend->AddEntry(gr_ofov_source,"MCS_96", "l");
  legend->AddEntry(gr_ifov_source_ofov_scat,"gr_ifov_source_ofov_scat","l");
  //legend->AddEntry(gr_ifov_source_ofov_scat,"MCS_24","l");
  //legend->AddEntry((TObject*)0, position, "");
  legend->Draw();

} 
	


string Float_to_string(float input_float,int precision_num)
{
  stringstream s_stream;
  s_stream << fixed << setprecision(precision_num) << input_float;
  string s = s_stream.str();

  return s;
}
string IntToString (long long a)
{
    ostringstream temp;
    temp<<a;
    return temp.str();
}



void write_vector_to_root(string path_root_file,float_vec_t vector_events_per_z ,string saved_vector_name)
{
  //string path_root_file="./my_root_file.root";


  TFile* f=new TFile(path_root_file.c_str(),"update");



  //1: declare a TVector with is the child of TObject.
  int array_size=vector_events_per_z.size();
  cout<<"the vector size is: "<< array_size<<endl;

  TVector* v=new TVector(array_size);

  for(int i=0;i<array_size;i++)
  {
    //set value to the vector
    v(i) =vector_events_per_z[i];
  }



  //TObjString* ts=new TObjString("The total axile distibution of the LOR data");
 
  //4: write to the file.
  v->Write(saved_vector_name.c_str(),TObject::kOverwrite);//// save only the new version of the tree
  
  f->Close();


  v->Print();

  delete f;
  f=NULL;

  delete v;
  v=NULL;


}

void write_vector_to_text(string saved_path_data, float_vec_t vector_events_per_z_mcs)
{
  ofstream of;
  of.open(saved_path_data.c_str(),std::ofstream::out);

  of<<setw(20)<<"ring index"<<setw(20)<<"Event counts"<<"\r"<<endl;

 int z_number=vector_events_per_z_mcs.size();

 for(int i=0;i<=z_number;i++)
 {
   of<<setw(20)<<i<<setw(20)<<vector_events_per_z_mcs[i]<<"\r"<<endl;

 }

of.close();


}


string get_string_total_counts(float* array_events_per_z,int array_size)
{
  
  double total_counts=0;


  for(int i=0;i<=array_size;i++)
  {

    total_counts+=array_events_per_z[i];
  }
  cout<<"the total counts  is: "<<total_counts<<endl;


  string string_total_counts=IntToString(total_counts);

  cout<<"the total counts  is: "<<string_total_counts<<endl;
  
  //string full_pave_text="MCS counts: "+string_total_counts;

  return string_total_counts;
}

string get_string_count_diff(float* array_events_per_z_mcs,float* array_events_per_z_sss,int array_size)
{
  

  
  double total_counts_mcs=0;

   //long long total_counts_sss=0;
   double total_counts_sss=0;

  for(int i=0;i<=array_size;i++)
  {

    total_counts_mcs+=array_events_per_z_mcs[i];
    total_counts_sss+=array_events_per_z_sss[i];
  }
  cout<<"the total counts of mcs scatter is: "<<total_counts_mcs<<endl;
  cout<<"the total counts of sss scatter is: "<<total_counts_sss<<endl;

  float difference_mcs_sss=fabs(total_counts_mcs-total_counts_sss)/total_counts_sss;

 cout<<"the difference is: "<<difference_mcs_sss<<endl;

  //change to percentage format
  difference_mcs_sss*=100;
  cout<<"the difference is: "<<difference_mcs_sss<<endl;


  int precision_num=2;
  string string_difference=Float_to_string(difference_mcs_sss,precision_num);

  cout<<"the difference is: "<<string_difference<<endl;


  return string_difference;


  
}

string get_string_count_max_diff(float* array_events_per_z_mcs,float* array_events_per_z_sss,int array_size,int z_start,int z_end)
{



  

  float mid_events_mcs=0.0;
  float mid_events_sss=0.0;
  float max_differ_mcs_sss=0.0;
  int max_sino=0;
  for(int i=z_start;i<=z_end;i++)
  {
      mid_events_mcs+=array_events_per_z_mcs[i];
      mid_events_sss+=array_events_per_z_sss[i];

  }

  max_differ_mcs_sss=abs(mid_events_mcs-mid_events_sss)/mid_events_sss;

  //change to percentage format
  max_differ_mcs_sss*=100;
  cout<<"the max difference is: "<<max_differ_mcs_sss<<endl;

  int precision_num=2;
  string string_max_differ_mcs_sss=Float_to_string(max_differ_mcs_sss,precision_num);


  return string_max_differ_mcs_sss;


  
}


void save_canvas_to_root_file(TCanvas *c1, string path_saved_root_file,TGraph *gr_ofov_source=NULL, TGraph *gr_ifov_source_ofov_scat=NULL, TMultiGraph *mg=NULL)
{

  TFile *f = TFile::Open(path_saved_root_file.c_str(), "update");
  f->ls();

  f->cd();

  c1->Write("",TObject::kOverwrite);

 //gr_ofov_source->Write("graph_mcs",TObject::kOverwrite);
 //gr_ifov_source_ofov_scat->Write("graph_sss",TObject::kOverwrite);
 /*mg->Write("multi_graph_mcs_sss",TObject::kOverwrite);*/




  f->Close();
 
}











