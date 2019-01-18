/****Name:mcs_sss_plot.C
//
//Function: Generate 2D histograms of the defined z slice of the 3D reconstructed image and plot a profile of a defined range of the histogram.
//steps:
//
//Editor: Bo Ma
 *Time:2016.08.03
Version:1.0

 * Modified: Bo Ma
 * Time:2107.12.20
 * 1: Add the Total count number and the differences of rconstructed images generated with MCS and SSS on the TpaveText;
 * 2:Add the function to define the range of Y and Z direction to get the profile;
 * 3:Add the function to save the generated image.
 * 4: Add the function to scale the original image to 0-255;
 * 5: After the scaling of the image, a threshold value can be set to remove the background;
 * 6: Add the function to scale the image to the SUV with an input value of the total activity with Bq/cc;
 *
 * Version:2.0
 *
 * Modified: Bo Ma
 *
 *
 * Functions:
 *         1: void Draw_histgram_mcs_sss(string path_image_mcs,string path_image_sss,string saved_img_root_file,TH2F *h2_mcs,TH2F *h2_sss,int z_slice=70,int dimx=256,int dimy=256,int dimz=153, float spacing=1.25)
 *         2: float get_histgram_from_img(string path_image,TH2F *h2,int z_slice=70,int dimx=256,int dimy=256,int dimz=153, float spacing=1.25)
 *         3: void Draw_plot_mcs_sss(TH2F *h2_mcs,TH2F *h2_sss,string saved_root_file,int z_slice=70,int yValue=155;int dimx=256)
 *         4: void save_image( TCanvas* can, string saved_img_path)
 *         5: void save_TH2_to_root_file(TH2F *h2, string path_saved_root_file,string hist_name)
 *         6: void save_canvas_to_root_file(TCanvas *c1, string path_saved_root_file)
 *         7: void save_vector_to_root_file(TVector* my_vector, string path_saved_root_file,string vector_name)
 *         8: string IntToString (int a)
 *         9: void Read_image(string file_path,float* imageData,int voxel_num)
 *         10: float GenerateHistogram(TH2F* pHistgram, float * imageData, int z_slice,int zRange)
 *         11: float ScaleHistogram(TH2F* pHistgram, float Maximu_after_scale,float threshold_val)
 *         12: float SumHistogram(TH2F* pHistgram)
 *         13: void DrawHistgram(TCanvas* can_hist,TH2F* pHistgram,TPaletteAxis *palette, int subImage_indext)
 *         14: float GenerateGraphData(TGraph *myGraph,float* imageData,int yValue,int yRange, int z_slice,int zRange)
 *         15: float GenerateGraphData(TGraph *myGraph,TH2F* pHistgram,TVector* p_vector,int yValue,int yRange)
 *         16: void GraphAttributeSet(TGraph *myGraph,Color_t lcolor)
 *         17: void MultiGraphAttributeSet(TMultiGraph *mg)
 *         18: void Gernerate_pave_text(TPaveText *pt,char* insert_text,Color_t text_color )
 *         19: string Float_to_string(float input_float,int precision_num)
 *         20: void SUV_convert(float* input_image,float SUV_factor,int z_min=20,int z_max=149,int dimx=256,int dimy=256,int dimz=153)
 *         21: float Get_max_image(float* input_image,int dimx=256,int dimy=256,int dimz=153)
 *         22: float Get_min_image(float* input_image,int dimx=256,int dimy=256,int dimz=153)
 *
 * Editor: Bo Ma
 *
 * Date: 2018.12.07
 * Version:3.0
 *
 *


 ***/
