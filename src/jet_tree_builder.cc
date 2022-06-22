
#include "jet_tree_builder.h"

#include <TTree.h>

#include <fun4all/Fun4AllReturnCodes.h>
#include <fun4all/Fun4AllHistoManager.h>
#include <fun4all/Fun4AllServer.h>

#include <g4jets/JetMap.h>

#include <phool/PHCompositeNode.h>
#include <phool/getClass.h>

#include "jet_data.h"

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
  std::cout << "I'm running!!" << std::endl;
  Fun4AllServer *server = Fun4AllServer::instance();
  this->hist_manager = new Fun4AllHistoManager("hist_manager");
  server->registerHistoManager(this->hist_manager);
  
  this->jet_tree = new TTree("jet_tree", "Jet Tree");
  this->jets = new std::vector<jet_data*>;
  for (uint i = 2; i <= 8; i++) {
    jets->push_back(new jet_data(i / 10.0, jet_tree));
  }



  hist_manager->registerHisto(this->jet_tree);
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
  std::cout << "Processing event!" << std::endl;
  for (jet_data *collection : *(this->jets)) {
    if (collection->fill_trees(topNode) == Fun4AllReturnCodes ::ABORTEVENT) {
      return Fun4AllReturnCodes::ABORTEVENT;
    }
  }
  jet_tree->Fill();

  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int jet_tree_builder::ResetEvent(PHCompositeNode *topNode)
{
  for (jet_data *collection : *(this->jets)) {
    collection->clear();
  }
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
  std::cout << "I'm ending!" << std::endl;
  this->hist_manager->dumpHistos("out.root");
  delete hist_manager;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int jet_tree_builder::Reset(PHCompositeNode *topNode)
{
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
void jet_tree_builder::Print(const std::string &what) const
{
  std::cout << "jet_tree_builder::Print(const std::string &what) const Printing info for " << what << std::endl;
}

