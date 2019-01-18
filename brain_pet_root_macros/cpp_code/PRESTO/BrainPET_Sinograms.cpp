
#include "BrainPET_Sinograms.hh"



//----------------------------------------------------------------------------------------
//---Constructor I
//----------------------------------------------------------------------------------------
BrainPET_Sinograms::BrainPET_Sinograms(MRPET_dump *brainPET1, string title0,
    int verbose_level){

  verbose = verbose_level;
  BrainPET1 = brainPET1;
  sinogram_flatformat = NULL;
  format_version=1;
  maxrd =  brainPET1->get_max_ringdiff();
  span =   brainPET1->get_span();
  nPlanes= brainPET1->get_n_planes();
  nViews = brainPET1->get_n_views(); 
  nProjs = brainPET1->get_n_projs(); 
  nEvents_all=0.0;

  //--- allocate memory for sinograms in flat format
  sinogram_flatformat = new float[nViews*nProjs*nPlanes];
  if(sinogram_flatformat==NULL){
    cout << "ERROR in BrainPET_Sinograms::BrainPET_Sinograms" << endl;
    cout << "out of memory" << endl;
    exit(0);
  }
  for(int i = 0; i< nViews*nProjs*nPlanes; i++)
    sinogram_flatformat[i]= 0.0;
  //--- allocate memory for sinograms
  sinograms = new R_BrainPET_SinogramData *[nPlanes];
  if(sinograms == NULL){
    cout << "ERROR in BrainPET_Sinograms::BrainPET_Sinograms" << endl;
    cout << "out of memory" << endl;
    exit(0);
  }
  //--- generate sinograms for promtps and delayeds
  for(int i = 0; i < nPlanes; i++){
    float *sino_plane = sinogram_flatformat + (i*nViews*nProjs);
    sinograms[i]  = new R_BrainPET_SinogramData(BrainPET1, nViews, nProjs, i, sino_plane);
    if(sinograms[i]==NULL){
      cout << "Error in BrainPET_Sinograms " << endl;
      cout << "Failed to acquire Sinograms " << endl;
      cout << "Probably out of memory" << endl;
      exit(0);
    }
  }

  //---unique title
  title=title0;
}


//----------------------------------------------------------------------------------------
//--- set title
//----------------------------------------------------------------------------------------
void BrainPET_Sinograms::set_title(string title0)
{
  title=title0;
}



//----------------------------------------------------------------------------------------
//---conversion from BrainPET_LORfileData to BrainPET_Sinograms
//---LOR-file contains data, that will be filled into this sinogram
//----------------------------------------------------------------------------------------
void BrainPET_Sinograms::LOR2Sino(BrainPET_LORfileData *lor_data,
    bool use_lookup,
    char* lookup_filename){

  if(verbose> 0){
    cout << "call:BrainPET_Sinograms::LOR2Sino() "
      << "filling sinogram with LOR data" 
      << endl;
  }

  nEvents_all=0.0;
  resetAllSinograms();


  int lor_data_offset = 0;
  int lut_offset = 0;
  int index = 0;
  int *lorlookup=NULL;

  if(use_lookup){
    if(verbose> 0){
      cout << "use lookuptable " << lookup_filename  << endl;
    }
    if(lookup_filename==NULL){
      cout << "ERROR: BrainPET_Sinograms::BrainPET_Sinograms(): "
        << "LOR LOOKUPTABLE NOT FOUND!" << endl;
      exit(0);
    }
    FileAccess *lor_LUT_File = new FileAccess(lookup_filename);
    lor_LUT_File->open_readonly(true);

    if(lor_LUT_File->isReadable()){
      lorlookup = new int[304*864*864];
      if(lorlookup ==NULL){
        cout << "ERROR in BrainPET_Sinograms::LOR2Sino" << endl;
        cout << "out of memory" << endl;
        exit(0);
      }
      lor_LUT_File->readBlock((char *)lorlookup, sizeof(int)*304*864*864);
      lor_LUT_File->close();
    }else{
      cout << "**ERROR in BrainPET_Sinograms::BrainPET_Sinograms(): LOR LOOKUPTABLE NOT READABLE!"<< endl;
      exit(0);
    }
  }else{
    if(verbose> 0){
      cout << "NO lookuptable used for conversion..." << endl;
    }
  }



  //for every of the 304 modulpairs:
  int bp_n_mpairs = BrainPET1->get_n_mpairs();
  for (int i=0; i<bp_n_mpairs; i++){
    int amod, bmod, ax, ay, bx, by;
    int rd; //, j;
    //float fv;
    float deta_pos[3]; //(xyz) for detector a
    float detb_pos[3]; //(xyz) for detector b
    float lor[4]; // LOR: (r, phi, z, tan_theta)
    int view;
    int proj;
    int plane;
    int segment;
    int mod_nx = BrainPET1->get_mod_nx();
    int mod_ny = BrainPET1->get_mod_ny();

    //(amod, bmod) is combination of modulpairs in the allowed fov(19)
    amod=BrainPET1->mpair_a[i];
    bmod=BrainPET1->mpair_b[i];

    //cout << "mpair " << i << " of " << brainPET1->n_mpairs << endl;
    int verbose = 0;
    if(verbose> 0){
      printf("progress: MP%03d of %d %c",i,304,(verbose>1)?'\n':'\r'); 
      fflush(stdout);
    }

    for (ax=0; ax < mod_nx; ax++){
      for (ay=0; ay < mod_ny; ay++) {
        BrainPET1->det_to_phy(amod, ax, ay, deta_pos);
        for (bx=0; bx < mod_nx; bx++)
          for (by=0; by < mod_ny; by++) {
            rd=by-ay; // ring diff
            if (rd<0) 
              rd=ay-by;
            if (rd > maxrd)//if current ring difference > max ringdiff 
              continue;

            //detector to physical coords
            BrainPET1->det_to_phy( bmod, bx, by, detb_pos);
            //detector pair to lor(r, phi, z, tan_theta)
            BrainPET1->phy_to_pro( deta_pos, detb_pos, lor);
            //flat index in sinogram (or projection, view, plane)

            if (use_lookup){ 
              index=lorlookup[lut_offset+(ax+ay*mod_nx)*mod_nx*mod_ny+bx+by* mod_nx];          
            }
            else{
              index = BrainPET1->pro_to_addr(lor, proj, view, plane, segment);
            }
            // index_lor is position in lor array
            int index_lor = ((by/12)*144+12*(11-bx)+(11-(by%12)))+
              ((ay/12)*144+12*(11-ax)+(11-(ay%12)))*864;
            index_lor +=lor_data_offset;

            float val2 = lor_data->get_lor_data(index_lor);

            // add event to sinogram, hence several lors have one
            // bin in sinogram (thus no usage of "set value")
            if(index>=0 && index < nPlanes*nViews*nProjs){
              add_event_to_sinogram1(index, val2);
              // event counter for verification
              nEvents_all+=val2;          
            }
          }
      }
    }
    // offset to index_lor for next module pair:
    lor_data_offset += 864*864;
    lut_offset += 864*864;
  }
  if(verbose> 0){
    cout <<"TOTAL NUMBER OF EVENTS IN SINOGRAM:" << get_n_Events() << endl;
  }
  delete[] lorlookup;

}



