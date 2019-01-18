/*
 *    some basic operations of sinogram data 
 *
 *
 */

void presto_basic_sino_operation()
{  //---Counts of Sino Data


  //------load sinograms----------
  //---------1: ----------------
 MRPET_dump *kludge = new MRPET_dump();

  //---------2: ----------------
 BrainPET_Sinograms *sino_ra = new BrainPET_Sinograms(kludge, "randoms_sinogram_phantom");

  //---------3: ----------------
 sino_ra->read_flatSinogram("sinogram_16_randoms_frames_with_phantom.fs",false);

  //---------4: ----------------
 double counts=sino_ra->get_n_Events();



 cout <<"number of random counts with phantom:"<< counts << endl;
 cout << "                               "<< endl;


  MRPET_dump *kludge = new MRPET_dump();
 BrainPET_Sinograms *sino_norm_nc = new BrainPET_Sinograms(kludge, "NORMnocoil");
 sino_norm_nc->read_flatSinogram("sinogram_16_prompt_frames_nc.fs",false);

  //---------1: ----------------
 sino_norm_nc->setAllSinogramBins(0.0);

 double counts=sino_norm_nc->get_n_Events();
 cout <<"number of counts of empty sino:"<< counts << endl;
 cout << "                               "<< endl;


//----------find number of planes, views and projections-------------

//sino_pr_blank->write_flat_sinograms("trans_scan_div_wra.fs",false);

  //---------1: ----------------
 int planes=sino_pr->get_n_planes();
  //---------1: ----------------
 int views=sino_pr->get_n_views();
  //---------1: ----------------
 int projs= sino_pr->get_n_projs();

 cout <<"planes with coil:"<< planes <<"         views with coil: "<< views <<"            projections with coil "<< projs<< endl; 







 //-------subtract and supress sinograms with check----------also creating the NORM file-------------



 double tr_min;
 double tr_max;
 double tr_min2;
 double tr_max2;

  //---------1: ----------------
 sino_pr->subtract_sinogram(sino_ra);

  //---------5: ----------------
 sino_pr->get_min_max_values (tr_min, tr_max);
 cout <<"minimum of trues with phantom:"<< tr_min << endl;
 cout <<"maximum of trues with phantom:"<< tr_max << endl;
 cout<< "                           " << endl;


 
 int zeros=0;
 int goods=0;
 for(int addressnum=1; addressnum<planes*views*projs; addressnum++)
   {
     float valuebin_wc=sino_pr->get_event_from_address(addressnum);
     
     if (valuebin_wc<=0)
       {zeros++;
  //---------1: ----------------
	 sino_norm_wc->set_event_at_address(addressnum,1e+20);
	 sino_norm_total->set_event_at_address(addressnum,1e+20);
       }
     else
       {goods++;}
   };
 
/*
for(int i=1; i<=planes; i++)
     {
       sino_add->addSinogramData(sino_norm_wc);
     };

 break;
*/

 float percent=float(zeros)/float(goods)*100;
 cout<<"  zeros trues with phantom:"<<zeros<<endl;
 cout<<"  good trues with phantom:"<<goods<<endl;
 cout<<"  percent of zeros with phantom:"<<percent<<"%"<<endl;    
 //sino_norm_wc->write_flat_sinograms("NORMzeroswithphantom.fs",false);
 sino_norm_wc->showSinogram(2);

 sino_pr->set_value_below (0.00, 0.00);
  //---------1: ----------------
 sino_pr->get_min_max_values (tr_min, tr_max);
 cout <<"minimum of trues with phantom after suppresion:"<< tr_min << endl;
 cout <<"maximum of trues with phantom after suppresion:"<< tr_max << endl;
 cout<< "                           " << endl;
 

   

//------check for good bad and zeros------------


int goodt=0;
int noatt=0;
int badt=0;
int zeros=0;
for(int addressnum=1; addressnum<planes*views*projs; addressnum++)
{
  //---------1: ----------------
float valuebin=sino_pr_blank->get_event_from_address(addressnum);
float valuebinp=sino_pr->get_event_from_address(addressnum);

if (valuebin>valuebinp)
{goodt++;}
else
{
	if (valuebin==valuebinp)
	{

if(valuebin==0)
{zeros++;
//norm->set_event_at_address(addressnum,1e+20);
}
else
{noatt++;};

}
	else{badt++;
//	norm->set_event_at_address(addressnum,1e+20);
};
};


};


cout <<"GOOD: " <<goodt << endl;
cout <<"BAD: " <<badt << endl;
cout <<"NO: " <<noatt << endl;
cout <<"ZEROS: "<<zeros<<endl;

 



//-------------- Apply Divsion------create NORM sino --------------------
  //---------1: ----------------
sino_pr_blank->divide_by_sinogram(sino_pr);

int zeros=0;
int goods=0;
 for(int addressnum=1; addressnum<planes_nc*views_nc*projs_nc; addressnum++)
   {
     float valuebin_division=sino_pr_blank->get_event_from_address(addressnum);
     
     if (valuebin_division<1)
       {zeros++;
	 sino_norm_division->set_event_at_address(addressnum,1e+20);
	 sino_norm_total->set_event_at_address(addressnum,1e+20); 
       }
 else
   {goods++;}
   };
 float percent=float(zeros)/float(goods)*100;
 cout<<"  trues divides under one:"<<zeros<<endl;
 cout<<"  good trues divided:"<<goods<<endl;  
 cout<<"  percent of trues divided under one:"<<percent<<"%"<<endl;   
 sino_norm_division->write_flat_sinograms("NORMbaddivisionphantom.fs",false);
 sino_norm_division->showSinogram(2);


  //---------6: ----------------
 sino_norm_total->write_flat_sinograms("NORMtotalcountsphantom.fs",false);
  //---------1: ----------------
 sino_norm_total->showSinogram(2);


//---------Set values below or equal to 1 to 1--------------

 sino_pr_blank->set_value_below (1.00, 1.00);


//----Check values under 1 for lag!----apply the logarithm

for(int addressnum=1; addressnum<planes*views*projs; addressnum++)
{
float valuebin=sino_pr_blank->get_event_from_address(addressnum);

if (valuebin>1)
{float valuebinnew=TMath::Log(valuebin);
}
else
{float valuebinnew=0;};

sino_pr_blank->set_event_at_address(addressnum,valuebinnew);

};
sino_pr_blank->get_min_max_values (tr_min, tr_max);
 cout <<"minimum after division and logarithm:"<< tr_min << endl;
 cout <<"maximum after division and logarithm:"<< tr_max << endl;

 

sino_pr_blank->write_flat_sinograms("sum_16_frames_logarithmiert_phantom.fs",false);
sino_pr_blank->showSinogram(2);





}
