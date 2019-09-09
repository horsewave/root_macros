********************************************************************************
********************************************************************************

Description: all these root macros focus on the manipulation of data relating to the brain PET insert, including the operation of Listmode, lor, sino and reconstructed images.

Editor : Bo Ma;
Date: 2018.12.06
********************************************************************************
********************************************************************************



********************************************************************************
  File list
********************************************************************************
1: presto_convert_listmode_to_sino.C 
  Convert listmode data to LOR and prompt and variant reduced random sinos. This is written by others.

2: read_binary_data_to_root.C
  stored the scatter coincedences from the binary file to root file for further processing.
  The binary file is acquired from the GPU-MCS,which is sorted with  different data types:
  int,int, short int,short int, char.


3: compare_MCS_SSS_scatter_sino.C
  compare the defference of the scatter sinograms aquired by SSS and MCS, and Save the distibution of scatter events along the z axial to root file 
     and also their plot.

4: draw_ofov_scatter.C
    This macro is used to get the axial distribution of the ofov scatter coincidences from the root file tree, and draw the OFOV source, ifov sourc ofov scatter and the scatter Coincidences only inside the fov.

5: Lor_to_sino.C
    This macro is used to convert the LOR data to sino data. 

6: Image_reconstruction_Simens.C
   This macro is used for the image reconstruction using the Siemens reconstruction method.

7:  convert_img_intensity_to_suv.C
        This macro is used to convert the voxel intensity to activity concentration of the image.
        The total activity is acquired from the reconstructed image with the format of ecat(*.v).
        The image which is based on the sss is always needed to calculate the suv convert factor;
        Then you can use this suv_factor to convert the input image to intensity image.

8: mcs_sss_img_plot.C
    Generate 2D histograms of the defined z slice of the 3D reconstructed image and plot a profile of a defined range of the histogram.

9: compare_MCS_SSS_prompt_sino_axial.C 
  Compare the axial distribution of the sino of prompt, mcs-based scatter and sss-based scatter. This is for all the 1399 planes of a sino.

10: compare_MCS_SSS_prompt_sino_radial.C:
   Compare the radial distribution of the sino of prompt, mcs-based scatter and sss-based scatter. This is for all the 1399 planes of a sino.

11: compare_MCS_SSS_prompt_sino_radial_seg_0.C
   Compare the radial distribution of the sino of prompt, mcs-based scatter and sss-based scatter. This is for all the 153 planes of segment0 of a sino.

12: compare_MCS_SSS_sino_axial.C
    Compare the axial distribution of the sino of mcs-based scatter and sss-based scatter. This is for all the 1399 planes of a sino.

13: compare_MCS_SSS_sino_radial.C
 Compare the radial distribution of the sino of  mcs-based scatter and sss-based scatter. This is for all the 1399 planes of a sino.

14: compare_MCS_SSS_sino_radial_seg_0.C

 Compare the radial distribution of the sino of  mcs-based scatter and sss-based scatter. This is for all the 153 planes of segment0 of a sino.


15: compare_MCS_SSS_sino_matrix.C:
  compare the projection view of the sinogram in transaxile view. This can be used to test how noisy of the scatter sinos with different simulated statistics.

16: draw_ofov_scatter.C
    This macro is used to get the axial distribution of the ofov scatter coincidences from the root file tree, and draw the OFOV source, ifov sourc ofov scatter and the scatter Coincidences only inside the fov.

17: FwdProjSiemens.C
this macro is used to forward projection the image, you can either choose to get the attenuation map or get the common sino.

18: GATE_GGEMS_TOF_compare.C 
    compare the TOF diff. between gate and GGEMS.


19:Image_reconstruction_Simens.C
 This macro is used for the image reconstruction using the Siemens reconstruction method.

20: img_voi_ave_std_calc_304.C

  this macro is used to calculate the RC and its standard error for the cylindrical phantom with three cold inserts.

21: img_voi_ave_std_calc_310.C
this macro is used to calculate the RC and its standard error for the cylindrical phantom with six spheres

22: Lor_to_sino.C
This macro is used to convert the LOR data to sino data 


23:mcs_sss_img_plot.C
Generate 2D histograms of the defined z slice of the 3D reconstructed image and plot a profile of a defined range of the histogram.

24: siemens_sss.C
whole procedure for the sss


25:sino_add_gaps.C
Add gaps to sino, the sinograms from the forward projection does not have gaps

26:sino_bad_planes_cor.C
 There are four bad planes in the sino, this is used to set the bad planes with the value of the neighbouring planes.

27: Sino_manipulation.C
 Operations with Sinograms,including random,normlization,scatter, attenuation corrections, sinogram scale(slice by slice, whole sinograms and by axial directions

28:


********************************************************************************
 Standard procedure for OFOV scatter processing
********************************************************************************
1: get the position of the OFOV events from the binary data:
   read_binary_data_to_root.C
   to save the binary data to root file.


2:   get the axial distribution of the ofov scatter coincidences from the root file tree, and draw the OFOV source, ifov sourc ofov scatter and the scatter Coincidences only inside the fov.
      draw_ofov_scatter.C


3: convert the LOR data to sino data. 
   Lor_to_sino.C



4:Save the distibution of scatter events along the z axial to root file and also their plot:
  compare_MCS_SSS_scatter_sino.C

5: Image reconstruction using the Siemens reconstruction method:
   Image_reconstruction_Simens.C

6: convert the voxel intensity to activity concentration of the image: 
    convert_img_intensity_to_suv.C

8: Generate 2D histograms of the defined z slice of the 3D reconstructed image and plot a profile of a defined range of the histogram.
   mcs_sss_img_plot.C




