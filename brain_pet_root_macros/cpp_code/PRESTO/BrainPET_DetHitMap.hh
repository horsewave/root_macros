
#ifndef _BPET_DETHITMAP_H
#define _BPET_DETHITMAP_H

#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <iomanip>
#include <fstream>
#include <string.h>
#include <stdio.h>

using namespace std;

//---ROOT stuff 
#include "TCanvas.h"
#include "TH2.h"
#include "TH1.h"
#include "TStyle.h"
#include "TLatex.h"

//---project classes
#include "../utils/inc/FileAccess.hh"
#include "../utils/inc/TextStream.hh"

#include "BrainPET_TypDefs.hh"
#include "BrainPETbasics_ABC.hh"


//---class: BrainPET_DetHitMap
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//---create crystal hit maps of BrainPET insert from LOR data files;
//---central decoding is provided via instance BrainPETbasics (no ownership here)
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//---\brief BrainPET insert crystal hit maps
class BrainPET_DetHitMap
{
public:

  
  //---friend class
  friend class MultipleHitMaps;


  //---methods:

  //---constructor I
  //---no ownership of instance BrainPETbasics_ABC 
  BrainPET_DetHitMap(string name0, BrainPETbasics_ABC* bpet_basics0);


  //---constructor II
  //---no ownership of instance BrainPETbasics_ABC 
  BrainPET_DetHitMap(char* name0, BrainPETbasics_ABC* bpet_basics0);


  //---constructor II
  //---no ownership of instance BrainPETbasics_ABC 
  BrainPET_DetHitMap(StreamReader* stream, BrainPETbasics_ABC* bpet_basics0);

  //---destructor
  ~BrainPET_DetHitMap();

  //---draw hit map
  void draw_hitmap(void);  

  //---get hit map
  TH2D* get_hitmap(string name0);  

  //---draw hit map (using absolute values for color scale)
  void draw_hitmap(float min_col_scale, float max_col_scale);
   
  //---draw control plots
  void draw_control_plots(void);

  //---add header information 
  void load_header(char* header_fname);

  //---print header (if existing)
  void print_header(void);

  //---create LOR file data as product of detector hitmap crystal values of any combination
  //---LOR file data must be in float format!
  void fill_LORfiledata_as_products(BrainPET_LORfileData* lordata);
  
  //---add LOR file data (eihter short int or floats)
  void add_LORfiledata(BrainPET_LORfileData* lordata);

  //---reset all
  void reset(void);

  //---set all crystals to value
  void set_all_crystals_to_value(float value);

  //---set crystal values
  void set_crystal_value(int ihead1, int iblock1, int iaxial1, int itransax1,
			 float val);

  //---fill control plots
  void fill_control_plots(int ihead1, int iblock1, int iaxial1, int itransax1, 
			  int ihead2, int iblock2, int iaxial2, int itransax2, 
			  float val);

  //---get crystal combination product
  float get_crystal_combi_product(int ihead1, int iblock1, int iaxial1, int itransax1, 
				  int ihead2, int iblock2, int iaxial2, int itransax2);

  //---get crystal combination product (typ II)
  float get_crystal_combi_product(crystal_pos_typ c1, crystal_pos_typ c2);
  

  //---divide block hit map by another hit map to see any significant differences
  void divide_hit_map_by(BrainPET_DetHitMap* hitmap);

  //---subtract block hit map by another hit map
  void subtract_hit_map_by(BrainPET_DetHitMap* hitmap);

  //---compute 1/crystal-value for each crystal
  //---if divided by zero, set crystal value to "div_zero_val"
  void compute_bin_inverse(float div_zero_val = 0.0);

  //---scale map
  void scale(float factor);

  //---add hitmap to this 
  void add_hit_map_by(BrainPET_DetHitMap* hitmap);

  //---decay correction F18; input time in sec.
  void apply_decay_correction(float time_sec);

  //---fill control plots (typ II)
  void fill_control_plots(crystal_pos_typ c1, crystal_pos_typ c2, float val);

  //---get number of crystals
  int get_number_of_crystals(void);

