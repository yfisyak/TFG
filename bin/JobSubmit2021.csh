#! /bin/tcsh -f
cd ~/bin/; onl CURRENT | SortRun.pl | tee RunXXIDefs.pm
git diff .
git ci -m "Update" RunXXIDefs.pm
#cd ~/reco/2020/TFG19m/RF/11p5GeV.B
#cd CreateRunDirs.pl/net/l401/data/scratch2/reco/2020/TFG20a/RF/31p2GeV_fixedTarget
#cd /net/l401/data/scratch2/reco/2020/TFG20a/RF/9p8GeV_fixedTarget
#cd /net/l401/data/scratch2/reco/2020/TFG20a/RF/9p2GeV
#cd /net/l401/data/scratch2/reco/2020/TFG20a/RF/9p2GeVb
#cd /hlt/cephfs/reco/2021/RF/DEV2/7p7GeV_2021.C
cd /hlt/cephfs/reco/2021/RF/TFG21c.B/7p7GeV_2021
CreateRunDirs.pl
foreach d (`ls -1d ???/2*`)
  cd $d;
  if (-r Done || -r Submitted) then
    cd -
    continue
  endif
  daq_2021dR.pl
  if ($?) then
    ls -1d *bla.root
    if ($?) then
      rm sched* *xml fisyak.log;
      touch Done
    endif
    cd -;
    continue;
  endif
#  /net/l402/data/fisyak/STAR/packages/.DEV2/scripts/star-submit -p bnl_condor_online_CpuModelNumber63 ~/xml/daq_2021StiCA.Minuit.TFG19m.xml
#  /net/l402/data/fisyak/STAR/packages/.DEV2/scripts/star-submit -p bnl_condor_online_CpuModelNumber6X ~/xml/daq_2021StiCA.Minuit.TFG20a.xml
#  /net/l402/data/fisyak/STAR/packages/.DEV2/scripts/star-submit -p bnl_condor_online_CpuModelNumber6X ~/xml/daq_2021_Cosmics.xml
  /net/l402/data/fisyak/STAR/packages/.DEV2/scripts/star-submit -p bnl_condor_online_CpuModelNumber6X ~/xml/daq_2021.TFG21c.xml
  touch Submitted
  cd -
end
