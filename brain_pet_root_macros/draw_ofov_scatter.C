/*
 * Description: This macro is used to get the axial distribution of the ofov scatter coincidences from the root file tree, and draw the OFOV source, ifov sourc ofov scatter and the scatter Coincidences only inside the fov.
 *
 *
 * Input: 
 * Procedure:
 *
            // -------------- 1: read the tree from the big raw data to a new small root file---------;
            //save_all_branch_to_vector(root_file_name_raw,root_file_name_saved);

             // -------------- 2: draw the vector-------------------------;
              draw_vector(root_file_name_saved);

 *
 *
 * Functions:
 *
 *         1: void draw_vector(string root_file_name)
 *         2: void Create_graph_from_vector( string saved_data_path, TVector* vector_ofov_source,TVector* vector_ifov_source_ofov_scatter,TVector* vector_ifov_scatter,bool is_whole_range=true,bool is_only_ofov=false)

 *         3: void GenerateGraphData(TGraph *myGraph,float* imageData)
 *         4: void GraphAttributeSet(TGraph *myGraph,Color_t lcolor)
 *         5: void Gernerate_pave_text(TPaveText *pt,char* insert_text,Color_t text_color )
 *         6: void MultiGraphAttributeSet(TMultiGraph *mg)
 *         7: void set_axis_range(TMultiGraph *mg,int i_start=0, int i_end=200)
 *         8: string Float_to_string(float input_float,int precision_num)
 *         9: void write_vector_to_root(string path_root_file,float_vec_t vector_events_per_z ,string saved_vector_name)
 *         10: void write_vector_to_text(string saved_path_data, float_vec_t vector_events_per_z_mcs)
 *         11: string get_string_total_counts(float* array_events_per_z,int array_size)
 *         12:string get_string_total_counts(TVector* array_events_per_z)
 *         13: string get_string_count_diff(float* array_events_per_z_mcs,float* array_events_per_z_sss,int array_size)
 *         14: string get_string_count_diff(TVector* array_events_per_z_ofov,TVector* array_events_per_z_total)
 *         15: string get_string_count_max_diff(float* array_events_per_z_mcs,float* array_events_per_z_sss,int array_size,int z_start,int z_end)
 *         16: void save_canvas_to_root_file(TCanvas *c1, string path_saved_root_file,TGraph *gr_mcs=NULL, TGraph *gr_sss=NULL, TMultiGraph *mg=NULL)

 *
 *
 *
 *
 * Editor: Bo Ma
 *
 * Date: 2018.12.07
 *
 */





#include <vector>

typedef std::vector<int> int_vec_t; 
typedef std::vector<float> float_vec_t; 


void draw_ofov_scatter()
{

 string root_file_name_raw="/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/XB1BN305N-BI/XB1BN305N-BI-01/root_file/coincidence_root_file.root";
 string root_file_name_saved="/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/XB1BN305N-BI/XB1BN305N-BI-01/root_file/ofov_scatter_root_file.root";


 // -------------- 1: read the tree from the big raw data to a new small root file;
 //save_all_branch_to_vector(root_file_name_raw,root_file_name_saved);

 // -------------- 2: draw the vector;
draw_vector(root_file_name_saved);
 
gApplication->Terminate();



}


