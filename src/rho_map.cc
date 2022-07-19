#include "rho_map.h"

#include <map>

RhoMap::RhoMap() {
    this->rho = new std::map<int, double>;
    (*(this->rho))[2] = -1.;
    (*(this->rho))[3] = -1.;
    (*(this->rho))[4] = -1.;
    (*(this->rho))[5] = -1.;
    (*(this->rho))[6] = -1.;
    (*(this->rho))[7] = -1.;
    (*(this->rho))[8] = -1.;
}

RhoMap::~RhoMap() {
    delete this->rho;
}