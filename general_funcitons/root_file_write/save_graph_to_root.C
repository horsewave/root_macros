
void save_graph_to_root()
{
  string path_saved_root_file="./my_root_file.root";
  string vector_name="lor_events_axile_distribution";
  string graph_name="my_graph_2";
  //string graph_title="my_graph_2";
  //get_1d_graph_from_root(path_saved_root_file,graph_name);
  //save_multi_graph_to_root();



  vector<string> v_graph_name;

  vector<string> v_graph_name;
  string graph_name="my_graph";
  v_graph_name.push_back(graph_name);

  graph_name="my_graph_2";
  v_graph_name.push_back(graph_name);

  string multi_graph_name="my_multi_graphs";

  save_multi_graph_to_root(path_saved_root_file,v_graph_name,multi_graph_name);
  //get_multi_graph_from_root();

}

void save_1d_graph_to_root()
{
  // open a new ROOT file 
  TFile *f = TFile::Open("MyFile.root", "RECREATE"); 
  // create the graph (in memory) 
  TGraph *g = new TGraph();
  for (Int_t i = 0; i < 100; i++) 
    g->SetPoint(g->GetN(), i*i, i*i*i);
  f->cd(); 
  // make sure that we will write to this ROOT file 
  g->Write("MyGraph"); 
  // save the graph as "MyGraph" 
  f->ls();
  // show the contents of the ROOT file 
  delete f; // close the ROOT file
  g->Draw("A*"); // draw the graph (which is still in memory) 
  delete g; // delete the graph (from memory) 


}


void Save_1d_graph_to_root( string path_saved_root_file,string vector_name, string graph_name,string graph_title) 
{


  TFile *f=new TFile(path_saved_root_file.c_str(),"update");

  TVector* vec;
  f->GetObject(vector_name.c_str(),vec);

  //Get the size of vector
  int array_size=vec->GetNrows();

  TGraph *myGraph = new TGraph (array_size);


  // get the points of the graph
  int max_z=myGraph->GetN();
  cout<<"Graph N:"<<max_z<<endl;

  for(int x = 0;  x< max_z; x++)
  {
    //average values of y+-2,to smooth the plot.
    //float event_count=vec(x); 
    float event_count=vec(x); 

    myGraph->SetPoint(x,(double)x,(double)event_count);

    //cout<<"  "<<x<<":  "<<event_count<<endl;
  }

  myGraph->SetName(graph_name.c_str());
  myGraph->SetTitle(graph_title.c_str());
  //Color_t graph_color=kRed;
  Color_t graph_color=kBlue;
  myGraph->SetLineColor(graph_color);
  myGraph->SetLineWidth(4);
  myGraph->Draw("AC*");

  //write the graph to root file
  f->cd();

  myGraph->Write(graph_name.c_str(),TObject::kOverwrite);

  f->Close();

  delete f;
  f=NULL;



}


void Save_1d_graph_to_root( string path_saved_root_file,float *array_data,int array_size, string graph_name,string graph_title)
{

  //string path_saved_root_file="./my_root_file.root";
  //int array_size=72;

  TFile *f=new TFile(path_saved_root_file.c_str(),"update");


  TGraph *myGraph = new TGraph (array_size);


  int max_z=myGraph->GetN();
  cout<<"Graph N:"<<max_z<<endl;

  for(int x = 0;  x< max_z; x++)
  {
    //average values of y+-2,to smooth the plot.
    float event_count=array_data[x]; 

    myGraph->SetPoint(x,(double)x,(double)event_count);

  }

  myGraph->SetName(graph_name.c_str());
  myGraph->SetTitle(graph_title.c_str());

  Color_t graph_color=kRed;
  myGraph->SetLineColor(graph_color);
  myGraph->SetLineWidth(4);
  myGraph->Draw("AC*");

  //write the graph to root file
  f->cd();
  myGraph->Write(graph_name.c_str(),TObject::kOverwrite);

  f->Close();

  delete f;
  f=NULL;





}


//void save_multi_graph_to_root(string path_saved_root_file)
void save_multi_graph_to_root()
{

  string path_saved_root_file="./my_root_file.root";
  //string vector_name="lor_events_axile_distribution";
  string graph_name="my_graph";
  TFile *f = TFile::Open(path_saved_root_file.c_str(), "update");
  f->ls();


  TCanvas *c1 = new TCanvas("c1","Exclusion graphs examples",200,10,600,400);
  c1->SetGrid();

  TMultiGraph *mg = new TMultiGraph();
  mg->SetTitle("Exclusion graphs");


  // show the contents of the ROOT file
  TGraph *myGraph_1; 
  f->GetObject(graph_name.c_str(), myGraph_1);

  graph_name="my_graph_2";
  TGraph *myGraph_2; 
  f->GetObject(graph_name.c_str(), myGraph_2);

  //myGraph_1->Draw("AC*");
  //myGraph_2->Draw("AC*");

  //  with specific option, the two plots is in the same cavas.
  /*myGraph_1->Draw("CA*");*/
  /*myGraph_2->Draw("PC* ");*/

  mg->Add(myGraph_1);
  mg->Add(myGraph_2);


  mg->Draw("AC*");

  f->cd();
  mg->Write("multiple_graph",TObject::kOverwrite);


  c1->Write("canvas",TObject::kOverwrite);
  f->Close();

  delete f;
  f=NULL;


}


