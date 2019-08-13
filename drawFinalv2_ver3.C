#include <TROOT.h>
#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <TF1.h>
#include <TH1.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TMultiGraph.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TString.h>
#include <iostream>
#include <fstream>
#include <TMath.h>
#include <RooFit.h>
#include <RooGlobalFunc.h>
#include <RooCategory.h>
#include <RooGenericPdf.h>
#include <RooFFTConvPdf.h>
#include <RooWorkspace.h>
#include <RooBinning.h>
#include <RooHistPdf.h>
#include <RooProdPdf.h>
#include <RooAddPdf.h>
#include <RooRealVar.h>
#include <RooDataSet.h>
#include <RooHist.h>
#include <RooFitResult.h>
#include <RooPlot.h>
#include <RooConstVar.h>

#include <Fit/Fitter.h>
#include <Fit/BinData.h>
#include <Fit/Chi2FCN.h>
#include <Math/WrappedMultiTF1.h>
#include <HFitInterface.h>

#include "/home/deathold/work/CMS/analysis/Upsilon_v2/UpsilonPbPb2018_v2/commonUtility.h"
#include "../week5/cutsAndBinUpsilonV2.h"
#include "/home/deathold/work/CMS/analysis/Upsilon_v2/UpsilonPbPb2018_v2/HiEvtPlaneList.h"
#include "/home/deathold/work/CMS/analysis/Upsilon_v2/UpsilonPbPb2018_v2/Style_jaebeom.h"
#include "/home/deathold/work/CMS/analysis/Upsilon_v2/UpsilonPbPb2018_v2/tdrstyle.C"
#include "/home/deathold/work/CMS/analysis/Upsilon_v2/UpsilonPbPb2018_v2/CMS_lumi_v2mass.C"
#include "../week5/setvariables.h"

using namespace std;

bool ctaucut(double c) {

if (c < 9.99) {return true;}
else {return false;}
}


void drawFinalv2_ver3(){

    TCanvas *c1 = new TCanvas("canvas1","Final v2",0,50,550,520);

    c1 -> SetLeftMargin(0.15);

    TFile *f[npt];
    TGraphErrors *gr[npt];



    for (int i = 0; i < npt1; i++) {

        f[i] = new TFile(
                Form("../week5/FitResult/SimFitResult_pt%.1f-%.1f_y%.1f-%.1f_ctau%.3f_muPt0.0_centrality%d-%d_m2.6-3.5_OS.root", ptBin[i], ptBin[i + 1], yLow2, yHigh, ctauBin[i], cLow, cHigh), "read");
        gr[i] = (TGraphErrors *) f[i]->Get("v2vspt");
        cout << "ptLow, ptHigh, yLow, yHigh, ctau : " << ptBin[i] << " " << ptBin[i+1] << " " << yLow2 << " " << yHigh << " " << ctauBin[i]  << endl;

    }


    for (int i = npt1; i < npt-1; i++) {

        f[i] = new TFile(
                Form("../week5/FitResult/SimFitResult_pt%.1f-%.1f_y%.1f-%.1f_ctau%.3f_muPt0.0_centrality%d-%d_m2.6-3.5_OS.root", ptBin[i], ptBin[i + 1], yLow1, yHigh, ctauBin[i],cLow, cHigh),"read");
        gr[i] = (TGraphErrors *) f[i]->Get("v2vspt");
    
        cout << "ptLow, ptHigh, yLow, yHigh, ctau : " << ptBin[i] << " " << ptBin[i+1] << " " << yLow1 << " " << yHigh << " " << ctauBin[i]  << endl;

    }

// pt 15 -50 (centrality 40-80) needs a special treatment/////////////////////////////
//
//
  double mass = 2.6;
  if(cLow == 40) {mass = 2.8;}

    f[npt-1] = new TFile(
                Form("../week5/FitResult/SimFitResult_pt15.0-50.0_y%.1f-%.1f_ctau%.3f_muPt0.0_centrality%d-%d_m%.1f-3.5_OS.root", yLow1, yHigh, ctauBin[npt-1],cLow, cHigh, mass),"read");


    
  
    gr[npt-1] = (TGraphErrors*) f[npt-1] -> Get("v2vspt");


  
////////////////////////////////////////////////////////////////////



    Double_t y[npt], ex[npt], ey[npt];

    Double_t xp, yp;


    for(int i = 0; i<npt; i++) {
        
        ex[i] = (ptBin[i+1]-ptBin[i])/2.;

        gr[i] -> GetPoint(0, xp, y[i]);
        gr[i] -> SetPoint(0, x[i], y[i]);
        ey[i] = gr[i] -> GetErrorY(0);
        gr[i] -> SetPointError(0,ex[i], ey[i]);
        cout << "v2 : " << y[i] << endl;
  }



    TLegend* leg = new TLegend(0.65,0.72,0.88,0.55); //fitleg->SetTextSize(15);
    SetLegendStyle(leg);
    leg->AddEntry(gr[0],Form("%.1f < |y| < 2.4", yLow2),"pe");

    leg->AddEntry(gr[npt-1],"|y| < 2.4","pe");





    c1 -> cd();
    gr[0] -> GetXaxis() -> SetLimits(0,50);
    gr[0] -> GetYaxis() -> SetLimits(0,0.2);
    gr[0] -> GetYaxis() -> SetRangeUser(0,0.2);
   // gr[0] -> GetYaxis() -> SetLeftMargin(0.5);
    gr[0] -> GetXaxis() -> SetTitle("p_{T} (GeV/c)");
    gr[0] -> GetXaxis() -> CenterTitle();
    gr[0] -> GetYaxis() -> SetTitle("v_{2}^{J/#psi}");
    gr[0] -> GetYaxis() -> CenterTitle();


    gr[0] -> Draw("AP");



    for (int i = 0; i < npt1; i++) {

        gr[i] -> SetMarkerColor(kBlue);
        gr[i] -> SetLineColor(kBlue);
        gr[i+1] -> Draw("P");

    }

    for (int i = npt1; i < npt; i++) {

        gr[i] -> SetMarkerColor(kRed);
        gr[i] -> SetLineColor(kRed);
        gr[i] -> Draw("P");
    }


    float pos_x = 0.3;
    float pos_y = 0.75;
    float pos_y_diff = 0.05;
    int text_color = 1;
    float text_size = 16;
    TString perc = "%";

    leg->Draw("same");

    if (ctaucut(ctauBin[0]) == 0) {


    drawText("Inclusive J/#psi",pos_x,pos_y,text_color,text_size);
    
    }

    else { 
    drawText("Prompt J/#psi",pos_x,pos_y,text_color,text_size);
    }
    
    drawText(Form("Centrality %d-%d%s",cLow/2,cHigh/2,perc.Data()),pos_x,pos_y-pos_y_diff,text_color,text_size);

    c1-> cd();
    c1 -> SaveAs(Form("v2final_centrality%d-%d_ver3_ctaucut%d.pdf", cLow, cHigh, ctaucut(ctauBin[0])));

}
