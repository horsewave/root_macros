/*
 * whole procedure for the sss
 *
 *
 */


void siemens_sss()
{
  //sss_81();
  sss_310();
gApplication->Terminate();



}

void sss_81()
{
  // *********1: SSS simulation*************
  string emission_image_fname = "/data/PET/mr_pet_temp/Ma/software/data/gpupet/patients/FDG/HM1BP081F-BI/img/sss/GUI_generated/range_0-3600_reco_W3_na.dat";
  string mumap_image_fname = "/data/PET/mr_pet_temp/Ma/software/data/gpupet/patients/FDG/HM1BP081F-BI/atten/GUI_generated/AttenMap_for_reco.i";

  string scatter_lor_fname = "/data/PET/mr_pet_temp/Ma/software/data/gpupet/patients/FDG/HM1BP081F-BI/scatterSSS/test/scatter_lor.flor";
  string sino_fname_unscaled = "/data/PET/mr_pet_temp/Ma/software/data/gpupet/patients/FDG/HM1BP081F-BI/scatterSSS/test/scatter_sino_unscaled.fs";
  string sino_fname_scaled = "/data/PET/mr_pet_temp/Ma/software/data/gpupet/patients/FDG/HM1BP081F-BI/scatterSSS/test/scatter_sino_scaled.fs";
  bool dwell_corr = true;

  ////void 	SIEMENS_SCATTER3D_V4 (string emission_image_fname, string mumap_image_fname, string scatter_lor_fname)
//stools->SIEMENS_SCATTER3D_V4(emission_image_fname,mumap_image_fname,scatter_lor_fname);
  
// *********2: LOR TO SINO*************

////void 	SIEMENS_SCANSORT_V3_LOR2SINO (string lor_fname, string sino_fname, bool dwell_corr)
//stools->SIEMENS_SCANSORT_V3_LOR2SINO (scatter_lor_fname, sino_fname, dwell_corr);

// *********3: SSS scaling*************

string fname_norm_sino = "/data/PET/mr_pet_temp/Ma/software/data/gpupet/patients/FDG/HM1BP081F-BI/norm/Norm_2013_02_22_STD_norm.fs" ;
string fname_acf_sino = "/data/PET/mr_pet_temp/Ma/software/data/gpupet/patients/FDG/HM1BP081F-BI/atten/commonFiles/HM1BP081F-BI_AttenMap_HeadOnly.a" ;
string fname_prompt_sino = "/data/PET/mr_pet_temp/Ma/software/data/gpupet/patients/FDG/HM1BP081F-BI/sinos/range_0-3600_prompt.fs" ;
string fname_vrr_sino = "/data/PET/mr_pet_temp/Ma/software/data/gpupet/patients/FDG/HM1BP081F-BI/sinos/range_0-3600_vrr.fs" ;

//block effect sino
string fname_scatbe_sino =  "/data/PET/mr_pet_temp/Ma/software/data/gpupet/commonFiles/siemens_scale_scatter_blockeffect_default.n" ;

////void 	SIEMENS_SCALE_SCATTER_V4 (string fname_unscaled_scatter_sino, string fname_norm_sino, string fname_acf_sino, string fname_prompt_sino, string fname_vrr_sino, string fname_scatbe_sino, string fname_scaled_scatter_sino)
stools->SIEMENS_SCALE_SCATTER_V4 (sino_fname_unscaled, fname_norm_sino, fname_acf_sino, fname_prompt_sino, fname_vrr_sino, fname_scatbe_sino, sino_fname_scaled);



}

void sss_310()
{
  // *********1: SSS simulation*************
  string base_folder = "/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/XB1BN310N-BI/XB1BN310N-BI-01/" ;
  string emission_image_fname = base_folder + "img/sss/GUI_generated/range_0-1800_reco_W3_na.dat";
  string mumap_image_fname =  base_folder + "atten/GUI_generated/AttenMap_for_reco.i";

  string scatter_lor_fname =  base_folder +"scatterSSS/test/scatter_lor.flor";
  //string sino_fname_unscaled = base_folder + "scatterSSS/test/scatter_sino_unscaled.fs";
  //string sino_fname_scaled =  base_folder +"scatterSSS/test/scatter_sino_scaled.fs";
  string sino_fname_unscaled = base_folder + "scatterSSS/test/scatter_sino_unscaled_no_dwell.fs";
  string sino_fname_scaled =  base_folder +"scatterSSS/test/scatter_sino_scaled_no_dwell.fs";
  bool dwell_corr = false;

  ////void 	SIEMENS_SCATTER3D_V4 (string emission_image_fname, string mumap_image_fname, string scatter_lor_fname)
//stools->SIEMENS_SCATTER3D_V4(emission_image_fname,mumap_image_fname,scatter_lor_fname);
  
// *********2: LOR TO SINO*************

////void 	SIEMENS_SCANSORT_V3_LOR2SINO (string lor_fname, string sino_fname, bool dwell_corr)
stools->SIEMENS_SCANSORT_V3_LOR2SINO (scatter_lor_fname, sino_fname_unscaled, dwell_corr);

// *********3: SSS scaling*************

//string fname_norm_sino = base_folder + "norm/BAD-BLOCK--norm-PSNorm-170720.fs" ;
string fname_norm_sino = base_folder + "norm/PSNorm-170720_STD_norm.fs" ;
string fname_acf_sino =  base_folder +"atten/XB1BN310N-BI-01_AttenMap_HeadOnly.a" ;
string fname_prompt_sino =  base_folder +"sinos/range_0-1800_prompt.fs" ;
string fname_vrr_sino =  base_folder +"sinos/range_0-1800_vrr.fs" ;

//block effect sino
string fname_scatbe_sino =  "/data/PET/mr_pet_temp/Ma/software/data/gpupet/commonFiles/siemens_scale_scatter_blockeffect_default.n" ;

////void 	SIEMENS_SCALE_SCATTER_V4 (string fname_unscaled_scatter_sino, string fname_norm_sino, string fname_acf_sino, string fname_prompt_sino, string fname_vrr_sino, string fname_scatbe_sino, string fname_scaled_scatter_sino)
stools->SIEMENS_SCALE_SCATTER_V4 (sino_fname_unscaled, fname_norm_sino, fname_acf_sino, fname_prompt_sino, fname_vrr_sino, fname_scatbe_sino, sino_fname_scaled);



}
