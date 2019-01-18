void save_histograms_to_root()
{

  string base_folder="/data/PET/mr_pet_temp/Ma/software/data/gpupet/patients/FET/FE2BP034F-BI/";
  string path_saved_root_file = base_folder + "root_file/compare_MCS_SSS_scatter_image.root";
  string path_image_data = base_folder +"img/sss/sub2_ite32/FE2BP034F-BI_sss.data";
  
  int array_size=153;


  string hist_name="my_hist";
  string hist_title="This is my hist";

  //save_1d_histograms_to_root(path_saved_root_file,array_data,array_size, hist_name,hist_title);


string hist_name="my_hist_3d_1";
  string hist_title="This is my hist_3d_1";
//save_2d_histograms_to_root(path_image_data,path_saved_root_file,hist_name,hist_title);




string matrix_name="matrix_img_slice_70_range_1";
//save_2d_img_slic_to_matrix_to_root(path_image_data,path_saved_root_file,matrix_name);

  //save_1d_histograms_to_root();
//save_2d_histograms_from_matrix_to_root(path_saved_root_file,matrix_name,hist_name,hist_title);
//save_3d_histograms_from_img_to_root(path_image_data,path_saved_root_file,hist_name,hist_title);


 //string ntuple_name="Coincidence_ntuple";
 //string ntuple_title="gpu_mcs_coincidence_data";

//save_3d_img_to_tntuple_to_root(path_image_data,path_saved_root_file,ntuple_name,ntuple_title);

}


//void save_1d_histograms_to_root()
void save_1d_histograms_to_root(string path_saved_root_file,float *array_data,int array_size, string hist_name,string hist_title)
{


  TFile *f=new TFile(path_saved_root_file.c_str(),"update");

  int bin_num=array_size+2;
  int x_low=0;
  int x_up=bin_num-1;

  TH1F *my_hist = new TH1F(hist_name.c_str(),hist_title.c_str(),bin_num,x_low,x_up);

  for(int i = 0; i< array_size; i++)
  {

    float event_count=array_data[i]; 
    my_hist->SetBinContent(i,(double)event_count);
    //cout<<event_count<<endl;

  }

  my_hist->Print();

  //--------set hist attribute------------
  //h1_backup->SetTitle("cloned histo");

  my_hist->SetLineColor(kGreen);
  my_hist->SetLineWidth(2);
  //h1->SetFillColor(kRed);

  gStyle->SetOptStat(kFALSE);// disable stat box
  my_hist->GetXaxis()->SetTitle("this is x axis");
  my_hist->GetYaxis()->SetTitle("this is Y axis");
  my_hist->GetXaxis()->CenterTitle();
  my_hist->GetYaxis()->CenterTitle();

  //my_hist->GetXaxis()->SetRangeUser(0,200); // not working
  //my_hist->GetXaxis()->SetRangeUser(0,20); // bin 0 to 20 

  //gPad->SetLogy(kTRUE);// this is good for large numbers

  //my_hist->SetMaximum(100);
  //my_hist->SetMinimum(0)

  //my_hist->Draw();
  //my_hist->Draw("CH*");
  my_hist->Draw("CH");

  //--------end of set hist attribute------------

  my_hist->Write("",TObject::kOverwrite);

  f->Close();

  delete f;
  f=NULL;

}


