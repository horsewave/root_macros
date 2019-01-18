
#ifndef _BRAINPET_SINOGRAMS_HH_
#define _BRAINPET_SINOGRAMS_HH_

#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <iomanip>
#include <fstream>
#include <string.h>
#include <stdio.h>

using namespace std;


//--- utils
#include "../../utils/inc/FileAccess.hh"
#include "../../utils/inc/TextStream.hh"
#include "../../utils/inc/StreamWriter.hh"
#include "../../utils/inc/StreamReader.hh"


//--- BrainPET_data
#include "TimingScheme.hh"
#include "MMatrix.hh"
#include "MRPET_dump.hh"
#include "R_BrainPET_SinogramData.hh"
#include "BrainPET_LORfileData.hh"

//---ROOT classes
#include "TCanvas.h"
#include "TH1.h"


//---class: BrainPET_Sinograms
//-----------------------------------------------
//---author: Christoph Weirich
//---Class to conatain sinogram structure:
//---flat format (siemens) with array of nViews*nProjs*nPlanes
//---Container for objects nPlanes x BrainPET_SinogramData Sinograms
//---Datatype is float!
//-----------------------------------------------
//---\brief sinogram handling
class BrainPET_Sinograms
{

  public:

    //---methods:

    //---Constructor I
    //---Class IS NOT the owner of MRPET_dump object (care for delete)
    BrainPET_Sinograms(MRPET_dump *brainPET1, string title0="__undefined", 
        int verbose_level = 0);

    //---conversion from BrainPET_LORfileData to BrainPET_Sinograms
    //---LOR-file contains data, that will be filled into this sinogram
    void LOR2Sino(BrainPET_LORfileData *lor_data,
        bool use_lookup=false,
        char* lookup_filename=NULL);


    //---conversion from BrainPET_LORfileData to BrainPET_Sinograms
    //---LOR-file contains data, that will be filled into this sinogram
    void LOR2Sino(BrainPET_LORfileData *lor_data,
        int* lookup);


    //---conversion from BrainPET_Sinograms to BrainPET_LORfileData
    //---LOR-file will be overwritten with data from sinogram
    void sino2LOR(BrainPET_LORfileData *lor_data,
        bool use_lookup=false,
        char* lookup_filename=NULL,
        bool sino_contains_dwell=true);



    //---adds an event to sinogram
    void add_event_to_sinogram(int h1, int b1, int crys1, 
        int h2, int b2, int crys2);

    //---add an event to sinogram by flat adress (double value):
    void add_event_to_sinogram1(int addr, float value);

    //---get event from flat adress
    float get_event_from_address(int addr);

    //---replace value below threshold by replace_value
    void set_value_below(float threshold, float replace_value);

    //---replace value above threshold by replace_value
    void set_value_above(float threshold, float replace_value);

    //---set values in sinogram plane to value
    void set_single_plane_to_value(int planenumber, float value=0.0);

    //---set event at flat adress
    void set_event_at_address(int addr, float value);

    //---set title
    void set_title(string title0);

    //---apply dwell correction to sinogram
    //---load lookuptable from file or compute it
    bool apply_dwell_correction(bool use_lookup=false,
        char* lookup_filename=NULL);


    //---compute bin-wise inverse of each bin value
    //---if bin value == 0, set inverse to bin_zero_value (default 0)
    bool compute_bin_inverse(float bin_zero_value=0.0);

    //---apply dwell correction to *this
    bool apply_dwell_correction(BrainPET_Sinograms *dwell_sino);

    //---compute dwell sinogram (set all bins in LOR file to 1 and 
    //---convert to sinogram
    bool compute_dwell_sino(BrainPET_Sinograms *dwell_sino, bool use_lookup=false,
        char* lookup_filename=NULL);

    //---scale every sinogram bin by value (simple multiplication)
    void scale_by_value(float value);

    //---add sinogram to this 
    void add_sinogram(BrainPET_Sinograms *sinogram);

    //---subtract sinogram from *this 
    void subtract_sinogram(BrainPET_Sinograms *sinogram);


    //---divide this sinogram element wise by "sinogram"
    void divide_by_sinogram(BrainPET_Sinograms *sinogram);

    //---multiply this sinogram element wise by "sinogram"
    void multiply_by_sinogram(BrainPET_Sinograms *sinogram);


    //---write sinogram object into stream
    void write_into_stream(StreamWriter *w_stream);

    //---read sinogram object from stream
    void load_from_stream(StreamReader *stream);

    //---writes flat sinograms by filename
    void write_flat_sinograms(string filename,
        bool writeAsShortInt);

    //---writes flat sinograms by stream
    void write_flat_sinograms(StreamWriter *stream,
        bool writeAsShortInt);



    //---reads sinogram by filename
    void read_flatSinogram(string filename, 
        bool isShortInt);


    //---reads sinogram from StreamReader object
    void read_flatSinogram_from_stream(StreamReader *stream, 
        bool isShortInt);


    //---sets prompt and delayed sinogram values to zero
    void resetAllSinograms();

    //---set all bins with value (shows no dwell!)
    void setAllSinogramBins(float value);

    //---fill sinogram homogeniously with value
    void fill_sinogram_homogeniously(float value=1.0);

    //---shows sinogram by plane number
    void showSinogram(int plane);

    //---shows sinogram by segment and z position
    void showSinogram(int segment, int z);


    //---number of delayed events
    double get_n_Events();


    //---function to make Siemens 3D-scatterestimation usable 
    //---for PRESTO
    //---USE WITH CAUTION!!!!
    void copy_planes_for_hi_scatter_est();

    //---number of planes (sinograms)
    int get_n_planes(); 

    //---number of views (angle)
    int get_n_views(); 

    //---number of projections (radial offset)
    int get_n_projs(); 

    //---return pointer to plane 
    R_BrainPET_SinogramData* get_plane(int planeNumber);

    //---show frequency of values
    void show_frequency_values();

    //---show sum values for every plane;
    void show_plane_sums();

    //---get min/max values
    void get_min_max_values(double &min,double &max);

    //---get float float data space handle
    float* get_handle_flat_float(int &nfloats);

    //---destructor
    ~BrainPET_Sinograms();

    //---/

  private:


    //---data members:

    //---span
    int span;

    //---sinogram dimensions
    int nPlanes;
    int nViews;
    int nProjs;

    //---maximum ring difference
    int maxrd; 

    //---plane separation
    float plane_sep;

    //---crystal radius
    float crystal_radius;

    //---current version of data format
    unsigned short int format_version;

    //---total number of events
    double nEvents_all;

    //---geometry information of scanner
    MRPET_dump *BrainPET1;

    //---sinogram object sructure
    R_BrainPET_SinogramData **sinograms;

    //---sinogram flat data
    float *sinogram_flatformat;

    //---unique title
    string title;

    //---verbose level
    int verbose;

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
//Generated on Tue Jun 18 17:17:16 2013 for *** PRESTO *** Pet Reconstruction Software TOolbox *** revision 333 *** by   doxygen 1.5.6
