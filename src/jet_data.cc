#include "jet_data.h"

#include <phool/PHCompositeNode.h>
#include <phool/getClass.h>

#include <g4jets/JetMap.h>

#include <fun4all/Fun4AllReturnCodes.h>

#include <TTree.h>

#include <string>
#include <vector>

jet_data::jet_data(float _jet_r, TTree *_tree) {
  this->tree = _tree;
  this->jet_r = _jet_r;
  this->tag = std::string(Form("r0%i", (int)floor(this->jet_r * 10)));
  std::cout << "new jet_data with tag " << this->tag << std::endl;

  this->eta = new std::vector<double>;
  this->phi = new std::vector<double>;
  this->pt = new std::vector<double>;
  this->area = new std::vector<double>;
  this->rho = new std::vector<double>;

  this->sphenix_eta = new std::vector<double>;
  this->sphenix_phi = new std::vector<double>;
  this->sphenix_pt = new std::vector<double>;
  this->sphenix_area = new std::vector<double>;
  this->sphenix_rho = new std::vector<double>;
  
  this->g_eta = new std::vector<double>;
  this->g_phi = new std::vector<double>;
  this->g_pt = new std::vector<double>;
  this->g_area = new std::vector<double>;
  this->g_rho = new std::vector<double>;

  tree->Branch(Form("%s_eta", this->tag.c_str()), this->eta);
  tree->Branch(Form("%s_phi", this->tag.c_str()), this->phi);
  tree->Branch(Form("%s_pt", this->tag.c_str()), this->pt);
  tree->Branch(Form("%s_area", this->tag.c_str()), this->area);
  tree->Branch(Form("%s_rho", this->tag.c_str()), this->rho);

  tree->Branch(Form("%s_sphenix_eta", this->tag.c_str()), this->sphenix_eta);
  tree->Branch(Form("%s_sphenix_phi", this->tag.c_str()), this->sphenix_phi);
  tree->Branch(Form("%s_sphenix_pt", this->tag.c_str()), this->sphenix_pt);
  tree->Branch(Form("%s_sphenix_area", this->tag.c_str()), this->sphenix_area);
  tree->Branch(Form("%s_sphenix_rho", this->tag.c_str()), this->sphenix_rho);

  tree->Branch(Form("%s_g_eta", this->tag.c_str()), this->g_eta);
  tree->Branch(Form("%s_g_phi", this->tag.c_str()), this->g_phi);
  tree->Branch(Form("%s_g_pt", this->tag.c_str()), this->g_pt);
  tree->Branch(Form("%s_g_area", this->tag.c_str()), this->g_area);
  tree->Branch(Form("%s_g_rho", this->tag.c_str()), this->g_rho);

}

int jet_data::fill_trees(PHCompositeNode *topNode) {
  std::string truth_node_name(Form("AntiKt_Truth_%s", this->tag.c_str()));
  std::string tower_node_name(Form("AntiKt_Tower_%s", this->tag.c_str()));
  std::string subtracted_node_name(Form("AntiKt_Tower_sub_%s", this->tag.c_str()));

  JetMap *truth_jets = findNode::getClass<JetMap>(topNode, truth_node_name);
  JetMap *reco_jets  = findNode::getClass<JetMap>(topNode, tower_node_name);
  JetMap *subtracted_jets = findNode::getClass<JetMap>(topNode, subtracted_node_name);
  if (!truth_jets) {
    std::cout << PHWHERE << " Cannot find " << truth_node_name << std::endl;
    return Fun4AllReturnCodes::ABORTEVENT;
  }
  if (!reco_jets) {
    std::cout << PHWHERE << " Cannot find " << tower_node_name << std::endl;
    return Fun4AllReturnCodes::ABORTEVENT;
  }
  if (!subtracted_jets) {
    std::cout << PHWHERE << " Cannot find " << subtracted_node_name << std::endl;
    return Fun4AllReturnCodes::ABORTEVENT;
  }

  for (JetMap::Iter itr = truth_jets->begin(); itr != truth_jets->end(); itr++) {
    Jet *jet = itr->second;
    if (!jet) {
      continue;
    }
    g_eta->push_back(jet->get_eta());
    g_phi->push_back(jet->get_phi());
    g_pt->push_back(jet->get_pt());
  }

  for (JetMap::Iter itr = subtracted_jets->begin(); itr != subtracted_jets->end(); itr++) {
    Jet *jet = itr->second;
    if (!jet) {
      continue;
    }
    sphenix_eta->push_back(jet->get_eta());
    sphenix_phi->push_back(jet->get_phi());
    sphenix_pt->push_back(jet->get_pt());
  }

  for (JetMap::Iter itr = reco_jets->begin(); itr != reco_jets->end(); itr++) {
    Jet *jet = itr->second;
    if (!jet) {
      continue;
    }
    eta->push_back(jet->get_eta());
    phi->push_back(jet->get_phi());
    pt->push_back(jet->get_pt());
  }
  return Fun4AllReturnCodes::EVENT_OK;
}

void jet_data::clear() {
  this->eta->clear();
  this->phi->clear();
  this->pt->clear();
  this->area->clear();
  this->rho->clear();

  this->sphenix_eta->clear();
  this->sphenix_phi->clear();
  this->sphenix_pt->clear();
  this->sphenix_area->clear();
  this->sphenix_rho->clear();

  this->g_eta->clear();
  this->g_phi->clear();
  this->g_pt->clear();
  this->g_area->clear();
  this->g_rho->clear();
}