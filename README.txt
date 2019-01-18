**********************************************************************
**********************************************************************
Description: all root macros I have wroten

Editor: Bo Ma;

Date: 2018.12.07


**********************************************************************
**********************************************************************


1: brain_pet_root_macros:
 These are the latest root macros I wrote. These are all root macros using for the process of data for the brain pet insert, including LOR data, sino data and reconstructed image. Also included is some operations of the root file, including how to read binary file , historame, vectors etc. to root file. Most of these macros need the libraries of PRESTO.


2: general_funcitons: 
  In this folder, some basic general functions are stored, including : convert the format of reconstructed image, generate some raw image, merge two images in size, draw the scanner geometry just using lines and dots, etc..


3: gpupet_root_macros
  In this folder, all the macros relating to the simulation of GPU-mcs are here. It includes two main folders, macros_run_on_petpc36 and macros_run_on_petpc28 the former is used for the simulation on petpc36, while the later is the data process on petpc28 where PRESTO libraries can be used here.

4: gate_root_macros:

  In this folder, root macros needed for processing of the gate simulation data are here, including: converting root data to LOR data, get the axial distribution of the LOR data, etc..

5: random_OFOV

  In this folder,  root macros used for the process of LOR data and sino data are here, mainly for the acquisition of the axial distribution of the events.


6: root_linux_command.C

  In this macro, the useful commands which relate to the operations of linux command in root running are listed.

7: root_learning

    In this folder, some basic root operations I have learned are there, including: the basic ways to read, write root file and root trees, the operations of histograms, graphs, data fitting, cpp compiling, etc..

8: root_cpp_compbine_ACLiC

  In this folder, the way to compile and use of the cpp data in root is listed. 
