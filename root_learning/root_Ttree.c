{

  /*

//***************************************************
//1:Generate a tree
//****************************************************
// example of macro to read data from an ascii file and
// create a root file with an histogram and a TTree
gROOT->Reset();
// the structure to hold the variables for the branch
struct staff_t {
Int_t cat;
Int_t division;
Int_t flag;
Int_t age;
Int_t service;
Int_t children;
Int_t grade;
Int_t step;
Int_t nation;
Int_t hrweek;
Int_t cost;
};

staff_t staff;
// continued...
// open the ASCII file
FILE *fp = fopen("cernstaff.dat","r");

char line[81];
// create a new ROOT file
TFile *f = new TFile("staff.root","RECREATE");

// create a TTree
TTree *tree = new TTree("T","staff data from ascii file");

// create one branch with all information from the stucture
tree->Branch("staff",&staff.cat,"cat/I:division:flag:age:service:
children:grade:step:nation:hrweek:cost");

// fill the tree from the values in ASCII file
while (fgets(&line,80,fp)) {
sscanf(&line[0],"%d%d%d%d",&staff.cat,&staff.division,
&staff.flag,&staff.age);

sscanf(&line[13],"%d%d%d%d",&staff.service,&staff.children,
&staff.grade,&staff.step);

sscanf(&line[24],"%d%d%d",&staff.nation,&staff.hrweek,
&staff.cost);

tree->Fill();
}


// check what the tree looks like
tree->Print();

fclose(fp);

f->Write();
*/



//***********************************
//2:show an entry
//**********************************

/*TFile f("staff.root");*/
/*T->Show(10);*/

//***********************************
//3:Print the Tree Structure with TTree::Print
////**********************************
/*T->Print();*/


//***********************************
//4: Scan a Variable the Tree with TTree::Scan
////**********************************

/*T->Scan("Cost:Age:Children");*/


//***********************************
//5: The Tree Viewer
//////**********************************

TFile f("staff.root");
T->StartViewer();

}
