

/*
 *   This root file is to draw the Transaxial view and Axial of a scanner
 *   very good marco. Tell you how to draw using just lines .
 *
 *
 *
 *
 */
void GenericScannerAll()
{
  run();

}

void run()
{
  float xoff=-0.2;
  float yoff=0.0;
  
  TCanvas* c=new TCanvas("GenericScanner","genericScanner",1200,800);


  
  //---------------------------Transaxial Component----------------------------
   
  //
  // number cassete
  int nc=80;
  float R=0.7;
  float RR=R*1.03;
  float RRR=R*0.99;

  double PI=3.141592653589793;
  
  // angle for each cassete
  double Dangle=2*PI/float(nc);

  float stretch=12.0/8.0;

  //Set world coordinate system for the pad.(x1,y1,x2,y2)
  gPad->Range(-1.0,-1.0,1.0*stretch*1.4,1.0);




  
  for (int i=0; i<=nc; i++)
    {
      double angle=Dangle*float(i);
      double x=R*sin(angle);
      double y=R*cos(angle);

      //TMarker* m=new TMarker(x,y,20);
      //m->Draw();

      if (i<80)
	{
	  char TT[100];
	  if (i<1000)	    
	    {
	      sprintf(TT,"%d",i);
	    }
	  else
	    {
	      sprintf(TT,"...");
	    }
     double xx=-1.07*R*sin(angle+Dangle*0.5);
	  double yy=1.07*R*cos(angle+Dangle*0.5);  

	  TText* t=new TText(xx+xoff,yy+yoff,TT);
	  t->SetTextAlign(22);
	  t->SetTextAngle((angle+(Dangle*0.5))*57.3);
	  t->SetTextColor(1);
	  t->SetTextSize(.024);
	 
    if ((i%2)==0) t->Draw();

	  
	 	}


        double angle2=Dangle*float(i+1);
      double x2=R*sin(angle2);
      double y2=R*cos(angle2);

      TLine* l=new TLine(x+xoff,y+yoff,x2+xoff,y2+yoff);
      l->SetLineWidth(2);
      l->SetLineColor(14);
      l->Draw();


      double x3=RR*sin(angle);
      double y3=RR*cos(angle);
      TLine* l=new TLine(x+xoff,y+yoff,x3+xoff,y3+yoff);
      l->SetLineWidth(4);
      l->SetLineColor(14);
      l->Draw();

x3=RRR*sin(angle);
      y3=RRR*cos(angle);
      TLine* l=new TLine(x+xoff,y+yoff,x3+xoff,y3+yoff);
      l->SetLineWidth(4);
      l->SetLineColor(14);
      l->Draw();



    }

 //---horizonzal line
  //TLine* ll=new TLine(-R+xoff,0+yoff,R+xoff,0+yoff);
  TLine* ll=new TLine(-R+xoff,0+yoff,2.2+xoff,0+yoff);
  ll->SetLineColor(14);
  ll->SetLineStyle(2);
  ll->SetLineWidth(2);
  ll->Draw();

//vertical line
  TLine* ll=new TLine(0+xoff,-R+yoff,0+xoff,R+yoff);
  ll->SetLineColor(14);
  ll->SetLineStyle(2);
  ll->SetLineWidth(2);
  ll->Draw();


//diagnle

  TLine* ll=new TLine(-0.535+xoff,-0.767+yoff,0.69+xoff,0.67+yoff);
  ll->SetLineColor(1);
  ll->SetLineStyle(1);
  ll->SetLineWidth(3);
  ll->Draw();




  TMarker* m=new TMarker(0.52+xoff,0.474+yoff,20);
  m->SetMarkerColor(4);
  m->SetMarkerSize(1.2);
  m->Draw();


  TMarker* m=new TMarker(-0.380+xoff,-0.588+yoff,20);
  m->SetMarkerColor(4);
  m->SetMarkerSize(1.2);
  m->Draw();



  TText* t=new TText(0.0+xoff,0.9+yoff,"Transaxial View");
  t->SetTextAlign(22);
  t->SetTextSize(.04);
  t->Draw();




  int nc=80;
  float R=0.7;
  float RR=R*1.04;
  float RRR=R*0.96;

  double PI=3.141592653589793;
  double Dangle=2*PI/float(nc);
  //gPad->Range(-1.0,-1.0,1.0,1.0);

  float xoff2=1.4;

 
  float DZ=0.4;
  for (int i=0; i<=nc; i++)
    {
      double angle=Dangle*float(i);
      double x=R*sin(angle);
      double y=R*cos(angle);


      TLine* l=new TLine(-DZ+xoff2,y+yoff,DZ+xoff2,y+yoff);
      l->SetLineWidth(2);
      l->SetLineColor(14);
      l->Draw();

    }



 int nrings=7;
  float dz=DZ/float(nrings);
  for (i=0; i<=2*nrings; i++)
    {
      float z=-DZ+(dz*float(i));
      TLine* l=new TLine(z+xoff2,-R+yoff,z+xoff2,R+yoff);
      l->Draw();

      if (((i%2)==0) && (i<14))
	{
	  char tt[100];
	  sprintf(tt,"%d",i);
	  TText* t=new TText(z+xoff2+dz*0.5,0.73,tt);
	  t->SetTextAlign(22);
	  t->SetTextColor(1);
	  t->SetTextSize(.024);
	  t->Draw();
	}

    }



TText* t=new TText(0.0+xoff2,0.9+yoff,"Axial View");
  t->SetTextAlign(22);
  t->SetTextSize(.04);
  t->Draw();



  TMarker* m=new TMarker(0.2+xoff2,0.474+yoff,20);
  m->SetMarkerColor(4);
  m->SetMarkerSize(1.2);
  m->Draw();

  TMarker* m=new TMarker(-0.2+xoff2,-0.588+yoff,20);
  m->SetMarkerColor(4);
  m->SetMarkerSize(1.2);
  m->Draw();


  TLine* l=new TLine(0.52+xoff,0.474+yoff,0.2+xoff2,0.474+yoff);
  l->SetLineStyle(3);
  l->SetLineWidth(2);
  l->SetLineColor(4);
  l->Draw();


  TLine* l=new TLine(-0.380+xoff,-0.588+yoff,-0.2+xoff2,-0.588+yoff);
  l->SetLineStyle(3);
  l->SetLineWidth(2);
  l->SetLineColor(4); //14
  l->Draw();
  
  
  TLine* l=new TLine(0.2+xoff2,0.474+yoff,-0.2+xoff2,-0.588+yoff);
  l->SetLineColor(1);
  l->SetLineStyle(1);
  l->SetLineWidth(3);

  l->Draw();


  //----others 
  TArrow *arrow = new TArrow(-0.2042642,0.002590674,-0.8704013,0.1891192,0.01,"<|>");
   arrow->SetFillColor(1);
   arrow->SetFillStyle(1001);
   arrow->SetLineWidth(3);
   arrow->SetAngle(30);
   arrow->Draw();

   TText* t=new TText(-0.53,0.14,"R");
   t->SetTextAlign(22);
   t->SetTextColor(1);
   t->SetTextSize(.04);
   t->Draw();


   arrow = new TArrow(1.001003,-0.7746114,1.804515,-0.7746114,0.02,"<|>");
   arrow->SetFillColor(1);
   arrow->SetFillStyle(1001);
   arrow->SetLineWidth(3);
   arrow->SetAngle(24);
   arrow->Draw();


  TLatex *   tex = new TLatex(1.390569,-0.85,"Z");
   tex->SetLineWidth(2);
   tex->SetTextSize(.04);
   tex->Draw();

}




