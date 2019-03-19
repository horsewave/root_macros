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
     * 4: float_vec_t Get_radial_events_sino(string sino_data_path,bool is_float)
     * 5: float_vec_t vector_events_radial(max_z);
     * 6: void GenerateGraphData(TGraph *myGraph,float* imageData)
     * 7: void GraphAttributeSet(TGraph *myGraph,Color_t lcolor)
     * 8: void MultiGraphAttributeSet(TMultiGraph *mg,TLegend *legend, TGraph *gr_mcs,TGraph *gr_sss)
     * 9: string Float_to_string(float input_float,int precision_num)
     * 10: string IntToString (long long a)
     * 11: void write_vector_to_root(string path_root_file,float_vec_t vector_events_radial ,string saved_vector_name)
     * 12: void write_vector_to_text(string saved_path_data, float_vec_t vector_events_radial_mcs)
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
void compare_MCS_SSS_sino_radial()
{

 gROOT->Reset();


/////////////running without input parameters//////////////////////////////////////////////////

  string base_folder="/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/XB1BN304N-BI/30min-data_0-30/";
  //string part_name_mcs="scatterMCS/scatter_multi_bad_plane_cor.fs";
  string part_name_mcs="scatterMCS/scatter_multi_bad_plane_cor_scaled.fs";
  //string part_name_sss="scatterMCS/scatter_multi_bad_plane_cor.fs";
  string part_name_sss="scatterMCS/scatter_multi_subtract.fs";
  //string part_name_sss="scatterSSS/scater_bad_plane_cor.fs";

  
  string part_root_file_name="root_file/test.root";

  string path_sino_mcs = base_folder + part_name_mcs;
  string path_sino_sss = base_folder + part_name_sss;
 string saved_path_root_file =  base_folder + part_root_file_name;

 cout<< saved_path_root_file<<endl;
/////////////running without input parameters//////////////////////////////////////////////////

  cout<< path_sino_mcs<<endl;
  //cout<< path_sino_sss<<endl;


  //--------------------1: get the axial data-------------------------------


  bool is_short=false;
  float_vec_t vector_events_radial_mcs=Get_radial_events_sino(path_sino_mcs);
  float_vec_t vector_events_radial_sss=Get_radial_events_sino(path_sino_sss);
 
  //--------------------1.1: write the vector data to root file-------------------------------

  /*string saved_vector_name_mcs="vector_events_radial_mcs";*/
  /*string saved_vector_name_sss="vector_events_radial_sss";*/

for(int i=0;i<=vector_events_radial_mcs.size();i++)
  {
    if(vector_events_radial_mcs[i]==0)
    {
      double random_temp= 0.2*((double) rand()/(RAND_MAX));
      //vector_events_radial_mcs[i]=vector_events_radial_sss[i]-vector_events_radial_sss[i]*random_temp;
      //vector_events_radial_mcs[i]=vector_events_radial_sss[i]-vector_events_radial_sss[i]*0.83;

      //cout<<"view : "<<i<< "is: "<<vector_events_radial_mcs[i]<<endl;
      //cout<<"view : "<<i<< " is: "<<random_temp<<endl;
    }
    //vector_events_radial_mcs[i];
    //array_events_per_z_sss[i]=vector_events_radial_sss[i];
   
  }

  string saved_vector_name_mcs="vector_events_radial_mcs";
  string saved_vector_name_sss="vector_events_radial_sss";
  write_vector_to_root(saved_path_root_file,vector_events_radial_mcs,saved_vector_name_mcs);
  write_vector_to_root(saved_path_root_file,vector_events_radial_sss,saved_vector_name_sss);


  //--------------------1.2: write the vector data to text file-------------------------------

 //write_vector_to_text(saved_path_data, vector_events_radial_mcs); 

  cout<<"the vector size is:"<<vector_events_radial_mcs.size()<<endl;
  cout<<"the vector size is:"<<vector_events_radial_sss.size()<<endl;

  if(vector_events_radial_mcs.size()!=vector_events_radial_sss.size())
  {
    cout<<"Errer: mcs and sss sinos have different z numbers!"<<endl;
    return;
  }


  //event counts of every Z position.
  int view_num=vector_events_radial_mcs.size();
  float* array_events_per_z_mcs= new float[view_num];
  float* array_events_per_z_sss= new float[view_num];
  //float* array_events_differ= new float[view_num];
  memset(array_events_per_z_mcs,0,sizeof(float)*view_num);
  memset(array_events_per_z_sss,0,sizeof(float)*view_num);
  //memset(array_events_differ,0,sizeof(float)*view_num);

 for(int i=0;i<=view_num;i++)
  {
    array_events_per_z_mcs[i]=vector_events_radial_mcs[i];
    array_events_per_z_sss[i]=vector_events_radial_sss[i];
   
  }



  //--------------------2: draw the plot and save it to root file -------------------------------
  ////start  canvas  for plot
    
//string saved_plot_path="./pictures/MCS_SSS_comparison_sino_"+frame_time+".png";
Generate_save_mcs_sss_sino_plot(array_events_per_z_mcs, array_events_per_z_sss,view_num,saved_path_root_file);

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

  string canvas_name="canvas_scat_sino_radial_MCS_SSS_compare";
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
gPad->WaitPrimitive();
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
 


float_vec_t Get_radial_events_sino(string path_sino_input)
{

  //string path_sino = "/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/cylinder_phantom_norm_file_mcs/test.fs" ;

 string path_sino_gaps = "/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/cylinder_phantom_norm_file_mcs/common_sino_files/gaps_sino.fs" ;

  int dimx=256;
  int dimy=192;
  int dimz=1399;
  int nVoxels= dimx* dimy* dimz;

  float* sino_data=new float[nVoxels];
  memset(sino_data, 0, sizeof(float)*nVoxels);

  float* sino_data_gaps=new float[nVoxels];
  memset(sino_data_gaps, 0, sizeof(float)*nVoxels);


  read_sino_flat(path_sino_input, sino_data, dimx, dimy,  dimz);
  read_sino_flat(path_sino_gaps, sino_data_gaps, dimx, dimy,  dimz);

  double* radial_sino=new double[dimx];
  memset(radial_sino, 0, sizeof(double)*dimx);

  double* radial_gap_ratio=new double[dimx];
  memset(radial_gap_ratio, 0, sizeof(double)*dimx);


  get_sino_radial_gaps_ratio(sino_data_gaps, radial_gap_ratio);

  get_sino_radial_pro( sino_data, radial_sino);
  scale_radial_gaps(radial_sino, radial_gap_ratio);


  for(int i=0;i<dimx;i++)
  {
    cout<<"view i: "<<i <<" : "<<radial_sino[i]<<endl;

  }

 

//copy the data to a vector to return
float_vec_t vector_events_radial(dimx);
 for(int j=0;j<dimx;j++)
  {
    vector_events_radial[j]=radial_sino[j];
  }

 


  if(sino_data!=NULL)
  {
    delete [] sino_data;
    sino_data=NULL;

  }

  if(radial_sino!=NULL)
  {
    delete [] radial_sino;
    radial_sino=NULL;

  }



  if(sino_data_gaps!=NULL)
  {
    delete [] sino_data_gaps;
    sino_data_gaps=NULL;

  }

  if(radial_gap_ratio!=NULL)
  {
    delete [] radial_gap_ratio;
    radial_gap_ratio=NULL;

  }


 return vector_events_radial;


 }

void read_sino_flat(string path_sino, float* sino_data, int dimx=256, int dimy=192, int dimz=1399)
{
  int wordlength=sizeof(float);	
  //read original image to memory
  ifstream fin;
  fin.open(path_sino.c_str());

  cout<< "path image: "<< path_sino<<endl;
  if(fin.good()){
    cout<<"Reading emission file from "<<path_sino.c_str()<<endl;
    fin.read((char *)sino_data, (dimx* dimy* dimz* wordlength));
    fin.close();

  }
  else
  {
    cout<<"Error opening emission file "<<path_sino.c_str()<<endl;	
    exit (EXIT_FAILURE);
    fin.close();
  }




}

void get_sino_radial_pro(float* sino_data, double* radial_sino,int dimx=256, int dimy=192, int dimz=1399)
{
   
  for(int z=0;z<dimz;z++)
  {
    int temp_z_index=z*dimy*dimx;
    for (int y=0;y<dimy;y++)
    {
      int temp_y_index=y*dimx;
      for(int x=0;x<dimx;x++)
      {
        int i_sino=temp_z_index+temp_y_index+x;
       radial_sino[x] += sino_data[i_sino];
      }


    }
  }

}




void get_sino_radial_gaps_ratio(float* sino_gaps_bad_plane_cor, double* radial_gap_ratio,int dimx=256, int dimy=192, int dimz=1399)
{
  int z=700;


  int temp_z_index=z*dimy*dimx;
  for (int y=0;y<dimy;y++)
  {
    int temp_y_index=y*dimx;
    for(int x=0;x<dimx;x++)
    {
      int i_sino=temp_z_index+temp_y_index+x;
      radial_gap_ratio[x] += sino_gaps_bad_plane_cor[i_sino];
    }


  }

  int n_projs_per_view= dimy;
  for(int x=0;x<dimx;x++)
  {
    int non_gap_num=n_projs_per_view-radial_gap_ratio[x];

    if(non_gap_num!=0)
    {

      //radial_gap_ratio[x]=double(n_projs_per_view)/double(non_gap_num)
      radial_gap_ratio[x]=double(n_projs_per_view)/double(non_gap_num);
    }
    else
    {
      radial_gap_ratio[x]=1;

    }


    //cout<< "view:" <<x<<" ratio: " <<radial_gap_ratio[x]<<endl;


  }



}

void scale_radial_gaps(double* radial_sino, double* radial_gap_ratio, int dimx=256)
{
  for(int x=0;x<dimx;x++)
  {

    radial_sino[x]*= radial_gap_ratio[x];

    //cout<< "view:" <<x<<" ratio: " <<radial_sino[x]<<endl;


  }




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



void write_vector_to_root(string path_root_file,float_vec_t vector_events_radial ,string saved_vector_name)
{

  TFile* f=new TFile(path_root_file.c_str(),"update");



  //1: declare a TVector with is the child of TObject.
  int array_size=vector_events_radial.size();
  cout<<"the vector size is: "<< array_size<<endl;

  TVector* v=new TVector(array_size);

  for(int i=0;i<array_size;i++)
  {
    //set value to the vector
    v(i) =vector_events_radial[i];
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

void write_vector_to_text(string saved_path_data, float_vec_t vector_events_radial_mcs)
{
  ofstream of;
  of.open(saved_path_data.c_str(),std::ofstream::out);

  of<<setw(20)<<"view index"<<setw(20)<<"Event counts"<<"\r"<<endl;

 int view_num=vector_events_radial_mcs.size();

 for(int i=0;i<=view_num;i++)
 {
   of<<setw(20)<<i<<setw(20)<<vector_events_radial_mcs[i]<<"\r"<<endl;

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










