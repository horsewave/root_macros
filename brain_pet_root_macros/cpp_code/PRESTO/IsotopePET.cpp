
 #include "IsotopePET.hh"
 
 
 //----------------------------------------------------------------------------------------
 //---constructor
 //----------------------------------------------------------------------------------------
 IsotopePET::IsotopePET(string isotope_x, int verbose0)
 {
   verbose=verbose0;
 
   if (verbose>0)
     {
       cout << "IsotopePET::IsotopePET(" << isotope_x  << ")" << endl;
     }
 
   isotope_number=0;  
   halflife_time=0;  
   name = "";
   set_isotope(isotope_x); 
 
 }
 
 
 //----------------------------------------------------------------------------------------
 //---destructor
 //----------------------------------------------------------------------------------------
 IsotopePET::~IsotopePET()
 {
  
 }
 
 
 //----------------------------------------------------------------------------------------
 //---define specific isotope
 //----------------------------------------------------------------------------------------
 bool IsotopePET::set_isotope(string isotope_x)
 {
   //---define list of isotopes
   //---halflife time in seconds:
   const float hlt_list[6]={9.0, 6586.2, 1218.0, 121.8, 606.0, 400.0};
 
   //---branching fractions in %:
   const float bf_list[6]={ 1.0, 0.9673, 0., 0., 0.6, 0.8994};
 
   //---name of radio-isotope:
   const string isotope_names[6]={"No_Isotope_defined", "F-18", "C-11", "O-15", "N-13", "Ge-68"};
 
   bool isotope_set = false;
 
   char iso;
   iso=isotope_x[0];
 
   switch(iso){
   case 'x': isotope = 'X';
     isotope_number = 0;
     isotope_set = true;
     break;
   case 'X': isotope = 'X';
     isotope_number = 0;
     isotope_set = true;
     break;
   case 'f': isotope = 'F';
     isotope_number = 1;
     isotope_set = true;
     break;
   case 'F': isotope = 'F';
     isotope_number = 1;
     isotope_set = true;
     break;
     
   case 'c': isotope = 'C';
     isotope_number = 2;
     isotope_set = true;
     break;
   case 'C': isotope = 'C';
     isotope_number = 2;
     isotope_set = true;
     break; 
     
   case 'o': isotope = 'O';
     isotope_number = 3;
     isotope_set = true;
     break;
   case 'O': isotope = 'O';
     isotope_number = 3;
     isotope_set = true;
     break;
    
   case 'n': isotope = 'N';
     isotope_number = 4;
     isotope_set = true;
     break;
   case 'N': isotope = 'N';
     isotope_number = 4;
     isotope_set = true;
     break;
 
   case 'g': isotope = 'G';
     isotope_number = 5;
     isotope_set = true;
     break;
   case 'G': isotope = 'G';
     isotope_number = 5;
     isotope_set = true;
     break; 
 
     
   default:  
 
     cout << "IsotopePET not well defined!" << endl;
     cout << "set 'x' or 'X' for NO ISOTOPE" << endl
            << "set 'c' or 'C' for C-11" << endl
            << "set 'f' or 'F' for F-18" << endl
            << "set 'o' or 'O' for O-15" << endl
            << "set 'n' or 'N' for N-13" << endl
            << "set 'g' or 'G' for Ge-68" << endl;
     isotope_set = false;    
     break;
   }
   
 
   if (isotope_set)
     {
       halflife_time = hlt_list[isotope_number];
       branching_fraction = bf_list[isotope_number];
       name = isotope_names[isotope_number];
       if (verbose>0) 
           {
             cout << "IsotopePET is set to " << name << endl;
           }
     }
   return isotope_set;
 }
 
 
 //----------------------------------------------------------------------------------------
 //---get decay correction factor
 //----------------------------------------------------------------------------------------
 double IsotopePET::get_decay_correction_factor(int istart_time_sec, int istop_time_sec)
 {
   string method="IsotopePET::get_decay_correction_factor: ";
   if (isotope_number==0)
     {
       cout << "Error: No Isotope defined" << endl;
       return 0.0;
     }
 
   double lambda=log(2)/halflife_time;
 
   //---check
   double unit=2.0*exp(-halflife_time*lambda);
   if (fabs(unit-1.0)>1E-7)
     {
       cout << method << "Error: Bug in decay correction!" << endl;
       cout << "unit=" << unit << endl;
       exit(0);
     }
   if ((istart_time_sec<0) || (istop_time_sec<0) || 
       (istop_time_sec<=istart_time_sec))
     {
       cout << method << "Error: invalid time range: ";
       cout << istart_time_sec << " - " << istop_time_sec << endl;
     }
 
   double nstatX=0.0;
   int nticks=0;
   for (int ii=istart_time_sec; ii<istop_time_sec; ii++)
     {
       double t=double(ii);
       double val=exp(-t*lambda);
       nstatX+=val;
       nticks++;
     }
 
   double decay_corr_factor=double(nticks)/nstatX;
   return decay_corr_factor;
 }
 
 
 
 //----------------------------------------------------------------------------------------
 //---
 //----------------------------------------------------------------------------------------
 float IsotopePET::get_halflife_time(void)
 {
   return halflife_time;
 }
 
 
 //----------------------------------------------------------------------------------------
 //----------------------------------------------------------------------------------------
 string IsotopePET::get_isotope_name(void)
 {
   return name;
 }
 
 //----------------------------------------------------------------------------------------
 //----------------------------------------------------------------------------------------
 float IsotopePET::get_branching_fraction(void)
 {
   return branching_fraction;
 }
 
 
 //----------------------------------------------------------------------------------------
 //----------------------------------------------------------------------------------------
 void IsotopePET::get_info(void)
 {
   cout << "-------------------------------" << endl;
   cout << "IsotopePET info:" << endl;
   cout << "name: " << name << endl;
   cout << "half life: " << halflife_time << " s" << endl;
   cout << "branching fraction: " <<  branching_fraction << endl;
   cout << "-------------------------------" << endl;
 }
//Generated on Tue Jun 18 17:17:17 2013 for *** PRESTO *** Pet Reconstruction Software TOolbox *** revision 333 *** by  ￼ 1.5.6
