#ifndef JET_DATA_H
#define JET_DATA_H

#include <phool/PHCompositeNode.h>

#include <TTree.h>

class jet_data 
{
    public:
        TTree *tree;
        std::string tag;
        float jet_r;
        std::vector<double> *eta, *phi, *pt;
        std::vector<double> *g_eta, *g_phi, *g_pt;


        jet_data(float _jet_r, TTree *_tree);
        ~jet_data();
        int fill_trees(PHCompositeNode *topNode);
        void clear();

};

#endif // JET_DATA_H