/* Bo Ma

   * Description: this macro is used to forward projection the image, you can either choose to get the attenuation map or get the common sino.

// Time:2016.06.08
//
  // Editor: Bo Ma

 
 */
 /*parameters for the  reconstruction:
sprintf(cmd,".! %s -i %s -T 1 -F 1,0 -h %s -I 0 ",file_path_FWPexe.c_str(),input_img.c_str(),out_sino.c_str());
 OP_OSEM3D V3.1.3 27-May-2010
 Hardware information: 
 Total RAM: 3917971456
 Free RAM: 789446656
 Num PROC: 4
 OSEM3D weighted reconstruction for parallel projections in flat file format
 OP_OSEM3D V3.1.3 27-May-2010
 Compile Info: 14:44:26 May 27 2010
 usage :
 osem3d_rm  -t 3D_scan (or -p 3D_scan -d 3D_scan)  
 -i initial_image [-n 3D_norm] -[a 3D_atten]  [-s 3D_scatter] 
 -o image (or -O image) 
 -F in_flip,out_flip [-h output corrected 3D scan or attenuation (see -U) ] 
 -g max_group,min_group -S number of subsets  -I number of iterations -W weighting method 
 -e epsilon1,epsilon2,epsilon3 
 -u image max 
 -k number of planes 
 -w 
-z zoom -Z zoom 
 -f rel_fov,trimFlag,zmin,zmax 
 -m span,Rd 
 -T nthreads 
 -v verbose_level]
 where :
-t  3D_flat_integer_scan (true) (or -p  3D_flat_integer_scan (prompt) and -d  3D_flat_float_scan (smoothed un-normalized delayed))
-P  Flag: input scan is pre-corrected (i.e. float rather than integer format) [not set] 
-Q  Flag: input scan is float but is not pre-corrected [not set]
-R  Flag: input scan is 4-bytes integer [not set]
 -F  input_flip, output_flip flag, 0/1  no/yes, [0,0] 
 -N  normfac.i will be in memory if there is enough memory. If it is slower with this option, don't use it
 -n  3D_flat_normalisation 
 -a  3D_flat_attenuation 
 -i  initial image in flat format default is 1 in FOV specified by -f 
-U  initial image is a mu_map in cm-1 
-s  input scatter scan (already normalized) in flat format
 -o  output image (flat format) 
 -O  output image and save each iteration
-h  output corrected 3D scan or attenuation (flat format) 
-g  max group, min group, [groupmax,0] 
 -S  number of subsets; [16] 
 -I  number of iterations; [1]  
 -W  weighting method (0=UWO3D, 1=AWO3D, 2=ANWO3D, 3=OPO3D) [2]
-W  weighting method (0=UWO3D, 1=AWO3D, 2=ANWO3D, 3=OPO3D) [2]
-w  Flag: positivity in image space [sinogram space]
 -e  epsilon[3] 5.000000e-02,1.000000e-02,1.000000e-10
 -u  image upper threshold [10000.000000]
-k  kill end segments with specified planes in image space [0]
-z  zoom [1.0] 
-Z  zoom [1.0] zoom in projection space 
-f  fov as tomographic_fov_in_%,trimFlag,z_min,z_max [0.95,0,0,nplanes-1] 
-m  span,Rd (m-o-gram parameters of all input files) [9,67] 
-M  scanner model number [3000] 
if iModel==0, needs file name which have Optional parameters 
 -T  number of threads [# of CPU-core - 2 for linux] 
-v  verbose_level [none = 0]
-B  resolution modeling parameter FWHM1,FWHM2,ratio2/1 [0,0,0]
 1: Scanner info             4: Normalization info 
 8: Reconstruction info     16: I/O info           
32: Processing info         64: Memory allocation info

 */
									


void FwdProjSiemens()
{

  
fwd_presto_siemens();
//fwd_direct();

 gApplication->Terminate();




}


