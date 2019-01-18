
 #include "BrainPET_QuickConverter.hh"
 
 
 //----------------------------------------------------------------------------------------
 //---Constructor I
 //----------------------------------------------------------------------------------------
 BrainPET_QuickConverter::BrainPET_QuickConverter(void)
 {
   //---number of LORs in LOR file
   nlors=0;
 
   //---number of bins in sinogram file
   nsino_bins=0;
 
   //---quick lookup
   lookup_fast=NULL;
 
   Sino_Data_float=NULL;
   Sino_Data_short=NULL;
   LOR_Data_float=NULL;
   LOR_Data_short=NULL;
   Sino_Data_IS_FLOAT=true;
   LOR_Data_IS_FLOAT=true;
 
   dwell_map=NULL;
 
 } 
 
 //----------------------------------------------------------------------------------------
 //---Constructor II
 //---create Converter instance from file 
 //----------------------------------------------------------------------------------------
 BrainPET_QuickConverter::BrainPET_QuickConverter(string converter_filename)
 {
   cout << "BrainPET_QuickConverter::BrainPET_QuickConverter:  Info: " << endl;
   cout << "load: " << converter_filename << endl;
 
   //---number of LORs in LOR file
   nlors=0;
 
   //---number of bins in sinogram file
   nsino_bins=0;
 
   //---quick lookup
   lookup_fast=NULL;
 
   Sino_Data_IS_FLOAT=true;
   LOR_Data_IS_FLOAT=true;
 
   dwell_map=NULL;
 
   FileAccess f(converter_filename);
   if (!f.exists())
     {
       cout << "BrainPET_QuickConverter::BrainPET_QuickConverter:  Error: ";
       cout << "lookup file not found  f= " << converter_filename << endl;
     }
   f.open_readonly();
   load_from_stream(&f);
   
 
   cout << "BrainPET_QuickConverter::BrainPET_QuickConverter:  Info: " << endl;
   cout << "lookup table loaded successfully!" << endl;
   cout << "fname=" << converter_filename << endl;
   cout << "Nlors=" << nlors << endl;
   cout << "nsino bins=" << nsino_bins << endl;
 
   Sino_Data_float=new float[nsino_bins];
   Sino_Data_short=new short int[nsino_bins];
   LOR_Data_float=new float[nlors];
   LOR_Data_short=new short int[nlors];
 
   generate_dwell_map();
  
 }
 
 //----------------------------------------------------------------------------------------
 //---load lookup table in fast format
 //----------------------------------------------------------------------------------------
 void BrainPET_QuickConverter::load_from_stream(StreamReader* stream)
 {
  if (stream->isReadable())
     {
       unsigned short int ID;
       stream->read(ID);
       if (ID==ID_BrainPET_QuickConverter)
         {
           unsigned short int format_version;
           stream->read(format_version);
           if (format_version==1)
             {
                 
                 stream->read(nlors);
                 stream->read(nsino_bins);
 
                 if (lookup_fast!=NULL) 
                     delete[] lookup_fast;
                 lookup_fast=new int[nlors];
 
                 stream->readBlock((char*)lookup_fast,sizeof(int)*nlors);
                
               unsigned short int endtag;
               stream->read(endtag);
               if (ID_EndTag!=endtag)
                 {
                   cout << "BrainPET_QuickConverter::BrainPET_QuickConverter:  Error: " << endl;
                   cout << "End Tag not identified correctly!" << endl;
                   exit(0);
                 }
             }
           else
             {
               cout << "BrainPET_QuickConverter::BrainPET_QuickConverter:  Error: " << endl;
               cout << "Unknown Format Version! Version=" << format_version << endl;
               exit(0);
             }
         }
      else
         {
           cout << "BrainPET_QuickConverter::BrainPET_QuickConverter:  Error: " << endl;
           cout << "FileStream data do not match! Stream was closed!" << endl;
           exit(0);
         }
     }
   else
     {
       cout << "BrainPET_QuickConverter::BrainPET_QuickConverter:  Error: " << endl;
       cout << "FileStream not readable!" << endl;
       exit(0);
     }
 }
 
 
 //----------------------------------------------------------------------------------------
 //---convert flat sinogram file into flat LOR file
 //----------------------------------------------------------------------------------------
 void BrainPET_QuickConverter::convert_Sino2LOR(string sino_fname, string lor_fname,
                                                          bool output_is_float)
 {
   
 
   if ((nlors==0) || (nsino_bins==0))
     {
       cout << "BrainPET_QuickConverter::convert_Sino2LOR:  Error: " << endl;
       cout << "no lookup table available!" << endl;
       exit(0);
     }
 
   FileAccess fin(sino_fname);
   if (!fin.exists())
     {
       cout << "BrainPET_QuickConverter::convert_Sino2LOR:  Error: " << endl;
       cout << "file not found; filename: " << sino_fname << endl;
       exit(0);
     }
   cout << "BrainPET_QuickConverter::convert_Sino2LOR:  Info: " << endl;
   cout << "load: " << sino_fname << endl;
 
   //---check sino file size
   int size=fin.size();
   if (size==(int)(nsino_bins*sizeof(short int)))
     {
       //---Load short sinogram
       fin.open_readonly(true);
       fin.readBlock((char*)Sino_Data_short,sizeof(short int)*nsino_bins);
       fin.close();
       Sino_Data_IS_FLOAT=false;      
     }
   else if (size==(int)(nsino_bins*sizeof(float)))
     {
       //---Load short sinogram
       fin.open_readonly(true);
       fin.readBlock((char*)Sino_Data_float,sizeof(float)*nsino_bins);
       fin.close();
       Sino_Data_IS_FLOAT=true;      
     }
   else
     {
       cout << "BrainPET_QuickConverter::convert_Sino2LOR:  Error: " << endl;
       cout << "file has unexpected size; filename: " << sino_fname << endl;
       cout << "size=" << size << endl;
       exit(0);      
     }
 
   LOR_Data_IS_FLOAT=output_is_float;
   if ((!LOR_Data_IS_FLOAT) && (Sino_Data_IS_FLOAT))
     {
       cout << "BrainPET_QuickConverter::convert_Sino2LOR:  Error: " << endl;
       cout << "conversion FLOAT to SHORT not supported due to bad accuracy!" << endl;
       exit(0);
     }
   convert_sino2LOR();
 
 
   
   FileAccess fout(lor_fname);
   if (fout.exists())
     {
       cout << "BrainPET_QuickConverter::convert_Sino2LOR:  Warning: " << endl;
       cout << "file exits & will be overwriten; file: " << lor_fname << endl;
     }
 
   cout << "BrainPET_QuickConverter::convert_Sino2LOR:  Info: " << endl;
   cout << "store: " << lor_fname << endl;
 
   fout.open_writeonly(true);
 
   if (LOR_Data_IS_FLOAT)
     {
       fout.writeBlock((char*)LOR_Data_float,sizeof(float)*nlors);
       double sum=0.0;
       float *d=LOR_Data_float;
       for (int i=0; i<nlors; i++, d++)
           {           
             sum+=*d;
           }
       cout << "BrainPET_QuickConverter::convert_Sino2LOR:  Info: " << endl;
       cout << "LOR FLOAT DATA SUM=" << sum << endl;
     }
   else
     {
       fout.writeBlock((char*)LOR_Data_short,sizeof(short int)*nlors);
     }
   fout.close();
 }
 
 
 //----------------------------------------------------------------------------------------
 //---convert flat LOR file into flat sinogram file
 //----------------------------------------------------------------------------------------
 void BrainPET_QuickConverter::convert_LOR2sino(string lor_fname, string sino_fname,
                                                          bool dwell_correction,
                                                          bool output_is_float)
 {
   if ((nlors==0) || (nsino_bins==0))
     {
       cout << "BrainPET_QuickConverter::convert_LOR2sino:  Error: " << endl;
       cout << "no lookup table available!" << endl;
       exit(0);
     }
 
   FileAccess fin(lor_fname);
   if (!fin.exists())
     {
       cout << "BrainPET_QuickConverter::convert_LOR2sino:  Error: " << endl;
       cout << "file not found; filename: " << lor_fname << endl;
       exit(0);
     }
   
   cout << "BrainPET_QuickConverter::convert_LOR2Sino:  Info: " << endl;
   cout << "load: " << lor_fname << endl;
 
 
   //---check sino file size
   int size=fin.size();
   if (size==(int)(nlors*sizeof(short int)))
     {
       //---Load short LOR
       fin.open_readonly(true);
       fin.readBlock((char*)LOR_Data_short,sizeof(short int)*nlors);
       fin.close();
       LOR_Data_IS_FLOAT=false;      
     }
   else if (size==(int)(nlors*sizeof(float)))
     {
       //---Load short sinogram
       fin.open_readonly(true);
       fin.readBlock((char*)LOR_Data_float,sizeof(float)*nlors);
       fin.close();
       LOR_Data_IS_FLOAT=true;      
     }
   else
     {
       cout << "BrainPET_QuickConverter::convert_LOR2sino:  Error: " << endl;
       cout << "file has unexpected size; filename: " << lor_fname << endl;
       cout << "size=" << size << endl;
       exit(0);      
     }
 
   Sino_Data_IS_FLOAT=output_is_float;
   if ((LOR_Data_IS_FLOAT) && (!Sino_Data_IS_FLOAT))
     {
       cout << "BrainPET_QuickConverter::convert_LOR2sino:  Error: " << endl;
       cout << "conversion FLOAT to SHORT not supported due to bad accuracy!" << endl;
       exit(0);
     }
   convert_LOR2sino();
 
   //---optional dwell correction
   cout << "BrainPET_QuickConverter::convert_LOR2sino:  Info: " << endl;
   if (!dwell_correction)
     {
       cout << "NO dwell correction for sinograms!" << endl;
     }
   else
     {
       cout << "APPLY dwell correction for sinograms!" << endl;
       if (!Sino_Data_IS_FLOAT)
           {
             cout << "BrainPET_QuickConverter::convert_LOR2sino:  Error: " << endl;
             cout << "Requested dwell correction on short not supported due to accuracy!" << endl;
             exit(0);
           }
       float* sdata=Sino_Data_float;
       
 
       for (int i=0; i<nsino_bins; i++)
           {
             if (dwell_map[i]>0)
               sdata[i]  /= float(dwell_map[i]);
           }
     }      
 
 
 
   cout << "store: " << sino_fname << endl;
   FileAccess fout(sino_fname);
   if (fout.exists())
     {
       cout << "BrainPET_QuickConverter::convert_LOR2sino:  Warning: " << endl;
       cout << "file exits & will be overwriten; file: " << sino_fname << endl;
     }
   fout.open_writeonly(true);
   if (Sino_Data_IS_FLOAT)
     {
       fout.writeBlock((char*)Sino_Data_float,sizeof(float)*nsino_bins);
     }
   else
     {
       fout.writeBlock((char*)Sino_Data_short,sizeof(short int)*nsino_bins);
     }
   fout.close();
 }
 
 
 
 
 
 //----------------------------------------------------------------------------------------
 //---convert flat LOR data into flat sinogram file
 //----------------------------------------------------------------------------------------
 void BrainPET_QuickConverter::convert_LOR2sino(float* lordata, int nlordata,
                                                          float* sinodata, int nsinodata,
                                                          bool dwell_correction)
 {
   if (lookup_fast==NULL)
     {
       cout << "BrainPET_QuickConverter::convert_LOR2sino:  Error: ";
       cout << "no lookup table existing!" << endl;
       exit(0);
     }
 
   if ((lordata==NULL) || (sinodata==NULL))
     {
       cout << "BrainPET_QuickConverter::convert_LOR2sino:  Error: ";
       cout << "Missing data space(s)!" << endl;
       exit(0);
     }
   if (nlordata!=nlors)
     {
       cout << "BrainPET_QuickConverter::convert_LOR2sino:  Error: ";
       cout << "LOR space mismatch!" << endl;
       exit(0);
     }
   if (nsinodata!=nsino_bins)
     {
       cout << "BrainPET_QuickConverter::convert_LOR2sino:  Error: ";
       cout << "SINOGRAM space mismatch!" << endl;
       exit(0);
     }
 
   int* Look=lookup_fast;
   int Nx=nlors/8;
 
   cout << "BrainPET_QuickConverter::convert_LOR2sino:  Info: " << endl;
   cout << "start conversion...";
   cout << "FLOAT --> FLOAT" << endl;
 
   float* sino_data=sinodata;  
   memset(sino_data,0,sizeof(float)*nsino_bins);  
   float* lor_data=lordata;      
   for (int i=0; i<Nx; i++, lor_data+=8, Look+=8)
     {
       sino_data[ *(Look)   ] +=  *(lor_data);
       sino_data[ *(Look+1) ] +=  *(lor_data+1);
       sino_data[ *(Look+2) ] +=  *(lor_data+2);
       sino_data[ *(Look+3) ] +=  *(lor_data+3);
       sino_data[ *(Look+4) ] +=  *(lor_data+4);
       sino_data[ *(Look+5) ] +=  *(lor_data+5);
       sino_data[ *(Look+6) ] +=  *(lor_data+6);
       sino_data[ *(Look+7) ] +=  *(lor_data+7);
     }
 
   if (!dwell_correction)
     {
       cout << "NO dwell correction for sinograms!" << endl;
     }
   else
     {
       cout << "APPLY dwell correction for sinograms!" << endl;     
       for (int i=0; i<nsino_bins; i++)
           {
             if (dwell_map[i]>0)
               sino_data[i]  /= float(dwell_map[i]);
           }
     }      
 }
 
 
 //----------------------------------------------------------------------------------------
 //---convert flat LOR file into flat sinogram file
 //---specific treatment of data to calculate an effective normalisation
 //---instead of norm factors N the efficiencies (1/N) have to be added (!)
 //---output is float anyway
 //----------------------------------------------------------------------------------------
 void BrainPET_QuickConverter::convert_LOR2sino_norm(string lor_fname, string sino_fname, 
                                                                 bool dwell_correction)
 {
   if ((nlors==0) || (nsino_bins==0))
     {
       cout << "BrainPET_QuickConverter::convert_LOR2sino_norm:  Error: " << endl;
       cout << "no lookup table available!" << endl;
       exit(0);
     }
 
   FileAccess fin(lor_fname);
   if (!fin.exists())
     {
       cout << "BrainPET_QuickConverter::convert_LOR2sino_norm:  Error: " << endl;
       cout << "file not found; filename: " << lor_fname << endl;
       exit(0);
     }
   
   cout << "BrainPET_QuickConverter::convert_LOR2sino_norm:  Info: " << endl;
   cout << "load: " << lor_fname << endl;
 
 
   //---check sino file size
   int size=fin.size();
   if (size==(int)(nlors*sizeof(short int)))
     {
       //---short LOR
       cout << "BrainPET_QuickConverter::convert_LOR2sino_norm:  Error: " << endl;
       cout << "file has short int; others than float not reasonable " << lor_fname << endl;
       exit(0);
     }
   else if (size==(int)(nlors*sizeof(float)))
     {
       //---Load short sinogram
       fin.open_readonly(true);
       fin.readBlock((char*)LOR_Data_float,sizeof(float)*nlors);
       fin.close();
       LOR_Data_IS_FLOAT=true;      
     }
   else
     {
       cout << "BrainPET_QuickConverter::convert_LOR2sin_norm:  Error: " << endl;
       cout << "file has unexpected size; filename: " << lor_fname << endl;
       cout << "size=" << size << endl;
       exit(0);      
     }
 
   Sino_Data_IS_FLOAT=true;
 
   //---instead of norm factors N the efficiencies (1/N) have to be added (!)
   //---output is float anyway
   int nnegative=0;
   int nzero=0;
   int npositive=0;
   for (int i=0; i<nlors; i++)
     {      
       if (LOR_Data_float[i]<0.0)
           {
             nnegative++;
             LOR_Data_float[i]=0.0;
           }
       else if (LOR_Data_float[i]==0.0)
           {
             nzero++;            
           }
       else
           {
             npositive++;
             float val=LOR_Data_float[i];
             LOR_Data_float[i]=1.0/val;
           }         
     }
 
   cout << "BrainPET_QuickConverter::convert_LOR2sino_norm:  Info: " << endl;
   cout << "nnegative=" << nnegative << endl;
   cout << "nzero=" << nzero << endl;
   cout << "npositive=" << npositive << endl;
 
   convert_LOR2sino();
 
   //---now convert sino data --> 1/bin
   float* sdata=Sino_Data_float;
   for (int i=0; i<nsino_bins; i++)
     {
       if (sdata[i]>1E-9)
           {
             sdata[i]=1.0/sdata[i];
           }
       else
           {
             sdata[i]=0.0;
           }
     }
 
   //---optional dwell correction
   if (!dwell_correction)
     {
       cout << "NO dwell correction for sinograms!" << endl;
     }
   else
     {
       cout << "APPLY dwell correction for sinograms!" << endl;
       for (int i=0; i<nsino_bins; i++)
           {
             if (dwell_map[i]>0)
               sdata[i]  /= float(dwell_map[i]);
           }
     }      
 
 
 
   cout << "store: " << sino_fname << endl;
   FileAccess fout(sino_fname);
   if (fout.exists())
     {
       cout << "BrainPET_QuickConverter::convert_LOR2sino_norm:  Warning: " << endl;
       cout << "file exits & will be overwriten; file: " << sino_fname << endl;
     }
   fout.open_writeonly(true);
   fout.writeBlock((char*)Sino_Data_float,sizeof(float)*nsino_bins);
   fout.close();
 
 }
 
 
 //----------------------------------------------------------------------------------------
 //---convert flat sinogram file into flat LOR data
 //----------------------------------------------------------------------------------------
 void BrainPET_QuickConverter::convert_Sino2LOR(float* sinodata, int nsinodata,
                                                          float* lordata, int nlordata)
 {
   if (lookup_fast==NULL)
     {
       cout << "BrainPET_QuickConverter::convert_Sino2LOR:  Error: ";
       cout << "no lookup table existing!" << endl;
       exit(0);
     }
 
   if ((lordata==NULL) || (sinodata==NULL))
     {
       cout << "BrainPET_QuickConverter::convert_Sino2LOR:  Error: ";
       cout << "Missing data space(s)!" << endl;
       exit(0);
     }
   if (nlordata!=nlors)
     {
       cout << "BrainPET_QuickConverter::convert_Sino2LOR:  Error: ";
       cout << "LOR space mismatch!" << endl;
       exit(0);
     }
   if (nsinodata!=nsino_bins)
     {
       cout << "BrainPET_QuickConverter::convert_Sino2LOR:  Error: ";
       cout << "SINOGRAM space mismatch!" << endl;
       exit(0);
     }
 
   int* Look=lookup_fast;
   int Nx=nlors/8;
 
   cout << "BrainPET_QuickConverter::convert_Sino2LOR:  Inof: ";
   cout << "start conversion...";
   cout << "FLOAT --> FLOAT" << endl;
 
   float* sino_data=sinodata;  
   float* lor_data=lordata;      
   for (int i=0; i<Nx; i++, lor_data+=8, Look+=8)
     {
       *(lor_data  ) = sino_data[ *(Look)   ];
       *(lor_data+1) = sino_data[ *(Look+1) ];
       *(lor_data+2) = sino_data[ *(Look+2) ];
       *(lor_data+3) = sino_data[ *(Look+3) ];
       *(lor_data+4) = sino_data[ *(Look+4) ];
       *(lor_data+5) = sino_data[ *(Look+5) ];
       *(lor_data+6) = sino_data[ *(Look+6) ];
       *(lor_data+7) = sino_data[ *(Look+7) ];
     }
 }
 
 
 //----------------------------------------------------------------------------------------
 //---convert from LOR file to sinogram file (as flat data only)
 //----------------------------------------------------------------------------------------
   void BrainPET_QuickConverter::convert_LOR2sino(void)
 {
   if (lookup_fast==NULL)
     {
       cout << "BrainPET_QuickConverter::convert_LOR2sino:  Error: ";
       cout << "no lookup table existing!" << endl;
       exit(0);
     }
 
   int* Look=lookup_fast;
   int Nx=nlors/8;
 
   cout << "BrainPET_QuickConverter::convert_LOR2sino:  Info: " << endl;
   cout << "start conversion...";
   if (Sino_Data_IS_FLOAT)
     {
       float* sino_data=Sino_Data_float;
       memset(sino_data,0,sizeof(float)*nsino_bins);
 
 
       if (LOR_Data_IS_FLOAT)
         {
 
           cout << "FLOAT --> FLOAT" << endl;
           //---FLOAT --> FLOAT
           float* lor_data=LOR_Data_float;
 
           for (int i=0; i<Nx; i++, lor_data+=8, Look+=8)
             {
               sino_data[ *(Look)   ] +=  *(lor_data);
               sino_data[ *(Look+1) ] +=  *(lor_data+1);
               sino_data[ *(Look+2) ] +=  *(lor_data+2);
               sino_data[ *(Look+3) ] +=  *(lor_data+3);
               sino_data[ *(Look+4) ] +=  *(lor_data+4);
               sino_data[ *(Look+5) ] +=  *(lor_data+5);
               sino_data[ *(Look+6) ] +=  *(lor_data+6);
               sino_data[ *(Look+7) ] +=  *(lor_data+7);
             }
         }
       else
         {
           //---SHORT --> FLOAT
           cout << "SHORT --> FLOAT" << endl;
           short int* lor_data=LOR_Data_short;
           for (int i=0; i<Nx; i++, lor_data+=8, Look+=8)
             {
               sino_data[ *(Look)   ] += (float)(*(lor_data));
               sino_data[ *(Look+1) ] += (float)(*(lor_data+1));
               sino_data[ *(Look+2) ] += (float)(*(lor_data+2));
               sino_data[ *(Look+3) ] += (float)(*(lor_data+3));
               sino_data[ *(Look+4) ] += (float)(*(lor_data+4));
               sino_data[ *(Look+5) ] += (float)(*(lor_data+5));
               sino_data[ *(Look+6) ] += (float)(*(lor_data+6));
               sino_data[ *(Look+7) ] += (float)(*(lor_data+7));
             }
         }
     }
   else
     {
       short int* sino_data=Sino_Data_short;
       memset(sino_data,0,sizeof(short int)*nsino_bins);
 
       if (LOR_Data_IS_FLOAT)
         {
           //---FLOAT --> SHORT
           cout << "FLOAT --> SHORT" << endl;
           cout << "BrainPET_QuickConverter::convert_LOR2sino:  Warning: " << endl;
           cout << "conversion from FLOAT to SHORT possibly inaccurate!" << endl;
 
           float* lor_data=LOR_Data_float;
           for (int i=0; i<Nx; i++, lor_data+=8, Look+=8)
             {
               sino_data[ *(Look)   ] += (short int)(*(lor_data));
               sino_data[ *(Look+1) ] += (short int)(*(lor_data+1));
               sino_data[ *(Look+2) ] += (short int)(*(lor_data+2));
               sino_data[ *(Look+3) ] += (short int)(*(lor_data+3));
               sino_data[ *(Look+4) ] += (short int)(*(lor_data+4));
               sino_data[ *(Look+5) ] += (short int)(*(lor_data+5));
               sino_data[ *(Look+6) ] += (short int)(*(lor_data+6));
               sino_data[ *(Look+7) ] += (short int)(*(lor_data+7));
             }
         }
       else
         {
           //---SHORT --> SHORT
           cout << "SHORT --> SHORT" << endl;
           short int* lor_data=LOR_Data_short;
           for (int i=0; i<Nx; i++, lor_data+=8, Look+=8)
             {
               sino_data[ *(Look)   ] += (*(lor_data));
               sino_data[ *(Look+1) ] += (*(lor_data+1));
               sino_data[ *(Look+2) ] += (*(lor_data+2));
               sino_data[ *(Look+3) ] += (*(lor_data+3));
               sino_data[ *(Look+4) ] += (*(lor_data+4));
               sino_data[ *(Look+5) ] += (*(lor_data+5));
               sino_data[ *(Look+6) ] += (*(lor_data+6));
               sino_data[ *(Look+7) ] += (*(lor_data+7));
             }
         }
     }
   cout << "finished conversion..." << endl;
 }
 
 
 //----------------------------------------------------------------------------------------
 //---convert from sinogram file to LOR file (as flat data only)
 //----------------------------------------------------------------------------------------
 void BrainPET_QuickConverter::convert_sino2LOR(void)
 {
   if (lookup_fast==NULL)
     {
       cout << "BrainPET_QuickConverter::convert_sino2LOR:  Error: ";
       cout << "no lookup table existing!" << endl;
       exit(0);
     }
 
   int* Look=lookup_fast;
   int Nx=nlors/8;
 
   cout << "BrainPET_QuickConverter::convert_sino2LOR:  Info: ";
   cout << "start conversion...";
   if (Sino_Data_IS_FLOAT)
     {
       float* sino_data=Sino_Data_float;  
       if (LOR_Data_IS_FLOAT)
           {
             //---FLOAT --> FLOAT
             cout << "FLOAT --> FLOAT" << endl;
             float* lor_data=LOR_Data_float;
             memset(lor_data,0,sizeof(float)*nlors);
             for (int i=0; i<Nx; i++, lor_data+=8, Look+=8)
               {
                 *(lor_data)   = sino_data[ *(Look) ] ;
                 *(lor_data+1) = sino_data[ *(Look+1) ];  
                 *(lor_data+2) = sino_data[ *(Look+2) ];  
                 *(lor_data+3) = sino_data[ *(Look+3) ];  
                 *(lor_data+4) = sino_data[ *(Look+4) ];        
                 *(lor_data+5) = sino_data[ *(Look+5) ];  
                 *(lor_data+6) = sino_data[ *(Look+6) ];  
                 *(lor_data+7) = sino_data[ *(Look+7) ];  
               }
           }
       else
           {
             //---FLOAT --> SHORT
             short int* lor_data=LOR_Data_short;     
             memset(lor_data,0,sizeof(short int)*nlors);
 
             cout << "FLOAT --> SHORT" << endl;
             cout << "BrainPET_QuickConverter::convert_sino2LOR:  Warning: " << endl;
             cout << "conversion from FLOAT to SHORT possibly inaccurate!" << endl;
             for (int i=0; i<Nx; i++, lor_data+=8, Look+=8)
               {
                 *(lor_data)   = (short int)sino_data[ *(Look) ] ;
                 *(lor_data+1) = (short int)sino_data[ *(Look+1) ];  
                 *(lor_data+2) = (short int)sino_data[ *(Look+2) ];  
                 *(lor_data+3) = (short int)sino_data[ *(Look+3) ];  
                 *(lor_data+4) = (short int)sino_data[ *(Look+4) ];        
                 *(lor_data+5) = (short int)sino_data[ *(Look+5) ];  
                 *(lor_data+6) = (short int)sino_data[ *(Look+6) ];  
                 *(lor_data+7) = (short int)sino_data[ *(Look+7) ];  
 
               }
           }
     }
   else
     {
       short int* sino_data=Sino_Data_short;
       if (LOR_Data_IS_FLOAT)
           {
             //---SHORT --> FLOAT
             cout << "SHORT --> FLOAT" << endl;
             float* lor_data=LOR_Data_float;
             memset(lor_data,0,sizeof(float)*nlors);
             for (int i=0; i<Nx; i++, lor_data+=8, Look+=8)
               {
                 *(lor_data)   = (float)sino_data[ *(Look) ] ;
                 *(lor_data+1) = (float)sino_data[ *(Look+1) ];  
                 *(lor_data+2) = (float)sino_data[ *(Look+2) ];  
                 *(lor_data+3) = (float)sino_data[ *(Look+3) ];  
                 *(lor_data+4) = (float)sino_data[ *(Look+4) ];        
                 *(lor_data+5) = (float)sino_data[ *(Look+5) ];  
                 *(lor_data+6) = (float)sino_data[ *(Look+6) ];  
                 *(lor_data+7) = (float)sino_data[ *(Look+7) ];  
               }
           }
       else
           {
             //---SHORT --> SHORT
             cout << "SHORT --> SHORT" << endl;
             short int* lor_data=LOR_Data_short;
             memset(lor_data,0,sizeof(short int)*nlors);
             for (int i=0; i<Nx; i++, lor_data+=8, Look+=8)
               {
                 *(lor_data)   = sino_data[ *(Look) ] ;
                 *(lor_data+1) = sino_data[ *(Look+1) ];  
                 *(lor_data+2) = sino_data[ *(Look+2) ];  
                 *(lor_data+3) = sino_data[ *(Look+3) ];  
                 *(lor_data+4) = sino_data[ *(Look+4) ];        
                 *(lor_data+5) = sino_data[ *(Look+5) ];  
                 *(lor_data+6) = sino_data[ *(Look+6) ];  
                 *(lor_data+7) = sino_data[ *(Look+7) ];  
 
               }
           }
     }
   cout << "finished conversion..." << endl;
  
 }
 
 
 
 
 //----------------------------------------------------------------------------------------
 //---conversion create quick Lookup table
 //---input is the original MRPET_dump lookuptable
 //----------------------------------------------------------------------------------------
 void BrainPET_QuickConverter::create_quick_lookup(string lookupfilename)
 {
   if (lookup_fast!=NULL)
     {
       cout << "BrainPET_QuicConverter::create_quick_lookup:  Error: ";
       cout << "lookup table already existing!" << endl;
       exit(0);
     }
 
   FileAccess f(lookupfilename);
   if (!f.exists())
     {
       cout << "BrainPET_QuicConverter::create_quick_lookup:  Error: ";
       cout << "MRpet_dump lookup file not existing!" << endl;
       cout << "file= " << lookupfilename << endl; 
     }
 
   long int size0=f.size();
   long int NN=304*864*864;
   if (size0!=NN*4)
     {
       cout << "BrainPET_QuicConverter::create_quick_lookup:  Error: ";
       cout << "file length not as expected!" << endl;
       cout << "size=" << size0 << endl;
       exit(0);
     }
   if (sizeof(int)!=4)
     {
       cout << "BrainPET_QuicConverter::create_quick_lookup:  Error: ";
       cout << "Bad machine architecture!" << endl;
       exit(0);
     }
 
   nlors=NN;
   lookup_fast=new int[nlors];
   //memset(lookup_fast,0,sizeof(int)*nlors);
   for (int i=0; i<nlors; i++)
     {
       lookup_fast[i]=-1;
     }
 
   bool suppress_starttag=true;
   f.open_readonly(suppress_starttag);
   int* lorlookup=new int[NN]; 
 
   cout << "BrainPET_QuicConverter::create_quick_lookup:  Info: ";
   cout << "load lookupfile=" << lookupfilename << endl;
   f.readBlock((char*)lorlookup,sizeof(int)*NN);
 
 
   int lor_data_offset = 0;
   int lut_offset = 0;
   int index = 0;
 
   MRPET_dump* bpet=new MRPET_dump;
   int maxrd =  bpet->get_max_ringdiff();
   //int span =   bpet->get_span();
   int nPlanes= bpet->get_n_planes();
   int nViews = bpet->get_n_views(); 
   int nProjs = bpet->get_n_projs(); 
 
 
   nsino_bins=nPlanes*nViews*nProjs;
 
 
   Sino_Data_float=new float[nsino_bins];
   Sino_Data_short=new short int[nsino_bins];
   LOR_Data_float=new float[nlors];
   LOR_Data_short=new short int[nlors];
 
 
 
 
 
   //---for every of the 304 modulpairs:
   int bp_n_mpairs = bpet->get_n_mpairs();
 
   int ndouble_access=0;
   for (int i=0; i<bp_n_mpairs; i++)
     {
       int amod, bmod, ax, ay, bx, by;
       int rd; //, j;
       //float fv;
       int view;
       int proj;
       int plane;
       //int segment;
       int mod_nx = bpet->get_mod_nx();
       int mod_ny = bpet->get_mod_ny();
       
       //(amod, bmod) is combination of modulpairs in the allowed fov(19)
       amod=bpet->mpair_a[i];
       bmod=bpet->mpair_b[i];
       
       //cout << "mpair " << i << " of " << brainPET1->n_mpairs << endl;
       int verbose = 0;
       printf("progress: MP%03d of %d %c",i,304,(verbose>0)?'\n':'\r'); 
       fflush(stdout);
           
       for (ax=0; ax < mod_nx; ax++)
           {
             for (ay=0; ay < mod_ny; ay++) 
               {
                 for (bx=0; bx < mod_nx; bx++)
                     {
                       for (by=0; by < mod_ny; by++) 
                         {
                           //---ring diff
                           rd=by-ay; 
                           if (rd<0)
                               {
                                 rd=ay-by;
                               }
 
                           //---check current ring difference <= max ringdiff 
                           if (rd <=maxrd)
                               {                               
                                 //---flat index in sinogram (or projection, view, plane)                
                                 int idx1=lut_offset+(ax+ay*mod_nx)*mod_nx*mod_ny+bx+by* mod_nx;
                                 if ((idx1<0) || (idx1>=nlors))
                                   {
                                     cout << "index out of range! (1)" << endl;
                                     exit(0);
                                   }
                                   
                                 index=lorlookup[idx1];             
                                 
                                 
                                 // index_lor is position in lor array
                                 int index_lor = ((by/12)*144+12*(11-bx)+(11-(by%12)))+
                                   ((ay/12)*144+12*(11-ax)+(11-(ay%12)))*864;
                                 index_lor +=lor_data_offset;
                                 
                                 if ((index_lor<0) || (index_lor>=nlors))
                                   {
                                     cout << "index out of range! (2)" << endl;
                                     exit(0);
                                   }
 
 
                                 if ((index>=0) && (index < nsino_bins))
                                   {
                                     bpet->addr_to_view_proj_plane(index, view, proj, plane);      
                                     if ((plane < nPlanes) && (view < nViews) && (proj < nProjs) &&
                                           (plane >= 0) && (view >= 0) && (proj >= 0))
                                         {
                                           if (lookup_fast[index_lor]!=-1)
                                             {
                                               ndouble_access++;
                                               if (ndouble_access<20)
                                                   {
                                                     cout << "BrainPET_QuicConverter::create_quick_lookup:  Warning: ";
                                                     cout << "unexpected double lor addressing!" << endl;
                                                     cout << "index_lor=" << index_lor << endl;
                                                     //exit(0);
                                                   }
                                             }
                                           lookup_fast[index_lor]=index;
                                         }
                                   }
                               }
                         }
                     }
               }
           }
 
       // offset to index_lor for next module pair:
       lor_data_offset += 864*864;
       lut_offset += 864*864;
     }
 
   cout << "Ndouble accesses = " << ndouble_access << endl;
   delete[] lorlookup;
 
   generate_dwell_map();
 
   cout << "############################################################" << endl;
   cout << "BrainPET_QuicConverter::create_quick_lookup:  Warning: " << endl;
   cout << "ERROR........MRPET_dump destructor not working.....memory leak!" << endl;
   cout << "MRPET_dump destructor bug fix needed!" << endl;
   cout << "############################################################" << endl;
   //delete[] bpet;
 
 }
 
 
 
 //----------------------------------------------------------------------------------------
 //---store lookup table (create valid Converter instance)
 //----------------------------------------------------------------------------------------
 void BrainPET_QuickConverter::store_lookup_table(string fname)
 {
   cout << "BrainPET_QuickConverter::store_lookup_table: Info: " << endl;
   cout << "store: " << fname << endl;
   FileAccess f(fname);
   f.open_writeonly();
   write_into_stream(&f);
   f.close();
 }
 
 
 
 //----------------------------------------------------------------------------------------
 //---write instance into stream
 //----------------------------------------------------------------------------------------
 void BrainPET_QuickConverter::write_into_stream(StreamWriter* stream)
 {
   if ((nlors==0) || (lookup_fast==NULL))
     {
       cout << "BrainPET_QuickConverter::write_into_stream:  Error: ";
       cout << "No lookup table existing; no storage!" << endl;
       exit(0);
     }
 
   if (stream->isWriteable())
     {
       stream->write(ID_BrainPET_QuickConverter);
       unsigned short int format_version=1;
       stream->write(format_version);
 
       stream->write(nlors);
       stream->write(nsino_bins);
       stream->writeBlock((char*)lookup_fast,sizeof(int)*nlors);
 
       stream->write(ID_EndTag);
     }
   else
     {
       cout << "BrainPET_QuickConverter::write_into_file:   Error: ";
       cout << "DataStream not Writeable!" << endl;
     }
 }
 
 
 //----------------------------------------------------------------------------------------
 //---generate dwell map
 //----------------------------------------------------------------------------------------
 void BrainPET_QuickConverter::generate_dwell_map(void)
 {
   if (dwell_map==NULL)
     {
       if ((nsino_bins==0) || (lookup_fast==NULL) || (nlors==0))
           {
             cout << "BrainPET_QuickConverter::generate_dwell_map:  Error: ";
             cout << "No lookup table available!" << endl;
             exit(0);
           }
 
       dwell_map=new short int[nsino_bins];
       memset(dwell_map,0,sizeof(short int)*nsino_bins);
 
 
       cout << "BrainPET_QuickConverter::generate_dwell_map: Info:";
 
       LOR_Data_IS_FLOAT=false;
       short int* ldata=(short int*)LOR_Data_short;
       for (int i=0; i<nlors; i++)
           {
             ldata[i]=1;
           }
       Sino_Data_IS_FLOAT=false;
       convert_LOR2sino();
       memcpy(dwell_map,Sino_Data_short,sizeof(short int)*nsino_bins);
       
       cout << "created" << endl;
     }
   else
     {
       cout << "BrainPET_QuickConverter::generate_dwell_map:  Error: ";
       cout << "dwell map exists already!" << endl;
     }
   
 }
 
 
 
 //----------------------------------------------------------------------------------------
 //----------------------------------------------------------------------------------------
 BrainPET_QuickConverter::~BrainPET_QuickConverter(void)
 {
   //---quick lookup
   delete[] lookup_fast;
 
   //---temp data storage
   delete[] LOR_Data_short;
   delete[] Sino_Data_short;
   delete[] LOR_Data_float;
   delete[] Sino_Data_float;
 
 
   delete[] dwell_map;
 
 }
 
 
//Generated on Tue Jun 18 17:17:16 2013 for *** PRESTO *** Pet Reconstruction Software TOolbox *** revision 333 *** by  ￼ 1.5.6
