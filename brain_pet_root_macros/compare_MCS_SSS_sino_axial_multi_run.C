/***sino_compare_MCS_SSS_scatter_sino.C*
 ** Function: compare the defference of the scatter sinograms aquired by SSS and MCS;
	 * Editor: Bo Ma;
   * 
  
		 * Procedure: 1: Acquire the plane number of a flate format sinogram.(BrainPET_Sinograms)
     * *          2:  for(int i=0;i<plane_num;i++)
     *               {
     *                  1) get the plane pointer;
     *                  2) Get the total number of this plane;
     *                  3) get the axial Position of this plane;
     *                  4) put the events of this plane to the position of this plane;
     *
     *               }
 		 * 			      3: Draw the plot: X: axile values, Y: the number of events.
     *
     *
     * Functions:
     * 1: void Generate_save_mcs_sss_sino_plot(float*array_events_per_z_mcs, float* array_events_per_z_sss,int array_size,string saved_data_path)
     * 2: void Gernerate_pave_text(TPaveText *pt,char* insert_text,Color_t text_color )
     * 3: void save_image( TCanvas* can, string saved_img_path)
     * 4: float_vec_t Get_axile_events_sino(string sino_data_path,bool is_float)
     * 5: float_vec_t vector_events_per_z(max_z);
     * 6: void GenerateGraphData(TGraph *myGraph,float* imageData)
     * 7: void GraphAttributeSet(TGraph *myGraph,Color_t lcolor)
     * 8: void MultiGraphAttributeSet(TMultiGraph *mg,TLegend *legend, TGraph *gr_mcs,TGraph *gr_sss)
     * 9: string Float_to_string(float input_float,int precision_num)
     * 10: string IntToString (long long a)
     * 11: void write_vector_to_root(string path_root_file,float_vec_t vector_events_per_z ,string saved_vector_name)
     * 12: void write_vector_to_text(string saved_path_data, float_vec_t vector_events_per_z_mcs)
     * 13: string get_string_total_counts(float* array_events_per_z,int array_size)
     * 14: string get_string_count_diff(float* array_events_per_z_mcs,float* array_events_per_z_sss,int array_size)
     * 15:string get_string_count_max_diff(float* array_events_per_z_mcs,float* array_events_per_z_sss,int array_size,int z_start,int z_end)
     * 16: void save_canvas_to_root_file(TCanvas *c1, string path_saved_root_file,TGraph *gr_mcs=NULL, TGraph *gr_sss=NULL, TMultiGraph *mg=NULL)

     * Output:
     *  Save the distibution of scatter events along the z axial to root file 
     *  and also their plot.
     *  The root file name is: compare_MCS_SSS_scatter_sino.root
     *
  ** Date:2016.10.25.
	 * Version:1.0

   *Modified: Bo Ma
   * Add the Difference of the MCS and SSS using TPaveText
   * Add the function to save the image automatically.
   * using the function with parameters, it can be batch running in the shell script
   * Add the comparison of a defined region in the axial range,which is to get the largest difference in a defined region:max_differ_mcs_sss, especially for the FDG.

   *Date:2017.11.20
   *Version:2.0

	 * ****/

#include <vector>

