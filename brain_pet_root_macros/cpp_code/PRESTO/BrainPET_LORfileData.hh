
 #ifndef _BRAINPET_LORFILEDATA_H
 #define _BRAINPET_LORFILEDATA_H
 
 #include <stdlib.h>
 #include <iostream>
 #include <math.h>
 #include <iomanip>
 #include <fstream>
 #include <string.h>
 #include <stdio.h>
 
 using namespace std;
 
 //---ROOT classes
 #include "TCanvas.h"
 #include "TH1.h"
 #include "TH2.h"
 
 //---project classes
 #include "../utils/inc/ClassIDs.hh"
 #include "../utils/inc/StreamReader.hh"
 #include "../utils/inc/StreamWriter.hh"
 #include "../utils/inc/FileAccess.hh"
 
 #include "BrainPET_BasicProperties.hh"
 
 //--- sources from mrpet_dump.c (included by c.w.)
 #include "MRPET_dump.hh"
 
 
 
 //---class: BrainPET_LORfileData
 //----------------------------------------------------------------------------------------
 //----------------------------------------------------------------------------------------
 //---BrainPET_LORfileData basic LORfile data interface
 //----------------------------------------------------------------------------------------
 //----------------------------------------------------------------------------------------
 //---\brief basic LORfile data interface
 class BrainPET_LORfileData
 {
 public:
 
 //---methods:
 
 //---Constructor I
 BrainPET_LORfileData(int n_modul_pairs0, int n_modulpair_size0, bool data_float0=true);
 
 //---Constructor II
 BrainPET_LORfileData(StreamReader* stream);
 
 //---load data from LORdata file
 void load_data(char* filename, bool expect_floats=false);
 
 //---get modul pair data 
 float* get_modul_pair_data_float(int imodul_pair);
 
 //---get modul pair data as array
 short int* get_modul_pair_data_short(int imodul_pair);
 
 //---get modul pair data LOR value
 //---return NULL if not float format
 float get_modul_pair_LOR_value(int imodul_pair, int ilor);
 
 //---set modul pair data LOR value
 void set_modul_pair_LOR_value(int imodul_pair, int ilor, float data0);
 
 //---get min/max values
 void get_min_max_values(float &min, float &max);
 
 //---total sum
 double get_count_sum(void);
 
 //---add data
 void add_data_from(BrainPET_LORfileData* lordata);
 
 //---multiply data with given instance (e.g. for efficiency correction)
 void multiply_data_by(BrainPET_LORfileData* lordata);
 
 //---divide data with given instance (e.g. for efficiency correction)
 void divide_data_by(BrainPET_LORfileData* lordata);
 
 //---subtract data with of instance (negative values possible & not suppressed)
 void subtract(BrainPET_LORfileData* lordata);
 
 //---scale data with factor
 void scale_data(float scale_factor);
 
 //---supress negative values (set to zero)
 void supress_negative_values(void);
 
 //---store LORfileData as short int file
 void store_as_LORfileData_short_int(char *fname, bool overwrite=false);
 
 //---store LORfileData as float file
 void store_as_LORfileData_float(char *fname, bool overwrite=false);
 
 //---write into stream
 void write_into_stream(StreamWriter* stream);
 
 //---reset all channels
 void set_all_channels(float value=0.0);
 
 //---data status (either float or short int)
 bool data_is_float(void);
 
 //---change data status to float (in case of short int)
 void change_data_typ_to_float(void);
 
 //---show frequency of values
 void show_frequency_values(void);
 
 //---shows the coincidences between the defined heads:
 void show_head_pair(int h1, int h2);
 
 //---separate to 1/-1 at threshold: -1 below thress, +1 above thress
 //---to mimic dwell information used in Distribution Keys
 void set_DWELL_threshold(float threshold);
 
 //--- subtract two lor-arrays: "this - lordata" (element-wise)
 void subtract_data_from(BrainPET_LORfileData* lordata);
 
 //--- add event by HBC-notation
 //--- bin is incremented by "value" (e.g. for decay correction)
 //--- if array is short int, value is set to "1" 
 void add_lor_event(int h1, int b1, int c1,
 int h2, int b2, int c2, float value);
 
 //--- get LOR data by flat address; always as float. 
 float get_lor_data(int addr);
 
 //--- get LOR data by flat address; always as float.
 void set_lor_data(int addr, float value);
 
 //--- set MRPET_dump object for code handling of mrpet_dump.c
 void set_mrpet_dump(MRPET_dump *dump_x);
 
 //---replace value below threshold by replace_value
 void set_value_below(float threshold, float replace_value);
 
 //---replace value above threshold by replace_value
 void set_value_above(float threshold, float replace_value);
 
 //---get pointer to data array (float)
 float* get_data_pointer_float(int &size);
 
 //---get pointer to data array (short int)
 short int* get_data_pointer_int(int &size);
 
 //--- return modul pair size (total number of LORs)
 int getMPairSize();
 
 void deleteData(void);
 
 //---Destructor
 virtual ~BrainPET_LORfileData();
 
 //---/
 
 private:
 
 
 //---data members:
 
 //---number of modul pairs 
 int n_mpairs;
 
 //---number of LORs for any modul pair
 int n_mpair_size;
 
 //---total number of elements
 int ntot_channels;
 
 //---data flag (either float or short int)
 bool data_float;
 
 //---data array
 float* data;
 
 //---data array
 short int* data_short;
 
 //---modul pair data pointers
 float** mpair_data;
 
 //---modul pair data pointers
 short int** mpair_data_short;
 
 //---object for usage of mrpet_dump.c (c.w.)
 //---only handle. Class is not owner of the object!!!
 MRPET_dump *dump;
 
 //---/
 
 //---methods:
 
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
