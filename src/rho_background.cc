#include "rho_background.h"

#include <fun4all/Fun4AllReturnCodes.h>

#include <phool/PHCompositeNode.h>
#include <phool/PHIODataNode.h>
#include <phool/getClass.h>

// Not really sure which of these we need, lets just throw everything at it for now
#include <fastjet/AreaDefinition.hh>
#include <fastjet/ClusterSequence.hh>
#include <fastjet/ClusterSequenceArea.hh>
#include <fastjet/JetDefinition.hh>
#include <fastjet/PseudoJet.hh>
#include <fastjet/tools/BackgroundEstimatorBase.hh>
#include <fastjet/tools/GridMedianBackgroundEstimator.hh>
#include <fastjet/tools/JetMedianBackgroundEstimator.hh>
#include <fastjet/tools/Subtractor.hh>

#include <calobase/RawTower.h>
#include <calobase/RawTowerContainer.h>
#include <calobase/RawTowerGeom.h>
#include <calobase/RawTowerGeomContainer.h>

#include <vector>

#include "rho_map.h"


rho_background::rho_background(const std::string &name): SubsysReco(name)
{
  return;
}

rho_background::~rho_background()
{
  return;
}

int rho_background::Init(PHCompositeNode *topnode)
{
  // Looking for the DST node
  PHNodeIterator iter(topnode); 
  PHCompositeNode *dstNode = static_cast<PHCompositeNode *>(iter.findFirst("PHCompositeNode", "DST"));
  if (!dstNode)
  {
    std::cout << PHWHERE << "DST Node missing, doing nothing." << std::endl;
  }
  RhoMap *rho = findNode::getClass<RhoMap>(topnode, "rho");
  if (rho == nullptr) {
    RhoMap *rho = new RhoMap;
    PHIODataNode<RhoMap> *rho_node = new PHIODataNode<RhoMap>(rho, "rho", "rho_map");
    dstNode->addNode(rho_node);
  }
  

  this->background_area_spec = fastjet::GhostedAreaSpec(this->ghost_maxrap);
  this->background_jet_area = fastjet::AreaDefinition(fastjet::active_area_explicit_ghosts, this->background_area_spec);
  this->background_subtraction_def = fastjet::JetDefinition(fastjet::kt_algorithm, this->resolution);
  
  this->jet_selector = fastjet::SelectorAbsRapMax(1.1) * (!fastjet::SelectorNHardest(2)); // wtf?
  this->jet_background_estimator = new fastjet::JetMedianBackgroundEstimator(this->jet_selector, this->background_subtraction_def, this->background_jet_area);
  return Fun4AllReturnCodes::EVENT_OK;
}

int rho_background::process_event(PHCompositeNode *topnode)
{
  RhoMap *rho = findNode::getClass<RhoMap>(topnode, "rho");
  if (rho == nullptr) {
    std::cerr << PHWHERE << " cannot find rho node" << std::endl;
    return Fun4AllReturnCodes::ABORTEVENT;
  }

  // Get calorimeter data
  RawTowerContainer *towersEM = findNode::getClass<RawTowerContainer>(topnode, "TOWER_CALIB_CEMC");
  RawTowerContainer *towersIH = findNode::getClass<RawTowerContainer>(topnode, "TOWER_CALIB_HCALIN");
  RawTowerContainer *towersOH = findNode::getClass<RawTowerContainer>(topnode, "TOWER_CALIB_HCALOUT");

  RawTowerGeomContainer *geomEM = findNode::getClass<RawTowerGeomContainer>(topnode, "TOWERGEOM_CEMC");
  RawTowerGeomContainer *geomIH = findNode::getClass<RawTowerGeomContainer>(topnode, "TOWERGEOM_HCALIN");
  RawTowerGeomContainer *geomOH = findNode::getClass<RawTowerGeomContainer>(topnode, "TOWERGEOM_HCALOUT");

  // Make towers into pseudojets
  std::vector<fastjet::PseudoJet> tower_collection;
  this->towers_to_pseudojets(towersEM, geomEM, tower_collection);
  this->towers_to_pseudojets(towersIH, geomIH, tower_collection);
  this->towers_to_pseudojets(towersOH, geomOH, tower_collection);

  std::cout << "Jet finding on " << tower_collection.size() << " jets" << std::endl;
  this->jet_background_estimator->set_particles(tower_collection);
  (*(rho->rho))[4] = this->jet_background_estimator->rho();
  return Fun4AllReturnCodes::EVENT_OK;
}

void rho_background::towers_to_pseudojets(RawTowerContainer *towers, RawTowerGeomContainer *geom, std::vector<fastjet::PseudoJet> &jets)
{
  double vtxz = 0; // Currently assume the vertex is always at 0, probaby a fine assumption for now
  RawTowerContainer::ConstRange tower_range = towers->getTowers();
  for (RawTowerContainer::ConstIterator tower_iter = tower_range.first; tower_iter != tower_range.second; ++tower_iter)
  {
    // Get the current tower and its geometry 
    RawTower *tower = tower_iter->second;
    RawTowerGeom *tower_geom = geom->get_tower_geometry(tower->get_key());

    double r = tower_geom->get_center_radius();
    double phi = atan2(tower_geom->get_center_y(), tower_geom->get_center_x());
    double z0 = tower_geom->get_center_z();

    double z = z0 - vtxz;

    double eta = asinh(z / r);  // eta after shift from vertex

    double pt = tower->get_energy() / cosh(eta);
    double px = pt * cos(phi);
    double py = pt * sin(phi);
    double pz = pt * sinh(eta);

    fastjet::PseudoJet jet(px, py, pz, tower->get_energy());
    jets.push_back(jet);
  }
}
