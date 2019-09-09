
 #ifndef __BRAINPET_LMSTREAMMANAGER_H__
 #define __BRAINPET_LMSTREAMMANAGER_H__
 
 
 #include <iostream>
 #include <stdlib.h>
 #include <stdio.h>
 #include <math.h>
 #include <string.h>
 
 using namespace std;
 
 #include "../utils/inc/FileAccess.hh"
 #include "../utils/inc/TextStream.hh"
 #include "../utils/inc/SharedMem.hh"
 
 //---
 #include "BrainPET_TypDefs.hh" //defines struct lm_event
 #include "BrainPET_BasicProperties.hh"
 #include "BrainPET_ListmodeFrame.hh"
 #include "BrainPET_DetHitMap.hh"
 #include "BrainPET_LORfileData.hh"
 //#include "BrainPET_DeadTimeCorrection.hh"
 //#include "BrainPET_PileupManager.hh"
 #include "MRPET_dump.hh"
 #include "Isotope.hh"
 #include "MR_TriggerManager.hh"
 
 //---ROOT classes
 #include "TGraph.h"
 #include "TCanvas.h"
 #include "TH1.h"
 #include "TGraphQQ.h"
 
 
 
 //---class: BrainPET_LMStreamManager
 //----------------------------------------------------------------------------------------
 //----------------------------------------------------------------------------------------
 //---BrainPET_LMStreamManager
 //---Class 
 //----------------------------------------------------------------------------------------
 //----------------------------------------------------------------------------------------
 //---\brief basic Listmode File manager
 class BrainPET_LMStreamManager
 {
 public:
 
 //---methods:
 
 //---Constructor I
 BrainPET_LMStreamManager(string RUNID_x = "__undefined");
 
 //---Destructor
 ~BrainPET_LMStreamManager();
 
 //---print info to cout
 void get_info();
 
 //---export listmode info to ASCII file
 void export_to_ASCII(char *filename);
 
 //---export listmode info and mr_trigger info to ASCII file
 void export_to_ASCII(char *filename, char *mr_trigger_filename);
 
 //---get RUD_ID of measurement
 string get_run_id();
 
 //---get listmode filename list
 string* get_listmode_filenames(int &n_lst_files);
 
 //---check input listmode files before processing
 bool check_files_lst(string* filenames_lst, int nFiles_lst_x);
 
 //---check consistency between listmode files and 
 //---header information after processing or reading of the object
 //---path: folder of listmode data
 bool check_processing(string path, string listmode_header);
 
 //---set isotope for decay correction
 //---this object IS NOT owner
 //---HINT: use function after read_from_stream()!
 void set_isotope(Isotope *isotope_x);
 
 //---get isotope that is currently defined
 Isotope* get_isotope();
 
 //---reset all data structures
 void reset();
 
 //---initialise data structures
 void init();
 
 //---preprocess listmode file to generate header 
 //---datastructure is be filled if pointer is not NULL 
 //--- AND
 //---decode_coincidences=true
 //---display headcurves shows headcurve of listmode data
 //---for visual verification
 void preprocess_lst(string listmode_path,
 string listmode_header_file,
 string* filenames_lst, int nFiles_lst,
 int start_second_x = 0, int end_second_x = -1,
 bool decode_coincidences = false,
 BrainPET_LORfileData *prompts_lor = NULL, 
 BrainPET_LORfileData *delayeds_lor = NULL, 
 BrainPET_DetHitMap *pcmap = NULL, 
 BrainPET_DetHitMap *dcmap = NULL,
 bool display_headcurves=false, 
 bool veto_get_info=true);
 
 
 //---fill map by definition of start and endtime
 void fill_maps_by_timedef(string listmode_path,
 long int frame_start_time, long int frame_end_time,
 bool perform_decay_corr, 
 float &decay_correcton_factor,
 BrainPET_LORfileData *prompts_lor, 
 BrainPET_LORfileData *delayeds_lor, 
 BrainPET_DetHitMap *pcmap, 
 BrainPET_DetHitMap *dcmap,
 unsigned long int *mean_cfd_block_counter,
 bool reset_data = true);
 
 //---fill map by definition of counts
 //---filling maps with equal statistics
 //---equal_stat_stop_time contains end time of frame -> start time for next frame
 //---set or "n_prompts" or "n_delayeds" or "n_trues" 
 //---to number of desired counts for this frame
 //---set frame_start_time to start-time for this frame
 void fill_maps_by_countdef(string listmode_path,
 long int frame_start_time, long int global_end_time,
 long int n_prompts, long int n_delayeds, long int n_trues, 
 bool perform_decay_corr,
 float &decay_correcton_factor, 
 BrainPET_LORfileData *prompts_lor, 
 BrainPET_LORfileData *delayeds_lor, 
 BrainPET_DetHitMap *pcmap, 
 BrainPET_DetHitMap *dcmap,
 long int &equal_stat_stop_time,
 unsigned long int *mean_cfd_block_counter,
 bool reset_data = true);
 
 //---get start time of first frame (by definition zero)
 unsigned int get_start_time();
 
 //---get end time last frame 
 unsigned int get_end_time();
 
 //---get acquisition time
 unsigned int get_frame_length();
 
 //---write object into stream
 void write_into_stream(StreamWriter* stream);
 
 //---read object from stream
 bool read_from_stream(StreamReader* stream);
 
 //---display prompts headcurve:
 bool displayPrompts_hc();
 
 //---make TGraph prompts headcurve:
 TGraph* get_TGraph_prompts_hc(int max_sec=0);
 
 //---make TGraph delayeds headcurve:
 TGraph* get_TGraph_delayed_hc(int max_sec=0);
 
 //---display delayeds headcurve:
 bool displayDelayeds_hc();
 
 //---return CFD countrate of required second
 unsigned long int* get_cfd_countrate_for_second(int second);
 
 //---define shared memory message buffer
 void define_shared_mem(SharedMem* sh_mem0);
 
 //---message to shared memory
 void message_to_shared_mem(string message);
 
 //---get total number of trigger events
 int get_number_of_trigger_events(void);
 
 //---get time of trigger event since start
 //---1 tick == 200 micro sec
 //---return -1 if requested trigger number not available
 long long int get_triggerevent_time(int trig_number);
 
 //---get time of trigger event since start (seconds)
 //---return -1 if requested trigger number not available
 float get_triggerevent_time_sec(int trig_number);
 
 
 
 //---get handle on trigger manager
 MR_TriggerManager* get_MR_TriggerManager(void);
 
 //---/
 
 private:
 
 
 
 //---data members:
 
 //---class IS owner of this object!
 MRPET_dump *mrpet_dump;
 
 //---Manager object for MR trigger (object IS owner)
 MR_TriggerManager *trigger_manager;
 
 //---EPM Pattern for module pair order (object IS owner)
 EPMpattern *epm_pattern;
 
 //---object for used isotope (object IS NOT the owner)
 Isotope *isotope;
 
 //---half life of isotope (set from Isotope *isotope)
 float halflifetime;
 
 //---decay constant (ln(2)/halflife)
 float lambda;
 
 //----RunID of this measurement (Unique for listmode acquisition)
 string RUNID;
 
 //---number of acquired listmode files
 int nFiles_lst;
 
 //---filename list of acquired listmode files
 string* filenames_lst;
 
 //---startposition of data for new second in listmode stream
 long int *listmode_file_pointer;
 
 //---number of listmode file for each second in the datastream
 int *listmode_file_number;
 
 //---starttime obtained from listmode (usually 0)
 unsigned int start_time;
 
 //---endtime obtained from listmode processing in sec (usually acquisition time)
 unsigned int end_time;
 
 //---acquisition time (end_time - start_time)
 unsigned int frame_length;
 
 //---detected number of prompts in listmode
 unsigned int n_prompts_total;
 
 //---detected number of delayeds in listmode
 unsigned int n_delayeds_total;
 
 //---prompts headcurve
 long int *prompts_hc; 
 
 //---delayeds deadcurve
 long int *delayeds_hc; 
 
 //---counter for block singles:
 unsigned long int *countrate_CFD; // cfd counts/sec
 unsigned long int *countrate_XYE; // stored xy events/sec
 unsigned long int *countrate_qual; // qualified energy events/sec
 unsigned long int *countrate_rio_singles; // RIO Singles Events Counter/sec
 
 //---number of counter packets per second (for mean value):
 int cnt_cdf [192]; //tmp counter
 int cnt_xye [192]; //tmp counter
 int cnt_qual[192]; //tmp counter
 int cnt_rio [192]; //tmp counter
 
 
 //---flag is processing has been initialised (by processing acquired
 //---rawdata or by reading the header information from the file
 bool processing_initialised;
 
 
 //---optional shared memory for progress report
 SharedMem* sh_mem;
 
 //---/
 
 
 //---methods:
 
 //---fill listmode data in prompts_lor and dcmap
 //---Caution: lor and cmap objects need to be initialized or set to NULL
 //---before framing.
 //---By default objects are reset; to add data set reset_data = false!!
 //---"n_prompts", "n_delayeds" or "n_trues" can be used to frame by equal statistics
 //---mean_cfd_block_counter needs to be an arry of 192 unsigned long ints 
 void fill_maps(string listmode_path,
 long int frame_start_time, long int frame_end_time,
 long int n_prompts, long int n_delayeds, long int n_trues,
 bool perform_decay_corr,
 float &decay_correcton_factor,
 BrainPET_LORfileData *prompts_lor, 
 BrainPET_LORfileData *delayeds_lor, 
 BrainPET_DetHitMap *pcmap, 
 BrainPET_DetHitMap *dcmap,
 long int &equal_stat_stop_time,
 unsigned long int *mean_cfd_block_counter,
 bool reset_data = true);
 
 //---from mrpet_dump.c from l.byars
 off_t ftellx(FILE *stream); 
 
 //---from mrpet_dump.c from l.byars
 int graycode_sync_file(FILE *fptr);
 
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
