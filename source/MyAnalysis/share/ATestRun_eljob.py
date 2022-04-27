#!/usr/bin/env python

# Read the submission directory as a command line argument. You can
# extend the list of arguments with your private ones later on.
import optparse
parser = optparse.OptionParser()
parser.add_option( '-s', '--submission-dir', dest = 'submission_dir',
                   action = 'store', type = 'string', default = 'submitDir',
                   help = 'Submission directory for EventLoop' )
parser.add_option('-d', '--fileDirectory', type = 'string')
parser.add_option('-f', '--fileName', type = 'string')
( options, args ) = parser.parse_args()

# Set up (Py)ROOT.
import ROOT
ROOT.xAOD.Init().ignore()

# Set up the sample handler object. See comments from the C++ macro
# for the details about these lines.
import os
sh = ROOT.SH.SampleHandler()
sh.setMetaString( 'nc_tree', 'CollectionTree' )
#inputFilePath = os.getenv( 'ALRB_TutorialData' ) + '/mc16_13TeV.410470.PhPy8EG_A14_ttbar_hdamp258p75_nonallhad.deriv.DAOD_PHYS.e6337_s3126_r10201_p4172/' #tutorial
#inputFilePath = '/afs/cern.ch/work/e/ebusch/public/SVJ/mc16_13TeV' #cutflow
inputFilePath = '/afs/cern.ch/work/e/ebusch/public/SVJ/data/'+options.fileDirectory 
#ROOT.SH.ScanDir().filePattern( 'DAOD_PHYS.21569875._001323.pool.root.1' ).scan( sh, inputFilePath ) #tutorial
#ROOT.SH.ScanDir().filePattern( 'DAOD_PHYS.27236297._000003.pool.root.1' ).scan( sh, inputFilePath ) #cutflow
#ROOT.SH.ScanDir().filePattern( 'DAOD_EXOT3.test.508547.SVJSChan_1500_8.p9364.root' ).scan( sh, inputFilePath ) #jetm9
ROOT.SH.ScanDir().filePattern( options.fileName ).scan( sh, inputFilePath ) #jetm9
sh.printContent()

# Create an EventLoop job.
job = ROOT.EL.Job()
job.sampleHandler( sh )
#job.options().setDouble( ROOT.EL.Job.optMaxEvents, 500 )
job.options().setString( ROOT.EL.Job.optSubmitDirMode, 'unique-link')

#Creating trees
job.outputAdd (ROOT.EL.OutputStream ('ANALYSIS'))


# Create the algorithm's configuration.
from AnaAlgorithm.DualUseConfig import createAlgorithm
alg = createAlgorithm ( 'MyxAODAnalysis', 'AnalysisAlg' )

# later on we'll add some configuration options for our algorithm that go here
#alg.ElectronPtCut = 30000.0
#alg.SampleName = 'Zee'

# add the GRL tool to the algorithm
from AnaAlgorithm.DualUseConfig import addPrivateTool
addPrivateTool( alg, 'grlTool', 'GoodRunsListSelectionTool' )

# configure the properties of the GRL tool
#fullGRLFilePath = os.getenv ("ALRB_TutorialData") + "/data16_13TeV.periodAllYear_DetStatus-v89-pro21-01_DQDefects-00-02-04_PHYS_StandardGRL_All_Good_25ns.xml"
#alg.grlTool.GoodRunsListVec = [ fullGRLFilePath ]
alg.grlTool.PassThrough = 1 # if true (default) will ignore result of GRL and will just pass all events

# Add our algorithm to the job
job.algsAdd( alg )

# Run the job using the direct driver.
driver = ROOT.EL.DirectDriver()
driver.submit( job, options.submission_dir )
