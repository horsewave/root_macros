
/*
    Good Introduction:
    http://natureofcode.com/book/chapter-10-neural-networks/

    TMultiLayerPerceptron Reference:
    http://cp3.irmp.ucl.ac.be/~delaere/MLP/

    This script is to demonstrate how to use TMultiLayerPerceptron class
    and TMLPAnalyzer class to exam the properties.
    
    For an ANN to work, we need to provide data to train ( to calibrate ) it.
    It is analogous to the process of trial and error -- we feed in the data
    and the ANN adjust the inner parameters so that when we put the inputs,
    and then we will get the approximated outputs.

    Think ANN in term of linear algebra:
    Let a is a vector of inputs,  a = ( a1, a2, ... ,ai)
    Let b is a vector of outputs, b = ( b1, b2, ..., bo )

    suppose we have just two hidden layers, then
    we have two weight matrices w1, w2.

    It will very similar to:
    w2*(w1*a) = b
    we feed in the data to get the most appropriate w1 and w2,
    but we will have a sigmoid function 1/(1 + exp(-1*input) ) 
    to adjust the output from each layer.
*/

class TDataPreparation {

private:
     TTree* fDataTree;

public:

    TDataPreparation() {

        fDataTree = new TTree( "datatree", "datatree");

        // r and z are cylindrical coordinates
        // Br is the radial component of the magnetic field B
        Int_t nlines = fDataTree->ReadFile("Br.dat","r:z:Br");

    }


    TTree* getDataTree() { return fDataTree; }
};



void lesson_20_demo_ANN() {

    // STEP 0
    // for using ANN, it is very important to prepare the feeding data.
    //
    TDataPreparation  myFeedingData;
    TTree* t = myFeedingData.getDataTree();



    //====================================================================
    // STEP 1
    // to setup mlp and train it.
    //
    TMultiLayerPerceptron *mlp =
    new TMultiLayerPerceptron( "@r,@z:10:10:10:@Br", t, 
    			       "Entry$%2","(Entry$+1)%2");

    /*
    |  two inputs are r and z
    |  three hiden layers with 10+10+10 neurons.
    |  the more nerons, the more calculation time.
    |  one output = Br, and it is normalized "@"
    |
    |  training data from the entries when Entry No. = even (Entry$%2)  =true
    |  testing  data from the entries when Entry No. = odd  (Entry+1)%2 =true
    |  1:1
    */

    mlp->SetLearningMethod( TMultiLayerPerceptron::kStochastic );
    /*
        kStochastic // default
        kBatch
        kSteepestDescent
        kRibierePolak
        kFletcherReeves
        kBFGS
    */

    mlp->Train( 5000, "text,update=100");

    /*
    |  to train the net work. iteration = 5000
    |  only use text to display the process of training.
    |  we have to observe the error for training and test dataset
    |  to see if they are converge or small.
    */





    // here we prepara the input data
    // and hence we can compare the results
    //
    Int_t nEvent = t->GetEntries();
    Double_t* r  = new Double_t [ nEvent ];
    Double_t* z  = new Double_t [ nEvent ];
    Double_t* Br = new Double_t [ nEvent ];
    Double_t* Br_cal  = new Double_t [ nEvent ];
    fstream readIn;
    readIn.open( "Br.dat", ios::in );
    char buffer[200];
    readIn.getline( buffer, 200 );  
    readIn.getline( buffer, 200 );  
    for( int i=0; i<nEvent; i++) {
        readIn >> r[i] >> z[i] >> Br[i];
    }
    readIn.close();



    //====================================================================
    // STEP2
    // using the trainned ANN.
    //
     
    Double_t inputs[2];
    for( Int_t i=0; i<nEvent; i++ ){

        inputs[0] = r[i];
        inputs[1] = z[i];
        Br_cal[i] = mlp->Evaluate( 0, inputs ); //! look here.
    }
     

    TCanvas* c1 = new TCanvas("c1", "c1", 500, 400);
    c1->Divide( 2,1 );
    c1->cd(1);



    TGraph2D* gr1 = new TGraph2D( nEvent, r, z, Br );
    gr1->SetTitle("original taken from data");
    gr1->Draw( "TRI" );
    // TRI is to use the Delaunay triangles for the filled area.

    c1->cd(2);
    TGraph2D* gr2 = new TGraph2D("gr2", "gr2", nEvent, r, z, Br_cal );
    gr2->SetTitle("From the ANN calculation");
    gr2->Draw( "TRI" );



    //====================================================================
    // STEP3
    // TMLPAnalyzer class demo
    //

    TCanvas* mlp_analysis_canvas = new TCanvas("mlp_analysis","mlp_analysis");
    mlp_analysis_canvas->Divide( 2,2 );

    TMLPAnalyzer* mlp_analyzer = new  TMLPAnalyzer( mlp );
    // put in our trained mlp object.

    // Initialisation
    mlp_analyzer->GatherInformations();

    // print out information to the console
    mlp_analyzer->CheckNetwork();


    mlp_analysis_canvas->cd(1);

    mlp_analyzer->DrawDInputs();

    /*  
    x-axis is the derivative of the NN with respect to each input, y-axis is the number of entries. ie. x-axis represent how the NN change when we change one unit of input so we would like to see there are a lot of events with very low x value.


    What you can learn from that is:
    is variable a really useful, or is my network insensitive to it ?
    is there any risk of big systematic ? 
    Is the network extremely sensitive to small variations of any of my inputs ?


    */



    mlp_analysis_canvas->cd(2);

    mlp->Draw(); //shows the network structure




     mlp_analysis_canvas->cd(3);

     mlp_analyzer->DrawTruthDeviations();
    // draw the difference between the ANN's output for (x,y) and
    // the true value f(x,y), vs. f(x,y), as TProfiles




     mlp_analysis_canvas->cd(4);

     mlp_analyzer->DrawTruthDeviationInsOut();
    // draw the difference between the ANN's output for (x,y) and
    // the true value f(x,y), vs. x, and vs. y, as TProfiles


    //
    // write ANN out to files
    //
    mlp->Export("myANN","python");
    mlp->Export("myANN","c++");
}