void save_1d_histograms_to_root(string path_saved_root_file,string vector_name, string hist_name,string hist_title)
{


  TFile *f=new TFile(path_saved_root_file.c_str(),"update");


  TVector* vec;
  f->GetObject(vector_name.c_str(),vec);

  //Get the size of vector
  int array_size=vec->GetNrows();


  int bin_num=array_size+2;
  int x_low=0;
  int x_up=bin_num-1;

  TH1F *my_hist = new TH1F(hist_name.c_str(),hist_title.c_str(),bin_num,x_low,x_up);

  for(int i = 0; i< array_size; i++)
  {

    float event_count=vec(i); 
    my_hist->SetBinContent(i,(double)event_count);
    //cout<<event_count<<endl;

  }

  my_hist->Print();

  //--------set hist attribute------------
  //h1_backup->SetTitle("cloned histo");

  my_hist->SetLineColor(kGreen);
  my_hist->SetLineWidth(2);
  //h1->SetFillColor(kRed);

  gStyle->SetOptStat(kFALSE);// disable stat box
  my_hist->GetXaxis()->SetTitle("this is x axis");
  my_hist->GetYaxis()->SetTitle("this is Y axis");
  my_hist->GetXaxis()->CenterTitle();
  my_hist->GetYaxis()->CenterTitle();

  //my_hist->GetXaxis()->SetRangeUser(0,200); // not working
  //my_hist->GetXaxis()->SetRangeUser(0,20); // bin 0 to 20 

  //gPad->SetLogy(kTRUE);// this is good for large numbers

  //my_hist->SetMaximum(100);
  //my_hist->SetMinimum(0)

  //my_hist->Draw();
  //my_hist->Draw("CH*");
  my_hist->Draw("CH");

  //--------end of set hist attribute------------
  my_hist->Write("",TObject::kOverwrite);

  f->Close();

  delete f;
  f=NULL;

}


// save 2d histogram from the 3d image
void save_2d_histograms_from_img_to_root(string path_image_data,string path_saved_root_file,string hist_name,string hist_title,int z_slice=70,int dimx=256,int dimy=256,int dimz=153, float spacing=1.25)
{
   int nVoxels = dimx*dimy*dimz;
  float *image_data=new float[nVoxels];
  memset(image_data,0,sizeof(float)*nVoxels);

  get_image_data_for_root(path_image_data,image_data);


  string canvas_name="my_canvas_h3";
  string canvas_title="my_canvas_h3";
  TCanvas *c1 = new TCanvas(canvas_name.c_str(),canvas_title.c_str(),200,10,600,400);
  c1->SetGrid();

  // start the construction of TH2

  int x_bin_num=dimx;
  float x_low=0;
  float x_up=dimx*spacing;

  int y_bin_num=dimy;
  float y_low=0;
  float y_up=dimy*spacing;
  TH2F *h2= new TH2F(hist_name.c_str(),hist_title.c_str(), x_bin_num, x_low , x_up, y_bin_num, y_low, y_up);

  //------get the defined slice image into the 2d hist
  //int z_slice=70;
  int zRange=1;
  GenerateHistogram(h2,image_data,z_slice,zRange);

  //------set the attribute of the 2d his 

  set_TH2_attribute(c1,h2,hist_name);

    //----save to root file
  TFile *f=new TFile(path_saved_root_file.c_str(),"update");

  f->cd();
  h2->Write("",TObject::kOverwrite);

  c1->Write("",TObject::kOverwrite);

  f->Close();

  delete f;
  f=NULL;

  delete [] image_data;
  image_data=NULL;



}



void save_2d_histograms_from_matrix_to_root(string path_saved_root_file,string matrix_name,string hist_name,string hist_title,float spacing=1)
{
   

  //1: open root file

  TFile* f=new TFile(path_saved_root_file.c_str(),"update");

 TMatrix* mat=(TMatrix*)f->Get(matrix_name.c_str());
 int dimx=mat->GetNcols();
 int dimy=mat->GetNrows();

 cout<<"dimx is: "<<dimx<<endl;
 cout<<"dimy is: "<<dimy<<endl;

  string canvas_name="my_canvas_h4";
  string canvas_title="my_canvas_h4";
  TCanvas *c1 = new TCanvas(canvas_name.c_str(),canvas_title.c_str(),200,10,600,400);
  c1->SetGrid();

  // start the construction of TH2

  int x_bin_num=dimx;
  float x_low=0;
  float x_up=dimx*spacing;

  int y_bin_num=dimy;
  float y_low=0;
  float y_up=dimy*spacing;
  TH2F *h2= new TH2F(hist_name.c_str(),hist_title.c_str(), x_bin_num, x_low , x_up, y_bin_num, y_low, y_up);

  //------get the defined slice image into the 2d hist
  for(int y = 0; y < dimy; y++)
  {

    for(int x= 0; x< dimx; x++)
    {
      float val= mat(y,x);

      h2->SetBinContent(x+1,y+1,val);

    }	
  }

  //------set the attribute of the 2d his 

  set_TH2_attribute(c1,h2,hist_name); 
  //----save to root file

  f->cd();
  h2->Write("",TObject::kOverwrite);

  c1->Write("",TObject::kOverwrite);

  f->Close();

  delete f;
  f=NULL;

  delete c1;
  c1=NULL;

  delete h2;
  h2=NULL;

}



