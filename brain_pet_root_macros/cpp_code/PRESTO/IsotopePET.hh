
 #ifndef __ISOTOPEPET__
 #define __ISOTOPEPET__
 
 
 //---project classes
 #include "../utils/inc/Includes_Basic.hh"
 #include "../utils/inc/ClassIDs.hh"
 #include "../utils/inc/StreamReader.hh"
 #include "../utils/inc/StreamWriter.hh"
 
 
 //---class: IsotopePET
 //----------------------------------------------------------------------------------------
 //---handle declaration of isotopes
 //---\brief IsotopePET infos
 //----------------------------------------------------------------------------------------
 class IsotopePET
 {
 
 public:
 
 //---methods:
 
 //---Constructor I
 IsotopePET(string isotope_x="x", int verbose0=0);
 
 
 //---Destructor:
 ~IsotopePET(void);
 
 //---half life time
 float get_halflife_time(void);
 
 //---BR
 float get_branching_fraction(void);
 
 //---Name
 string get_isotope_name(void);
 
 //---define 
 bool set_isotope(string isotope_x);
 
 //---get decay correction factor
 double get_decay_correction_factor(int istart_time_sec, 
 int istop_time_sec);
 
 //---dump
 void get_info(void);
 
 //---/
 
 
 private:
 
 //---data members:
 
 //---verbose 
 int verbose;
 
 //---isotope char
 char isotope;
 
 //---unique number
 int isotope_number;
 
 //---half life time
 float halflife_time;
 
 //---BR
 float branching_fraction;
 
 //---Name
 string name;
 
 //---/
 
 
 }; 
 
 #endif
 //----------------------------------------------------------------------
 //--------------DOXGEN Classdecription from preprocessing --------------
 //--------------preproc_classdescrip.pl --------------------------------
 //----------------------------------------------------------------------
 //---DoxygenTAG1
 //----------------------------------------------------------------------
 //----------------------------------------------------------------------
 //--------------DOXGEN Classdecription from preprocessing --------------
 //--------------preproc_datamembers.pl ---------------------------------
 //----------------------------------------------------------------------
 //---DoxygenTAG2
 //----------------------------------------------------------------------
 //--------------DOXGEN Classdecription from preprocessing --------------
 //--------------preproc_methods.pl -------------------------------------
 //----------------------------------------------------------------------
 //---finished
 //---DoxygenTAG3
 //----------------------------------------------------------------------
 //--------------DOXGEN Classdecription from preprocessing --------------
 //--------------preproc_struct.pl --------------------------------------
 //----------------------------------------------------------------------
 //----------------------------------------------------------------------
 //---DoxygenTAG4
//Generated on Tue Jun 18 17:17:17 2013 for *** PRESTO *** Pet Reconstruction Software TOolbox *** revision 333 *** by  ￼ 1.5.6