typedef std::vector<int> int_vec_t; 
typedef std::vector<float> float_vec_t; 
//void compare_MCS_SSS_scatter_sino(string path_sino_mcs,string path_sino_sss, string saved_path_root_file)
void compare_MCS_SSS_sino_axial_seg_0()
{



/////////////running without input parameters//////////////////////////////////////////////////

   string base_folder="/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/XB1BN310N-BI/XB1BN310N-BI-01/";
  string part_name_mcs="scatterMCS/run_num_4000/error_norm/scater_bad_plane_cor.fs";
  string part_name_sss="scatterSSS/scater_bad_plane_cor.fs";
  
    
  string part_root_file_name="root_file/compare_MCS_SSS_scatter_sino_seg_0.root";

  string path_sino_mcs = base_folder + part_name_mcs;
  string path_sino_sss = base_folder + part_name_sss;
 string saved_path_root_file =  base_folder + part_root_file_name;

 cout<< saved_path_root_file<<endl;
/////////////running without input parameters//////////////////////////////////////////////////

  cout<< path_sino_mcs<<endl;
  //cout<< path_sino_sss<<endl;


  //--------------------1: get the axial data-------------------------------


  bool is_short=false;
  //float_vec_t vector_events_per_z_mcs=Get_axile_events_sino(path_sino_mcs,is_short);
  //float_vec_t vector_events_per_z_sss=Get_axile_events_sino(path_sino_sss,is_short);

  float_vec_t vector_events_per_z_mcs=Get_axile_events_sino_seg_0(path_sino_mcs,is_short);
  float_vec_t vector_events_per_z_sss=Get_axile_events_sino_seg_0(path_sino_sss,is_short);

 
  //--------------------1.1: write the vector data to root file-------------------------------

  /*string saved_vector_name_mcs="vector_events_per_z_mcs";*/
  /*string saved_vector_name_sss="vector_events_per_z_sss";*/


  string saved_vector_name_mcs="vector_events_per_z_mcs";
  string saved_vector_name_sss="vector_events_per_z_sss";
  write_vector_to_root(saved_path_root_file,vector_events_per_z_mcs,saved_vector_name_mcs);
  write_vector_to_root(saved_path_root_file,vector_events_per_z_sss,saved_vector_name_sss);


  //--------------------1.2: write the vector data to text file-------------------------------

 //write_vector_to_text(saved_path_data, vector_events_per_z_mcs); 

  cout<<"the vector size is:"<<vector_events_per_z_mcs.size()<<endl;
  cout<<"the vector size is:"<<vector_events_per_z_sss.size()<<endl;

  if(vector_events_per_z_mcs.size()!=vector_events_per_z_sss.size())
  {
    cout<<"Errer: mcs and sss sinos have different z numbers!"<<endl;
    return;
  }


  //event counts of every Z position.
  int z_number=vector_events_per_z_mcs.size();
  float* array_events_per_z_mcs= new float[z_number];
  float* array_events_per_z_sss= new float[z_number];
  //float* array_events_differ= new float[z_number];
  memset(array_events_per_z_mcs,0,sizeof(float)*z_number);
  memset(array_events_per_z_sss,0,sizeof(float)*z_number);
  //memset(array_events_differ,0,sizeof(float)*z_number);

 for(int i=0;i<=z_number;i++)
  {
    array_events_per_z_mcs[i]=vector_events_per_z_mcs[i];
    array_events_per_z_sss[i]=vector_events_per_z_sss[i];
   
  }



  //--------------------2: draw the plot and save it to root file -------------------------------
  ////start  canvas  for plot
    
//string saved_plot_path="./pictures/MCS_SSS_comparison_sino_"+frame_time+".png";
Generate_save_mcs_sss_sino_plot(array_events_per_z_mcs, array_events_per_z_sss,z_number,saved_path_root_file);

 if(array_events_per_z_mcs!=NULL)
  {
    delete [] array_events_per_z_mcs;
    array_events_per_z_mcs=NULL;

  }
 if(array_events_per_z_sss!=NULL)
  {
    delete [] array_events_per_z_sss;
    array_events_per_z_sss=NULL;

  }

  gApplication->Terminate();
}	




