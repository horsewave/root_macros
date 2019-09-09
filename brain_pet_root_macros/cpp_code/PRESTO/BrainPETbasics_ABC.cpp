
 #include "BrainPETbasics_ABC.hh"
 
 //----------------------------------------------------------------------------------------
 //---Constructor
 //----------------------------------------------------------------------------------------
 BrainPETbasics_ABC::BrainPETbasics_ABC(void):EPMpattern()
 {
   cout << "################################################################################" << endl;
   cout << "BrainPETbasics_ABC::BrainPETbasics_ABC:  Info: " << endl;
   cout << "BrainPETbasics_ABC based on Code from Larry Byars  v1.0   7/12/2007" << endl;
   cout << "extracted from code: mrpet_sort.c" << endl;
   cout << "Note: " << endl;
   cout << "Z Mirroring of detector system introduced 3/1/2008 " << endl;
   cout << "see method_compute_crystal_position()" << endl;
   cout << "################################################################################" << endl;
   cout << endl;
 
   //---check word length
   if (sizeof(short int)!=2)
     {
       cout << "BrainPETbasics_ABC::BrainPETbasics_ABC:  Error: " << endl; 
       cout << "Unexpected word length for short int!" << endl;
       exit(0);
     }
 
   //---basic dimensions
   crystal_radius=0.;   //---7mm interaction point +0.7;
 
   //---re-introduced (lost for update to version 169 from C.Weirich updates)
   crystal_radius+=0.007;   //---7mm interaction point +0.7;
   cout << "BrainPETbasics_ABC::BrainPETbasics_ABC:  Info: ";
   cout << "SystemRadius=" << crystal_radius << endl;
   cout << "additional offset 7mm (was missing from revision 169ff); repaired 6/9/2011" << endl;
   
 
   binsize=0.;          //---half crystal width
 
 
   //---basic parameters
   mod_nx=12;     //---transaxial number of crystals per block
   mod_ny=72;     //---axial number of crystals (6x12) for 6 blocks 12 crystals each
   mod_nrings=6;  //---number of blocks per modul
   maxrd=67;      //---default ring differnce (if used)
   nrings=77;     //---number of rings (including gaps)
   n_mods=32;     //---number of moduls 
 
   //---modul pair size
   mpsize = mod_nx*mod_ny*mod_nx*mod_ny;
 
   //---true ringID lookup table (account for gaps)
   true_ringID=new int[mod_ny];
   cout << "Axial true ring numbering: ";
   for (int i=0; i<mod_ny; i++)
     {
       if (i%mod_nx==0) cout << endl;
       int igaps=i/mod_nx;
       true_ringID[i]=i+igaps;
       printf("%02d ",true_ringID[i]);
     }
   cout << endl;
 
 
   mod_fov=19;    //---fan size of opposite moduls
   mpair_a=NULL;  //---modul number A of modul pair
   mpair_b=NULL;  //---modul number B of modul pair 
 
   //---make init of modul pair data sorting
   init_mpair_data();
 
   //---geometrical stuff of crystals addressed via 1D index=[0..ncrystals[
   ncrystals_per_ring=n_mods*mod_nx;
   ncrystals=ncrystals_per_ring*mod_ny;
   cout << "Number of Crystals per Ring=" << ncrystals_per_ring << endl;
   cout << "Number of Crystals=" << ncrystals << endl;
 
   //---define crystal central points arrays (filled in compute_crystal_positions())
   cpoint=new point3D_typ[ncrystals];
   cpoint_SiemensConvention=new point3D_typ[ncrystals];
   memset(cpoint_SiemensConvention,0,sizeof(point3D_typ)*ncrystals);
   memset(cpoint,0,sizeof(point3D_typ)*ncrystals);
 
 
   //---decoding pattern for all crystals
   icrystal_adr=new crystal_pos_typ[ncrystals];
 
   //---decoding pattern for all crystals (ring-wise)
   //---crystal_posII:   icrystal_on_ring=[0..12*32[;  iring=[0..72[;  
   icrystal_adr_typII=new crystal_pos_typII[ncrystals];
 
   //---fill geometrical structures & create crystal polyhedrons
   compute_crystal_positions();
 
 
   //---check address conversion (1)
   for (int i=0; i<ncrystals; i++)
     {
       crystal_pos_typ cpos=get_crystal_address(i);
       int ii=get_crystal_index(cpos);
       if (ii!=i)
           {
             cout << "BrainPETbasics_ABC::BrainPETbasics_ABC:  Error: " << endl; 
             cout << "error(s) in address conversion; bug in code!" << endl;
             exit(0);
           }
     }
 
   //---check address conversion (2)
   for (int icrystal=0; icrystal<ncrystals_per_ring; icrystal++)
     {
       for (int iring=0; iring<mod_ny; iring++)
           {
             crystal_pos_typII cposII_a,cposII_b;
             cposII_a.iring=iring;
             cposII_a.icrystal_on_ring=icrystal;
             crystal_pos_typ cpos;
             cpos=convert_address(cposII_a);
             cposII_b=convert_address(cpos);
 
             if ((cposII_b.iring!=iring) || (cposII_b.icrystal_on_ring!=icrystal))
               {
                 cout << "BrainPETbasics_ABC::BrainPETbasics_ABC:  Error: " << endl; 
                 cout << "check provides conversion mismatches!" << endl;
                 exit(0);
               }
           }
     }
 
   //---check coding/decoding
   for (int i=0; i<mpsize; i++)
     {
       int iay,iax,ibx,iby;
       decode(i,iay,iax,iby,ibx);
       int i2;
       i2=code(iay,iax,iby,ibx);
       if (i2!=i)
           {
             cout << "BrainPETbasics_ABC::BrainPETbasics_ABC:  Error: " << endl; 
             cout << "methods code() decode() incompatible!" << endl;
             exit(0);
           }
     }
   
   //---check 1D crystal addressing
   for (int i=0; i<ncrystals; i++)
     {
       int idx=get_crystal_index(icrystal_adr[i]);
       if (i!=idx)
           {
             cout << "BrainPETbasics_ABC::BrainPETbasics_ABC:  Error: " << endl; 
             cout << "get_crystal_index() mismatch!" << endl;
             exit(0);
           }
     }
 
   cout << "coding/decoding OK!" << endl; 
 
   cout << "BrainPETbasics_ABC::BrainPETbasics_ABC:  Info: --- init done --- " << endl; 
   cout << "################################################################################" << endl;  
 } 
 
 
 
 //----------------------------------------------------------------------------------------
 //---init of modul pair data
 //----------------------------------------------------------------------------------------
 void BrainPETbasics_ABC::init_mpair_data(void)
 {
   if ((mpair_a!=NULL) || (mpair_b!=NULL))
     {
       cout << "BrainPETbasics_ABC::init_mpair_data:  Error: ";
       cout << "You try to initialise twice!" << endl;
       cout << "stop program!" << endl;
       exit(0);
     }
 
   //int a, b, bh, i;
   int bh;
   int m, mp, oh, mpoff;  
   n_mpairs=(n_mods*mod_fov)/2;
   cout << "Number of Modul Pairs: " << n_mpairs << "   for FANsize=" << mod_fov << endl;
 
   //---init modul pair numbers
   mpair_a=new int[n_mpairs];
   mpair_b=new int[n_mpairs];
   mpoff=(n_mods-mod_fov+1)/2;  
   for (m=0,mp=0; m<n_mods; m++) 
     {
       for (oh=0; oh<mod_fov; oh++) 
           {
             bh=m+mpoff+oh;
             if (bh > n_mods-1) continue;
             mpair_a[mp]=m;
             mpair_b[mp]=bh;
             mp++;
           }
     }
 
   //---modul pair IDs
   mod_pairID=new Array2D_NxM<int>(n_mods,n_mods);
   int dummy=-1;
   mod_pairID->set_all_elements(dummy);
   for (int i=0; i<n_mpairs; i++)
     {
       mod_pairID->set_element(mpair_a[i],mpair_b[i],i);
       //mod_pairID->set_element(mpair_b[i],mpair_a[i],i);
     }
 
 
   //---decode addressing within one Modul combination
   decoder_lookup=new modul_combi_decoder_lookup_typ[mpsize];
   memset(decoder_lookup,0,sizeof(modul_combi_decoder_lookup_typ)*mpsize);
   for (int i=0; i<mpsize; i++)
     {
       decoder_lookup[i].iax=-1;
     }
 
   //---create lookup table for quick referencing
   //---0..11
   int icheck=0;
   for (int ax=0; ax<mod_nx; ax++)  
     {
       //---0..71
       for (int ay=0; ay<mod_ny; ay++) 
           {
             int index_a=((ay/12)*144+12*(11-ax)+(11-(ay%12)))*864;      
             for (int bx=0; bx<mod_nx; bx++)
               {
                 for (int by=0; by<mod_ny; by++) 
                     {
                       int kk=((by/12)*144+12*(11-bx)+(11-(by%12)))+index_a;
                       if ((kk<0) || (kk>=mpsize))
                         {
                           cout << "BrainPETbasics_ABC::init_mpair_data:  Error: ";
                           cout << "Unexpected index!" << endl;
                           exit(0);
                         }
                       if (decoder_lookup[kk].iax!=-1)
                         {
                           cout << "BrainPETbasics_ABC::init_mpair_data:  Error: ";
                           cout << "Lookup table generation error; bug in code!" << endl;
                           exit(0);
                         }
                       decoder_lookup[kk].iax=ax;
                       decoder_lookup[kk].iay=ay;
                       decoder_lookup[kk].ibx=bx;
                       decoder_lookup[kk].iby=by;
 
                       int II=code(ay,ax,by,bx);
                       if (II!=kk)
                         {
                           cout << "BrainPETbasics_ABC::init_mpair_data:  Error: ";
                           cout << "decoding error!" << endl;
                           cout << "II=" << II << "kk=" << kk << endl;
                           exit(0);
                         }
                       icheck++;
                     }
               }
           }
     }
   if (icheck!=mpsize)
     {
       cout << "BrainPETbasics_ABC::init_mpair_data:  Error: ";
       cout << "Lookup table generation error (missing elements); bug in code!" << endl;
       exit(0);      
     }
 }
 
 //----------------------------------------------------------------------------------------
 //---find modul pair ID
 //----------------------------------------------------------------------------------------
 int BrainPETbasics_ABC::get_modul_pairID(int imodA, int imodB)
 {
   if ((imodA<0) || (imodA>=n_mods) ||
       (imodB<0) || (imodB>=n_mods))
     {
       cout << "BrainPETbasics_ABC::get_modul_pairID:  Error: ";
       cout << "Invalid modul pair!" << endl;
     }
   int ii=mod_pairID->element(imodA,imodB);
   if (ii<0)
     {
       cout << "BrainPETbasics_ABC::get_modul_pairID:  Error: ";
       cout << "Modul pair not in lookup table!" << endl;
       cout << "imodA=" << imodA << endl;
       cout << "imodB=" << imodB << endl;
       exit(0);
     }
   return ii;
 }
 
 //----------------------------------------------------------------------------------------
 //---hardware coding/decoding of modul pair data index
 //----------------------------------------------------------------------------------------
 void BrainPETbasics_ABC::decode(int ipair_data, int &ay, int &ax, int &by, int &bx)
 {
   if ((ipair_data<0) || (ipair_data>=mpsize)) 
     {
       cout << "BrainPETbasics_ABC::decode:  Error: ";
       cout << "invalid index!" << endl;
       exit(0);
     }
   
   int IA=ipair_data / 864;
   int IB=ipair_data % 864;
 
   //---TERM 1
   int i1=IA/144;
   ay=i1*12;
 
   //---TERM 2
   int i2=IA%144;
   int i2a=i2/12;
   ax=11-i2a;
 
   //---TERM 3
   int i2b=i2%12;
   ay+=(11-i2b);
 
   //---TERM 1
   i1=IB/144;
   by=i1*12;
 
   //---TERM 2
   i2=IB%144;
   i2a=i2/12;
   bx=11-i2a;
 
   //---TERM 3
   i2b=i2%12;
   by+=(11-i2b);
 
 }
 
 //----------------------------------------------------------------------------------------
 //---hardware coding/decoding of modul pair data index
 //----------------------------------------------------------------------------------------
 int BrainPETbasics_ABC::code(int ay, int ax, int by, int bx)
 {
   //----------index_a= IBLOCK*144 +
   int index_a=((ay/12)*144 + 12*(11-ax) + (11-(ay%12)))*864;            
   int ipair_data=((by/12)*144+12*(11-bx)+(11-(by%12)))+index_a;
   return ipair_data;
 }
 
 
 
 //----------------------------------------------------------------------------------------
 //---calculate true ring difference which can be larger due to
 //---existing ring gaps (iy1,iy2 < 72)
 //----------------------------------------------------------------------------------------
 int BrainPETbasics_ABC::get_true_ring_diff(int iy1, int iy2)
 {
   //---one ring gap between each axial block
   int iblock1=iy1/12;
   int iblock2=iy2/12;
 
   iy1+=iblock1;
   iy2+=iblock2;
   return abs(iy1-iy2);
 }
 
 
 
 //----------------------------------------------------------------------------------------
 //---compute crystal positions
 //----------------------------------------------------------------------------------------
 void BrainPETbasics_ABC::compute_crystal_positions(void)
 {
   //--------------------------------------------------------
   //---Modification 3/1/2008 
   //--- (1) Z mirroring (!)
   //--- (2) axial shift by one ring to have |zmin|>|zmax|
   //--------------------------------------------------------
 
   cout << "BrainPETbasics_ABC::compute_crystal_positions: start" << endl;
   MMatrix* rot;
   MMatrix* vec;
   MMatrix* pos;
   float cspace=0.;
   float axgap=0.;
 
 
   //---CRYSTAL LENGTH RELEVANT FOR CRYSTAL DISTRIBUTION KEY CALCULATIONS
   //---float crystal_length=0.02; // 20 mm
 
   //---reduced value to reduce smearing due to depth of interaction problems
   //float crystal_length=0.0001; // 0.1 mm
   float crystal_length=0.005; // 0.1 mm
 
   cout << "--------> CRYSTAL LENGTH=" << crystal_length << " [m]" << endl;
 
 
   float rotoff=0.0;
   int k;
   //int b;
   //float rangle; 
   float bangle, x, y, z, zoff;
 
   rot=new MMatrix(4,4);
   vec=new MMatrix(1,4);
   float* vec_data=vec->get_data();
   vec_data[3]=1.0;
   pos=new MMatrix(1,4);
   float* pos_data=pos->get_data();
 
   zoff=6*12*cspace+cspace*4;
   //  zoff=0.0;
   binsize=cspace/2.0;
 
   //---shrink crystal size a little bit in z direction
   double zscaler=0.999;
 
   //---define 8 edges of crystal  
   point3D_typ pcrystal[8];
   memset(pcrystal,0,sizeof(point3D_typ)*8);
   pcrystal[0].x=-binsize;
   pcrystal[0].y= 0.0;
   pcrystal[0].z=-binsize*zscaler;
 
   pcrystal[1].x=-binsize;
   pcrystal[1].y= 0.0;
   pcrystal[1].z= binsize*zscaler;
 
   pcrystal[2].x= binsize;
   pcrystal[2].y= 0.0;
   pcrystal[2].z= binsize*zscaler;
 
 
   pcrystal[3].x= binsize;
   pcrystal[3].y= 0.0;
   pcrystal[3].z=-binsize*zscaler;
 
   for (int i=0;i<4; i++)
     {
       pcrystal[4+i]=pcrystal[i];
       pcrystal[4+i].y=crystal_length;
     }
  
   //---final Z coordinate shift for symmetric axial structure
   //---corresponds to +1 ring + 1/2 shift for crystal center
   double ZOFF=0.095+(0.5*cspace);  
 
   k=0;
   for (int izblock=0; izblock<mod_nrings; izblock++)  //---mod_nrings=6
     for (int ihead=0; ihead<n_mods; ihead++)          //---n_mods=32  (head number)
       { 
           rot->mat_unity();
           bangle = 360.*ihead/n_mods;
           rot->rotate( 0.0, 0.0, -bangle+rotoff);
           for (int ictransax=0; ictransax<mod_nx; ictransax++)  // crystals in this block
             for (int icaxial=0; icaxial<mod_nx; icaxial++) 
               {
                 vec_data[0]=cspace*(ictransax-(mod_nx-1)/2.0);
                 vec_data[1]=crystal_radius;
                 vec_data[2]=zoff-icaxial*cspace-izblock*(mod_nx*cspace+axgap);
               
                 pos->matmpy( vec, rot);
                 x=pos_data[0];
                 y=pos_data[1];
 
                 //***********************
                 z=    -   pos_data[2];
                 //***********************
 
                
 
                 // k is the "crystal number" ordered by head, block, row, col
                 // k = head*864+block*144+dety*12+detx
 
                 k=ihead*(mod_nrings*mod_nx*mod_nx)+
                     izblock*(mod_nx*mod_nx)+
                     icaxial*mod_nx+
                     ictransax;
 
                 icrystal_adr[k].ihead=ihead;
                 icrystal_adr[k].iblock=izblock;
                 icrystal_adr[k].iaxial_crystal=icaxial;
                 icrystal_adr[k].itransaxial_crystal=ictransax;
 
                 //---calculate Siemens convention of crystal coordinates
                 //---Note: units are centimeter (!)  [cm] not meter as the others (!)
                 cpoint_SiemensConvention[k].x=pos_data[0]*100.0;
                 cpoint_SiemensConvention[k].y=pos_data[1]*100.0;
                 cpoint_SiemensConvention[k].z=pos_data[2]*100.0;
 
                 
                 //---change coordinate calculations to original Siemens cms definition 
                 //---20/04/2009 Scheins
                 
 //              //---calculate crystal coordinates of PRESTO convention
 //              point3D_typ cms;
                 
 //              cms.x=0.0;
 //              cms.y=0.0;
 //              cms.z=0.0;
 //              for (int ii=0; ii<8; ii++)
 //                 {
 //                   vec_data[0]=cspace*(ictransax-(mod_nx-1)/2.0) + pcrystal[ii].x;
 //                   vec_data[1]=crystal_radius + pcrystal[ii].y;
 //                   vec_data[2]=zoff-icaxial*cspace-izblock*(mod_nx*cspace+axgap) + pcrystal[ii].z;
 
                       
 //                    //---original mrpet_dump code.....
 //                    //vec->data[0]=cspace*(i-(mod_nx-1)/2.0);
 //                    //vec->data[1]=crystal_radius;
 //                    //vec->data[2]=zoff-j*cspace-r*(mod_nx*cspace+axgap);
                       
 
 //                   pos->matmpy( vec, rot);
 //                   point3D_typ pp;
 //                   pp.x=pos_data[0];
 //                   pp.y=pos_data[1];
 
 //                   //***********************
 //                   //***********************
 
 //                   pp.z=   -   (pos_data[2] - ZOFF);
 
 //                   pp.z-=  cspace;
 
 //                   //***********************
 //                   //***********************
 
 //                    cms.x+=pp.x;
 //                    cms.y+=pp.y;
 //                    cms.z+=pp.z;
 
 //                 }
 
 //                  //---use crystal cms point for all LOR definitions(!)
 //                  cms.x/=8.0;
 //                  cms.y/=8.0;
 //                  cms.z/=8.0;
                 
 
                     
 //              //---the new version (close to mr_petdum)  3.4.2009_
 //              vec_data[0]=cspace*(ictransax-(mod_nx-1)/2.0);
 //              vec_data[1]=crystal_radius;
 //              vec_data[2]=zoff-icaxial*cspace-izblock*(mod_nx*cspace+axgap);
 
 //              pos->matmpy( vec, rot);
 //              cms.x=pos_data[0];
 //              cms.y=pos_data[1];
 //              cms.z=   -   (pos_data[2] - ZOFF);                
 //              cms.z-=  cspace;
 
 //               cpoint[k]=cms;
                
 
 
                 //---simple version of calculation since 20/04/2009
 
                 vec_data[0]=cspace*(ictransax-(mod_nx-1)/2.0);
                 //#####################################################################################################################
                 //#####################################################################################################################
                 //---stretch factor to get same geometrical properties as for Siemens ReKo
                 //vec_data[1]=crystal_radius*1.024;  
                 vec_data[1]=crystal_radius*1.00;  
                 //#####################################################################################################################
                 //#####################################################################################################################
                 vec_data[2]=zoff-icaxial*cspace-izblock*(mod_nx*cspace+axgap);
                 
                 pos->matmpy( vec, rot);
                 point3D_typ pp;
                 pp.x=pos_data[0];
                 pp.y=pos_data[1];
 
                 //-----------------------------
                 //-----------------------------
                 
                 pp.z=   -   (pos_data[2] - ZOFF);
                 
                 pp.z-=  cspace;
 
                 //-----------------------------
                 //-----------------------------
 
               cpoint[k]=pp;
 
               }
       } 
   delete pos;
   delete rot;
   delete vec;
 
   //---detect min/max Z positions of polyhedrons
   double zmi,zma;
   zmi=+1E30;
   zma=-1E30;
   crystal_pos_typ ima,imi;
   for (int i=0; i<ncrystals; i++)
     {
       point3D_typ p=cpoint[i];
 
       if (p.z>zma) 
           {
             zma=p.z;
             ima=get_crystal_address(i);
           }
       if (p.z<zmi) 
           {
             zmi=p.z;
             imi=get_crystal_address(i);
           }
     }
   cout << "-------------------------------------------------------------------------------" << endl;
   cout << "BrainPETbasics_ABC::compute_crystal_positions:  Info: ";
   cout << "ZRANGE:  [" << zmi << " , " << zma << "]" << endl; 
   cout << "MAX. address:   iblock=" << ima.iblock << "   iaxial="  << ima.iaxial_crystal << endl;
   cout << "MIN. address:   iblock=" << imi.iblock << "   iaxial="  << imi.iaxial_crystal << endl;
   cout << "-------------------------------------------------------------------------------" << endl;
 
   //---create lookup for 2nd addressing scheme
   for (int i=0; i<ncrystals; i++)
     {
       icrystal_adr_typII[i]=convert_address(icrystal_adr[i]);
     }
 
   cout << "BrainPETbasics_ABC::compute_crystal_positions: finished" << endl;
 }
 
 
 
 //----------------------------------------------------------------------------------------
 //---get physical coordinates
 //----------------------------------------------------------------------------------------
 void BrainPETbasics_ABC::det_to_phy(int mod, int x, int y, point3D_typ &ccoord)
 {
   int k=get_crystal_index(mod,x,y);
   ccoord=cpoint[k];
 }
 
 
 //----------------------------------------------------------------------------------------
 //---get physical coordinates in Siemens convention
 //----------------------------------------------------------------------------------------
 void BrainPETbasics_ABC::det_to_phy_SiemensConvention(int mod, int x, int y, 
                                                                   point3D_typ &ccoord)
 {
   int k=get_crystal_index(mod,x,y);
   ccoord=cpoint_SiemensConvention[k];
 }
 
 
 //----------------------------------------------------------------------------------------
 //---get crystal index
 //---imod=[0..32[  ix=[0..12[  iy=[0..72[
 //----------------------------------------------------------------------------------------
 int BrainPETbasics_ABC::get_crystal_index(int imod, int ix, int iy)
 {
   if ((imod<0) || (imod>=n_mods) || 
       (ix<0) || (ix>=mod_nx) ||
       (iy<0) || (iy>=mod_ny))
     {
       cout << "BrainPETbasics_ABC::get_crystal_index:  Error: ";
       cout << "Invalid crystal address!" << endl;
       cout << "ranges: imod=[0..32[  ix=[0..12[  iy=[0..72[" << endl;
       exit(0);
     }
   return (imod*mod_ny*mod_nx + mod_nx*iy + ix);  
 }
 
 
 
 //----------------------------------------------------------------------------------------
 //---get crystal index 
 //----------------------------------------------------------------------------------------
 int BrainPETbasics_ABC::get_crystal_index(crystal_pos_typ &crystal_pos)
 {
   if ((crystal_pos.ihead<0) || (crystal_pos.ihead>=n_mods) ||
       (crystal_pos.iblock<0) || (crystal_pos.iblock>=mod_nrings) ||
       (crystal_pos.iaxial_crystal<0)  || (crystal_pos.iaxial_crystal>=mod_nx) || // (! mod_nx) 
       (crystal_pos.itransaxial_crystal<0)  || (crystal_pos.itransaxial_crystal>=mod_nx))
     {
       cout << "BrainPETbasics_ABC::get_crystal_index:  Error: ";
       cout << "Invalid address!" << endl;
       exit(0);
     }
 
   int iy=crystal_pos.iblock*mod_nx+crystal_pos.iaxial_crystal;
   return get_crystal_index(crystal_pos.ihead, crystal_pos.itransaxial_crystal, iy);
 }
 
 
 //----------------------------------------------------------------------------------------
 //---get number of sequential blocks (i.e. head combinations resp. modul pairs)
 //---and number of elements in each block (same for all blocks)
 //----------------------------------------------------------------------------------------
 void BrainPETbasics_ABC::get_sequential_data_sizes(int &nblocks, int &nblock_size)
 {
   nblocks=n_mpairs;
   nblock_size=mpsize;
 }
 
 
 
 //----------------------------------------------------------------------------------------
 //---get decoded crystal combinations for specific sequential block of modul pair
 //---arrays ic1 and ic2 must exist and have length mpsize!
 //----------------------------------------------------------------------------------------
 void BrainPETbasics_ABC::get_crystal_combis_for_modulpair(int impair, int* ic1, int *ic2)
 {
   if ((ic1==NULL) || (ic2==NULL))
     {
       cout << "BrainPETbasics_ABC::get_crystal_combis_for_block:  Error: ";
       cout << "Invalid/missing array pointers!" << endl;
       exit(0);
     }
   if ((impair<0) || (impair>=n_mpairs))
     {
       cout << "BrainPETbasics_ABC::get_crystal_combis_for_block:  Error: ";
       cout << "Invalid sequential block number requested!" << endl;
       cout << "impair=" << impair << endl;
       cout << "imax < " << n_mpairs << endl;
       exit(0);
     }
   for (int ii=0; ii<mpsize; ii++)
     {
       ic1[ii]=-1;
       ic2[ii]=-1;
     }
 
 
   int amod,bmod;
   amod=mpair_a[impair];
   bmod=mpair_b[impair];
 
   //---evaluate all crystal combinations
   int ncount=0;
   for (int ax=0; ax<mod_nx; ax++)
     {
       for (int ay=0; ay<mod_ny; ay++) 
           {
             int index_a=((ay/12)*144+12*(11-ax)+(11-(ay%12)))*864;
             
             for (int bx=0; bx<mod_nx; bx++)
               {
                 for (int by=0; by<mod_ny; by++) 
                     {
                       int idx=((by/12)*144+12*(11-bx)+(11-(by%12)))+index_a;
                       if (ic1[idx]!=-1)
                         {
                           cout << "BrainPETbasics_ABC::get_crystal_combis_for_block:  Error: ";
                           cout << "Invalid access! Bug in decoder!" << endl;
                           exit(0);
                         }
                       ic1[idx]=get_crystal_index(amod,ax,ay);
                       ic2[idx]=get_crystal_index(bmod,bx,by);
                       ncount++;
                     }
               }
           }
     }
   if (ncount!=mpsize)
     {
       cout << "BrainPETbasics_ABC::get_crystal_combis_for_block:  Error: ";
       cout << "Invalid access! Bug in decoder! (exception 2)" << endl;
       exit(0);      
     }
 }
 
 
 //----------------------------------------------------------------------------------------
 //---get crystal coordinates
 //----------------------------------------------------------------------------------------
 point3D_typ BrainPETbasics_ABC::crystal_coordinates(int index)
 {
   if ((index<0) || (index>=ncrystals))
     {
       cout << "BrainPETbasics_ABC::crystal_coordinates:  Error: ";
       cout << "invalid crystal index!" << endl;
       exit(0);
     }
   return cpoint[index];
 }
 
 
 //----------------------------------------------------------------------------------------
 //---Siemens convention not used for PRESTO virtual scanner approach
 //---get crystal coordinates for index=[0..number_of_crystals[ 
 //----------------------------------------------------------------------------------------
 point3D_typ BrainPETbasics_ABC::crystal_coordinates_SiemensConvention(int index)
 {
   if ((index<0) || (index>=ncrystals))
     {
       cout << "BrainPETbasics_ABC::crystal_coordinates_SiemensConvention:  Error: ";
       cout << "invalid crystal index!" << endl;
       exit(0);
     }
   return cpoint_SiemensConvention[index];
 }
 
 
 //----------------------------------------------------------------------------------------
 //---get crystal coordinates
 //---iaxial      -> relative ring number with respect to block
 //---itransaxial -> relative crystal number with respect to block
 //----------------------------------------------------------------------------------------
 point3D_typ BrainPETbasics_ABC::crystal_coordinates(int ihead, int iblock, 
                                                                 int iaxial, int itransaxial)
 {
   int index=ihead*(mod_nrings*mod_nx*mod_nx)+
             iblock*(mod_nx*mod_nx)+
             iaxial*mod_nx+
             itransaxial;
   
   if ((index<0) || (index>=ncrystals))
     {
       cout << "BrainPETbasics_ABC::crystal_coordinates:  Error: ";
       cout << "invalid crystal index!" << endl;
       exit(0);
     }
   return cpoint[index];
 }
 
 //----------------------------------------------------------------------------------------
 //---get crystal coordinates
 //----------------------------------------------------------------------------------------
 point3D_typ BrainPETbasics_ABC::crystal_coordinates(crystal_pos_typ &cpos)
 {
   int index=get_crystal_index(cpos);
   return cpoint[index];
 }
 
 
 //----------------------------------------------------------------------------------------
 //---get crystal address
 //----------------------------------------------------------------------------------------
 crystal_pos_typ BrainPETbasics_ABC::get_crystal_address(int index)
 {
   if ((index<0) || (index>=ncrystals))
     {
       cout << "BrainPETbasics_ABC::get_crystal_address:  Error: ";
       cout << "Bad index!" << endl;
       exit(0);
     }
   return icrystal_adr[index];
 }
 
 //----------------------------------------------------------------------------------------
 //---get crystal address
 //---for index=[0..number_of_crystals[ 
 //---crystal_posII:   icrystal_on_ring=[0..12*32[;  iring=[0..72[;  
 //----------------------------------------------------------------------------------------
 crystal_pos_typII BrainPETbasics_ABC::get_crystal_address_typII(int index)
 {
   if ((index<0) || (index>=ncrystals))
     {
       cout << "BrainPETbasics_ABC::get_crystal_address(2):  Error: ";
       cout << "Bad index!" << endl;
       exit(0);
     }
   return icrystal_adr_typII[index];
 }
 
 
 //----------------------------------------------------------------------------------------
 //---decode address to ringID=[0..72[ & crystalID=[0..12*32=384[
 //----------------------------------------------------------------------------------------
 crystal_pos_typII BrainPETbasics_ABC::convert_address(crystal_pos_typ &crystal_pos)
 {
   crystal_pos_typII cposII;
   cposII.icrystal_on_ring=get_crystalID_on_ring(crystal_pos);
   cposII.iring=get_ringID(crystal_pos);
   return cposII;
 }
 
 
 //----------------------------------------------------------------------------------------
 //---decode address from ringID=[0..72[ & crystalID=[0..12*32=384[ to blockwise addressing
 //----------------------------------------------------------------------------------------
 crystal_pos_typ BrainPETbasics_ABC::convert_address(crystal_pos_typII &crystal_pos)
 {
   crystal_pos_typ cpos;
   cpos.ihead=crystal_pos.icrystal_on_ring / mod_nx;
   cpos.itransaxial_crystal=crystal_pos.icrystal_on_ring % mod_nx;
 
   cpos.iblock=crystal_pos.iring / mod_nx;
   cpos.iaxial_crystal=crystal_pos.iring % mod_nx;
   return cpos;
 }
 
 
 
 //----------------------------------------------------------------------------------------
 //---get unique crystal position in terms of unique transaxial crystal number [0..12x32=384[
 //----------------------------------------------------------------------------------------
 int BrainPETbasics_ABC::get_crystalID_on_ring(crystal_pos_typ &crystal_pos)
 {
   if ((crystal_pos.ihead<0) || (crystal_pos.ihead>=n_mods) || 
       (crystal_pos.itransaxial_crystal<0)  || (crystal_pos.itransaxial_crystal>=mod_nx))
     {
       cout << "BrainPETbasics_ABC::get_crystalID_on_ring:  Error: ";
       cout << "Invalid address!" << endl;
       exit(0);
     }
 
   int icrystal=crystal_pos.ihead*mod_nx+crystal_pos.itransaxial_crystal;
   if ((icrystal<0) || (icrystal>=ncrystals_per_ring))
     {
       cout << "BrainPETbasics_ABC::get_crystalID_on_ring:  Error: ";
       cout << "Unexpected index obtained!" << endl;
       cout << "icrystal=" << icrystal <<  "  " << endl;
       exit(0);
     }
   return (icrystal);
 }
 
 
 //----------------------------------------------------------------------------------------
 //---get unique crystal position in terms of unique transaxial crystal number [0..12x32=384[
 //---ihead -> modul number [0..32[
 //---itransaxial -> relative crystal number with respect to block
 //----------------------------------------------------------------------------------------
 int BrainPETbasics_ABC::get_crystalID_on_ring(int ihead, int itransaxial)
 {
   if ((ihead<0) || (ihead>=n_mods) || 
       (itransaxial<0)  || (itransaxial>=mod_nx))
     {
       cout << "BrainPETbasics_ABC::get_crystalID_on_ring:  Error: ";
       cout << "Invalid address!" << endl;
       exit(0);
     }
 
   int icrystal=ihead*mod_nx+itransaxial;
   return (icrystal);
 }
 
 
 //----------------------------------------------------------------------------------------
 //---get unique ring number  [0..72[
 //----------------------------------------------------------------------------------------
 int BrainPETbasics_ABC::get_ringID(crystal_pos_typ &crystal_pos)
 {
   if ((crystal_pos.iblock<0) || (crystal_pos.iblock>=mod_nrings) ||
       (crystal_pos.iaxial_crystal<0)  || (crystal_pos.iaxial_crystal>=mod_nx)) // (! mod_nx) 
     {
       cout << "BrainPETbasics_ABC::get_ringID:  Error: ";
       cout << "Invalid address!" << endl;
       exit(0);
     }
   
   int iring=crystal_pos.iblock*mod_nx + crystal_pos.iaxial_crystal;
   return iring;
 }
 
 //----------------------------------------------------------------------------------------
 //---convert ringID in true ringID (considering gaps)
 //----------------------------------------------------------------------------------------
 int BrainPETbasics_ABC::get_true_ringID(int iringID)
 {
   if ((iringID<0) || (iringID>=mod_ny))
     {
       cout << "BrainPETbasics_ABC::get_true_ringID:  Error: ";
       cout << "Invalid ring id!" << endl;
       exit(0);
     }
   return true_ringID[iringID];
 }
 
 
 //----------------------------------------------------------------------------------------
 //---get number of crystals
 //----------------------------------------------------------------------------------------
 int BrainPETbasics_ABC::number_of_crystals(void)
 {
   return ncrystals;
 }
 
 
 //----------------------------------------------------------------------------------------
 //---some basics properties
 //----------------------------------------------------------------------------------------
 int BrainPETbasics_ABC::get_number_of_heads(void)
 {
  return n_mods;
 }
 
 //----------------------------------------------------------------------------------------
 int BrainPETbasics_ABC::get_nblocks_per_head(void)
 {
   if (mod_nrings!=6)
     {
       cout << "BrainPETbasics_ABC::get_nblocks_per_head:  Error: ";
       cout << "unexpected number!" << endl;
       exit(0);
     }
   return mod_nrings;
 }
 
 //----------------------------------------------------------------------------------------
 int BrainPETbasics_ABC::get_nrystals_per_block_axial(void)
 {
   int nn=mod_ny/mod_nrings;
   if (nn!=12)
     {
       cout << "BrainPETbasics_ABC::get_nrystals_per_block_axial:  Error: ";
       cout << "unexpected number!" << endl;
       exit(0);
     }
   return nn;
 }
 
 //----------------------------------------------------------------------------------------
 int BrainPETbasics_ABC::get_nrystals_per_block_transaxial(void)
 {
   if (mod_nx!=12)
     {
       cout << "BrainPETbasics_ABC::get_nrystals_per_block_transaxial:  Error: ";
       cout << "unexpected number!" << endl;
       exit(0);
     }
   return mod_nx;
 }
 
 
 
 
 //-----------------------------------------------------------------------------------------------------
 //---create LORfile reader (either for short int or float data)
 //---Note, the created instance goes beyond this scope, so take care of ownership
 //-----------------------------------------------------------------------------------------------------
 BrainPET_LORfileData* BrainPETbasics_ABC::get_LORfile_reader(bool use_floats)
 {
   cout << "BrainPETbasics_ABC::get_LORfile_reader:  Info: ";
   cout << "create LORfile reader for ";
   if (use_floats)
     {
       cout << "FLOATs" << endl;
     }
   else
     {
       cout << "SHORT INTs" << endl;
     }
   BrainPET_LORfileData* reader=new BrainPET_LORfileData(n_mpairs, mpsize,use_floats);
   return reader;
 }
 
 
 
 //----------------------------------------------------------------------------------------
 //----------------------------------------------------------------------------------------
 BrainPETbasics_ABC::~BrainPETbasics_ABC(void)
 { 
   delete[] mpair_a;
   delete[] mpair_b;
   delete[] cpoint;
   delete[] cpoint_SiemensConvention;
   delete[] icrystal_adr;
   delete[] icrystal_adr_typII;
   delete[] true_ringID;
 
 
   delete[] decoder_lookup;
   delete mod_pairID;
 }
 
//Generated on Tue Jun 18 17:17:16 2013 for *** PRESTO *** Pet Reconstruction Software TOolbox *** revision 333 *** by  ￼ 1.5.6
