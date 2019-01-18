/*
*    In this tutorial, 
*    We will expore the TCut, TCutG,
*    and some useful skills for Draw() and Scan() method.
*
*******************************************************************/
{
    //________________Part 1 "Draw"________________________
    
    TFile* f = new TFile("simple.root");
    TTree* t;
    f->GetObject("t",t);

    TCanvas* c1 = new TCanvas("c1", "my histo", 500, 800);    
    c1->Divide(1,2);
    /*
    | set a Canvas with the title "my histo"  
    | width  = 500 px.
    | height = 800 px.
    |
    | Divide to sub_Pad, 1 col X 2 row    
    |
    */



    c1->cd(1);  t->Draw("money");
    c1->cd(2);  t->Draw("money", "money < 2000");
    /*
    | whole data, and partial data from the selection.
    | x axis: money value 
    | y axis: counts (frequency)  
    |
    */

          
    c1->cd(1);  t->Draw("log10(money)");
    c1->cd(2);  t->Draw("sqrt(money)");
    /*
    |
    | you can use math.
    | ie. you can call function, and get the return value.
    |
    */



    c1->cd(1); t->Draw("money", "money > 1000 && money < 2000 ");
    c1->cd(2); t->Draw("money", "money < 1000 || money > 2000 ");
    /*
    |
    | Logic operator
    | &&, ||, etc to combine the selection.
    |
    */



    /*
    |
    | Using TCut obj
    | to make the selection input easier.
    |
    */
    TCut cut1 = "money < 1000";
    TCut cut2 = "money > 2000";
    c1->cd(1); t->Draw("money");
    c1->cd(2); t->Draw("money", cut1 || cut2);


    t->Draw("money>>h1(2000,0,4000)", "", "goff");
    t->Draw("money>>h2(2000,0,4000)", cut1 || cut2, "goff");
    c1->cd(1); h1->Draw();
    c1->cd(2); h2->Draw();
    /*
    |
    | redirect to a histogram
    | 
    | num of bin = 2000
    | x-axis from 0 to 4000
    | 
    | in this way, it is convenient histo to files.
    */






    //________________Part 2 "Draw" with TCutG_____________________


    TCanvas* c2 = new TCanvas("c2", "my 2D plot", 500, 500);  
    t->Draw("x:y>>hxy(2000,0,1500,2000,0,12000)", "", "colz");
    /*
    | x-axis = value of x
    | y-axis = value of y
    | color  = counts ( frequency)
    | x-axis range = 0 to  1500
    | y-axis range = 0 to 12000
    | option: colz, surf, lego, etc...
    */

    /*
    | to make a graphic cut
    | 1) at menu bar "view" -> "Toolbar"
    | 2) at toolbar select the icon of scissors
    | 3) use mouse left click to draw a region.
    | 4) use double click to finish the region.
    | 5) select the outline, and right click --> "SetName"
    | 6) give a name, ex mycut
    */

    t->Draw("x:y>>hxy(2000,0,1500,2000,0,12000)", "mycut",  "colz");
    t->Draw("x:y>>hxy(2000,0,1500,2000,0,12000)", "!mycut", "colz");

    c1.cd(1); t->Draw("money", "mycut", "");
    c1.cd(2); t->Draw("money", "", "");


    
    TFile g("cut.root","recreate");
    mycut->Write();
    /*
    |   save the mycut to file, then you can reuse it again.
    |   you can also modify it.
    |   
    */

    TCanvas* ctest = new TCanvas("ctest", "just for test", 500, 500);
    mycut->Draw(); 
    mycut->Print();  

    /*
    |   for refining the region,
    |           
    |   you can use Print(), and use SetPoint() method,
    |   ex. mycut->SetPoint(14, 100,1000 );
    */
 
    f->Close();

    





    //________________Part 3 Scan_____________________ 

    TFile* f = new TFile("simple.root");

    TTree* t;
    f->GetObject("t",t);

    
    t->Scan("*")

    /* about the number of entries on print */
    t->SetScanField(10);
    t->Scan("*")


    /* about the column width */
    t->Scan("*","","colsize=20")
    t->Scan("*","","col=5:10:15")



    /* about the precision */
    t->Scan("*","","colsize=15 precision=4")
    t->Scan("*","","col=5.0F:10.0F:15.0F")
    t->Scan("*","","col=5.1F:10.2F:15.3F")



    /* about the range */
    t->Scan("*","","",10,0) // 10 runs, start from index = 0
    t->Scan("*","","",10,5) // 10 runs, start from index = 5



    /* about math or invoking function */
    t->Scan("sqrt(money)",        "", "colsize=15", 10,0 )
    t->Scan("TMath::Log2(money)", "", "colsize=15", 10,0 )



    f->Close();




    /*
    |
    |   4 Special variables for array:
    |   Entry$ , Entries$, Length$, Iteration$
    */
    
    // Let's create some data first.

    TFile* f2 = new TFile("simple_array.root","recreate");
    TTree* t  = new TTree("t", " array ");

    vector<Int_t> x;
    /*
    |   think of it is a class from vector, 
    |   with Int_t type specified in the template.
    */

    t->Branch("x",&x);

    x.push_back(10);
    x.push_back(11);
    x.push_back(12);
    t->Fill(); x.clear();
    /*
    |   push_back() to add elements
    |       clear() to clear all elements
    |        size() to return number of elements.
    |          at()
    |            []
    */
    
    x.push_back(9);
    x.push_back(99);
    x.push_back(7);
    x.push_back(77);
    x.push_back(777);
    t->Fill(); x.clear();

    x.push_back(2);
    x.push_back(21);
    x.push_back(121);
    x.push_back(111);
    x.push_back(333);
    x.push_back(444);
    x.push_back(555);
    t->Fill(); x.clear();

    t->Write();
    
    t->Scan("x");
    t->Scan("x:Entry$:Entries$");
    /*
    |    Entry$   represents the current run number.
    |    Entries$ represents the total run number.
    */

    t->Scan("x:Length$(x):Entry$");
    t->Scan("x:Length$(x):Iteration$");
    /*
    |    Length$(x)  returns the size of x
    |    Iteration$  returns the current index.
    */


     t->Scan("x : x[Iteration$+1] :x[Iteration$+2]"); 
     t->Scan("x : x[0]: x[Length$(x)-1]");
    /*
    |    x[i] : x[i+1] : x[i+2]
    |    x[i] : x[0]   : x[end] 
    */

    f->Close();

}
