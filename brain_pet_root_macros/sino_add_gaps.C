/*
 * Add gaps to sino, the sinograms from the forward projection does not have gaps,
 * so you you want to use it in the reconstruction, you need to add gaps according to 
 * your real system.
 *
 *
 */

void sino_add_gaps()
{
  string input_sino_no_gap = "/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/sphere_phantom_simu/1_fov_305/att_fwp.fs";
  string input_sino_gap = "/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/cylinder_phantom_norm_file_mcs/common_sino_files/norm_ones_with_gaps.fs";
  string saved_sino_with_gap = "/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/sphere_phantom_simu/1_fov_305/att_fwp_with_gaps.fs";

add_gaps_to_sino(input_sino_no_gap,input_sino_gap, saved_sino_with_gap);
gApplication->Terminate();

}

/*
 * set the gaps of the sino with defined value, the value of the  other points are not changed.
 * 1:path_sino_no_gap: fwp sino;
 * 2:path_sino_with_gap: sino with the value of gaps 0, and others 1;
 * 3: path_saved_sino_with_gap: saved sino with gaps
 *
 */
void add_gaps_to_sino(string path_sino_no_gap,string path_sino_with_gap, string path_saved_sino_with_gap,float gap_value=0)
{
 //string input_sino_gap = "/data/PET/mr_pet_temp/Ma/software/data/gpupet/phantom/cylinder_phantom_norm_file_mcs/common_sino_files/gaps_sino.fs";

  //class including all the basic information of the MRPET.
  MRPET_dump *dump = new MRPET_dump();	

  ///---measurement data, correction of random and scatter//////////////////////////////////////////////////
  BrainPET_Sinograms* sino_input_1=new BrainPET_Sinograms(dump,"sino_norm");  
  BrainPET_Sinograms* sino_input_2=new BrainPET_Sinograms(dump,"temp");

  bool is_int=false;
  sino_input_1->read_flatSinogram(path_sino_no_gap,is_int);
  sino_input_2->read_flatSinogram(path_sino_with_gap,is_int);

  cout<<" the total events of sino_1 is: " << sino_input_1->get_n_Events()<< endl;
  cout<<" the total events of sino_2 is: " << sino_input_2->get_n_Events()<< endl;

  int n_planes=sino_input_1->get_n_planes(); 
  int n_views=sino_input_1->get_n_views(); 
  int n_projs=sino_input_1->get_n_projs(); 

  for (int i_plane=0;i_plane<n_planes;i_plane++)
  {
    for(int i_view=0;i_view<n_views;i_view++)
    {
      for(int i_proj=0;i_proj<n_projs;i_proj++)
      {

        int addr = i_plane*n_views*n_projs + i_view*n_projs + i_proj;
        //get the address in the sino
        //dump->proj_to_addr(i_view, i_proj, i_plane, addr);

        float sino_value_1=sino_input_1->get_event_from_address ( addr);
        float sino_value_2=sino_input_2->get_event_from_address ( addr);

        if(sino_value_1>0 && sino_value_2 == 0)
        {

          int addr_1 = (i_plane-1)*n_views*n_projs + i_view*n_projs + i_proj;
          int addr_2 = (i_plane+1)*n_views*n_projs + i_view*n_projs + i_proj;

          float sino_value_down = 0;
          float sino_value_up=0;

          if(i_plane>0 && i_plane<(n_planes-1))
          {
            sino_value_down = sino_input_2->get_event_from_address ( addr_1);
            sino_value_up=sino_input_2->get_event_from_address ( addr_2);

          }
          else if(i_plane==0)
          {
            sino_value_up=sino_input_2->get_event_from_address ( addr_2);


          }
          else if(i_plane==(n_planes-1))
          {
            sino_value_down = sino_input_2->get_event_from_address ( addr_1);


          }


          if(sino_value_down==0 && sino_value_up == 0)
          {

            sino_input_1->set_event_at_address (addr, gap_value);

          }
        }

      }
    }
  }


  cout<<" the total events of merged sino is: " << sino_input_1->get_n_Events()<< endl;

  sino_input_1->write_flat_sinograms(path_saved_sino_with_gap,is_int);


  if(sino_input_1!=NULL)
  {
    delete sino_input_1;
    sino_input_1=NULL;

  }

  if(sino_input_2!=NULL)
  {
    delete sino_input_2;
    sino_input_2=NULL;

  }
  if(dump!=NULL)
  {
    delete dump;
  }






}


