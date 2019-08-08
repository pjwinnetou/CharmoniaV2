#include <iostream>
#include "TFile.h"
#include "TROOT.h"
#include "TTree.h"
#include "TH1.h"
#include "TStyle.h"
#include "Style_jaebeom.h" //

void histogram_Pmpt_nonPmpt_ctau(){

  gStyle->SetOptStat(0); // 그림 그렸을때 statistical box를 없애줌 (0)
  //TFile *ptfile = new TFile("OniaSkimSimple_isMC1.root","read");
  //TFile *ptfile = TFile::Open("OniaSkimSimple_isMC1.root");
  TFile *filePmpt = new TFile("OniaSkimSimple_isMC1.root","read"); // Monte Carlo로 돌린 애들 불러옴
  TFile *fileNon = new TFile("OniaSkimSimple_nonPmpt_isMC1.root","read"); //루트파일(실제로 필터된 데이터) 안에 있는 애들 불러옴
  //ptfile -> GetName(); // 없어도 되는거

  TTree *TreePmpt = (TTree*) filePmpt -> Get("dimutree"); // 트리를 복붙!
  TTree *TreeNon = (TTree*) fileNon -> Get("dimutree");

  //Declaration (for data root file)
  Int_t nDimu0;
  Int_t nDimu1;
  Float_t ctauPmpt[100]; // array니까 크기 지정해줌
  Float_t ctauNon[100];




  // 내data 트리에 가져온 브랜치 몇개 빼서 넣음
 
  TreePmpt -> SetBranchAddress("nDimu", &nDimu0); // 하나라서 &있음
  TreeNon -> SetBranchAddress("nDimu", &nDimu1); // 하나라서 &있음
  TreePmpt -> SetBranchAddress("ctau",ctauPmpt); //array 라서 & 사인 없음
  TreeNon -> SetBranchAddress("ctau",ctauNon);



  TH1D *hctauPmpt = new TH1D("hctauPmpt","C#tau(Prompt);C#tau ;Counts",250,-2,3); // 히스토그램 만들어주고
  TH1D *hctauNon = new TH1D("hctauNon","C#tau(NonPrompt);C#tau ;Counts",250,-2,3);



  TCanvas *cvs = new TCanvas("cvs1", "C#tau of Prompt and Non-Prompt J/#psi ", 800, 650); // 캔버스도 만들고


/*
  Int_t nevt0 = TreePmpt -> GetEntries(); // 이벤트 갯수 가져오고
  Int_t nevt1 = TreeNon -> GetEntries();

  
     for (Int_t i=0; i<nevt0; i++){

     TreePmpt -> GetEntry(i);

     for (Int_t j=0; j<nDimu; j++){
     hctauPmpt -> Fill(ctauPmpt[j]);
     
     }  
   
     for (Int_t i=0; i<nevt1; i++){

     TreePmpt -> GetEntry(i);

     for (Int_t j=0; j<nDimu; j++){
     hctauNon -> Fill(ctauNon[j]);
     }
    }
  */
  
   // 트리에 있는것들을 히스토그램애 넣음
    


  TreePmpt->Draw("ctau>>hctauPmpt","",""); // 조건 없으니까 가운데 비워둠
  TreeNon->Draw("ctau>>hctauNon","","");


 // hctauPmpt->GetYaxis()->SetRangeUser(0,50000);
  hctauPmpt->GetYaxis()->CenterTitle();
  hctauPmpt->GetXaxis()->CenterTitle();
  
  hctauPmpt -> SetLineColor(kRed);
  hctauNon -> SetLineColor(kBlue);
 
  hctauPmpt -> SetMarkerColor(kRed);
  hctauNon -> SetMarkerColor(kBlue);

  hctauPmpt -> SetMarkerStyle(kFullCircle);
  hctauNon -> SetMarkerStyle(kFullCircle);

  gPad->SetLeftMargin(0.15); // gPad는 gStyle 같은 루트 안의 클래스

  Double_t norm0 = hctauPmpt -> Integral();
  Double_t norm1 = hctauNon -> Integral();
 
  hctauPmpt -> Scale(1/norm0);
  hctauNon -> Scale(1/norm1);
  hctauPmpt -> Draw();              //히스토그램 그리기!
  hctauNon -> Draw("same"); // pe : point and error bar




  
  //Set Legend
  double posx_start = 0.6;
  double posx_end = 0.95;
  double posy_start = 0.55;
  double posy_end = 0.7;

  TLegend *leg = new TLegend(posx_start, posy_start, posx_end, posy_end);
  SetLegendStyle(leg); // style.jabeom .h 안에 있는거
  leg->AddEntry(hctauPmpt,"C#tau (Prompt)","pe");
  leg->AddEntry(hctauNon,"C#tau (Non-Prompt)","pe");

  leg->Draw("same");
  //cvs->SaveAs("ResultPlot/Data_MuonPt_MCdata.pdf"); 
  
  




 /*                                                   

  TLegend *legm = new TLegend(posx_start, posy_start, posx_end, posy_end);
  SetLegendStyle(legm); // style.jabeom .h 안에 있는거
                                                    
  legm->Draw("same");
 */
  cvs->cd();

  cvs->Update();
  cvs->Draw();
  cvs->SaveAs("week3/ctau_MC_histogram_pmpt_non-pmpt.pdf");
 }