//void mcs_sss_img_plot(int z_slice)
void mcs_sss_img_plot()
{


  string base_path = "/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/XB1BN305N-BI/XB1BN305N-BI-01/";
  string path_image_mcs = base_path + "img/mcs/real_2_fov_mcs.i";
  string path_image_sss = base_path + "img/sss/XB1BN305N-BI-01_0-1800.i";


  int z_slice=70;

  string saved_root_file = base_path + "root_file/compare_MCS_SSS_scatter_reconstructed_img.root";
  //string saved_img_hist_name="./pictures/mcs_sss_hist_zSlice_"+IntToString(z_slice)+".png";
  //string saved_img_plot_name="./pictures/mcs_sss_plot_zSlice_"+IntToString(z_slice)+".png";

  /* string path_image_mcs="./reconstructed_img/mcs.i";*/
  /*string path_image_sss="./reconstructed_img/sss.i";*/

  cout<<path_image_mcs<<endl;
  cout<<path_image_sss<<endl;

  int dimx=256;
  int dimy=256;
  int dimz=153;
  //float spacing=1.25;
  float spacing=1.00;

//----------------------1: draw the 2d image of a single slice------------------------

  char histTitle_mcs[200];
  char histTitle_sss[200];
  sprintf (histTitle_mcs, "mcs_z_slice_%i",z_slice);
  sprintf (histTitle_sss, "sss_z_slice_%i",z_slice);

  TH2F *h2_mcs = new TH2F(histTitle_mcs,histTitle_mcs, dimx, 0 , dimx*spacing, dimy, 0, dimy*spacing);
  TH2F *h2_sss = new TH2F(histTitle_sss,histTitle_sss, dimx, 0 , dimx*spacing, dimy, 0, dimy*spacing);


  Draw_histgram_mcs_sss(path_image_mcs,path_image_sss,saved_root_file,h2_mcs,h2_sss,z_slice);

//----------------------2: draw the profile of 2d slice------------------------

  Draw_plot_mcs_sss(h2_mcs,h2_sss,saved_root_file,z_slice);



  if(h2_mcs!=NULL)
  {
    delete h2_mcs ;
    h2_mcs=NULL;

  }
  if(h2_sss!=NULL)
  {
    delete h2_sss;
    h2_sss=NULL;
  }

  gApplication->Terminate();

}



void Draw_histgram_mcs_sss(string path_image_mcs,string path_image_sss,string saved_img_root_file,TH2F *h2_mcs,TH2F *h2_sss,int z_slice=70,int dimx=256,int dimy=256,int dimz=153, float spacing=1.25)
{


  //---------------------------------1: create hist from img;---------------------------------------

  // return value: the total counts of the histogram.
  float total_counts_mcs = get_histgram_from_img(path_image_mcs,h2_mcs,z_slice,dimx,dimy,dimz, spacing);
  float total_counts_sss = get_histgram_from_img(path_image_sss,h2_sss,z_slice,dimx,dimy,dimz, spacing);


  float max_val_mcs=h2_mcs->GetMaximum();	
  float max_val_sss=h2_sss->GetMaximum();	 
  float max_val=max_val_mcs;
  if(max_val<max_val_sss){
    max_val=max_val_sss;
  }
  //set the max value for both the images.
  h2_mcs->SetMaximum(max_val);
  h2_sss->SetMaximum(max_val);



  //---------------------------------2: draw hist--------------------------------------

  //************start canvas  for brain image
  string canvasTitle="canvas_MCS_vs_SSS_img";
  TCanvas* can_hist=new TCanvas(canvasTitle.c_str(),canvasTitle.c_str(), 1600, 800);

  can_hist->Divide(2,1);
  //cout<<"the max value is:"<<max_val<<endl;
  TPaletteAxis *palette_mcs;
  TPaletteAxis *palette_sss;

  DrawHistgram(can_hist,h2_mcs,palette_mcs,1);
  DrawHistgram(can_hist,h2_sss,palette_sss,2);


  TPaveText *pt = new TPaveText(.05,.7,.35,.9,"brNDC");

  float difference_mcs_sss=float(abs(total_counts_mcs-total_counts_sss))/total_counts_sss;

  //change to percentage format
  difference_mcs_sss*=100;

  int precision_num=0;
  string string_total_counts_mcs=Float_to_string(total_counts_mcs,precision_num);
  string string_total_counts_sss=Float_to_string(total_counts_sss,precision_num);
  precision_num=2;
  string string_difference=Float_to_string(difference_mcs_sss,precision_num);


  cout<<"the total counts of mcs scatter is: "<<total_counts_mcs<<endl;
  cout<<"the total counts of sss scatter is: "<<total_counts_sss<<endl;
  cout<<"the difference is: "<<difference_mcs_sss<<endl;


  cout<<"the total counts of mcs scatter is: "<<string_total_counts_mcs<<endl;
  cout<<"the total counts of sss scatter is: "<<string_total_counts_sss<<endl;
  cout<<"the difference is: "<<string_difference<<endl;


  string mcs_text="MCS counts: "+string_total_counts_mcs;
  string sss_text="SSS counts: "+string_total_counts_sss;
  string difference_text="Difference: "+string_difference+"%";

  Color_t mcs_color=kBlue;
  Color_t sss_color=kRed;
  Color_t diff_color=kBlack;

  Gernerate_pave_text(pt, mcs_text.c_str(),mcs_color);
  Gernerate_pave_text(pt, sss_text.c_str(),sss_color);
  Gernerate_pave_text(pt, difference_text.c_str(),diff_color);

  gPad->Update();
  gPad->Modified();

  gSystem->ProcessEvents();



  //-------------------3: save image ,hist to root file---------------
  //-------------save_image(can_hist,saved_img_hist_name);
  //
  string h2_name=h2_mcs->GetName();

  save_TH2_to_root_file(h2_mcs, saved_img_root_file,h2_name);

  h2_name=h2_sss->GetName();
  save_TH2_to_root_file(h2_sss, saved_img_root_file,h2_name);

  save_canvas_to_root_file(can_hist,saved_img_root_file);

  gSystem->ProcessEvents();


  if(pt!=NULL)
  {
    delete pt;
    pt=NULL;
  }


  if(can_hist!=NULL)
  {
    delete can_hist;
    can_hist=NULL;
  }


}





