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
#include <TLegend.h>
#include <TLatex.h>
#include <TCanvas.h>
#include <TText.h>
#include <TString.h>
#include <TMath.h>
using namespace std;

void ctau_eff_MC_pmpt_nonpmpt_jaebeomEx(float ptlow = 3, float pthigh = 5, float ylow = 1.6, float yhigh = 2.4 ){

  const int nbin = 200; 
  TH1D *h1 = new TH1D("h1","C#tau Efficiency (MC);C#tau;Efficiency",nbin,-0.2,1);
  TH1D *h2 = new TH1D("h2","C#tau Efficiency (MC);C#tau;Efficiency",nbin,-0.2,1);
  TH1D *hctau1 = new TH1D("hctau1","hctau;ctau;n",nbin,-0.2,1);
  TH1D *hctau2 = new TH1D("hctau2","hctau;ctau;n",nbin,-0.2,1);


  TCanvas *c1 = new TCanvas("c1","canvas",800,650);

  TFile *f1 = new TFile("../OniaSkimSimple_isMC1.root","read");
  TFile *f2 = new TFile("../OniaSkimSimple_nonPmpt_isMC1.root");

  TTree *tree1 = (TTree*) f1 -> Get("dimutree");
  TTree *tree2 = (TTree*) f2 -> Get("dimutree");


  TString label = Form("pt%.1f-%.1f_y%.1f-%.1f",ptlow, pthigh, ylow, yhigh);

  Int_t nDimu1;
  const int maxBrancSize = 1000;
  Int_t nDimu2;
  Float_t ctau1[maxBrancSize];
  Float_t ctau2[maxBrancSize];
  Float_t pt1[maxBrancSize];
  Float_t y1[maxBrancSize];
  Float_t pt2[maxBrancSize];
  Float_t y2[maxBrancSize];

  tree1 -> SetBranchAddress("ctau", ctau1);
  tree1 -> SetBranchAddress("nDimu", &nDimu1); 
  tree1 -> SetBranchAddress("pt", pt1);
  tree1 -> SetBranchAddress("y", y1); 

  tree2 -> SetBranchAddress("ctau", ctau2);
  tree2 -> SetBranchAddress("nDimu", &nDimu2);
  tree2 -> SetBranchAddress("pt", pt2);
  tree2 -> SetBranchAddress("y", y2); 

  Int_t nevt1 = tree1 -> GetEntries();

  Int_t nevt2 = tree2 -> GetEntries();


  Float_t eff1[nbin];
  Float_t num1[nbin];
  Float_t dem1;

  Float_t eff2[nbin];
  Float_t num2[nbin];
  Float_t dem2;



  Float_t Peff_40;
  Float_t Peff_41;
  Float_t Peff_42;

  // tree1 -> Draw("ctau>>hctau");

  for(int j = 0; j < nevt1; j++){

    tree1 -> GetEntry(j);
    for(int i = 0; i<nDimu1 ; i++) {
      if(pt1[i]>ptlow && pt1[i]<pthigh && TMath::Abs(y1[i])>ylow && TMath::Abs(y1[i])<yhigh){
        hctau1 -> Fill(ctau1[i]);
      }
    }
  }


  dem1 = hctau1 -> Integral();

  for (int k=0; k<nbin; k++){
    num1[k] = hctau1 -> Integral(k+1,nbin);
    eff1[k] = 1-(num1[k]/dem1);
    h1 -> SetBinContent(k+1, eff1[k]);
  }


  for(int j = 0; j < nevt2; j++){

    tree2 -> GetEntry(j);
    for(int i = 0; i<nDimu2 ; i++) {
      if(pt2[i]>ptlow && pt2[i]<pthigh && TMath::Abs(y2[i])>ylow && TMath::Abs(y2[i])<yhigh){
        hctau2 -> Fill(ctau2[i]);
      }
    }
  }
  dem2 = hctau2 -> Integral();

  for (int k=0; k<nbin; k++){
    num2[k] = hctau2 -> Integral(k+1,nbin);
    eff2[k] = (num2[k]/dem2);

    h2 -> SetBinContent(k+1, eff2[k]);
  }

  Double_t pr_eff_90 = 0;
  Double_t nonpr_eff_90 = 0;
  Double_t eff1_90 = 0;
  Double_t eff2_RejRate = 0;


  for(int ibin = 0; ibin<nbin; ibin++){
    if(eff1[ibin]>0.9){
      pr_eff_90 = ibin+1; 
      eff1_90 = eff1[ibin];
      eff2_RejRate = eff2[ibin];
      break;}
    }



Double_t  ctau_pr_90 = (-0.2 + ( (1-(-0.2))/nbin)*pr_eff_90) - ( (1-(-0.2))/nbin/2);



  cout << ptlow << " " << pthigh << " " << ylow << " " << yhigh << " " << ctau_pr_90 << " " << eff1_90 << " " << eff2_RejRate << endl;

  Peff_40 = h1-> GetBinContent(40);
  Peff_41 = h1 -> GetBinContent(41);
  Peff_42 = h1 -> GetBinContent(42);

  Float_t Neff_40 = h2 -> GetBinContent(40);
  Float_t Neff_41 = h2 -> GetBinContent(41);
  Float_t Neff_42 = h2 -> GetBinContent(42);

  TString fileLabel = "file_" + label + ".txt";
  
  ofstream file;
  
  file.open(Form("%s",fileLabel.Data()));

  file << ptlow << " " << pthigh << " " << ylow << " " << yhigh << " " << ctau_pr_90 << " " << eff1_90 << " " << eff2_RejRate << endl;
file.close();

;
  TH1D* h3 = new TH1D("h3",";;;",7,0,7);
  h3->SetBinContent(1, ptlow);
  h3->SetBinContent(2, pthigh);
  h3->SetBinContent(3, ylow);
  h3->SetBinContent(4, yhigh);
  h3->SetBinContent(5, ctau_pr_90);
  h3->SetBinContent(6, eff1_90);
  h3->SetBinContent(7, eff2_RejRate);

  c1 -> cd();  
  h1 -> Draw();
  h2 -> Draw("same");
  h1 -> SetLineColor(kRed);
  h2->SetLineColor(kBlue);


   float pos_x = 0.23;
     float pos_x_mass = 0.53;
      float pos_y = 0.76;
       float pos_y_diff = 0.071;
        int text_color = 1;
        float text_size = 16;
         TString perc = "%";


  TLegend *leg = new TLegend(0.6, 0.35, 0.55, 0.5);
  SetLegendStyle(leg);
  leg -> AddEntry(h1, "C#tau efficiency (Prompt)", "l");
  leg -> AddEntry(h2, "C#tau efficiency (Non-Prompt)", "l");
  leg -> Draw ("same");

 if(ptlow==0) drawText(Form("p_{t}^{#mu#mu} < %.1f gev/c",pthigh ),pos_x_mass,pos_y,text_color,text_size);
  else if(ptlow!=0) drawText(Form("%.1f < p_{t}^{#mu#mu} < %.1f gev/c",ptlow, pthigh ),pos_x_mass,pos_y,text_color,text_size);
    if(ylow==0) drawText(Form("|y^{#mu#mu}| < %.1f",yhigh ), pos_x_mass,pos_y-pos_y_diff,text_color,text_size);
    else if(ylow!=0) drawText(Form("%.1f < |y^{#mu#mu}| < %.1f",ylow, yhigh ), pos_x_mass,pos_y-pos_y_diff,text_color,text_size);
  //drawtext(form("p_{t}^{#mu} > %.1f gev/c", simuptcut ), pos_x_mass,pos_y-pos_y_diff*2,text_color,text_size);
   




  c1 -> SaveAs(Form("ctau_efficiency_MC_pmpt_nonpmpt_%s.pdf",label.Data())); 

  TFile *wf = new TFile(Form("ctau_efficiency_MC_pmpt_nonpmpt_%s.root",label.Data()),"recreate");
  wf->cd();
  c1->Write();
  h1->Write();
  h2->Write();
  h3->Write();

}