// get vector from root file and draw 
void draw_vector(string root_file_name)
{


  string tree_tag="coincidence_scatter";


 TFile* f=new TFile(root_file_name.c_str());

 // ----------------1: Get the vector------------------------- 
string branch_name="cry1_ofov_scatter_z_pos";

string saved_vector_name= "vector_" + branch_name+"_type_2" ;

TVector* vector_ofov_source= (TVector*)f->Get(saved_vector_name.c_str());


  branch_name="cry1_ofov_scatter_z_pos";
  saved_vector_name="vector_" + branch_name +"_type_3";

TVector*  vector_ifov_source_ofov_scatter= (TVector*)f->Get(saved_vector_name.c_str());

branch_name="cry2_ofov_scatter_z_pos";
  saved_vector_name="vector_" + branch_name +"_type_3";

 TVector* vector_ifov_source_ofov_scatter_temp= (TVector*)f->Get(saved_vector_name.c_str());

 int z_number= vector_ifov_source_ofov_scatter_temp->GetNrows();

 for( int i=0;i<z_number;i++ )
 {
   vector_ifov_source_ofov_scatter(i)+=vector_ifov_source_ofov_scatter_temp(i);

 }

  branch_name="cry1_ofov_scatter_z_pos";
  saved_vector_name="vector_" + branch_name +"_type_4";

TVector* vector_ifov_scatter= (TVector*)f->Get(saved_vector_name.c_str());

//vector_ofov_source->Print();
//vector_ifov_source_ofov_scatter->Print();
//vector_ifov_scatter->Print();

 // ---------------- end 1: Get the vector------------------------- 
 
 // ---------------- 2: Draw------------------------- 

// don't draw the whole range
bool is_whole_range=false;// set if draw the whole z range[0 460]
bool is_only_ofov=true;// set if only draw the ofov scatter

Create_graph_from_vector(root_file_name,vector_ofov_source, vector_ifov_source_ofov_scatter, vector_ifov_scatter,is_whole_range,is_only_ofov);

 // ---------------- end 2: Draw------------------------- 




}


// save all the branches to vector for further processing.
void save_all_branch_to_vector(string root_file_name_raw,string root_file_name_saved)
{
   //string root_file_name="/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/XB1BN305N-BI/XB1BN305N-BI-01/root_file/coincidence_root_file.root";

  string tree_tag="coincidence_scatter";

  int z_number=460;

  string branch_name="cry1_ofov_scatter_z_pos";
  string saved_vector_name="vector_" + branch_name+"_type_2" ;
  short int coin_type = 2; 

  save_branch_to_vector(root_file_name_raw,root_file_name_saved,tree_tag,branch_name, saved_vector_name, z_number,coin_type );


  branch_name="cry1_ofov_scatter_z_pos";
  saved_vector_name="vector_" + branch_name +"_type_3";
  coin_type = 3; 

  save_branch_to_vector(root_file_name_raw,root_file_name_saved,tree_tag,branch_name, saved_vector_name, z_number,coin_type );

  branch_name="cry2_ofov_scatter_z_pos";
  saved_vector_name="vector_" + branch_name +"_type_3";
  coin_type = 3; 

  save_branch_to_vector(root_file_name_raw,root_file_name_saved,tree_tag,branch_name, saved_vector_name, z_number,coin_type );


  branch_name="cry1_ofov_scatter_z_pos";
  saved_vector_name="vector_" + branch_name +"_type_4";
  coin_type = 4; 

  save_branch_to_vector(root_file_name_raw,root_file_name_saved,tree_tag,branch_name, saved_vector_name, z_number,coin_type );


}



// save tree branch to vector.
void save_branch_to_vector(string root_file_name_raw,string root_file_name_saved,string tree_tag,string branch_name, string saved_vector_name, int vector_size,short int coin_type )
{

  TFile *root_file=new TFile(root_file_name_raw.c_str());

  TTree *t= (TTree*)root_file->Get(tree_tag.c_str());

  TVector* my_vector = new TVector(vector_size) ;

  short int cry_ofov_scatter_z_pos;

  // type=1: true events,no scatter;
  // type=2: ofov source, scatter occur
  // type=3: ifov source,scatter occur in ofov,then back to be detected
  // type=4: normal scatter,e.g: source ifov, scatter ifov. 
  // type=5: random events
  short int coincidence_type;

  t->SetBranchAddress(branch_name.c_str(),&cry_ofov_scatter_z_pos);
  t->SetBranchAddress("coincidence_type",&coincidence_type);

  Int_t nentries = t->GetEntries();

  for (Int_t i=0; i<nentries;i++) 
  {
    t->GetEntry(i);	

    if(coincidence_type==coin_type)
    {

      my_vector(cry_ofov_scatter_z_pos)++;

    }

  }

  root_file->Close();

TFile *root_file_saved=new TFile(root_file_name_saved.c_str(), "update");

  my_vector->Print();
  // write the vector to file
  my_vector->Write(saved_vector_name.c_str(),TObject::kOverwrite);
  root_file_saved->Close();




}
 

