
 #ifndef __BRAINPET_VARIANCE_REDUCTION__
 #define __BRAINPET_VARIANCE_REDUCTION__
 
 #include <stdlib.h>
 #include <iostream>
 #include <math.h>
 #include <iomanip>
 #include <fstream>
 #include <string.h>
 #include <stdio.h>
 
 #include "BrainPET_DetHitMap.hh"
 #include "BrainPET_LORfileData.hh"
 #include "BrainPET_BasicProperties.hh"
 
 
 //---class: BrainPET_VarianceReduction
 //--------------------------------------
 //---\brief class to generate a variance reduced
 //--- (VRR)-LOR file from a single DCMAP
 //---
 //---Date: 06.10.2009
 //--------------------------------------
 class BrainPET_VarianceReduction{
 
 public:
 
 //---methods:
 
 //---Constructor
 BrainPET_VarianceReduction(int cwin_x=19);
 
 //---Destructor
 ~BrainPET_VarianceReduction();
 
 //---calculate VR-LORdata from crystal hitmap (flat DCMAP):
 int variance_reduction(char *dcmap_filename, BrainPET_LORfileData *lor_data);
 
 //---calculate VR-LORdata from crystal hitmap (BrainPET_DetHitMap):
 int variance_reduction(BrainPET_DetHitMap *dcmap0, BrainPET_LORfileData *lor_data);
 
 //---/
 
 private:
 
 //---data members:
 
 //---see mrpet_dump
 int *hpairs;
 int *hpmap;
 float *srates;
 int *hp_list;
 float *destimate;
 float *hrates;
 float *off_rates;
 float df;
 int n_heads;
 int fov;
 int cwin;
 float tau;
 int n_hpairs;
 int verbose;
 
 //---/
 
 //---methods:
 
 //---kernel implementation of variance reduction
 int variance_reduction_kernel(BrainPET_LORfileData *lor_data);
 
 //--- calculate single rates
 //---interface filename flat:
 void calc_srates(char *dcmap_filename);
 
 //---interface BrainPET_DetHitMap:
 void calc_srates(BrainPET_DetHitMap *dcmap0);
 
 //---implementation of calc_scates kernel:
 void calc_srates_kernel(float *dcmap);
 
 //---estimation of delay factor:
 float estimate_delay_factor(int cwin, int offset);
 
 //---initialise head pair data
 void init_hpair_data(int fov);
 
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
//Generated on Tue Jun 18 17:17:16 2013 for *** PRESTO *** Pet Reconstruction Software TOolbox *** revision 333 *** by  ￼ 1.5.6
