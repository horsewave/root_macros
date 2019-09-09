/*
 * Compare the time of flight between GATE and GGEMS using a cube phantom with a point soure in the middle of it.1.25mm.
 *
 *
 */

#include <vector>

typedef std::vector<int> int_vec_t; 
typedef std::vector<float> float_vec_t;

void GATE_GGEMS_TOF_compare()
{

  gROOT->Reset();



  //string full_path_gate= "/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/phantom_gate_mcs_tof_comp/root_file/tof_gate.root";
  string full_path_gate= "/data/PET/mr_pet_temp/Ma/software/src/gate/gate_macros/gate_mcs_tof_compare/results/root_tof_gate_3.root";
  string full_path_mcs= "/data/PET/mr_pet_temp/Ma/software/src/gate/gate_macros/gate_mcs_tof_compare/results/root_tof_gate_2.root";
  //string full_path_mcs= "/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/phantom_gate_mcs_tof_comp/root_file/tof_mcs_1.root";


  string saved_root_path = "/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/phantom_gate_mcs_tof_comp/root_file/saved_root.root" ;



  //float_vec_t vector_time_diff_mcs=Root_data_analize_mcs(full_path_mcs);
  float_vec_t vector_time_diff_mcs=Root_data_analize_gate(full_path_mcs);
  cout <<"finished 1"<<endl;
  float_vec_t vector_time_diff_gate=Root_data_analize_gate(full_path_gate);
  //float_vec_t vector_time_diff_gate=Root_data_analize_gate(full_path_gate);
  cout <<"finished 2"<<endl;


  cout<<"step 1"<<endl;

  string saved_vector_name_mcs="time_diff_mcs";
  string saved_vector_name_gate="time_diff_gate";
  write_vector_to_root(saved_root_path,vector_time_diff_mcs,saved_vector_name_mcs);
  write_vector_to_root(saved_root_path,vector_time_diff_gate,saved_vector_name_gate);


  //event counts of every Z position.
  int vec_size_mcs=vector_time_diff_mcs.size();
  int vec_size_gate=vector_time_diff_gate.size();
  cout<< "vector size mcs: " <<vec_size_mcs<<endl;
  cout<< "vector size gate: " <<vec_size_gate<<endl;

 /* // get the bigger size of the vector*/
  //if(vec_size_mcs>=vec_size_gate)
  //{
    //vec_size_mcs=vec_size_gate;
  //}

  /*int vec_size=vec_size_mcs;*/

  float* tof_diff_mcs= new float[vec_size_mcs];
  float* tof_diff_gate= new float[vec_size_gate];
  //float* array_events_differ= new float[vec_size];
  memset(tof_diff_mcs,0,sizeof(float)*vec_size_mcs);
  memset(tof_diff_gate,0,sizeof(float)*vec_size_gate);
  //memset(array_events_differ,0,sizeof(float)*vec_size);

  //cout<< "vector size: " <<vec_size<<endl;

  double total_mcs=0;
  double total_gate=0;

  for(int i=0;i<vec_size_mcs;i++)
  {
    tof_diff_mcs[i]=vector_time_diff_mcs[i];
    total_mcs+=vector_time_diff_mcs[i];

  }
 
  for(int i=0;i<vec_size_gate;i++)
  {
    tof_diff_gate[i]=vector_time_diff_gate[i];
    total_gate+=vector_time_diff_gate[i];


  }

  float scale_gate_to_mcs= total_mcs/total_gate;
  //float scale_gate_to_mcs= 1;
  cout<<"******mcs  is"<<total_mcs<<endl;
  cout<<"******gate is"<<total_gate<<endl;
  cout<<"******scaling factor is"<<scale_gate_to_mcs<<endl;

   for(int i=0;i<vec_size_gate;i++)
  {
    tof_diff_gate[i]=tof_diff_gate[i]*scale_gate_to_mcs;


  }


  cout<<"step 2"<<endl;


  Generate_save_mcs_gate_sino_plot(tof_diff_mcs, tof_diff_gate,vec_size_mcs,vec_size_gate,saved_root_path);
  //Generate_hist_mcs_gate_tof(tof_diff_mcs, tof_diff_gate,vec_size, saved_root_path);
  //Generate_hist_mcs_gate_tof_test(tof_diff_mcs, tof_diff_gate,vec_size, saved_root_path);


  if(tof_diff_mcs!=NULL)
  {
    delete [] tof_diff_mcs;
    tof_diff_mcs=NULL;
  }

  if(tof_diff_gate!=NULL)
  {
    delete [] tof_diff_gate;
    tof_diff_gate=NULL;
  }

  gApplication->Terminate();

}	