//----------------------------------------------------------------------------------------
//---conversion from BrainPET_LORfileData to BrainPET_Sinograms
//---LOR-file contains data, that will be filled into this sinogram
//----------------------------------------------------------------------------------------
void BrainPET_Sinograms::LOR2Sino(BrainPET_LORfileData *lor_data,
    int* lookup)
{

  if(verbose> 0){
    cout << "call:BrainPET_Sinograms::LOR2Sino() "
      << "filling sinogram with LOR data" 
      << endl;
  }

  nEvents_all=0.0;
  resetAllSinograms();


  int lor_data_offset = 0;
  int lut_offset = 0;
  int index = 0;
  int *lorlookup=lookup;

  if(lorlookup==NULL){
    cout << "**ERROR in BrainPET_Sinograms::BrainPET_Sinograms(): LOR LOOKUPTABLE = NULL!"<< endl;
    exit(0);
  }

  //for every of the 304 modulpairs:
  int bp_n_mpairs = BrainPET1->get_n_mpairs();
  for (int i=0; i<bp_n_mpairs; i++){
    int amod, bmod, ax, ay, bx, by;
    int rd; //, j;
    //float fv;
    /*
       float deta_pos[3]; //(xyz) for detector a
       float detb_pos[3]; //(xyz) for detector b
       float lor[4]; // LOR: (r, phi, z, tan_theta)
       int view;
       int proj;
       int plane;
       int segment;
       */
    int mod_nx = BrainPET1->get_mod_nx();
    int mod_ny = BrainPET1->get_mod_ny();

    //(amod, bmod) is combination of modulpairs in the allowed fov(19)
    amod=BrainPET1->mpair_a[i];
    bmod=BrainPET1->mpair_b[i];

    //cout << "mpair " << i << " of " << brainPET1->n_mpairs << endl;
    int verbose = 0;
    if(verbose> 0){
      printf("progress: MP%03d of %d %c",i,304,(verbose>1)?'\n':'\r'); 
      fflush(stdout);
    }

    for (ax=0; ax < mod_nx; ax++){
      for (ay=0; ay < mod_ny; ay++) {
        //BrainPET1->det_to_phy(amod, ax, ay, deta_pos);
        for (bx=0; bx < mod_nx; bx++)
          for (by=0; by < mod_ny; by++) {
            rd=by-ay; // ring diff
            if (rd<0) 
              rd=ay-by;
            if (rd > maxrd)//if current ring difference > max ringdiff 
              continue;


            index=lorlookup[lut_offset+(ax+ay*mod_nx)*mod_nx*mod_ny+bx+by* mod_nx];            

            // index_lor is position in lor array
            int index_lor = ((by/12)*144+12*(11-bx)+(11-(by%12)))+
              ((ay/12)*144+12*(11-ax)+(11-(ay%12)))*864;
            index_lor +=lor_data_offset;

            float val2 = lor_data->get_lor_data(index_lor);

            // add event to sinogram, hence several lors have one
            // bin in sinogram (thus no usage of "set value")
            if(index>=0 && index < nPlanes*nViews*nProjs){
              add_event_to_sinogram1(index, val2);
              // event counter for verification
              nEvents_all+=val2;          
            }
          }
      }
    }
    // offset to index_lor for next module pair:
    lor_data_offset += 864*864;
    lut_offset += 864*864;
  }
  if(verbose> 0){
    cout <<"TOTAL NUMBER OF EVENTS IN SINOGRAM:" << get_n_Events() << endl;
  }


}






