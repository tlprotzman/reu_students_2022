#include "rho_map.h"

#include <fastjet/PseudoJet.hh>

#include <map>
#include <vector>

RhoMap::RhoMap() {
    this->rho = new std::map<int, double>;
    (*(this->rho))[2] = -1.;
    (*(this->rho))[3] = -1.;
    (*(this->rho))[4] = -1.;
    (*(this->rho))[5] = -1.;
    (*(this->rho))[6] = -1.;
    (*(this->rho))[7] = -1.;
    (*(this->rho))[8] = -1.;

    this->jets = new std::map<int, std::vector<fastjet::PseudoJet>*>;
    (*(this->jets))[2] = new std::vector<fastjet::PseudoJet>;
    (*(this->jets))[3] = new std::vector<fastjet::PseudoJet>;
    (*(this->jets))[4] = new std::vector<fastjet::PseudoJet>;
    (*(this->jets))[5] = new std::vector<fastjet::PseudoJet>;
    (*(this->jets))[6] = new std::vector<fastjet::PseudoJet>;
    (*(this->jets))[7] = new std::vector<fastjet::PseudoJet>;
    (*(this->jets))[8] = new std::vector<fastjet::PseudoJet>;
}

RhoMap::~RhoMap() {
    delete this->rho;
}

void RhoMap::clear_jets() {
    for (const auto &[key, value] : *(this->jets)) {
        value->clear();
    }
}