// return value: the total counts of the histogram.
float get_histgram_from_img(string path_image,TH2F *h2,int z_slice=70,int dimx=256,int dimy=256,int dimz=153, float spacing=1.25)
{


  int nVoxels = dimx*dimy*dimz;

  float *image_data=new float[nVoxels];
  memset(image_data,0,sizeof(float)*nVoxels);


  //read data from disk to memory
  Read_image(path_image, image_data,nVoxels);


  Remove_image_noise(image_data);

  int zRange=2;
  GenerateHistogram(h2,image_data,z_slice,zRange);


  float total_counts;

  total_counts=SumHistogram(h2);

  cout<<"the total counts of scatter is: "<<total_counts<<endl;

  if(image_data!=NULL)
  {
    delete [] image_data;
    image_data=NULL;

  }

  return total_counts;


}

// draw the profile of defined region of the 2d slice img
// the yRange and zrange should be setup in the function to 
// reduce noise in the profile.
void Draw_plot_mcs_sss(TH2F *h2_mcs,TH2F *h2_sss,string saved_root_file,int z_slice=70,int yValue=155;int dimx=256)
{

  string canvasTitle="canvas_MCS_vs_SSS_plot";

  TCanvas* can_graph=new TCanvas(canvasTitle.c_str(),canvasTitle.c_str(), 1600, 1000);
  can_graph->SetGrid();

  TGraph *gr_mcs = new TGraph (dimx);
  TGraph *gr_sss = new TGraph (dimx);

  int yRange=4;
  int zRange=2;

  TVector* vector_mcs=new TVector(dimx);
  TVector* vector_sss=new TVector(dimx);

  float total_counts_mcs = GenerateGraphData(gr_mcs, h2_mcs, vector_mcs,yValue,yRange);
  float total_counts_sss = GenerateGraphData(gr_sss, h2_sss, vector_sss, yValue,yRange);


  Color_t mcs_color=kBlue;
  Color_t sss_color=kRed;

  GraphAttributeSet(gr_mcs,kBlue);
  GraphAttributeSet(gr_sss,kRed);

  TMultiGraph *mg = new TMultiGraph();
  mg->Add(gr_mcs);	
  mg->Add(gr_sss);


  MultiGraphAttributeSet(mg);

  TLegend *legend = new TLegend(0.70,0.70,0.90,0.90);

  //****draw legend 
  legend->AddEntry(gr_mcs,"MCS", "l");
  legend->AddEntry(gr_sss,"SSS","l");
  stringstream ss;
  ss<<"Z="<<z_slice<<"+-"<<zRange<<"  Y="<<yValue<<"+-"<<yRange;

  legend->AddEntry((TObject*)0, ss.str().c_str(), "");

  legend->Draw();



  TPaveText *pt = new TPaveText(.1,.7,.3,.9,"brNDC");

  float difference_mcs_sss=float(abs(total_counts_mcs-total_counts_sss))/total_counts_sss;
  //change to percentage format
  difference_mcs_sss*=100;
  cout<<"the total counts of mcs scatter is: "<<total_counts_mcs<<endl;
  cout<<"the total counts of sss scatter is: "<<total_counts_sss<<endl;
  cout<<"the difference is: "<<difference_mcs_sss<<endl;

  int precision_num=0;
  string string_total_counts_mcs=Float_to_string(total_counts_mcs,precision_num);
  string string_total_counts_sss=Float_to_string(total_counts_sss,precision_num);
  precision_num=2;
  string string_difference=Float_to_string(difference_mcs_sss,precision_num);

  cout<<"the total counts of mcs scatter is: "<<string_total_counts_mcs<<endl;
  cout<<"the total counts of sss scatter is: "<<string_total_counts_sss<<endl;
  cout<<"the difference is: "<<string_difference<<endl;



  string mcs_text="MCS counts: "+string_total_counts_mcs;
  string sss_text="SSS counts: "+string_total_counts_sss;
  string difference_text="Difference: "+string_difference+"%";

  Gernerate_pave_text(pt, mcs_text.c_str(),mcs_color);
  Gernerate_pave_text(pt, sss_text.c_str(),sss_color);
  Gernerate_pave_text(pt, difference_text.c_str(),kBlack);


  gPad->Update();
  gPad->Modified();


  //string saved_root_file="./pictures/mcs_sss_plot_zSlice_"+IntToString(z_slice)+".png";
  cout<<"image name is :"<<saved_root_file<<endl;
  gSystem->ProcessEvents();


  //-----------------------save the canvas and the TVector---------------------------
  //save_image(can_graph,saved_root_file);
  //
  string vec_name = "vector_mcs_" +ss.str();
  save_vector_to_root_file( vector_mcs, saved_root_file,vec_name);

  vec_name = "vector_sss_" +ss.str();
  save_vector_to_root_file( vector_sss, saved_root_file,vec_name);


  save_canvas_to_root_file(can_graph,saved_root_file);

  gSystem->ProcessEvents();

  //gSystem->Sleep(2000);

  if(gr_mcs!=NULL)
  {
    delete gr_mcs;
    gr_mcs=NULL;
  }

  if(gr_sss!=NULL)
  {
    delete gr_sss;
    gr_sss=NULL;
  }


  if(legend!=NULL)
  {
    delete legend;
    legend=NULL;
  }

  if(pt!=NULL)
  {
    delete pt;
    pt=NULL;
  }


  if(can_graph!=NULL)
  {
    delete can_graph;
    can_graph=NULL;
  }

  if(mg!=NULL)
  {
    delete mg;
    mg=NULL;
  }

  return;

}



