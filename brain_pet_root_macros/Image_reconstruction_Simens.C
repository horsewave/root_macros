/**
 * Description: This macro is used for the image reconstruction using the Siemens reconstruction method. 
 * Input: 
 * Procedure:
 * Functions:
 *
 * Editor: Bo Ma
 * Date: 2018.12.07
 */

//void Image_reconstruction_Simens( string patient_name,string inputFolder, string path_norm_sino,string frame_time)
void Image_reconstruction_Simens(){

  gROOT->Reset();
  // temp_recon_81_dm_scatter();
  // temp_recon_81_dm_true_1();
  // temp_recon_81_dm_true_2();
  temp_recon_81_dm_true_3();
  gApplication->Terminate();
}

/**
* @brief: img recon for scatter events with detector modual for patient 81
* @verbatim
* details
* @endverbatim
* @param[in] details : 
* @param[out] details :
* @return details :
* @retval details
* @deprecated 
* @see 
* @pre 
**/
void temp_recon_81_dm_scatter()
{

  string prompt_sino="/home/mabo/software/data/patient_data/FDG/HM1BP081F-BI/scatterMCS/merged/HM1BP081F-BI_gpuSimu_0_scatter.fs";

  string random_sino="/home/mabo/software/data/phantom_data/cylinder_phantom_norm_file_mcs/common_sino_files/random.fs";

  //--- Norm file for measurement data
  //norm without coil
  //string norm_sino= "/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/cylinder_phantom_norm_file_mcs/norm_with_detection_modual/norm_sino_320_192_with_detection_modual.fs";
  string norm_sino= "/home/mabo/software/data/phantom_data/cylinder_phantom_norm_for_scatter_recon/scatterMCS/merged/norm_sino_for_scatter.fs";
  //string norm_sino= "/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/cylinder_phantom_norm_file_mcs/common_sino_files/norm_ones_with_gaps.fs";

  ///@todo what attenuation map should be used here?
  // first try the clinical att map
  string atte_sino= "/home/mabo/software/data/patient_data/FDG/HM1BP081F-BI/atten/GUI_generated/AttenMap_for_reco.a";//float format

  // scatter with scatter modual;
  string scatter_sino_scaled = "/home/mabo/software/data/phantom_data/cylinder_phantom_norm_file_mcs/common_sino_files/random.fs";
  string reconstructe_img= "/home/mabo/software/data/patient_data/FDG/HM1BP081F-BI/scatterMCS/merged/img_recon_scatter_norm_for_scatter.i";

   
  int nsubsets=2;
  int niterations=16;

  Simens_recon(prompt_sino, random_sino, norm_sino, atte_sino ,scatter_sino_scaled ,reconstructe_img,  niterations, nsubsets);


}
/**
* @brief: img recon for true events with detector modual for patient 81
* @note: the norm file is acquired with detection modual, without coil, using
* only the true events. There are still artifacts in the img, but is much
* better than true-1 without coil.
**/

void temp_recon_81_dm_true_1(){

  string prompt_sino="/home/mabo/software/data/patient_data/FDG/HM1BP081F-BI/scatterMCS/merged/HM1BP081F-BI_gpuSimu_0_true.fs";

  string random_sino="/home/mabo/software/data/phantom_data/cylinder_phantom_norm_file_mcs/common_sino_files/random.fs";

  //--- Norm file for measurement data
  //norm without coil
  // //this norm file is specifically acquired for the GGEMS acquired true events reconstruction.
  string norm_sino= "/home/mabo/software/data/phantom_data/cylinder_phantom_norm_file_mcs/norm_with_detection_modual/norm_sino_320_192_with_detection_modual.fs";

  ///@todo what attenuation map should be used here?
  // first try the clinical att map
  string atte_sino= "/home/mabo/software/data/patient_data/FDG/HM1BP081F-BI/atten/GUI_generated/AttenMap_for_reco.a";//float format

  // scatter with scatter modual;
  string scatter_sino_scaled = "/home/mabo/software/data/phantom_data/cylinder_phantom_norm_file_mcs/common_sino_files/random.fs";
  string reconstructe_img= "/home/mabo/software/data/patient_data/FDG/HM1BP081F-BI/scatterMCS/merged/img_recon_true_norm_for_true-1.i";

  //string norm_sino= "/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/cylinder_phantom_norm_file_mcs/common_sino_files/norm_ones_with_gaps.fs";
  
   
  int nsubsets=2;
  int niterations=16;

  Simens_recon(prompt_sino, random_sino, norm_sino, atte_sino ,scatter_sino_scaled ,reconstructe_img,  niterations, nsubsets);

}