void fwd_presto_siemens()
{


   string inputFolder = "/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/Brain_phantom_ma/gpu_mcs/" ;

 
   string input_img = inputFolder  + "att_true_att_coeff.i" ;
   string out_sino = inputFolder + "att_fwp.fs" ;
   //bool is_calACF = false;
   bool is_calACF = true;
   int software_version = 1 ;

 stools->SIEMENS_FWDPROJ( input_img ,out_sino , is_calACF, software_version);
//void SiemensTools::SIEMENS_FWDPROJ	(	string 	image_file,string sino_file,bool calcACF = true,int software_version = 1)	

}

void fwd_direct()
{

  string file_path_FWPexe="/data/BrainPET_Sys/Software/SiemensCode/v1/hi_osem3d_rm_313";
//string file_path_attImage="/data/mr_gpu/bma/gpupet/input_files/CaliQC-F18-cylinder_test_atten.i";
//string file_path_attSino="/data/mr_gpu/bma/gpupet/input_files/CaliQC-F18-cylinder_test_atten.fs";


 string inputFolder = "/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/cylinder_phantom_norm_file_mcs/" ;

 
   string input_img = inputFolder  + "att.i" ;
   //string out_sino = inputFolder + "att_with_acf_direct.fs" ;
   string out_sino = inputFolder + "att_no_acf_direct.fs" ;


char cmd[1000];
// only for attenuation map
//sprintf(cmd,".! %s -i %s -T 1 -F 1,0 -U -h %s -I 0 ",file_path_FWPexe.c_str(),input_img.c_str(),out_sino.c_str());

//common  forward projection
sprintf(cmd,".! %s -i %s -T 1 -F 1,0 -h %s -I 0 ",file_path_FWPexe.c_str(),input_img.c_str(),out_sino.c_str());
cout << cmd <<endl;
gROOT->ProcessLine(cmd);


//gROOT->ProcessLine(".!/data/pet_mr03/siemens_software/reco/hi_osem3d_rm_313 -i /data/mr_gpu/bma/gpupet/input_files/ CaliQC-F18-cylinder_test_atten.i -F 1,0 -U -h /data/mr_gpu/bma/gpupet/input_files/ CaliQC-F18-cylinder_test_atten.fs -I 0");


}

//void Simens_recon_single(string patient_name,string frame_time,string inputFolder,string path_norm_sino,int niterations,int nsubsets)
void Simens_recon_single(int niterations,int nsubsets)
{

  string inputFolder = "/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/XB1BN310N-BI/XB1BN310N-BI-01/scatterMCS/" ;

  //sinogram files:
  string prompt_sino=inputFolder + "run_num_5000/XB1BN310N-BI-01_0-1800_gpuSimu_true_scatter_sino.fs";//integer format
  //string prompt_sino="/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/cylinder_phantom_norm_file_mcs/scatterMCS/run_num_5000/norm_acq_gpuSimu_0_true.fs";//integer format
  string random_sino=inputFolder +"common_sino_files/random.fs";//float format

  //--- Norm file for measurement data
  //string norm_sino= inputFolder +"common_sino_files/norm_mcs.fs";
  string norm_sino= inputFolder +"common_sino_files/norm_add_gap.fs";

  //...attenuation sinogram generated by forward projection by Siemens tools;
  string atte_sino= inputFolder +"common_sino_files/att.fs";//float format


  string scatter_sino_scaled = inputFolder +"common_sino_files/scatter.fs";

  //the reconstructed image will stored in the path bellow.
  string reconstructe_img=inputFolder + "run_num_5000/reconstructed_img_new_gaps.i";
  //string reconstructe_img=inputFolder + "run_num_5000/reconstructed_img_no_gaps.i";
  //string reconstructe_img= "/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/cylinder_phantom_norm_file_mcs/scatterMCS/run_num_5000/reconstructed_img_new_gaps.i";


  stools->SIEMENS_RECO_W3(prompt_sino, random_sino, norm_sino,  atte_sino, scatter_sino_scaled, niterations, nsubsets, reconstructe_img);





  cout <<endl<<endl;
  cout << "image reconstruction finished :image saved path:"<< endl;
  cout <<reconstructe_img <<endl;
  cout << "***********************************************************************************" << endl;


  cout <<endl<<endl;
  cout << "******************"<<endl;
}

