/*
*   In this tutorial, 
*   We will discuss some basic properties of 2D histogram obj.
*   
*******************************************************************/
{

    //TOPIC1: Concept of Global bin.

    int binN = 100;
    int xmin = -2, xmax = 2;
    int ymin = -2, ymax = 2;
    
    TCanvas* c_1 = new TCanvas("c_1","2D histo demo ", 500,0, 500,800);
    c_1->Divide(1,2);
    c_1->cd(1);
    TH2C* htwoD = new TH2C("htwoD","a 2D histogram", 100, -2, 2,\
                                                     100, -2, 2);

    /*
    | in 2D or 3D histograms, the binning can be redefined 
    | by a simiplier 1D arary.
    | ex. bin#1 = (1,1) #2=(1,2) #3=(2,1) #4=(2,2)
    |
    | h->GetBin(binx, biny, binz) will return the global bin
    | and that is quite useful for "for" loop.
    | such as: h->GetBinContent(global_bin)
    */





    //TOPIC2: Filling a 2D histogram.
    
    for( int i=0; i< 10000; i++) {
    
      htwoD->Fill( gRandom->Gaus(-0.5,0.1),gRandom->Gaus(-0.5,0.1) );
      htwoD->Fill( gRandom->Gaus(0.2,0.2), gRandom->Gaus(1.4,0.2) );
    }
    
    htwoD->Draw("colz");
    
    /*
    | htwoD->Fill(x,y)
    | htwoD->Fill(x,y,w) ..note: w is the wight.
    */


    

    //TOPIC3: Projection 
    /*
    |   use the ProjectionX() and ProjectionY() method.
    |   to project a 2D histogram to a 1D histrogram
    */
    c_1->cd(2);
    htwoD->ProjectionX()->Draw();
    htwoD->ProjectionY()->Draw();


    /*
    |   If you want to have a "gate"
    |   such as see the y projection, but
    |   limit the data from the x range 0 to 1 
    |   1. Click the canvas in htwoD 
    |   2. select "SetShowProjectiony", input "25" (bins) 
    |   3. move the cursor to the range 0..1
    */
    
    
    /*
    |   to count the in a certain area 
    |   1. make a TCutG obj. and set name (eg. mycut)
    |   2. mycut->IntegralHist(htwoD)
    |   
    */

    


} // end of script.

