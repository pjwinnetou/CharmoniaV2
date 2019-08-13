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
#include "doFitJpsi.C"
#include "setvariables.h"

using namespace std;

void recursiveDoFitJpsi()
{


  for(int i = 0; i<npt1; i++)
  {
	doFitJpsi(ptBin[i], ptBin[i+1], yLow2, yHigh, cLow, cHigh, ctauBin[i], true, 0);
    cout << "slkfjas;ldkfja;lsdkjf; : " << ptBin[i] << " " << ptBin[i+1] << " " << yLow2 << " " << yHigh << " " << ctauBin[i]  << endl;
  }
  
  for(int i=npt1; i < npt ; i++){
    
    doFitJpsi(ptBin[i], ptBin[i+1], yLow1, yHigh, cLow, cHigh, ctauBin[i], true, 0);
    cout << "slkfjas;ldkfja;lsdkjf; : " << ptBin[i] << " " << ptBin[i+1] << " " << yLow1 << " " << yHigh << " " << ctauBin[i]  << endl;
 }
  
  
  }