float_vec_t Root_data_analize_mcs(string full_path)
{


  TFile *root_file=new TFile(full_path.c_str());

  //   //---define data tree for evaluation
  TTree *Coincidences = (TTree*)gDirectory->Get("Coincidences");

  char         coincidence_type;
  Double_t         time1;
  Double_t         time2;

  Coincidences->SetBranchAddress("coincidence_type",&coincidence_type);
  Coincidences->SetBranchAddress("time1",&time1);
  Coincidences->SetBranchAddress("time2",&time2);



  Int_t nentries = Coincidences->GetEntries();

  cout<<"entry: "<<nentries<<endl;


  //******1: Get the max and min value of tof diff************
  int min_val=1000000;
  int max_val=-1000000;

  //---Loop for each event in the TTree Coincidences
  for (Int_t i=0; i<nentries;i++) 
  {
    //double random_temp=((double) rand()/(RAND_MAX));

    Coincidences->GetEntry(i);

    if (coincidence_type<3) 
    {
      // in picosecond
      double time_1_val=time1*1e3;
      double time_2_val=time2*1e3;

      double tof_diff_val=time_1_val-time_2_val;
      if(i%2==0)
      {
        tof_diff_val=0-tof_diff_val;

      }
      //tof_diff[i]=tof_1[i]+tof_2[i];

      if(tof_diff_val>=max_val)
      {
        max_val=tof_diff_val;
      }
      if(tof_diff_val<=min_val)
      {
        min_val=tof_diff_val;
      }
      cout<<"time1: "<<time_1_val<< " time2: "<<time_2_val<<" diff: "<< tof_diff_val<<endl;


    }


  }

  root_file->Close();

  cout<<"time1 max: "<<max_val<< " time min: "<<min_val<<endl;

  //******2: determin the size of the time diff vector ************
  // min_val should be negative, get the abs
  int abs_min=abs(min_val);

  // compare max_val and abs(min_val), select the bigger one
  if(abs_min>=max_val)
  {
    max_val=abs_min;
  }

  // max_val should be half size of the array
  cout<< "max_after is :"<<max_val<<endl;
  // the array size of the time diff
  int array_size = max_val*2+2;
  cout<< "array size is :"<<array_size<<endl;;


  //copy the data to a vector to return


  //******3: get  diff vector ************
  float_vec_t vector_time_diff(array_size);


  root_file->Close();
  cout<<"step 2"<<endl;


  TFile *root_file_1=new TFile(full_path.c_str());

  //   //---define data tree for evaluation
  TTree *Coincidences_1 = (TTree*)gDirectory->Get("Coincidences");


  Coincidences_1->SetBranchAddress("coincidence_type",&coincidence_type);
  Coincidences_1->SetBranchAddress("time1",&time1);
  Coincidences_1->SetBranchAddress("time2",&time2);





  for (Int_t i=0;i<nentries;i++) 
  {

    //cout<<"enry:"<<i<<endl;
    Coincidences_1->GetEntry(i);
    //cout<<"enry:" <<i<<endl;

    if (coincidence_type<3) 
    {

      double time_1_val=time1*1e3;
      double time_2_val=time2*1e3;
      double tof_diff_val=time_1_val-time_2_val;
      //double random_temp=((double) rand()/(RAND_MAX));

      if(i%2==0)
      {
        tof_diff_val=0-tof_diff_val;

      }


      // this is the index of the vector
      int int_tof_diff=tof_diff_val +max_val;

      // add one to this time diff
      if(int_tof_diff>=array_size)
      {
        cout<< "time is: "<<int_tof_diff<<endl;
        cout<< "arry size is: "<<array_size<<endl;
        cout<<"error!!!"<<endl;
        return;
      }
      else
        vector_time_diff[int_tof_diff]++;

      cout<<"entry: "<< i<<"time1: "<<time_1_val<< " time2: "<<time_2_val<<" diff: "<< tof_diff_val<<endl;


    }


  }


  cout<<"step 5"<<endl;

  cout<<"entry num: "<< nentries<<endl;

  cout<< "array size is :"<<array_size<<endl;

  root_file_1->Close();


  if(root_file!=NULL)
  {
  delete root_file;
  root_file=NULL;
  }


  return vector_time_diff;


}







