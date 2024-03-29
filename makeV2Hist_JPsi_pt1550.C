#include <iostream>
#include "commonUtility.h"
#include "cutsAndBinUpsilonV2.h"
#include "HiEvtPlaneList.h"
#include "Style_jaebeom.h"
#include "tdrstyle.C"
#include "CMS_lumi_v2mass.C"
#include "rootFitHeaders.h"
#include <RooGaussian.h>
#include <RooCBShape.h>
#include <RooWorkspace.h>
#include <RooChebychev.h>
#include <RooPolynomial.h>
#include "RooPlot.h"
#include "TText.h"
#include "TArrow.h"
#include "TFile.h"

using namespace std;
using namespace RooFit;

using namespace hi;

void GetHistSqrt(TH1D* h1 =0, TH1D* h2=0);

void makeV2Hist_JPsi_pt1550(int cLow = 40, int cHigh = 80,
                double ptLow = 15.0, double ptHigh = 50.0, 
                double yLow = 0, double yHigh=2.4, double ctauCut=0.025,
                float SiMuPtCut = 0, float massLow = 2.75, float massHigh =3.45, bool dimusign=true, bool fAccW = true, bool fEffW = true)
{
  //Basic Setting
  gStyle->SetOptStat(0);
  setTDRStyle();
  writeExtraText= true;
  int iPeriod = 2;
  int iPos = 33;
  TH1::SetDefaultSumw2();
  TString kineLabel = getKineLabel(ptLow, ptHigh, yLow, yHigh, ctauCut, SiMuPtCut, cLow, cHigh) ;
  TString dimusignString;
  if(dimusign) dimusignString = "OS";
  else if(!dimusign) dimusignString = "SS";


  //READ Input Skimmed File
  TString f1 = "/home/deathold/work/CMS/analysis/Upsilon_v2/UpsilonPbPb2018_v2/skimmedFiles/OniaFlowSkim_JpsiTrig_DBPD_isMC0_HFNom_190716.root";
  TString f2 = "/home/deathold/work/CMS/analysis/Upsilon_v2/UpsilonPbPb2018_v2/skimmedFiles/OniaFlowSkim_JpsiTrig_DBPsiPeriPD_isMC0_HFNom_190716.root";
  
  TChain *tree = new TChain("mmepevt");
  tree->Add(f1.Data());
  tree->Add(f2.Data());

  
  //SetBranchAddress
  const int nMaxDimu = 1000;
  float mass[nMaxDimu];
  float qxa[nMaxDimu];
  float qya[nMaxDimu];
  float qxb[nMaxDimu]; 
  float qyb[nMaxDimu];
  float qxc[nMaxDimu];
  float qyc[nMaxDimu];
  float qxdimu[nMaxDimu];
  float qydimu[nMaxDimu];
  float pt[nMaxDimu];
  float y[nMaxDimu]; 
  float pt1[nMaxDimu];
  float pt2[nMaxDimu];
  float eta[nMaxDimu];
  float eta1[nMaxDimu];
  float eta2[nMaxDimu];
  Int_t cBin;
  Int_t event; 
  Int_t nDimu; 
  float vz;
  int recoQQsign[nMaxDimu];
  float ctau[nMaxDimu];

  TBranch *b_event;
  TBranch *b_cBin;
  TBranch *b_nDimu;
  TBranch *b_vz;
  TBranch *b_mass;
  TBranch *b_recoQQsign;
  TBranch *b_pt;
  TBranch *b_y;
  TBranch *b_eta;
  TBranch *b_eta1;
  TBranch *b_eta2;
  TBranch *b_pt1;
  TBranch *b_pt2;
  TBranch *b_qxa;
  TBranch *b_qxb;
  TBranch *b_qxc;
  TBranch *b_qxdimu;
  TBranch *b_qya;
  TBranch *b_qyb;
  TBranch *b_qyc;
  TBranch *b_qydimu;
  TBranch *b_ctau;
  

  tree -> SetBranchAddress("event", &event, &b_event);
  tree -> SetBranchAddress("cBin", &cBin, &b_cBin);
  tree -> SetBranchAddress("nDimu", &nDimu, &b_nDimu);
  tree -> SetBranchAddress("vz", &vz, &b_vz);
  tree -> SetBranchAddress("recoQQsign", recoQQsign, &b_recoQQsign);
  tree -> SetBranchAddress("mass", mass, &b_mass);
  tree -> SetBranchAddress("y", y, &b_y);
  tree -> SetBranchAddress("pt", pt, &b_pt);
  tree -> SetBranchAddress("pt1", pt1, &b_pt1);
  tree -> SetBranchAddress("pt2", pt2, &b_pt2);
  tree -> SetBranchAddress("eta", eta, &b_eta);
  tree -> SetBranchAddress("eta1", eta1, &b_eta1);
  tree -> SetBranchAddress("eta2", eta2, &b_eta2);
  tree -> SetBranchAddress("qxa", qxa, &b_qxa);
  tree -> SetBranchAddress("qxb", qxb, &b_qxb);
  tree -> SetBranchAddress("qxc", qxc, &b_qxc);
  tree -> SetBranchAddress("qxdimu", qxdimu, &b_qxdimu);
  tree -> SetBranchAddress("qya", qya, &b_qya);
  tree -> SetBranchAddress("qyb", qyb, &b_qyb);
  tree -> SetBranchAddress("qyc", qyc, &b_qyc);
  tree -> SetBranchAddress("qydimu", qydimu, &b_qydimu);
  tree -> SetBranchAddress("ctau3D", ctau,&b_ctau);

  const int nMassBin = 6;
  
  float massBinDiff[nMassBin+1]={2.75, 2.9, 3.04, 3.10, 3.16, 3.28, 3.45};

  float massBin_[nMassBin+1];
  
  kineLabel = kineLabel + Form("_m%.1f-%.1f",massLow,massHigh) + "_" + dimusignString;
  
  for(int i=0; i<=nMassBin; i++){
    massBin_[i] = massBinDiff[i];
    //massBin[i] = 2.6 + massBinDiff[i]*0.02;
  }

  float* massBin;
  int nMassBin_;
  massBin = massBin_; nMassBin_ = nMassBin;

  int bfevt =-1;
  int afevt =-1;

  double mass_low_SB1 = 2.6; 
  double mass_high_SB1 = 2.9; 
  double mass_low_SB = 3.0; 
  double mass_high_SB = 3.2; 
  double mass_low_SB2 = 3.3; 
  double mass_high_SB2 = 3.5; 

  int nQBin = 200;
  const int nMass_div = 3;

  TString fSB[nMass_div] = {"SB1 (2.6<m<2.9)","SB2 (3.0<m<3.2)","S (3.3<m<3.5)"};
  
  //Define drawing histogram
  TH1D* h_v2_1[nMass_div];
  TH1D* h_v2_2[nMass_div];
  TH1D* h_v2_3[nMass_div];
  TH1D* h_v2_4[nMass_div];

  double Q_avg_low = -6500;
  double Q_avg_high = 27000;
  double Q_avg_low_dimu = -150;
  double Q_avg_high_dimu = 150;
  for(int imass=0; imass<nMass_div;imass++){
    h_v2_1[imass] = new TH1D(Form("h_v2_1_%d",imass),";#LTQ_{2}Q_{2A}^{*}#GT;counts",100,Q_avg_low_dimu,Q_avg_high_dimu);
    h_v2_2[imass] = new TH1D(Form("h_v2_2_%d",imass),";#LTQ_{2A}Q_{2B}^{*}#GT;counts",nQBin,Q_avg_low,Q_avg_high);
    h_v2_3[imass] = new TH1D(Form("h_v2_3_%d",imass),";#LTQ_{2A}Q_{2C}^{*}#GT;counts",nQBin,Q_avg_low,Q_avg_high);
    h_v2_4[imass] = new TH1D(Form("h_v2_4_%d",imass),";#LTQ_{2B}Q_{2C}^{*}#GT;counts",nQBin,Q_avg_low,Q_avg_high);
  }
  
  TH1D* h_v2_num_q1 = new TH1D("h_v2_num_q1",";m_{#mu^{+}#mu^{-}};#LTQ_{2}Q_{2A}^{*}#GT" ,nMassBin,massBin);
  TH1D* h_v2_den_q2 = new TH1D("h_v2_num_q2",";m_{#mu^{+}#mu^{-}};#LTQ_{2A}Q_{2B}^{*}#GT",nMassBin,massBin);
  TH1D* h_v2_den_q3 = new TH1D("h_v2_num_q3",";m_{#mu^{+}#mu^{-}};#LTQ_{2A}Q_{2C}^{*}#GT",nMassBin,massBin);
  TH1D* h_v2_den_q4 = new TH1D("h_v2_num_q4",";m_{#mu^{+}#mu^{-}};#LTQ_{2B}Q_{2C}^{*}#GT",nMassBin,massBin);
  
  TH1D* h_mass = new TH1D("h_mass",";m_{#mu^{+}#mu^{-}};Counts",42,massLow,massHigh);

  RooRealVar* massVar = new RooRealVar("mass","mass variable",0,200,"GeV/c^{2}");

  RooArgSet* argSet = new RooArgSet(*massVar);
  RooDataSet* dataSet = new RooDataSet("dataset","a dataset",*argSet); 

  const static int countMax = 1000000;
  vector<vector<double>> v2_1(nMassBin,vector<double> (countMax,0));
  vector<vector<double>> v2_2(nMassBin,vector<double> (countMax,0));
  vector<vector<double>> v2_3(nMassBin,vector<double> (countMax,0));
  vector<vector<double>> v2_4(nMassBin,vector<double> (countMax,0));
  vector<vector<double>> v2_1_raw(nMassBin,vector<double> (countMax,0));
  vector<vector<double>> v2_2_raw(nMassBin,vector<double> (countMax,0));
  vector<vector<double>> v2_3_raw(nMassBin,vector<double> (countMax,0));
  vector<vector<double>> v2_4_raw(nMassBin,vector<double> (countMax,0));

  vector<double> v2_1_avg(nMassBin,0);
  vector<double> v2_2_avg(nMassBin,0);
  vector<double> v2_3_avg(nMassBin,0);
  vector<double> v2_4_avg(nMassBin,0);
   
  int dbcount=0;
  vector<unsigned int> count(nMassBin,0);
  vector<unsigned int> count_ss(nMassBin,0);
  
  bool dimusignPass = true;

  int nDimuPass=0;
  int nDimu_one=0;
  int nDimu_more=0;


  Int_t nEvt = tree->GetEntries();
  cout << "nEvt : " << nEvt << endl;
  
  //Begin Loop
  for(int i=0; i<nEvt; i++){
    tree->GetEntry(i);
    nDimuPass=0;
    
    if(!(cBin>cLow&&cBin<cHigh)) continue; 
    if(fabs(vz)>=15) continue; // 15 cm

    //Remove double candidate
    for(int j=0; j<nDimu; j++){
      if(dimusign) {dimusignPass = (recoQQsign[j] == 0) ? true : false;}
      else if(!dimusign) {dimusignPass = (recoQQsign[j] == 0) ? false : true;}
      if(dimusignPass == false) continue; // dimuon charge selection
      
      if( (mass[j]<massLow) || (mass[j]>massHigh) ) continue; // dimuon mass range
      if(pt[j]>ptLow&&pt[j]<ptHigh&&abs(y[j])<yHigh&&abs(y[j])>yLow&&pt1[j]>SiMuPtCut&&pt2[j]>SiMuPtCut&&abs(eta1[j])<2.4&&abs(eta2[j])<2.4){
        nDimuPass++;
      }
    }

    if(nDimuPass>1) {nDimu_more++; continue;}
    nDimu_one++;

    // Fill Dimuon Loop
    for(int j=0; j<nDimu; j++){
      if(dimusign) {dimusignPass = (recoQQsign[j] == 0) ? true : false;}
      else if(!dimusign) {dimusignPass = (recoQQsign[j] == 0) ? false : true;}
      if(dimusignPass == false) continue; // dimuon charge selection
      if(!IsAcceptanceQQ(pt1[j],eta1[j]) || !IsAcceptanceQQ(pt2[j],eta2[j]) ) continue;

      if(mass[j]<massLow || mass[j]>massHigh) continue; // dimuon mass range
      if(ctau[j] >= ctauCut) continue;// Give Ctau Cut!!!

      if(pt[j]>ptLow&&pt[j]<ptHigh&&abs(y[j])<yHigh&&abs(y[j])>yLow&&pt1[j]>SiMuPtCut&&pt2[j]>SiMuPtCut&&abs(eta1[j])<2.4&&abs(eta2[j])<2.4){
        for(int imbin=0; imbin<nMassBin; imbin++){
          if(mass[j]>=massBin[imbin] && mass[j]<massBin[imbin+1]){
              v2_1[imbin][count[imbin]] = (qxa[j]*qxdimu[j] + qya[j]*qydimu[j]);
              v2_2[imbin][count[imbin]] = (qxa[j]*qxb[j] + qya[j]*qyb[j]);
              v2_3[imbin][count[imbin]] = (qxa[j]*qxc[j] + qya[j]*qyc[j]);
              v2_4[imbin][count[imbin]] = (qxb[j]*qxc[j] + qyb[j]*qyc[j]);
              v2_1_raw[imbin][count[imbin]] = (qxa[j]*qxdimu[j] + qya[j]*qydimu[j]);
              v2_2_raw[imbin][count[imbin]] = (qxa[j]*qxb[j] + qya[j]*qyb[j]);
              v2_3_raw[imbin][count[imbin]] = (qxa[j]*qxc[j] + qya[j]*qyc[j]);
              v2_4_raw[imbin][count[imbin]] = (qxb[j]*qxc[j] + qyb[j]*qyc[j]);

              v2_1_avg[imbin] += v2_1[imbin][count[imbin]];
              v2_2_avg[imbin] += v2_2[imbin][count[imbin]];
              v2_3_avg[imbin] += v2_3[imbin][count[imbin]];
              v2_4_avg[imbin] += v2_4[imbin][count[imbin]];

              count[imbin]++;
          }
        }
        massVar->setVal((double)mass[j]);
        dataSet->add(*argSet);
      h_mass->Fill(mass[j]);
      }
    }
  }

  int nMassFrameBin = 42;
  RooWorkspace *ws = new RooWorkspace("workspace");
  ws->import(*dataSet);
  ws->data("dataset")->Print();
  RooDataSet *reducedDS = new RooDataSet("reducedDS","A sample",*dataSet->get(),Import(*dataSet));
  reducedDS->SetName("reducedDS");
  ws->import(*reducedDS);
  ws->var("mass")->setRange(massLow, massHigh);
  ws->var("mass")->Print();

  RooPlot* myPlot = ws->var("mass")->frame(nMassFrameBin);
  ws->data("reducedDS")->plotOn(myPlot,Name("massDataHist"));

  RooHist* hist = (RooHist*) myPlot->findObject("massDataHist");
  int nHistP = hist->GetN();
  cout << "nHistP : " << nHistP << endl;
  if(nHistP != nMassFrameBin) cout << "ERROR::: INCONSISTENT NUMBER OF BINS" << endl;
  
  TGraphAsymmErrors *g_mass = new TGraphAsymmErrors();
  g_mass->SetName("g_mass");
  Double_t xp, yp, ypl, yph;
  for(int ip = 0; ip < nHistP; ip++){
    xp=0; yp=0; ypl=0; yph=0;
    hist->GetPoint(ip,xp,yp);
    ypl = hist->GetErrorYlow(ip);
    yph = hist->GetErrorYhigh(ip);
    g_mass->SetPoint(ip,xp,yp);
    g_mass->SetPointError(ip,0,0,ypl,yph);
  }
    
  cout << "more than one dimuon : " << nDimu_more << endl;
  cout << "one dimuon : " << nDimu_one << endl;

  vector<double> v2_1_err(nMassBin,0);
  vector<double> v2_2_err(nMassBin,0);
  vector<double> v2_3_err(nMassBin,0);
  vector<double> v2_4_err(nMassBin,0);

  for(int ibin=0; ibin<nMassBin; ibin++){
    v2_1_avg[ibin] = v2_1_avg[ibin]/count[ibin];
    v2_2_avg[ibin] = v2_2_avg[ibin]/count[ibin];
    v2_3_avg[ibin] = v2_3_avg[ibin]/count[ibin];
    v2_4_avg[ibin] = v2_4_avg[ibin]/count[ibin];

    for(int icount=0; icount<count[ibin]; icount++){
      v2_1_err[ibin] += (v2_1_raw[ibin][icount]-v2_1_avg[ibin])*(v2_1_raw[ibin][icount]-v2_1_avg[ibin]) ;
      v2_2_err[ibin] += (v2_2_raw[ibin][icount]-v2_2_avg[ibin])*(v2_2_raw[ibin][icount]-v2_2_avg[ibin]) ;
      v2_3_err[ibin] += (v2_3_raw[ibin][icount]-v2_3_avg[ibin])*(v2_3_raw[ibin][icount]-v2_3_avg[ibin]) ;
      v2_4_err[ibin] += (v2_4_raw[ibin][icount]-v2_4_avg[ibin])*(v2_4_raw[ibin][icount]-v2_4_avg[ibin]) ;
    }

    v2_1_err[ibin] = TMath::Sqrt(v2_1_err[ibin])/count[ibin];
    v2_2_err[ibin] = TMath::Sqrt(v2_2_err[ibin])/count[ibin];
    v2_3_err[ibin] = TMath::Sqrt(v2_3_err[ibin])/count[ibin];
    v2_4_err[ibin] = TMath::Sqrt(v2_4_err[ibin])/count[ibin];

    h_v2_num_q1->SetBinContent(ibin+1,v2_1_avg[ibin]);
    h_v2_num_q1->SetBinError(ibin+1,v2_1_err[ibin]);
    h_v2_den_q2->SetBinContent(ibin+1,v2_2_avg[ibin]);
    h_v2_den_q2->SetBinError(ibin+1,v2_2_err[ibin]);
    h_v2_den_q3->SetBinContent(ibin+1,v2_3_avg[ibin]);
    h_v2_den_q3->SetBinError(ibin+1,v2_3_err[ibin]);
    h_v2_den_q4->SetBinContent(ibin+1,v2_4_avg[ibin]);
    h_v2_den_q4->SetBinError(ibin+1,v2_4_err[ibin]);
    
    cout << ibin << "th Bin : " << count[ibin] << ",  sum  : " << count[ibin] << endl;
    cout << "v2_1_avg " << ibin << " : " << v2_1_avg[ibin] << endl;
    cout << "v2_2_avg " << ibin << " : " << v2_2_avg[ibin] << endl;
    cout << "v2_3_avg " << ibin << " : " << v2_3_avg[ibin] << endl;
    cout << "v2_4_avg " << ibin << " : " << v2_4_avg[ibin] << endl;
    
    cout << "h_v2_num_q1 " << ibin << ", val : " << h_v2_num_q1->GetBinContent(ibin+1) << " err : " << h_v2_num_q1->GetBinError(ibin+1) << endl;
    cout << "h_v2_den_q2 " << ibin << ", val : " << h_v2_den_q2->GetBinContent(ibin+1) << " err : " << h_v2_den_q2->GetBinError(ibin+1) << endl;
    cout << "h_v2_den_q3 " << ibin << ", val : " << h_v2_den_q3->GetBinContent(ibin+1) << " err : " << h_v2_den_q3->GetBinError(ibin+1) << endl;
    cout << "h_v2_den_q4 " << ibin << ", val : " << h_v2_den_q4->GetBinContent(ibin+1) << " err : " << h_v2_den_q4->GetBinError(ibin+1) << endl;
  }

  TH1D* h_v2_den_ = (TH1D*)h_v2_den_q2->Clone("h_v2_den_");
  h_v2_den_->Multiply(h_v2_den_q3);
  h_v2_den_->Divide(h_v2_den_q4);

  TH1D* h_v2_den = (TH1D*) h_v2_den_->Clone("h_v2_den"); h_v2_den->Reset();
  GetHistSqrt(h_v2_den_,h_v2_den);

  TH1D* h_v2_final = (TH1D*) h_v2_num_q1 -> Clone("h_v2_SplusB");
  h_v2_final->Divide(h_v2_den);

  h_v2_final->GetYaxis()->SetRangeUser(-0.1,0.2);
  h_v2_final->GetYaxis()->SetTitle("v_{2}^{S+B}");
  h_v2_final->GetXaxis()->SetTitle("m_{#mu^{+}#mu^{-}} (GeV)");
  h_v2_final->GetYaxis()->SetLabelSize(0.055);
  h_v2_final->GetXaxis()->SetLabelSize(0.055);
  h_v2_final->GetXaxis()->SetTitleSize(0.07);
  h_v2_final->GetYaxis()->SetTitleSize(0.07);
  h_v2_final->GetYaxis()->SetTitleOffset(1.2);
  h_v2_final->GetXaxis()->SetTitleOffset(1.0);
  h_v2_final->GetXaxis()->SetLabelOffset(0.011);
  SetHistStyle(h_v2_final,0,0);
  SetGraphStyle2(g_mass,0,0);
  g_mass->GetYaxis()->SetLimits(0,14000);
  g_mass->GetXaxis()->SetLimits(massLow,massHigh);
  g_mass->GetXaxis()->SetRangeUser(massLow,massHigh);
  g_mass->GetYaxis()->SetLabelSize(0.055);
  g_mass->GetXaxis()->SetLabelSize(0.04);
  g_mass->GetYaxis()->SetTitleSize(0.055);
  g_mass->GetYaxis()->SetTitleOffset(1.7);
  g_mass->GetXaxis()->SetTitleSize(0.065);
  
  float pos_x = 0.23;
  float pos_x_mass = 0.23;
  float pos_y = 0.76;
  float pos_y_diff = 0.071;
  int text_color = 1;
  float text_size = 16;
  TString perc = "%";


  TCanvas* c_mass_v2 = new TCanvas("c_mass_v2","",590,750);
  TPad* pad1 = new TPad("pad1","pad1",0,0.5,1.0,1.0);
  TPad* pad2 = new TPad("pad2","pad2",0,0.0,1.0,0.5);
  c_mass_v2->cd();
  pad1->SetTicks(1,1);
  pad1->SetBottomMargin(0);
  pad1->SetLeftMargin(0.19);
  pad1->SetTopMargin(0.08);
  pad1->Draw();
  pad1->cd();
  g_mass->GetXaxis()->SetTitleSize(0);
  g_mass->GetXaxis()->SetLabelSize(0);
  g_mass->Draw("AP");
  if(ptLow==0) drawText(Form("p_{T}^{#mu#mu} < %.1f GeV/c",ptHigh ),pos_x_mass,pos_y,text_color,text_size);
  else if(ptLow!=0) drawText(Form("%.1f < p_{T}^{#mu#mu} < %.1f GeV/c",ptLow, ptHigh ),pos_x_mass,pos_y,text_color,text_size);
  if(yLow==0) drawText(Form("|y^{#mu#mu}| < %.1f",yHigh ), pos_x_mass,pos_y-pos_y_diff,text_color,text_size);
  else if(yLow!=0) drawText(Form("%.1f < |y^{#mu#mu}| < %.1f",yLow, yHigh ), pos_x_mass,pos_y-pos_y_diff,text_color,text_size);
  drawText(Form("p_{T}^{#mu} > %.1f GeV/c", SiMuPtCut ), pos_x_mass,pos_y-pos_y_diff*2,text_color,text_size);
  drawText("|#eta^{#mu}| < 2.4", pos_x_mass,pos_y-pos_y_diff*3,text_color,text_size);
  drawText(Form("Centrality %d-%d%s",cLow/2,cHigh/2,perc.Data()),pos_x_mass,pos_y-pos_y_diff*4,text_color,text_size);
  pad2->SetTopMargin(0);
  pad2->SetBottomMargin(0.17);
  pad2->SetLeftMargin(0.19);
  pad2->SetTicks();
  pad2->cd();
  jumSun(massLow,0,massHigh,0,1,1);
  h_v2_final->Draw("P");

  CMS_lumi_v2mass(pad1,iPeriod,iPos,1);  
  pad1->Update();
  pad2->Update();
  c_mass_v2->cd();
  pad1->Draw();
  pad2->Draw();

  c_mass_v2->SaveAs(Form("v2Mass_%s.pdf",kineLabel.Data()));


  TCanvas* c_mass = new TCanvas("c_mass","",600,600);
  c_mass->cd();
  h_mass->Draw("P");
  if(ptLow==0) drawText(Form("p_{T}^{#mu#mu} < %.1f GeV/c",ptHigh ),pos_x_mass,pos_y,text_color,text_size);
  else if(ptLow!=0) drawText(Form("%.1f < p_{T}^{#mu#mu} < %.1f GeV/c",ptLow, ptHigh ),pos_x_mass,pos_y,text_color,text_size);
  if(yLow==0) drawText(Form("|y^{#mu#mu}| < %.1f",yHigh ), pos_x_mass,pos_y-pos_y_diff,text_color,text_size);
  else if(yLow!=0) drawText(Form("%.1f < |y^{#mu#mu}| < %.1f",yLow, yHigh ), pos_x_mass,pos_y-pos_y_diff,text_color,text_size);
  drawText(Form("p_{T}^{#mu} > %.1f GeV/c", SiMuPtCut ), pos_x_mass,pos_y-pos_y_diff*2,text_color,text_size);
  drawText("|#eta^{#mu}| < 2.4", pos_x_mass,pos_y-pos_y_diff*3,text_color,text_size);
  drawText(Form("Centrality %d-%d%s",cLow/2,cHigh/2,perc.Data()),pos_x_mass,pos_y-pos_y_diff*4,text_color,text_size);
  CMS_lumi_v2mass(c_mass,iPeriod,iPos,0);
  c_mass->Update();
  c_mass->SaveAs(Form("MassDist_%s.pdf",kineLabel.Data()));


  TLegend *leg_v2_1 = new TLegend(0.38,0.64,0.77,0.9);
  TLegend *leg_v2_2 = new TLegend(0.38,0.64,0.77,0.9);
  TLegend *leg_v2_3 = new TLegend(0.38,0.64,0.77,0.9);
  TLegend *leg_v2_4 = new TLegend(0.38,0.64,0.77,0.9);
  SetLegendStyle(leg_v2_1);
  SetLegendStyle(leg_v2_2);
  SetLegendStyle(leg_v2_3);
  SetLegendStyle(leg_v2_4);

  double mean_v2_1[nMass_div];
  double mean_v2_2[nMass_div];
  double mean_v2_3[nMass_div];
  double mean_v2_4[nMass_div];

  double mean_err_v2_1[nMass_div];
  double mean_err_v2_2[nMass_div];
  double mean_err_v2_3[nMass_div];
  double mean_err_v2_4[nMass_div];

  for(int i=0;i<nMass_div;i++){
    SetHistStyle(h_v2_1[i],i,i);
    SetHistStyle(h_v2_2[i],i,i);
    SetHistStyle(h_v2_3[i],i,i);
    SetHistStyle(h_v2_4[i],i,i);
    scaleInt(h_v2_1[i]);
    scaleInt(h_v2_2[i]);
    scaleInt(h_v2_3[i]);
    scaleInt(h_v2_4[i]);

    mean_v2_1[i] = h_v2_1[i]->GetMean();
    mean_v2_2[i] = h_v2_2[i]->GetMean();
    mean_v2_3[i] = h_v2_3[i]->GetMean();
    mean_v2_4[i] = h_v2_4[i]->GetMean();

    mean_err_v2_1[i] = h_v2_1[i]->GetMeanError();
    mean_err_v2_2[i] = h_v2_2[i]->GetMeanError();
    mean_err_v2_3[i] = h_v2_3[i]->GetMeanError();
    mean_err_v2_4[i] = h_v2_4[i]->GetMeanError();

    leg_v2_1->AddEntry(h_v2_1[i],(fSB[i]+Form(" mean:%.2f",mean_v2_1[i])+Form(" err:%.2f",mean_err_v2_1[i])).Data(),"l");
    leg_v2_2->AddEntry(h_v2_2[i],(fSB[i]+Form(" mean:%.2f",mean_v2_2[i])+Form(" err:%.2f",mean_err_v2_2[i])).Data(),"l");
    leg_v2_3->AddEntry(h_v2_3[i],(fSB[i]+Form(" mean:%.2f",mean_v2_3[i])+Form(" err:%.2f",mean_err_v2_3[i])).Data(),"l");
    leg_v2_4->AddEntry(h_v2_4[i],(fSB[i]+Form(" mean:%.2f",mean_v2_4[i])+Form(" err:%.2f",mean_err_v2_4[i])).Data(),"l");
  }
/*
  TCanvas *c_qq_1 = new TCanvas("c_qqa","",600,600);
  c_qq_1->cd();
  h_v2_1[0]->Draw("hist");
  h_v2_1[1]->Draw("hist same");
  h_v2_1[2]->Draw("hist same");
  leg_v2_1->Draw("same");
  c_qq_1->SaveAs(Form("c_qqa_%s.pdf",kineLabel.Data()));

  TCanvas *c_qq_2 = new TCanvas("c_qaqb","",600,600);
  c_qq_2->cd();
  h_v2_2[0]->Draw("hist");
  h_v2_2[1]->Draw("hist same");
  h_v2_2[2]->Draw("hist same");
  leg_v2_2->Draw("same");
  c_qq_2->SaveAs(Form("c_qaqb_%s.pdf",kineLabel.Data()));

  TCanvas *c_qq_3 = new TCanvas("c_qaqc","",600,600);
  c_qq_3->cd();
  h_v2_3[0]->Draw("hist");
  h_v2_3[1]->Draw("hist same");
  h_v2_3[2]->Draw("hist same");
  leg_v2_3->Draw("same");
  c_qq_3->SaveAs(Form("c_qaqc_%s.pdf",kineLabel.Data()));

  TCanvas *c_qq_4 = new TCanvas("c_qbqc","",600,600);
  c_qq_4->cd();
  h_v2_4[0]->Draw("hist");
  h_v2_4[1]->Draw("hist same");
  h_v2_4[2]->Draw("hist same");
  leg_v2_4->Draw("same");
  c_qq_4->SaveAs(Form("c_qbqc_%s.pdf",kineLabel.Data()));
  */
  TFile *wf = new TFile(Form("Jpsi_%s.root",kineLabel.Data()),"recreate");
  wf->cd();
  h_v2_final->Write();
  g_mass->Write();

}
    

void GetHistSqrt(TH1D* h1, TH1D* h2){
  if(h1->GetNbinsX() != h2->GetNbinsX()){ cout << "Inconsistent # of bins b/w histograms !! " << endl;}
  double content;
  double err;
  for(int i=1; i<=h1->GetNbinsX(); i++){
    content=0;err=0;
    content = h1->GetBinContent(i);
    err = h1->GetBinError(i);
    err = 0.5*err*TMath::Power(content,-0.5);
    h2->SetBinContent(i,TMath::Sqrt(content));
    h2->SetBinError(i,err);
  }
} 
