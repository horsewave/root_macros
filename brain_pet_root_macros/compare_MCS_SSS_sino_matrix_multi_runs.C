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
 * 17: void get_sino_matrix(float* sino_data, TMatrix* mat,int dimx=256, int dimy=192, int dimz=1399,int row, int column)
 * 18: float_vec_t Get_matrix_sino(string path_sino_input,TMatrix* mat)
 * 19: void draw_sino_matrix(TMatrix *mat, int row=256, int column=192)
 * 20:float_vec_t Get_proj_per_view_sino(string path_sino_input)





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
const int mcs_num=5;

//void compare_MCS_SSS_scatter_sino(string path_sino_mcs,string path_sino_sss, string saved_path_root_file)
//void compare_MCS_SSS_sino_matrix_multi_runs()
  void compare_MCS_SSS_sino_matrix_multi_runs(int view_index)
{


  string base_folder="/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/XB1BN310N-BI/XB1BN310N-BI-01/";
  //string part_name_mcs="scatterMCS/run_num_4000/error_norm/scater_bad_plane_cor.fs";
  //const int mcs_num=2;
  //2.4E9,1.2E10,3.6E10,6E10
  string part_name_mcs[mcs_num]={"scatterMCS/run_num_200/XB1BN310N-BI-01_0-1800_gpuSimu_scattersino_scaled.fs",
    "scatterMCS/run_num_600/XB1BN310N-BI-01_0-1800_gpuSimu_scattersino_scaled.fs",
    "scatterMCS/run_num_1000/XB1BN310N-BI-01_0-1800_gpuSimu_scattersino_scaled.fs",
    "scatterMCS/run_num_3000/XB1BN310N-BI-01_0-1800_gpuSimu_scattersino_scaled.fs",
    "scatterMCS/run_num_5000/XB1BN310N-BI-01_0-1800_gpuSimu_scattersino_scaled.fs"
  };

  string part_name_sss="scatterSSS/scater_bad_plane_cor.fs";
  string part_root_file_name="root_file/compare_MCS_SSS_scatter_sino_multi_run.root";
  string path_sino_mcs[mcs_num];
  for(int i=0;i<mcs_num;i++)
  {
    path_sino_mcs[i] = base_folder + part_name_mcs[i];
    cout<< path_sino_mcs[i]<<endl;

  }
  string path_sino_sss = base_folder + part_name_sss;
  string saved_path_root_file =  base_folder + part_root_file_name;

  cout<< saved_path_root_file<<endl;

  cout<< path_sino_mcs<<endl;
  //cout<< path_sino_sss<<endl;


  //--------------------1: get the axial data-------------------------------


  bool is_short=false;
  //int view_index=110;

  bool is_short=false;
  float_vec_t vector_events_radial_mcs[mcs_num];
  for(int i=0;i<mcs_num;i++)
  {
    vector_events_radial_mcs[i] = Get_proj_per_view_sino(path_sino_mcs[i],view_index);

  }

  float_vec_t vector_events_radial_sss=Get_proj_per_view_sino(path_sino_sss,view_index);

  cout<<" *** 1: get the proj per view finished"<<endl; 


  //--------------------1.1: write the vector data to root file-------------------------------

  /*string saved_vector_name_mcs="vector_events_radial_mcs";*/
  /*string saved_vector_name_sss="vector_events_radial_sss";*/
  int vector_size = vector_events_radial_mcs[0].size();

  cout<< "vector size is: "<< vector_size<<endl;

  /*for(int i=0;i<=vector_events_radial_mcs.size();i++)*/
  //{
  //if(vector_events_radial_mcs[i]==0)
  //{
  //double random_temp= 0.2*((double) rand()/(RAND_MAX));
  ////vector_events_radial_mcs[i]=vector_events_radial_sss[i]-vector_events_radial_sss[i]*random_temp;
  //vector_events_radial_mcs[i]=vector_events_radial_sss[i]-vector_events_radial_sss[i]*0.83;

  //}

  //}


  cout<<" *** 2: start to write to root file"<<endl; 
  string saved_vector_name_mcs[mcs_num]={"vector_events_radial_mcs_1",
    "vector_events_radial_mcs_2",
    "vector_events_radial_mcs_3",
    "vector_events_radial_mcs_4",
    "vector_events_radial_mcs_5"
  };
  string saved_vector_name_sss="vector_events_radial_sss";
  for(int i=0;i<mcs_num;i++)
  {
    write_vector_to_root(saved_path_root_file,vector_events_radial_mcs[i],saved_vector_name_mcs[i]);

  }

  write_vector_to_root(saved_path_root_file,vector_events_radial_sss,saved_vector_name_sss);


  //--------------------1.2: write the vector data to text file-------------------------------

  //write_vector_to_text(saved_path_data, vector_events_radial_mcs); 

  cout<<"the vector size is:"<<vector_events_radial_mcs.size()<<endl;
  cout<<"the vector size is:"<<vector_events_radial_sss.size()<<endl;

  if(vector_events_radial_mcs[0].size()!=vector_events_radial_sss.size())
  {
    cout<<"Errer: mcs and sss sinos have different z numbers!"<<endl;
    return;
  }


  //event counts of every Z position.
  int proj_num=vector_events_radial_mcs[0].size();
  float* array_events_per_z_mcs[mcs_num];

  for(int i=0;i<mcs_num;i++)
  {
    array_events_per_z_mcs[i]= new float[proj_num];
    memset(array_events_per_z_mcs[i],0,sizeof(float)*proj_num);
  }
  float* array_events_per_z_sss= new float[proj_num];
  memset(array_events_per_z_sss,0,sizeof(float)*proj_num);

  for(int j=0;j<mcs_num;j++)
  {
    for(int i=0;i<=proj_num;i++)
    {
      array_events_per_z_mcs[j][i]=vector_events_radial_mcs[j][i];

    }


  }
  for(int i=0;i<=proj_num;i++)
  {
    array_events_per_z_sss[i]=vector_events_radial_sss[i];

  }


  //--------------------2: draw the plot and save it to root file -------------------------------
  ////start  canvas  for plot
  string s_view_index=IntToString(view_index);
  saved_path_root_file="./pictures/MCS_SSS_comparison_sino_"+s_view_index+".png";
  Generate_save_mcs_sss_sino_plot(array_events_per_z_mcs, array_events_per_z_sss,proj_num,saved_path_root_file);

  for(int i=0;i<mcs_num;i++)
  {
    if(array_events_per_z_mcs[i]!=NULL)
    {
      delete [] array_events_per_z_mcs[i];
      array_events_per_z_mcs[i]=NULL;

    }

  }
  if(array_events_per_z_sss!=NULL)
  {
    delete [] array_events_per_z_sss;
    array_events_per_z_sss=NULL;

  }

  gApplication->Terminate();
}	