float_vec_t Root_data_analize_gate(string full_path)
{


  //cout<<"step0"<<endl;
  //std::ofstream of;
  //cout<<"step1"<<endl;
  //of.open(log_path.c_str());
  //of.open("test.txt");

  //cout<<"step2"<<endl;
  //--- adding coincidence chains
  /* TChain chain("Coincidences");*/
  //chain.Add(full_path);
  //chain.Add(basename +str_frame_num+ str_slice + str_slice_num + ext);


  //--- This line for single ROOT file
  //TFile f("BrainPET_Plane_source.root");
  TFile *root_file=new TFile(full_path.c_str());

  //   //---define data tree for evaluation
  TTree *Coincidences = (TTree*)gDirectory->Get("Coincidences");


  //---Declaration of leaves types - TTree Coincidences  
  Int_t           RayleighCrystal1;
  Int_t           RayleighCrystal2;
  Int_t           RayleighPhantom1;
  Int_t           RayleighPhantom2;
  Char_t          RayleighVolName1[40];
  Char_t          RayleighVolName2[40];
  Float_t         axialPos;
  Char_t          comptVolName1[40];
  Char_t          comptVolName2[40];
  Int_t           compton1;
  Int_t           compton2;
  Int_t           crystalID1;
  Int_t           crystalID2;
  Int_t           comptonPhantom1;
  Int_t           comptonPhantom2;
  Float_t         energy1;
  Float_t         energy2;   
  Int_t           eventID1;
  Int_t           eventID2;
  Float_t         globalPosX1;
  Float_t         globalPosX2;
  Float_t         globalPosY1;
  Float_t         globalPosY2;      
  Float_t         globalPosZ1;
  Float_t         globalPosZ2;
  Int_t           layerID1;
  Int_t           layerID2;
  Int_t           moduleID1;
  Int_t           moduleID2;
  Float_t         rotationAngle;
  Int_t           rsectorID1;
  Int_t           rsectorID2;
  Int_t           runID;
  Float_t         sinogramS;
  Float_t         sinogramTheta;
  Int_t           sourceID1;
  Int_t           sourceID2;
  Float_t         sourcePosX1;
  Float_t         sourcePosX2;
  Float_t         sourcePosY1;
  Float_t         sourcePosY2;
  Float_t         sourcePosZ1;
  Float_t         sourcePosZ2;
  Int_t           submoduleID1;
  Int_t           submoduleID2;
  Double_t         time1;
  Double_t         time2;

  //---extra variables   
  Float_t         zmin,zmax,z;

  cout<<"step4"<<endl;

  //---Set branch addresses - TTree Coincidences
  //---to setup data reading
  Coincidences->SetBranchAddress("RayleighCrystal1",&RayleighCrystal1);
  Coincidences->SetBranchAddress("RayleighCrystal2",&RayleighCrystal2);
  Coincidences->SetBranchAddress("RayleighPhantom1",&RayleighPhantom1);
  Coincidences->SetBranchAddress("RayleighPhantom2",&RayleighPhantom2);
  Coincidences->SetBranchAddress("RayleighVolName1",&RayleighVolName1);
  Coincidences->SetBranchAddress("RayleighVolName2",&RayleighVolName2);
  Coincidences->SetBranchAddress("axialPos",&axialPos);
  Coincidences->SetBranchAddress("comptVolName1",&comptVolName1);
  Coincidences->SetBranchAddress("comptVolName2",&comptVolName2);
  Coincidences->SetBranchAddress("comptonCrystal1",&compton1);
  Coincidences->SetBranchAddress("comptonCrystal2",&compton2);
  Coincidences->SetBranchAddress("crystalID1",&crystalID1);
  Coincidences->SetBranchAddress("crystalID2",&crystalID2);
  Coincidences->SetBranchAddress("comptonPhantom1",&comptonPhantom1);
  Coincidences->SetBranchAddress("comptonPhantom2",&comptonPhantom2);
  Coincidences->SetBranchAddress("energy1",&energy1);
  Coincidences->SetBranchAddress("energy2",&energy2);   
  Coincidences->SetBranchAddress("eventID1",&eventID1);
  Coincidences->SetBranchAddress("eventID2",&eventID2);
  Coincidences->SetBranchAddress("globalPosX1",&globalPosX1);
  Coincidences->SetBranchAddress("globalPosX2",&globalPosX2);
  Coincidences->SetBranchAddress("globalPosY1",&globalPosY1);
  Coincidences->SetBranchAddress("globalPosY2",&globalPosY2);      
  Coincidences->SetBranchAddress("globalPosZ1",&globalPosZ1);
  Coincidences->SetBranchAddress("globalPosZ2",&globalPosZ2);
  Coincidences->SetBranchAddress("layerID1",&layerID1);
  Coincidences->SetBranchAddress("layerID2",&layerID2);
  Coincidences->SetBranchAddress("moduleID1",&moduleID1);
  Coincidences->SetBranchAddress("moduleID2",&moduleID2);
  Coincidences->SetBranchAddress("rotationAngle",&rotationAngle);
  Coincidences->SetBranchAddress("rsectorID1",&rsectorID1);
  Coincidences->SetBranchAddress("rsectorID2",&rsectorID2);
  Coincidences->SetBranchAddress("runID",&runID);
  Coincidences->SetBranchAddress("sinogramS",&sinogramS);
  Coincidences->SetBranchAddress("sinogramTheta",&sinogramTheta);
  Coincidences->SetBranchAddress("sourceID1",&sourceID1);
  Coincidences->SetBranchAddress("sourceID2",&sourceID2);
  Coincidences->SetBranchAddress("sourcePosX1",&sourcePosX1);
  Coincidences->SetBranchAddress("sourcePosX2",&sourcePosX2);
  Coincidences->SetBranchAddress("sourcePosY1",&sourcePosY1);
  Coincidences->SetBranchAddress("sourcePosY2",&sourcePosY2);
  Coincidences->SetBranchAddress("sourcePosZ1",&sourcePosZ1);
  Coincidences->SetBranchAddress("sourcePosZ2",&sourcePosZ2);
  Coincidences->SetBranchAddress("submoduleID1",&submoduleID1);
  Coincidences->SetBranchAddress("submoduleID2",&submoduleID2);
  Coincidences->SetBranchAddress("time1",&time1);
  Coincidences->SetBranchAddress("time2",&time2);




  Int_t nbytes = 0, nbytesdelay = 0, nrandom = 0, nscatter = 0, ntrue = 0;  


  Int_t nentries = Coincidences->GetEntries();



  //******1: Get the max and min value of tof diff************
  int min_val=1000000;
  int max_val=-1000000;

  //---Loop for each event in the TTree Coincidences
  for (Int_t i=0; i<nentries;i++) 
  {
    Coincidences->GetEntry(i);

    if (eventID1 == eventID2) 
    {
      // in picosecond
      double time_1_val=time1*1e12;
      double time_2_val=time2*1e12;
      double tof_diff_val=time_1_val-time_2_val;
      //tof_diff[i]=tof_1[i]+tof_2[i];

      if(tof_diff_val>=max_val)
      {
        max_val=tof_diff_val;
      }
      if(tof_diff_val<=min_val)
      {
        min_val=tof_diff_val;
      }
      //cout<<"time1: "<<tof_1[i]<< " time2: "<<tof_2[i]<<" diff: "<< tof_diff[i]<<endl;


    }


  }


  cout<<"time1 max: "<<max_val<< " time min: "<<min_val<<endl;

  //******2: determin the size of the time diff vector ************
  // min_val should be negative, get the abs
  int abs_min=abs(min_val);

  // compare max_val and abs(min_val), select the bigger one
  if(abs_min>=max_val)
  {
    max_val=abs_min;
  }

  // max_val should be half size of the array
  cout<< "max_after is :"<<max_val<<endl;
  // the array size of the time diff
  int array_size = max_val*2+2;
  cout<< "array size is :"<<array_size<<endl;;


  //copy the data to a vector to return


  //******3: get  diff vector ************
  //float_vec_t vector_time_diff(array_size);
  float_vec_t vector_time_diff(array_size);

  for (Int_t i=0; i<nentries;i++) 
  {
    Coincidences->GetEntry(i);

    if (eventID1 == eventID2) 
    {
      double time_1_val=time1*1e12;
      double time_2_val=time2*1e12;
      double tof_diff_val=time_1_val-time_2_val;

      // this is the index of the vector
      int int_tof_diff=tof_diff_val +max_val;

      // add one to this time diff
      vector_time_diff[int_tof_diff]++;

      cout<<"entry: "<< i<<"time1: "<<time_1_val<< " time2: "<<time_2_val<<" diff: "<< tof_diff_val<<endl;


    }


  }



  cout<<"entry num: "<< nentries<<endl;

  cout<< "array size is :"<<array_size<<endl;;
  root_file->Close();


  if(root_file!=NULL)
  {
    delete root_file;
    root_file=NULL;
  }


  return vector_time_diff;


}









