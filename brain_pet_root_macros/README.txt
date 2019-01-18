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