void Generate_save_mcs_sss_sino_plot(float*array_events_per_z_mcs, float* array_events_per_z_sss,int array_size,string saved_data_path)
{

  string canvas_name="canvas_scat_sino_z_events_MCS_SSS_compare";
  string canvas_title="Scatter Sinogram Comparison for MCS and SSS";

  TCanvas* can_graph=new TCanvas(canvas_name.c_str(),canvas_title.c_str(), 1600, 1000);
  can_graph->SetGrid();

  TGraph *gr_mcs = new TGraph (array_size);
  TGraph *gr_sss = new TGraph (array_size);


  GenerateGraphData(gr_mcs,array_events_per_z_mcs);
  GenerateGraphData(gr_sss,array_events_per_z_sss);

  Color_t mcs_color=kBlue;
  Color_t sss_color=kRed;
  GraphAttributeSet(gr_mcs,kBlue);
  GraphAttributeSet(gr_sss,kRed);


  TMultiGraph *mg = new TMultiGraph();
  TLegend *legend = new TLegend(0.10,0.80,0.3,0.90);


  MultiGraphAttributeSet(mg,legend, gr_mcs,gr_sss);


  string string_total_counts_mcs = get_string_total_counts(array_events_per_z_mcs,array_size);
  string string_total_counts_sss= get_string_total_counts( array_events_per_z_sss,array_size);
  string string_difference = get_string_count_diff( array_events_per_z_mcs, array_events_per_z_sss,array_size);
  
  int z_start=50;
  int z_end=120;
  string string_max_differ_mcs_sss = get_string_count_max_diff(array_events_per_z_mcs,array_events_per_z_sss,array_size,z_start,z_end);


  string mcs_text="MCS counts: "+string_total_counts_mcs;
  string sss_text="SSS counts: "+string_total_counts_sss;
  string difference_text="Difference: "+string_difference+"%";

  string string_max_sino="50-120";
  string max_difference_text="Differ("+string_max_sino+"): "+string_max_differ_mcs_sss+"%";



  TPaveText *pt = new TPaveText(.1,.6,.3,.8,"brNDC");
  Gernerate_pave_text(pt, mcs_text.c_str(),mcs_color);
  Gernerate_pave_text(pt, sss_text.c_str(),sss_color);
  Gernerate_pave_text(pt, difference_text.c_str(),kBlack);
  //Gernerate_pave_text(pt, max_difference_text.c_str(),kBlack);

  gPad->Update();
  gPad->Modified();

  //save_image(can_graph,saved_data_path);

  save_canvas_to_root_file(can_graph, saved_data_path);
  //save_canvas_to_root_file(can_graph, saved_data_path,gr_mcs,gr_sss,mg);

 gSystem->ProcessEvents();

 if(legend!=NULL)
 {
   delete legend;
   legend=NULL;

 }


if(gr_sss!=NULL)
 {
   delete gr_sss;
   gr_sss=NULL;
 }

if(gr_mcs!=NULL)
 {
   delete gr_mcs;
   gr_mcs=NULL;

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



float_vec_t Get_axile_events_sino_seg_0(string sino_data_path,bool is_float)
{

  
  cout<< sino_data_path<<endl;

  //event counts of every Z position.
  int dimz=153;
  float* array_events_per_z;
 array_events_per_z= new float[dimz];
 
  memset(array_events_per_z,0,sizeof(float)*dimz);

  //class including all the basic information of the MRPET.
  MRPET_dump *dump = new MRPET_dump();	

  ///---measurement data, correction of random and norm//////////////////////////////////////////////////
  BrainPET_Sinograms* whole_sinogram=new BrainPET_Sinograms(dump,"whole_sinogram"); 

  cout<<"start to read  sino data from: "<<sino_data_path<<endl;

  whole_sinogram->read_flatSinogram(sino_data_path, is_float);
  cout<<"end of reading  sino data from: "<<sino_data_path<<endl;


  for(int j=0;j<dimz;j++)
  {
    //single sino plane
    BrainPET_SinogramData* single_sinogram=whole_sinogram->get_plane(j);

    //event counts of a sino plane.
     array_events_per_z[j]=single_sinogram->get_n_Events();

  }


 
//copy the data to a vector to return
float_vec_t vector_events_per_z(dimz);
 for(int j=0;j<dimz;j++)
  {
    vector_events_per_z[j]=array_events_per_z[j];

  }

  
  if(array_events_per_z!=NULL)
  {
    delete [] array_events_per_z;
    array_events_per_z=NULL;

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
void MultiGraphAttributeSet(TMultiGraph *mg,TLegend *legend, TGraph *gr_mcs,TGraph *gr_sss)
{
  mg->Add(gr_mcs);	
  mg->Add(gr_sss);


  mg->Draw("AC");		

  mg->GetXaxis()->SetTitle("Axile sinograme number");
  mg->GetYaxis()->SetTitle("Event counts");
  mg->GetXaxis()->CenterTitle();
  mg->GetYaxis()->CenterTitle();	
  mg->Draw("AC");

  //****draw legend 
  legend->AddEntry(gr_mcs,"MCS", "l");
  //legend->AddEntry(gr_mcs,"MCS_96", "l");
  legend->AddEntry(gr_sss,"SSS","l");
  //legend->AddEntry(gr_sss,"MCS_24","l");
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


void save_canvas_to_root_file(TCanvas *c1, string path_saved_root_file,TGraph *gr_mcs=NULL, TGraph *gr_sss=NULL, TMultiGraph *mg=NULL)
{

  TFile *f = TFile::Open(path_saved_root_file.c_str(), "update");
  f->ls();

  f->cd();

  c1->Write("",TObject::kOverwrite);

 //gr_mcs->Write("graph_mcs",TObject::kOverwrite);
 //gr_sss->Write("graph_sss",TObject::kOverwrite);
 /*mg->Write("multi_graph_mcs_sss",TObject::kOverwrite);*/



  f->Close();
 
}










