
 #include "MRPET_dump.hh"
 
 
 //---------------------------------------------------------------------------
 //---Constructor I
 //---------------------------------------------------------------------------
 MRPET_dump::MRPET_dump(int nViews, int nProjs, 
                            int maxRingdiff_x, int span_x,
                            float crystalRadius, int verbose_x){
   
 
   verbose= verbose_x;
 
 
   if(verbose>0){
     cout << "MRPET_dump()"<< endl;
   }
   
   //--- initialise basic parameters:    
   nviews = nViews;
   nprojs = nProjs;
   maxRingdiff = maxRingdiff_x;
   span = span_x;
   nplanes = 0;
   ncrystals = 0; 
   binsize =0.125;
   n_mpairs = 304;
   mpair_a = NULL;
   mpair_b = NULL;
   mod_fov = 19;
   maxseg = 0;
   nsegs = 0;
   nrings = 77;
   d_tan_theta = 0.0;
   plane_sep = 0.125;// scale to # of bin-elements??
 
   bad_hp =0;
 
   if(verbose>0){
     cout << "n views: "<< nviews << " n projections: " << nprojs << endl;
   }
   rot = NULL;
   vec = NULL; 
   pos = NULL;
   vec_data = NULL;
   pos_data  = NULL;
   segz0 = NULL;
   segnz = NULL;
   segzoff = NULL;
   segnums = NULL;
   segment_struct=NULL;
   slut = NULL;
   blut = NULL;
  
   //lookuptable for sinogram adressing
   segments_lut=NULL;
   z_val_lut=NULL;
   min_angle_lut  =NULL;
   mean_angle_lut =NULL;
   max_angle_lut  =NULL;
 
 //   radiusHistogram = NULL;
 //   radiusHistogram = new int[nprojs];
 //   for(int i = 0; i< nprojs; i++){
 //     radiusHistogram[i]=0;
 //   }
 
   //--- from BrainPET_BasicProperties:
 
   n_mods = BrainPET_number_of_modules;         // number of heads
   nheads = BrainPET_number_of_modules; 
   mod_nx = BrainPET_crystals_transaxial_per_block; // transaxial
   mod_ny = BrainPET_rings_without_gaps;       // axial (scanners z direction)
   mod_nrings =Nblocks_per_head_EPM;
 
   ncrystals_per_ring=n_mods*mod_nx;
   ncrystals=ncrystals_per_ring*mod_ny;
 
   crystal_radius = crystalRadius;
   crystalPositions = new point3D_typ[ncrystals];
 
   // compute crystal positions:
   int ncrys_x = compute_crystal_positions();
   if(ncrys_x != ncrystals){
     cout << "ERROR: number of crystals not well specified!" << endl;
     exit(0);
   }
   // initialise segment information:
   init_seginfo();
   init_mpair_data();
 
   //---allocate memory for arrays
   slut = NULL;
   blut = NULL;
   slut = new int[N_EPM*Ngroup_EPM];
   blut = new int[Nhead_EPM* Nblocks_per_head_EPM];
 
     //--- memory for lookup tables
   hpair=NULL;
   hplookup=NULL;
   hpair = new short int[Nhead_EPM*(Nhead_EPM-1)];
   hplookup =new short int[Nhead_EPM*Nhead_EPM];
   make_slut();
   init_lor_info();
 
   if(verbose>0){
     cout << "MRPET_dump::MRPET_dump();" << endl;
   }
 
 } 
 
 //---------------------------------------------------------------------------
 //---Destructor
 //---------------------------------------------------------------------------
 MRPET_dump::~MRPET_dump(){
   delete[] crystalPositions;
   delete[] segz0;
   delete[] segnz;
   delete[] segzoff;
   delete[] segnums;
   delete segment_struct;
   delete[] slut;
   delete[] blut;
   segz0 = NULL;
   segnz = NULL;
   segzoff = NULL;
   segnums = NULL;
   segment_struct = NULL;
   slut = NULL;
   blut = NULL;
 
   if(verbose>0){
     cout << "MRPET_dump::~MRPET_dump()" << endl;
   }
 }
 
 
 //---------------------------------------------------------------------------
 
 //---------------------------------------------------------------------------
 int MRPET_dump::init_lor_info(){
   int hoff, a,b;
   int i, o, hb ,cntr;
   
   hoff=(Nhead_EPM-BrainPET_n_FOV+1)/2;
   cntr=0;
   n_mpairs=Nhead_EPM*BrainPET_n_FOV/2;
   // check to be sure we don't exceed the maximum allowable (496=32*31/2);
   if (n_mpairs > 496){
     cout << "ERROR: Invalid head_fov..n_mpairs" << endl;;
     exit(0);
   }
   if (BrainPET_n_FOV%2 == 0){
     cout << "ERROR: FOV should be odd number" << endl;
     exit(0);
   }
   for (i=0; i<Nhead_EPM; i++) {
     for (o=0; o<BrainPET_n_FOV; o++) {
       hb=i+o+hoff;
       if (hb < Nhead_EPM) {
         hpair[2*cntr]=i;
         hpair[2*cntr+1]=hb;
         cntr++;
       }
     }
   }
   if (cntr != n_mpairs){
     cout << "ERROR: init_lor_info(): number of n_mpairs" << endl;
     exit(0);
   }
   for (i=0; i<Nhead_EPM*Nhead_EPM; i++) 
     hplookup[i]=0;
   for (i=0; i<n_mpairs; i++) {
     
     a=hpair[2*i];
     b=hpair[2*i+1];
     hplookup[32*b+a]=i+1;
     hplookup[32*a+b]=-i-1;
   }
   return(n_mpairs);
 }
 
 
 //---------------------------------------------------------------------------
 //---compute flat address for crystal combination
 //---------------------------------------------------------------------------
 int MRPET_dump::compute_flat_address(int h1, int b1, int c1, 
                                              int h2, int b2, int c2){
   int a1; //adress of crystal_1
   int a2; //adress of crystal_2
   int ax; //tmp for switch
   int addr; //total adress of lor in lor-array
 
   int hp=hplookup[h1*32+h2];
   
   if (hp == 0) {
     bad_hp++; 
     return -1;
   }
   
   a1=144*b1+c1;
   a2=144*b2+c2;
   if (hp < 0) {
     hp=-hp;
     ax=a1;
     a1=a2;
     a2=ax;
   }
   
   //--- caluclation of adress:
   addr=(144*6*144*6)*(hp-1)+a2*144*6+a1;
   return addr;
 
 }
 
 
 //---------------------------------------------------------------------------
 //---get number of modul pairs (head combinations)
 //---------------------------------------------------------------------------
 int MRPET_dump::get_n_mpairs(){
   return n_mpairs;
 }
 
 
 //---------------------------------------------------------------------------
 //---get number of axial views
 //---------------------------------------------------------------------------
 int MRPET_dump::get_n_views(){
   return nviews;
 }  
 
 //---------------------------------------------------------------------------
 //---get size of a sinogram bin
 //---------------------------------------------------------------------------
 float MRPET_dump::get_binsize(){
   return binsize;
 }
 
 //---------------------------------------------------------------------------
 //---get size of a angular bin
 //---------------------------------------------------------------------------
 float MRPET_dump::get_angular_binsize()
 {
   return M_PI/nviews;
 }
 
 
 
 //---------------------------------------------------------------------------
 //---get number of radial projections
 //---------------------------------------------------------------------------
 int MRPET_dump::get_n_projs(){
   return nprojs;
 }
  
 //---------------------------------------------------------------------------
 //---get number of planes
 //---------------------------------------------------------------------------
 int MRPET_dump::get_n_planes(){
   return nplanes;
 }
 
 //---------------------------------------------------------------------------
 //---get maximum ringdifference (67 for brainPET)
 //---------------------------------------------------------------------------
 int MRPET_dump::get_max_ringdiff(){
   return maxRingdiff;
 }
 
 //---------------------------------------------------------------------------
 //---get span number (9 for brainPET)
 //---------------------------------------------------------------------------
 int MRPET_dump::get_span(){
   return span;
 }
 
 
 //---------------------------------------------------------------------------
 //--- initialise segments lookup tables:
 //---------------------------------------------------------------------------
 void MRPET_dump::init_seginfo(){
   int np, sp2, segnzs, segnum; //##z0,
   int i; //## j, rsq, ix, iy; 
 
   if(span == 0){
     cout << "ERROR: span = 0!" << endl;
     exit(0);
   }
 
   maxseg=maxRingdiff/span;
   nsegs=2*maxseg+1;
   np=2*nrings-1; // number of planes (2n+1 with interleaving)
   sp2=(span+1)/2; // min ring difference (5 for span=9)
 
   segz0  = new int[nsegs]; // minRD for segment[i]
   segnz  = new int[nsegs]; // number of elements in segment[i]
   segzoff= new int[nsegs]; // 
   segnums= new int[nsegs]; // segment index
   segnzs=0;
   nplanes=0;
 
   // for every segment
   for (i=0; i < nsegs; i++){
     segnum=(1-2*(i%2))*(i+1)/2;
     segnums[i]=segnum;
     if (i==0) 
       segz0[0]=0;
     else 
       segz0[i]=sp2+span*((i-1)/2);
     segnz[i]=np-2*segz0[i];
     segnzs+=segnz[i];
     if (i==0) 
       segzoff[0]=0;
     else 
       segzoff[i] = segzoff[i-1] + segnz[i-1];
     nplanes += segnz[i];
   }
   if(verbose>0){
     cout << "nPlanes: " << nplanes << endl;
   }
   // tilt angle of segments:
   d_tan_theta = span*plane_sep/crystal_radius;
   double tan0=0.0;
 
 
 
   if(verbose>0){
     cout << "-----------------------------------------------------------" << endl;
     cout << "MRPET_dump::init_seginfo:  Info: " << endl;
     cout << "-----------------------------------------------------------" << endl;
     cout << "maxseg=" << maxseg << endl;
     cout << "nsegs =" << nsegs << endl;
     cout << "d_tan_theta=" << d_tan_theta  << endl;
     //##int nsino=0;
     for (i=0; i< nsegs; i++)
       {
           printf("[i]=%2d  segnums=%3d   segnz=%3d   segz0=%3d  segzoff=%3d \n",
                  i,segnums[i], segnz[i],segz0[i],segzoff[i]);  
       }
     
     cout  << "Nsinos=" << nplanes << endl;
     cout << "-----------------------------------------------------------" << endl;
     
     
     cout << "Segment Angles:  "<< endl;
     cout << "span=" << span << endl;
     cout << "plane_sep=" << plane_sep << endl;
     cout << "crystal_radius=" << crystal_radius << endl;
     cout << "d_tan_theta = span*plane_sep / crystal_radius = " << d_tan_theta << endl;
   }
   double RAD2DEG=180.0/M_PI;
   for (i=0; i<=maxseg; i++)
     {
       if(verbose>0){
           printf("[%2d]:  %6.3f degree \n",i, RAD2DEG*atan(tan0));
       }
       tan0+=d_tan_theta;
     }
   
     
   double TAN_THETA=0.0;
   int segment0 = (int)floor(0.0/d_tan_theta+0.5); 
   if(verbose>0){
     cout << "-----------------------------------------------------------" << endl;
     cout << "segment0=" << segment0 << endl;
   }
   int is=0;
   double seg_angle_bound[10];
   seg_angle_bound[0]=0.0;
   for (i=0; (is<maxseg+2); i++)
     {
       int segment1 = (int)floor(TAN_THETA/d_tan_theta+0.5); 
       if (segment1>segment0)
           {
             is++;
             seg_angle_bound[is]=atan(TAN_THETA)*RAD2DEG;
             if(verbose>0){
               cout << is << "#: segment angle bound=" << seg_angle_bound[is] << endl;
             }
             segment0=segment1;
           }
       TAN_THETA+=0.025;
     }
   if(verbose>0){
     cout << "-----------------------------------------------------------" << endl;
     cout << "SegmentAngles: " << endl;
     for (i=1; i<=maxseg; i++)
       {
           printf("Seg %d    Angle1= %6.3f deg    Angle2= %6.3f deg      AngleMEAN= %6.3f deg\n",
                  i,seg_angle_bound[i], seg_angle_bound[i+1], 
                  0.5*(seg_angle_bound[i]+seg_angle_bound[i+1]));
       }
     cout << "-----------------------------------------------------------" << endl;
   }
 
   //generate segment LUTs:
   //--------------------------------------------------------------------------
   segments_lut   =new int[nplanes];
   z_val_lut   =new int[nplanes];
   min_angle_lut  =new float[nplanes];
   mean_angle_lut =new float[nplanes];
   max_angle_lut  =new float[nplanes];
 
 
   int plane;  
   for(i = 0; i < nsegs-1; i ++){
     for(plane = segzoff[i]; plane < segzoff[i+1]; plane++){
       //segment:
       segments_lut[plane] = segnums[i];
       //angles:
       if(segnums[i]==0){
           min_angle_lut[plane]  = 0;
           mean_angle_lut[plane] = 0;
           max_angle_lut[plane]  = 0;
       }else{
           if(i%2==0){
             min_angle_lut[plane]  = seg_angle_bound[(int)(i/2)];
             mean_angle_lut[plane] = 0.5*(seg_angle_bound[(int)(i/2)]+seg_angle_bound[(int)(i/2)+1]);
             max_angle_lut[plane]  = seg_angle_bound[(int)(i/2)+1];
           }else{
             min_angle_lut[plane]  = seg_angle_bound[(int)(i/2)+1];
             mean_angle_lut[plane] = 0.5*(seg_angle_bound[(int)(i/2)+1]+seg_angle_bound[(int)(i/2)+2]);
             max_angle_lut[plane]  = seg_angle_bound[(int)(i/2)+2];
           }
       }
       z_val_lut[plane] = (plane - segzoff[i] + segz0[i]);  
     }
   }    
   
 
   for(int plane = segzoff[i]; plane < nplanes; plane++){
     //segment:
     segments_lut[plane] = segnums[i];
       //angles:
       if(segnums[i]==0){
           min_angle_lut[plane]  = 0;
           mean_angle_lut[plane] = 0;
           max_angle_lut[plane]  = 0;
       }else{
           if(i%2==0){
             min_angle_lut[plane]  = seg_angle_bound[(int)(i/2)];
             mean_angle_lut[plane] = 0.5*(seg_angle_bound[(int)(i/2)]+seg_angle_bound[(int)(i/2)+1]);
             max_angle_lut[plane]  = seg_angle_bound[(int)(i/2)+1];
           }else{
             min_angle_lut[plane]  = seg_angle_bound[(int)(i/2)+1];
             mean_angle_lut[plane] = 0.5*(seg_angle_bound[(int)(i/2)+1]+seg_angle_bound[(int)(i/2)+2]);
             max_angle_lut[plane]  = seg_angle_bound[(int)(i/2)+2];
           }
       }
       z_val_lut[plane] = (plane - segzoff[i] + segz0[i]);  
     }
   //--------------------------------------------------------------------------
  
    
   //--------------------------------------------------------------------------
   //---create instanve of SinoSegmentStructure which collects 
   //---all axial compression properties
   SinoSegmentStructure* s=new SinoSegmentStructure;
   s->nviews=nviews;
   s->nradial=nprojs;
   s->nrings=nrings;
   s->nplanes_segment_0=(nrings*2)-1;
   s->ispan=span;
   s->imax_ring_diff=maxRingdiff;
   s->plane_separation=plane_sep/100.0;  //[from cm to m]
   s->nsinos=nplanes;
   
   s->set_number_of_segments(nsegs);
   for (i=1; i<nsegs; i++)
     {
       int is=abs(segnums[i]);
 
       double mean=0.5*(seg_angle_bound[is]+seg_angle_bound[is+1]);
       s->set_segment_info(i, segnums[i], segnz[i], segz0[i],
                           seg_angle_bound[is]/RAD2DEG, seg_angle_bound[is+1]/RAD2DEG, 
                           mean/RAD2DEG);                          
     }
 
   //---set also segment 0 infos
   s->set_segment_info(0,0,s->nplanes_segment_0, 0, 0.0, 0.0, 0.0);
 
   
   if (segment_struct!=NULL)
     {
       cout << "ERROR: MRPET_dump::init_seginfo: ";
       cout << "already defined; (overwrite not supported)" << endl;
       exit(0);
     }
   segment_struct=s;
 
   //---set sinogram segment identifier lookup
   s->set_sino_identifiers();
   //--------------------------------------------------------------------------
 
 
   compute_crystal_positions();
 }
 
 
 //---------------------------------------------------------------------------
 //---get sinogram segment structure
 //---(new instance created & ownership goes beyond this scope)
 //---------------------------------------------------------------------------
 SinoSegmentStructure* MRPET_dump::get_SinoSegmentStructure(void)
 {
   if (segment_struct==NULL)
     {
       cout << "ERROR: MRPET_dump::get_SinoSegmentStructure: ";
       cout << "Missing segment info!" << endl;
       exit(0);
     }
   return segment_struct->clone();
 }
 
 
 //---------------------------------------------------------------------------
 //---get number of sinograms as given in Siemens SinoSegment structure
 //---------------------------------------------------------------------------
 int MRPET_dump::get_number_of_sinos_siemens(void)
 {
    if (segment_struct==NULL)
      {
        cout << "ERROR: MRPET_dump::get_number_of_sinos_siemens ";
        cout << "Missing segment info!" << endl;
        exit(0);
      }
    return segment_struct->get_number_of_sinograms();
 }
 
 
 //---------------------------------------------------------------------------
 //---get SYMMETRIC sinogram segment structure (all number of planes are even!)
 //---(new instance created & ownership goes beyond this scope)
 //---symmetry is requested for sino_reco modul which exploits symmetries
 //---------------------------------------------------------------------------
 SinoSegmentStructure* MRPET_dump::get_SYMMETRIC_SinoSegmentStructure(void)
 {
   SinoSegmentStructure* s=get_SinoSegmentStructure();
   s->make_EVEN_plane_numbers();
   return s;
 }
 
 
 //---------------------------------------------------------------------------
 //---get segment info for plane number using LUTs
 //---------------------------------------------------------------------------
 void MRPET_dump::get_seg_info(int planeNumber, int &segment, int &z_value, 
                                    float &min_angle, float &max_angle, float &mean_angle){
   
   if(planeNumber < 0 || planeNumber > nplanes){
     cout << "ERROR: MRPET_dump::get_seg_info(), planenumber out of range" << endl;
   }else{
     segment =segments_lut[planeNumber];
     z_value=z_val_lut[planeNumber];
     min_angle=min_angle_lut[planeNumber];
     max_angle=max_angle_lut[planeNumber];
     mean_angle = mean_angle_lut[planeNumber];
   }
 }
 
 //---------------------------------------------------------------------------
 
 //---------------------------------------------------------------------------
 int MRPET_dump::seg_z_to_plane_number(int seg, int z){
   int segment = 2*abs(seg);
   if(seg<0)
     segment--;
   int plane = segzoff[segment] + z;
   return plane;
 }
 
 
 
 //---------------------------------------------------------------------------
 //---initialise modul pair data
 //---------------------------------------------------------------------------
 void MRPET_dump::init_mpair_data()
 {
   int bh; //##a, b, i;
     int m, mp, oh, mpoff;
 
     n_mods=32;
     n_mpairs=(n_mods*mod_fov)/2;
     mpair_a= new char[n_mpairs];//304
     mpair_b= new char[n_mpairs]; //304
     mpoff=(n_mods-mod_fov+1)/2;
     for (m=0,mp=0; m<n_mods; m++) {//32
       for (oh=0; oh<mod_fov; oh++) {//19
           bh=m+mpoff+oh;
           if (bh > n_mods-1) 
               continue;
           mpair_a[mp]=m;
           mpair_b[mp]=bh;
           mp++;
       }
     }
 }
 
 
 //---------------------------------------------------------------------------
 //--- computes for every scanner of the crystal its position
 //--- and stores it in point3D_typ *crystalPositions[ncrystals]
 //---------------------------------------------------------------------------
 int MRPET_dump::compute_crystal_positions()
 {
   rot = new MMatrix(4,4);
   vec = new MMatrix(1,4);
   pos = new MMatrix(1,4);
 
   float cspace=0.250; // crystal space in cm
   float axgap=0.250;  //
   float rotoff=0.0;
 
   int i,j; // index for crystal in x and y
   int currentCrystal, module, ring;
   float x, y, z;
   float  bangle, zoff; //## rangle;
 
   // get handle for data arrays (bad programming style):
   vec_data = vec->get_data();
   pos_data = pos->get_data();
 
   zoff=6*12*cspace+cspace*4;
   binsize=cspace/2.0;
   currentCrystal = 0;
 
   for (ring=0; ring <mod_nrings; ring++)
     for (module=0; module<nheads; module++) { 
       rot->mat_unity();
       bangle = 360.*module/nheads;
       rot->rotate(0.0, 0.0, -bangle+rotoff);
       for (i=0; i<mod_nx; i++)  // crystals in this block
           for (j=0; j<mod_nx; j++) {
             vec_data[0]=cspace*(i-(mod_nx-1)/2.0);
             vec_data[1]=crystal_radius;
             vec_data[2]=zoff-j*cspace-ring*(mod_nx*cspace+axgap);
 
             pos->matmpy(vec, rot);
 
             x=pos_data[0];
             y=pos_data[1];
             z=pos_data[2];
 
             currentCrystal=module*mod_nrings*mod_nx*mod_nx+ring*mod_nx*mod_nx+j*mod_nx+i;
             
             if(currentCrystal<ncrystals){
               crystalPositions[currentCrystal].x = x;
               crystalPositions[currentCrystal].y = y;
               crystalPositions[currentCrystal].z = z;
             }else{
               cout << "ERROR: MRPET_dump::get_crystal_position()" << endl;
             }
           }
     } 
   return (ncrystals);
 }
 
 //---------------------------------------------------------------------------
 //---compute LOR address
 //---------------------------------------------------------------------------
 int MRPET_dump::compute_lor_addr(int block1, int crystal1, int block2, int crystal2, 
                                          int &proj, int &view, int &plane, int &segment)
 {
     float det1_pos[3], det2_pos[3], lor[4];
     int addr;
     get_crystal_position( block1, crystal1, det1_pos);
     get_crystal_position( block2, crystal2, det2_pos);
     phy_to_pro( det1_pos, det2_pos, lor);
     addr = pro_to_addr(lor, proj, view, plane, segment);
     return addr;
 }
 
 
 
 //---------------------------------------------------------------------------
 //---get 3d crystal position from blockID and crystalID
 //---------------------------------------------------------------------------
 void MRPET_dump::get_crystal_position(int block_x, int crystal, float pos[3])
 {
     int head, block, det_x, det_y; 
     int crystalNumber;
     
     // NOTE: for MRPet, detx swaped with dety and order reversed
     // normally x=c%12 y=c/12
     // what seems to work is x=11-c/12 y=11-c%12
     
     head = block_x/6;
     block = block_x%6;
     det_x =11-crystal/12;
     det_y =11-crystal%12;
     crystalNumber = head*864 + block*144 + det_y*12 + det_x;
     if(crystalNumber<ncrystals){
       pos[0]=crystalPositions[crystalNumber].x;
       pos[1]=crystalPositions[crystalNumber].y;
       pos[2]=crystalPositions[crystalNumber].z;
     }else{
       cout << "ERROR: : MRPET_dump::get_crystal_position()" << endl;
     }
 
 }
 
 
 
 //---------------------------------------------------------------------------
 //---detector to physical address
 //---------------------------------------------------------------------------
 void MRPET_dump::det_to_phy(int mod, int x, int y, float pos[3]){
   int k;
   //crystalNumber = head*864          + block*144 + det_y*12 + det_x;
   k               = mod*mod_ny*mod_nx             + mod_nx*y + x;
   pos[0]=crystalPositions[k].x;
   pos[1]=crystalPositions[k].y;
   pos[2]=crystalPositions[k].z;
 }
 
 //---------------------------------------------------------------------------
 //---physical to projection
 //---------------------------------------------------------------------------
 void MRPET_dump::phy_to_pro( float deta[3], float detb[3], float projection[4])
 {
   double dx, dy, dz, d;
   float  r, phi, tan_theta, z;
   float pi = M_PI;
 
   dz = detb[2]-deta[2];
   dy = deta[1]-detb[1];
   dx = deta[0]-detb[0];
   d = sqrt( dx*dx + dy*dy);
   r = (deta[1]*detb[0]-deta[0]*detb[1])/d;
   phi = atan2( dx, dy);
   tan_theta = dz / d;
   z = deta[2]+(deta[0]*dx+deta[1]*dy)*dz/(d*d);
   if (phi < 0.0) {
     phi += pi;
     r *= -1.0;
     tan_theta *= -1.0;
   }
   if (phi == pi){
     phi=0.0;
     r *= -1.0;
     tan_theta *= -1.0;
   }
   projection[0] = r;
   projection[1] = phi;
   projection[2] = z;
   projection[3] = tan_theta;
 }
 
 
 //---------------------------------------------------------------------------
 //---projection to address
 //---------------------------------------------------------------------------
 int MRPET_dump::pro_to_addr( float pro[4], int &bin, int &view, int &plane, int &segment)
 {
   int zplane, segnum;
   float r, phi, z, tan_theta;
   int addr=-1;
 
   r = pro[0];
   phi = pro[1];
   z = pro[2];
   tan_theta = pro[3];
 
   bin = (int)(nprojs/2+(r/binsize+0.5));             // pro[0] is r
   view = (int)(nviews*phi/M_PI);                     // pro[1] is phi
   zplane = (int)(0.5+z/plane_sep);                 // pro[2] is z
   segment = (int)floor(tan_theta/d_tan_theta+0.5); // pro[3] is tan_theta
   segnum = 2*abs(segment);
   // cout << " view: " << view
   //      << " projection: " << bin; 
 
      
   if (segment<0) 
     segnum--;
   if ((bin <1) || (bin > nprojs-1)){ 
 //     cout << "radius: " << r << " phi: "<< phi 
 //         << " z: " << z << " tan_theta: "<< tan_theta << endl;
     return( -1);
   }
   if (segnum>nsegs-1){
 //     cout << "radius: " << r << " phi: "<< phi 
 //         << " z: " << z << " tan_theta: "<< tan_theta << endl;
     return( -1);
   }
   if(zplane < segz0[segnum]) {
 //     cout << "radius: " << r << " phi: "<< phi 
 //         << " z: " << z << " tan_theta: "<< tan_theta << endl;
     return(-1);
   }
   if (zplane > segz0[segnum]+segnz[segnum]-1) {
 //     cout << "radius: " << r << " phi: "<< phi 
 //         << " z: " << z << " tan_theta: "<< tan_theta << endl;
     return(-1);
   }
   plane = (zplane-segz0[segnum]+segzoff[segnum]);
 //   svals[0]=bin;
 //   svals[1]=view;
 //   svals[2]=zplane;
 //   svals[3]=segnum;
 //   svals[4]=plane;
 
   addr = bin + nprojs*view + nprojs*nviews*plane;
   //histogram
 //   if(plane<143)
 //     radiusHistogram[bin]++;
   //cout   << " plane: " << plane << " flat addr: " << addr << endl;
   return(addr);
 }
 
 
 //---------------------------------------------------------------------------
 //---modul pair lookup: modules to index
 //---------------------------------------------------------------------------
 void MRPET_dump::mpair_lookup(int &amod, int &bmod, int index){
         amod=mpair_a[index];
         bmod=mpair_b[index];
 }
 
 //---------------------------------------------------------------------------
 //---modul pair lookup: index to modules
 //---------------------------------------------------------------------------
 int MRPET_dump::mpair_lookup2(int amod, int bmod){
   int index = -5;
   for(int i = 0; i < n_mpairs; i++){
     if(mpair_a[i]== amod && mpair_b[i] == bmod){
       index = i;
       break;
     }
   }
 //   if(index ==-5){
 //     for(int i = 0; i < n_mpairs; i++){
 //       if(mpair_a[i]== bmod && mpair_b[i] == amod){
 //        index = i;
 //        break;
 //       }
 //     }
 //   }
   if(index ==-5){
     cout << "ERROR: combination of mod " << amod <<  " and " 
            << bmod << " not found" << endl;
   }
   return index;
 }
 
 
 
 
 //---------------------------------------------------------------------------
 //---Convert flat address to (view, projection, plane)
 //---------------------------------------------------------------------------
 void MRPET_dump::addr_to_view_proj_plane(int addr, int &view, int &proj, int &plane){
   plane = addr/(nprojs*nviews);
   view = (addr-(nprojs*nviews*plane))/nprojs;
   proj = (addr-(nprojs*nviews*plane)-(nprojs*view));    
 }
 
 
 //---------------------------------------------------------------------------
 //---Convert (view, projection, plane) to address
 //---bug fix: missing implementation body until 17/08/2011 JScheins
 //---------------------------------------------------------------------------
 void MRPET_dump::proj_to_addr(int view, int proj, int plane, int &addr)
 {
   cout << "ERROR: MRPET_dump::proj_to_addr:";
   cout << "missing implementation!" << endl;
   exit(0);
 }
 
 //---------------------------------------------------------------------------
 //---projection (z, tan_theta) to planenumber
 //---------------------------------------------------------------------------
 int MRPET_dump::pro_to_planenumber(float z, float tan_theta)
 {
   int zplane, segment, segnum;
   int plane;
 
   zplane = (int)(0.5+z/plane_sep);         // pro[2] is z
   segment= (int)floor(tan_theta/d_tan_theta+0.5); // pro[3] is tan_theta
   segnum = (int)(2*abs(segment));
      
   if (segment<0) 
     segnum--;
 
   if (segnum>nsegs-1){
     return( -2);
   }
   if(zplane < segz0[segnum]) {
     return(-3);
   }
   if (zplane > segz0[segnum]+segnz[segnum]-1) {
     return(-4);
   }
   plane = (zplane-segz0[segnum]+segzoff[segnum]);
   return plane;
 }
 
 int MRPET_dump::get_mod_nx(){
   return mod_nx;
 }
 
 int MRPET_dump::get_mod_ny(){
   return mod_ny;
 }
 
 
 
 //---------------------------------------------------------------------------
 //---Initialise EPM lookuptable
 //---------------------------------------------------------------------------
 void MRPET_dump::make_slut(){
   int head;
   int block;
   int slot;
   int chan;
   int d;
 
   for (head=0; head<32; head++) 
     for (block=0; block<6; block++) {
       d=(head%2)*6+block;
       slot=6*(head/4)+2*(d/4)+((head/2)%2);
       chan=d%4;
       slut[4*slot+chan]=6*head+block;
       blut[6*head+block]=4*slot+chan;
   }
 }
 
 
 
 //---------------------------------------------------------------------------
 short int MRPET_dump::get_hp_lookup(int headA, int headB){
   return hplookup[headA*32+headB];
 }
//Generated on Tue Jun 18 17:17:17 2013 for *** PRESTO *** Pet Reconstruction Software TOolbox *** revision 333 *** by  ￼ 1.5.6
