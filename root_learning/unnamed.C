{
//=========Macro generated from canvas: Canvas_1/Canvas_1
//=========  (Thu Nov 22 13:08:54 2018) by ROOT version5.34/36
   TCanvas *Canvas_1 = new TCanvas("Canvas_1", "Canvas_1",370,117,1306,807);
   Canvas_1->Range(-12.5,-1.44375,12.5,12.99375);
   Canvas_1->SetFillColor(0);
   Canvas_1->SetBorderMode(0);
   Canvas_1->SetBorderSize(2);
   Canvas_1->SetFrameBorderMode(0);
   Canvas_1->SetFrameBorderMode(0);
   
   TH1F *h0 = new TH1F("h0","histogram0",100,-10,10);
   h0->SetBinContent(41,1);
   h0->SetBinContent(42,3);
   h0->SetBinContent(43,1);
   h0->SetBinContent(44,6);
   h0->SetBinContent(45,7);
   h0->SetBinContent(46,8);
   h0->SetBinContent(47,5);
   h0->SetBinContent(48,9);
   h0->SetBinContent(49,8);
   h0->SetBinContent(50,11);
   h0->SetBinContent(51,8);
   h0->SetBinContent(52,8);
   h0->SetBinContent(53,9);
   h0->SetBinContent(54,6);
   h0->SetBinContent(55,3);
   h0->SetBinContent(57,2);
   h0->SetBinContent(58,1);
   h0->SetBinContent(59,2);
   h0->SetBinContent(61,1);
   h0->SetBinContent(68,1);
   h0->SetEntries(100);
   
   TPaveStats *ptstats = new TPaveStats(0.78,0.775,0.98,0.935,"brNDC");
   ptstats->SetName("stats");
   ptstats->SetBorderSize(1);
   ptstats->SetFillColor(0);
   ptstats->SetTextAlign(12);
   ptstats->SetTextFont(42);
   TText *text = ptstats->AddText("h0");
   text->SetTextSize(0.0368);
   text = ptstats->AddText("Entries = 100    ");
   text = ptstats->AddText("Mean  = -0.1586");
   text = ptstats->AddText("RMS   = 0.8992");
   ptstats->SetOptStat(1111);
   ptstats->SetOptFit(0);
   ptstats->Draw();
   h0->GetListOfFunctions()->Add(ptstats);
   ptstats->SetParent(h0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   h0->SetLineColor(ci);
   h0->GetXaxis()->SetLabelFont(42);
   h0->GetXaxis()->SetLabelSize(0.035);
   h0->GetXaxis()->SetTitleSize(0.035);
   h0->GetXaxis()->SetTitleFont(42);
   h0->GetYaxis()->SetLabelFont(42);
   h0->GetYaxis()->SetLabelSize(0.035);
   h0->GetYaxis()->SetTitleSize(0.035);
   h0->GetYaxis()->SetTitleFont(42);
   h0->GetZaxis()->SetLabelFont(42);
   h0->GetZaxis()->SetLabelSize(0.035);
   h0->GetZaxis()->SetTitleSize(0.035);
   h0->GetZaxis()->SetTitleFont(42);
   h0->Draw("");
   
   TPaveText *pt = new TPaveText(0.3885849,0.9351869,0.6114151,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   text = pt->AddText("histogram0");
   pt->Draw();
   Canvas_1->Modified();
   Canvas_1->cd();
   Canvas_1->SetSelected(Canvas_1);
}
