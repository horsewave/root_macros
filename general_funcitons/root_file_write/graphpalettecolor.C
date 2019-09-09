void graphpalettecolor () {
   gStyle->SetOptTitle(kFALSE);
   gStyle->SetPalette(kSolar);
   double x[5]  = {1,2,3,4,5};
   double y1[5] = {1.0,2.0,1.0,2.5,3.0};
   double y2[5] = {1.1,2.1,1.1,2.6,3.1};
   double y3[5] = {1.2,2.2,1.2,2.7,3.2};
   double y4[5] = {1.3,2.3,1.3,2.8,3.3};
   double y5[5] = {1.4,2.4,1.4,2.9,3.4};
   TGraph *g1 = new TGraph(5,x,y1); g1->SetTitle("Graph with a red star");
   TGraph *g2 = new TGraph(5,x,y2); g2->SetTitle("Graph with a circular marker");
   TGraph *g3 = new TGraph(5,x,y3); g3->SetTitle("Graph with an open square marker");
   TGraph *g4 = new TGraph(5,x,y4); g4->SetTitle("Graph with a blue star");
   TGraph *g5 = new TGraph(5,x,y5); g5->SetTitle("Graph with a full square marker");
   g1->SetLineWidth(3); g1->SetMarkerColor(kRed);
   g2->SetLineWidth(3); g2->SetMarkerStyle(kCircle);
   g3->SetLineWidth(3); g3->SetMarkerStyle(kOpenSquare);
   g4->SetLineWidth(3); g4->SetMarkerColor(kBlue);
   g5->SetLineWidth(3); g5->SetMarkerStyle(kFullSquare);
   g1->Draw("CA* PLC PFC");
   g2->Draw("PC  PLC PFC");
   g3->Draw("PC  PLC PFC");
   g4->Draw("*C  PLC PFC");
   g5->Draw("PC  PLC PFC");
   gPad->BuildLegend();
}
