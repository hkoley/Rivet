// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/UnstableFinalState.hh"

namespace Rivet {


  /// @brief Add a short analysis description here
  class ALICE_2017_I1645239 : public Analysis {
  public:

    /// Constructor
    DEFAULT_RIVET_ANALYSIS_CTOR(ALICE_2017_I1645239);


    /// @name Analysis methods
    //@{

    /// Book histograms and initialise projections before the run
    void init() {

      // Initialise and register projections
      declare(UnstableFinalState(Cuts::absrap < 0.96), "UFS");
      
      std::vector<double> binEdges = {1., 2., 3., 4., 6., 8.};
      std::vector<double> binEdges1 = {2., 4., 6., 8., 12.};
      std::vector<double> binEdges2 = {2., 4., 6., 8.};

      // Book histograms
      _h_Lc = bookHisto1D(1, 1, 1);
      _h_LcPb = bookHisto1D(2, 1, 1);
      _h_LcD0= bookScatter2D(3, 1, 1);
      _h_LcD0Pb = bookScatter2D(4, 1, 1);
      _h_LcD0int = bookScatter2D(5, 1, 1);
      _h_LcD0Pbint = bookScatter2D(6, 1, 1);
      _h_RpPb = bookScatter2D(7,1,1);
      _h_Lcdummy = bookHisto1D("Lcdummy",binEdges, "Lcdummy");
      _h_D0 = bookHisto1D("D0", binEdges, "D0");
      _h_LcPbdummy0 = bookHisto1D("LcPbdummy0", binEdges, "LcPbdummy0");
      _h_LcPbdummy = bookHisto1D("LcPbdummy", binEdges1, "LcPbdummy");
      _h_D0Pb = bookHisto1D("D0Pb", binEdges1, "D0Pb");
      _h_D0Pb0 = bookHisto1D("D0Pb0", binEdges, "D0Pb0");
      brutalD0 = bookScatter2D(8,1,1);
      brutalLc = bookScatter2D(9,1,1);
      d0pbnum = bookHisto1D("d0pbnum", binEdges1, "d0pbnum");
      lcpbnum = bookHisto1D("lcpbnum", binEdges1, "lcpbnum");
      d0pbnumw = bookHisto1D("d0pbnumw", binEdges1, "d0pbnumw");
      lcpbnumw = bookHisto1D("lcpbnumw", binEdges1, "lcpbnumw");
      _h_Lcint = bookHisto1D("Lcint", 1, -0.1, 0.1 , "Lc int");
      _h_D0int = bookHisto1D("D0int", 1, -0.1, 0.1 , "D0 int");
      _h_LcintPb = bookHisto1D("LcintPb", 1, -0.6, -0.4 , "Lc int Pb");
      _h_D0intPb = bookHisto1D("D0intPb", 1, -0.6, -0.4 , "D0 int Pb");
      _h_LcR = bookHisto1D("LcR", binEdges2, "Lc R");
      _h_LcRPb = bookHisto1D("LcRPb", binEdges2, "Lc RPb");

      bo1 = bo2 = bo3 = false;
    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {
      PdgIdPair beamp; 
      beamp = beamIds();
      const double weight = event.weight();
      const UnstableFinalState& ufs = apply<UnstableFinalState>(event, "UFS");
        
        /*PDG code IDs used inside the foreach cycle: 421 = D0, 411 = D+, 413 = D*+ */
      if(beamp.first == 2212 && beamp.second ==2212){
        if(fuzzyEquals(sqrtS()/GeV,5000)){
         bo2 = true;
         foreach (const Particle& p, ufs.particles()) {
            if(p.fromBottom())
                continue;
            else
                {    
                 if(p.rap() < 0.04 && p.rap() > -0.96){
                     if(p.abspid() == 4122){
                         _h_LcR->fill(p.pT()/GeV, weight);
                         }   
                }    
        } 
        }}
       else{
         bo1 = true;
         foreach (const Particle& p, ufs.particles()) {
            if(p.fromBottom())
                continue;
            else
                {    
                 if(p.absrap() < 0.5){
                     if(p.abspid() == 421){
                         _h_D0->fill(p.pT()/GeV, weight); 
                         _h_D0int->fill(0,weight);}
                     else if(p.abspid() == 4122){
                         _h_Lc->fill(p.pT()/GeV, weight);
			 _h_Lcdummy->fill(p.pT()/GeV, weight);
                         _h_Lcint->fill(0,weight);}
                     }   
                }    
        }
      }}
      else if((beamp.first == 2212 && beamp.second == 1000822080) || (beamp.second ==2212 && beamp.first == 1000822080)){
        bo3 = true;
        foreach (const Particle& p, ufs.particles()) {
            if(p.fromBottom())
                continue;
            else
                {    
                 if(p.rap() < 0.04 && p.rap() > -0.96){
                     if(p.abspid() == 421){
                         _h_D0Pb->fill(p.pT()/GeV, weight);
			 d0tot += 1; 
                         _h_D0Pb0->fill(p.pT()/GeV, weight);
                         _h_D0intPb->fill(-0.5,weight);
                         d0pbnum->fill(p.pT()/GeV,1);
                         d0pbnumw->fill(p.pT()/GeV,weight);
                     }
                     else if(p.abspid() == 4122){
                         _h_LcPb->fill(p.pT()/GeV, 1);
			 lctot += 1;
			 _h_LcPbdummy0->fill(p.pT()/GeV, weight);
                         _h_LcPbdummy->fill(p.pT()/GeV, weight);
                         _h_LcRPb->fill(p.pT()/GeV, weight);
                         _h_LcintPb->fill(-0.5,weight);
			 cout << "\nWeight is " << weight <<"\n";
                         lcpbnum->fill(p.pT()/GeV,1);
                         lcpbnumw->fill(p.pT()/GeV,weight);
                     }
                     }    
                }    
        }
      }  

    }


    /// Normalise histograms etc., after the run
    void finalize() {

      if(bo1 == true) scale(_h_D0, crossSection()/(microbarn*2*sumOfWeights())); // norm to cross section
      if(bo1 == true) scale(_h_D0int, crossSection()/(microbarn*2*sumOfWeights())); // norm to cross section
      if(bo1 == true) scale(_h_Lc, crossSection()/(microbarn*2*sumOfWeights())); // norm to cross section
      if(bo1 == true) scale(_h_Lcdummy, crossSection()/(microbarn*2*sumOfWeights())); // norm to cross section
      if(bo3 == true) scale(_h_LcPbdummy, crossSection()/(microbarn*2*sumOfWeights())); // norm to cross section
      if(bo3 == true) scale(_h_LcPbdummy0, crossSection()/(microbarn*2*sumOfWeights())); // norm to cross section
      if(bo1 == true) scale(_h_Lcint, crossSection()/(microbarn*2*sumOfWeights())); //norm to cross section
      if(bo3 == true) scale(_h_D0Pb, crossSection()/(microbarn*2*sumOfWeights())); // norm to cross section
      if(bo3 == true) scale(_h_D0Pb0, crossSection()/(microbarn*2*sumOfWeights())); // norm to cross section
      if(bo3 == true) scale(_h_D0intPb, crossSection()/(microbarn*2*sumOfWeights())); // norm to cross section
      if(bo3 == true) scale(_h_LcPb, crossSection()/(microbarn*2*sumOfWeights())); // norm to cross section
      cout <<"\n lctot is = " << lctot << '\n';
      cout <<"\n d0tot is = " << d0tot << '\n';
      cout <<"\n sumOfWeights is = " << sumOfWeights() << '\n';
      cout <<"\n crossSection() is = " << crossSection() << '\n';
      if(bo3 == true) scale(_h_LcintPb, crossSection()/(microbarn*2*sumOfWeights())); //norm to cross section
      if (_h_Lcdummy->numEntries()>0 && _h_D0->numEntries()>0) divide(_h_Lcdummy, _h_D0, _h_LcD0);
      if (_h_LcPbdummy->numEntries()>0 && _h_D0Pb->numEntries()>0) divide(_h_LcPbdummy, _h_D0Pb, _h_LcD0Pb);
      if (_h_LcPbdummy0->numEntries()>0 && _h_Lcdummy->numEntries()>0) divide(_h_LcPbdummy0, _h_Lcdummy, brutalLc);
      if (_h_D0Pb0->numEntries()>0 && _h_D0->numEntries()>0) divide(_h_D0Pb0, _h_D0, brutalD0);
      if (_h_Lcint->numEntries()>0 && _h_D0int->numEntries()>0) divide(_h_Lcint, _h_D0int, _h_LcD0int);
      if (_h_LcintPb->numEntries()>0 && _h_D0intPb->numEntries()>0) divide(_h_LcintPb, _h_D0intPb, _h_LcD0Pbint);
      if(bo2 == true) scale(_h_LcR, 208*crossSection()/(microbarn*2*sumOfWeights())); // norm to cross section
      if(bo3 == true) scale(_h_LcRPb, crossSection()/(microbarn*2*sumOfWeights())); // norm to cross section
      if (_h_LcRPb->numEntries()>0 && _h_LcR->numEntries()>0) divide(_h_LcRPb, _h_LcR, _h_RpPb);
    }

    //@}


    /// @name Histograms
    //@{
    Histo1DPtr _h_Lc, _h_LcPb, _h_D0, _h_D0Pb, _h_Lcint, _h_LcintPb, _h_D0int, _h_D0intPb, _h_LcR, _h_LcRPb, _h_Lcdummy, _h_LcPbdummy, _h_LcPbdummy0, _h_D0Pb0, d0pbnum, lcpbnum, d0pbnumw, lcpbnumw ;
    Scatter2DPtr _h_LcD0, _h_LcD0Pb, _h_LcD0int,  _h_LcD0Pbint, _h_RpPb, brutalLc, brutalD0;
    bool bo1, bo2, bo3;
    int lctot = 0, d0tot = 0;    

    //@}


  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ALICE_2017_I1645239);


}