//----------------------------------------------------------------------------------------
//---conversion from BrainPET_Sinograms  to BrainPET_LORfileData
//---LOR-file will be overwritten with data from sinogram
//----------------------------------------------------------------------------------------
void BrainPET_Sinograms::sino2LOR(BrainPET_LORfileData *lor_data,
    bool use_lookup,
    char* lookup_filename, 
    bool sino_contains_dwell){


  if(verbose> 0){
    cout << "call:BrainPET_Sinograms::sino2LOR() "
      << "filling LOR data with Sinogram data" 
      << endl;
  }


  int lor_data_offset = 0;
  int lut_offset = 0;
  int index_sino = 0;
  int index_lor = 0;
  int *lorlookup=NULL;
  float value = 0;

  if(use_lookup){
    if(verbose> 0){
      cout << "use lookuptable " << lookup_filename  << endl;
    }
    if(lookup_filename==NULL){
      cout << "ERROR: BrainPET_Sinograms::BrainPET_Sinograms(): "
        << "LOR LOOKUPTABLE NOT FOUND!" << endl;
      exit(0);
    }
    FileAccess *lor_LUT_File = new FileAccess(lookup_filename);
    lor_LUT_File->open_readonly(true);

    if(lor_LUT_File->isReadable()){
      lorlookup = new int[304*864*864];
      if(lorlookup ==NULL){
        cout << "ERROR in BrainPET_Sinograms::sino2LOR" << endl;
        cout << "out of memory" << endl;
        exit(0);
      }
      lor_LUT_File->readBlock((char *)lorlookup, sizeof(int)*304*864*864);
      lor_LUT_File->close();
    }else{
      cout << "**ERROR in BrainPET_Sinograms::BrainPET_Sinograms(): LOR LOOKUPTABLE NOT READABLE!"<< endl;
      exit(0);
    }
  }else{
    if(verbose> 0){
      cout << "NO lookuptable used for conversion..." << endl;
    }
  }


  BrainPET_LORfileData *dwell_lor = NULL;
  BrainPET_Sinograms *sino_dwell = NULL;

  if(sino_contains_dwell){
    dwell_lor = new BrainPET_LORfileData(304,864*864,true);
    dwell_lor->set_all_channels(1.0);
    sino_dwell = new BrainPET_Sinograms(BrainPET1);
    sino_dwell->LOR2Sino(dwell_lor,use_lookup, lookup_filename);
    delete dwell_lor;
  }

  int bp_n_mpairs = BrainPET1->get_n_mpairs();
  //for every of the 304 modulpairs:
  for (int i=0; i < bp_n_mpairs; i++){
    int amod, bmod, ax, ay, bx, by;
    int rd; //, j;
    //float fv;
    float deta_pos[3]; //(xyz) for detector a
    float detb_pos[3]; //(xyz) for detector b
    float lor[4]; // LOR: (r, phi, z, tan_theta)
    int view;
    int proj;
    int plane;
    int segment;
    int mod_nx = BrainPET1->get_mod_nx();
    int mod_ny = BrainPET1->get_mod_ny();


    //(amod, bmod) is combination of modulpairs in the allowed fov(19)
    amod=BrainPET1->mpair_a[i];
    bmod=BrainPET1->mpair_b[i];

    //cout << "mpair " << i << " of " << brainPET1->n_mpairs << endl;

    if(verbose> 0){
      printf("progress: MP%03d of %d %c",i,304,(verbose>1)?'\n':'\r'); 
      fflush(stdout);
    }

    for (ax=0; ax < mod_nx; ax++){
      for (ay=0; ay < mod_ny; ay++) {
        BrainPET1->det_to_phy(amod, ax, ay, deta_pos);
        for (bx=0; bx < mod_nx; bx++)
          for (by=0; by < mod_ny; by++) {
            rd=by-ay; // ring diff
            if (rd<0) 
              rd=ay-by;
            if (rd > maxrd)//if current ring difference > max ringdiff 
              continue;

            //detector to physical coords
            BrainPET1->det_to_phy( bmod, bx, by, detb_pos);
            //detector pair to lor(r, phi, z, tan_theta)
            BrainPET1->phy_to_pro( deta_pos, detb_pos, lor);
            //flat index in sinogram (or projection, view, plane)

            if (use_lookup){ 
              index_sino=lorlookup[lut_offset+(ax+ay*mod_nx) * mod_nx*mod_ny+bx+by * mod_nx];           
            }
            else{
              index_sino = BrainPET1->pro_to_addr(lor, proj, view, plane, segment);
            }
            // index_lor is position in lor array
            index_lor = ((by/12)*144+12*(11-bx)+(11-(by%12)))+
              ((ay/12)*144+12*(11-ax)+(11-(ay%12)))*864;
            index_lor +=lor_data_offset;

            value = get_event_from_address(index_sino);
            if(sino_contains_dwell){
              float dwell_value = sino_dwell->get_event_from_address(index_sino);
              if(dwell_value!=0){
                value /= dwell_value;
              }else{
                value = 0;
              }
            }
            // add event to sinogram, hence several lors have one
            // bin in sinogram (thus no usage of "set value")
            if((index_sino>=0 && index_sino) < (nPlanes*nViews*nProjs)){
              lor_data->set_lor_data(index_lor, value);
              // event counter for verification
              nEvents_all+=value;         
            }
          }
      }
    }
    // offset to index_lor for next module pair:
    lor_data_offset += 864*864;
    lut_offset += 864*864;
  }

  if(verbose> 0){
    cout <<"TOTAL NUMBER OF EVENTS IN LOR FILE:" << lor_data->get_count_sum() << endl;
  }
  delete[] lorlookup;
  delete sino_dwell;

}



//----------------------------------------------------------------------------------------
//---set values in sinogram plane to value
//----------------------------------------------------------------------------------------
void BrainPET_Sinograms::set_single_plane_to_value(int planenumber, float value){
  if(verbose> 0){
    cout << "BrainPET_Sinograms::set_single_plane_to_value(" 
      << planenumber<< ") to value "<< value << endl;
  }
  //int view, proj;
  //int addr;
  if(planenumber>= 0 && planenumber < nPlanes){
    //     for(int view = 0; view < nViews; view++){
    //       for(int proj =0; proj < nProjs; proj++){
    //          BrainPET1->proj_to_addr(view, proj, planenumber, addr);
    //          sinogram_flatformat[addr]=value;
    //       }
    //     }
    sinograms[planenumber]->setAllSinogramBins(value);
  }else{
    cout << "ERROR: planenumber out of range " << endl;
  }
}


//----------------------------------------------------------------------------------------
//--- set event at flat adress
//----------------------------------------------------------------------------------------
void BrainPET_Sinograms::set_event_at_address(int addr, float value){
  int view, proj, plane;

  if(addr >= 0 && addr < nPlanes*nViews*nProjs){
    BrainPET1->addr_to_view_proj_plane(addr, view, proj, plane);      
    if((plane < nPlanes) && 
        (view < nViews) && 
        (proj < nProjs) &&
        (plane >= 0) && 
        (view >= 0) &&
        (proj >= 0))
    {
      //sinograms[plane]->setElement(view, proj, value);   
      sinogram_flatformat[addr]=value;
    }else{
      if(verbose> 0){
        cout << "plane, view, proj:  out of range "
          << "plane: " << plane
          << "view: " << view
          << "proj" << proj
          << endl;
      }
    }
  }else{
    cout << "addr out of range "; 
  }
}


