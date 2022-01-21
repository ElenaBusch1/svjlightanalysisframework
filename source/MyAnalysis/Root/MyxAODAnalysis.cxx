#include <AsgTools/MessageCheck.h>
#include <MyAnalysis/MyxAODAnalysis.h>
#include <xAODEventInfo/EventInfo.h>


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

  declareProperty( "ElectronPtCut", m_electronPtCut = 25000.0,
                   "Minimum electron pT (in MeV)" );
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
  ANA_CHECK (book (TH1F ("h_jetPt", "h_jetPt", 100, 0, 500))); // jet pt [GeV]

  ANA_MSG_INFO( "ElectronPtCut = " << m_electronPtCut );
  ANA_MSG_INFO( "SampleName    = " << m_sampleName );

  ANA_CHECK (m_grl.retrieve());

  ANA_CHECK (book (TTree ("analysis", "My analysis ntuple")));
  TTree* mytree = tree ("analysis");
  mytree->Branch ("RunNumber", &m_runNumber);
  mytree->Branch ("EventNumber", &m_eventNumber);
  m_jetEta = new std::vector<float>();
  mytree->Branch ("JetEta", &m_jetEta);
  m_jetPhi = new std::vector<float>();
  mytree->Branch ("JetPhi", &m_jetPhi);
  m_jetPt = new std::vector<float>();
  mytree->Branch ("JetPt", &m_jetPt);
  m_jetE = new std::vector<float>();
  mytree->Branch ("JetE", &m_jetE);

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


  // loop over the jets in the container
  const xAOD::JetContainer* jets = nullptr;
  //ANA_CHECK (evtStore()->retrieve (jets, "AntiKt4EMPFlowJets")); // tutorial
  ANA_CHECK (evtStore()->retrieve (jets, "AntiKt10LCTopoTrimmedPtFrac5SmallR20Jets")); // analysis
  //ANA_MSG_INFO ("execute(): number of jets = " << jets->size());
  for (const xAOD::Jet* jet : *jets) {
    hist ("h_jetPt")->Fill (jet->pt() * 0.001); // GeV
    //std::vector< std::vector< float > > constituents = jet->auxdataConst< std::vector< std::vector<float> > >("Constituent4Vectors");

    // Add constituents
    const auto& partLinks = jet->constituentLinks(); // IParticleLink Container
    //std::vector<std::vector<float>> constituent_fourvecs;

    int const_ctr = 0;
    for (const xAOD::IParticleLink& link : partLinks) {
        //if(const_ctr >= 20) {continue;}
        //if(const_ctr >= 40) {continue;}
        if (!link.isValid()) {
            // leaving warning commented out, because it happens almost every event for a few constituents of the lowest pT track jets.
            // Why? Perhaps a track that was skimmed out from EXOT3?
            // Warning("TrackJetHandler_YXH::decorate()", "Invalid track jet constituent link encountered. Skipping...");
            ANA_MSG_INFO("Link Invalid");
            continue;
        }

        const xAOD::IParticle* prt = dynamic_cast<const xAOD::IParticle*>(*link);
        //std::vector<float> const_fourvec;

        if (!prt) {
            // Warning("TrackJetHandler_YXH::decorate()", "Couldn't cast constituent EL to xAOD::TrackParticle. Skipping...");
            ANA_MSG_INFO("Couldn't cast constituent EL to xAOD::TrackParticle. Skipping...");
        } else {
            if(const_ctr < 40){
              //const_fourvec.push_back(prt->p4().Pt());
              //const_fourvec.push_back(prt->p4().Eta());
              //const_fourvec.push_back(prt->p4().Phi());
              ANA_MSG_INFO("jet constituent " << const_ctr << " pt: " << prt->p4().Pt());
              ANA_MSG_INFO("jet constituent " << const_ctr << " eta: " << prt->p4().Eta());
              ANA_MSG_INFO("jet constituent " << const_ctr << " phi: " << prt->p4().Phi());
              //constituent_fourvecs.push_back(const_fourvec);
            } else{
              ANA_MSG_INFO("const_ctr > 40");
            }
            //constituent_fourvecs_for_d2.push_back(fastjet::PseudoJet(prt->p4().Px(), prt->p4().Py(), prt->p4().Pz(), prt->p4().E()));
        }
        const_ctr++;
    } // end loop over links


    /*
    unsigned int j = 0; // Constituent Number
    for( auto constituent : constituents ) {
        //m_LargeRJets_constituents[i][j][0] = constituent[0] * 1.e-3; // Pt
        //m_LargeRJets_constituents[i][j][1] = constituent[1]; // Eta
        //m_LargeRJets_constituents[i][j][2] = constituent[2]; // Phi
        ANA_MSG_INFO("jet constituent " << j << " pt: " << constituent[0]);
        ANA_MSG_INFO("jet constituent " << j << " eta: " << constituent[1]);
        ANA_MSG_INFO("jet constituent " << j << " phi: " << constituent[2]);
        j++;
    } // end loop over constituents
    */
    //ANA_MSG_INFO ("execute(): jet pt = " << (jet->pt() * 0.001) << " GeV");
  } // end for loop over jets 


  // Read/fill the EventInfo variables:
  const xAOD::EventInfo* ei = nullptr;
  ANA_CHECK (evtStore()->retrieve (ei, "EventInfo"));
  m_runNumber = ei->runNumber ();
  m_eventNumber = ei->eventNumber ();
  m_jetEta->clear();
  m_jetPhi->clear();
  m_jetPt->clear();
  m_jetE->clear();
  for (const xAOD::Jet* jet : *jets) {
    m_jetEta->push_back (jet->eta ());
    m_jetPhi->push_back (jet->phi ());
    m_jetPt-> push_back (jet->pt ());
    m_jetE->  push_back (jet->e ());
  }
  // Fill the event into the tree:
  tree ("analysis")->Fill ();
 
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
  delete m_jetEta;
  delete m_jetPhi;
  delete m_jetPt;
  delete m_jetE;
}
