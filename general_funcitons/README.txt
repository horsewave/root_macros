Description: all these root macros focus on the manipulation of data relating to the brain PET insert, including the operation of Listmode, lor, sino and reconstructed images.

Editor : Bo Ma;
Date: 2018.12.06


1: CreateVirtualGeom_Norm.C
  To create a virtural geometry of the brain PET.

2: convert_image_format.C
Gate seems only to take the short int format image data. So this macros is used to convert float img to short int.

3: cubic_phantom_generation.C
creat the cubic phantom used to compare gate and ggems.

4: GenericScannerAll.C
This root file is to draw the Transaxial view and Axial of a scanner


5: Image_extension.C
enlarge the image to the default image size for BrainPET(256*256*153)

6: Image_merge.C
merge two images in the z direction, to generate the OFOV phantom

7: Image_merge_coil.C
combine the phantom and the coil

8: read_binary_data_to_root.C
read binary file with different data types, save them to root file.
9:read_lor_data_to_root.C
Get the axial distribution of the events in the lor data and save it to root file

10:write_array_to_root.C 
