
 #include "BrainPET_LMStreamManager.hh"
 
 
 //----------------------------------------------------------------------------------------
 //---Constructor I
 //----------------------------------------------------------------------------------------
 BrainPET_LMStreamManager::BrainPET_LMStreamManager(string RUNID_x){
 
   if (sizeof(long int)==4)
     {
       cout << "****************************************************************" << endl;
       cout << "****** BrainPET_LMStreamManager::BrainPET_LMStreamManager ******" << endl;
       cout << "****************************************************************" << endl;
       cout << "FATAL ERROR:  32BIT machine, but code only working on 64Bit !!!!" << endl;
       cout << "sizeof(long int) different 32-64 bit (4/8 bytes) !!!!!!!!!!!!!!!" << endl;
       cout << "****************************************************************" << endl;
       cout << "****************************************************************" << endl;
       cout << "32bit machine not supported!" << endl;
       exit(0);
     }
 
 
   cout << "BrainPET_LMStreamManager( "<< RUNID_x << " )" << endl;
   RUNID = RUNID_x;
   init();
   //class_owns_deadtime_object = false;
 
 
   //---optional shared memory for progress report
   sh_mem=NULL;
 
  
 }
  
 
 //----------------------------------------------------------------------------------------
 //---print info to cout
 //----------------------------------------------------------------------------------------
 void BrainPET_LMStreamManager::get_info(){
   cout << " -------------------------------------------------------- " << endl;
   cout << "BrainPET_LMStreamManager::get_info()" << endl;
   cout << "RunID: " << RUNID << endl;
   cout << "Number of listmode files: " << nFiles_lst << endl;
   cout << "Listmode filenames: " << endl;
   for (int i = 0; i < nFiles_lst; i++){
     cout << " - " << filenames_lst[i] << endl;
   }
   cout << "Listmode starttime: " << start_time << endl;
   cout << "Listmode endtime: " <<end_time << endl;
   cout << "Listmode duration: " << frame_length << endl;
   cout << "N Prompts: " << n_prompts_total << endl;
   cout << "N Delayeds " << n_delayeds_total << endl;
   if(trigger_manager!=NULL){
     trigger_manager->get_info();
   }else{
     cout << "No trigger manager defined" << endl;
   }
   cout << " -------------------------------------------------------- " << endl;
 
 }
 
 //-------------------------------------------------
 //--- export listmode info to ASCII file
 //-------------------------------------------------
 void BrainPET_LMStreamManager::export_to_ASCII(char *filename){
 
 
   if(processing_initialised == false){
     cout << "ERROR in BrainPET_LMStreamManager" << endl;
     cout << "Object has not been initialised! " << endl;
     exit(0);
   }
 
   TextStream *logStream = new TextStream();  
 
   char buffer[256];
 
   sprintf(buffer, "RunID: %s", RUNID.c_str());
   logStream->append(buffer);
   sprintf(buffer, "Number of listmode files: %i", nFiles_lst);
   logStream->append(buffer);
   sprintf(buffer, "Listmode filenames:");
   logStream->append(buffer);
   for (int i = 0; i < nFiles_lst; i++){
     sprintf(buffer, " - %s",  filenames_lst[i].c_str());
   }
   sprintf(buffer, "Listmode starttime: %i", start_time);
   logStream->append(buffer);
   
   sprintf(buffer,"Listmode endtime: %i",end_time);
   logStream->append(buffer);
   sprintf(buffer,"Listmode duration: %i", frame_length);
   logStream->append(buffer);
   sprintf(buffer,"N Prompts: %u", n_prompts_total);
   logStream->append(buffer);
   sprintf(buffer,"N Delayeds %u", n_delayeds_total);
   logStream->append(buffer);
   
   logStream->store(filename);
   cout << "file exported" << endl;
 
 }
 
 
 //-------------------------------------------------
 //---export listmode info and mr_trigger info to ASCII file
 //------------------------------------------------
 void BrainPET_LMStreamManager::export_to_ASCII(char *filename, char *mr_trigger_filename){
   export_to_ASCII(filename);
   trigger_manager->export_to_ASCII(mr_trigger_filename);
 }
 
 
 //----------------------------------------------------------------------------------------
 //----------------------------------------------------------------------------------------
 string BrainPET_LMStreamManager::get_run_id(){
   return RUNID;
 }
 
 //----------------------------------------------------------------------------------------
 //---get listmode filename list
 //----------------------------------------------------------------------------------------
 string* BrainPET_LMStreamManager::get_listmode_filenames(int &n_lst_files){
   n_lst_files = nFiles_lst;
   return filenames_lst;
 }
 
 //----------------------------------------------------------------------------------------
 //---set isotope 
 //----------------------------------------------------------------------------------------
 void BrainPET_LMStreamManager::set_isotope(Isotope *isotope_x){
   
   isotope = isotope_x;
   halflifetime = isotope->get_halflife_time();
   
   //---decay correction DCF = exp(lambda*t)
   if(halflifetime!=0){
     lambda = 0.6931/(halflifetime);
   }else{
     lambda = 0.0;
   }    
   isotope->get_info();
 }
 
 
 
 
 //----------------------------------------------------------------------------------------
 
 //----------------------------------------------------------------------------------------
 Isotope* BrainPET_LMStreamManager::get_isotope(){
   return isotope;
 }
 
 
 
 //----------------------------------------------------------------------------------------
 //---convert listmode stream to single frames
 //----------------------------------------------------------------------------------------
 void BrainPET_LMStreamManager::preprocess_lst(string listmode_path,
                                                         string listmode_header_file,
                                                         string* filenames_lst_x, int nFiles_lst_x,
                                                         int start_second_x, int end_second_x,
                                                         bool decode_coincidences,
                                                         BrainPET_LORfileData *prompts_lor, 
                                                         BrainPET_LORfileData *delayeds_lor, 
                                                         BrainPET_DetHitMap *pcmap, 
                                                         BrainPET_DetHitMap *dcmap,
                                                         bool display_headcurves,
                                                         bool veto_get_info){
 
 
   cout << "LMStreamManager::preprocess_lst()" << endl;
   
   if(decode_coincidences){
     cout << "Coincidence decoding enabled!"  << endl;
   }else{
     cout << "Coincidence decoding disabled!"  << endl;
   }
   
   nFiles_lst = nFiles_lst_x;
   trigger_manager = new MR_TriggerManager(RUNID);
   
   filenames_lst = new string[nFiles_lst];
   for(int i = 0; i <  nFiles_lst; i++){
     filenames_lst[i]=filenames_lst_x[i];
     cout << "file " << i << ": " << filenames_lst[i] << endl;
   }
 
   //string filename_o_hdr = listmode_path + "/" + RUNID + "_LMStreamManager.o";
   string filename_o_hdr = listmode_header_file;
   FileAccess *fa_o_header = new FileAccess(filename_o_hdr.c_str());
   fa_o_header->open_writeonly(true);
   if(!fa_o_header->isWriteable()){
     cout << filename_o_hdr << " is not writeable!" << endl;
     exit(0);      
   }
   cout << "start conversion " << endl;
   lm_event bufr_tmp;
 
   //int lm_size;
 
   long unsigned int lst_counter; 
 
   //int pd;     // delayed event
      
   FILE *input_fptr = NULL;
   //unsigned long int event_counter_lst = 0;
 
   //-statistics for header files:
   char **frame_filename = new char*[1024];
   //char *output_filename = new char[1024];
   int *start_second = new int[1024];
   int *stop_second = new int[1024];
   for(int i = 0; i < 1024; i++){
     frame_filename[i] = new char[1024];
     start_second[i] = 0;
     stop_second[i] = 0;
   }
 
 
 
   if(listmode_file_pointer == NULL){
     listmode_file_pointer = (long int *)calloc(1, sizeof(long int));
   }else{
     cout << "ERROR in LMStreamManager::init_conversion(); array not NULL before acquisition!" << endl;
     exit(0);
   }
   if(listmode_file_number == NULL){
     listmode_file_number = (int *)calloc(1, sizeof(int));
   }else{
     cout << "ERROR in LMStreamManager::init_conversion(); array not NULL before acquisition!" << endl;
     exit(0);
   }
   
   if(prompts_hc == NULL){
     prompts_hc = ( long int *)calloc(1, sizeof(long int));
   }else{
     cout << "ERROR in BrainPET_LMStreamManager::init_conversion(); array not NULL before acquisition!" << endl;
     exit(0);
   }
   
   if(delayeds_hc == NULL){
     delayeds_hc = (long int *)calloc(1, sizeof(long int));
   }else{
     cout << "ERROR in BrainPET_LMStreamManager::init_conversion(); array not NULL before acquisition!" << endl;
     exit(0);
   }
  
  if(countrate_CFD == NULL){
     countrate_CFD = (unsigned long int *)calloc(192, sizeof(unsigned long int));
   }else{
     cout << "ERROR in BrainPET_LMStreamManager::init_conversion(); array not NULL before acquisition!" << endl;
     exit(0);
   }
   if(countrate_XYE == NULL){
     countrate_XYE = (unsigned long int *)calloc(192, sizeof(unsigned long int));
   }else{
     cout << "ERROR in BrainPET_LMStreamManager::init_conversion(); array not NULL before acquisition!" << endl;
     exit(0);
   }
   if(countrate_qual == NULL){
     countrate_qual = (unsigned long int *)calloc(192, sizeof(unsigned long int));
   }else{
     cout << "ERROR in BrainPET_LMStreamManager::init_conversion(); array not NULL before acquisition!" << endl;
     exit(0);
    }
   if(countrate_rio_singles == NULL){
     countrate_rio_singles = (unsigned long int *)calloc(192, sizeof(unsigned long int));
   }else{
     cout << "ERROR in BrainPET_LMStreamManager::init_conversion(); array not NULL before acquisition!" << endl;
     exit(0);
    }
 
   for(int i = 0; i < 192; i++){
     countrate_CFD[i]=0;
     countrate_XYE[i]=0;
     countrate_qual[i]=0;
     countrate_rio_singles[i]=0;
     cnt_cdf[i]=0;
     cnt_xye[i]=0;
     cnt_qual[i]=0;
     cnt_rio[i]=0;
   }
   
 
   //some statistics:
   //int end_second        = end_second_x;
   int current_second    = 0;
   long int n_timemarks  = 0;
   long int n_events     = 0;
   long int last_tmark   = 0;
   bool is_first_tmark   = true;  
   long int n_coincidence_events = 0;
   //long int n_delayeds = 0;
   //long int n_prompts  = 0;
   long int cr_prompts   = 0;
   long int cr_delayeds  = 0; 
 
   frame_length = 1;
 
   // for every listmode file:
   unsigned int tmark = 0;
   for(int lst_index = 0; lst_index < nFiles_lst; lst_index++){
     
     cout << endl << "open listmode file " << lst_index << endl;
     input_fptr=fopen((listmode_path +"/" +filenames_lst[lst_index]).c_str(), "r");
     
     if(input_fptr==NULL){
       cout << "ERROR in BrainPET_LMStreamManager::preprocess_lst" << endl;
       cout << listmode_path << "/" << filenames_lst[lst_index] << " is NOT READABLE" << endl;
       exit(0);
     }
 
     // while 6byte words are in file (i.e. not eof):
     int next_gcptr = 0;
     while(fread(bufr_tmp.data, 1, 6, input_fptr) == 6){ 
       if ((bufr_tmp.data[5]&0xf0)==graycodes[next_gcptr++]) {
           next_gcptr %= 8;}
       else { 
           next_gcptr=graycode_sync_file(input_fptr); 
           continue; 
       }
       
       n_events++;
       
       unsigned int first_timemark = 0;
       
   
       if((bufr_tmp.data[5]&0x8) == 0) { 
           //coincidence event:
           n_coincidence_events++;       
           //if(decode_coincidences){      
             // //cout << "48 bit coincidence event" << endl;
             //int e1 = (bufr_tmp.data[4]&0x30)>>4;
             int h1 = ((bufr_tmp.data[4]&0x0f)<<2)|((bufr_tmp.data[3]&0xc0)>>6);
             int b1 = (bufr_tmp.data[3]&0x38)>>3;
             int c1 = ((bufr_tmp.data[3]&0x07)<<5)|((bufr_tmp.data[2]&0xf8)>>3);
             //int e2 = (bufr_tmp.data[2]&0x06)>>1;
             int h2 = ((bufr_tmp.data[2]&0x01)<<5)|((bufr_tmp.data[1]&0xf8)>>3);
             int b2 = (bufr_tmp.data[1]&0x07);
             int c2 = bufr_tmp.data[0];
             int pd = (bufr_tmp.data[5]&0x04)>>2;
             pd=1-pd;
             if(((c1>143) || (c2>143) || (h1>31) || (h2>31) || (b1>5) || (b2>5)) ||
                ((c1<0) || (c2<0) || (h1<0) || (h2<0) || (b1<0) || (b2<0))){ 
               //bad_address_counter++;
             }
             else{
               short int hp = mrpet_dump->get_hp_lookup(h1, h2);       
               if(pd){
                 if(hp != 0){
                     //delayed_event_data[n_delayeds] = bufr_tmp;        
                     n_delayeds_total++;
                     cr_delayeds++;                                    
                     if(delayeds_lor != NULL){
                       //fill delayeds lor file
                       delayeds_lor->add_lor_event(h1, b1, c1, h2, b2, c2, 1.0);
                     }
                     if(dcmap != NULL){
                       //fill dcmap      
                       dcmap->fill_control_plots(h1, b1, 11-c1%12, 11-c1/12, 
                                               h2, b2, 11-c2%12, 11-c2/12, 1.0);
                     }
                 }                       
               }else{
                 // hp==0: invalid head combination 
                 if(hp != 0){  
                     //prompt_event_data[n_prompts] = bufr_tmp;
                     n_prompts_total++;            
                     cr_prompts++;                 
                     if(prompts_lor != NULL){
                       //fill prompts lor file
                       prompts_lor->add_lor_event(h1, b1, c1, h2, b2, c2, 1.0);
                     }
                     
                     if(pcmap != NULL){
                       // fill pcmap     
                       pcmap->fill_control_plots(h1, b1, 11-c1%12, 11-c1/12, 
                                               h2, b2, 11-c2%12, 11-c2/12, 1.0);
                     }
                 }      
               }
             }
             //} end: coincidence decoding
       } //end: coincidence event
       
       //if buffer is single-counter
       if((bufr_tmp.data[5]&0xf) == 0xa){ // we have a 48 bit count packet...
           switch((bufr_tmp.data[4]&0xfc)){
           case 0x10:  // EPM CFD Singles Count
           case 0x20:  // EPM Stored Singles Count
           case 0x30:  // EPM Valid Singles Count
           case 0x40:  // EPM RIO Singles Count
           case 0x50:  // EPM Delays Count
           case 0x60:  // EPM RIO Delays Count
           case 0x70:  // EPM Prompts Count
           case 0x80:  // EPM RIO Prompts Count
             int cntr_type=(bufr_tmp.data[4]&0xf0)>>4;
             int cntr_val=(bufr_tmp.data[2]<<16)|(bufr_tmp.data[1]<<8)|bufr_tmp.data[0];
             int cntr_slot=((bufr_tmp.data[4]&0x0f)<<4)|(bufr_tmp.data[3]&0xf0)>>4;
             int d=(bufr_tmp.data[3]&0x0c)>>2;       
             int h;
             int b;
 
             //conversion from epm-det notation to head-block notation:
             h = epm_pattern->get_head(cntr_slot, d);
             b = epm_pattern->get_block(cntr_slot, d);
             
             // CFD Singles
             if(cntr_type==1 && (cntr_val!=0)){
               countrate_CFD[(frame_length-1)*192+(h*6+b)]+=(unsigned long int)cntr_val;
               cnt_cdf[h*6+b]++;
             }
             
             // XYE Counter
             if(cntr_type==2 && (cntr_val!=0)){
               countrate_XYE[(frame_length-1)*192+(h*6+b)]+=(unsigned long int)cntr_val;
               cnt_xye[h*6+b]++;
             }
             
             // Qualified Events
             if(cntr_type==3 && (cntr_val!=0)){
               countrate_qual[(frame_length-1)*192+(h*6+b)]+=(unsigned long int)cntr_val;
               cnt_qual[h*6+b]++;
             }      
             // RocketIO Events
             if(cntr_type==4 && (cntr_val!=0)){
               countrate_rio_singles[(frame_length-1)*192+(h*6+b)]+=(unsigned long int)cntr_val;
               cnt_rio[h*6+b]++;
             }      
             
           }
       }
       //IOS Event && TRIGGER Event:
       if (((bufr_tmp.data[5]&0xf) == 0xc)&&(bufr_tmp.data[4]&0xf0) == 0x10) { 
           int port_number = bufr_tmp.data[0]&0x1f;
           trigger_manager->add_trigger_event(last_tmark,  port_number);
       }
    
       // TIME MARKS:
       if((bufr_tmp.data[5]&0xf) == 0xa) {
           switch((bufr_tmp.data[4]&0xfc)) {
           case 0: 
             last_tmark = tmark;
             tmark=(((((bufr_tmp.data[3]<<8)|bufr_tmp.data[2])<<8)|bufr_tmp.data[1])<<8)|bufr_tmp.data[0];
             if(is_first_tmark){
               first_timemark = tmark;
               trigger_manager->set_first_timemark(first_timemark);
               is_first_tmark = false;
               
             }else{
               if((int)last_tmark != (int)tmark-1){
                 long int diff_tmarks = (int)((long long int)tmark - (long long int)last_tmark);
                 cout << "t1 = " << last_tmark << " t2 = " <<  tmark << endl;
                 cout << "WARNING in LMStreamManager: number of lost time marks: " << diff_tmarks  << " (=" 
                        << (double)diff_tmarks/5000.0 << "sec) "<<endl << endl;
                 if(diff_tmarks < 5000){
                     //cout << "Corrupt timemarks. no further processing..." << endl;
                     //exit(0);
                 }
               }
             }                   
             if(n_timemarks%5000==0 &&n_timemarks!=0){   
               end_time = n_timemarks/5000;
               //store current pointer position:
               listmode_file_pointer[frame_length-1] = ftell(input_fptr);
               listmode_file_number[frame_length-1]  = lst_index;
               //cout << "file pointer: " << listmode_file_pointer[frame_length-1] << endl;
               
               prompts_hc[frame_length -1] = cr_prompts;
               delayeds_hc[frame_length -1] = cr_delayeds;
 
               //divide by number of counted packets (should be 5 per second):
               for(int i = 0; i < 192; i++){
                 
                 if(cnt_cdf[i]!=0){
                     float weight = 5.0/((float)cnt_cdf[i]);
                     //cout << countrate_CFD[(frame_length-1)*192+i];
                     countrate_CFD[(frame_length-1)*192+i]=weight*countrate_CFD[(frame_length-1)*192+i];
                     //cout << " - " << countrate_CFD[(frame_length-1)*192+i] << " - "  <<  weight << endl;              
                 }else{
                     countrate_CFD[(frame_length-1)*192+i]=0;
                 }
                 
                 if(cnt_xye[i]!=0){
                     float weight = 5.0/((float)cnt_xye[i]);
                     countrate_XYE[(frame_length-1)*192+i]=weight*countrate_XYE[(frame_length-1)*192+i];
                 }else{
                     countrate_XYE[(frame_length-1)*192+i]=0;
                 }
                 
                 if(cnt_qual[i]!=0){
                     float weight = 5.0/((float)cnt_qual[i]);
                     countrate_qual[(frame_length-1)*192+i]=weight*countrate_qual[(frame_length-1)*192+i];
                 }else{
                     countrate_qual[(frame_length-1)*192+i]=0;
                 }
                 
                 if(cnt_rio[i]!=0){
                     float weight = 5.0/((float)cnt_rio[i]);
                     countrate_rio_singles[(frame_length-1)*192+i]=weight*countrate_rio_singles[(frame_length-1)*192+i];
                 }else{
                     countrate_rio_singles[(frame_length-1)*192+i]=0;
                 }
           
               }
             
 
               frame_length++;
               //current end second
               current_second = (n_timemarks/5000);
              
               printf("progress: %u sec: %ld prompts - %ld delayeds  \r", 
                        current_second, cr_prompts, cr_delayeds); 
               fflush(stdout);
 
               char Xstr[1000];
               sprintf(Xstr,"progress: %u sec: %ld prompts - %ld delayeds", 
                         current_second, cr_prompts, cr_delayeds); 
               string xstr=Xstr;
               message_to_shared_mem(xstr);
 
 
               cr_prompts = 0;
               cr_delayeds = 0;
               
               listmode_file_pointer =  (long int *)realloc(listmode_file_pointer,
                                                                        frame_length*sizeof(long int));
               if(listmode_file_pointer == NULL){
                 cout << "ERROR realloc: listmode_file_pointer"  << endl;
                 exit(0);       
               }
 
               listmode_file_number =  (int *)realloc(listmode_file_number,
                                                                        frame_length*sizeof(int));
               if(listmode_file_number == NULL){
                 cout << "ERROR realloc: listmode_file_number"  << endl;
                 exit(0);       
               }
 
 
 
               prompts_hc = ( long int *)realloc(prompts_hc, frame_length*sizeof( long int));  
               if(prompts_hc == NULL){
                 cout << "ERROR realloc:prompts_hc"  << endl;
                 exit(0);       
               }
               delayeds_hc = ( long int *)realloc(delayeds_hc,frame_length*sizeof( long int)); 
               if(delayeds_hc == NULL){
                 cout << "ERROR realloc: delayeds_hc"  << endl;
                 exit(0);       
               }
 
 
               // counter for block singles:           
               countrate_CFD = (unsigned long int *)realloc(countrate_CFD, 192*frame_length*sizeof(unsigned long int));  
               if(countrate_CFD == NULL){
                 cout << "ERROR realloc:countrate_CFD"  << endl;
                 exit(0);       
               }
               countrate_XYE = (unsigned long int *)realloc(countrate_XYE,192*frame_length*sizeof(unsigned long int)); 
               if(countrate_XYE == NULL){
                 cout << "ERROR realloc: countrate_XYE"  << endl;
                 exit(0);       
               }
               countrate_qual = (unsigned long int *)realloc(countrate_qual,192*frame_length*sizeof(unsigned long int));
               if(countrate_qual == NULL){
                 cout << "ERROR realloc: countrate_qual"  << endl;
                 exit(0);       
               }
               countrate_rio_singles = (unsigned long int *)realloc(countrate_rio_singles, 192*frame_length*sizeof(unsigned long int));
               if(countrate_rio_singles == NULL){
                 cout << "ERROR realloc: countrate_rio_singles"  << endl;
                 exit(0);       
               }
 
               for(int i = 0; i < 192; i++){
                 countrate_CFD[(frame_length-1)*192+i]=0;
                 countrate_XYE[(frame_length-1)*192+i]=0;
                 countrate_qual[(frame_length-1)*192+i]=0;
                 countrate_rio_singles[(frame_length-1)*192+i]=0;
                 cnt_cdf[i]=0;
                 cnt_xye[i]=0;
                 cnt_qual[i]=0;
                 cnt_rio[i]=0;
               }                   
             }
             n_timemarks++;      
           }
       }            
       lst_counter++;      
     } 
   } 
   frame_length =  n_timemarks/5000;
   cout << endl << n_timemarks/5000 << " + " << n_timemarks%5000 << endl;
   //write LMStreamManager object into stream:
   write_into_stream(fa_o_header);
 
   fa_o_header->close();
   delete fa_o_header;
   fa_o_header=NULL;
   processing_initialised=true;
 
   if (!veto_get_info)
     {
       get_info();
     }
 
   if(display_headcurves){
     displayPrompts_hc();
     displayDelayeds_hc();
   }
 }
 
 
 //----------------------------------------------------------------------------------------
 //---check listmode-files
 //----------------------------------------------------------------------------------------
 bool BrainPET_LMStreamManager::check_files_lst(string* filenames_lst, int nFiles_lst_x){
   
   bool isOK = true;
   cout << endl << "----------- BrainPET_LMStreamManager::check_files() ---------" << endl;
   for(int i = 0; i < nFiles_lst_x; i++){
     char filename_tmp[1024];
     sprintf(filename_tmp, "%s" ,filenames_lst[i].c_str());
 
     FileAccess *fa = new FileAccess(filename_tmp);
     fa->open_readonly(true);
     if(fa->isReadable()){
      cout << "File " << filename_tmp << " IS readable!" << endl;
     }else{
       cout << "WARNING: File " << filename_tmp << " IS NOT readable!" << endl;
     }
     fa->close();
   }
   
   return isOK;
 }
 
 
 
 //----------------------------------------------------------------------------------------
 //---PRIVATE:
 //---use by "fill_maps_by_timedef()" or "fill_maps_by_countdef()"
 //---frame listmode data in DetHitMaps and LORfileData
 //----------------------------------------------------------------------------------------
 void BrainPET_LMStreamManager::fill_maps(string listmode_path,
                                                    long int frame_start_time, long int frame_end_time,
                                                    long int n_prompts, long int n_delayeds, long int n_trues,
                                                    bool perform_decay_corr,
                                                    float &decay_correction_factor,
                                                    BrainPET_LORfileData *prompts_lor, 
                                                    BrainPET_LORfileData *delayeds_lor, 
                                                    BrainPET_DetHitMap *pcmap, 
                                                    BrainPET_DetHitMap *dcmap,
                                                    long int &equal_stat_stop_time,
                                                    long unsigned int *mean_cfd_block_counter,
                                                    bool reset_data){
 
 
   bool equal_statistics_mode = false;
 
 
   cout << "BrainPET_LMStreamManager::fill_maps()" << endl;
   decay_correction_factor = 1.0;
  
   if(n_prompts > 0 || n_delayeds > 0 || n_trues > 0){
     equal_statistics_mode = true;
     cout << "FILL MAPS WITH EQUAL STATISTICS" << endl;
   }
 
 
   if(!processing_initialised){
     cout << "ERROR: BrainPET_LMStreamManager::fill_maps()" << endl;
     cout << "ListmodeManager object is not initialised" << endl;
     exit(0);
   }
 
   if((int)frame_start_time < (int)start_time ||  (int)frame_start_time >(int)end_time){
     cout << "ERROR: BrainPET_LMStreamManager::fill_maps()" << endl;
     cout << "frame start time is out of bounds" << endl;
     cout << "Measurement start: " << frame_start_time << endl;
     cout << "Frame start: " << start_time << endl;
     cout << "Frame end:   " << end_time << endl;
     exit(0);
   }
 
   if((int)frame_end_time < (int)start_time || (int)frame_end_time > (int)end_time){
     cout << "ERROR: BrainPET_LMStreamManager::fill_maps()" << endl;
     cout << "frame end time is out of bounds (" << end_time << ")" << endl;
     cout << "Measurement start: " << frame_start_time << endl;
     cout << "Frame start: " << start_time << endl;
     cout << "Frame end:   " << end_time << endl;
     exit(0);
   }
   if(frame_end_time <= frame_start_time){
     cout << "ERROR: BrainPET_LMStreamManager::fill_maps()" << endl;
     cout << "frame_end_time <= frame_start_time" << endl;
     exit(0);
   }
 
   if(prompts_lor==NULL){
     cout << "ERROR in BrainPET_LMStreamManager::fill_maps()" << endl;
     cout << "prompts_lor == NULL. Initialise object before framing!" << endl;
     exit(0);
   }
   
   if(dcmap==NULL){
     cout << "ERROR in BrainPET_LMStreamManager::fill_maps()" << endl;
     cout << "dcmap == NULL. Initialise object before framing!" << endl;
     exit(0);
   }
 
 
   if(perform_decay_corr){
     if(isotope==NULL){
       cout << "Isotope not set for decay correction" << endl;
       cout << "use:  set_isotope(Isotope *isotope_x)" << endl;
       exit(0);
     }    
   }
 
   if(prompts_lor!=NULL && reset_data){
     prompts_lor->set_all_channels(0.0);
     cout << "Info: Reset PROMPTS LOR data" << endl;
   }
   if(delayeds_lor!=NULL && reset_data){
     delayeds_lor->set_all_channels(0.0);
     cout << "Info: Reset DELAYEDS LOR data" << endl;
   }
   if(pcmap!=NULL && reset_data){
     pcmap->reset();
     cout << "Info: Reset PCMAP data" << endl;
   }
   if(dcmap!=NULL && reset_data){
     dcmap->reset();
     cout << "Info: Reset DCMAP data" << endl;
   }
 
   if(!reset_data){
     cout << "WARNING: BrainPET_LMStreamManager::fill_maps" << endl;
     cout << "reset of LOR data and CMAPs is disabled!" << endl;
   }
   
   
 
   // buffer for listmode event
   unsigned char bufr[6]; 
 
   //timemark variables:
   unsigned int tmark      = 0; //--- current time mark
   long int delayeds_count = 0;
   long int prompts_count  = 0;
 
   //parameters of the two events:
   int e1 = 0;// energy
   int e2 = 0;  
   int h1 = 0;// headnumber
   int h2 = 0;  
   int b1 = 0;// blocknumber
   int b2 = 0; 
   int c1 = 0;// crystalnumber
   int c2 = 0;  
   int pd = 0;// delayed event
 
   int bad_address_counter = 0;
   bool stopReading        =false;
 
   FILE *input_fptr                = NULL;
   long int next_gcptr             = 0;
   long int fileStartPosition      = 0;
   int current_listmodefile_number = 0;
   string input_filename;
 
 
   long int timeCounter     = frame_start_time*5000;
   bool first_run           = true;
   long int timeCounter_rel = 0;
 
   while(stopReading==false){
     if(first_run){
       //set filepointer to fist startpoint:
       fileStartPosition = listmode_file_pointer[frame_start_time];
       current_listmodefile_number = listmode_file_number[frame_start_time];
       input_filename = listmode_path + "/" + filenames_lst[current_listmodefile_number];
       cout << "open file " << input_filename << endl;
       input_fptr= fopen(input_filename.c_str(), "rb");
       if(input_fptr== NULL){
           cout << input_filename << " IS NOT readable" << endl;
           exit(0);
       }
       fseek(input_fptr, fileStartPosition, SEEK_SET); 
       first_run = false;
     }else{
       cout << endl << timeCounter/5000 << " + " << timeCounter%5000 << endl;
       //set filepointer to start of next listmode file:
       fclose(input_fptr);
       current_listmodefile_number++;
       if(current_listmodefile_number >= nFiles_lst){
           cout << "WARNING in BrainPET_LMStreamManager::fill_maps()" << endl;
           cout << "no further listmode file exists" << endl;
           cout << "LAST FRAME IS SHORTER!" << endl;
           stopReading = true;             
       }else{
           input_filename = listmode_path + "/" + filenames_lst[current_listmodefile_number];
           cout << "open file " << input_filename << endl;
           input_fptr= fopen(input_filename.c_str(), "rb");
           if(input_fptr== NULL){
             cout << input_filename << " IS NOT readable" << endl;
             exit(0);
           }
       }
     }
     while(stopReading==false && fread(bufr, 1, 6, input_fptr) == 6) {    
       if ((bufr[5]&0xf0)==graycodes[next_gcptr++]) {
           next_gcptr %= 8;}
       else { 
           next_gcptr=graycode_sync_file(input_fptr); 
           continue; 
       }
       
       //------------------------------------
       // we have a 48 bit coincidence event:
       //------------------------------------
       if ((bufr[5]&0x8) == 0) { 
           //cout << "48 bit coincidence event" << endl;
         e1=(bufr[4]&0x30)>>4;
         h1=((bufr[4]&0x0f)<<2)|((bufr[3]&0xc0)>>6);
         b1=(bufr[3]&0x38)>>3;
         c1=((bufr[3]&0x07)<<5)|((bufr[2]&0xf8)>>3);
         e2=(bufr[2]&0x06)>>1;
         h2=((bufr[2]&0x01)<<5)|((bufr[1]&0xf8)>>3);
         b2=(bufr[1]&0x07);
         c2=bufr[0];
         pd=(bufr[5]&0x04)>>2;
         pd=1-pd;
         if(((c1>143) || (c2>143) || (h1>31) || (h2>31) || (b1>5) || (b2>5)) ||
              ((c1<0) || (c2<0) || (h1<0) || (h2<0) || (b1<0) || (b2<0))){ 
               bad_address_counter++;        
           }
           else{
             short int hp = mrpet_dump->get_hp_lookup(h1, h2);
             if(pd){
               if(hp != 0){
 
                 if(dcmap!=NULL){
                     dcmap->fill_control_plots(h1, b1, 11-c1%12, 11-c1/12, 
                                                     h2, b2, 11-c2%12, 11-c2/12, 1.0);
                 }
                 if(delayeds_lor!=NULL){
                     delayeds_lor->add_lor_event(h1, b1, c1, h2, b2, c2,1.0);
                 }
                 delayeds_count++;
               }        
             }
             else{
               if(hp != 0){
                 if(pcmap!=NULL){        
                     pcmap->fill_control_plots(h1, b1, 11-c1%12, 11-c1/12, 
                                                     h2, b2, 11-c2%12, 11-c2/12, 1.0);
                 }
                 if(prompts_lor!= NULL){
                     prompts_lor->add_lor_event(h1, b1, c1, h2, b2, c2, 1.0);                   
                 }
                 prompts_count++;
               }         
             }
           }         
       }
 
        
       //---------------------------------
       //-- we have a 48 bit count packet:
       //---------------------------------
       if ((bufr[5]&0xf) == 0xa) {
           switch((bufr[4]&0xfc)) {
           case 0:                        
             tmark=(((((bufr[3]<<8)|bufr[2])<<8)|bufr[1])<<8)|bufr[0];
             timeCounter++;
             timeCounter_rel++;
             if(timeCounter%5000==0){
               //for equal statistic mode:
               if(equal_statistics_mode){
                 //if framed to equal prompts:
                 if(n_prompts != 0){
                     if(n_prompts <= prompts_count){
                       stopReading = true;
                     }
                 }
                 //if framed to equal delayeds:
                 if(n_delayeds != 0){
                     if(n_delayeds <= delayeds_count){
                       stopReading = true;
                     }
                 }
                 //if framed to equal trues:
                 if(n_trues != 0){
                     if(n_trues <= (prompts_count - delayeds_count)){
                       stopReading = true;
                     }
                 }
                 equal_stat_stop_time = timeCounter/5000;
               }else{
                 if((((int)timeCounter/5000)>=(int)frame_end_time) || (((int)timeCounter/5000)>=(int)end_time)){
                     cout << endl << timeCounter/5000 << " + " << timeCounter%5000 << endl;              
                     stopReading = true;
                 }
               }
               
             }
 
             if(timeCounter%5000==0){
               printf("progress: %ld sec, total promps: %li  total delayeds: %li  %c",
                        timeCounter/5000,prompts_count,delayeds_count,'\r'); 
               fflush(stdout);
 
               char Xstr[1000];
               sprintf(Xstr,"progress: %ld sec, total promps: %li  total delayeds: %li",
                        timeCounter/5000,prompts_count,delayeds_count); 
               string xstr=Xstr;
               message_to_shared_mem(xstr);
 
 
             }
           }
       }
       
     }
   }
   
   float duration_sec = timeCounter_rel/5000.0;
   float frame_start_time_f = (float)frame_start_time;
   float duration_f         = (float)duration_sec;
   if(perform_decay_corr){
     decay_correction_factor = ((duration_f*lambda)/(1.0-exp(-(duration_f*lambda))))*
       exp(frame_start_time_f*lambda);
     
   }
 
   
   //for every second of frame:
   for(int k = 0; k < 192; k++){
     mean_cfd_block_counter[k]=0;
   }
   unsigned long int *cfd_cts        = NULL;
   unsigned long int val_cfd = 0;
 
   for(int i = frame_start_time; i < frame_start_time+duration_sec; i++){   
     cfd_cts = get_cfd_countrate_for_second(i);
     for(int j = 0; j < 192; j++){
       val_cfd = cfd_cts[j];
       mean_cfd_block_counter[j]+=val_cfd;
     }
   }
 
   //divide by framelength:
   for(int i = 0; i < 192; i++){
     mean_cfd_block_counter[i]/=duration_sec;
   }
 
 
   // end while for reading lm-file
   cout << "--- done reading file() ---" << endl;
   cout << "time for DTC map: " << timeCounter/5000 << endl;
   cout << "-----------------------------------" << endl;
   cout << "INFO: LMStreamManager::fill_maps() " << endl;
   cout << "Decay correction factor: "           << decay_correction_factor << endl;
   cout << "prompts: "                           << prompts_count  << endl;
   cout << "delayeds: "                          << delayeds_count << endl;
   cout << "-----------------------------------" << endl;
 
 }
 
 
 
 
 
 //----------------------------------------------------------------------------------------
 //---reset all data structures
 //----------------------------------------------------------------------------------------
 void BrainPET_LMStreamManager::reset(){
   cout << "BrainPET_LMStreamManager::reset()" << endl;
   if(mrpet_dump!=NULL){
     delete mrpet_dump;
     mrpet_dump=NULL;
   }
   if(trigger_manager!=NULL){
     delete trigger_manager;
     trigger_manager=NULL;
   }
   if(epm_pattern!=NULL){
     delete epm_pattern;
     epm_pattern=NULL;
   }
   if(isotope!=NULL){
     delete isotope;
     isotope =NULL;
   }
   if(filenames_lst!=NULL){
     delete[] filenames_lst;
     filenames_lst=NULL;
   }
   if(listmode_file_pointer!=NULL){
     delete[] listmode_file_pointer;
     listmode_file_pointer=NULL;
   }
   if(listmode_file_number!=NULL){
     delete[] listmode_file_number;
     listmode_file_number=NULL;
   }
 
   if(prompts_hc!=NULL){
     delete[] prompts_hc;
     prompts_hc=NULL;
   }
   if(delayeds_hc!=NULL){
     delete[] prompts_hc;
     delayeds_hc=NULL;
   }
   if(countrate_CFD!=NULL){
     delete[] countrate_CFD;
     countrate_CFD=NULL;
   }
   if(countrate_XYE!=NULL){
     delete[] countrate_XYE;
     countrate_XYE=NULL;
   }
   if(countrate_qual!=NULL){
     delete[] countrate_qual;
     countrate_qual=NULL;
   }
   if(countrate_rio_singles!=NULL){
     delete[] countrate_rio_singles;
     countrate_rio_singles=NULL;
   }
 
 
   trigger_manager=NULL;
 
   isotope=NULL;
 
   processing_initialised = false;
 
   cout << "done" << endl;
 }
 
 
 //----------------------------------------------------------------------------------------
 //---initialise all data structures
 //----------------------------------------------------------------------------------------
 void BrainPET_LMStreamManager::init(){
 
   cout << "BrainPET_LMStreamManager::init()" << endl;
   //initialise MRPET_dump object:
   mrpet_dump = NULL;
   mrpet_dump = new MRPET_dump();
   if(mrpet_dump == NULL){    
     cout << "ERROR in BrainPET_LMStreamManager::BrainPET_LMStreamManager()" << endl;
     cout << "MRPET_dump object not initialised! " << endl;
     exit(0);
   }
 
   
   //dead_time_object = NULL;
   trigger_manager = NULL;
   isotope = NULL;
   
   epm_pattern= NULL;
   epm_pattern = new EPMpattern();
   if(epm_pattern == NULL){    
     cout << "ERROR in BrainPET_LMStreamManager::BrainPET_LMStreamManager()" << endl;
     cout << "EPMpattern object not initialised! " << endl;
     exit(0);
   }
 
   //time: 
   start_time = 0;
   end_time = 0;
   frame_length = 0;
 
   //counts:
   n_prompts_total = 0;
   n_delayeds_total = 0;
   
   prompts_hc=NULL;
   delayeds_hc=NULL;
 
   //---counter for block singles:
   countrate_CFD         = NULL;       
   countrate_XYE         = NULL;          
   countrate_qual        = NULL;         
   countrate_rio_singles = NULL;   
 
   //---startposition of data for new second in listmode stream:
   listmode_file_pointer = NULL;
   listmode_file_number  = NULL;
     
   processing_initialised = false;
 
 }
 
 //----------------------------------------------------------------------------------------
 //---Destructor I
 //----------------------------------------------------------------------------------------
 BrainPET_LMStreamManager::~BrainPET_LMStreamManager(){  
 
   cout << "BrainPET_LMStreamManager::~BrainPET_LMStreamManager()";  
   reset();
   cout << "...done" << endl;
 }
 
 
 //----------------------------------------------------------------------------------------
 //----------------------------------------------------------------------------------------
 int BrainPET_LMStreamManager::graycode_sync_file(FILE *fptr){
   if (fptr==NULL)
     {
       cout << "BrainPET_LMStreamManager:::graycode_sync_file:  Error: ";
       cout << "No file stream given!" << endl;
       exit(0);
     }
 
   int i,j,sync_offset,n;
   char *bufr = new char[18];
   int next_gray;
   
   sync_offset=0;
   n=fread( bufr, 1, 18, fptr);
   if (n != 18)
     {
       cout << "BrainPET_LMStreamManager::graycode_sync_file:  Error: ";
       cout << "Not enough bytes in file to do graycode sync...HELP!!!\n";
       exit(0);
     }
 
   for (i=0; i<6; i++) for (j=0; j<8; j++) {
     if (((bufr[i]&0xf0) == graycodes[j]) &&
         ((bufr[i+6]&0xf0) == graycodes[j+1]) && 
         ((bufr[i+12]&0xf0) == graycodes[j+2])) {
           sync_offset=i+1;
           next_gray=(j+4)%8;
     }
   }
   n=fread( bufr, 1, sync_offset, fptr);
   if (n != sync_offset) 
     {
       cout << "BrainPET_LMStreamManager::graycode_sync_file:  Error: ";
       cout << "Not enough bytes in file to do graycode sync...HELP!!!\n";
       exit(0);
     }
  
   printf("Graycode Sync fpos=0x%llx offset=%d starting code=0x%02x\n", 
            (long long unsigned int)ftellx(fptr), sync_offset, graycodes[next_gray]);
   delete[] bufr;
   return(next_gray);
 }
 
 
 
 //----------------------------------------------------------------------------------------
 //---get global start time
 //----------------------------------------------------------------------------------------
 unsigned int BrainPET_LMStreamManager::get_start_time(){
   return start_time;
 }
 
 //----------------------------------------------------------------------------------------
 //---get global end time
 //----------------------------------------------------------------------------------------
 unsigned int BrainPET_LMStreamManager::get_end_time(){
   return end_time;
 }
 
 //-------------------------------------------------
 //---get acquisition time
 //-------------------------------------------------
 unsigned int BrainPET_LMStreamManager::get_frame_length(){
   return frame_length;
 }
 
 //-------------------------------------------------
 //---write object into stream
 //-------------------------------------------------
 void BrainPET_LMStreamManager::write_into_stream(StreamWriter* stream){
 
   if(stream->isWriteable()){
     stream->write(ID_BrainPET_ListmodeManager);
     stream->write(RUNID);
     stream->write(nFiles_lst);
     for(int i = 0; i < nFiles_lst; i++){
       stream->write(filenames_lst[i]);
     }
     stream->write(start_time);
     stream->write(end_time);
     stream->write(frame_length);
     stream->write(n_prompts_total);
     stream->write(n_delayeds_total);
     trigger_manager->write_into_stream(stream);
 
     //write file pointer:
     stream->writeBlock((char *)listmode_file_pointer, sizeof(long int)*frame_length);
     stream->writeBlock((char *)listmode_file_number, sizeof(int)*frame_length);
     //write headcurve:
     stream->writeBlock((char *)prompts_hc, sizeof(long int)*frame_length);
     stream->writeBlock((char *)delayeds_hc, sizeof(long int)*frame_length);
     //write block singels:
     stream->writeBlock((char *)countrate_CFD, sizeof(unsigned long int)*192*frame_length);
     stream->writeBlock((char *)countrate_XYE, sizeof(unsigned long int)*192*frame_length);
     stream->writeBlock((char *)countrate_qual, sizeof(unsigned long int)*192*frame_length);
     stream->writeBlock((char *)countrate_rio_singles, sizeof(unsigned long int)*192*frame_length);
     stream->write(ID_EndTag);
   }else{
     cout << "BrainPET_LMStreamManager::write_into_stream:   Error: ";
     cout << "DataStream not Writeable!" << endl;
   }
   cout << "...done writing BrainPET_LMStreamManager" << endl;
   return;
 }
 
 
 //-------------------------------------------------
 //---read object from stream
 //-------------------------------------------------
 bool BrainPET_LMStreamManager::read_from_stream(StreamReader* stream){
 
   if(processing_initialised){
     cout << "WARNING: data is initialised but will be overwritten!" << endl;
     reset();
     init();
   }
   processing_initialised = false;
   if (stream->isReadable()){
     unsigned short int ID;
     stream->read(ID);
     if (ID==ID_BrainPET_ListmodeManager){
       stream->read(RUNID);
       stream->read(nFiles_lst);
       filenames_lst = new string[nFiles_lst];
       for(int i = 0; i < nFiles_lst; i++){
           stream->read(filenames_lst[i]);
       }
       stream->read(start_time);
       stream->read(end_time);
       stream->read(frame_length);
       stream->read(n_prompts_total);
       stream->read(n_delayeds_total);
       trigger_manager = new MR_TriggerManager();
       trigger_manager->read_from_stream(stream);
       cout << "BrainPET_LMStreamManager:: read_from_stream...read triggermanager" << endl;
       
       //read file pointer:
       if(listmode_file_pointer == NULL){
           listmode_file_pointer = (long int *)calloc(frame_length, sizeof(long int));
       }else{
           cout << "ERROR in BrainPET_LMStreamManager::init_conversion(); array not NULL before acquisition!" << endl;
           exit(0);
       }      
       stream->readBlock((char *)listmode_file_pointer, sizeof(long int)*frame_length);
 
       if(listmode_file_number == NULL){
           listmode_file_number = (int *)calloc(frame_length, sizeof(int));
       }else{
           cout << "ERROR in BrainPET_::init_conversion(); array not NULL before acquisition!" << endl;
           exit(0);
       }      
       stream->readBlock((char *)listmode_file_number, sizeof(int)*frame_length);
 
       //read headcurve
       if(prompts_hc == NULL){
           prompts_hc = (long int *)calloc(frame_length, sizeof(long int));
       }else{
           cout << "ERROR in BrainPET_LMStreamManager::init_conversion(); array not NULL before acquisition!" << endl;
           exit(0);
       }      
       stream->readBlock((char *)prompts_hc, sizeof(long int)*frame_length);
 
       if(delayeds_hc == NULL){
           delayeds_hc = (long int *)calloc(frame_length, sizeof(long int));
       }else{
           cout << "ERROR in BrainPET_LMStreamManager::init_conversion(); array not NULL before acquisition!" << endl;
           exit(0);
       }      
       stream->readBlock((char *)delayeds_hc, sizeof(long int)*frame_length);
 
 
       //read block singels:
       if(countrate_CFD == NULL){
           countrate_CFD = (unsigned long int *)calloc(192*frame_length, sizeof(unsigned long int));
       }else{
           cout << "ERROR in BrainPET_LMStreamManager::init_conversion(); array not NULL before acquisition!" << endl;
           exit(0);
       }
       if(countrate_XYE == NULL){
           countrate_XYE = (unsigned long int *)calloc(192*frame_length, sizeof(unsigned long int));
       }else{
           cout << "ERROR in BrainPET_LMStreamManager::init_conversion(); array not NULL before acquisition!" << endl;
           exit(0);
       }
       if(countrate_qual == NULL){
           countrate_qual = (unsigned long int *)calloc(192*frame_length, sizeof(unsigned long int));
       }else{
           cout << "ERROR in BrainPET_LMStreamManager::init_conversion(); array not NULL before acquisition!" << endl;
           exit(0);
       }
       if(countrate_rio_singles == NULL){
           countrate_rio_singles = (unsigned long int *)calloc(192*frame_length, sizeof(unsigned long int));
       }else{
           cout << "ERROR in BrainPET_LMStreamManager::init_conversion(); array not NULL before acquisition!" << endl;
           exit(0);
       }
       stream->readBlock((char *)countrate_CFD, sizeof(unsigned long int)*192*frame_length);
       stream->readBlock((char *)countrate_XYE, sizeof(unsigned long int)*192*frame_length);
       stream->readBlock((char *)countrate_qual, sizeof(unsigned long int)*192*frame_length);
       stream->readBlock((char *)countrate_rio_singles, sizeof(unsigned long int)*192*frame_length);
       unsigned short int endtag;
       stream->read(endtag);
       if (ID_EndTag!=endtag){
           cout << "ERROR: BrainPET_LMStreamManager::read_from_stream" << endl;
           cout << "End Tag not identified correctly!" << endl;
           return false;         
       }
     }else{
       cout << "ERROR: BrainPET_LMStreamManager::read_from_stream(StreamReader *stream)"<< endl;
       cout << "ClassID not identified correctly." << endl;
       return false;
     }
   }else{
     cout << "ERROR: BrainPET_LMStreamManager::read_from_stream(StreamReader *stream)"<< endl;
     cout << "stream is not readable." << endl;
     return false;
   }
 
   processing_initialised = true;
   cout << "...done reading BrainPET_LMStreamManager" << endl;
   return true;
 }
 
 
 //-------------------------------------------------
 //---get handle on trigger manager
 //-------------------------------------------------
 MR_TriggerManager* BrainPET_LMStreamManager::get_MR_TriggerManager(void)
 {
   return trigger_manager;
 }
 
 //-------------------------------------------------
 //---get total number of trigger events
 //-------------------------------------------------
 int BrainPET_LMStreamManager::get_number_of_trigger_events(void)
 {
   if (trigger_manager==NULL) return 0;
   return trigger_manager->get_number_of_events();
 }
 
 //-------------------------------------------------
 //---get time of trigger event since start
 //---1 tick == 200 micro sec
 //---return -1 if requested trigger number not available
 //-------------------------------------------------
 long long int BrainPET_LMStreamManager::get_triggerevent_time(int trig_number)
 {
   if (trigger_manager==NULL) return -1;
   return trigger_manager->get_trigger_time(trig_number);
 }
 
 
 //-------------------------------------------------
 //---get time of trigger event since start (seconds)
 //---return -1 if requested trigger number not available
 //-------------------------------------------------
 float BrainPET_LMStreamManager::get_triggerevent_time_sec(int trig_number)
 {
   long long int t=get_triggerevent_time(trig_number);
   if (t<0) return -1.0;
   
 
   t/=100;
   float tt=((float)t)/50.0;
   return tt;
 
 }
 
 
 //-------------------------------------------------
 //---check consistency between listmode files and 
 //---header information after processing or reading of the object
 //-------------------------------------------------
 bool BrainPET_LMStreamManager::check_processing(string path, string listmode_header){
   bool is_ok = false;
 
   string header_file = listmode_header;
   FileAccess fa_header(header_file);
   if (!fa_header.exists())
     {
       cout << "BrainPET_LMStreamManager::check_processing(" << header_file << ") does not exist" << endl;
       return false;
     }
 
   fa_header.open_readonly(true);
   if(fa_header.isReadable()){
     cout << "BrainPET_LMStreamManager::check_processing(" << header_file << ") exists" << endl;
     is_ok = read_from_stream(&fa_header);
     if(!is_ok){
       cout << "ERROR: reading object" << endl;
       return false;
     }
     fa_header.close();    
   }else{
     cout << "BrainPET_LMStreamManager::check_processing(" << header_file << ") not readable" << endl;
     fa_header.close();
     return false;
   }
   
   cout << "check listmode files" << endl;
   string current_lst_file;
   for(int ifile = 0; ifile < nFiles_lst;ifile++){
     current_lst_file = path + "/" + filenames_lst[ifile];
     FileAccess fa(current_lst_file);
     if (!fa.exists())
       {
           cout << "Path: " << path << endl;
           cout << "Header: " << filenames_lst[ifile] << endl;
           cout << current_lst_file << " does not exist" << endl;
           return false;
       }
     else
       {
           fa.open_readonly(true);
           if(fa.isReadable()){
             cout << "Path: " << path << endl;
             cout << "Header: " << filenames_lst[ifile] << endl;
             cout << current_lst_file << " exists" << endl;      
           }else{
             cout << "Path: " << path << endl;
             cout << "Header: " << filenames_lst[ifile] << endl;
             cout << current_lst_file << " does not exist" << endl;
             return false;
           }
       }    
   }
   is_ok = true;
   return is_ok;
 }
 
 
 //-------------------------------------------------
 //--- some specifics
 //--- Larry's comment my version of ftello 
 //--- which doesn't seem to work beyond 2G
 //-------------------------------------------------
 off_t BrainPET_LMStreamManager::ftellx(FILE *stream) {
   //  return(lseek(fileno(stream), 0, SEEK_CUR));
   #ifdef _OS_MAC_
   return(ftello(stream));
   #else
   return(ftello64(stream));
   #endif
 }
 
 
 
 //------------------------------------------------------------------------
 //---make gif file prompts headcurve
 //------------------------------------------------------------------------
 TGraph* BrainPET_LMStreamManager::get_TGraph_prompts_hc(int max_sec)
 {
   char title1[1000];
   sprintf(title1,"Prompt_events_%s", RUNID.c_str());
 
   float *tmp = new float[frame_length];
   float *time_scale1 = new float[frame_length];
     
   int Nbins=(int)frame_length;
   if (max_sec>0)
     {
       if (max_sec<(int)frame_length)
           {
             Nbins=max_sec;
           }
     }
 
   for(int i = 0; i < Nbins; i++){
     tmp[i]        = (float)prompts_hc[i];
     time_scale1[i]= (float)i;
   }
   
   TGraph *graph_prompts=new TGraph(Nbins-1, time_scale1, tmp);
   graph_prompts->SetNameTitle(title1,title1);
   graph_prompts->GetXaxis()->SetTitle("time / s");
   graph_prompts->GetYaxis()->SetTitle("prompt cps");
   graph_prompts->SetLineColor(1); 
   delete[] tmp;
   delete[] time_scale1;
 
   return graph_prompts;  
 }
 
 
 //------------------------------------------------------------------------
 //---make gif file delayeds headcurve
 //------------------------------------------------------------------------
 TGraph* BrainPET_LMStreamManager::get_TGraph_delayed_hc(int max_sec)
 {
   char title1[1000];
   sprintf(title1,"Delayed_events_%s", RUNID.c_str());
 
   float *tmp = new float[frame_length];
   float *time_scale1 = new float[frame_length];
 
   int Nbins=frame_length;
   if (max_sec>0)
     {
       if (max_sec<(int)frame_length)
           {
             Nbins=max_sec;
           }
     }
     
   for(int i = 0; i < Nbins; i++){
     tmp[i]        = (float)delayeds_hc[i];
     time_scale1[i]= (float)i;
   }
   
   TGraph *graph=new TGraph(Nbins-1, time_scale1, tmp);
   graph->SetNameTitle(title1,title1);
   graph->GetXaxis()->SetTitle("time / s");
   graph->GetYaxis()->SetTitle("delayed cps");
   graph->SetLineColor(1); 
   delete[] tmp;
   delete[] time_scale1;
   return graph;
   
 }
 
 
 
 //------------------------------------------------------------------------
 //---define shared memory message buffer
 //------------------------------------------------------------------------
 void BrainPET_LMStreamManager::define_shared_mem(SharedMem* sh_mem0)
 {
   sh_mem=sh_mem0;
 }
 
 
 //------------------------------------------------------------------------
 //---message to shared memory
 //------------------------------------------------------------------------
 void BrainPET_LMStreamManager::message_to_shared_mem(string message)
 {
   if (sh_mem!=NULL)
     {
       sh_mem->write_message(message);
     }
       
 }
 
 
 //------------------------------------------------------------------------
 //---display prompts headcurve
 //------------------------------------------------------------------------
 bool BrainPET_LMStreamManager::displayPrompts_hc(){
 
   char title1[400];
   sprintf(title1,"Prompt_events_%s", RUNID.c_str());
   TCanvas *can_hc_prompts = new TCanvas(title1,title1);
   can_hc_prompts->SetGrid();
 
   float *tmp = new float[frame_length];
   float *time_scale1 = new float[frame_length];
   
   
   for(int i = 0; (unsigned int)i < frame_length; i++){
     tmp[i]        = (float)prompts_hc[i];
     time_scale1[i]= (float)i;
   }
   
   
 
   TGraph *graph_prompts=new TGraph(frame_length-1, time_scale1, tmp);
   graph_prompts->SetNameTitle(title1,title1);
   graph_prompts->Draw("AL");
   graph_prompts->GetXaxis()->SetTitle("time / s");
   graph_prompts->GetYaxis()->SetTitle("prompt cps");
   graph_prompts->SetLineColor(1);
   
   can_hc_prompts->Update();
   delete[] tmp;
   delete[] time_scale1;
   return true;
 }
 
 
 //------------------------------------------------------------------------
 //---display delayeds headcurve
 //------------------------------------------------------------------------
 bool BrainPET_LMStreamManager::displayDelayeds_hc(){
 
   char title1[400];
   sprintf(title1,"Delayed_events_%s", RUNID.c_str());
   TCanvas *can_hc_delayeds = new TCanvas(title1,title1);
   can_hc_delayeds->SetGrid();
 
   float *tmp = new float[frame_length];
   float *time_scale1 = new float[frame_length];
   
   
   for(int i = 0; (unsigned int)i < frame_length; i++){
     tmp[i]= (float)delayeds_hc[i];
     time_scale1[i]= (float)i;
   }
   
   
 
   TGraph *graph_delayeds=new TGraph(frame_length-1, time_scale1, tmp);
   delete[] tmp;
   delete[] time_scale1;
   graph_delayeds->SetNameTitle(title1,title1);
   graph_delayeds->Draw("AL");
   graph_delayeds->GetXaxis()->SetTitle("time / s");
   graph_delayeds->GetYaxis()->SetTitle("delayed cps");
   graph_delayeds->SetLineColor(1);
   
   can_hc_delayeds->Update();
  
   return true;
 }
 
 
 
 //------------------------------------------------------------------------
 //---return CFD countrate of required second
 //------------------------------------------------------------------------
 unsigned long int *BrainPET_LMStreamManager::get_cfd_countrate_for_second(int second){
 
   if((unsigned int)second >= start_time && (unsigned int)second <= end_time){
     return &countrate_CFD[192*second];
   }else{
     cout << "WARNING: " << second << " is out of bounds" << endl;
     return NULL;
     exit(0);
   }
 
 }
 
 
 //------------------------------------------------------------------------
 //---fill map by definition of start and endtime
 //------------------------------------------------------------------------
 void BrainPET_LMStreamManager::fill_maps_by_timedef(string listmode_path,
                                                                 long int frame_start_time, long int frame_end_time,
                                                                 bool perform_decay_corr,
                                                                 float &decay_correcton_factor,
                                                                 BrainPET_LORfileData *prompts_lor, 
                                                                 BrainPET_LORfileData *delayeds_lor, 
                                                                 BrainPET_DetHitMap *pcmap, 
                                                                 BrainPET_DetHitMap *dcmap,
                                                                 unsigned long int *mean_cfd_block_counter,
                                                                 bool reset_data){
 
   cout << "BrainPET_LMStreamManager::fill_maps_by_timedef" << endl;
 
   long int n_prompts  = 0;
   long int n_delayeds = 0;
   long int n_trues    = 0;  
   long int equal_stat_stop_time = 0;//dummy is not used:
   fill_maps(listmode_path, frame_start_time, frame_end_time,
               n_prompts, n_delayeds, n_trues, 
               perform_decay_corr,
               decay_correcton_factor, 
               prompts_lor,  delayeds_lor, 
               pcmap, dcmap,
               equal_stat_stop_time,
               mean_cfd_block_counter,
               reset_data);
 
   
 }
 
 //------------------------------------------------------------------------
 //---fill map by definition of counts
 //---filling maps with equal statistics
 //---equal_stat_stop_time contains end time of frame -> start time for next frame
 //---set or "n_prompts" or "n_delayeds" or "n_trues" to number of desired counts for this frame
 //---set frame_start_time to start-time for this frame
 //------------------------------------------------------------------------
 void BrainPET_LMStreamManager::fill_maps_by_countdef(string listmode_path,
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
                                                                  bool reset_data){
   
 
   
   cout << "BrainPET_LMStreamManager::fill_maps_by_countdef" << endl;
   if(n_prompts != 0){
     cout << "FRAME TO EQUAL PROMPTS STATISTICS" << endl;
   }
   if(n_delayeds != 0){
     cout << "FRAME TO EQUAL DELAYEDS STATISTICS" << endl;
   }
   if(n_trues != 0){
     cout << "FRAME TO EQUAL TRUES STATISTICS" << endl;
   }
   
   fill_maps(listmode_path, frame_start_time, global_end_time,
               n_prompts, n_delayeds, n_trues,
               perform_decay_corr,
               decay_correcton_factor, 
               prompts_lor, delayeds_lor, 
               pcmap, dcmap,
               equal_stat_stop_time,
               mean_cfd_block_counter,
               reset_data);
 }
 
 
 
 
//Generated on Tue Jun 18 17:17:16 2013 for *** PRESTO *** Pet Reconstruction Software TOolbox *** revision 333 *** by  ￼ 1.5.6
