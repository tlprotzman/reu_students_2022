
#include "jet_tree_builder.h"

#include <TTree.h>

#include <fun4all/Fun4AllReturnCodes.h>
#include <fun4all/Fun4AllHistoManager.h>
#include <fun4all/Fun4AllServer.h>

#include <g4jets/JetMap.h>

#include <phool/PHCompositeNode.h>
#include <phool/getClass.h>

//____________________________________________________________________________..
jet_tree_builder::jet_tree_builder(const std::string &name):
 SubsysReco(name)
{
  return;
}

//____________________________________________________________________________..
jet_tree_builder::~jet_tree_builder()
{
  return;
}

//____________________________________________________________________________..
int jet_tree_builder::Init(PHCompositeNode *topNode)
{
  Fun4AllServer *server = Fun4AllServer::instance();
  this->hist_manager = new Fun4AllHistoManager("hist_manager");
  server->registerHistoManager(this->hist_manager);
  this->setup_tree();
  hist_manager->registerHisto(this->jet_data);
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int jet_tree_builder::InitRun(PHCompositeNode *topNode)
{
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int jet_tree_builder::process_event(PHCompositeNode *topNode)
{
  JetMap *truth_jets = findNode::getClass<JetMap>(topNode, "AntiKt_Truth_r04");
  JetMap *reco_jets  = findNode::getClass<JetMap>(topNode, "AntiKt_Tower_r04");
  if (!truth_jets) {
    std::cerr << PHWHERE << " Cannot find AntiKt_Truth_r04" << std::endl;
    return Fun4AllReturnCodes::ABORTRUN;
  }
  if (!reco_jets) {
    std::cerr << PHWHERE << " Cannot find AntiKt_Tower_r04" << std::endl;
    return Fun4AllReturnCodes::ABORTRUN;
  }

  for (JetMap::Iter itr = truth_jets->begin(); itr != truth_jets->end(); itr++) {
    if (g_num_jets >= MAX_JETS) {
      break;
    }Jet *jet = itr->second;
    g_eta[g_num_jets] = jet->get_eta();
    g_phi[g_num_jets] = jet->get_phi();
    g_pt[g_num_jets] = jet->get_pt();
    g_num_jets++;
  }

  for (JetMap::Iter itr = reco_jets->begin(); itr != reco_jets->end(); itr++) {
    if (num_jets >= MAX_JETS) {
      break;
    }
    Jet *jet = itr->second;
    eta[num_jets] = jet->get_eta();
    phi[num_jets] = jet->get_phi();
    pt[num_jets] = jet->get_pt();
    num_jets++;
  }
  jet_data->Fill();


  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int jet_tree_builder::ResetEvent(PHCompositeNode *topNode)
{
  num_jets = 0;
  g_num_jets = 0;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int jet_tree_builder::EndRun(const int runnumber)
{
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int jet_tree_builder::End(PHCompositeNode *topNode)
{
  this->hist_manager->dumpHistos("out.root");
  delete hist_manager;
  delete jet_data;
  
  free(this->eta);
  free(this->phi);
  free(this->pt);

  free(this->g_eta);
  free(this->g_phi);
  free(this->g_pt);

  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int jet_tree_builder::Reset(PHCompositeNode *topNode)
{
  this->num_jets = 0;
  this->g_num_jets = 0;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
void jet_tree_builder::Print(const std::string &what) const
{
  std::cout << "jet_tree_builder::Print(const std::string &what) const Printing info for " << what << std::endl;
}

void jet_tree_builder::setup_tree() {
  jet_data = new TTree("jet_data", "Jet Data");
  jet_data->Branch("MAX_JETS", &this->MAX_JETS);
  jet_data->Branch("jet_R", &this->jet_R);
  jet_data->Branch("num_jets", &this->num_jets);
  jet_data->Branch("g_num_jets", &this->g_num_jets);

  this->eta = (double*) malloc(this->MAX_JETS * sizeof(double));
  this->phi = (double*) malloc(this->MAX_JETS * sizeof(double));
  this->pt = (double*) malloc(this->MAX_JETS * sizeof(double));
  
  this->g_eta = (double*) malloc(this->MAX_JETS * sizeof(double));
  this->g_phi = (double*) malloc(this->MAX_JETS * sizeof(double));
  this->g_pt = (double*) malloc(this->MAX_JETS * sizeof(double));

  jet_data->Branch("eta", this->eta, "eta[num_jets]/D");
  jet_data->Branch("phi", this->phi, "phi[num_jets]/D");
  jet_data->Branch("pt", this->pt, "pt[num_jets]/D");

  jet_data->Branch("g_eta", this->g_eta, "g_eta[g_num_jets]/D");
  jet_data->Branch("g_phi", this->g_phi, "g_phi[g_num_jets]/D");
  jet_data->Branch("g_pt", this->g_pt, "g_pt[g_num_jets]/D");

  return;
}