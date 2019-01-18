/*
    How to use the macro
    in the termainal $ root "pin_image.cxx (\"xxx.png\")"
    or in the CINT promt >>.x pin_image.cxx ( "xxx.png" )

*/





//--------------------------------------------//
// for counting the clicks

class FOR_COUNTING {

    public:
        FOR_COUNTING() {}
        static int count;
};

int FOR_COUNTING::count = 0;

//--------------------------------------------//






fstream writeout;
//--------------------------------------------------//
// for print and write out the coordinates
void GetCoordinate()
{
    int event = gPad->GetEvent();
    if( event != 11) return;

    int coordinate_x;
    int coordinate_y;



    TObject* select = gPad->GetSelected();
    // if( select) cout << "clicked" <<endl; 
    // when you click, this print 'click'..
    if( select )
    {
        FOR_COUNTING::count += 1 ;
        coordinate_x = gPad->GetEventX();
        coordinate_y = gPad->GetEventY();


        // print info to terminal
        if ( FOR_COUNTING::count  ==1 ) cout << "\n";
        cout << Form( "No. %d  ( %d, %d )", FOR_COUNTING::count,
         coordinate_x, coordinate_y ) << endl;

        // write out info
        writeout.open("coorindate.txt", ios::out | ios::app   );
        TString strOut = Form( "%d\t%d\t%d\n", FOR_COUNTING::count,
         coordinate_x, coordinate_y );
        writeout << strOut.Data();
        writeout.close();

    }
}
//--------------------------------------------------//




void pin_image( const char* filename = "koehler86_fig8.png" )
{

    //!! preload the setting in .rootrc !!!
    // set editor and event status bar to be alive



    // open the file
    TImage *img = TImage::Open( filename );
    writeout.open("coorindate.txt",ios::out); writeout.close();




    // create a canvas with 1200 x 800 pixels.
    TCanvas* c1 = new TCanvas("c1","c1",1200,800);



    // we add a command "GetCoordinate()" to Canvas,
    // when there is an event -- mouse click, 
    //it will run "GetCoordinate()"
    c1->AddExec("to print coordinate", "GetCoordinate()");


    // set crosshair to be alive
    gPad->SetCrosshair(1);

    img->Draw();
}
