#include <AsgTools/MessageCheck.h>
#include <MyAnalysis/MyxAODAnalysis.h>
#include <xAODEventInfo/EventInfo.h>
#include <math.h>
#include <algorithm>

MyxAODAnalysis :: MyxAODAnalysis (const std::string& name,
                                  ISvcLocator *pSvcLocator)
    : EL::AnaAlgorithm (name, pSvcLocator),
      m_grl ("GoodRunsListSelectionTool/grl", this)
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  This is also where you
  // declare all properties for your algorithm.  Note that things like
  // resetting statistics variables or booking histograms should
  // rather go into the initialize() function.

  // declare the tool handle as a property on the algorithm
  declareProperty ("grlTool", m_grl, "the GRL tool");

  //declareProperty( "ElectronPtCut", m_electronPtCut = 25000.0,
  //                 "Minimum electron pT (in MeV)" );
  declareProperty( "SampleName", m_sampleName = "Unknown",
                   "Descriptive name for the processed sample" );
}



StatusCode MyxAODAnalysis :: initialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.
  ANA_MSG_INFO("in initialize");
  ANA_CHECK (book (TH1F ("h_jet1Pt", "h_jet1Pt", 100, 0, 500))); // jet pt [GeV]

  //ANA_MSG_INFO( "ElectronPtCut = " << m_electronPtCut );
  ANA_MSG_INFO( "SampleName    = " << m_sampleName );

  ANA_CHECK (m_grl.retrieve());

  ANA_CHECK (book (TTree ("Nominal", "My analysis ntuple")));
  TTree* mytree = tree ("Nominal");
  mytree->Branch ("runNumber", &m_runNumber);
  mytree->Branch ("eventNumber", &m_eventNumber);

  //float m_LargeRJets[2][4]={}; // !

  mytree->Branch ("Jet1Pt", &m_LargeRJets[0][0]);
  mytree->Branch ("Jet1Eta", &m_LargeRJets[0][1]);
  mytree->Branch ("Jet1Phi", &m_LargeRJets[0][2]);
  mytree->Branch ("Jet1M", &m_LargeRJets[0][3]);

  mytree->Branch ("Jet2Pt", &m_LargeRJets[1][0]);
  mytree->Branch ("Jet2Eta", &m_LargeRJets[1][1]);
  mytree->Branch ("Jet2Phi", &m_LargeRJets[1][2]);
  mytree->Branch ("Jet2M", &m_LargeRJets[1][3]);
 
  mytree->Branch ("Jet1_D2", &m_jet1D2);
  mytree->Branch ("Jet1_Tau32", &m_jet1Tau32);
  mytree->Branch ("Jet1_Split12", &m_jet1Split12);
  mytree->Branch ("Jet1_Split23", &m_jet1Split23);

  mytree->Branch ("Jet2_D2", &m_jet2D2);
  mytree->Branch ("Jet2_Tau32", &m_jet2Tau32);
  mytree->Branch ("Jet2_Split12", &m_jet2Split12);
  mytree->Branch ("Jet2_Split23", &m_jet2Split23);
  
  mytree->Branch ("nJets", &m_nJets);
  mytree->Branch ("Ht", &m_Ht);
  mytree->Branch ("MET_met", &m_MET_met);
  mytree->Branch ("MET_phi", &m_MET_phi);
  mytree->Branch ("dPhi_min", &m_dPhi_min);
  mytree->Branch ("dPhi_max", &m_dPhi_max);
  mytree->Branch ("MaxPhiMinPhi", &m_MaxPhiMinPhi);
  mytree->Branch ("pt_balance", &m_pt_balance);

  //float m_LargeRJets_constituents[2][20][3]={}; // !

  // Jet 1 constituents
  mytree->Branch ("Jet1Constituent1Pt",  &m_LargeRJets_constituents[0][0][0]);
  mytree->Branch ("Jet1Constituent1Eta", &m_LargeRJets_constituents[0][0][1]);
  mytree->Branch ("Jet1Constituent1Phi", &m_LargeRJets_constituents[0][0][2]);
  mytree->Branch ("Jet1Constituent2Pt",  &m_LargeRJets_constituents[0][1][0]);
  mytree->Branch ("Jet1Constituent2Eta", &m_LargeRJets_constituents[0][1][1]);
  mytree->Branch ("Jet1Constituent2Phi", &m_LargeRJets_constituents[0][1][2]);
  mytree->Branch ("Jet1Constituent3Pt",  &m_LargeRJets_constituents[0][2][0]);
  mytree->Branch ("Jet1Constituent3Eta", &m_LargeRJets_constituents[0][2][1]);
  mytree->Branch ("Jet1Constituent3Phi", &m_LargeRJets_constituents[0][2][2]);
  mytree->Branch ("Jet1Constituent4Pt",  &m_LargeRJets_constituents[0][3][0]);
  mytree->Branch ("Jet1Constituent4Eta", &m_LargeRJets_constituents[0][3][1]);
  mytree->Branch ("Jet1Constituent4Phi", &m_LargeRJets_constituents[0][3][2]);
  mytree->Branch ("Jet1Constituent5Pt",  &m_LargeRJets_constituents[0][4][0]);
  mytree->Branch ("Jet1Constituent5Eta", &m_LargeRJets_constituents[0][4][1]);
  mytree->Branch ("Jet1Constituent5Phi", &m_LargeRJets_constituents[0][4][2]);
  mytree->Branch ("Jet1Constituent6Pt",  &m_LargeRJets_constituents[0][5][0]);
  mytree->Branch ("Jet1Constituent6Eta", &m_LargeRJets_constituents[0][5][1]);
  mytree->Branch ("Jet1Constituent6Phi", &m_LargeRJets_constituents[0][5][2]);
  mytree->Branch ("Jet1Constituent7Pt",  &m_LargeRJets_constituents[0][6][0]);
  mytree->Branch ("Jet1Constituent7Eta", &m_LargeRJets_constituents[0][6][1]);
  mytree->Branch ("Jet1Constituent7Phi", &m_LargeRJets_constituents[0][6][2]);
  mytree->Branch ("Jet1Constituent8Pt",  &m_LargeRJets_constituents[0][7][0]);
  mytree->Branch ("Jet1Constituent8Eta", &m_LargeRJets_constituents[0][7][1]);
  mytree->Branch ("Jet1Constituent8Phi", &m_LargeRJets_constituents[0][7][2]);
  mytree->Branch ("Jet1Constituent9Pt",  &m_LargeRJets_constituents[0][8][0]);
  mytree->Branch ("Jet1Constituent9Eta", &m_LargeRJets_constituents[0][8][1]);
  mytree->Branch ("Jet1Constituent9Phi", &m_LargeRJets_constituents[0][8][2]);
  mytree->Branch ("Jet1Constituent10Pt",  &m_LargeRJets_constituents[0][9][0]);
  mytree->Branch ("Jet1Constituent10Eta", &m_LargeRJets_constituents[0][9][1]);
  mytree->Branch ("Jet1Constituent10Phi", &m_LargeRJets_constituents[0][9][2]);
  mytree->Branch ("Jet1Constituent11Pt",  &m_LargeRJets_constituents[0][10][0]);
  mytree->Branch ("Jet1Constituent11Eta", &m_LargeRJets_constituents[0][10][1]);
  mytree->Branch ("Jet1Constituent11Phi", &m_LargeRJets_constituents[0][10][2]);
  mytree->Branch ("Jet1Constituent12Pt",  &m_LargeRJets_constituents[0][11][0]);
  mytree->Branch ("Jet1Constituent12Eta", &m_LargeRJets_constituents[0][11][1]);
  mytree->Branch ("Jet1Constituent12Phi", &m_LargeRJets_constituents[0][11][2]);
  mytree->Branch ("Jet1Constituent13Pt",  &m_LargeRJets_constituents[0][12][0]);
  mytree->Branch ("Jet1Constituent13Eta", &m_LargeRJets_constituents[0][12][1]);
  mytree->Branch ("Jet1Constituent13Phi", &m_LargeRJets_constituents[0][12][2]);
  mytree->Branch ("Jet1Constituent14Pt",  &m_LargeRJets_constituents[0][13][0]);
  mytree->Branch ("Jet1Constituent14Eta", &m_LargeRJets_constituents[0][13][1]);
  mytree->Branch ("Jet1Constituent14Phi", &m_LargeRJets_constituents[0][13][2]);
  mytree->Branch ("Jet1Constituent15Pt",  &m_LargeRJets_constituents[0][14][0]);
  mytree->Branch ("Jet1Constituent15Eta", &m_LargeRJets_constituents[0][14][1]);
  mytree->Branch ("Jet1Constituent15Phi", &m_LargeRJets_constituents[0][14][2]);
  mytree->Branch ("Jet1Constituent16Pt",  &m_LargeRJets_constituents[0][15][0]);
  mytree->Branch ("Jet1Constituent16Eta", &m_LargeRJets_constituents[0][15][1]);
  mytree->Branch ("Jet1Constituent16Phi", &m_LargeRJets_constituents[0][15][2]);
  mytree->Branch ("Jet1Constituent17Pt",  &m_LargeRJets_constituents[0][16][0]);
  mytree->Branch ("Jet1Constituent17Eta", &m_LargeRJets_constituents[0][16][1]);
  mytree->Branch ("Jet1Constituent17Phi", &m_LargeRJets_constituents[0][16][2]);
  mytree->Branch ("Jet1Constituent18Pt",  &m_LargeRJets_constituents[0][17][0]);
  mytree->Branch ("Jet1Constituent18Eta", &m_LargeRJets_constituents[0][17][1]);
  mytree->Branch ("Jet1Constituent18Phi", &m_LargeRJets_constituents[0][17][2]);
  mytree->Branch ("Jet1Constituent19Pt",  &m_LargeRJets_constituents[0][18][0]);
  mytree->Branch ("Jet1Constituent19Eta", &m_LargeRJets_constituents[0][18][1]);
  mytree->Branch ("Jet1Constituent19Phi", &m_LargeRJets_constituents[0][18][2]);
  mytree->Branch ("Jet1Constituent20Pt",  &m_LargeRJets_constituents[0][19][0]);
  mytree->Branch ("Jet1Constituent20Eta", &m_LargeRJets_constituents[0][19][1]);
  mytree->Branch ("Jet1Constituent20Phi", &m_LargeRJets_constituents[0][19][2]);
  
  // Jet 2 constituents
  mytree->Branch ("Jet2Constituent1Pt",  &m_LargeRJets_constituents[1][0][0]);
  mytree->Branch ("Jet2Constituent1Eta", &m_LargeRJets_constituents[1][0][1]);
  mytree->Branch ("Jet2Constituent1Phi", &m_LargeRJets_constituents[1][0][2]);
  mytree->Branch ("Jet2Constituent2Pt",  &m_LargeRJets_constituents[1][1][0]);
  mytree->Branch ("Jet2Constituent2Eta", &m_LargeRJets_constituents[1][1][1]);
  mytree->Branch ("Jet2Constituent2Phi", &m_LargeRJets_constituents[1][1][2]);
  mytree->Branch ("Jet2Constituent3Pt",  &m_LargeRJets_constituents[1][2][0]);
  mytree->Branch ("Jet2Constituent3Eta", &m_LargeRJets_constituents[1][2][1]);
  mytree->Branch ("Jet2Constituent3Phi", &m_LargeRJets_constituents[1][2][2]);
  mytree->Branch ("Jet2Constituent4Pt",  &m_LargeRJets_constituents[1][3][0]);
  mytree->Branch ("Jet2Constituent4Eta", &m_LargeRJets_constituents[1][3][1]);
  mytree->Branch ("Jet2Constituent4Phi", &m_LargeRJets_constituents[1][3][2]);
  mytree->Branch ("Jet2Constituent5Pt",  &m_LargeRJets_constituents[1][4][0]);
  mytree->Branch ("Jet2Constituent5Eta", &m_LargeRJets_constituents[1][4][1]);
  mytree->Branch ("Jet2Constituent5Phi", &m_LargeRJets_constituents[1][4][2]);
  mytree->Branch ("Jet2Constituent6Pt",  &m_LargeRJets_constituents[1][5][0]);
  mytree->Branch ("Jet2Constituent6Eta", &m_LargeRJets_constituents[1][5][1]);
  mytree->Branch ("Jet2Constituent6Phi", &m_LargeRJets_constituents[1][5][2]);
  mytree->Branch ("Jet2Constituent7Pt",  &m_LargeRJets_constituents[1][6][0]);
  mytree->Branch ("Jet2Constituent7Eta", &m_LargeRJets_constituents[1][6][1]);
  mytree->Branch ("Jet2Constituent7Phi", &m_LargeRJets_constituents[1][6][2]);
  mytree->Branch ("Jet2Constituent8Pt",  &m_LargeRJets_constituents[1][7][0]);
  mytree->Branch ("Jet2Constituent8Eta", &m_LargeRJets_constituents[1][7][1]);
  mytree->Branch ("Jet2Constituent8Phi", &m_LargeRJets_constituents[1][7][2]);
  mytree->Branch ("Jet2Constituent9Pt",  &m_LargeRJets_constituents[1][8][0]);
  mytree->Branch ("Jet2Constituent9Eta", &m_LargeRJets_constituents[1][8][1]);
  mytree->Branch ("Jet2Constituent9Phi", &m_LargeRJets_constituents[1][8][2]);
  mytree->Branch ("Jet2Constituent10Pt",  &m_LargeRJets_constituents[1][9][0]);
  mytree->Branch ("Jet2Constituent10Eta", &m_LargeRJets_constituents[1][9][1]);
  mytree->Branch ("Jet2Constituent10Phi", &m_LargeRJets_constituents[1][9][2]);
  mytree->Branch ("Jet2Constituent11Pt",  &m_LargeRJets_constituents[1][10][0]);
  mytree->Branch ("Jet2Constituent11Eta", &m_LargeRJets_constituents[1][10][1]);
  mytree->Branch ("Jet2Constituent11Phi", &m_LargeRJets_constituents[1][10][2]);
  mytree->Branch ("Jet2Constituent12Pt",  &m_LargeRJets_constituents[1][11][0]);
  mytree->Branch ("Jet2Constituent12Eta", &m_LargeRJets_constituents[1][11][1]);
  mytree->Branch ("Jet2Constituent12Phi", &m_LargeRJets_constituents[1][11][2]);
  mytree->Branch ("Jet2Constituent13Pt",  &m_LargeRJets_constituents[1][12][0]);
  mytree->Branch ("Jet2Constituent13Eta", &m_LargeRJets_constituents[1][12][1]);
  mytree->Branch ("Jet2Constituent13Phi", &m_LargeRJets_constituents[1][12][2]);
  mytree->Branch ("Jet2Constituent14Pt",  &m_LargeRJets_constituents[1][13][0]);
  mytree->Branch ("Jet2Constituent14Eta", &m_LargeRJets_constituents[1][13][1]);
  mytree->Branch ("Jet2Constituent14Phi", &m_LargeRJets_constituents[1][13][2]);
  mytree->Branch ("Jet2Constituent15Pt",  &m_LargeRJets_constituents[1][14][0]);
  mytree->Branch ("Jet2Constituent15Eta", &m_LargeRJets_constituents[1][14][1]);
  mytree->Branch ("Jet2Constituent15Phi", &m_LargeRJets_constituents[1][14][2]);
  mytree->Branch ("Jet2Constituent16Pt",  &m_LargeRJets_constituents[1][15][0]);
  mytree->Branch ("Jet2Constituent16Eta", &m_LargeRJets_constituents[1][15][1]);
  mytree->Branch ("Jet2Constituent16Phi", &m_LargeRJets_constituents[1][15][2]);
  mytree->Branch ("Jet2Constituent17Pt",  &m_LargeRJets_constituents[1][16][0]);
  mytree->Branch ("Jet2Constituent17Eta", &m_LargeRJets_constituents[1][16][1]);
  mytree->Branch ("Jet2Constituent17Phi", &m_LargeRJets_constituents[1][16][2]);
  mytree->Branch ("Jet2Constituent18Pt",  &m_LargeRJets_constituents[1][17][0]);
  mytree->Branch ("Jet2Constituent18Eta", &m_LargeRJets_constituents[1][17][1]);
  mytree->Branch ("Jet2Constituent18Phi", &m_LargeRJets_constituents[1][17][2]);
  mytree->Branch ("Jet2Constituent19Pt",  &m_LargeRJets_constituents[1][18][0]);
  mytree->Branch ("Jet2Constituent19Eta", &m_LargeRJets_constituents[1][18][1]);
  mytree->Branch ("Jet2Constituent19Phi", &m_LargeRJets_constituents[1][18][2]);
  mytree->Branch ("Jet2Constituent20Pt",  &m_LargeRJets_constituents[1][19][0]);
  mytree->Branch ("Jet2Constituent20Eta", &m_LargeRJets_constituents[1][19][1]);
  mytree->Branch ("Jet2Constituent20Phi", &m_LargeRJets_constituents[1][19][2]);

  return StatusCode::SUCCESS;
}



