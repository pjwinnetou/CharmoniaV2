#include <ctime>
#include <TLorentzVector.h>
#include "commonUtility.h"
#include "HiEvtPlaneList.h"
#include <iostream>
#include "TFile.h"
#include "TStyle.h"
#include "Style_jaebeom.h"

using namespace std;

void ctau_eff_MC(){
  
  
 TH1D *h1 = new TH1D("h1","C#tau Efficiency (MC);C#tau;Efficiency",100,-0.2,0.2);
 TH1D *hctau = new TH1D("h2","hctau;ctau;n",100,-0.2,0.2);

 
 
 TCanvas *c1 = new TCanvas("c1","canvas",800,650);
 TCanvas *c2 = new TCanvas("c2","canvas",800,650);
  
 TFile *f1 = new TFile("OniaSkimSimple_isMC1.root","read");

 TTree *ctauTree = (TTree*) f1 -> Get("dimutree");



 Int_t nDimu;
 const int maxBrancSize = 1000;
 Float_t ctau[maxBrancSize];
 ctauTree -> SetBranchAddress("ctau", ctau);
 ctauTree -> SetBranchAddress("nDimu", &nDimu);

 Int_t nevt = ctauTree -> GetEntries();


 Float_t eff[100];
 Float_t num[100];
 Float_t dem;

 // ctauTree -> Draw("ctau>>hctau");

 for(int j = 0; j < nevt; j++){

   ctauTree -> GetEntry(j);
   for(int i = 0; i<nDimu ; i++) {
     hctau -> Fill(ctau[i]);
   }
 }


 dem = hctau -> Integral();

 for (int k=0; k<100; k++){
   num[k] = hctau -> Integral(k+1,100);
   eff[k] = 1-(num[k]/dem);
   h1 -> SetBinContent(k+1, eff[k]);
 }

   
   
  
  
  
  c1 -> cd();  
  h1 -> Draw();
 // hctau -> Draw("same");
  c1 -> SaveAs("ctau_efficiency_MC.pdf");
 
 c2 -> cd();
 hctau -> Draw();
 c2 -> SaveAs("hctau.pdf");
 
  
  }
