#!/bin/bash

#Discription:Generate 2D histograms of the defined z slice of the 3D reconstructed image and plot a profile of a defined range of the histogram.

for ((NUMBER=60;NUMBER <80;NUMBER++))
{

  #root './mcs_sss_plot_FDG.C('$NUMBER')'
  #root './mcs_sss_plot_FET.C('$NUMBER')'
  root './compare_MCS_SSS_sino_matrix.C('$NUMBER')'
  #root './mcs_sss_plot.C('$NUMBER')'
  #root './mcs_sss_plot_no_paveText.C('$NUMBER')'
  #root './mcs_sss_plot_no_paveText_FDG.C('$NUMBER')'

  wait
  sleep 1
}
