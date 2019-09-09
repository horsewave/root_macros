
 
 #include "BrainPET_LORfileData.hh"
 
 
 //----------------------------------------------------------------------------------------
 //---Constructor I
 //----------------------------------------------------------------------------------------
 BrainPET_LORfileData::BrainPET_LORfileData(int n_modul_pairs0, int n_modulpair_size0,
                                                      bool data_float0)
 {
   dump = NULL; //c.w.
   if ((n_modul_pairs0<=0) || (n_modulpair_size0<=0))
     {
       cout << "BrainPET_LORfileData::BrainPET_LORfileData:  Error: ";
       cout << "Invalid data dimensions!" << endl;
       exit(0);
     }
 
 
   //---number of modul pairs 
   n_mpairs=n_modul_pairs0;
 
   //---number of LORs for any modul pair
   n_mpair_size=n_modulpair_size0;
   
   //---create data storage
   ntot_channels=n_mpairs*n_mpair_size;
   data_float=data_float0;
   if (data_float)
     {
       cout << "BrainPET_LORfileData::BrainPET_LORfileData:  Info: ";
       long long int nbytes=ntot_channels*sizeof(float);
       cout << "(float) allocate=" << nbytes/1024 <<  " KB" << endl;
       data=new(nothrow)float[ntot_channels];
       if (data==NULL)
           {
             cout << "BrainPET_LORfileData::BrainPET_LORfileData:  Error: ";
             cout << "out of memory!" << endl;
             exit(0);
           }
 
       memset(data,0,sizeof(float)*ntot_channels);
       data_short=NULL;
       
       //---modul pair data pointers
       mpair_data=new float*[n_mpairs];
       for (int i=0; i<n_mpairs; i++)
           {
             mpair_data[i]=&(data[i*n_mpair_size]);
           }
       mpair_data_short=NULL;
     }
   else
     {
       long long int nbytes=ntot_channels*sizeof(short int);
       cout << "(short) allocate=" << nbytes/1024 <<  " KB" << endl;
 
       data_short=new short int[ntot_channels];
       memset(data_short,0,sizeof(short int)*ntot_channels);
       data=NULL;
 
       //---modul pair data pointers
       mpair_data_short=new short int*[n_mpairs];
       for (int i=0; i<n_mpairs; i++)
           {
             mpair_data_short[i]=&(data_short[i*n_mpair_size]);
           }
       mpair_data=NULL;
     }
     
       
   cout << "BrainPET_LORfileData::BrainPET_LORfileData:  Info: " << endl;
   cout << "Number of mpairs=" << n_mpairs << endl;
   cout << "Modul pair size=" << n_mpair_size << endl;
   int word;
   if (data_float)
     {
       cout << "DataTyp is float" << endl;
       word=sizeof(float);
     }
   else
     {
       cout << "DataTyp is short int" << endl;
       word=sizeof(short int);
     }
 
   cout << "Allocated Memory: " << ntot_channels*word/(1024*1024) << "MB" << endl;
 } 
 
 
 //----------------------------------------------------------------------------------------
 //---Constructor II 
 //----------------------------------------------------------------------------------------
 BrainPET_LORfileData::BrainPET_LORfileData(StreamReader* stream)
 {
   dump = NULL; //c.w.
   if (stream->isReadable())
     {
       unsigned short int ID;
       stream->read(ID);
       if (ID==ID_BrainPET_LORfileData)
         {
           unsigned short int format_version;
           stream->read(format_version);
           if ((format_version==1) || (format_version==2))
             {
                 stream->read(n_mpairs);
                 stream->read(n_mpair_size);
                 stream->read(ntot_channels);
                 
                 if (format_version==2)
                     {
                       stream->read(data_float);
                     }
                 else
                     {
                       data_float=true;
                     }
                 
                 if (data_float)
                     {
                       cout << "datatyp=float " << endl;
                       data=new float[ntot_channels];
                       memset(data,0,sizeof(float)*ntot_channels);
                       data_short=NULL;
                       
                       //---modul pair data pointers
                       mpair_data=new float*[n_mpairs];
                       for (int i=0; i<n_mpairs; i++)
                         {
                           mpair_data[i]=&(data[i*n_mpair_size]);
                         }
                       mpair_data_short=NULL;
                       stream->readBlock((char*)data,sizeof(float)*ntot_channels);  
                     }
                 else
                     {
                       cout << "datatyp=short int " << endl;
                       data_short=new short int[ntot_channels];
                       memset(data_short,0,sizeof(short int)*ntot_channels);
                       data=NULL;
                       
                       //---modul pair data pointers
                       mpair_data_short=new short int*[n_mpairs];
                       for (int i=0; i<n_mpairs; i++)
                         {
                           mpair_data_short[i]=&(data_short[i*n_mpair_size]);
                         }
                       mpair_data=NULL;
                       stream->readBlock((char*)data_short,sizeof(short int)*ntot_channels);
                     }
                                           
               unsigned short int endtag;
               stream->read(endtag);
               if (ID_EndTag!=endtag)
                 {
                   cout << "BrainPET_LORfileData::BrainPET_LORfileData:  Error: " << endl;
                   cout << "End Tag not identified correctly!" << endl;
                   exit(0);
                 }
             }
           else
             {
               cout << "BrainPET_LORfileData::BrainPET_LORfileData:  Error: " << endl;
               cout << "Unknown Format Version! Version=" << format_version << endl;
               exit(0);
             }
         }
      else
         {
           cout << "BrainPET_LORfileData::BrainPET_LORfileData:  Error: " << endl;
           cout << "FileStream data do not match! Stream was closed!" << endl;
           exit(0);
         }
     }
   else
     {
       cout << "BrainPET_LORfileData::BrainPET_LORfileData:  Error: " << endl;
       cout << "FileStream not readable!" << endl;
       exit(0);
     }
 }
 
 
 
 
 
 
 //----------------------------------------------------------------------------------------
 //---write instance into stream
 //----------------------------------------------------------------------------------------
 void BrainPET_LORfileData::write_into_stream(StreamWriter* stream)
 {
   if (stream->isWriteable())
     {
       stream->write(ID_BrainPET_LORfileData);
       unsigned short int format_version=2;
       stream->write(format_version);
 
       stream->write(n_mpairs);
       stream->write(n_mpair_size);
       stream->write(ntot_channels);     
       stream->write(data_float);
 
       if (data_float)
           {
             stream->writeBlock((char*)data,sizeof(float)*ntot_channels);
           }
       else
           { 
             stream->writeBlock((char*)data_short,sizeof(short int)*ntot_channels);
           }
 
       stream->write(ID_EndTag);
     }
   else
     {
       cout << "BrainPET_LORfileData::write_into_file:   Error: ";
       cout << "DataStream not Writeable!" << endl;
     }
 }
 
 
 //----------------------------------------------------------------------------------------
 //---change data status to float (in case of short int)
 //----------------------------------------------------------------------------------------
 
 void BrainPET_LORfileData::change_data_typ_to_float(void)
 {
   cout << "BrainPET_LORfileData::change_data_typ_to_float:  Info: ";
   if (data_float)
     {
       cout << "typ is already float! nothing to be done!" << endl;
       return;
     }
 
   long long int nbytes=ntot_channels*sizeof(float);
   cout << "(float) allocate=" << nbytes/1024 <<  " KB" << endl;
   data=new(nothrow)float[ntot_channels];
   if (data==NULL)
     {
       cout << "BrainPET_LORfileData::BrainPET_LORfileData:  Error: ";
       cout << "out of memory!" << endl;
       exit(0);
     }
   
   memset(data,0,sizeof(float)*ntot_channels);
 
   //---modul pair data pointers
   mpair_data=new float*[n_mpairs];
   for (int i=0; i<n_mpairs; i++)
     {
       mpair_data[i]=&(data[i*n_mpair_size]);
     }
 
   for (int i=0; i<ntot_channels; i++)
     {
       data[i]=float(data_short[i]);
     }
   
   delete[] data_short;
   delete[] mpair_data_short;
   data_short=NULL;
   mpair_data_short=NULL;
   data_float=true;
 }
 
 
 //----------------------------------------------------------------------------------------
 //---load data from LORdata file
 //----------------------------------------------------------------------------------------
 void BrainPET_LORfileData::load_data(char* filename, bool expect_floats)
 {
   cout << "BrainPET_LORfileData::load_data:  Info: filename=" << filename << endl;
 
   FileAccess f(filename);
   if (!f.exists())
     {
       cout << "BrainPET_LORfileData::load_data:  Error: ";
       cout << "File " << filename << " not found!" << endl;
       exit(0);
     }
 
   //---verify file size
   long long int size=f.size();
 
   if (expect_floats)
     {
       cout << "BrainPET_LORfileData::load_data:  Info: FLOATS expected" << endl;
       long long int size0=ntot_channels*sizeof(float);      
       if (size0!=size)
           {
             cout << "BrainPET_LORfileData::load_data:  Error: ";
             cout << "Unexpecetd file size for float data!" << endl;
             printf("size=%llu     expected size=%llu\n",size,size0);
             exit(0);
           }
     }
   else
     {
       cout << "BrainPET_LORfileData::load_data:  Info: SHORT INTs expected" << endl;
       long long int size0=ntot_channels*sizeof(short int);      
       if (size0!=size)
           {
             cout << "BrainPET_LORfileData::load_data:  Error: ";
             cout << "Unexpecetd file size for short int data!" << endl;
             printf("size=%llu     expected size=%llu\n",size,size0);
             exit(0);
           }
     }
 
   bool suppress_start_tag=true;
   f.open_readonly(suppress_start_tag);
   
   if (sizeof(short int)!=2)
     {
       cout << "BrainPET_LORfileData::load_data:  Error: ";
       cout << "Machine word length of short int not 2 bytes!" << endl;
       exit(0);
     }
 
 
 
   short int* lordata_S=NULL;
   float* lordata_F=NULL;
   unsigned int blocklength=0;
 
   if (expect_floats)
     {
       lordata_F=new float[n_mpair_size];
       blocklength=sizeof(float)*n_mpair_size;
     }
   else
     {
       lordata_S=new short int[n_mpair_size];
       blocklength=sizeof(short int)*n_mpair_size;
     }
 
   //---read data
   int verbose=0;
   double sum=0.0;
   //if (expect_floats) cout << "BrainPET_LORfileData::load_data:  Info: expect floats" << endl;
   //if (data_float) cout << "BrainPET_LORfileData::load_data:  Info: data is FLOAT" << endl;
   //int nchannels=0;
   //int nn=n_mpairs*n_mpair_size;
   for (int i=0; i<n_mpairs; i++)
     {
       if ((i%20)==0) 
           {
             printf("progress: MP%03d of %d %c",i,n_mpairs,(verbose>1)?'\n':'\r'); 
             fflush(stdout);
           }
       if (expect_floats)
           {
             f.readBlock((char*)lordata_F,blocklength);
 
             //---cast to float
             if (data_float)
               {
                 for (int k=0; k<n_mpair_size; k++)
                     {
                       mpair_data[i][k]=lordata_F[k];
                       sum+=lordata_F[k];
                       //nchannels++;
                     }
               }
             else
               {
                 cout << "BrainPET_LORfileData::load_data:  Error: ";
                 cout << "floats expected, but short ints as storage!" << endl;
                 cout << "that's not accurate!" << endl;
                 cout << "operation aborted!" << endl;
                 exit(0);
                 
                 /*
                 for (int k=0; k<n_mpair_size; k++)
                     {
                       mpair_data_short[i][k]=(short int)lordata_S[k];
                     }
                 */
               }
           }
       else
           {
             f.readBlock((char*)lordata_S,blocklength);
 
 
             //---cast to float
             if (data_float)
               {
                 for (int k=0; k<n_mpair_size; k++)
                     {
                       mpair_data[i][k]=float(lordata_S[k]);
                       sum+=float(lordata_S[k]);
                     }
               }
             else
               {           
                 for (int k=0; k<n_mpair_size; k++)
                     {
                       mpair_data_short[i][k]=lordata_S[k];
                       sum+=float(lordata_S[k]);
                     }              
               }
           }
     }
   f.close();
   cout << endl;
 
   cout << "BrainPET_LORfileData::load_data:  Info: ";
   cout << "DATASUM=" << sum << endl;
   //cout << "SUM=" << get_count_sum() << endl;
   //cout << "Nchannels=" << nn << "   filled: " << nchannels << endl;
 
   delete[] lordata_F;
   delete[] lordata_S;
 
 }
 
 
 //----------------------------------------------------------------------------------------
 //---reset all channels
 //----------------------------------------------------------------------------------------
 void BrainPET_LORfileData::set_all_channels(float value)
 {
   if (data_float)
     {
       cout << "BrainPET_LORfileData::set_all_channels:  Info: ";
       cout << "float value=" << value << endl;
       for (int i=0; i<ntot_channels; i++)
           {
             data[i]=value;
           }
     }
   else
     {
       cout << "BrainPET_LORfileData::set_all_channels:  Info: ";
       short int val=(short int)value;
       cout << "short value=" << val << endl;
       
       for (int i=0; i<ntot_channels; i++)
           {
             data_short[i]=val;
           }
     }
 }
 
 //----------------------------------------------------------------------------------------
 //---store LORfileData as short int file
 //----------------------------------------------------------------------------------------
 void BrainPET_LORfileData::store_as_LORfileData_short_int(char *fname, bool overwrite)
 {
   if (sizeof(short int)!=2)
     {
       cout << "BrainPET_LORfileData::store_as_LORfileData_short_int:  Error: ";
       cout << "Machine word length of short int not 2 bytes!" << endl;
       exit(0);
     }
 
   //---check file
   FileAccess f(fname);
   if ((f.exists()) && (!overwrite))
     {
       cout << "BrainPET_LORfileData::store_as_LORfileData_short_int:  Warning: ";
       cout << "file exists and will NOT be overwritten!" << endl;
       return;
     }
   bool suppress_tag=true;
   f.open_writeonly(suppress_tag);
 
 
   if (!data_short)
     {
       cout << "BrainPET_LORfileData::store_as_LORfileData_short_int:  Warning: ";
       cout << "original data type is FLOATs, but will be stored as SHORTs! data loss!!" << endl;
     }
 
   short int* lordata;
   lordata=new short int[n_mpair_size];
   unsigned int blocklength=sizeof(short int)*n_mpair_size;
 
 
   //---read data
   for (int i=0; i<n_mpairs; i++)
     {
       if (data_float)
           {
             float* fdata=get_modul_pair_data_float(i);
             if (fdata==NULL)
               {
                 cout << "BrainPET_LORfileData::store_as_LORfileData_short_int:  Error: ";
                 cout << "Missing data typ! (float)" << endl;
                 exit(0);
               }
             for (int j=0; j<n_mpair_size; j++)
               {
                 lordata[j]=(int(fdata[j]));
               }
           }
       else
           {
             short int* idata=get_modul_pair_data_short(i);
             if (idata==NULL)
               {
                 cout << "BrainPET_LORfileData::store_as_LORfileData_short_int:  Error: ";
                 cout << "Missing data typ! (int)" << endl;
                 exit(0);
               }
 
             for (int j=0; j<n_mpair_size; j++)
               {
                 lordata[j]=idata[j];
               }
           }
 
 
       if ((i%20)==0) 
           {
             printf("writing progress: MP%03d of %d \r",i,n_mpairs); fflush(stdout);
           }
       f.writeBlock((char*)lordata,blocklength);
     }
   f.close();
   cout << endl;
  
   delete[] lordata;
 }
 
 //----------------------------------------------------------------------------------------
 //---store LORfileData as float file
 //----------------------------------------------------------------------------------------
 void BrainPET_LORfileData::store_as_LORfileData_float(char *fname, bool overwrite)
 {
   if (sizeof(float)!=4)
     {
       cout << "BrainPET_LORfileData::store_as_LORfileData_float:  Error: ";
       cout << "Machine word length of float not 4 bytes!" << endl;
       exit(0);
     }
 
   //---check file
   FileAccess f(fname);
   if ((f.exists()) && (!overwrite))
     {
       cout << "BrainPET_LORfileData::store_as_LORfileData_float:  Warning: ";
       cout << "file exists and will NOT be overwritten!" << endl;
       return;
     }
   bool suppress_tag=true;
   f.open_writeonly(suppress_tag);
 
   float* lordata=NULL;
   int blocklength=sizeof(float)*n_mpair_size;
   if (!data_float)
     {
       lordata=new float[n_mpair_size];
     }
 
   //---check count sum
   cout << "BrainPET_LORfileData::store_as_LORfileData_float:  Info: ";
   cout << get_count_sum() << endl;
 
   //---read mpair data
   for (int i=0; i<n_mpairs; i++)
     {
       if (data_float)
           {
             float* fdata=get_modul_pair_data_float(i);
             if (fdata==NULL)
               {
                 cout << "BrainPET_LORfileData::store_as_LORfileData_float:  Error: ";
                 cout << "Missing data typ! (float)" << endl;
                 exit(0);
               }
             f.writeBlock((char*)fdata,blocklength);
           }
       else
           {
             short int* idata=get_modul_pair_data_short(i);
             if (idata==NULL)
               {
                 cout << "BrainPET_LORfileData::store_as_LORfileData_float:  Error: ";
                 cout << "Missing data typ! (int)" << endl;
                 exit(0);
               }
             if (lordata==NULL)
               {
                 cout << "BrainPET_LORfileData::store_as_LORfileData_float:  Error: ";
                 cout << "Unexpeceted missing float array!" << endl;
                 exit(0);
               }
 
             for (int j=0; j<n_mpair_size; j++)
               {
                 lordata[j]=float(idata[j]);
               }
             f.writeBlock((char*)lordata,blocklength);
           }
 
       if ((i%20)==0) 
           {
             printf("writing progress: MP%03d of %d \r",i,n_mpairs); fflush(stdout);
           }
 
     }
   f.close();
   cout << endl;
  
   delete[] lordata;
 }
 
 
 //----------------------------------------------------------------------------------------
 //---data status (either float or short int)
 //----------------------------------------------------------------------------------------
 bool BrainPET_LORfileData::data_is_float(void)
 {
   return data_float; 
 }
 
 //----------------------------------------------------------------------------------------
 //---get modul pair data (pointer float);
 //---return NULL if not float format
 //----------------------------------------------------------------------------------------
 float* BrainPET_LORfileData::get_modul_pair_data_float(int imodul_pair)
 {
   if (!data_float)
     {
       cout << "BrainPET_LORfileData::get_modul_pair_data_float:  Warning: ";
       cout << "float requested but stored as short int!" << endl;
       cout << "NULL pointer returned!" << endl;
       return NULL;
     }
   else
     {      
       if ((imodul_pair<0) || (imodul_pair>=n_mpairs))
           {
             cout << "BrainPET_LORfileData::get_modul_pair_data_float:  Error: ";
             cout << "Invalid modul pair requested!" << endl;
             exit(0);
           }
       return mpair_data[imodul_pair];
     }
 }
 
 
 //----------------------------------------------------------------------------------------
 //---get modul pair data (pointer short int)
 //----------------------------------------------------------------------------------------
 short int* BrainPET_LORfileData::get_modul_pair_data_short(int imodul_pair)
 {
   if (data_float)
     {
       cout << "BrainPET_LORfileData::get_modul_pair_data_short:  Warning: ";
       cout << "short int requested but stored as float!" << endl;
       cout << "NULL pointer returned!" << endl;
       return NULL;
     }
   else
     {      
       if ((imodul_pair<0) || (imodul_pair>=n_mpairs))
           {
             cout << "BrainPET_LORfileData::get_modul_pair_data_short:  Error: ";
             cout << "Invalid modul pair requested!" << endl;
             exit(0);
           }
       return mpair_data_short[imodul_pair];
     }
 }
 
 
 
 
 //----------------------------------------------------------------------------------------
 //---get modul pair data LOR value
 //----------------------------------------------------------------------------------------
 float BrainPET_LORfileData::get_modul_pair_LOR_value(int imodul_pair, int ilor)
 {
   if ((imodul_pair<0) || (imodul_pair>=n_mpairs))
     {
       cout << "BrainPET_LORfileData::get_modul_pair_data:  Error: ";
       cout << "Invalid modul pair requested!" << endl;
       exit(0);
     }
   if ((ilor<0) || (ilor>=n_mpair_size))
     {
       cout << "BrainPET_LORfileData::get_modul_pair_data:  Error: ";
       cout << "Invalid LOR requested within modul pair!" << endl;
       exit(0);
     }
   if (data_float)
     {
       return mpair_data[imodul_pair][ilor];
     }
   else
     {
       return (float)mpair_data_short[imodul_pair][ilor];
     }
 }
 
 
 //----------------------------------------------------------------------------------------
 //---set modul pair data LOR value
 //----------------------------------------------------------------------------------------
 void BrainPET_LORfileData::set_modul_pair_LOR_value(int imodul_pair, int ilor, float data0)
 {
   if ((imodul_pair<0) || (imodul_pair>=n_mpairs))
     {
       cout << "BrainPET_LORfileData::set_modul_pair_data:  Error: ";
       cout << "Invalid modul pair requested!" << endl;
       exit(0);
     }
   if ((ilor<0) || (ilor>=n_mpair_size))
     {
       cout << "BrainPET_LORfileData::set_modul_pair_data:  Error: ";
       cout << "Invalid LOR requested within modul pair!" << endl;
       exit(0);
     }
 
   if (data_float)
     {
       mpair_data[imodul_pair][ilor]=data0;
     }
   else
     {
       short int idata=(short int)data0;
       mpair_data_short[imodul_pair][ilor]=idata;
     } 
 }
 
 
 //----------------------------------------------------------------------------------------
 //---get min/max values
 //----------------------------------------------------------------------------------------
 void BrainPET_LORfileData::get_min_max_values(float &min, float &max)
 {
   min=+1E30;
   max=-1E30;
   float dat;
   for (int i=0; i<ntot_channels; i++)
     {
       if (data_float)
           {
             dat=data[i];
           }
       else
           {
             dat=(float)data_short[i];
           }
       if (min>dat) min=dat;
       if (max<dat) max=dat;
     }
   cout << "BrainPET_LORfileData::get_min_max_values:  Info: ";
   cout << "min=" << min << " max=" << max << endl;
 }
 
 //----------------------------------------------------------------------------------------
 //---total sum
 //----------------------------------------------------------------------------------------
 double BrainPET_LORfileData::get_count_sum(void)
 {
   
   double sum=0.0;
   if(data_float)
     {
       for (int i=0; i<ntot_channels; i++)
           {
               sum+=(double)data[i];
           }
     }
   else
     {
       long long int s=0;
       for (int i=0; i<ntot_channels; i++)
           {
             s+=data_short[i];
           }
       sum=(double)s;
     }
   return sum;
 }
 
 
 //----------------------------------------------------------------------------------------
 //---add data
 //----------------------------------------------------------------------------------------
 void BrainPET_LORfileData::add_data_from(BrainPET_LORfileData* lordata)
 {
   if (lordata==NULL)
     {
       cout << "BrainPET_LORfileData::add_data_from:  Error: ";
       cout << "missing data!" << endl;
       exit(0);
     }
 
   if ((lordata->n_mpairs!=n_mpairs) || (lordata->n_mpair_size!=n_mpair_size))
     {
       cout << "BrainPET_LORfileData::add_data_from:  Error: ";
       cout << "Data does not match!" << endl;
       exit(0);
     }
 
   if (lordata->data_float!=data_float)
     {
       cout << "BrainPET_LORfileData::add_data_from:  Error: ";
       cout << "DataTyp (short <--> float) does not match!" << endl;
       exit(0);
     }
 
   //float sum=0.0;
   if (data_float)
     {
       cout << "BrainPET_LORfileData::add_data_from:  Info: use float data" << endl;
       float* data0=lordata->data;
       if ((data0==NULL) || (data==NULL))
           {
             cout << "BrainPET_LORfileData::add_data_from:  Error: ";
             cout << "float data missing!"  << endl;
             exit(0);
           }
 
       for (int i=0; i<ntot_channels; i++)
           {
             data[i]+=data0[i];
           }
     }
   else
     {
       cout << "BrainPET_LORfileData::add_data_from:  Info: use short data" << endl;
       short int* data0=lordata->data_short;
       if ((data0==NULL) || (data_short==NULL))
           {
             cout << "BrainPET_LORfileData::add_data_from:  Error: ";
             cout << "short int data missing!"  << endl;
             exit(0);
           }
 
       for (int i=0; i<ntot_channels; i++)
           {
             data_short[i]+=data0[i];
           }
     }  
 }
 
 
 
 //----------------------------------------------------------------------------------------
 //---subtract data with of instance (negative values possible & not suppressed)
 //----------------------------------------------------------------------------------------
 void BrainPET_LORfileData::subtract(BrainPET_LORfileData* lordata)
 {
   if (lordata==NULL)
     {
       cout << "BrainPET_LORfileData::subtract:  Error: ";
       cout << "missing data!" << endl;
       exit(0);
     }
 
   if ((lordata->n_mpairs!=n_mpairs) || (lordata->n_mpair_size!=n_mpair_size))
     {
       cout << "BrainPET_LORfileData::subtract:  Error: ";
       cout << "Data does not match!" << endl;
       exit(0);
     }
 
   if (lordata->data_float!=data_float)
     {
       cout << "BrainPET_LORfileData::subtract:  Error: ";
       cout << "DataTyp (short <--> float) does not match!" << endl;
       exit(0);
     }
 
   //float sum=0.0;
   if (data_float)
     {
       cout << "BrainPET_LORfileData::subtract:  Info: use float data" << endl;
       float* data0=lordata->data;
       if ((data0==NULL) || (data==NULL))
           {
             cout << "BrainPET_LORfileData::subtract:  Error: ";
             cout << "float data missing!"  << endl;
             exit(0);
           }
 
       for (int i=0; i<ntot_channels; i++)
           {
             data[i]-=data0[i];
           }
     }
   else
     {
       cout << "BrainPET_LORfileData::subtract:  Info: use short data" << endl;
       short int* data0=lordata->data_short;
       if ((data0==NULL) || (data_short==NULL))
           {
             cout << "BrainPET_LORfileData::subtract:  Error: " << endl;
             cout << "short int data missing!"  << endl;
             exit(0);
           }
 
       for (int i=0; i<ntot_channels; i++)
           {
             data_short[i]-=data0[i];
           }
     }  
 }
 
 
 //----------------------------------------------------------------------------------------
 //---supress negative values (set to zero)
 //----------------------------------------------------------------------------------------
 void BrainPET_LORfileData::supress_negative_values(void)
 {
   float sum_pos=0.0;
   float sum_neg=0.0;
   if (data_float)
     {      
       for (int i=0; i<ntot_channels; i++)
           {
             if (data[i]<0.0)
               {
                 sum_neg+=data[i];
                 data[i]=0.0;        
               }
             else
               {
                 sum_pos+=data[i];
               }
           }
     }
   else
     {
       for (int i=0; i<ntot_channels; i++)
           {
             if (data_short[i]<0)
               {
                 sum_neg+=float(data_short[i]);
                 data_short[i]=0;              
               }
             else
               {
                 sum_pos+=float(data_short[i]);
               }
           }
     }  
   
   cout << "BrainPET_LORfileData::supress_negative_values:  Info: " << endl;
   cout << "SUMpositive=" << sum_pos << endl;
   cout << "SUMnegative=" << sum_neg << endl;
   cout << "Ratio=" << fabs(sum_neg)/sum_pos << endl;
 
 }
 
 
 //----------------------------------------------------------------------------------------
 //---scale data with factor
 //----------------------------------------------------------------------------------------
 void BrainPET_LORfileData::scale_data(float scale_factor)
 {
   //float sum_pos=0.0;
   //float sum_neg=0.0;
   if (!data_float)
     {  
       cout << "BrainPET_LORfileData::scale_data:  Error: ";
       cout << "data not in float format; scaling not yet supported!" << endl;
       exit(0);
     }
   else
     {
       for (int i=0; i<ntot_channels; i++)
           {
             data[i]*=scale_factor;
           }
     }
 }
 
 
 //----------------------------------------------------------------------------------------
 //---multiply data with given instance (e.g. for efficiency correction)
 //----------------------------------------------------------------------------------------
 void BrainPET_LORfileData::multiply_data_by(BrainPET_LORfileData* lordata)
 {
 
   if (lordata==NULL)
     {
       cout << "BrainPET_LORfileData::multiply_data_by:  Error: ";
       cout << "missing data!" << endl;
       exit(0);
     }
 
   if ((lordata->n_mpairs!=n_mpairs) || (lordata->n_mpair_size!=n_mpair_size))
     {
       cout << "BrainPET_LORfileData::multiply_data_by:  Error: ";
       cout << "Data does not match!" << endl;
       exit(0);
     }
 
   if (!data_float)
     {
       cout << "BrainPET_LORfileData::multiply_data_by:  Error: ";
       cout << "Operation only supported in case of floats for executing instance!" << endl;
       cout << "No multiplication executed!" << endl;
       return;
     }
 
   //float sum=0.0;
   if (lordata->data_float)
     {
       cout << "BrainPET_LORfileData::multiply_data_by:  Info: everything in float! OK!";
       float* data0=lordata->data;
       if ((data0==NULL) || (data==NULL))
           {
             cout << "BrainPET_LORfileData::add_data_from:  Error: ";
             cout << "float data missing!"  << endl;
             exit(0);
           }
 
       for (int i=0; i<ntot_channels; i++)
           {
             data[i]*=data0[i];
           }
     }
   else
     {
       cout << "BrainPET_LORfileData::multiply_data_by:  Info: short will be converted beforehand!" << endl;
       short int* data0=lordata->data_short;
       if ((data0==NULL) || (data==NULL))
           {
             cout << "BrainPET_LORfileData::add_data_from:  Error: ";
             cout << "data array(s) missing!"  << endl;
             exit(0);
           }
 
       for (int i=0; i<ntot_channels; i++)
           {
             data[i]*=float(data0[i]);
           }
     }  
 }
 
 
 
 
 //----------------------------------------------------------------------------------------
 //---divide data with given instance (e.g. for efficiency correction)
 //----------------------------------------------------------------------------------------
 void BrainPET_LORfileData::divide_data_by(BrainPET_LORfileData* lordata)
 {
 
   if (lordata==NULL)
     {
       cout << "BrainPET_LORfileData::divide_data_by:  Error: ";
       cout << "missing data!" << endl;
       exit(0);
     }
 
   if ((lordata->n_mpairs!=n_mpairs) || (lordata->n_mpair_size!=n_mpair_size))
     {
       cout << "BrainPET_LORfileData::divide_data_by:  Error: ";
       cout << "Data does not match!" << endl;
       exit(0);
     }
 
   if (!data_float)
     {
       cout << "BrainPET_LORfileData::divide_data_by:  Error: ";
       cout << "Operation only supported in case of floats for executing instance!" << endl;
       cout << "No divisons executed!" << endl;
       return;
     }
 
   //float sum=0.0;
   int nzero=0;
   int nok=0;
   if (lordata->data_float)
     {
       cout << "BrainPET_LORfileData::divide_data_by:  Info: everything in float! OK!" << endl;
       float* data0=lordata->data;
       if ((data0==NULL) || (data==NULL))
           {
             cout << "BrainPET_LORfileData::add_data_from:  Error: ";
             cout << "float data missing!"  << endl;
             exit(0);
           }
 
       for (int i=0; i<ntot_channels; i++)
           {
             if (fabs(data0[i])>1E-20)
               {
                 data[i]/=data0[i];
                 nok++;
               }
             else
               {
                 data[i]=0.0;
                 nzero++;
               }
           }
     }
   else
     {
       cout << "BrainPET_LORfileData::divide_data_by:  Info: short will be converted beforehand!" << endl;
       short int* data0=lordata->data_short;
       if ((data0==NULL) || (data==NULL))
           {
             cout << "BrainPET_LORfileData::add_data_from:  Error: ";
             cout << "data array(s) missing!"  << endl;
             exit(0);
           }
 
       for (int i=0; i<ntot_channels; i++)
           {
             if (abs(data0[i])!=0)
               {
                 data[i]/=float(data0[i]);
                 nok++;
               }
             else
               {
                 data[i]=0.0;
                 nzero++;
               }
           }
     }  
   cout << "BrainPET_LORfileData::divide_data_by:  ";
   cout << "Info: number of divisions by zero=" << nzero << endl;
   cout << "Nok=" << nok << endl;
 }
 
 
 
 
 //----------------------------------------------------------------------------------------
 //---show frequency of values
 //----------------------------------------------------------------------------------------
 void BrainPET_LORfileData::show_frequency_values(void)
 {
   cout << "--- BrainPET_LORfileData::show_frequency_values ---" << endl;
 
   float min,max;
   get_min_max_values(min,max);
   TH1D* h=new TH1D("ValueFrequency","ValueFrequency",1000,min,max);
   h->SetLineColor(1);
 
   if (data_float)
     {
       for (int i=0; i<ntot_channels; i++)
           {
             h->Fill(data[i]);
           }
     }
   else
     {
       for (int i=0; i<ntot_channels; i++)
           {
             h->Fill(float(data[i]));
           }
     }
 
   TCanvas* c=new TCanvas("BrainPET_LORfileData","BrainPET_LORfileData");
   h->Draw();
   c->Update();
 }
 
 //----------------------------------------------------------------------------------------
  //---shows the coincidences between the defined heads:
 //----------------------------------------------------------------------------------------
 void BrainPET_LORfileData::show_head_pair(int h1, int h2){
  
   
 
 
   //int a, b; 
   int bh, i;
     int h, hp, oh;
     int n_hpairs= 304; //(32*19)/2
     int fov = 19;
     int n_heads = 32;
     short int *hpairs= new short int[n_hpairs*2];
     int hpoff=(n_heads-fov+1)/2;
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
     int selected_hp = -1;
     bool hp_exists = false;
     for(i = 0; i < 304; i++){
       cout << "hp " << i << " h1: " << hpairs[2*i] << " h2: " << hpairs[(2*i)+1] << endl;
       if(hpairs[2*i]== h1&&hpairs[(2*i)+1]==h2){
           selected_hp = i;
           hp_exists = true;
           break;
       }
     }
     
     if(hp_exists == true){      
       char buffer[256];
       sprintf(buffer, "headpairs: h%i, h%i (hp_nr: %i)", h1, h2, selected_hp);
       TH2F *hist2 = new TH2F(buffer,buffer, 864,0 , 863, 864 ,0, 863);
       for(int i = 0; i < 864; i++){
           for(int k = 0; k < 864; k++){ 
             float val = data[(selected_hp*864*864) +  i+864*k];
             hist2->SetBinContent(i+1,k+1,val);
           }
       }
       
       TCanvas* c=new TCanvas(buffer,buffer, 900, 900);
       hist2->Draw("colz");
       delete[] hpairs;
       hpairs = NULL;
       c->Update();
     }else{
       cout << "Required combination of heads is not in coincidence" << endl;
       cout << "Select other combination" << endl;
     }
     
     return;
 }
 
 
 
 
 
 //----------------------------------------------------------------------------------------
 //---separate to 1/-1 at threshold:  -1  below thress, +1 above thress
 //---to mimic dwell information used in Distribution Keys
 //----------------------------------------------------------------------------------------
 void BrainPET_LORfileData::set_DWELL_threshold(float threshold)
 {
   
   if (data_float)
     {
       for (int i=0; i<ntot_channels; i++)
           {
             if (data[i]>threshold)
               {
                 data[i]=1.0;
               }
             else
               {
                 data[i]=-1.0;
               }
           }
     }
   else
     {     
       cout << "BrainPET_LORfileData::set_DWELL_threshold:  Error: ";
       cout << "Not supproted for short int data format!" << endl;
       exit(0);
     }
 
 }
 
 
 //----------------------------------------------------------------------------------------
 //--- add event by hbc-notation (c.w.)
 //--- bin is incremented by "value"
 //----------------------------------------------------------------------------------------
 
 void BrainPET_LORfileData::add_lor_event(int h1, int b1, int c1,
                                                    int h2, int b2, int c2, float value){
 
   if(dump==NULL){
     cout << "Error BrainPET_LORfileData::add_lor_event(): "
            << "MRPET_dump object not initialised!!!" << endl;
     exit(0);
   }else{
       int addr = dump->compute_flat_address( h1,  b1,  c1, 
                                                        h2,  b2,  c2);
 
       if(addr >0 && addr<n_mpairs*n_mpair_size){ 
           if(data_float){
             data[addr]+=value;
           }else{
             data_short[addr]++;
           }
       }
   }
       
 }
 
 //----------------------------------------------------------------------------------------
 //--- get LOR data by flat address; always as float. (c.w.)
 //----------------------------------------------------------------------------------------
 float BrainPET_LORfileData::get_lor_data(int addr){
   float value=0;
 
   if(addr >0 && addr<n_mpairs*n_mpair_size){    
     if(data_float)
       value = data[addr];
     else
       value = (float) data_short[addr];
   }
   return value;
   
   
 }
 
 //----------------------------------------------------------------------------------------
 //--- get LOR data by flat address; always as float. (c.w.)
 //----------------------------------------------------------------------------------------
 void BrainPET_LORfileData::set_lor_data(int addr, float value){
 
   if(addr>=0 && addr <n_mpairs*n_mpair_size){ 
     if(data_float)
       data[addr] = value;
     else
       data_short[addr] = (short int) value;
   }else{
     cout << "ADDRESS OUT OF BOUNDS! addr: " << addr << endl;
   }
 
 
 }
 
 
 
 //----------------------------------------------------------------------------------------
 //--- set MRPET_dump object for code handling of mrpet_dump.c (c.w.)
 //----------------------------------------------------------------------------------------
 void BrainPET_LORfileData::set_mrpet_dump(MRPET_dump *dump_x){
   if(dump!=NULL)
     cout << "warning: BrainPET_LORfileData::MRPET_dump will be overwritten!!!" 
            << endl;
   
   dump = dump_x;
 }
 
 
 //----------------------------------------------------------------------------------------
   //---replace value below threshold by replace_value (c.w.)
 //----------------------------------------------------------------------------------------
 void BrainPET_LORfileData::set_value_below(float threshold, float replace_value){
   int counter =0;
   if (data_float)
     {
       cout << "BrainPET_LORfileData::set_value_below:  Info: ";
       cout << "replace below=" << threshold  << " by " << replace_value << endl;      
       for (int i=0; i<ntot_channels; i++){
           if(data[i]<threshold){
             data[i]=replace_value;
             counter++;
           }
       }
     }
   else
     {
       cout << "BrainPET_LORfileData::set_value_below:  Info: ";
       short int val=(short int)replace_value;
       cout << "replace below=" << threshold  << " by " << val << endl;
       for (int i=0; i<ntot_channels; i++){
           if(data_short[i]<val){
             data_short[i]=val;
             counter++;
           }
       }
     }
   cout << "Number of values replaced: " << counter 
        << "(" << (float)(counter/ntot_channels) << "%)" <<endl;
 
 }
   
 
 
 //----------------------------------------------------------------------------------------
   //---replace value above threshold by replace_value (c.w.)
 //----------------------------------------------------------------------------------------
 void BrainPET_LORfileData::set_value_above(float threshold, float replace_value){
   int counter =0;
 
   if (data_float)
     {
       cout << "BrainPET_LORfileData::set_value_above:  Info: ";
       cout << "replace below=" << threshold  << " by " << replace_value << endl;      
       for (int i=0; i<ntot_channels; i++){
           if(data[i]>threshold){
             data[i]=replace_value;
             counter++;
           }
       }
     }
   else
     {
       cout << "BrainPET_LORfileData::set_value_above:  Info: ";
       short int val=(short int)replace_value;
       cout << "replace above=" << threshold  << " by " << val << endl;
       for (int i=0; i<ntot_channels; i++){
           if(data_short[i]>val){
             data_short[i]=val;
             counter++;
           }
       }
     }
   cout << "Number of values replaced: " << counter 
        << "(" <<(float)(counter/ntot_channels) << "%)" <<endl;
 
   }
 
 
 
 
 
 
 
 
 
 
 //---------------------------------------------------------------------------------------
 //--- return modul pair size (total number of LORs)
 //---------------------------------------------------------------------------------------
 int BrainPET_LORfileData::getMPairSize(){
   return ntot_channels;
 }
 
 
 
 //----------------------------------------------------------------------------------------
 //---subtract data
 //----------------------------------------------------------------------------------------
 void BrainPET_LORfileData::subtract_data_from(BrainPET_LORfileData* lordata)
 {
   if (lordata==NULL)
     {
       cout << "BrainPET_LORfileData::add_data_from:  Error: ";
       cout << "missing data!" << endl;
       exit(0);
     }
 
   if ((lordata->n_mpairs!=n_mpairs) || (lordata->n_mpair_size!=n_mpair_size))
     {
       cout << "BrainPET_LORfileData::add_data_from:  Error: ";
       cout << "Data does not match!" << endl;
       exit(0);
     }
 
   if (lordata->data_float!=data_float)
     {
       cout << "BrainPET_LORfileData::add_data_from:  Error: ";
       cout << "DataTyp (short <--> float) does not match!" << endl;
       exit(0);
     }
 
   //float sum=0.0;
   if (data_float)
     {
       cout << "BrainPET_LORfileData::add_data_from:  Info: use float data" << endl;
       float* data0=lordata->data;
       if ((data0==NULL) || (data==NULL))
           {
             cout << "BrainPET_LORfileData::add_data_from:  Error: ";
             cout << "float data missing!"  << endl;
             exit(0);
           }
 
       for (int i=0; i<ntot_channels; i++)
           {
             data[i]-=data0[i];
           }
     }
   else
     {
       cout << "BrainPET_LORfileData::add_data_from:  Info: use short data" << endl;
       short int* data0=lordata->data_short;
       if ((data0==NULL) || (data_short==NULL))
           {
             cout << "BrainPET_LORfileData::add_data_from:  Error: ";
             cout << "short int data missing!"  << endl;
             exit(0);
           }
 
       for (int i=0; i<ntot_channels; i++)
           {
             data_short[i]-=data0[i];
           }
     }  
 }
 
 void BrainPET_LORfileData::deleteData(void)
 { 
   cout << "BrainPET_LORfileData::~BrainPET_LORfileData(void)";
 
     delete[] data;
     delete[] mpair_data;
     delete[] data_short;
     delete[] mpair_data_short;
   
   cout << "... done" << endl;
 }
 
 //----------------------------------------------------------------------------------------
 //----------------------------------------------------------------------------------------
 BrainPET_LORfileData::~BrainPET_LORfileData(void)
 { 
   cout << "BrainPET_LORfileData::~BrainPET_LORfileData(void)";
 
 
     delete[] data;
     delete[] mpair_data;
     delete[] data_short;
     delete[] mpair_data_short;
   
   cout << "... done" << endl;
 }
 
 
 //----------------------------------------------------------------------------------------
 //---get pointer to data array (float)
 //----------------------------------------------------------------------------------------
 float* BrainPET_LORfileData::get_data_pointer_float(int &size){
   if(data_float){
      size = ntot_channels;
      return data;
   }else{
     size = 0;
     return NULL;
   }
   
 }
   
 
 //----------------------------------------------------------------------------------------
 //---get pointer to data array (short int)
 //----------------------------------------------------------------------------------------
 short int* BrainPET_LORfileData::get_data_pointer_int(int &size){
   if(!data_float){
     size = ntot_channels;
     return data_short;
   }else{
     size = 0;
     return NULL;
   }
 }
//Generated on Tue Jun 18 17:17:16 2013 for *** PRESTO *** Pet Reconstruction Software TOolbox *** revision 333 *** by  ￼ 1.5.6
