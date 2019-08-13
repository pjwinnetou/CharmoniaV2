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
#include "doSimultaneousv2MassFit.C"
#include "setvariables.h"

using namespace std;

void re_doSimultaneousv2MassFit()
{


  for(int i = 0; i<npt1; i++)
  {
	doSimultaneousv2MassFit(cLow, cHigh, ptBin[i], ptBin[i+1], yLow2, yHigh, ctauBin[i], 0.0,  2.6 , 3.5 , true, fpol2);
    
  }
  
  for(int i=npt1; i < npt ; i++){
    
    doSimultaneousv2MassFit(cLow, cHigh,ptBin[i], ptBin[i+1], yLow1, yHigh, ctauBin[i], 0.0, 2.6, 3.5, true, fpol2);
 }
  
  
  }
