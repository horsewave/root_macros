/*
*
*   the basic of a gui
*
*******************************/
class SIMPLEGUI : public TGMainFrame  {
// our class is base on  TGMainFrame

private:


public:

    SIMPLEGUI(const TGWindow *p, UInt_t w, UInt_t h);
    /* main windos, width, height */

    void CloseWindow();

    virtual ~SIMPLEGUI();


};


// constructor
SIMPLEGUI::SIMPLEGUI(const TGWindow *p,UInt_t w,UInt_t h):
TGMainFrame ( p, w, h ) // <-- main frame is created here.
{

    SetCleanup(kDeepCleanup); // important step for closing windows properly.


    // ----------------------------------------------------------------|
    // Frame


    // Create a horizontal frame widget with buttons
    TGHorizontalFrame *hframe = new TGHorizontalFrame( this, 200, 40);
    // parent frame = this TGMainFrame obj.
    // Create a horizontal frame widget with buttons.
    // width=200, height =  40,


    // ----------------------------------------------------------------|






    //------------------------------------------------------------------|
    // Text Button
    //
    TGTextButton *exit = new TGTextButton( hframe, "&Exit");
    // parent frame = hframe
    // text on the button = "Exit" (alter+E)


    exit-> Connect( "Clicked()", "SIMPLEGUI", this, "CloseWindow()"  );
    // when get a signal "Clicked()"
    // then go to this obj , and apply CloseWindows().

    //------------------------------------------------------------------|





    //------------------------------------------------------------------|
    // Placement
    //

    TGLayoutHints* layout1 = new TGLayoutHints(kLHintsCenterX, 5,5,3,4);
    // to center on X axis, padding (L, R, U, D ) = 5, 5, 3, 4

    TGLayoutHints* layout2 = new TGLayoutHints(kLHintsCenterX, 2, 2, 2, 2);

    hframe->AddFrame( exit,  layout1  );

    this->AddFrame( hframe,  layout2 ); // you can ignore this->

    //------------------------------------------------------------------|



    SetName("Simple_GUI");
    SetWindowName("Simple gui"); // Set a name to the main frame
    MapSubwindows(); // Map all subwindows of main frame
    Resize( GetDefaultSize() ); // Initialize the layout algorithm
    MapWindow();     // Map main frame
}


void SIMPLEGUI::CloseWindow(){
    DeleteWindow();
}

SIMPLEGUI::~SIMPLEGUI() {
   DeleteWindow();
}



void _00_just_one_button() {

   // Popup the GUI...
   new SIMPLEGUI( gClient->GetRoot(), 200, 200 );

}


