
 #ifndef _BRAINPETBASICS_ABC_H
 #define _BRAINPETBASICS_ABC_H
 
 #include <stdlib.h>
 #include <iostream>
 #include <math.h>
 #include <iomanip>
 #include <fstream>
 #include <string.h>
 #include <stdio.h>
 
 using namespace std;
 
 //---project classes
 #include "../utils/inc/TypDefs_Basic.hh"
 #include "../utils/inc/FileAccess.hh"
 #include "../utils/inc/Array2D_NxM.hh"
 
 //---BrainPET specific codes 
 #include "BrainPET_TypDefs.hh"
 #include "MMatrix.hh"
 #include "BrainPET_LORfileData.hh"
 #include "EPMpattern.hh"
 
 
 //---class: BrainPETbasics_ABC
 //----------------------------------------------------------------------------------------
 //----------------------------------------------------------------------------------------
 //---elementary geometry infos of BrainPET system; implemented are convienient addressing
 //---schemes (ring-wise vs. hardware block-wise);
 //---offer also original coordinates of crystals ala Siemens convention
 //----------------------------------------------------------------------------------------
 //----------------------------------------------------------------------------------------
 //---\brief elementary geometry infos of BrainPET system
 class BrainPETbasics_ABC: public EPMpattern
 {
 public:
 
 
 //---methods:
 
 //---constructor
 BrainPETbasics_ABC(void);
 
 //---destructor
 virtual ~BrainPETbasics_ABC();
 
 //---get number of crystals
 int number_of_crystals(void);
 
 //---get crystal coordinates for index=[0..number_of_crystals[ 
 point3D_typ crystal_coordinates(int index);
 
 //---get crystal coordinates
 //---iaxial -> relative ring number with respect to block
 //---itransaxial -> relative crystal number with respect to block
 point3D_typ crystal_coordinates(int ihead, int iblock, 
 int iaxial, int itransaxial);
 
 //---get crystal coordinates
 point3D_typ crystal_coordinates(crystal_pos_typ &cpos);
 
 //---get crystal coordinates for index=[0..number_of_crystals[ 
 //---Siemens convention not used for PRESTO virtual scanner approach
 //---Note: units are centimeter (!) [cm] not meter as the others (!)
 point3D_typ crystal_coordinates_SiemensConvention(int index);
 
 //---get crystal address
 //---for index=[0..number_of_crystals[ 
 //---crystal_pos: ihead=[0..32[; iblock=[0..6[; iaxial_crystal=[0..12[; itransaxial_crystal=[0..12[;
 crystal_pos_typ get_crystal_address(int index);
 
 //---get crystal address
 //---for index=[0..number_of_crystals[ 
 //---crystal_posII: icrystal_on_ring=[0..12*32[; iring=[0..72[; 
 crystal_pos_typII get_crystal_address_typII(int index);
 
 //---get crystal index
 //---crystal_pos: ihead=[0..32[; iblock=[0..6[; iaxial_crystal=[0..12[; itransaxial_crystal=[0..12[;
 int get_crystal_index(crystal_pos_typ &crystal_pos);
 
 //---decode address to ringID=[0..72[ & crystalID=[0..12*32=384[
 crystal_pos_typII convert_address(crystal_pos_typ &crystal_pos);
 
 //---decode address from ringID=[0..72[ & crystalID=[0..12*32=384[ to blockwise addressing
 crystal_pos_typ convert_address(crystal_pos_typII &crystal_pos);
 
 //---get number of sequential blocks (i.e. head combinations resp. modul pairs)
 //---and number of elements in each block (same for all blocks)
 void get_sequential_data_sizes(int &nblocks, int &nblock_size);
 
 //---get decoded crystal combinations for specific sequential block of modul pair
 //---arrays ic1 and ic2 must exist and have length mpsize!
 void get_crystal_combis_for_modulpair(int iblock, int* ic1, int *ic2);
 
 //---some basics properties
 int get_number_of_heads(void);
 int get_nblocks_per_head(void);
 int get_nrystals_per_block_axial(void);
 int get_nrystals_per_block_transaxial(void);
 
 //---create LORfile reader (either for short int or float data)
 //---Note, the created instance goes beyond this scope, so take care of ownership
 BrainPET_LORfileData* get_LORfile_reader(bool use_floats=false);
 
 //---/
 
 
 protected:
 
 
 //---struct:
 //---typ to decode addressing within one Modul combination (private structure only)
 struct modul_combi_decoder_lookup_typ
 {
 short int iax;
 short int iay; //---0..71
 short int ibx;
 short int iby; //---0..71
 };
 
 
 //---data members:
 
 //---basic variables for init
 int mod_nx; //---transaxial number of crystals per block
 int mod_ny; //---axial number of crystals (6x12=72) for 6 blocks 12 crystals each
 int mod_nrings; //---number of blocks per modul
 int maxrd; //---default ring differnce (if used)
 int nrings; //---number of rings (including gaps)
 int n_mods; //---number of moduls 
 int mpsize; //---modul pair size
 int n_mpairs; //--- number of modul pairs
 int mod_fov; //---fan size of opposite moduls
 int* mpair_a; //---modul number A of modul pair
 int* mpair_b; //---modul number B of modul pair
 float crystal_radius; //---radius
 int ncrystals; //---number of crystals (total)
 int ncrystals_per_ring;//---number of crystals per ring
 float binsize; //---bin size
 
 
 //---coordinates of all points (crystal center points)
 point3D_typ* cpoint;
 
 //---coordinates of all points ala mrpet_dump (Siemens convention)
 point3D_typ* cpoint_SiemensConvention;
 
 //---decoding pattern for all crystals
 crystal_pos_typ* icrystal_adr;
 
 //---decoding pattern for all crystals (ring-wise sorted)
 //---crystal_posII: icrystal_on_ring=[0..12*32[; iring=[0..72[; 
 crystal_pos_typII* icrystal_adr_typII; 
 
 //---decode addressing within one Modul combination
 modul_combi_decoder_lookup_typ* decoder_lookup;
 
 //---true ringID lookup table (account for gaps)
 int* true_ringID;
 
 //---modul pair 1D ID for modul number combination
 Array2D_NxM<int>* mod_pairID;
 
 //---/
 
 
 //---methods:
 
 //---init of modul pair data
 void init_mpair_data(void);
 
 //---compute crystal positions
 void compute_crystal_positions(void);
 
 //---get crystal index
 //---imod=[0..32[ ix=[0..12[ iy=[0..72[
 int get_crystal_index(int imod, int ix, int iy);
 
 //---get physical coordinates
 void det_to_phy(int mod, int x, int y, point3D_typ &ccoord);
 
 //---get physical coordinates in Siemens convention
 void det_to_phy_SiemensConvention(int mod, int x, int y, point3D_typ &ccoord);
 
 //---calculate true ring difference which can be larger due to
 //---existing ring gaps (iy1,iy2 < 72)
 int get_true_ring_diff(int iy1, int iy2);
 
 //---get unique crystal position in terms of unique transaxial crystal number [0..12x32=384[
 int get_crystalID_on_ring(crystal_pos_typ &crystal_pos);
 
 //---get unique crystal position in terms of unique transaxial crystal number [0..12x32=384[
 //---ihead -> modul number [0..32[
 //---itransaxial -> relative crystal number with respect to block
 int get_crystalID_on_ring(int ihead, int itransaxial);
 
 //---get unique ring number [0..72[
 int get_ringID(crystal_pos_typ &crystal_pos);
 
 //---convert ringID in true ringID (considering gaps)
 int get_true_ringID(int iringID);
 
 //---hardware coding/decoding of modul pair data index
 void decode(int ipair_data, int &ay, int &ax, int &by, int &bx);
 
 //---hardware coding/decoding of modul pair data index
 //---return impair index
 int code(int ay, int ax, int by, int bx);
 
 //---find modul pair ID
 int get_modul_pairID(int imodA, int imodB);
 
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
 // 
 // 
 // 
 // 
 // 
   //---struct:
   //---typ to decode addressing within one Modul combination (private structure only)
   struct modul_combi_decoder_lookup_typ
   {
     short int iax;
     short int iay;  //---0..71
     short int ibx;
     short int iby;  //---0..71
   };
 //----------------------------------------------------------------------
 //---DoxygenTAG4
//Generated on Tue Jun 18 17:17:16 2013 for *** PRESTO *** Pet Reconstruction Software TOolbox *** revision 333 *** by  ￼ 1.5.6