void save_2d_img_slic_to_matrix_to_root(string path_image_data,string path_saved_root_file,string matrix_name,int z_slice=70,int dimx=256,int dimy=256,int dimz=153, float spacing=1.25)
{
  int nVoxels = dimx*dimy*dimz;
  float *image_data=new float[nVoxels];
  memset(image_data,0,sizeof(float)*nVoxels);

  get_image_data_for_root(path_image_data,image_data);



  int col_num=dimx;
  int row_num=dimy;
  TMatrix* mat=new TMatrix(row_num,col_num);
 
  int zRange=1;


for(int y = 0; y < row_num; y++)
  {

    for(int x= 0; x< col_num; x++)
    {
      float val=0;
      for(int i=-zRange;i<=zRange;i++)
      {
        int z=z_slice+i;
        if(z<0)
        {
          continue;
        }
        val+ = image_data[z*dimx*dimy + y*dimx + x];

      }
      val/=(zRange*2+1);
      //val+ = imageData[z*dimx*dimy + y*dimx + x];

     mat(y,x)=val ;

    }	
  }


   //----save to root file
  TFile *f=new TFile(path_saved_root_file.c_str(),"update");

  f->cd();
  mat->Write(matrix_name.c_str(),TObject::kOverwrite);

  f->Close();

  delete f;
  f=NULL;

  delete mat;
  mat=NULL;



  delete [] image_data;
  image_data=NULL;


}




// save 2d histogram from the 3d image
void save_3d_histograms_from_img_to_root(string path_image_data,string path_saved_root_file,string hist_name,string hist_title,int dimx=256,int dimy=256,int dimz=153, float spacing=1.25)
{
   int nVoxels = dimx*dimy*dimz;
  float *image_data=new float[nVoxels];
  memset(image_data,0,sizeof(float)*nVoxels);

  get_image_data_for_root(path_image_data,image_data);


  string canvas_name="my_canvas_h3d";
  string canvas_title="my_canvas_h3d";
  TCanvas *c1 = new TCanvas(canvas_name.c_str(),canvas_title.c_str(),200,10,600,400);
  c1->SetGrid();

  // start the construction of TH2

  int x_bin_num=dimx;
  float x_low=0;
  float x_up=dimx*spacing;

  int y_bin_num=dimy;
  float y_low=0;
  float y_up=dimy*spacing;

  int z_bin_num=dimz;
  float z_low=0;
  float z_up=dimz*spacing;



  TH3F *h3= new TH3F(hist_name.c_str(),hist_title.c_str(), x_bin_num, x_low , x_up, y_bin_num, y_low, y_up,z_bin_num,z_low,z_up);

  //------get the defined slice image into the 2d hist
 for(int z=0;z<dimz;z++)
  {
    for(int y = 0; y < dimy; y++)
    {

      for(int x= 0; x< dimx; x++)
      {
        float val= image_data[z*dimx*dimy + y*dimx + x];
        h3->SetBinContent(z+1,y+1,x+1, val);
      }

    }	
  }


  //------set the attribute of the 2d his 

  set_TH3_attribute(c1,h3,hist_name);

  //h3->Draw("col");
  //h3->Draw("BOX2");
 /*   //----save to root file*/
  TFile *f=new TFile(path_saved_root_file.c_str(),"update");

  f->cd();
  h3->Write("",TObject::kOverwrite);

  c1->Write("",TObject::kOverwrite);

  f->Close();

  delete f;
  f=NULL;

   delete h3;
  h3=NULL;

 delete c1;
  c1=NULL;



  delete [] image_data;
  image_data=NULL;



}