/**
* @brief: img recon for true events with detector modual for patient 81
* @note: the norm file is acquired with detection modual, with coil, using
* only the true events. However there are artifacts in the img.
**/

void temp_recon_81_dm_true_2(){

  string prompt_sino="/home/mabo/software/data/patient_data/FDG/HM1BP081F-BI/scatterMCS/merged/HM1BP081F-BI_gpuSimu_0_true.fs";

  string random_sino="/home/mabo/software/data/phantom_data/cylinder_phantom_norm_file_mcs/common_sino_files/random.fs";

  //--- Norm file for measurement data
  //norm without coil
  // //this norm file is specifically acquired for the GGEMS acquired true events reconstruction.
  string norm_sino= "/home/mabo/software/data/phantom_data/cylinder_phantom_norm_file_mcs/norm_with_detection_modual_with_coil/norm_sino_320_192_with_detection_modual_with_coil.fs";

  ///@todo what attenuation map should be used here?
  // first try the clinical att map
  string atte_sino= "/home/mabo/software/data/patient_data/FDG/HM1BP081F-BI/atten/GUI_generated/AttenMap_for_reco.a";//float format

  // scatter with scatter modual;
  string scatter_sino_scaled = "/home/mabo/software/data/phantom_data/cylinder_phantom_norm_file_mcs/common_sino_files/random.fs";
  string reconstructe_img= "/home/mabo/software/data/patient_data/FDG/HM1BP081F-BI/scatterMCS/merged/img_recon_true_norm_for_true-2.i";

  //string norm_sino= "/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/cylinder_phantom_norm_file_mcs/common_sino_files/norm_ones_with_gaps.fs";
  
   
  int nsubsets=2;
  int niterations=16;

  Simens_recon(prompt_sino, random_sino, norm_sino, atte_sino ,scatter_sino_scaled ,reconstructe_img,  niterations, nsubsets);


}




/**
* @brief: img recon for true events with detector modual for patient 81
* @note: the norm file is acquired with detection modual, with coil, using
* both the true and the scatter events. However there are artifacts in the img.
**/

void temp_recon_81_dm_true_3(){

  string prompt_sino="/home/mabo/software/data/patient_data/FDG/HM1BP081F-BI/scatterMCS/merged/HM1BP081F-BI_gpuSimu_0_true.fs";

  string random_sino="/home/mabo/software/data/phantom_data/cylinder_phantom_norm_file_mcs/common_sino_files/random.fs";

  //--- Norm file for measurement data
  //norm without coil
  // //this norm file is specifically acquired for the GGEMS acquired true events reconstruction.
  string norm_sino= "/home/mabo/software/data/phantom_data/cylinder_phantom_norm_file_mcs/norm_with_detection_modual_with_coil/norm_sino_320_192_with_detection_modual_with_coil_true_plus_scatter.fs";

  ///@todo what attenuation map should be used here?
  // first try the clinical att map
  string atte_sino= "/home/mabo/software/data/patient_data/FDG/HM1BP081F-BI/atten/GUI_generated/AttenMap_for_reco.a";//float format

  // scatter with scatter modual;
  string scatter_sino_scaled = "/home/mabo/software/data/phantom_data/cylinder_phantom_norm_file_mcs/common_sino_files/random.fs";
  string reconstructe_img= "/home/mabo/software/data/patient_data/FDG/HM1BP081F-BI/scatterMCS/merged/img_recon_true_norm_for_true-3.i";

  //string norm_sino= "/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/cylinder_phantom_norm_file_mcs/common_sino_files/norm_ones_with_gaps.fs";
  
   
  int nsubsets=2;
  int niterations=16;

  Simens_recon(prompt_sino, random_sino, norm_sino, atte_sino ,scatter_sino_scaled ,reconstructe_img,  niterations, nsubsets);


}



//  image reconstruction using Siemens tool
void Simens_recon(string prompt_sino, string random_sino, string norm_sino, string atte_sino ,string scatter_sino_scaled ,string reconstructe_img, int niterations,int nsubsets){

  stools->SIEMENS_RECO_W3(prompt_sino, random_sino, norm_sino,  atte_sino, scatter_sino_scaled, niterations, nsubsets, reconstructe_img);



  cout <<endl<<endl;
  cout << "image reconstruction finished :image saved path:"<< endl;
  cout <<reconstructe_img <<endl;
  cout << "***********************************************************************************" << endl;


  cout <<endl<<endl;
  cout << "******************"<<endl;
}