StatusCode MyxAODAnalysis :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  //----------------------------
  // Event information
  //--------------------------- 
  const xAOD::EventInfo* eventInfo = 0;
  ANA_CHECK(evtStore()->retrieve( eventInfo, "EventInfo"));  
  
  // check if the event is data or MC
  // (many tools are applied either to data or MC)
  bool isMC = false;
  // check if the event is MC
  if (eventInfo->eventType (xAOD::EventInfo::IS_SIMULATION)) {
    isMC = true; // can do something with this later
  }

  // if data check if event passes GRL
  if (!isMC) { // it's data!
    if (!m_grl->passRunLB(*eventInfo)) {
      ANA_MSG_INFO ("drop event: GRL");
      return StatusCode::SUCCESS; // go to next event
    }
  } // end if not MC

  //ANA_MSG_INFO ("keep event: GRL");

  // print out run and event number from retrieved object
  //ANA_MSG_INFO ("execute(): runNumber = " << eventInfo->runNumber() << ", eventNumber = " << eventInfo->eventNumber());


  // MET
  const xAOD::MissingETContainer *m_metCont = nullptr;
  ANA_CHECK (evtStore()->retrieve (m_metCont, "MET_Core_AntiKt4EMPFlow"));
  const xAOD::MissingET* m_met;

  m_met = m_metCont->at(0);
  m_MET_met = m_met->met()*1e-3;
  m_MET_phi = m_met->phi();

  // loop over the jets in the container
  const xAOD::JetContainer* jets = nullptr;
  //ANA_CHECK (evtStore()->retrieve (jets, "AntiKt4EMPFlowJets")); // tutorial
  ANA_CHECK (evtStore()->retrieve (jets, "AntiKt10LCTopoTrimmedPtFrac5SmallR20Jets")); // analysis
  //ANA_MSG_INFO ("execute(): number of jets = " << jets->size());
  int j = 0; // jet counter
  float maxphi = 0;
  float minphi = 10; 
  float n_maxphi = 0;
  float n_minphi = 0;
  for (const xAOD::Jet* jet : *jets) {
    float dPhi = jet->phi() - m_MET_phi;
    if(dPhi > 3.14)  dPhi -= 2*3.14;
    if(dPhi < -3.14) dPhi += 2*3.14;
    if (fabs(dPhi) > maxphi){maxphi = fabs(dPhi); n_maxphi = j;}
    if (fabs(dPhi) < minphi){minphi = fabs(dPhi); n_minphi = j;}
    j++;
  }
  m_nJets = j;

  unsigned int k = 0; // jet counter
  unsigned int i = 0; // jet index (0 = minphi , 1 maxphi)
  for (const xAOD::Jet* jet : *jets) {
    if (k != n_maxphi && k != n_minphi){k++; continue;}
    if (k == n_minphi) i = 0;
    if (k == n_maxphi) i = 1;
 
    if(i==0) hist ("h_jet1Pt")->Fill (jet->pt() * 0.001); // GeV
    //std::vector< std::vector< float > > constituents = jet->auxdataConst< std::vector< std::vector<float> > >("Constituent4Vectors");
    m_LargeRJets[i][0] = jet->pt() * 1.e-3;
    m_LargeRJets[i][1] = jet->eta();
    m_LargeRJets[i][2] = jet->phi();
    m_LargeRJets[i][3] = jet->m() * 1.e-3;

    // MET dPhi
    if(i==0) m_dPhi_min = minphi; // fabs(m_LargeRJets[i][2] - m_MET_phi);
    if(i==1) m_dPhi_max = maxphi; //fabs(m_LargeRJets[i][2] - m_MET_phi);

    // D2
    const float& ecf1 = jet->getAttribute<float>("ECF1");
    const float& ecf2 = jet->getAttribute<float>("ECF2");
    const float& ecf3 = jet->getAttribute<float>("ECF3");
    float d2 = 0;
    if (fabs(ecf2) > 0) { d2 = ecf3 * pow(ecf1, 3) / pow(ecf2, 3); }
    if (i==0) {m_jet1D2 = d2;}
    else if (i==1) {m_jet2D2 = d2;} 

    // Tau32
    float tau2(-1.), tau3(-1.);
    float tau32(-1.);
    jet->getAttribute("Tau2_wta", tau2);
    jet->getAttribute("Tau3_wta", tau3);
    if (tau3 > 0.) tau32 = tau3 / tau2;
    if (i==0) {m_jet1Tau32 = tau32;}
    else if (i==1) {m_jet2Tau32 = tau32;}

    // Split12
    float d12(-1.), d23(-1.);
    jet->getAttribute("Split12", d12);
    jet->getAttribute("Split23", d23);
    if (i==0) {m_jet1Split12 = d12; m_jet1Split23 = d23;}
    else if (i==1) {m_jet2Split12 = d12; m_jet2Split23 = d23;}

    // Add constituents
    const auto& partLinks = jet->constituentLinks(); // IParticleLink Container
    //std::vector<std::vector<float>> constituent_fourvecs;

    int j = 0; //constituent counter
    for (const xAOD::IParticleLink& link : partLinks) {
        if(j >= 20) {break;}
        //if(const_ctr >= 40) {continue;}
        if (!link.isValid()) {
            // leaving warning commented out, because it happens almost every event for a few constituents of the lowest pT track jets.
            // Why? Perhaps a track that was skimmed out from EXOT3?
            // Warning("TrackJetHandler_YXH::decorate()", "Invalid track jet constituent link encountered. Skipping...");
            ANA_MSG_DEBUG("Link Invalid");
            continue;
        }

        const xAOD::IParticle* prt = dynamic_cast<const xAOD::IParticle*>(*link);
        //std::vector<float> const_fourvec;

        if (!prt) {
            // Warning("TrackJetHandler_YXH::decorate()", "Couldn't cast constituent EL to xAOD::TrackParticle. Skipping...");
            ANA_MSG_INFO("Couldn't cast constituent EL to xAOD::TrackParticle. Skipping...");
        } else {
            //ANA_MSG_INFO("jet constituent " << const_ctr << " pt: " << prt->p4().Pt());
            //ANA_MSG_INFO("jet constituent " << const_ctr << " eta: " << prt->p4().Eta());
            //ANA_MSG_INFO("jet constituent " << const_ctr << " phi: " << prt->p4().Phi());
            m_LargeRJets_constituents[i][j][0] = prt->p4().Pt() * 1.e-3; // Pt
            m_LargeRJets_constituents[i][j][1] = prt->p4().Eta(); // Eta
            m_LargeRJets_constituents[i][j][2] = prt->p4().Phi(); // Phi
            //constituent_fourvecs.push_back(const_fourvec);
            //constituent_fourvecs_for_d2.push_back(fastjet::PseudoJet(prt->p4().Px(), prt->p4().Py(), prt->p4().Pz(), prt->p4().E()));
        }
        j++;
    } // end loop over links

  } // end for loop over jets 

  // key observables
  m_Ht = m_LargeRJets[0][0] + m_LargeRJets[1][0];
  //m_dPhi_jetClosest_met = minphi; //std::min(m_dPhi_jet0_met, m_dPhi_jet1_met);
  m_MaxPhiMinPhi = maxphi - minphi; //fabs(m_dPhi_jet0_met - m_dPhi_jet1_met);
  // TODO : pt_balance
  TLorentzVector v1, v2, v12;
  v1.SetPtEtaPhiM(m_LargeRJets[0][0], m_LargeRJets[0][1], m_LargeRJets[0][2], m_LargeRJets[0][3]);
  v2.SetPtEtaPhiM(m_LargeRJets[1][0], m_LargeRJets[1][1], m_LargeRJets[1][2], m_LargeRJets[1][3]);
  v12 = v1 + v2;
  m_pt_balance = v12.Pt() / (v1.Pt() + v2.Pt()); 
  
  // Read/fill the EventInfo variables:
  const xAOD::EventInfo* ei = nullptr;
  ANA_CHECK (evtStore()->retrieve (ei, "EventInfo"));
  m_runNumber = ei->runNumber ();
  m_eventNumber = ei->eventNumber ();

  // Fill the event into the tree:
  tree ("Nominal")->Fill ();
 
  return StatusCode::SUCCESS;
}



StatusCode MyxAODAnalysis :: finalize ()
{
  // This method is the mirror image of initialize(), meaning it gets
  // called after the last event has been processed on the worker node
  // and allows you to finish up any objects you created in
  // initialize() before they are written to disk.  This is actually
  // fairly rare, since this happens separately for each worker node.
  // Most of the time you want to do your post-processing on the
  // submission node after all your histogram outputs have been
  // merged.
  return StatusCode::SUCCESS;
}

MyxAODAnalysis :: ~MyxAODAnalysis () {
  //delete m_jetEta;
  //delete m_jetPhi;
  //delete m_jetPt;
  //delete m_jetE;
}
