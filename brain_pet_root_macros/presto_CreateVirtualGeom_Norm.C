{
	string fname="/data/Pietrzyk5/mg/gpupet/water_cylinder_true_scattered.VirtualGeometry.dat";
	FileAccess f(fname);
	f.open_readonly();
	VirtualGeometry* vgeom=new VirtualGeometry(&f);
	f.close();
	
	f.open_readonly();
	VirtualGeometry* vgeom_norm=new VirtualGeometry(&f);
	f.close();
	vgeom_norm->reset_data();
	
	int ishift=0;
	int irot=32;
	
	for (int i=0; i<irot; i++)
	{
		cout << "i=" << i << endl;
		vgeom_norm->rotate_and_add(vgeom,ishift);
		ishift+=12;
	}
		
		f.open_readonly();
		VirtualGeometry* vgeom_ideal=new VirtualGeometry(&f);
		f.close();
		vgeom_ideal->reset_data();
		
		//	fname="/data/Pietrzyk5/mg/Reco/cylinder_atten.VirtualGeometry.dat";
		//	FileAccess f2(fname);
		//	f2.open_readonly();
		//	VirtualGeometry* vgeom_atten=new VirtualGeometry(&f2);
		//	f2.close();
		
		vgeom->get_number_of_locked_channels();
		vgeom_norm->get_number_of_locked_channels();
		
		CylinderSource *cyl=new CylinderSource(0.125,-0.1,0.1);
		vgeom_ideal->fill_with_intersection_length(cyl);
		
		RVirtualGeometry* rvgeom=new RVirtualGeometry(vgeom,"CylinderSim");
		rvgeom->draw_sinogram(38,38);
		
		RVirtualGeometry* rvgeom_2=new RVirtualGeometry(vgeom_norm,"Cylinder");
		rvgeom_2->draw_sinogram(38,38);
		
		
		vgeom_ideal->divide_by(vgeom_norm);
		
		//	RVirtualGeometry* rvgeom_3=new RVirtualGeometry(vgeom_atten,"Attenuation");
		//	rvgeom_3->draw_sinogram(38,38);
		
		
		

		
		string fname_norm2="/data/Pietrzyk5/mg/Reco/BrainPET_cylinder_norm_scatter_gpu.VirtualGeometry.dat";
	    FileAccess f3(fname_norm2);
	    f3.open_writeonly();
	    vgeom_ideal->write_into_file(&f3);
	    f3.close();
	
		
		}
