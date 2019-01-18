//get data from root and draw 

void draw_root_data()
{
  ntuple_draw();

}

void ntuple_draw()
{

  //gBenchmark->Start("ntuple1");



  string path_saved_root_file="./my_root_file.root";
  string ntuple_name="Coincidence_ntuple";
  string ntuple_title="gpu_mcs_coincidence_data";


  TFile *f1 = TFile::Open(path_saved_root_file.c_str());
  if (!f1) return;

  TCanvas *c1 = new TCanvas("c1","The Ntuple canvas",700,700);
  //TCanvas *c1 = new TCanvas("c1","The Ntuple canvas",200,10,700,780);
  //c1->SetGrid();
  //c1->SetLogy();
  //c1->GetFrame()->SetFillColor(15);
  TNtuple *ntuple = (TNtuple*)f1->Get(ntuple_name.c_str());

  //ntuple->Draw("dimy:dimx:voxel_value","dimz==70");
  //ntuple->Draw("dimy:dimx:voxel_value","dimz==70","colz pfc");
  //ntuple->Draw("dimy:dimx:voxel_value","dimz==70","colz box1");
  //ntuple->Draw("dimy:dimx:voxel_value","dimz==70","col2");
  //ntuple->Draw("dimy:dimx:voxel_value","dimz==70","colz2");
  ntuple->Draw("dimy:dimx:voxel_value","dimz==70","colz nostack");

  //c1->cd();
  //c1->Update();
  //gStyle->SetStatColor(2);


  //gBenchmark->Show("ntuple1");

}
