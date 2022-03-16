A very simple framework for producing light SVJ nTuples to feed into the YXH VRNN frame work.

Instructions (first time):
1. `mkdir build`
2. `mkdir run`
3. `cd build`
4. `cmake ../source`
5. `make`
6. `source x86\_64\*/setup.sh`

Instructions (general):
1. Algorithm lives in source/MyAnalysis/Root/MyxAODAnalysis.cxx
2. Header file lives in source/MyAnalysis/MyAnalysis/MyxAODAnalysis.h
3. Configuration lives in source/MyAnalysis/share/ATestRun\_eljob.py
4. Make sure ATestRun\_eljob.py is executable (chmod +x source/MyAnalysis/share/ATestRun\_eljob.py)
5. When the code is changed, compile with `cd ../build`, `make`, `source x86\_64\*/setup.sh`. If a package or file was added, rerun `cmake ../source` before `make`.
6. To run, `cd ../run`, `ATestRun_eljob.py --submission-dir=submitDir`
7. nTuple is in submitDir/data-ANALYSIS
8. Currently setting up with `asetup AnalysisBase 21.2.189`

Notes on this version:
1. This branch is dedicated to creating nTuple which focus on max\_phi and min\_phi jets, not leading and subleading

Notes:
1. Input file is currently hardcoded in ATestRun\_eljob.py . Set to your desired input (samples in /eos/atlas/atlascerngroupdisk/phys-exotics/jdm/svjets-schannel/cutflow suggested)

