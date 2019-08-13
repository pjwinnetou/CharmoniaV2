#include <ctime>
#include <TLorentzVector.h>
#include "commonUtility.h"
#include "HiEvtPlaneList.h"
#include <iostream>
#include "TFile.h"
#include "TStyle.h"
#include "TMath.h"
#include "TROOT.h"
#include "TSystem.h"
#include "Style_jaebeom.h"
#include "ctau_eff_MC_pmpt_nonpmpt_jaebeomEx.C"
#include "setvariables.h"

using namespace std;

void makeCtau_file(int version)
{


ifstream fich;
ofstream write;
  
  write.open(Form("calcul_ctau_ver%d.txt",verion));
  
  float a,b,c,d,e,f,g;


  for(int i = 0; i<npt1; i++)
  {
    ctau_eff_MC_pmpt_nonpmpt_jaebeomEx(ptBin[i], ptBin[i+1], yLow2, yHigh);
    fich.open(Form("file_pt%.1f-%.1f_y%.1f-%.1f.txt",ptBin[i], ptBin[i+1], yLow2, yHigh));
    fich >> a >> b >> c >> d >> e >> f >> g ;
    write << a << " " << b << " " << c << " " << d << " " << e << " " << f << " " << g << "\n" ;
    fich.close();
  }
  for(int i=npt1; i < npt; i++){
    ctau_eff_MC_pmpt_nonpmpt_jaebeomEx(ptBin[i], ptBin[i+1], yLow1, yHigh);
 
    fich.open(Form("file_pt%.1f-%.1f_y%.1f-%.1f.txt",ptBin[i], ptBin[i+1], yLow1, yHigh));
    fich >> a >> b >> c >> d >> e >> f >> g ;
    write << a << " " << b << " " << c << " " << d << " " << e << " " << f << " " << g << "\n" ;
  fich.close();
  }
  
  
  }
