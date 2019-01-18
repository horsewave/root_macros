
 #ifndef _BRAINPET_QUICKCONVERTER_H
 #define _BRAINPET_QUICKCONVERTER_H
 
 
 
 //---project classes
 #include "../utils/inc/Includes_Basic.hh"
 #include "../utils/inc/ClassIDs.hh"
 #include "../utils/inc/StreamReader.hh"
 #include "../utils/inc/StreamWriter.hh"
 
 #include "MRPET_dump.hh"
 
 //---class: BrainPET_QuickConverter
 //----------------------------------------------------------------------------------------
 //----------------------------------------------------------------------------------------
 //---Quick lookup based conversion between LOR file and Sinogram File format
 //---from pure flat files
 //----------------------------------------------------------------------------------------
 //----------------------------------------------------------------------------------------
 //---\brief Conversion LOR <<>> Sinograms
 class BrainPET_QuickConverter
 {
 public:
 
 //---methods:
 
 //---Constructor I
 BrainPET_QuickConverter(void);
 
 //---Constructor II
 //---create Converter instance from file 
 BrainPET_QuickConverter(string converter_filename);
 
 //---conversion create quick Lookup table
 //---input is the original MRPET_dump lookuptable 
 void create_quick_lookup(string lookupfilename);
 
 //---store lookup table (create valid Converter instance)
 void store_lookup_table(string fname);
 
 //---convert flat sinogram file into flat LOR file
 void convert_Sino2LOR(string sino_fname, string lor_fname, 
 bool output_is_float=true);
 
 //---convert flat sinogram file into flat LOR data
 void convert_Sino2LOR(float* sinodata, int nsinodata,
 float* lordata, int nlordata);
 
 //---convert flat LOR file into flat sinogram file
 void convert_LOR2sino(string lor_fname, string sino_fname, 
 bool dwell_correction,
 bool output_is_float=true);
 
 //---convert flat LOR data into flat sinogram file
 void convert_LOR2sino(float* lordata, int nlordata,
 float* sinodata, int nsinodata,
 bool dwell_correction);
 
 
 //---convert flat LOR file into flat sinogram file
 //---specific treatment of data to calculate an effective normalisation
 //---instead of norm factors N the efficiencies (1/N) have to be added (!)
 //---output is float anyway
 void convert_LOR2sino_norm(string lor_fname, string sino_fname, 
 bool dwell_correction);
 
 //---Destructor
 ~BrainPET_QuickConverter();
 
 //---/
 
 private:
 
 
 //---data members:
 
 //---number of LORs in LOR file
 int nlors;
 
 //---number of bins in sinogram file
 int nsino_bins;
 
 //---quick lookup
 int* lookup_fast;
 
 //---sino data (either short or float) 
 float* Sino_Data_float;
 short int* Sino_Data_short;
 
 //---Sino_Data typ (either short or float) 
 bool Sino_Data_IS_FLOAT;
 
 //---LOR data (either short or float)
 float* LOR_Data_float;
 short int* LOR_Data_short;
 
 //---LOR_Data typ (either short or float) 
 bool LOR_Data_IS_FLOAT;
 
 //---dwell map
 short int* dwell_map;
 
 //---/
 
 //---methods:
 
 //---load lookup table in fast format
 void load_from_stream(StreamReader* stream);
 
 //---write instance into stream
 void write_into_stream(StreamWriter* stream);
 
 //---convert from LOR file to sinogram file (as flat data only)
 void convert_LOR2sino(void);
 
 //---convert from inogram file to LOR (as flat data only)
 void convert_sino2LOR(void);
 
 //---generate dwell map
 void generate_dwell_map(void);
 
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