void save_image( TCanvas* can, string saved_img_path)
{
  //this function is very important, otherwise,the picture can not be saved;
  gSystem->ProcessEvents();

  TImage *img = TImage::Create();

  /*string saved_img_hist_name="./pictures/mcs_sss_hist_zSlice_"+IntToString(z_slice)+".png";*/
  cout<<"saved image path is :"<<saved_img_path<<endl;

  //img->FromPad(c, 10, 10, 300, 200);
  img->FromPad(can);

  img->WriteImage(saved_img_path.c_str());

  delete img;


}


// 1: get the 2d img matrix from the 2d hist
// 2:save 2d hist into root file
//input: hist_name: the basic name to save the th2 and the 2d matrix; 
void save_TH2_to_root_file(TH2F *h2, string path_saved_root_file,string hist_name)
{

  //-----------1: get the 2d data matrix from TH2--------------------
  int dimx=h2->GetNbinsX();
  int dimy=h2->GetNbinsY();

  cout<<"test dimX:"<<dimx<<endl;
  cout<<"test dimY:"<<dimy<<endl;

  TMatrix* mat=new TMatrix(dimy,dimx);

  for(int i=0;i<dimy;i++)
  {
    for(int j=0;j<dimx;j++)
    {
      int bin_index=h2->GetBin(j+1,i+1);
      double bin_content=h2->GetBinContent(bin_index);

      mat(i,j)=bin_content;

    }

  }

  //-----------2: save the matrix and TH2 to root file---------------

  TFile *f = TFile::Open(path_saved_root_file.c_str(), "update");
  f->ls();

  f->cd();

  string h2_name = "h2_" + hist_name;
  string matrix_name = "matrix_" + hist_name;


  h2->Write(h2_name.c_str(),TObject::kOverwrite);
  mat->Write(matrix_name.c_str(),TObject::kOverwrite);

  f->Close();

}



