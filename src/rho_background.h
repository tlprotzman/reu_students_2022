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
  rho_background(const std::string &name = "rho_background");
  ~rho_background();

  int Init(PHCompositeNode *topnode) override;
  int process_event(PHCompositeNode *topnode) override;

  private:
  void towers_to_pseudojets(RawTowerContainer *towers, RawTowerGeomContainer *geom, std::vector<fastjet::PseudoJet> &jets);

  const float ghost_maxrap = 1.1;
  double resolution = 0.4;
  
  fastjet::GhostedAreaSpec background_area_spec;
  fastjet::AreaDefinition background_jet_area;
  fastjet::JetDefinition background_subtraction_def;
  fastjet::Selector jet_selector;
  fastjet::JetMedianBackgroundEstimator *jet_background_estimator;
};

#endif // RHO_BACKGROUND_H