void Generate_save_mcs_sss_sino_plot(float*array_events_per_z_mcs[mcs_num], float* array_events_per_z_sss,int array_size,string saved_data_path)
{
  string canvas_name="canvas_scat_sino_radial_MCS_SSS_compare";
  string canvas_title="Scatter Sinogram Comparison for MCS and SSS";

  TCanvas* can_graph=new TCanvas(canvas_name.c_str(),canvas_title.c_str(), 1600, 1000);
  can_graph->SetGrid();

  TGraph *gr_mcs[mcs_num];
  for(int i=0;i<mcs_num;i++)
  {
    gr_mcs[i]=new TGraph (array_size);
  }

  TGraph *gr_sss = new TGraph (array_size);
  for(int i=0;i<mcs_num;i++)
  {
    GenerateGraphData(gr_mcs[i],array_events_per_z_mcs[i]);
  }
  cout<<"step 1 finishes"<<endl;

  GenerateGraphData(gr_sss,array_events_per_z_sss);
  cout<<"step 2 finishes"<<endl;

  Color_t mcs_color[mcs_num];
  for(int i=0;i<mcs_num;i++)
  {
    mcs_color[i]=i+1;
  }
  Color_t sss_color=1;

  for(int i=0;i<mcs_num;i++)
  {
    GraphAttributeSet(gr_mcs[i],mcs_color[i]);
  }

  cout<<"step 3 finishes"<<endl;

  GraphAttributeSet(gr_sss,sss_color);


  TMultiGraph *mg = new TMultiGraph();
  TLegend *legend = new TLegend(0.10,0.80,0.3,0.90);


  cout<<"step 4 finishes"<<endl;


  string legend_name[mcs_num];
  for(int i=0;i<mcs_num;i++)
  {
    int j=i+1;
    string i_string=IntToString(j);
    legend_name[i]="mcs_"+i_string;

  }

  cout<<"step 5 finishes"<<endl;
  for(int i=0;i<mcs_num;i++)
  {

    MultiGraphAttributeSet(mg,legend, gr_mcs[i],legend_name[i]);
  }

  //MultiGraphAttributeSet(mg,legend, gr_sss,"SSS");

  string string_total_counts_mcs[mcs_num]; 
  for(int i=0;i<mcs_num;i++)
  {

    string_total_counts_mcs[i] = get_string_total_counts(array_events_per_z_mcs[i],array_size);
  }

  cout<<"step 6 finishes"<<endl;
  string string_total_counts_sss= get_string_total_counts( array_events_per_z_sss,array_size);
  //string string_difference = get_string_count_diff( array_events_per_z_mcs, array_events_per_z_sss,array_size);

  int z_start=50;
  int z_end=120;
  //string string_max_differ_mcs_sss = get_string_count_max_diff(array_events_per_z_mcs,array_events_per_z_sss,array_size,z_start,z_end);


  string mcs_text[mcs_num];
  for(int i=0;i<mcs_num;i++)
  {

    mcs_text[i] = "MCS counts: "+string_total_counts_mcs[i];
  }


  string sss_text="SSS counts: "+string_total_counts_sss;
  //string difference_text="Difference: "+string_difference+"%";

  string string_max_sino="50-120";
  //string max_difference_text="Differ("+string_max_sino+"): "+string_max_differ_mcs_sss+"%";


  cout<<"step 7 finishes"<<endl;

  TPaveText *pt = new TPaveText(.1,.6,.3,.8,"brNDC");
  for(int i=0;i<mcs_num;i++)
  {

    Gernerate_pave_text(pt, mcs_text[i].c_str(),mcs_color[i]);
  }

  Gernerate_pave_text(pt, sss_text.c_str(),sss_color);
  //Gernerate_pave_text(pt, difference_text.c_str(),kBlack);
  //Gernerate_pave_text(pt, max_difference_text.c_str(),kBlack);

  gPad->Update();
  gPad->Modified();

  save_image(can_graph,saved_data_path);

  //save_canvas_to_root_file(can_graph, saved_data_path);
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

  for(int i=0;i<mcs_num;i++)
  {
    if(gr_mcs[i]!=NULL)
    {
      delete gr_mcs[i];
      gr_mcs[i]=NULL;

    }


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




float_vec_t Get_proj_per_view_sino(string path_sino_input,int view_index)
{

  //string path_sino = "/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/cylinder_phantom_norm_file_mcs/test.fs" ;

  string path_sino_gaps = "/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/cylinder_phantom_norm_file_mcs/common_sino_files/gaps_sino.fs" ;

  int dimx=256;
  int dimy=192;
  int dimz=1399;
  int nVoxels= dimx* dimy* dimz;

  float* sino_data=new float[nVoxels];
  memset(sino_data, 0, sizeof(float)*nVoxels);



  read_sino_flat(path_sino_input, sino_data, dimx, dimy,  dimz);



  TMatrix* mat= new TMatrix(dimx,dimy);

  get_sino_matrix(sino_data, mat);

  //draw the sino matrix
  //draw_sino_matrix(mat);

  //copy the data to a vector to return
  //

  int view_range=2;

  cout <<"view index is : " <<view_index<<endl;

  // get the proj of a range of view
  float_vec_t vector_events_radial(dimy);

  if(view_index>=view_range&&view_index<=(dimy-1-view_range))
  {
    for(int i=(view_index-view_range);i<=(view_index+view_range);i++)
    {
      for(int j=0;j<dimy;j++)
      {
        vector_events_radial[j]=mat(i,j);
        //cout<<"proj:"<<j<<" is: "<<mat(view_index,j)<<endl;;
      }

    }
  }

  // fill the gaps with the average 
  float sum_count_per_view=0;
  int no_gap_proj_num=0;
  for(int j=0;j<dimy;j++)
  {
    if(vector_events_radial[j]!=0)
    {
      no_gap_proj_num++; 

    }


    sum_count_per_view+=vector_events_radial[j];
  }
  int ave_count_per_view=sum_count_per_view/no_gap_proj_num;

  for(int j=0;j<dimy;j++)
  {
    if(vector_events_radial[j]==0)
    {
      vector_events_radial[j]=ave_count_per_view;

    }
  }




  //for(int j=0;j<dimy;j++)
  //{
  //vector_events_radial[j]=mat(view_index,j);
  ////cout<<"proj:"<<j<<" is: "<<mat(view_index,j)<<endl;;
  /*}*/


  if(sino_data!=NULL)
  {
    delete [] sino_data;
    sino_data=NULL;

  }


  if(mat!=NULL)
  {
    delete  mat;
    mat=NULL;

  }


  return vector_events_radial;


}


void draw_sino_matrix(TMatrix *mat, int row=256, int column=192)
{

  string canvasTitle="canvas_MCS_vs_SSS_img";

  TCanvas* can_hist=new TCanvas(canvasTitle.c_str(),canvasTitle.c_str(), 1000, 800);


  string Th_Title="hist_sino";
  TH2F *h2_sino = new TH2F(Th_Title.c_str(),Th_Title.c_str(), row, 0 , row, column, 0, column);
  for(int y = 0; y < column; y++)
  {

    for(int x= 0; x< row; x++)
    {
      int val = mat(x,y);
      h2_sino->SetBinContent(x+1,y+1,val);

    }	
  }

  h2_sino->GetXaxis()->SetTitle("x");
  h2_sino->GetYaxis()->SetTitle("y");

  h2_sino->Draw("colz");



  gPad->Update();

  gPad->WaitPrimitive();

  if(can_hist!=NULL)
  {
    delete can_hist;
    can_hist=NULL;
  }


  if(h2_sino!=NULL)
  {
    delete h2_sino;
    h2_sino=NULL;
  }




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


void get_sino_matrix(float* sino_data, TMatrix* mat,int dimx=256, int dimy=192, int dimz=1399,int row=256, int column=192)
{
  if(dimx!=row || dimy!=column)
  {
    cout<<"Error: matrix size if not right! "<<endl;
    return;
  }

  int z_start=0;
  int z_end=15;
  for(int z=z_start;z<z_end;z++)
  {
    int temp_z_index=z*dimy*dimx;
    for (int y=0;y<dimy;y++)
    {
      int temp_y_index=y*dimx;
      for(int x=0;x<dimx;x++)
      {
        int i_sino=temp_z_index+temp_y_index+x;
        mat(x,y)+= sino_data[i_sino];
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
void MultiGraphAttributeSet(TMultiGraph *mg,TLegend *legend, TGraph *gr,string legend_name)
{
  mg->Add(gr);	

  mg->Draw("AC");		

  mg->GetXaxis()->SetTitle("Axile sinograme number");
  mg->GetYaxis()->SetTitle("Event counts");
  mg->GetXaxis()->CenterTitle();
  mg->GetYaxis()->CenterTitle();	
  mg->Draw("AC");

  //****draw legend 
  legend->AddEntry(gr,legend_name.c_str(), "l");
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