void save_canvas_to_root_file(TCanvas *c1, string path_saved_root_file)
{

  TFile *f = TFile::Open(path_saved_root_file.c_str(), "update");
  f->ls();

  f->cd();

  c1->Write("",TObject::kOverwrite);

  f->Close();

}


void save_vector_to_root_file(TVector* my_vector, string path_saved_root_file,string vector_name)
{

  TFile *f = TFile::Open(path_saved_root_file.c_str(), "update");
  f->ls();

  f->cd();

  my_vector->Write(vector_name.c_str(),TObject::kOverwrite);

  f->Close();

}



//convert int to string
string IntToString (int a)
{
  ostringstream temp;
  temp<<a;
  return temp.str();
}
//read image from the disk to the memory
void Read_image(string file_path,float* imageData,int voxel_num)
{
  ifstream fin;
  fin.open(file_path.c_str());

  fin.read((char *)imageData, (voxel_num* sizeof(float)));
  fin.close();
}

//using the exiting data to generate a 2d histogram

float GenerateHistogram(TH2F* pHistgram, float * imageData, int z_slice,int zRange)
{
  int dimx=pHistgram->GetNbinsX();
  int dimy=pHistgram->GetNbinsY();

  cout<<"test dimX:"<<dimx<<endl;
  cout<<"test dimY:"<<dimy<<endl;


  for(int y = 0; y < dimy; y++)
  {

    for(int x= 0; x< dimx; x++)
    {
      float val=0;
      for(int i=-zRange;i<=zRange;i++)
      {
        int z=z_slice+i;
        if(z<0)
        {
          continue;
        }
        val+ = imageData[z*dimx*dimy + y*dimx + x];

      }
      val/=(zRange*2+1);
      //val+ = imageData[z*dimx*dimy + y*dimx + x];

      pHistgram->SetBinContent(x+1,y+1,val);

    }	
  }


  //to get the max value of the Histogram.
  return pHistgram->GetMaximum();	


}

