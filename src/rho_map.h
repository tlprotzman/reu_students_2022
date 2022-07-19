#ifndef RHO_MAP_H
#define RHO_MAP_H

#include <phool/PHObject.h>

#include <map>

class RhoMap : public PHObject
{
    public:
    RhoMap();
    ~RhoMap();

    std::map<int, double> *rho;
};

#endif // RHO_MAP_H