//----------------------------------------------------------------------------------------
//---function to make Siemens 3D-scatterestimation usable 
//---for PRESTO
//---USE WITH CAUTION!!!!
//----------------------------------------------------------------------------------------
void BrainPET_Sinograms::copy_planes_for_hi_scatter_est(){

  if(verbose> 0){
    cout << "BrainPET_Sinograms::copy_planes_for_hi_scatter_est()" << endl;
    cout << "use this function only for scatter sinogram generated with "
      << "Siemens hi_scatter3D! "<< endl;
  }

  //replace incomplete planes for scatter estimation by 
  //complete neighbouring plane:
  //hard coded!

  //********************************************
  const int planes_to_replace = 26;
  int bad_planes[planes_to_replace]={689, 795, 796, 902, 903, 991, 992, 1080, 1081, 1151, 1152, 1222, 1223, 
    1224, 1274, 1275, 1276, 1277, 1327, 1328, 1329, 1330, 1362, 1363, 1364, 1365};
  int good_planes[planes_to_replace] ={690, 794, 797, 901, 904, 990, 993, 1079, 1082, 1150, 1153, 1221, 1225, 
    1225, 1273, 1273, 1278, 1278, 1326, 1326, 1331, 1331, 1361, 1361, 1366, 1366};

  //********************************************
  for(int plane = 0; plane < planes_to_replace; plane++){

    if(verbose> 0){
      cout << "replace plane sinograms[" <<bad_planes[plane] 
        << "] by plane sinograms[" << good_planes[plane] << "] " << endl;
    }
    //delete old plane:
    delete sinograms[bad_planes[plane]];
    //replace by now plane:
    sinograms[bad_planes[plane]] = new R_BrainPET_SinogramData(sinograms[good_planes[plane]]);

    if(sinograms ==NULL){
      cout << "ERROR in BrainPET_Sinograms::copy_planes_for_hi_scatter_est" << endl;
      cout << "out of memory" << endl;
      exit(0);
    }
    int index1;
    int index2;

    //edit also flat format:
    for(int view = 0; view < nViews; view++){
      for(int proj = 0; proj < nProjs; proj++){   
        index1 = bad_planes[plane]*(nViews*nProjs) +  view*nProjs + proj;
        index2 = good_planes[plane]*(nViews*nProjs) +  view*nProjs + proj;
        sinogram_flatformat[index1] = sinogram_flatformat[index2];              
      }
    }

  }
}

//----------------------------------------------------------------------------------------
//---compute bin-wise inverse of each bin value
//---if bin value == 0, set inverse to bin_zero_value (default 0)
//----------------------------------------------------------------------------------------
bool BrainPET_Sinograms::compute_bin_inverse(float bin_zero_value){
  if(verbose> 0){
    cout << "BrainPET_Sinograms::compute_bin_inverse(" << bin_zero_value << ")" << endl;
  }
  //---add value in flat format
  float curr_binvalue = 0;
  for(int addr = 0; addr< nViews*nProjs*nPlanes; addr++){
    curr_binvalue = sinogram_flatformat[addr];
    if(curr_binvalue <= 0.  1 && curr_binvalue >= -0.  1){
      sinogram_flatformat[addr] = bin_zero_value;
    }else{
      sinogram_flatformat[addr] = 1.0/curr_binvalue;
    }
  }
  return false;
}

//----------------------------------------------------------------------------------------
//--- apply dwell correction to data;
//----------------------------------------------------------------------------------------
bool BrainPET_Sinograms::apply_dwell_correction(bool use_lookup,
    char* lookup_filename){
  if(verbose> 0){
    cout << "BrainPET_Sinograms::apply_dwell_correction()" << endl;
  }
  MRPET_dump *brainPET1 = new MRPET_dump();
  BrainPET_Sinograms *dwell_sino = new BrainPET_Sinograms(brainPET1);
  compute_dwell_sino(dwell_sino, use_lookup, lookup_filename);
  divide_by_sinogram(dwell_sino);

  delete dwell_sino;
  return false;
}



//----------------------------------------------------------------------------------------
//--- apply dwell correction to data;
//----------------------------------------------------------------------------------------
bool BrainPET_Sinograms::apply_dwell_correction(BrainPET_Sinograms *dwell_sino){
  if(verbose> 0){
    cout << "BrainPET_Sinograms::apply_dwell_correction()" << endl;
  }
  divide_by_sinogram(dwell_sino);
  return false;
}

//----------------------------------------------------------------------------------------
//--- apply dwell correction to data;
//----------------------------------------------------------------------------------------
bool BrainPET_Sinograms::compute_dwell_sino(BrainPET_Sinograms *dwell_sino, bool use_lookup,
    char* lookup_filename){
  if(verbose> 0){
    cout << "BrainPET_Sinograms::compute_dwell_sino" << endl;
  }

  BrainPET_LORfileData *dwell_lor = new BrainPET_LORfileData(304,864*864,true);
  dwell_lor->set_all_channels(1.0);
  dwell_sino->LOR2Sino(dwell_lor, use_lookup, lookup_filename);
  delete dwell_lor;  
  return false;
}


//----------------------------------------------------------------------------------------
//--- scale every sinogram bin by value (simple multiplication)
//----------------------------------------------------------------------------------------
void BrainPET_Sinograms::scale_by_value(float value){

  if(verbose> 0){
    cout << "scale sinogram by " << value << endl;
  }
  for(int i = 0; i< nViews*nProjs*nPlanes; i++){
    sinogram_flatformat[i]*=value;
  }
  //   for(int i = 0; i< nPlanes; i++){
  //     sinograms[i]->scale_by_value(value);
  //   }
  get_n_Events();

}

