#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include "TH1.h"
#include "TH2.h"
#include "TGriffin.h"
#include "TGriffinBgo.h"
#include "TChain.h"
#include "TVector3.h"
#include "TMath.h"

class EventManager
{
public:
    void ConvertRootData(TChain *inputChain);
    void GetEventData(TChain *gChain);


private:
    void PreProcessData();

    TGriffin *fGrif = NULL;
    TGriffinBgo *fGriffinBgo = NULL;

    int num_crystals = 64;
    std::vector<double> angle_combinations_vec = {15.442, 21.9054, 29.1432, 33.1433, 38.382, 44.57, 47.4453, 48.7411, 51.4734, 55.1704, 59.9782, 60.1024, 62.3396, 62.4924, 63.4231, 68.9567, 71.4314, 73.3582, 73.6291, 75.7736, 80.9423, 81.5464, 83.8936, 86.868, 88.9658, 91.0342, 93.132, 96.1064, 98.4536, 99.0577, 104.226, 106.371, 106.642, 108.569, 111.043, 116.577, 117.508, 117.66, 119.898, 120.022, 124.83, 128.527, 131.259, 132.555, 135.43, 141.618, 146.857, 150.857, 158.095, 164.558, 180.0};

    std::vector<float> energy_vec; // vector which contains the energy values
    std::vector<float> charge_vec; // vector which contains the charge values
    std::vector<long> time_vec; // vector which contains the time values
    std::vector<TVector3> pos_vec; // vector which contains the position values
    std::vector<float> kvalue_vec; // vector containing k-values
    std::vector<int> det_vec; // vector containing detector id's

};

#endif