void save_3d_img_to_tntuple_to_root(string path_image_data,string path_saved_root_file,string ntuple_name,string ntuple_title,int dimx=256,int dimy=256,int dimz=153, float spacing=1.25)
{
   int nVoxels = dimx*dimy*dimz;
  float *image_data=new float[nVoxels];
  memset(image_data,0,sizeof(float)*nVoxels);

  get_image_data_for_root(path_image_data,image_data);


 //string ntuple_name="Coincidence_ntuple";
 //string ntuple_title="gpu_mcs_coincidence_data";
 string var_list="dimx:dimy:dimz:voxel_value";
 TNtuple *ntuple = new TNtuple(ntuple_name.c_str(),ntuple_title.c_str(),var_list.c_str());

  
  //------get the defined slice image into the 2d hist
 for(int z=0;z<dimz;z++)
  {
    for(int y = 0; y < dimy; y++)
    {

      for(int x= 0; x< dimx; x++)
      {
        float val= image_data[z*dimx*dimy + y*dimx + x];
        ntuple->Fill(x,y,z,val);
      }

    }	
  }


  //------set the attribute of the 2d his 

    //h3->Draw("col");
  //h3->Draw("BOX2");
 /*   //----save to root file*/
  TFile *f=new TFile(path_saved_root_file.c_str(),"update");

  f->cd();
  ntuple->Write("",TObject::kOverwrite);


  ntuple->Print();

  f->Close();

  delete f;
  f=NULL;

   delete ntuple;
  ntuple=NULL;



  delete [] image_data;
  image_data=NULL;



}





void get_image_data_for_root(string path_image_data,float* image_data,int dimx=256,int dimy=256,int dimz=153, float spacing=1.25)

{
  int nVoxels = dimx*dimy*dimz;
  int nVoxels_per_slice=dimx*dimy;

  /*float *image_data=new float[nVoxels];*/
  /*memset(image_data,0,sizeof(float)*nVoxels);*/

  char* magfield="0";
  cout<<magfield<<endl;
  //read data from disk to memory
  Read_image(path_image_data, image_data,nVoxels);



  float *output_data=new float[nVoxels];
  memset(output_data,0,sizeof(float)*nVoxels);

  // and remove the noise of the image, the theshold is set in 
  // the function.
  // return value: the sum of intensity of the image
  float total_intensity= Remove_image_noise(image_data,output_data);

  //---if you want to change the image intensity to 
  //---activity concentration, then convert suv

  //This can be get from the ecat image
  float total_activity_ori=9746000;
  //usually, this is acquired by the sss reconstructed image
  float total_intensity_ori=9746000;

  //convert the intensity image to activity image
  SUV_convert(output_data,total_activity_ori,total_intensity_ori);

  //-----end of covert intensity to suv.

  //copy back the processd data 
  for(int z=0;z<dimz;z++)
  {
    for(int y = 0; y < dimy; y++)
    {

      for(int x= 0; x< dimx; x++)
      {

        image_data[z*dimx*dimy + y*dimx + x]=output_data[z*dimx*dimy + y*dimx + x];
      }

    }	
  }

}





/*---------Example illustrating how to save histograms in Tree branches.
//To run this example, do
// root > .L htest.C
// root > htw()
// root > htr1()
// root > htr2()
// root > htr3()
//
//Author: Rene Brun
---------------*/
void save_histograms_to_tree() {
  //htw();
  //htw1();
  //htr1();
  //htr2();
  //htr3();
  htr4();
}


void htw() {
  //create a Tree with a few branches of type histogram
  //25000 entries are filled in the Tree
  //For each entry, the copy of 3 histograms is written
  //The data base will contain 75000 histograms.
  gBenchmark->Start("hsimple");
  TFile f("ht.root","recreate");
  TTree *T     = new TTree("T","test");
  TH1F *hpx    = new TH1F("hpx","This is the px distribution",100,-4,4);
  TH2F *hpxpy  = new TH2F("hpxpy","py vs px",40,-4,4,40,-4,4);
  TProfile *hprof  = new TProfile("hprof","Profile of pz versus px",100,-4,4,0,20);
  T->Branch("hpx","TH1F",&hpx,32000,0);
  T->Branch("hpxpy","TH2F",&hpxpy,32000,0);
  T->Branch("hprof","TProfile",&hprof,32000,0);
  Float_t px, py, pz;
  for (Int_t i = 0; i < 25000; i++) {
    if (i%1000 == 0) printf("at entry: %d\n",i);
    gRandom->Rannor(px,py);
    pz = px*px + py*py;
    hpx->Fill(px);
    hpxpy->Fill(px,py);
    hprof->Fill(px,pz);
    T->Fill();
  }
  T->Print();
  f.Write();
  gBenchmark->Show("hsimple");
}

