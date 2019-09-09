
 #ifndef __MRPET_DUMP_HH__
 #define __MRPET_DUMP_HH__
 
 
 #include "../../utils/inc/TypDefs_Basic.hh"
 #include "../utils/inc/SinoSegmentStructure.hh"
 
 #include "MMatrix.hh"
 #include "BrainPET_BasicProperties.hh"
 
 
 
 
 
 //---class: MRPET_dump
 //--------------------------------------------------
 //--- code based on mrpet_dump.c by Larry Byars
 //--- some changes have been performed
 //--- The matrix transformations are defined in MMatrix.hh
 //--------------------------------------------------
 class MRPET_dump{
 
 
 public:
 
 //---methods:
 
 //---Constructor I
 MRPET_dump(int nViews = 192, int nProjs = 256, 
 int maxRingdiff_x = 67, int span_x = 9,
 float crystalRadius = 18.796,
 int verbose_x = 0);
 
 //--- Destructor
 ~MRPET_dump();
 
 //---computes for every scanner of the crystal its position
 //---and stores it in point3D_typ *crystalPositions[ncrystals]
 int compute_crystal_positions();
 
 //--- initialise segments lookup tables:
 void init_seginfo();
 
 //---get segment info for plane number using LUTs
 void get_seg_info(int planeNumber, int &segment, int &z_value, 
 float &min_angle, float &max_angle, float &mean_angle);
 
 //---convert z and segment to planenumber
 int seg_z_to_plane_number(int seg, int z);
 
 //---returns address for specified LOR:
 int compute_lor_addr(int block1, int crystal1, 
 int block2, int crystal2, 
 int &view, int &proj, int &plane, int &segment);
 
 //--- returns position of crystal specified by blockID and crystalID 
 void get_crystal_position( int block, int crystal, float pos[3]);
 
 
 //--- physical to projection
 void phy_to_pro( float deta[3], float detb[3], float projection[4]);
 
 //--- compute the physical coordiantes of a detector
 void det_to_phy(int mod, int x, int y, float pos[3]);
 
 
 //--- physical to address (flat and object)
 int pro_to_addr( float pro[4], int &bin, int &view, int &plane, int &segment);
 
 //--- returns planenumber
 int pro_to_planenumber(float z, float tan_theta);
 
 //---get number of axial views
 int get_n_views(); 
 
 //---get size of a sinogram bin
 float get_binsize();
 
 //---get size of a angular bin (radian)
 float get_angular_binsize();
 
 //---get number of radial projections
 int get_n_projs(); 
 
 //---get number of planes
 int get_n_planes();
 
 //---get maximum ringdifference (67 for brainPET)
 int get_max_ringdiff();
 
 //---get span number (9 for brainPET)
 int get_span();
 
 //---get number of modul pairs (head combinations)
 int get_n_mpairs();
 
 //---get number of modules in x-direction
 int get_mod_nx();
 
 //---get number of modules in y-direction
 int get_mod_ny();
 
 //---lookup for modul pairs
 void mpair_lookup(int &amod, int &bmod, int index);
 
 //---lookup: index to modules
 int mpair_lookup2(int amod, int bmod);
 
 //--- convert flat address to (view, projection, plane)
 void addr_to_view_proj_plane(int addr, int &view, int &proj, 
 int &plane);
 
 //---Convert (view, projection, plane) to address
 void proj_to_addr(int view, int proj, int plane, int &addr);
 
 //---converts (h1,b1,c1;h2,b2,c2) to flat address:
 int compute_flat_address(int h1, int b1, int c1, 
 int h2, int b2, int c2);
 
 //---hp lookup
 short int get_hp_lookup(int headA, int headB);
 
 
 
 //---get sinogram segment structure
 //---(new instance created & ownership goes beyond this scope)
 SinoSegmentStructure* get_SinoSegmentStructure(void);
 
 //---get number of sinograms as given in Siemens SinoSegment structure
 int get_number_of_sinos_siemens(void);
 
 //---get SYMMETRIC sinogram segment structure (all number of planes are even!)
 //---(new instance created & ownership goes beyond this scope)
 //---symmetry is requested for sino_reco modul which exploits symmetries
 SinoSegmentStructure* get_SYMMETRIC_SinoSegmentStructure(void);
 
 //---/
 
 //---data members: 
 
 //---mpair variables 
 char *mpair_a;
 char *mpair_b;
 
 //---/
 
 
 
 
 private:
 
 //---data members:
 
 //--verbose level (default 0)
 int verbose;
 
 //--- basic geometric properites:
 int nviews; 
 int nprojs;
 int maxRingdiff;
 int span;
 int nplanes;
 
 //---modul pairs:
 int n_mpairs;
 int n_mods;
 int nheads;
 int ncrystals; 
 int ncrystals_per_ring;
 int mod_nx;
 int mod_ny;
 int mod_fov;
 int mod_nrings;
 float crystal_radius;
 double plane_sep;
 
 //---segment information
 int maxseg; // 
 int nsegs; // number of segments (always odd)
 int nrings; // number of detector rings
 float d_tan_theta; // tilt angle of segments
 int *segz0;
 int *segnz;
 int *segzoff;
 int *segnums;
 
 //---bad head combination
 int bad_hp;
 
 //---contains cristal positions:
 point3D_typ *crystalPositions;
 
 //---binsize in mm
 float binsize;
 
 //---lookuptables
 int *segments_lut;
 int *z_val_lut;
 float *min_angle_lut;
 float *mean_angle_lut;
 float *max_angle_lut;
 
 //---Matrix elements for rotations
 MMatrix *rot;
 MMatrix *vec; 
 MMatrix *pos;
 float *vec_data;
 float *pos_data;
 
 //---epm lookup table:
 int *slut;
 
 //---block lookup table:
 int *blut;
 
 //---head pairs:
 short int *hpair;
 
 //---head pair lookup table
 short int *hplookup;
 
 //---get summary of sino segment structure
 SinoSegmentStructure* segment_struct;
 
 //---/
 
 
 //---methods:
 
 //--- make EPM lookup table:
 void make_slut();
 
 //---init 
 void init_mpair_data();
 
 //---init
 int init_lor_info();
 
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
//Generated on Tue Jun 18 17:17:18 2013 for *** PRESTO *** Pet Reconstruction Software TOolbox *** revision 333 *** by  ￼ 1.5.6
