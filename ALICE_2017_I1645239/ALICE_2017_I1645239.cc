// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/UnstableParticles.hh"

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
      declare(UnstableParticles(Cuts::absrap < 0.96), "upProj");

      // Book histograms
      _h_Lc         = bookHisto1D(1, 1, 1);                             // Lc in pp at 7 TeV
      _h_LcPb       = bookHisto1D(2, 1, 1);                             // Lc in p-Pb at 5.02 TeV 
      _h_LcD0       = bookScatter2D(3, 1, 1);                           // Lc/D0 in pp at 7 TeV
      _h_LcD0Pb     = bookScatter2D(4, 1, 1);                           // Lc/D0 in p-Pb at 5.02 TeV
      _h_LcD0int    = bookScatter2D(5, 1, 1);                           // Integrated Lc/D0 in pp at 7 TeV
      _h_LcD0Pbint  = bookScatter2D(6, 1, 1);                           // Integrated Lc/D0 in p-Pb at 5.02 TeV
      _h_RpPb       = bookScatter2D(7, 1, 1);                           // RpPb 
      _h_Lcdummy    = bookHisto1D("TMP/Lcdummy", refData(3,1,1));       // Lc in pp at 7 TeV with _h_LcD0 bins
      _h_D0         = bookHisto1D("TMP/D0",      refData(3,1,1));       // D0 in pp at 7 TeV with _h_LcD0 bins
      _h_LcPbdummy  = bookHisto1D("TMP/LcPbdummy", refData(4,1,1));     // Lc in p-Pb at 5.02 TeV with _h_LcD0Pb bins
      _h_D0Pb       = bookHisto1D("TMP/D0Pb",      refData(4,1,1));     // D0 in p-Pb at 5.02 TeV with _h_LcD0Pb bins
      _h_Lcint      = bookHisto1D("TMP/Lcint", refData(5,1,1));         // Integrated Lc in pp at 7 TeV with _h_LcD0int bins
      _h_D0int      = bookHisto1D("TMP/D0int", refData(5,1,1));         // Integrated D0 in pp at 7 TeV with _h_LcD0int bins
      _h_LcintPb    = bookHisto1D("TMP/LcintPb", refData(6,1,1));       // Integrated Lc in p-Pb at 5.02 TeV with _h_LcD0Pbint bins
      _h_D0intPb    = bookHisto1D("TMP/D0intPb", refData(6,1,1));       // Integrated D0 in p-Pb at 5.02 TeV with _h_LcD0Pbint bins
      _h_LcR        = bookHisto1D("TMP/LcR",   refData(7,1,1));         // Lc in pp at 5.02 TeV with _h_RpPb bins
      _h_LcRPb      = bookHisto1D("TMP/LcRPb", refData(7,1,1));         // Lc in p-Pb at 5.02 TeV with _h_RpPb bins
     }


    /// Perform the per-event analysis
    void analyze(const Event& event) {
      PdgIdPair beamp; 
      beamp = beamIds();
      const double weight = event.weight();
      const UnstableParticles& upProj = apply<UnstableParticles>(event, "upProj");
      
      /*PDG code IDs used in the code: 2212 = p+, 4122 = Lc, 421 = D0, 1000822080 = Pb */
      if(beamp.first == 2212 && beamp.second == 2212){
        //pp cycle
        if(fuzzyEquals(sqrtS()/GeV,5020)){ // pp 5.02 TeV
         foreach (const Particle& p, upProj.particles()) {
            if(p.fromBottom())
                continue;
            else{    
                 if(p.rap() < 0.04 && p.rap() > -0.96){
                     if(p.abspid() == 4122)
                        _h_LcR->fill(p.pT()/GeV, weight);   
                 } // end if -0.96 < y< 0.04      
            } // if prompt 
         }// end foreach
        }
        else{
         foreach (const Particle& p, upProj.particles()) {  // pp 7 TeV
            if(p.fromBottom())
                continue;
            else{    
                 if(p.absrap() < 0.5){
                    if(p.abspid() == 421){
                        _h_D0       ->fill(p.pT()/GeV, weight); 
                        _h_D0int    ->fill(0,weight);
                    }// end if D0
                    else if(p.abspid() == 4122){
                        _h_Lc       ->fill(p.pT()/GeV, weight);
                        _h_Lcdummy  ->fill(p.pT()/GeV, weight);
                        _h_Lcint    ->fill(0,weight);
                    }// end if Lc
                 }// end if |y| < 0.5   
            }// end if prompt    
         }// end foreach
        }
      }
      else if((beamp.first == 2212 && beamp.second == 1000822080) || (beamp.second == 2212 && beamp.first == 1000822080)){
        //p-Pb cycle at 5.02 TeV
        foreach (const Particle& p, upProj.particles()) {
            if(p.fromBottom())
                continue;
            else{    
                 if(p.rap() < 0.04 && p.rap() > -0.96){
                    if(p.abspid() == 421){
                        _h_D0Pb         ->fill(p.pT()/GeV, weight); 
                        _h_D0intPb      ->fill(-0.5,weight);
                    }// end if D0
                    else if(p.abspid() == 4122){
                        _h_LcPb         ->fill(p.pT()/GeV, weight);
                        _h_LcPbdummy    ->fill(p.pT()/GeV, weight);
                        _h_LcRPb        ->fill(p.pT()/GeV, weight);
                        _h_LcintPb      ->fill(-0.5,weight);
                    }// if Lc
                 }// end if -0.96 < y< 0.04       
            }// end if prompt
         }// end foreach
       }// end p-Pb  

    }


    /// Normalise histograms etc., after the run
    void finalize() {
        // NOTE 1 : At this point cross sections consider both particles and antiparticles, 
        // hence a factor 2 is added in the histos normalization in order to account for this (as done in the paper)
        // NOTE 2 : any rapidity range here is 1-unit wide (in pp and p-Pb), no further division by 1 is requested to get dsigma/dpTdy cross section
      if(_h_D0->numEntries()>0)         scale(_h_D0,        crossSection()/(microbarn*2*sumOfWeights()));       // norm to cross section
      if(_h_D0int->numEntries()>0)      scale(_h_D0int,     crossSection()/(microbarn*2*sumOfWeights()));       // norm to cross section
      if(_h_Lc->numEntries()>0)         scale(_h_Lc,        crossSection()/(microbarn*2*sumOfWeights()));       // norm to cross section
      if(_h_Lcdummy->numEntries()>0)    scale(_h_Lcdummy,   crossSection()/(microbarn*2*sumOfWeights()));       // norm to cross section
      if(_h_LcPbdummy->numEntries()>0)  scale(_h_LcPbdummy, crossSection()/(microbarn*2*sumOfWeights()));       // norm to cross section
      if(_h_Lcint->numEntries()>0)      scale(_h_Lcint,     crossSection()/(microbarn*2*sumOfWeights()));       // norm to cross section
      if(_h_D0Pb->numEntries()>0)       scale(_h_D0Pb,      crossSection()/(microbarn*2*sumOfWeights()));       // norm to cross section
      if(_h_D0intPb->numEntries()>0)    scale(_h_D0intPb,   crossSection()/(microbarn*2*sumOfWeights()));       // norm to cross section
      if(_h_LcPb->numEntries()>0)       scale(_h_LcPb,      crossSection()/(microbarn*2*sumOfWeights()));       // norm to cross section
      if(_h_LcintPb->numEntries()>0)    scale(_h_LcintPb,   crossSection()/(microbarn*2*sumOfWeights()));       // norm to cross section
      
      if (_h_Lcdummy->numEntries()>0    && _h_D0->numEntries()>0)       divide(_h_Lcdummy,  _h_D0,      _h_LcD0);
      if (_h_LcPbdummy->numEntries()>0  && _h_D0Pb->numEntries()>0)     divide(_h_LcPbdummy,_h_D0Pb,    _h_LcD0Pb);
      if (_h_Lcint->numEntries()>0      && _h_D0int->numEntries()>0)    divide(_h_Lcint,    _h_D0int,   _h_LcD0int);
      if (_h_LcintPb->numEntries()>0    && _h_D0intPb->numEntries()>0)  divide(_h_LcintPb,  _h_D0intPb, _h_LcD0Pbint);
      
      if(_h_LcR->numEntries()>0)        scale(_h_LcR,       208*crossSection()/(microbarn*2*sumOfWeights()));   // norm to cross section, 208 factor accounts for the atomic number of Lead
      if(_h_LcRPb->numEntries()>0)      scale(_h_LcRPb,     crossSection()/(microbarn*2*sumOfWeights()));       // norm to cross section
      if (_h_LcRPb->numEntries()>0      && _h_LcR->numEntries()>0)      divide(_h_LcRPb,    _h_LcR,     _h_RpPb);
    }

    //@}


    /// @name Histograms
    //@{
    Histo1DPtr _h_Lc, _h_LcPb, _h_D0, _h_D0Pb, _h_Lcint, _h_LcintPb, _h_D0int, _h_D0intPb, _h_LcR, _h_LcRPb, _h_Lcdummy, _h_LcPbdummy;
    Scatter2DPtr _h_LcD0, _h_LcD0Pb, _h_LcD0int,  _h_LcD0Pbint, _h_RpPb;
    //bool bo1, bo2, bo3;
    //@}


  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ALICE_2017_I1645239);


}