void htw1() {
  //create a Tree with a few branches of type histogram
  //25000 entries are filled in the Tree
  //For each entry, the copy of 3 histograms is written
  //The data base will contain 75000 histograms.
  gBenchmark->Start("hsimple");
  TFile f("ht.root","recreate");
  TH1F *hpx    = new TH1F("hpx","This is the px distribution",100,-4,4);
  TH2F *hpxpy  = new TH2F("hpxpy","py vs px",40,-4,4,40,-4,4);
  TProfile *hprof  = new TProfile("hprof","Profile of pz versus px",100,-4,4,0,20);
  Float_t px, py, pz;
  for (Int_t i = 0; i < 25000; i++) {
    if (i%1000 == 0) printf("at entry: %d\n",i);
    gRandom->Rannor(px,py);
    pz = px*px + py*py;
    hpx->Fill(px);
    hpxpy->Fill(px,py);
    hprof->Fill(px,pz);
  }

  hpx->SetLineColor(kGreen);
  hpx->SetLineWidth(2);
  hpx->SetFillColor(kRed);


  f.Write();
  gBenchmark->Show("hsimple");
}


void htr1() {
  //connect Tree generated by htw and show histograms for entry 12345
  TFile *f = new TFile("ht.root");
  TTree *T = (TTree*)f->Get("T");
  TH1F *hpx = 0;
  TH2F *hpxpy = 0;
  TProfile *hprof = 0;
  T->SetBranchAddress("hpx",&hpx);
  T->SetBranchAddress("hpxpy",&hpxpy);
  T->SetBranchAddress("hprof",&hprof);
  T->GetEntry(12345);
  //T->GetEntry(24999);
  TCanvas *c1 = new TCanvas("c1","test",10,10,600,1000);
  c1->Divide(1,3);
  c1->cd(1);
  hpx->Draw();
  c1->cd(2);
  hpxpy->Draw();
  c1->cd(3);
  hprof->Draw();
}
void htr2() {
  //connect Tree generated by htw and show histograms for entry 12345
  // a variant of htr1
  TFile *f = new TFile("ht.root");
  TTree *T = (TTree*)f->Get("T");
  TCanvas *c1 = new TCanvas("c1","test",10,10,600,1000);
  c1->Divide(1,3);
  c1->cd(1);
  T->Draw("hpx.Draw()","","goff",1,12345);
  c1->cd(2);
  T->Draw("hpxpy.Draw()","","goff",1,12345);
  c1->cd(3);
  T->Draw("hprof.Draw()","","goff",1,12345);
}
void htr3() {
  //connect Tree generated by htw
  //read all histograms and plot the RMS of hpx versus the Mean of hprof
  //for each of the 25000 entries
  TFile *f = new TFile("ht.root");
  TTree *T = (TTree*)f->Get("T");
  T->Draw("hpx.GetRMS():hprof.GetMean()");
}

void htr4() {
  //connect Tree generated by htw
  //read all histograms and plot the RMS of hpx versus the Mean of hprof
  //for each of the 25000 entries
  TFile *f = new TFile("ht.root");
  TH1F *hpx = (TH1F*)f->Get("hpx");
  hpx->SetLineColor(kGreen);
  hpx->SetLineWidth(2);
  hpx->SetFillColor(kRed);

  hpx->Draw();

  cout<<"max is: "<<hpx->GetMaximumBin()<<endl;

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

//----remove the noise of the input image, set the voxels below the threshold as 0
//-----threshold should be set in the function
//-----return: the sum of the image intensity.
float Remove_image_noise(float* input_image, float* output_image,int dimx=256,int dimy=256,int dimz=153)
{
  float Maximu_after_scale=256.0;

  float threshold_val=5.0;
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
          val=0;
        }
        output_image[z*dimx*dimy + y*dimx + x]=val;
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
        float val= output_image[z*dimx*dimy + y*dimx + x];

        if(val>0)
        {
          output_image[z*dimx*dimy + y*dimx + x]=input_image[z*dimx*dimy + y*dimx + x];
          total_value_before+= output_image[z*dimx*dimy + y*dimx + x];
        }
      }

    }	
  }


  return total_value_before;
}


