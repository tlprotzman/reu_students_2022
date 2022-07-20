#ifndef RHO_BACKGROUND_H
#define RHO_BACKGROUND_H

#include <fun4all/SubsysReco.h>

#include <phool/PHIODataNode.h>

#include <fastjet/AreaDefinition.hh>
#include <fastjet/ClusterSequence.hh>
#include <fastjet/ClusterSequenceArea.hh>
#include <fastjet/JetDefinition.hh>
#include <fastjet/PseudoJet.hh>
#include <fastjet/tools/BackgroundEstimatorBase.hh>
#include <fastjet/tools/GridMedianBackgroundEstimator.hh>
#include <fastjet/tools/JetMedianBackgroundEstimator.hh>
#include <fastjet/tools/Subtractor.hh>

#include <calobase/RawTowerContainer.h>
#include <calobase/RawTowerGeomContainer.h>

#include <vector>

class rho_background : public SubsysReco 
{
  public: 
  rho_background(const std::string &name, double reso);
  ~rho_background() override;

  int Init(PHCompositeNode *topnode) override;
  int process_event(PHCompositeNode *topnode) override;
  int ResetEvent(PHCompositeNode *topenode) override;

  private:
  void towers_to_pseudojets(RawTowerContainer *towers, RawTowerGeomContainer *geom, std::vector<fastjet::PseudoJet> &jets);

  const float ghost_maxrap = 1.1;
  double resolution;
  
  fastjet::GhostedAreaSpec area_spec;
  fastjet::GhostedAreaSpec background_area_spec;
  fastjet::AreaDefinition jet_area;
  fastjet::AreaDefinition background_jet_area;
  fastjet::JetDefinition jet_def;
  fastjet::JetDefinition background_subtraction_def;
  fastjet::Selector jet_selector;
  fastjet::JetMedianBackgroundEstimator *jet_background_estimator;
  fastjet::ClusterSequenceArea *cluster_sequence;
};

#endif // RHO_BACKGROUND_H
