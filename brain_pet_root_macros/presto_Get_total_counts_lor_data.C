/*
 *  Get the total counts of the lor data;
 *
 *  Input: 1: the path of the lor data;
 *        2: the data format of the data: is float or not
 *
 *
 */
void Get_total_counts_LOR_data()
{  //---create LOR data

  BrainPET_RingData* rdata;
  rdata=new BrainPET_RingData;

  // nblocks is the n_mpairs: the number of modual pairs:19*16=304;
  // nblock_size is the mpsize: i.g: modual pair size: (12*12*6)*(12*12*6)
  int nblocks, nblock_size;

  //void BrainPETbasics_ABC::get_sequential_data_sizes	(	int & 	nblocks,int & 	nblock_size	)
  rdata->get_sequential_data_sizes(nblocks, nblock_size);
 
  bool floats=true;

  //create LOR data
  //BrainPET_LORfileData::BrainPET_LORfileData	(	int	n_modul_pairs0, int	n_modulpair_size0,bool	data_float0 = true)	
  BrainPET_LORfileData* lordata=new BrainPET_LORfileData(nblocks, nblock_size,floats);

  lordata->load_data("/data/BrainPET_Sys/global/lcaldeira/sim/data/simtestUtah_nosc_rs005_stat225E7_prompts_detected.flor",floats);

  BrainPET_LORfileData* lordata2=new BrainPET_LORfileData(nblocks, nblock_size,floats);

  lordata2->load_data("/data/BrainPET_Sys/global/lcaldeira/sim/data/simtestUtah_nosc_rs005_stat225E7_randoms_detected_noisy.flor",floats);

  BrainPET_LORfileData* lordata3=new BrainPET_LORfileData(nblocks, nblock_size,floats);

  lordata3->load_data("/data/BrainPET_Sys/global/lcaldeira/sim/data/simtestUtah_nosc_rs005_stat225E7_randoms_detected.flor",floats);

  lordata->subtract_data_from(lordata3);
  cout <<"After SUbtraction"<<lordata->get_count_sum()<< endl;

  lordata->supress_negative_values();
  cout <<"After Supression" <<lordata->get_count_sum()<< endl;

  //cout <<"Before LORdata3" <<lordata3->get_count_sum()<< endl;
  //lordata3->supress_negative_values();
  //cout << lordata3->get_count_sum()<< endl;


}