//----------------------------------------------------------------------------------------
//--- subtract sinogram from this
//----------------------------------------------------------------------------------------
void BrainPET_Sinograms::subtract_sinogram(BrainPET_Sinograms *sinogram){

  //--- contains geometry information
  if(nViews != sinogram->get_n_views()){
    cout << "BrainPET_Sinograms::subtract_sinogram: Error, wrong # views" << endl;
    exit(0);
  }
  if(nProjs != sinogram->get_n_projs()){
    cout << "BrainPET_Sinograms::subtract_sinogram: Error, wrong # projections" 
      << endl;
    exit(0);
  }
  if(nPlanes != sinogram->get_n_planes()){
    cout << "BrainPET_Sinograms::subtract_sinogram: Error, wrong # planes" << endl;
    exit(0);
  }


  //---add value in flat format
  for(int addr = 0; addr< nViews*nProjs*nPlanes; addr++){
    sinogram_flatformat[addr] -= sinogram->get_event_from_address(addr);
  }

  //--- add sinogram 
  //   for(int plane = 0; plane < nPlanes; plane++){
  //     sinograms[plane]->addSinogramData(sinogram->get_plane(plane));
  //   }

}
//----------------------------------------------------------------------------------------
//--- add sinogram to this
//----------------------------------------------------------------------------------------
void BrainPET_Sinograms::add_sinogram(BrainPET_Sinograms *sinogram){

  //--- contains geometry information
  if(nViews != sinogram->get_n_views()){
    cout << "BrainPET_Sinograms::add_sinogram: Error, wrong # views" << endl;
    exit(0);
  }
  if(nProjs != sinogram->get_n_projs()){
    cout << "BrainPET_Sinograms::add_sinogram: Error, wrong # projections" 
      << endl;
    exit(0);
  }
  if(nPlanes != sinogram->get_n_planes()){
    cout << "BrainPET_Sinograms::add_sinogram: Error, wrong # plenes" << endl;
    exit(0);
  }


  //---add value in flat format
  for(int addr = 0; addr< nViews*nProjs*nPlanes; addr++){
    sinogram_flatformat[addr] += sinogram->get_event_from_address(addr);
  }

  //--- add sinogram 
  //   for(int plane = 0; plane < nPlanes; plane++){
  //     sinograms[plane]->addSinogramData(sinogram->get_plane(plane));
  //   }

}

//----------------------------------------------------------------------------------------
//---divide this sinogram by "sinogram"
//----------------------------------------------------------------------------------------
void BrainPET_Sinograms::divide_by_sinogram(BrainPET_Sinograms *sinogram){
  //--- contains geometry information
  if(nViews != sinogram->get_n_views()){
    cout << "BrainPET_Sinograms::add_sinogram: Error, wrong # views" << endl;
    exit(0);
  }
  if(nProjs != sinogram->get_n_projs()){
    cout << "BrainPET_Sinograms::add_sinogram: Error, wrong # projections" 
      << endl;
    exit(0);
  }
  if(nPlanes != sinogram->get_n_planes()){
    cout << "BrainPET_Sinograms::add_sinogram: Error, wrong # plenes" << endl;
    exit(0);
  }

  for(int i = 0; i < nPlanes*nViews*nProjs; i++){
    if(sinogram->get_event_from_address(i)!=0)
      sinogram_flatformat[i]/=sinogram->get_event_from_address(i);
  }
  //   for(int plane = 0; plane < nPlanes; plane++){
  //     sinograms[plane]->divideBySinogram(sinogram->get_plane(plane));
  //   }
}


//----------------------------------------------------------------------------------------
//---multiply this sinogram element wise by "sinogram"
//----------------------------------------------------------------------------------------
void BrainPET_Sinograms::multiply_by_sinogram(BrainPET_Sinograms *sinogram){
  //--- contains geometry information
  if(this->get_n_views() != sinogram->get_n_views()){
    cout << "BrainPET_Sinograms::add_sinogram: Error, wrong # views" << endl;
    exit(0);
  }
  if(this->get_n_projs() != sinogram->get_n_projs()){
    cout << "BrainPET_Sinograms::add_sinogram: Error, wrong # projections" 
      << endl;
    exit(0);
  }
  if(this->get_n_planes() != sinogram->get_n_planes()){
    cout << "BrainPET_Sinograms::add_sinogram: Error, wrong # plenes" << endl;
    exit(0);
  }

  float fac1;
  float fac2;
  for(int i = 0; i < nPlanes*nViews*nProjs; i++){
    fac1 = sinogram_flatformat[i];
    fac2 = sinogram->get_event_from_address(i);
    if((fac1<0. 001 && fac1> -0. 001) || (fac2<0. 001 && fac2> -0. 001)){
      sinogram_flatformat[i] = 0.0;
    }else{
      sinogram_flatformat[i]*=fac2;
    }
  }
  //   for(int plane = 0; plane < nPlanes; plane++){
  //     sinograms[plane]->multiplyBySinogram(sinogram->get_plane(plane));
  //   }
}



//----------------------------------------------------------------------------------------
//---get pointer to plane
//----------------------------------------------------------------------------------------
R_BrainPET_SinogramData* BrainPET_Sinograms::get_plane(int planeNumber){
  R_BrainPET_SinogramData *dataPtr = sinograms[planeNumber];
  return dataPtr;
}

//----------------------------------------------------------------------------------------
//---get event from flat adress
//----------------------------------------------------------------------------------------
float BrainPET_Sinograms::get_event_from_address(int addr){
  return sinogram_flatformat[addr];
}



//--------------------------------------------------------------------
//---replace value below threshold by replace_value
//--------------------------------------------------------------------
void BrainPET_Sinograms::set_value_below(float threshold, float replace_value){
  if(verbose> 0){
    cout << "replacing all values below " 
      << threshold << " by " << replace_value << endl;
  }
  int counter_out=0;
  int counter_ok=0;
  for(int i = 0; i < nPlanes*nViews*nProjs; i++){
    if(sinogram_flatformat[i]<threshold){
      set_event_at_address(i, replace_value);
      counter_out++;
    }else{
      counter_ok++;
    }
  }
  if(verbose> 0){
    cout << 100.0*(float)counter_out/(float)(counter_out+counter_ok) 
      << " percent of values replaced." << endl;
  }
}


