
//write the tree with the data type of struct and class 

void lesson_03_root_write_tree_with_struct_class()
{
  //example_4();
  example_5();

}


//to store in a "struct" data type
void example_4()
{
  /*
   * you can set up multiple leaves in a branch,
   *
   * "struct" or "class"
   *here a struct is demonstrated, but,
   *it is recommended to use "class"
   *
   */

  //build a struct
  struct particle_struct{
    Float_t energy;
    Float_t position[3];
  };

  particle_struct sParticle;

  //build a tree
  TTree t("t","struct in a tree");

  t.Branch("Energy",&sParticle.energy,"p_energy/F");
  t.Branch("position",sParticle.position,"p_pos[3]/F");

  //or 
  ////t.Branch("Particle_info",&sParticle,"e/F:pos[3]/F");
  //it is the same, only one branch


  //file the branch;
  //
  sParticle.energy=11.;
  sParticle.position[0]=7.7;
  sParticle.position[1]=5.7;
  sParticle.position[2]=4.7;

  t.Fill();

  sParticle.energy=14.;
  sParticle.position[0]=7.9;
  sParticle.position[1]=5.4;
  sParticle.position[2]=4.2;

  t.Fill();
  t.Scan("Energy:position");

//write into the root file....

}

void example_5()
{

  TTree t("t","class tree");

  //----------load class method 1-------------
  //
 /* string aclic_compile=".L ./particle_class.h";*/
  //gROOT->ProcessLine(aclic_compile.c_str());

  //particle_class c_particle;
  //c_particle.Set_energy(10.0);
  //c_particle.Set_position(111,222,333);
  
  //cout<<"energy si:"<<c_particle.Get_energy()<<endl;
  //cout<<"energy si:"<<c_particle.Get_position()[0]<<endl;


  //----------load class method 2-------------
  /*
   *define a class ,and compile it to .so file.
   *        
   *        x filename++
   *i.g you have myclass.h; type in >x myClass.h++
   *then load the .so file
   *
   *
   */
  //
 string aclic_compile=".x ./particle_class.h++";
  gROOT->ProcessLine(aclic_compile.c_str());

  gSystem->Load("particle_class_h.so");

  particle_class* sParticle=new particle_class();

  t.Branch("Particle_info",&sParticle);

  sParticle->Set_energy(11);
  sParticle->Set_position(123,222,333);
  t.Fill();

  //t.Scan();
  //the name is corresponding to the memeber name of class
  //t.Scan("energy:position");
  //t.Scan("energy:position[0]:position[1]:position[2]");
  t.Scan("energy:position[]");

  //t.GetListOfLeaves()->Print();
  //t.StartViewer();
  //t.TBrowser();


}
