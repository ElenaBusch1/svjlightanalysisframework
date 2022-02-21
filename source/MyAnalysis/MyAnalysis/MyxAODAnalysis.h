#ifndef MyAnalysis_MyxAODAnalysis_H
#define MyAnalysis_MyxAODAnalysis_H

#include <AnaAlgorithm/AnaAlgorithm.h>
#include <TH1.h>
#include <xAODJet/JetContainer.h>
// GRL
#include <AsgAnalysisInterfaces/IGoodRunsListSelectionTool.h>
#include <AsgTools/ToolHandle.h>
// Trees
#include <TTree.h>
#include <vector>
// Jets
#include "xAODParticleEvent/IParticleLink.h"
#include "xAODParticleEvent/IParticleLinkContainer.h"

class MyxAODAnalysis : public EL::AnaAlgorithm
{
public:
  // this is a standard algorithm constructor
  MyxAODAnalysis (const std::string& name, ISvcLocator* pSvcLocator);

  // these are the functions inherited from Algorithm
  virtual StatusCode initialize () override;
  virtual StatusCode execute () override;
  virtual StatusCode finalize () override;

  ~MyxAODAnalysis();

private:
  // Configuration, and any other types of variables go here.
  //float m_cutValue;
  //TTree *m_myTree;
  //TH1 *m_myHist;

  /// Electron pT cut
  double m_electronPtCut;
  /// Sample name
  std::string m_sampleName;

  ToolHandle<IGoodRunsListSelectionTool> m_grl;

  /// output variables for the current event
  /// \{
  unsigned int m_runNumber = 0; ///< Run number
  unsigned long long m_eventNumber = 0; ///< Event number
  /// Jet 4-momentum variables
  //std::vector<float> *m_jet1Eta = nullptr;
  //std::vector<float> *m_jet1Phi = nullptr;
  //std::vector<float> *m_jet1Pt = nullptr;
  //std::vector<float> *m_jet1E = nullptr;
  float m_LargeRJets[2][4] = {};
  float m_LargeRJets_constituents[2][20][3] = {};
 
  float m_jet1D2 = 0;
  float m_jet2D2 = 0;
  float m_jet1Tau32 = 0;
  float m_jet2Tau32 = 0;
  float m_jet1Split12 = 0;
  float m_jet2Split12 = 0;
  float m_jet1Split23 = 0;
  float m_jet2Split23 = 0;
  /// \}
};

#endif
