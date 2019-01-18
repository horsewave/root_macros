/*
*   In this tutorial, 
*   We will discuss some basic properties of 1D histogram obj.
*   
*   Fill(), Draw(), Clone(), Reset(), and SetXXXX
*******************************************************************/
{

    //TOPIC1: 1D histrogram basic usage,  class 

    int binN = 100;
    int xmin = 0;
    int xmax = 100;
    /*
    |   in ROOT convention.
    |   bin #0        underflow bin
    |   bin #1        first bin with "lower" edge xmin  INCLUDED
    |   bin #nbins    last  bin with "upper"-edge xmax EXCLUDED
    |   bin #nbins+1  overflow bin
    |   
    */

    TCanvas* c_1 = new TCanvas("c_1","histo demo ", 500, 600,500,800);
    c_1->Divide(1,2);
    c_1->cd(1);

    TH1C* h1 = new TH1C("h1","Char: 127 to -128 ", binN, xmin, xmax);

    h1->Draw(); // all empty now.



    /*
    |   Fill()
    |   histogram is designed to show the counts( frequency)   
    |   ex. 6th bin start with 5 end with 6.
    |   Fill(5.2) will add the 6th bin a count. now 1 count
    |   Fill(5.3) will add the 6th bin a count. now 2 count
    |   
    */
    for( int i=0; i< 20; i++) {  
        h1->Fill(0.1);
        h1->Fill(5.2);
        h1->Fill(-0.1); // under the range // no effect
        h1->Fill(120);  // over  the range // no effect 
    }
    h1->Draw();
    

    /*
    |   note: if you really want "120" filled in the histogram 
    |   which ranges from 0 to 100, You can set:
    |   h1->SetBit(TH1::kCanRebin); !!set before filling
    |   
    |   the range will automatically extend, "but" the bin size
    |   will be changed.
    */



    /*
    |   over the char upper limit 127,
    |   it will be only display 127 in counts.
    */
    for( int i=0; i< 500; i++) {  
        h1->Fill(0.1);
        h1->Fill(5.2);
    }
    h1->Draw();





    //TOPIC2: clone a histrogram
    c_1->cd(2);

    TH1C* h1_backup = (TH1C*) h1->Clone();
    h1_backup->SetTitle("cloned histo");
    h1_backup->Draw();
    /*
    |   use close() method, 
    |   then convert its return stuff to a TH1C pointer.
    |   
    |   now any modification in h1 wont affect h1_backup.
    */





    //TOPIC3: LineColor, LineWidth, LineStyle, FillColor

    h1->SetLineColor(kGreen);
    h1->SetLineWidth(2);
    h1->SetFillColor(kRed); 
    // https://root.cern.ch/root/html/TAttLine.html#L3




    //TOPIC4: OptStat
    gStyle->SetOptStat(kFALSE); // disable stat box


    
    
    //TOPIC5: Axis Label
    h1->GetXaxis()->SetTitle("this is Xaxis");
    h1->GetYaxis()->SetTitle("this is Yaxis");
    
    
    
    

    //TOPIC6: Axis range ( only for zooming in)    
    //h1->GetXaxis()->SetRangeUser(0,200); // not working.
    h1->GetXaxis()->SetRangeUser(0,20); // value 0 to 20.
    
    h1->GetXaxis()->SetRange(1,10); // bin1 to bin20
    
    
    
    
    
    //TOPIC7: Log scale.
    c_1->cd(1);
    gPad->SetLogy(kTRUE); 
    gPad->SetLogy(kFALSE);




    
    //TOPIC8: Max and min value 
    h1->SetMaximum(10);
    h1->SetMinimum(2);
    h1->SetMaximum();  // use default setting.
    h1->SetMinimum();  // use default setting.
    
    
    
    //TOPIC9: save to ps, jpg, pdf, eps or others
    c_1->Print("test.jpg");
    gPad->Print("JustOnePad.jpg");
    
    
    // Note: in GUI-->menu bar --> view-->Editor
    // you can find more drawing options.
    
    
} // end of script.

