{
//========= Macro generated from object: h0/histogram0
//========= by ROOT version5.34/36
   
   TH1F *h0 = new TH1F("h0","histogram0",100,-10,10);
   h0->SetBinContent(36,1);
   h0->SetBinContent(38,1);
   h0->SetBinContent(39,1);
   h0->SetBinContent(40,5);
   h0->SetBinContent(41,3);
   h0->SetBinContent(42,7);
   h0->SetBinContent(43,8);
   h0->SetBinContent(44,10);
   h0->SetBinContent(45,19);
   h0->SetBinContent(46,16);
   h0->SetBinContent(47,15);
   h0->SetBinContent(48,20);
   h0->SetBinContent(49,21);
   h0->SetBinContent(50,28);
   h0->SetBinContent(51,28);
   h0->SetBinContent(52,22);
   h0->SetBinContent(53,27);
   h0->SetBinContent(54,17);
   h0->SetBinContent(55,15);
   h0->SetBinContent(56,2);
   h0->SetBinContent(57,10);
   h0->SetBinContent(58,5);
   h0->SetBinContent(59,10);
   h0->SetBinContent(60,1);
   h0->SetBinContent(61,3);
   h0->SetBinContent(62,1);
   h0->SetBinContent(64,1);
   h0->SetBinContent(68,2);
   h0->SetBinContent(71,1);
   h0->SetEntries(300);

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
}