  //---get hit map value for
  //---crystal_index<ncrystals
  float get_hit_map_value(int crystal_index);

  //---set hit map value for
  //---crystal_index<ncrystals
  void set_hit_map_value(int crystal_index, float value);

  //---get hit map number of contributing LORs fo
  //---crystal_index<ncrystals
  int get_number_of_contributing_LORs(int crystal_index);

  //---get total block sum
  float get_total_block_sum(int ihead, int iblock);

  //---get total EPM sum (of four blocks)
  //---input: iEPM<48
  float get_total_EPM_sum(int iEPM);

  //---set all crystals of block
  void set_all_crystals_of_block(int ihead, int iblock, float value);

  //---set block average value to all crystals of block
  void make_block_average(void);

  //---normalise blocks to block average
  void normalise_blocks_to_block_average(void);

  //---normalise to number of contributing channels
  void normalise_to_number_of_contributing_channels(void);

  //---write hitmap object into file (not Siemens flat format!)
  void write_into_file(StreamWriter* stream);

  //---get pointer of BrainPETbasics_ABC
  BrainPETbasics_ABC* get_BrainPETbasics_handle(void);

  //---load Siemens dcmap & pcmap
  //---for files with two subsequent blocks of hitmaps bool second__block==true
  //---selects the second part (hopefully existing)
  void load_Siemens_hitmap(char* fname, bool second_block=false);

  //---get crystal index
  int get_crystal_index(int ihead, int iblock, int iaxial, int itransaxial);

  //---get crystal address
  void get_crystal_address(int  index, int &ihead, int &iblock, 
			   int &iaxial, int &itransaxial);


  //---displays mean of all 192 blocks
  void draw_mean_block();


  //---multiply block hit map by another hit map to see any significant differences
  void multiply_hit_map_by(BrainPET_DetHitMap* hitmap);

  //---get total hitmap sum
  float get_total_hitmap_sum();

  //---set value of crystal "index" to "value"
  void set_crystal_value(int index, float value);

  //---normalise blocks to block average of hmap
  void normalise_blocks_to_block_average(BrainPET_DetHitMap* hmap);

  //---write into file as siemens flat format (dcmap and pcmap)
  void write_Siemens_hitmap(StreamWriter *w_stream);

  //---write into file as siemens flat format (dcmap and pcmap)
  void write_Siemens_hitmap(string filename);

  //---write into file as siemens flat format (FLOAT format)
  void write_Siemens_hitmap_float(string filename);

  //---load siemens flat format (FLOAT format)
  void load_Siemens_hitmap_float(string filename);




  //---set value of crystal "index" to "value"
  void add_crystal_value(int index, float value);


  //---add single crystal value 
  void add_single_crystal(int ihead1, int iblock1, int iaxial1, int itransax1, float val);

  //---get single crystal value
  float get_single_crystal(int ihead1, int iblock1, int iaxial1, int itransax1);

  //---divide each crystal value by crystal event counter:
  void compute_mean_event();

  //---draw distribution of values
  //---return histogram object
  TH1F* draw_histogram();

  //---get min and max values by reference
  void get_min_max(float &min_val, float &max_val);

  //---retun data pointer:
  float *get_data(int &length);

  //---set edge and corner crystals of map to defined value 
  void mask_edge_and_corner_crystals(float value=0.0);

  //---/

private:



  //---data members:

  //---basic geometry & decoding of BrainPET data
  BrainPETbasics_ABC* bpet_basics;

  //---dimensions
  int nheads;
  int nblocks_per_head;
  int naxial_block;
  int ntrans_block;

  //---some abbreviations
  int ncrystals;
  int iblock_size;
  int ihead_size;

  //---file header (optional)
  TextStream* fheader;

  //---data
  float* data;

  //---number of contributing LORs per crystal
  int* Ndata;

  //---name of hitmap
  char name[200];

  //---canvas
  TCanvas* can;

  //---histogram
  TH2D* h2;

  //---canvas
  TCanvas* can2;

  //---histogram
  TH1F* h[10];

  //---/


  //---methods:

  //---global constructor init
  void init(BrainPETbasics_ABC* bpet_basics0);

  //---/


};  

#endif