// save MultiGraph to root.
// paras:
// 1: the root file where MultiGraph get the TGraph and save it to;
// 2:v_graph_name: this vector has all the graph names in the root file;
// 3:multi_graph_name: the name saved to the root file.
// 4; if you want set the name of the canvas, add the canvas name
//void save_multi_graph_to_root(string path_saved_root_file,vector<string> v_graph_name,string multi_graph_name,string canvas_name)
void save_multi_graph_to_root(string path_saved_root_file,vector<string> v_graph_name,string multi_graph_name)
{

  TFile *f = TFile::Open(path_saved_root_file.c_str(), "update");

  // show the contents of the ROOT file
  f->ls();

  TCanvas *c1 = new TCanvas("c1","Exclusion graphs examples",200,10,600,400);
  c1->SetGrid();

  TMultiGraph *mg = new TMultiGraph();

TLegend *legend = new TLegend(0.10,0.80,0.3,0.90);


  for(int i=0;i<v_graph_name.size();i++)
  {
    string graph_name=v_graph_name[i];
    TGraph *myGraph; 
    f->GetObject(graph_name.c_str(), myGraph);
    mg->Add(myGraph);

    // add legend
  legend->AddEntry(myGraph,graph_name.c_str(), "l");
  }

 
  //---set MultiGraph attribute

  // need to draw first,
  // then you can set the attribute.
  mg->Draw("AC*");
  mg->SetTitle("Exclusion graphs");
  mg->GetXaxis()->SetTitle("Axile sinograme number");
  mg->GetYaxis()->SetTitle("Event counts");
  mg->GetXaxis()->CenterTitle();
  mg->GetYaxis()->CenterTitle();
//draw multiple image
  mg->Draw("AC*");

//----end of set attribute

 
   //****draw legend 
  legend->Draw();

  
  //-------Generate pavetext
   TPaveText *pt = new TPaveText(.1,.6,.3,.8,"brNDC");

   string text_content="hello first one";
   Color_t pave_color=kBlue;
   Gernerate_pave_text(pt, text_content,pave_color);
 text_content="hello second one";
   Color_t pave_color=kRed;
   Gernerate_pave_text(pt, text_content,pave_color);
 
  
  //------end of Generate pavetext
 gPad->Update();
  gPad->Modified();


  f->cd();
  mg->Write(multi_graph_name.c_str(),TObject::kOverwrite);


  string canvas_name="my_canvas";
  c1->Write(canvas_name.c_str(),TObject::kOverwrite);
  f->Close();

  gSystem->ProcessEvents();

  delete f;
  f=NULL;


}




void get_1d_graph_from_root()
{
  // open the ROOT file with the graph
  TFile *f = TFile::Open("MyFile.root", "READ");
  f->ls();
  // show the contents of the ROOT file
  TGraph *g; f->GetObject("MyGraph", g);
  // retrieve "MyGraph"
  delete f; // close the ROOT file
  if (g)
    g->Draw("A*"); // draw the graph (from memory) //
  g->Draw("AC*"); // draw the graph (from memory) //
  delete g; // delete the graph (from memory) 

}

void get_1d_graph_from_root(string path_saved_root_file,string graph_name)
{
  // open the ROOT file with the graph
  TFile *f = TFile::Open(path_saved_root_file.c_str(), "READ");
  f->ls();
  // show the contents of the ROOT file
  TGraph *myGraph; 
  f->GetObject(graph_name.c_str(), myGraph);
  // retrieve "MyGraph"
  delete f; // close the ROOT file
  if (myGraph)
  {
    //myGraph->Draw("A*"); // draw the graph (from memory) //
    myGraph->Draw("AC*"); // draw the graph (from memory) //


  }
  //delete myGraph; // delete the graph (from memory) 

}


//void get_multi_graph_from_root(string path_saved_root_file,string multi_graph_name)
void get_multi_graph_from_root()
{

  string path_saved_root_file="./my_root_file.root";
  string multi_graph_name="my_multi_graphs";

  TFile *f = TFile::Open(path_saved_root_file.c_str(), "update");

  // show the contents of the ROOT file
  f->ls();

  TCanvas *c1 = new TCanvas("c1","Exclusion graphs examples",200,10,600,400);
  c1->SetGrid();

  TMultiGraph *mg;
  f->GetObject(multi_graph_name.c_str(),mg);


  TList* grlist;
  grlist = mg->GetListOfGraphs();
  int graph_num=grlist->GetSize();

  TGraph *gr=(TGraph*)grlist->At(0);
  gr->Draw("AC*");


  for(int i=1;i<graph_num;i++)
  {


    gr=(TGraph*)grlist->At(i);
    gr->Draw("PC*");
    //gSystem->Sleep(2000);
  }


  //f->cd();
  //mg->Write(multi_graph_name.c_str(),TObject::kOverwrite);


  //string canvas_name="my_canvas";
  /*c1->Write(canvas_name.c_str(),TObject::kOverwrite);*/
  f->Close();

  delete f;
  f=NULL;


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




