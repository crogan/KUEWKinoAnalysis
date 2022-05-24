#!/usr/bin/bash
# copy_samples.sh

# copy samples from one version to another

export old_version=v5
export new_version=v6

mkdir -p samples/NANO_SVSF/Summer16_102X_${new_version}
mkdir -p samples/NANO_SVSF/Fall17_102X_${new_version}
mkdir -p samples/NANO_SVSF/Autumn18_102X_${new_version}

rsync -az samples/NANO_SVSF/Summer16_102X_${old_version}/crab_TTJets-DiLept-FastSim-2016-${old_version}.txt samples/NANO_SVSF/Summer16_102X_${new_version}/crab_TTJets-DiLept-FastSim-2016-${new_version}.txt
rsync -az samples/NANO_SVSF/Summer16_102X_${old_version}/crab_TTJets-DiLept-FullSim-2016-${old_version}.txt samples/NANO_SVSF/Summer16_102X_${new_version}/crab_TTJets-DiLept-FullSim-2016-${new_version}.txt
rsync -az samples/NANO_SVSF/Fall17_102X_${old_version}/crab_TTJets-DiLept-FastSim-2017-${old_version}.txt samples/NANO_SVSF/Fall17_102X_${new_version}/crab_TTJets-DiLept-FastSim-2017-${new_version}.txt
rsync -az samples/NANO_SVSF/Fall17_102X_${old_version}/crab_TTJets-DiLept-FullSim-2017-${old_version}.txt samples/NANO_SVSF/Fall17_102X_${new_version}/crab_TTJets-DiLept-FullSim-2017-${new_version}.txt
rsync -az samples/NANO_SVSF/Autumn18_102X_${old_version}/crab_TTJets-DiLept-FastSim-2018-${old_version}.txt samples/NANO_SVSF/Autumn18_102X_${new_version}/crab_TTJets-DiLept-FastSim-2018-${new_version}.txt
rsync -az samples/NANO_SVSF/Autumn18_102X_${old_version}/crab_TTJets-DiLept-FullSim-2018-${old_version}.txt samples/NANO_SVSF/Autumn18_102X_${new_version}/crab_TTJets-DiLept-FullSim-2018-${new_version}.txt