void scale_two_hist(TH1F *my_hist_mcs,TH1F *my_hist_gate, int max_scaled)
{

  int hist_size =my_hist_gate->GetNbinsX();

  // scale the hisgram
  double max_mcs=my_hist_mcs->GetMaximum();
  double min_mcs=my_hist_mcs->GetMinimum();

  double max_gate=my_hist_gate->GetMaximum();
  double min_gate=my_hist_gate->GetMinimum();

  //get the bigger one for max and smaller on for min
  if(max_mcs<=max_gate)
  {
    max_mcs=max_gate;
  }

  if(min_mcs>=min_gate)
  {
    min_mcs=min_gate;

  }
  double max_val=max_mcs;
  double min_val=min_mcs;


  double max_sub_min=max_val-min_val;

  for(int x = 0; x < hist_size; x++)
  {

    float val=my_hist_mcs->GetBinContent(x+1);
    //val=((val-min_val)/max_sub_min)*max_scaled;
    val=((val)/max_sub_min)*max_scaled;
    int int_val=(int)val;
    my_hist_mcs->SetBinContent(x+1,int_val);


  }	
  for(int x = 0; x < hist_size; x++)
  {

    float val=my_hist_gate->GetBinContent(x+1);
    //val=((val-min_val)/max_sub_min)*max_scaled;
    val=((val)/max_sub_min)*max_scaled;
    int int_val=(int)val;
    my_hist_gate->SetBinContent(x+1,int_val);


  }	

}