//--------------------------------------------------------------------
//---replace value above threshold by replace_value
//--------------------------------------------------------------------
void BrainPET_Sinograms::set_value_above(float threshold, float replace_value){
  if(verbose> 0){
    cout << "replacing all values above " 
      << threshold << " by " << replace_value << endl;
  }
  int counter_out=0;
  int counter_ok=0;
  for(int i = 0; i < nPlanes*nViews*nProjs; i++){
    if(sinogram_flatformat[i] > threshold){
      set_event_at_address(i, replace_value);
      counter_out++;
    }else{
      counter_ok++;
    }
  }
  if(verbose> 0){
    cout << 100.0*(float)counter_out/(float)(counter_out+counter_ok) 
      << " percent of values replaced." << endl;
  }
}


//----------------------------------------------------------------------------------------
//--- adds an event to sinogram
//----------------------------------------------------------------------------------------
void BrainPET_Sinograms::add_event_to_sinogram(int h1, int b1, int crys1,
    int h2, int b2, int crys2){
  int addr;
  int blk1, blk2;
  int current_view, current_proj, current_plane, current_segment;

  blk1 = h1*6+b1;
  blk2 = h2*6+b2;

  addr=BrainPET1->compute_lor_addr(blk1, crys1, blk2, crys2,
      current_proj, current_view, current_plane,
      current_segment);

  if(current_view < nViews  && current_view>0 &&
      current_proj < nProjs  && current_proj>0 &&
      current_plane< nPlanes && current_plane>0 &&
      addr >=0 && addr < nPlanes*nViews*nProjs){ 
    //sinograms[current_plane]->incrementElement(current_view, current_proj);
    sinogram_flatformat[addr]+=1.0;

  }
}


//----------------------------------------------------------------------------------------
//--- add an event to sinogram by flat adress (float value):
//----------------------------------------------------------------------------------------
void BrainPET_Sinograms::add_event_to_sinogram1(int addr, float value){
  int view, proj, plane;

  if(addr >= 0 && addr < nPlanes*nViews*nProjs){
    BrainPET1->addr_to_view_proj_plane(addr, view, proj, plane);      
    if(plane < nPlanes && view < nViews && proj < nProjs &&
        plane >= 0 && view >= 0 && proj >= 0){
      //sinograms[plane]->addElement(view, proj, value);   
      sinogram_flatformat[addr]+=value;
    }else{
      cout << "addr out of range ";
    }
  }else{
    cout << "ppv out of range "; 
  }

}



//----------------------------------------------------------------------------------------
//--- writes sinogram and corresponging header to disk
//----------------------------------------------------------------------------------------
void BrainPET_Sinograms::write_flat_sinograms(string filename,
    bool writeAsShortInt){


  FileAccess *fa = new FileAccess(filename.c_str());
  fa->open_writeonly(true);
  if(!fa->isWriteable()){
    cout << "ERROR: file IS NOT writeable:" << endl;
    cout << filename << endl;
    exit(0);
  }else{
    cout << "INFO: file IS writeable:" << endl;
    cout << filename << endl;

  }
  write_flat_sinograms(fa, writeAsShortInt);
  fa->close();
  delete fa;
}

//----------------------------------------------------------------------------------------
//--- writes sinogram and corresponging header to disk
//----------------------------------------------------------------------------------------
void BrainPET_Sinograms::write_flat_sinograms(StreamWriter *stream,
    bool writeAsShortInt){

  if(stream->isWriteable()){
    if(writeAsShortInt){
      if(verbose> 0){
        cout << "writing sinograms as SHORT INT" << endl;
      }
      short int *intSino = new short int[nPlanes*nViews*nProjs];
      if(intSino ==NULL){
        cout << "ERROR in BrainPET_Sinograms::write_flat_sinograms" << endl;
        cout << "out of memory" << endl;
        exit(0);
      }

      for(int i = 0; i < nPlanes*nViews*nProjs; i++){
        intSino[i]=(short int)rint(sinogram_flatformat[i]);
      }    
      //write as short int:
      stream->writeBlock((char *)intSino, 
          sizeof(short int)*(nPlanes*nViews*nProjs));
      delete intSino;
    }
    else{
      // write as float
      if(verbose> 0){
        cout << "writing sinograms as FLOAT" << endl;
      }
      stream->writeBlock((char *)sinogram_flatformat, 
          sizeof(float)*(nPlanes*nViews*nProjs));
    }
  }else{
    cout << "ERROR: BrainPET_Sinograms::"
      << "write_flat_sinograms(StreamWriter *stream,bool writeAsShortInt)" << endl;
    cout << "stream ist NOT WRITEABLE!" << endl;
  }
}


//-------------------------------------------------------------------------
//---write sinogram object into stream
//-------------------------------------------------------------------------
void BrainPET_Sinograms::write_into_stream(StreamWriter *stream){
  //--- write object structure:

  if (stream->isWriteable()){
    stream->write(ID_BrainPET_Sinograms);
    stream->write(format_version);

    stream->write(nPlanes);
    stream->write(nViews);
    stream->write(nProjs);     
    stream->write(span);
    stream->write(maxrd);
    stream->write(plane_sep);     
    stream->write(crystal_radius);
    get_n_Events();
    stream->write(nEvents_all);     
    stream->writeBlock((char*)sinogram_flatformat, sizeof(float)*nViews*nProjs*nPlanes);

    stream->write(ID_EndTag);   
    if(verbose> 0){
      cout << "BrainPET_Sinograms: done writing sinograms " << endl;
    }
  }
  else{
    if(verbose> 0){
      cout << "BrainPET_Sinograms::writeIntoStream():   Error: ";
      cout << "DataStream not Writeable!" << endl;
    }
  }
}

