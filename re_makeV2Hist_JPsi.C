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
#include "makeV2Hist_JPsi.C"
#include "setvariables.h"

using namespace std;

void re_makeV2Hist_JPsi()
{


  for(int i = 0; i<npt1; i++)
  {
	makeV2Hist_JPsi(cLow, cHigh, ptBin[i], ptBin[i+1], yLow2, yHigh, ctauBin[i], 0, 2.6 , 3.5 , true, true, true);
    
  }
  
  for(int i=npt1; i < npt ; i++){
    
      makeV2Hist_JPsi(cLow, cHigh, ptBin[i], ptBin[i+1], yLow1, yHigh, ctauBin[i], 0, 2.6, 3.5, true, true, true);
 }
  
  
  }
