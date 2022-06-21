#include <g4jets/JetMap.h>
#include <g4jets/JetReco.h>
#include <g4jets/TowerJetInput.h>
#include <g4jets/FastJetAlgo.h>

#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllServer.h>

#include "jet_tree_builder.h"

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libjet_tree_builder.so)

namespace Input {
  int VERBOSITY;
}

void BuildJetTrees(const std::string& truth_jets, const std::string& calo_towers, int nEvents=0){

  Fun4AllServer *se = Fun4AllServer::instance();

  Input::VERBOSITY = 0;

  //load the data
  Fun4AllInputManager *hitsin = new Fun4AllDstInputManager("DSTIN");
	hitsin->AddListFile(truth_jets);
  hitsin->AddListFile(calo_towers);
  hitsin->Verbosity(Input::VERBOSITY);
  se->registerInputManager(hitsin);

  // Find calo jets
  JetReco *calo_jets = new JetReco();
  calo_jets->add_input(new TowerJetInput(Jet::CEMC_TOWER));
  calo_jets->add_input(new TowerJetInput(Jet::HCALIN_TOWER));
  calo_jets->add_input(new TowerJetInput(Jet::HCALOUT_TOWER));
  calo_jets->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.4), "AntiKt_Tower_r04");
  se->registerSubsystem(calo_jets);

  // Register tree builder
  jet_tree_builder *tree_builder = new jet_tree_builder("jet_tree_builder");
  se->registerSubsystem(tree_builder);


  //run over the data
  se->run(nEvents);

  //complete
  se->End();
  std::cout << "All done" << std::endl;
  delete se;
}
