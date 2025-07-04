// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/UnstableFinalState.hh"
#include "Rivet/Projections/FastJets.hh"

namespace Rivet {


  /// @brief Add a short analysis description here
  class ALICE_2012_I944757 : public Analysis {
  public:

    /// Constructor
    DEFAULT_RIVET_ANALYSIS_CTOR(ALICE_2012_I944757);


    /// @name Analysis methods
    //@{

    /// Book histograms and initialise projections before the run
    void init() {

      // Initialise and register projections
      declare(UnstableFinalState(Cuts::absrap < 0.5), "UFS");
      

      // Book histograms
      _h_D0 = bookHisto1D(1, 1, 1);
      _h_Dplus = bookHisto1D(2, 1, 1);
      _h_Dstarp= bookHisto1D(3, 1, 1);
      _h_integ = bookHisto1D(4, 1, 1);

    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {
        const double weight = event.weight();
        const UnstableFinalState& ufs = apply<UnstableFinalState>(event, "UFS");
        
        /*PDG code IDs used inside the foreach cycle: 421 = D0, 411 = D+, 413 = D*+ */

        foreach (const Particle& p, ufs.particles()) {
            if(p.fromBottom())
                continue;
            else
                {    
                if(p.abspid() == 421){
                    _h_D0->fill(p.pT()/GeV, weight); 
                    _h_integ->fill(1,weight);}
                else if(p.abspid() == 411){
                    _h_Dplus->fill(p.pT()/GeV, weight);
                    _h_integ->fill(2,weight);}
                else if(p.abspid()== 413){
                    _h_Dstarp->fill(p.pT()/GeV, weight);    
                    _h_integ->fill(3,weight);}
                }
        }
        

    }


    /// Normalise histograms etc., after the run
    void finalize() {

      //normalize(_h_YYYY); // normalize to unity
      scale(_h_D0, crossSection()/(microbarn*2*sumOfWeights())); // norm to cross section
      scale(_h_Dplus, crossSection()/(microbarn*2*sumOfWeights())); // norm to cross section
      scale(_h_Dstarp, crossSection()/(microbarn*2*sumOfWeights())); // norm to cross section
      scale(_h_integ, crossSection()/(microbarn*2*sumOfWeights())); // norm to cross section
      /* Obtained cross sections data at this point consider both particles and antiparticles 
      hence the added factor 2 in the normalization solves the issue (as done in the paper) */
    }

    //@}


    /// @name Histograms
    //@{
    Histo1DPtr _h_D0, _h_Dplus, _h_Dstarp, _h_integ;
    //Profile1DPtr _p_AAAA;
    //CounterPtr _c_BBBB;
    //@}


  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ALICE_2012_I944757);


}