/* function name:SUV_convert(float* input_image, float* output_image,float total_activity)
 * Parametors: total_activity: the total activity in kBq
 * total_activity_ori: This can be get from the ecat image
 * total_intensity_ori:  sum of image intensity: usually, this is acquired by the sss reconstructed image
 *
 * */
void SUV_convert(float* input_image,float total_activity_ori,float total_intensity_ori,int dimx=256,int dimy=256,int dimz=153,float spacing=1.25)
{


  float SUV_factor=total_activity_ori/total_activity_ori;
  float voxel_volume=spacing*spacing*spacing;
  SUV_factor=SUV_factor/voxel_volume;

  for(int z=0;z<dimz;z++)
  {
    for(int y = 0; y < dimy; y++)
    {

      for(int x= 0; x< dimx; x++)
      {
        float val= input_image[z*dimx*dimy + y*dimx + x];
        input_image[z*dimx*dimy + y*dimx + x]=val*SUV_factor;
      }

    }	
  }

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

void Gernerate_pave_text(TPaveText *pt,string insert_text,Color_t text_color )
{
  //TPaveText *pt = new TPaveText(.05,.1,.25,.3);
  //TText *t1 = pt->AddText("F(t) = #sum_{i=-#infty}^{#infty}A(i)cos#[]{#frac{i}{t+i}}");
  //pt->AddLine(.0,.5,1.,.5);
  TText *t1 = pt->AddText(insert_text.c_str());
  t1->SetTextColor(text_color);
  pt->Draw();
  pt->SetFillColor(0);


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


void set_TH2_attribute(TCanvas *c1,TH2F *h2,string hist_name)
{
  
  //h2->SetMaximum(max_val);
c1->Update();

  h2->GetXaxis()->SetTitle("x");
  h2->GetYaxis()->SetTitle("y");

  h2->Draw("colz");

  // set the pallet
  // update must be issued
  gPad->Update();

  TPaletteAxis *palette;

  palette = (TPaletteAxis*)h2->GetListOfFunctions()->FindObject("palette");
  
  palette->SetX1NDC(0.9);
  palette->SetX2NDC(0.93);
  palette->SetY1NDC(0.1);
  palette->SetY2NDC(0.9);

  // paveText
  TPaveText *pt = new TPaveText(.05,.7,.35,.9,"brNDC");
  string text_content=hist_name;
  Color_t pave_color=kBlue;
  Gernerate_pave_text(pt, text_content,pave_color);

  //------end of attribute set

  gPad->Update();
  gPad->Modified();
  gSystem->ProcessEvents();


}

void set_TH3_attribute(TCanvas *c1,TH3F *h3,string hist_name)
{
  
  //h3->SetMaximum(max_val);
c1->Update();

  h3->GetXaxis()->SetTitle("x");
  h3->GetYaxis()->SetTitle("y");
  h3->GetZaxis()->SetTitle("z");

  h3->Draw("");

  // set the pallet
  // update must be issued
  gPad->Update();

  
  TPaletteAxis *palette;

 /* palette = (TPaletteAxis*)h3->GetListOfFunctions()->FindObject("palette");*/
  
  //palette->SetX1NDC(0.9);
  //palette->SetX2NDC(0.93);
  //palette->SetY1NDC(0.1);
  /*palette->SetY2NDC(0.9);*/

  // paveText
  TPaveText *pt = new TPaveText(.05,.7,.35,.9,"brNDC");
  string text_content=hist_name;
  Color_t pave_color=kBlue;
  Gernerate_pave_text(pt, text_content,pave_color);

  //------end of attribute set

  gPad->Update();
  gPad->Modified();
  gSystem->ProcessEvents();


}