//--------------------------------------------------------------
//---read sinogram object from stream
//--------------------------------------------------------------
void BrainPET_Sinograms::load_from_stream(StreamReader *stream){

  sinogram_flatformat = NULL;

  if(stream->isReadable()){
    unsigned short int classID;
    stream->read(classID);
    if(ID_BrainPET_Sinograms==classID){
      stream->read(format_version);      
      stream->read(nPlanes);
      stream->read(nViews);
      stream->read(nProjs); 
      stream->read(span);
      stream->read(maxrd);
      stream->read(plane_sep);     
      stream->read(crystal_radius);
      stream->read(nEvents_all);   
      sinogram_flatformat = new float[nViews*nProjs*nPlanes];
      if(sinogram_flatformat ==NULL){
        cout << "ERROR in BrainPET_Sinograms::load_from_stream" << endl;
        cout << "out of memory" << endl;
        exit(0);
      }
      stream->readBlock((char*)sinogram_flatformat, sizeof(float)*nViews*nProjs*nPlanes);

      sinograms = new R_BrainPET_SinogramData *[nPlanes];

      for(int i = 0; i < nPlanes; i++){
        float *sino_plane = sinogram_flatformat + (i*nViews*nProjs);
        sinograms[i]  = new R_BrainPET_SinogramData(BrainPET1, nViews, nProjs, i, sino_plane);
        if(sinograms[i] ==NULL){
          cout << "ERROR in BrainPET_Sinograms::write_flat_sinograms" << endl;
          cout << "out of memory" << endl;
          exit(0);
        }
      }  

      if(verbose> 0){
        cout << "done reading sinogram data"<< endl;
      }
      unsigned short int endtag;
      stream->read(endtag);

      if (ID_EndTag!=endtag){
        cout << "BrainPET_Sinograms():  Error: " << endl;
        cout << "End Tag not identified correctly: " 
          << ID_EndTag << " != " << endtag << endl;
        exit(0);
      }
      if(verbose> 0){
        cout << "BrainPET_Sinograms(StreamReader *stream) done reading sinograms " << endl;
      }
    }else{
      cout << "BrainPET_Sinograms():   Error: ";
      cout << "Wrong classID" << endl;
      exit(0);
    }
  }
  else{
    cout << "BrainPET_Sinograms():   Error: ";
    cout << "DataStream not Readable!" << endl;
  }
  if(verbose> 0){
    cout <<"INFO: total number of events in sinogram:" << get_n_Events() << endl;
  }
}

//----------------------------------------------------------------------------------------
//--- fill sinogram homogeniously with value
//----------------------------------------------------------------------------------------
void  BrainPET_Sinograms::fill_sinogram_homogeniously(float value){
  if(verbose> 0){
    cout << "BrainPET_Sinograms::fill_sinogram_homogeniously(" << value << ")" << endl;
  }
  int nbins = nPlanes*nViews*nProjs;
  resetAllSinograms();
  for(int i = 0; i < nbins; i++){
    sinogram_flatformat[i]=value;
  }
}


//----------------------------------------------------------------------------------------
//--- reads sinogram by filename
//----------------------------------------------------------------------------------------
void BrainPET_Sinograms::read_flatSinogram(string filename, 
    bool isShortInt){

  FileAccess *fa = new FileAccess(filename);
  cout << filename << endl;
  fa->open_readonly(true);
  if(!fa->isReadable()){
    cout << "ERROR in BrainPET_Sinograms::read_flatSinogram" << endl;
    cout << filename << " is NOT READABLE!" << endl;
    exit(0);
  }else{
    read_flatSinogram_from_stream(fa, isShortInt);
  }
  fa->close();
  delete fa;  
}

//----------------------------------------------------------------------------------------
//--- reads sinogram from file system
//----------------------------------------------------------------------------------------
void BrainPET_Sinograms::read_flatSinogram_from_stream(StreamReader *r_stream, 
    bool isShortInt){


  //---verify file size
  long long int size=r_stream->size();
  if(isShortInt){
    if(verbose> 0){
      cout << "BrainPET_Sinograms::read_flatSinogram() SHORT INT expected" << endl;
    }
    long long int size0=nPlanes*nViews*nProjs*sizeof(short int);      
    if (size0!=size){
      cout << "BrainPET_Sinograms::read_flatSinogram_from_stream(): Error: ";
      cout << "Unexpecetd file size for float data!" << endl;
      printf("size=%llu     expected size=%llu\n",size,size0);
      exit(0);
    }
  }else{
    cout <<"BrainPET_Sinograms::read_flatSinogram_from_stream() FLOATS expected" << endl;
    long long int size0=nPlanes*nViews*nProjs*sizeof(float);      
    if (size0!=size){
      cout << "BrainPET_Sinograms::read_flatSinogram_from_stream(): Error: ";
      cout << "Unexpecetd file size for short int data!" << endl;
      printf("size=%llu     expected size=%llu\n",size,size0);
      exit(0);
    }
  }

  resetAllSinograms();
  if(isShortInt){
    if(verbose> 0){
      cout << "reading flat sinograms as (SHORT INT)" << endl;
    }
    short int *intSino = new short int[nPlanes*nViews*nProjs];

    r_stream->readBlock((char *)intSino, sizeof(short int)*(nPlanes*nViews*nProjs));

    for(int i = 0; i < nPlanes*nViews*nProjs; i++){
      sinogram_flatformat[i] = (float)intSino[i];
    }    
    delete intSino;

  }else{
    if(verbose> 0){
      cout << "reading flat sinograms as (FLOAT)" << endl;
    }
    r_stream->readBlock((char *)sinogram_flatformat, 
        sizeof(float)*(nPlanes*nViews*nProjs));
  }

  //--- convert data from flat format to object structure:
  //   int view, proj, plane;
  //   float val;
  //   for(int addr = 0; addr < (nPlanes*nViews*nProjs); addr++){
  //     val = sinogram_flatformat[addr];
  //     BrainPET1->addr_to_view_proj_plane(addr, view, proj, plane);
  //     sinograms[plane]->addElement(view, proj, val);
  //   } 
  get_n_Events();

}



//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
void BrainPET_Sinograms::resetAllSinograms(){
  //   for(int i = 0; i < nPlanes; i++){
  //     sinograms[i]->resetSinogram();
  //   }
  for(int i = 0; i< nViews*nProjs*nPlanes; i++){
    sinogram_flatformat[i]= 0.0;
  }
  nEvents_all=0;
  if(verbose> 0){
    cout << "BrainPET_Sinograms::resetAllSinograms()" << endl;
  }
}