//using the exiting data to generate a 2d histogram
float ScaleHistogram(TH2F* pHistgram, float Maximu_after_scale,float threshold_val)
{
  int dimx=pHistgram->GetNbinsX();
  int dimy=pHistgram->GetNbinsY();

  cout<<"test dimX:"<<dimx<<endl;
  cout<<"test dimY:"<<dimy<<endl;

  float max_before_scale=pHistgram->GetMaximum();
  float min_before_scale=pHistgram->GetMinimum();
  float max_minus_min=max_before_scale-min_before_scale;

  for(int x = 0; x < dimx; x++)
  {

    for(int y = 0; y< dimy; y++)
    {
      float val=pHistgram->GetBinContent(x+1,y+1);

      val=((val-min_before_scale)/max_minus_min)*Maximu_after_scale;
      if(val<threshold_val)
      {
        val=0;
      }

      pHistgram->SetBinContent(x+1,y+1,val);


    }	
  }

  float max_after_scale=0.0;
  float min_after_scale=0.0;
  float sum_events=0.0;
  for(int x = 0; x < dimx; x++)
  {

    for(int y = 0; y< dimy; y++)
    {
      float val=pHistgram->GetBinContent(x+1,y+1);
      sum_events+=val;
      if(max_after_scale<val)
      {
        max_after_scale=val;
      }
      if(min_after_scale>val)
      {
        min_after_scale=val;
      }

    }	
  }


  cout<<"the scaled number is :"<<Maximu_after_scale<<endl;

  cout<<"the maximum value before is: "<<max_before_scale<<endl;
  cout<<"the minimus value before is: "<<min_before_scale<<endl;

  cout<<"the maximum value after is: "<<max_after_scale<<endl;

  cout<<"the minimus value after is: "<<min_after_scale<<endl;
  cout<<"the total events is: "<<sum_events<<endl;

  //to get the max value of the Histogram.
  return sum_events;	


}



float SumHistogram(TH2F* pHistgram)
{
  int dimx=pHistgram->GetNbinsX();
  int dimy=pHistgram->GetNbinsY();

  cout<<"test dimX:"<<dimx<<endl;
  cout<<"test dimY:"<<dimy<<endl;



  float sum_events=0.0;
  for(int x = 0; x < dimx; x++)
  {

    for(int y = 0; y< dimy; y++)
    {
      float val=pHistgram->GetBinContent(x+1,y+1);
      sum_events+=val;

    }	
  }


  cout<<"the total events is: "<<sum_events<<endl;

  //to get the max value of the Histogram.
  return sum_events;	


}



//draw the hisgram
void DrawHistgram(TCanvas* can_hist,TH2F* pHistgram,TPaletteAxis *palette, int subImage_indext)
{
  can_hist->cd(subImage_indext);

  pHistgram->GetXaxis()->SetTitle("x");
  pHistgram->GetYaxis()->SetTitle("y");

  pHistgram->Draw("colz");

  can_hist->Update();

  palette = (TPaletteAxis*)pHistgram->GetListOfFunctions()->FindObject("palette");

  palette->SetX1NDC(0.9);
  palette->SetX2NDC(0.93);
  palette->SetY1NDC(0.1);
  palette->SetY2NDC(0.9);

  can_hist->Modified();
  can_hist->Update();


}


//generate a plot using one slice of the image
float GenerateGraphData(TGraph *myGraph,float* imageData,int yValue,int yRange, int z_slice,int zRange)
{
  int dimx=myGraph->GetN();
  int dimy=dimx;
  cout<<"Graph N:"<<dimx<<endl;


  float sum_events=0.0;
  for(int x = 0;  x< dimx; x++)
  {
    //average values of y+-2,to smooth the plot.
    float voxValue=0.0; 
    for(int i=-yRange;i<=yRange;i++)
    {
      yValue+=i;
      if(yValue<0)
      {
        continue;
      }

      for(int j=-zRange;j<=zRange;j++)
      {
        z_slice+=j;
        if(z_slice<0)
        {
          continue;
        }
        voxValue+= imageData[z_slice*dimx*dimy + yValue*dimx + x];

      }

    }
    voxValue/=(yRange*2+1+zRange*2+1);
    sum_events+=voxValue;

    myGraph->SetPoint(x,(double)x,(double)voxValue);
  }

  cout <<"the sum value is :"<<sum_events<<endl;
  return sum_events;

}


//generate a plot using one slice of the image
float GenerateGraphData(TGraph *myGraph,TH2F* pHistgram,TVector* p_vector,int yValue,int yRange)
{
  int dimx=myGraph->GetN();
  int dimy=dimx;
  cout<<"Graph N:"<<dimx<<endl;

  float sum_events=0.0;

  for(int x = 0;  x< dimx; x++)
  {
    //average values of y+-2,to smooth the plot.
    float voxValue=0.0; 
    for(int i=-yRange;i<=yRange;i++)
    {
      yValue+=i;
      if(yValue<=0)
      {
        continue;
      }
      //voxValue+= imageData[z_slice*dimx*dimy + yValue*dimx + x];
      voxValue+= pHistgram->GetBinContent(x+1,yValue+1);

    }
    voxValue/=(yRange*2+1);
    sum_events+=voxValue;


    myGraph->SetPoint(x,(double)x,(double)voxValue);
    p_vector(x)=voxValue;
  }

  cout <<"the sum value is :"<<sum_events<<endl;
  return sum_events;

}




