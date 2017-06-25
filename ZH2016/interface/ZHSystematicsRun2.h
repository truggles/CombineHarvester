#ifndef SM2016_ZHSystematicsRun2_h
#define SM2016_ZHSystematicsRun2_h
#include "CombineHarvester/CombineTools/interface/CombineHarvester.h"

namespace ch {
// Run2 SM analysis and AZh analysis systematics
// Implemented in src/ZHSystematicsRun2.cc
void AddZHRun2Systematics(CombineHarvester& cb, bool& azh);
}

#endif
