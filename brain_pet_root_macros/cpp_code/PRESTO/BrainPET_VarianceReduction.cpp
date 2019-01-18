
 #include "BrainPET_VarianceReduction.hh"
 
 //------------------------------------------------------------------
 //---Constructor
 //------------------------------------------------------------------
 BrainPET_VarianceReduction::BrainPET_VarianceReduction(int cwin_x){
 
   cout << "BrainPET_VarianceReduction::BrainPET_VarianceReduction(" 
        << cwin_x << ");" <<endl;
 
 
   //---definition of basic parameters:
   n_heads=BrainPET_number_of_modules;
   fov=BrainPET_n_FOV;
   cwin=cwin_x;
   tau=6.0e-9;
   n_hpairs=BrainPET_nHeadPairs;
   verbose=0;
   
   //---allocation of memory:
   hpairs=new int[n_hpairs*2];
   for(int i = 0; i < n_hpairs*2; i++){
     hpairs[i]= 0;
   }
   hpmap=new int[n_heads*n_heads];
   for(int i = 0; i < n_heads*n_heads; i++){
     hpmap[i]= 0;
   }
   hp_list= new int[BrainPET_nHeadPairs];
   for(int i = 0; i < BrainPET_nHeadPairs; i++){
     hp_list[i]= 0;
   }
   srates = new float[n_crystals_per_head*BrainPET_number_of_modules];
   destimate=new float[n_crystals_per_head*BrainPET_number_of_modules];
   for(int i = 0; i < n_crystals_per_head*BrainPET_number_of_modules; i++){
     srates[i]=0.0;
     destimate[i]=0.0;
   }
   hrates = new float[BrainPET_number_of_modules];
   off_rates = new float[BrainPET_number_of_modules];
   for(int i = 0; i < BrainPET_number_of_modules; i++){
     hrates[i] = 0.0;
     off_rates[i] = 0.0;
   }
 
   //---initialisation of all arrays with zero:
   for(int i = 0; i < n_hpairs*2; i++){
     hpairs[i]=0;
   }
   for(int i = 0; i < n_heads*n_heads; i++){
     hpmap[i]=0;
   }
   for(int i = 0; i < n_crystals_per_head*BrainPET_number_of_modules; i++){
     srates[i]=0.0;
     destimate[i]=0.0;
   }
   for(int i = 0; i < BrainPET_nHeadPairs; i++){
     hp_list[i]=0;
   }
   for(int i = 0; i < BrainPET_number_of_modules; i++){
    hrates[i]=0.0;
    off_rates[i]=0.0;
   }
 
   
   init_hpair_data(fov);
   
   df=estimate_delay_factor(cwin, 128);
   cout << "df: " << df << endl;
 
 }
 
 
 //------------------------------------------------------------------
 //---Destructor
 //------------------------------------------------------------------
 BrainPET_VarianceReduction::~BrainPET_VarianceReduction(){
 
   delete[] hpairs;
   delete[] hpmap;
   delete[] srates;
   delete[] hp_list;
   delete[] destimate;
   delete[] hrates;
   delete[] off_rates;
 
   cout << "BrainPET_VarianceReduction::~BrainPET_VarianceReduction()" << endl;
 }
 
 
 //------------------------------------------------------------------
 // initialise head pairs (also: module pairs)
 // lookuptables
 //------------------------------------------------------------------
 void BrainPET_VarianceReduction::init_hpair_data(int fov)
 {
   cout << "BrainPET_VarianceReduction::init_hpair_data("
        << fov << ");" << endl;
 
     int a, b, bh, i;
     int h, hp, oh, hpoff;
 
     hpoff=(n_heads-fov+1)/2;
     for (h=0,hp=0; h<n_heads; h++) {
       for (oh=0; oh<fov; oh++) {
           bh=h+hpoff+oh;
           if (bh > n_heads-1) 
               continue;
           hpairs[2*hp]=h;
           hpairs[2*hp+1]=bh;
           hp++;
       }
     }
     
     for (i=0; i<n_hpairs; i++) {
       a=hpairs[2*i];
       b=hpairs[2*i+1];
       hpmap[n_heads*a+b]=1;
       hpmap[n_heads*b+a]=1;
     }
 }
 
 
 //------------------------------------------------------------------
 //---calculate single rates
 //---iterative algorithm
 //---should converge in 100 iterations
 //------------------------------------------------------------------
 void BrainPET_VarianceReduction::calc_srates(char *dcmap_filename){
 
 
   //int ncrystals0 = 32*6*144;
   FileAccess *fa = new FileAccess(dcmap_filename);
   if (fa->exists()){
     if ((fa->size()!=2) && (fa->size()!=2*2)){
       cout << ":  Error: ";
       cout << "Unexpected file size!" << endl;
       exit(0);
     }
 
     bool suppress_start_tag=true;
     fa->open_readonly(suppress_start_tag);
     
     if(!(fa->isReadable())){
       cout << dcmap_filename
              << " is NOT READABLE!" << endl;
       exit(0);
     }
     int ncrystals0=144*32*6;
     int   *map  =new int[ncrystals0];
     float *dcmap=new float[ncrystals0];
 
     fa->readBlock((char*)map,sizeof(int)*ncrystals0);
           
     float sum=0.0;
     for (int i=0; i<ncrystals0; i++){
       dcmap[i]=float(map[i]);
       sum+=dcmap[i];
     }
 
     cout << "BrainPET_VarianceReduction::calc_srates:  TotCounts in dcmap: " << sum << endl;   
 
     calc_srates_kernel(dcmap);
 
     delete[] dcmap;
     delete[] map;
   
   }else{
     cout << "BrainPET_DetHitMap::load_Siemens_hitmap:  Error: ";
     cout << "file not found: " << dcmap_filename << endl;
     exit(0);
   }
   fa->close();
  
 
 }
 
 
 
 
 
 //------------------------------------------------------------------
 //---interface for BrainPET_DetHitMap
 //------------------------------------------------------------------
 void BrainPET_VarianceReduction::calc_srates(BrainPET_DetHitMap *dcmap0){
   int ncrystals0 = 32*6*144;
   float *dcmap = new float[ncrystals0];
   for (int i=0; i<ncrystals0; i++){
     dcmap[i]=dcmap0->get_hit_map_value(i);
   }
   calc_srates_kernel(dcmap);
   delete[] dcmap;
 }
 
 //------------------------------------------------------------------
 //---calculate single rates
 //---iterative algorithm
 //---should converge in 100 iterations
 //------------------------------------------------------------------
 void BrainPET_VarianceReduction::calc_srates_kernel(float *dcmap){
 
   cout << "BrainPET_VarianceReduction::calc_srates(BrainPET_DetHitMap *dcmap0)"
        << endl;
   
   double mean = 0.0;
   double savg;
   int i, j, iter, nlors;
   double error, err;
   int ncrystals0=144*32*6;
   
   double sum=0.0;
   double sum2 = 0.0;
   for (i=0; i<ncrystals0; i++){
       
       sum+=dcmap[i];
     }
   cout << "BrainPET_VarianceReduction::calc_srates:  TotCounts in dcmap: " << sum << endl;
 
     
   mean=0.0;
   for(i=0; i<864*32; i++){
     mean+=dcmap[i];
   }
   mean=mean/(864*32);
   nlors=fov*864*32;
   savg=sqrt(mean/(nlors*tau));
   for (i=0; i<864*32; i++){ 
     srates[i]=savg*dcmap[i]/mean;
   }
     //
     // we have the initial estimate of the singles rates based on mean of the DCMap
     // and the total coincidence pairs;
     //
     for (iter=0; iter<100; iter++) {
       for (i=0; i<32; i++) {
         hrates[i]=0;
         for (j=0; j<864; j++){ 
             hrates[i] += srates[864*i+j];
           }
       }
       for(i=0; i<32; i++){
         off_rates[i]=0.0;
         for (j=0; j<32; j++)
           if (hpmap[32*i+j]) 
               off_rates[i] += hrates[j];
       }
       for(i=0; i<32; i++) 
           for (j=0; j<864; j++){
             destimate[864*i+j]=(0.5+tau*srates[864*i+j]*off_rates[i]);
           }
       error=0;
       for(i=0; i<864*32; i++) {
         err=(double)destimate[i]-(double)(dcmap[i]);
           //sum2 += (double)destimate[i]*(double)destimate[i];
         error += err*err;
       }
       
       if (error < 1e-5){
           cout << "Info: Singles rates did converge in "<< iter << " iterations!" << endl;
           cout << " - Residual error: " << error << " (" << error/sum2  << " % )" << endl;
           break;
       }
       for (i=0; i<32; i++) 
           for (j=0; j<864; j++){
             srates[864*i+j]=(srates[864*i+j]+dcmap[864*i+j]/(tau*off_rates[i]))/2.0;
           }
     }
     if (iter > 99){
       cout << "Warning! Singles rates did not converge in 100 iterations!" << endl;
       cout << " - Residual error: " << error << endl;
     }
     double srates_sum=0.0;
     for (i=0; i<864*32; i++){
       srates_sum += srates[i];
     }
     cout << "BrainPET_VarianceReduction::calc_srates: TotCounts in srates: " << srates_sum << endl;
 
 
 
 
 }
 
 
 
 
 
 
 //------------------------------------------------------------------
 
 //------------------------------------------------------------------
 float BrainPET_VarianceReduction::estimate_delay_factor(int cwin, int offset)
 {
   
   cout << "BrainPET_VarianceReduction::estimate_delay_factor(int cwin, int offset)"
        << endl;
   
   int w=256;
   int i=0;
   int t1=0;
   int t2=0;
   int *ramp =new int[2*w];
  
   float f;
 
   
   for (i=0; i<w; i++) 
     ramp[i]=i;
   for (i=0; i<w; i++) 
     ramp[w+i]=w-i;
   t1=0;
   for (i=w-cwin; i<= w+cwin; i++) 
     t1 += ramp[i];
   t2=0;
   for (i=w-cwin-offset; i<=w-offset+cwin; i++) 
     t2 += ramp[i];
   f=1.0*t1/(2.0*t2);
   delete[] ramp;
   
   return(f);
 }
 
 
 //------------------------------------------------------------------
 //---calculate VR-LORdata from crystal hitmap
 //------------------------------------------------------------------
 int BrainPET_VarianceReduction::variance_reduction(BrainPET_DetHitMap *dcmap, BrainPET_LORfileData *lor_data)
 {
     
   if(lor_data==NULL){ 
     cout << "BrainPET_LORfileData *lor_data not specified!" << endl;
   }
   if(dcmap==NULL){
     cout << "BrainPET_DetHitMap *dcmap not specified!" << endl;
   }
 
   calc_srates(dcmap);
   variance_reduction_kernel(lor_data);
   
   return 0;
 
 }
 
 
 
 
 //------------------------------------------------------------------
 //---calculate VR-LORdata from crystal hitmap
 //------------------------------------------------------------------
 int BrainPET_VarianceReduction::variance_reduction(char *dcmap_filename, BrainPET_LORfileData *lor_data)
 {
     
   if(lor_data==NULL){ 
     cout << "BrainPET_LORfileData *lor_data not specified!" << endl;
   }
   if(dcmap_filename==NULL){
     cout << "char *dcmap not specified!" << endl;
   }
 
   calc_srates(dcmap_filename);
   variance_reduction_kernel(lor_data);
 
   return 0;
   
 }
 
 
 //------------------------------------------------------------------
 //---calculate VR-LORdata from crystal hitmap
 //------------------------------------------------------------------
 int BrainPET_VarianceReduction::variance_reduction_kernel(BrainPET_LORfileData *lor_data){
 
   int a,b;
   int hp;
   //int c;
   float value;
   double val_sum=0;
   int index1=0;
   int index2=0;
   int error_counter = 0;
 
   for(hp=0; hp<n_hpairs; hp++){
     for(a=0; a<864; a++){
       for(b=0; b<864; b++){
           index1 = 864*hpairs[2*hp]+a;
           index2 = 864*hpairs[2*hp+1]+b;
           if(index1 >= 864*32 || index2 >= 864*32 ||index1 < 0 || index2 < 0){
             error_counter++;
           }else{
             value = df*tau*srates[index1]*srates[index2];
             val_sum+=value;
             lor_data->set_lor_data(864*864*hp+864*a+b, value);
           }
       }
     }
   }
   cout << "Sum of values in LOR file " << val_sum << endl;
   cout << "Number of bad array accesses: " <<  error_counter << endl;
   cout << "... done: variance_reduction(BrainPET_DetHitMap *dcmap, BrainPET_LORfileData *lor_data)" << endl;
 
 
   return 0;
   
 }
 
//Generated on Tue Jun 18 17:17:16 2013 for *** PRESTO *** Pet Reconstruction Software TOolbox *** revision 333 *** by  ￼ 1.5.6