//set the plot attributes
void GraphAttributeSet(TGraph *myGraph,Color_t lcolor)
{
  myGraph->SetLineColor(lcolor);
  myGraph->SetLineWidth(3);


}

//put multi graph into on MultiGraph.
//position: to describe the y and z value of the image.
void MultiGraphAttributeSet(TMultiGraph *mg)
{


  mg->Draw("AC");		

  mg->GetXaxis()->SetTitle("voxel index(transverse)");
  mg->GetYaxis()->SetTitle("voxel value");
  mg->GetXaxis()->CenterTitle();
  mg->GetYaxis()->CenterTitle();	
  mg->Draw("AC");

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


//conver float to string
string Float_to_string(float input_float,int precision_num)
{
  stringstream s_stream;
  s_stream << fixed << setprecision(precision_num) << input_float;
  string s = s_stream.str();

  return s;
}
string IntToString (int a)
{
  ostringstream temp;
  temp<<a;
  return temp.str();
}


float Get_max_image(float* input_image,int dimx=256,int dimy=256,int dimz=153)
{
  int z_min=20;
  int z_max=149;

  float max_value=0.0;
  //float min_value=10000.0;
  for(int z=z_min;z<z_max;z++)
  {
    for(int y = 0; y < dimy; y++)
    {

      for(int x= 0; x< dimx; x++)
      {
        float val= input_image[z*dimx*dimy + y*dimx + x];
        if(max_value<val)
        {
          max_value=val;
        }
        /*if(min_value>val)*/
        //{
        //min_value=val;
        /*}*/

      }

    }	
  }


  return max_value;

}


float Get_min_image(float* input_image,int dimx=256,int dimy=256,int dimz=153)
{
  int z_min=20;
  int z_max=149;

  //float max_value=0.0;
  float min_value=1000000.0;
  for(int z=z_min;z<z_max;z++)
  {
    for(int y = 0; y < dimy; y++)
    {

      for(int x= 0; x< dimx; x++)
      {
        float val= input_image[z*dimx*dimy + y*dimx + x];
        /*if(max_value<val)*/
        //{
        //max_value=val;
        /*}*/
        if(min_value>val)
        {
          min_value=val;
        }

      }

    }	
  }


  return min_value;

}

//remove the noise of the input image, set the voxels below the threshold as 0
float Remove_image_noise(float* input_image,int dimx=256,int dimy=256,int dimz=153)
{
  float Maximu_after_scale=256.0;

  //float threshold_val=5.0;
  float threshold_val=3;
  //float threshold_val=10.0;
  //truncate the original data with the head and tail which is really noisy
  int z_min=20;
  int z_max=149;

  float max_value=Get_max_image(input_image);
  float min_value=Get_min_image(input_image);;

  float max_minus_min=max_value-min_value;


  for(int z=z_min;z<z_max;z++)
  {
    for(int y = 0; y < dimy; y++)
    {

      for(int x= 0; x< dimx; x++)
      {
        float val= input_image[z*dimx*dimy + y*dimx + x];
        val=((val-min_value)/max_minus_min)*Maximu_after_scale;
        if(val<threshold_val)
        {
          input_image[z*dimx*dimy + y*dimx + x]=0;
        }
      }

    }	
  }


  float total_value_before=0.0;
  for(int z=z_min;z<z_max;z++)
  {
    for(int y = 0; y < dimy; y++)
    {

      for(int x= 0; x< dimx; x++)
      {

        total_value_before+= input_image[z*dimx*dimy + y*dimx + x];
      }

    }	
  }

  return total_value_before;
}