void Generate_save_mcs_gate_sino_plot(float*tof_diff_mcs, float* tof_diff_gate,int array_size_mcs,int array_size_gate,string saved_data_path)
{

  string canvas_name="canvas_tof_diff_MCS_GATE_compare";
  string canvas_title="TOF DIFF catter Sinogram Comparison for MCS and gate";

  TCanvas* can_graph=new TCanvas(canvas_name.c_str(),canvas_title.c_str(), 1600, 1000);
  can_graph->SetGrid();

  TGraph *gr_mcs = new TGraph (array_size_mcs);
  TGraph *gr_gate = new TGraph (array_size_gate);


  GenerateGraphData(gr_mcs,tof_diff_mcs);
  GenerateGraphData(gr_gate,tof_diff_gate);

  Color_t mcs_color=kBlue;
  Color_t gate_color=kRed;
  GraphAttributeSet(gr_mcs,kBlue);
  GraphAttributeSet(gr_gate,kRed);


  TMultiGraph *mg = new TMultiGraph();
  TLegend *legend = new TLegend(0.10,0.80,0.3,0.90);


  MultiGraphAttributeSet(mg,legend, gr_mcs,gr_gate);


  gPad->Update();
  gPad->Modified();

  //save_image(can_graph,saved_data_path);

  save_canvas_to_root_file(can_graph, saved_data_path);
  //save_canvas_to_root_file(can_graph, saved_data_path,gr_mcs,gr_gate,mg);

  gSystem->ProcessEvents();
  gPad->WaitPrimitive();
  if(legend!=NULL)
  {
    delete legend;
    legend=NULL;

  }


  if(gr_gate!=NULL)
  {
    delete gr_gate;
    gr_gate=NULL;
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

//generate a plot using one slice of the image
void GenerateGraphData(TGraph *myGraph,float* imageData)
{
  int max_z=myGraph->GetN();
  cout<<"Graph N:"<<max_z<<endl;

  for(int x = 0;  x< max_z; x++)
  {
    //average values of y+-2,to smooth the plot.
    float event_count=imageData[x]; 
    double x_pos=x-(max_z/2);

    myGraph->SetPoint(x,(double)x_pos,(double)event_count);
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
void MultiGraphAttributeSet(TMultiGraph *mg,TLegend *legend, TGraph *gr_mcs,TGraph *gr_gate)
{
  mg->Add(gr_mcs);	
  mg->Add(gr_gate);


  mg->Draw("AC");		

  mg->GetXaxis()->SetTitle("time diff");
  mg->GetYaxis()->SetTitle("Event counts");
  mg->GetXaxis()->CenterTitle();
  mg->GetYaxis()->CenterTitle();	
  mg->Draw("AC");

  //****draw legend 
  legend->AddEntry(gr_mcs,"MCS", "l");
  //legend->AddEntry(gr_mcs,"MCS_96", "l");
  legend->AddEntry(gr_gate,"gate","l");
  //legend->AddEntry(gr_gate,"MCS_24","l");
  //legend->AddEntry((TObject*)0, position, "");
  legend->Draw();

} 

void save_canvas_to_root_file(TCanvas *c1, string path_saved_root_file,TGraph *gr_mcs=NULL, TGraph *gr_gate=NULL, TMultiGraph *mg=NULL)
{

  TFile *f = TFile::Open(path_saved_root_file.c_str(), "update");
  f->ls();

  f->cd();

  c1->Write("",TObject::kOverwrite);

  //gr_mcs->Write("graph_mcs",TObject::kOverwrite);
  //gr_gate->Write("graph_gate",TObject::kOverwrite);
  /*mg->Write("multi_graph_mcs_gate",TObject::kOverwrite);*/



  f->Close();

}


void write_vector_to_root(string path_root_file,float_vec_t vector_save ,string saved_vector_name)
{

  TFile* f=new TFile(path_root_file.c_str(),"update");



  //1: declare a TVector with is the child of TObject.
  int array_size=vector_save.size();
  cout<<"the vector size is: "<< array_size<<endl;

  TVector* v=new TVector(array_size);

  for(int i=0;i<array_size;i++)
  {
    //set value to the vector
    v(i) =vector_save[i];
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