//----------------------------------------------------------------------------------------
//---set all bins with value (shows no dwell!)
//----------------------------------------------------------------------------------------
void BrainPET_Sinograms::setAllSinogramBins(float value){

  //   for(int i = 0; i < nPlanes; i++){
  //     sinograms[i]->setAllSinogramBins(value);
  //   }
  for(int i = 0; i< nViews*nProjs*nPlanes; i++){
    sinogram_flatformat[i]= value;
  }
  if(verbose> 0){
    cout << "BrainPET_Sinograms::setAllSinogramBins(float value)" << endl;
  }
}


//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
int BrainPET_Sinograms::get_n_planes(){
  return nPlanes;
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
int BrainPET_Sinograms::get_n_views(){
  return nViews; 
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
int BrainPET_Sinograms::get_n_projs(){
  return nProjs;
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
double BrainPET_Sinograms::get_n_Events(){
  nEvents_all = 0.0;
  for(int i=0; i < nPlanes*nViews*nProjs; i++){
    nEvents_all+=sinogram_flatformat[i];
  }
  double nEvents2 = 0.0;
  for(int i = 0; i < nPlanes; i++){
    nEvents2+= sinograms[i]->get_n_Events();
  }

  if(nEvents_all - nEvents2 > 0.5){
    cout << "Caution!!! Inconsistent Sinogram data!" << endl;
    cout << nEvents_all << " != " << nEvents2 << " !!!!" << endl;
    cout << "Difference: " << nEvents_all - nEvents2 << endl;
  }
  return nEvents_all;
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
void BrainPET_Sinograms::showSinogram(int plane){
  if(plane < 0 || plane >= nPlanes-1){
    cout << "ERROR: BrainPET_Sinograms::showSinogram():"
      << "plane number out of range!" << endl;
  }else{
    char label[512];
    sprintf(label, "%s_plane_%i_segment_%i_z_%i",title.c_str(), plane,
        sinograms[plane]->getSegment(),
        sinograms[plane]->getAxialPosition());

    sinograms[plane]->r_showSinogram(label);
    sinograms[plane]->showDataMembers();
  }
}


//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
void BrainPET_Sinograms::showSinogram(int segment, int z){

  int plane = BrainPET1->seg_z_to_plane_number(segment, z);

  if(plane < 0 || plane >= nPlanes-1){
    cout << "ERROR: BrainPET_Sinograms::showSinogram():"
      << "segment or z selection number out of range!" << endl;
  }else{
    char label[512];
    sprintf(label, "%s_plane_%i_segment_%i_z_%i",title.c_str(), plane,
        sinograms[plane]->getSegment(),
        sinograms[plane]->getAxialPosition());

    sinograms[plane]->r_showSinogram(label);
    sinograms[plane]->showDataMembers();
  }
}

//----------------------------------------------------------------------------------------
//---get min/max values
//----------------------------------------------------------------------------------------
void BrainPET_Sinograms::get_min_max_values(double &min, double &max)
{
  min=+1E30;
  max=-1E30;
  double dat;
  for (int i=0; i<nPlanes*nViews*nProjs; i++)
  {
    dat=sinogram_flatformat[i];
    if (min>dat) 
      min=dat;
    if (max<dat) 
      max=dat;
  }
  cout << "BrainPET_Sinograms::get_min_max_values:  Info: ";
  cout << "min=" << min << " max=" << max << endl;
}


//----------------------------------------------------------------------------------------
//---show frequency of values
//----------------------------------------------------------------------------------------
void BrainPET_Sinograms::show_frequency_values()
{
  if(verbose> 0){
    cout << "BrainPET_Sinograms::show_frequency_values()" << endl;
  }
  double min,max;
  get_min_max_values(min,max);
  TH1D* hist=new TH1D("ValueFrequency","ValueFrequency",1000,min,max);
  hist->SetLineColor(1);

  for (int i=0; i<nPlanes*nViews*nProjs; i++)
  {
    hist->Fill(sinogram_flatformat[i]);
  }

  TCanvas* c=new TCanvas("BrainPET_Sinograms","BrainPET_Sinograms");
  c->Divide(1,1);
  hist->GetXaxis()->SetTitle("bin value");
  hist->GetYaxis()->SetTitle("frequency");
  hist->Draw();
}

//----------------------------------------------------------------------------------------
//---show frequency of values
//----------------------------------------------------------------------------------------
void BrainPET_Sinograms::show_plane_sums()
{
  if(verbose> 0){
    cout << "BrainPET_Sinograms::show_plane_sums()" << endl;
  }
  double max = 0;
  double val = 0;
  for(int i = 0; i < nPlanes; i++){
    val= sinograms[i]->get_n_Events();
    if(val>max){
      max = val;
    }
    //cout << "plane " << i<< " has " << val << " events" << endl;
  }
  TH1D* histo=new TH1D("PlaneSums","PlaneSums", nPlanes, 0, nPlanes-1);
  histo->SetLineColor(1);

  for (int i=0; i<nPlanes; i++){
    histo->Fill(i+1,sinograms[i]->get_n_Events());
  } 
  TCanvas* c1=new TCanvas("PlaneSum","PlaneSum");
  c1->Divide(1,1);
  histo->GetXaxis()->SetTitle("plane");
  histo->GetYaxis()->SetTitle("number of events");
  histo->Draw();


}


//----------------------------------------------------------------------------------------
//---get float float data space handle
//----------------------------------------------------------------------------------------
float* BrainPET_Sinograms::get_handle_flat_float(int &nfloats)
{
  nfloats=nViews*nProjs*nPlanes;
  return sinogram_flatformat;
}



//----------------------------------------------------------------------------------------
//---Destructor
//----------------------------------------------------------------------------------------
BrainPET_Sinograms::~BrainPET_Sinograms(){
  if(verbose> 0){
    cout << "BrainPET_Sinograms::~BrainPET_Sinograms()";
  }
  for(int i = 0; i < nPlanes; i++){
    if(NULL != sinograms[i]){
      delete sinograms[i];
      sinograms[i] = NULL;
    }
  }
  delete[] sinogram_flatformat;
  cout << "...done" << endl;
}
//Generated on Tue Jun 18 17:17:16 2013 for *** PRESTO *** Pet Reconstruction Software TOolbox *** revision 333 *** by   doxygen 1.5.6