// create the MultiGraph from the input vector and save it to root file.
void Create_graph_from_vector( string saved_data_path, TVector* vector_ofov_source,TVector* vector_ifov_source_ofov_scatter,TVector* vector_ifov_scatter,bool is_whole_range=true,bool is_only_ofov=false)
{

  string canvas_name="canvas_scat_coincidence_ofov";
  string canvas_title="Scatter coincidences along the z coordinite";

  TCanvas* can_graph=new TCanvas(canvas_name.c_str(),canvas_title.c_str(), 1600, 1000);
  can_graph->SetGrid();


  TMultiGraph *mg = new TMultiGraph();
  TLegend *legend = new TLegend(0.10,0.80,0.3,0.90);
  TPaveText *pt = new TPaveText(.1,.6,.3,.8,"brNDC");
  
  int array_size=vector_ofov_source->GetNrows();

 
  //-----------1: ofov source---------------
  TGraph *gr_ofov_source = new TGraph (array_size);
  GenerateGraphData(gr_ofov_source,vector_ofov_source);
  Color_t color_ofov_source=kBlue;
  GraphAttributeSet(gr_ofov_source,color_ofov_source);

  mg->Add(gr_ofov_source);	

  legend->AddEntry(gr_ofov_source,"ofov_source", "l");

string string_total_counts_ofov_source = get_string_total_counts(vector_ofov_source);
  string ofov_source_text="ofov source : "+string_total_counts_ofov_source;
  Gernerate_pave_text(pt, ofov_source_text.c_str(),color_ofov_source);



  //-----------2: ifov source,ofov scatter---------------
  TGraph *gr_ifov_source_ofov_scatter = new TGraph (array_size);
  GenerateGraphData(gr_ifov_source_ofov_scatter,vector_ifov_source_ofov_scatter);
  Color_t color_ifov_source_ofov_scatter=kRed;
  GraphAttributeSet(gr_ifov_source_ofov_scatter,color_ifov_source_ofov_scatter);

  mg->Add(gr_ifov_source_ofov_scatter);	

  legend->AddEntry(gr_ifov_source_ofov_scatter,"ifov_source_ofov_scatter","l");

   string string_total_counts_ifov_source_ofov_scatter= get_string_total_counts( vector_ifov_source_ofov_scatter);
  string ifov_source_ofov_scatter_text="ifov_source_ofov_scatter: "+string_total_counts_ifov_source_ofov_scatter;
  Gernerate_pave_text(pt, ifov_source_ofov_scatter_text.c_str(),color_ifov_source_ofov_scatter);



  //-----------3: ifov source,ofov scatter---------------
  if(is_only_ofov==false)
  {

    TGraph *gr_ifov_scatter = new TGraph (array_size);

  GenerateGraphData(gr_ifov_scatter,vector_ifov_scatter);

  Color_t color_ifov_scatter=kBlack;
  GraphAttributeSet(gr_ifov_scatter,color_ifov_scatter);
  mg->Add(gr_ifov_scatter);	
  legend->AddEntry(gr_ifov_scatter,"ifov_scatter","l");

  string string_total_counts_ifov_scatter = get_string_total_counts(vector_ifov_scatter);
  string ifov_scatter_text="ifov_scatter: "+string_total_counts_ifov_scatter;
  Gernerate_pave_text(pt, ifov_scatter_text.c_str(),color_ifov_scatter);

  }
  
  MultiGraphAttributeSet(mg);

  //****draw legend 
  legend->Draw();



  //----------------------4: set the canvas name to save 
  
 if( is_only_ofov==false && is_whole_range==false )
 {

 canvas_name="canvas_scat_coincidence_ofov_small_range";

 int i_start=0;
    int i_end=200;
    set_axis_range(mg,i_start,i_end);
    

 }
 else if(is_only_ofov==true && is_whole_range==false)
 {
   int i_start=100;
    int i_end=200;
    set_axis_range(mg,i_start,i_end);


   canvas_name="canvas_scat_coincidence_only_ofov_small_range";


 }
 else if(is_only_ofov==true && is_whole_range==true)
 {
   canvas_name="canvas_scat_coincidence_only_ofov";

 }

 can_graph.SetName(canvas_name.c_str());

 
  gPad->Update();
  gPad->Modified();

  //save_image(can_graph,saved_data_path);


  //------------- 5: save the canvas-------------------------
  save_canvas_to_root_file(can_graph, saved_data_path);

 gSystem->ProcessEvents();

 if(legend!=NULL)
 {
   delete legend;
   legend=NULL;

 }


if(gr_ofov_source!=NULL)
 {
   delete gr_ofov_source;
   gr_ofov_source=NULL;
 }

if(gr_ifov_source_ofov_scatter!=NULL)
 {
   delete gr_ifov_source_ofov_scatter;
   gr_ifov_source_ofov_scatter=NULL;

 }

/*if(gr_ifov_scatter!=NULL)*/
 //{
   //delete gr_ifov_scatter;
   //gr_ifov_scatter=NULL;

 /*}*/

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




// 
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

void GenerateGraphData(TGraph *myGraph,TVector* imageData)
{
  int max_z=myGraph->GetN();
  cout<<"Graph N:"<<max_z<<endl;
  
	for(int x = 0;  x< max_z; x++)
  {
    //average values of y+-2,to smooth the plot.
    double event_count=(double)imageData(x); 

		myGraph->SetPoint(x,(double)x,event_count);
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
void MultiGraphAttributeSet(TMultiGraph *mg)
{

  mg->Draw("AC");		

  mg->GetXaxis()->SetTitle("Axile sinograme number");
  mg->GetYaxis()->SetTitle("Event counts");
  mg->GetXaxis()->CenterTitle();
  mg->GetYaxis()->CenterTitle();	
  mg->Draw("AC");

} 
	
void set_axis_range(TMultiGraph *mg,int i_start=0, int i_end=200)
{


  mg->GetXaxis()->SetRange(i_start,i_end);
  mg->Draw("AC");

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

string get_string_total_counts(TVector* array_events_per_z)
{
  
  double total_counts=0;

int array_size=array_events_per_z->GetNrows();
  for(int i=0;i<=array_size;i++)
  {

    total_counts+=array_events_per_z(i);
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



string get_string_count_diff(TVector* array_events_per_z_ofov,TVector* array_events_per_z_total)
{
  

  
  double total_counts_ofov=0;

   double total_counts_total=0;

   int array_size = array_events_per_z_ofov->GetNrows();
  for(int i=0;i<=array_size;i++)
  {

    total_counts_ofov+=array_events_per_z_ofov[i];
    total_counts_total+=array_events_per_z_total[i];
  }
  cout<<"the total counts of ofov scatter is: "<<total_counts_ofov<<endl;
  cout<<"the total counts of total scatter is: "<<total_counts_total<<endl;

  float difference_ofov_total=fabs(total_counts_ofov-total_counts_total)/total_counts_total;

  cout<<"the difference is: "<<difference_ofov_total<<endl;

  //change to percentage format
  difference_ofov_total*=100;
  cout<<"the difference is: "<<difference_ofov_total<<endl;


  int precision_num=2;
  string string_difference=Float_to_string(difference_ofov_total,precision_num);

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




