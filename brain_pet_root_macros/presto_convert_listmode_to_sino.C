{

  
  //---------------------------------------------------------------------------------  
  //---macro: convert_listmode data to sino data, including the prompt sino and random sino(variance_reduction )
  //
  ////---reads listmode data and generates special header


  //---------------------------------------------------------------------------------

	//use rootlogon for development:
 //	gROOT->ProcessLine(".x /data/Weirich/root_macros/rootlogon_devel.C");


  //---set RUNID:
  string RunID = "XB1BN265N-BI";
  string Study = "/";
  //set path to RUNID folder:
  string store       = "/data/Caldeira/LSO";
  string path        = store +Study +RunID;
  string lst_path    = path;
  string path_output = "/data/Caldeira/LSO";
  

  //set isotope:
  // isotope is F-18
  char isotope = 'f';

  //define measured listmode files (in temporal order):
  const int n_lst_files = 1;
  string lst_files[n_lst_files]={RunID +".lst"};   
  
  //force preprocessing of listmode files, even if header already exists:
  bool force_listmode_processing = true;
  




  //define data structure (think before changing):
  //string rawdata = path + "/rawdata";
  string rawdata = "/data/Caldeira/LSO";
  string header = rawdata +"/"+RunID+"_LMStreamManager.o";
  string trigger_folder = path + "/" + "mr_trigger";
  string trigger_file = trigger_folder + "/" + RunID+"_mr_trigger.log";
  string listmode_file = rawdata + "/" + "listmode_info.log";
  string lut_filename = "/data/BrainPET_Sys/Software/TemplateProcData/version1/siemens_tools/lorlookup_p256_doi.dat";
  string filename_lor_lut= "/data/BrainPET_Sys/Software/TemplateProcData/version1/siemens_tools/lorlookup_p256_doi.dat";
	string randoms_sino_filename = "/data/Caldeira/LSO/randoms_phantom_60hwocoil_new.fs";
	string prompts_sino_filename =  "/data/Caldeira/LSO/prompts_phantom_60hwocoil_new.fs";

  char command_buffer[1024];
  sprintf(command_buffer, ".! mkdir %s", trigger_folder.c_str()); 
  gROOT->ProcessLine(command_buffer);
  sprintf(command_buffer, ".! mkdir %s", rawdata.c_str()); 
  gROOT->ProcessLine(command_buffer);
  
	bool perform_dead_time_corr = true; 
  	int dead_time_corr_level    = 2;
  	bool perform_decay_corr     = true;
    //pile up correction
  	bool perform_pu_correction  = false;
  	float decay_correcton_factor         = 1.0;
  
  bool lm_header_ok = true;

  BrainPET_LMStreamManager *lm = new BrainPET_LMStreamManager(RunID);  
//   lm->set_dead_time_correction(dtc);
//   lm->set_pileup_correction(pu_manager);

  MRPET_dump *kludge = new MRPET_dump();
  string filename_lm = path_output +"/"+ RunID + "_LMStreamManager.o";

  // f-18
  Isotope *isotope_x = new Isotope(isotope);
  lm->set_isotope(isotope_x);
  
  
//---Norm: -----------------------------------------------------------
  BrainPET_Sinograms *norm_sino = new BrainPET_Sinograms(kludge, "norm_sino");
 // FileAccess *fa_norm = new FileAccess(filename_norm.c_str());

  BrainPETbasics_ABC* bpet_basics0 = new BrainPETbasics_ABC();

  // hitmaps
  BrainPET_DetHitMap *dcmap = new BrainPET_DetHitMap("dcmap", bpet_basics0);
  BrainPET_DetHitMap *pcmap = new BrainPET_DetHitMap("pcmap", bpet_basics0);
  //BrainPET_DetHitMap *tcmap = new BrainPET_DetHitMap("tcmap", bpet_basics0);

  BrainPET_DetHitMap *corr_map = new BrainPET_DetHitMap("corr_map", bpet_basics0);

// sinograms
  BrainPET_Sinograms *prompts_sino = new BrainPET_Sinograms(kludge, "Prompts_Sino");
  BrainPET_Sinograms *randoms_sino = new BrainPET_Sinograms(kludge, "Randoms_Sino");

  //LOR data
  BrainPET_LORfileData *prompts_lor = new BrainPET_LORfileData(304, 864*864, true);
  BrainPET_LORfileData *delayeds_lor= new BrainPET_LORfileData(304, 864*864, true);

  BrainPET_VarianceReduction *var_red = new BrainPET_VarianceReduction();
  BrainPET_LORfileData *dynamic_norm_lor= new BrainPET_LORfileData(304, 864*864, true);

  BrainPET_Sinograms *dynamic_norm_sino = new BrainPET_Sinograms(kludge, "dynamic_norm_sino");
  BrainPET_LORfileData *randoms_lor= new BrainPET_LORfileData(304, 864*864, true);
  randoms_lor->set_mrpet_dump(kludge);
  prompts_lor->set_mrpet_dump(kludge);
  delayeds_lor->set_mrpet_dump(kludge);
  randoms_lor->set_mrpet_dump(kludge);
  dynamic_norm_lor->set_mrpet_dump(kludge);


  dynamic_norm_lor->set_mrpet_dump(kludge);

  prompts_lor->set_mrpet_dump(kludge);
  delayeds_lor->set_mrpet_dump(kludge);

  //check availability of ListmodeManager before framing:
  if(!force_listmode_processing) {
    lm_header_ok = lm->check_processing(lst_path, header);
  }

 
  // convert listmode to lor
  if(!lm_header_ok || force_listmode_processing){
    lm->preprocess_lst(lst_path,
		       header,	
		       lst_files, n_lst_files, -1, -1, true, 		
		       prompts_lor, 
		       delayeds_lor, 
		       pcmap,
		       dcmap);
  }

  
  // get the total counts of the lor data
  cout << "---------" << endl;
  cout << "dcmap: " << dcmap->get_total_hitmap_sum()<< endl;
  cout << "pcmap: " << pcmap->get_total_hitmap_sum()<< endl;
  cout << "p_lor: " << prompts_lor->get_count_sum()<< endl;
  cout << "d_lor: " << delayeds_lor->get_count_sum()<< endl;
  cout << "---------" << endl;

	long int start = 0;
	long int stop  = 215999; //60h to seconds

	unsigned long int *mean_cfd_block_counter = new unsigned long int[192];
	
  	lm->fill_maps_by_timedef(lst_path ,
								     						start, stop,  
								     					  	perform_decay_corr,
													     	decay_correcton_factor,
			     										  	prompts_lor, 
								    						delayeds_lor, 
			  					     						pcmap, 
			    					  						dcmap,
								     						mean_cfd_block_counter,
			     					  						true);

   BrainPET_VarianceReduction *vrr = new BrainPET_VarianceReduction();
   vrr->variance_reduction(dcmap, delayeds_lor);

//	char* lookup_filename = "/data/BrainPET_Sys/Software/TemplateProcData/version1/siemens_tools/lorlookup_p256_doi.dat";

/*
   //convert data to sino:
   cout << "convert prompt sinogram..." << endl;
   BrainPET_Sinograms *sino_pr = new BrainPET_Sinograms(kludge, "prompt_sinogram_60h");
  	sino_pr->LOR2Sino(prompts_lor, true, lookup_filename);
   sino_pr->write_flat_sinograms(prompts_sino_filename, false);
   double countspr=sino_pr->get_n_Events();
   cout << countspr<< endl;   


   cout << "convert delayed sinogram..." << endl;
   BrainPET_Sinograms *sino_ra = new BrainPET_Sinograms(kludge, "randoms_sinogram_60h");
   sino_ra->LOR2Sino(delayeds_lor, true, lookup_filename);
   sino_ra->write_flat_sinograms(randoms_sino_filename, false);
   double countsra=sino_ra->get_n_Events();
   cout << countsra << endl; 

*/

  	BrainPET_QuickConverter* converter=new BrainPET_QuickConverter("LookupTable.BrainPET_QuickConverter.dat");
        MRPET_dump *dump = new MRPET_dump();   

        int nelements;
        float* flor;
        flor=prompts_lor->get_data_pointer_float(nelements);
        BrainPET_Sinograms* s1=new BrainPET_Sinograms(dump,"process");
    	int nfloats;
    	float* fsino;
    	fsino=s1->get_handle_flat_float(nfloats);
	double countsra=s1->get_n_Events();     
        cout << countsra << endl;    
 	converter->convert_LOR2sino(flor,nelements,fsino,nfloats,true);
        s1->write_flat_sinograms(prompts_sino_filename, false);
        double countsra=s1->get_n_Events();
   	cout << countsra << endl; 


     int nelements;
        float* flor;                  
        flor=delayeds_lor->get_data_pointer_float(nelements);
        BrainPET_Sinograms* s2=new BrainPET_Sinograms(dump,"process");
        int nfloats;                  
        float* fsino;
        fsino=s2->get_handle_flat_float(nfloats);
        double countsra=s2->get_n_Events();     
        cout << countsra << endl;          
        converter->convert_LOR2sino(flor,nelements,fsino,nfloats,true);
        s2->write_flat_sinograms(randoms_sino_filename, false);
        double countsra=s2->get_n_Events();
        cout << countsra << endl; 



}
