#include <G4_Bbc.C>
#include <G4_CaloTrigger.C>
#include <G4_Centrality.C>
#include <G4_DSTReader.C>
#include <G4_Global.C>
#include <G4_HIJetReco.C>
#include <G4_Input.C>
#include <G4_Jets.C>
#include <G4_KFParticle.C>
#include <G4_ParticleFlow.C>
#include <G4_Production.C>
#include <G4_TopoClusterReco.C>
#include <G4_Tracking.C>
#include <G4_User.C>
#include <QA.C>



#include <g4jets/JetMap.h>
#include <g4jets/JetReco.h>
#include <g4jets/TowerJetInput.h>
#include <g4jets/FastJetAlgo.h>

#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllServer.h>

#include "jet_tree_builder.h"

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libjet_tree_builder.so)
R__LOAD_LIBRARY(libg4jets.so)

void BuildJetTrees(int nEvents=0){

  Fun4AllServer *se = Fun4AllServer::instance();

  Input::VERBOSITY = 0;

  //load the data
  Fun4AllInputManager *truth_jet_manager = new Fun4AllDstInputManager("DST_TRUTH_JET");
  truth_jet_manager->AddListFile("dst_truth_jet.list");
  truth_jet_manager->Verbosity(Input::VERBOSITY);
  se->registerInputManager(truth_jet_manager);

  Fun4AllInputManager *calo_cluster_manager = new Fun4AllDstInputManager("DST_CALO_CLUSTER");
  calo_cluster_manager->AddListFile("dst_calo_cluster.list");
  calo_cluster_manager->Verbosity(Input::VERBOSITY);
  se->registerInputManager(calo_cluster_manager);

//  BbcInit();
//  Bbc_Reco();
//  Global_FastSim();

  // Find calo jets
  JetReco *calo_jets = new JetReco("calo_jets");
  calo_jets->add_input(new TowerJetInput(Jet::CEMC_TOWER));
  calo_jets->add_input(new TowerJetInput(Jet::HCALIN_TOWER));
  calo_jets->add_input(new TowerJetInput(Jet::HCALOUT_TOWER));
  calo_jets->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.4), "AntiKt_Tower_r04");
	calo_jets->set_algo_node("ANTIKT");
	calo_jets->set_input_node("TOWER");
	calo_jets->Verbosity(Input::VERBOSITY);
  se->registerSubsystem(calo_jets);

  // Register tree builder
//  jet_tree_builder *tree_builder = new jet_tree_builder("jet_tree_builder");
//  se->registerSubsystem(tree_builder);


  //run over the data
  se->run(nEvents);

  //complete
  se->End();
  std::cout << "All done" << std::endl;
  delete se;
}
