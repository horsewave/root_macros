#include<stdio.h>
#include<iostream.h>
#include<list>

typedef struct {
  int icry1;
  int icry2;
  short int icry1_ofov_scatter_z_pos;
  short int icry2_ofov_scatter_z_pos;
  char type;
} Coincidence;



class list_coincidence:public TObject{
{
    public:
    list_coincidence(){   }//empty constructor;

    //---setter----
  

  ClassDef(particle_class,1)
    /*
     *ClassDef is a C preprocessor macro,
     *that must be used if your class derives from TObject
     */

    private:

 list<Coincidence> m_coincidence_list;


};

