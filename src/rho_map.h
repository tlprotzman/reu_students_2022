#ifndef RHO_MAP_H
#define RHO_MAP_H

#include <phool/PHObject.h>

#include <fastjet/PseudoJet.hh>

#include <map>
#include <vector>

class RhoMap : public PHObject
{
    public:
    RhoMap();
    ~RhoMap();
    void clear_jets();

    std::map<int, double> *rho;
    std::map<int, std::vector<fastjet::PseudoJet>*> *jets;
};

#endif // RHO_MAP_H