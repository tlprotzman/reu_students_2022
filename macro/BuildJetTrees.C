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
// #include <G4_Tracking.C>
#include <G4_User.C>
#include <QA.C>



#include <g4jets/JetMap.h>
#include <g4jets/JetReco.h>
#include <g4jets/TowerJetInput.h>
#include <g4jets/FastJetAlgo.h>

#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllServer.h>

#include <jetbackground/CopyAndSubtractJets.h>
#include <jetbackground/DetermineTowerBackground.h>
#include <jetbackground/FastJetAlgoSub.h>
#include <jetbackground/RetowerCEMC.h>
#include <jetbackground/SubtractTowers.h>
#include <jetbackground/SubtractTowersCS.h>

#include "jet_tree_builder.h"
#include "rho_background.h"

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libjet_tree_builder.so)
R__LOAD_LIBRARY(libg4jets.so)
R__LOAD_LIBRARY(libjetbackground.so)

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
  JetReco *calo_jets = new JetReco();
  calo_jets->add_input(new TowerJetInput(Jet::CEMC_TOWER));
  calo_jets->add_input(new TowerJetInput(Jet::HCALIN_TOWER));
  calo_jets->add_input(new TowerJetInput(Jet::HCALOUT_TOWER));
  calo_jets->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.2), "AntiKt_Tower_HIRecoSeedsRaw_r02");
	calo_jets->set_algo_node("ANTIKT");
	calo_jets->set_input_node("TOWER");
	calo_jets->Verbosity(Input::VERBOSITY);
  se->registerSubsystem(calo_jets);

  std::cout << "Calo stuff registered" << std::endl;

  // sPHENIX Background Subtraction Stuff
  RetowerCEMC *retower_cemc = new RetowerCEMC();
  retower_cemc->Verbosity(Input::VERBOSITY);
  se->registerSubsystem(retower_cemc);

  bool do_flow = false;   // Maybe?? 

  DetermineTowerBackground *tower_background = new DetermineTowerBackground();
  tower_background->SetBackgroundOutputName("TowerBackground_Sub1");
  tower_background->SetFlow(do_flow);
  tower_background->SetSeedType(0);
  tower_background->SetSeedJetD(3);
  tower_background->Verbosity(Input::VERBOSITY);
  se->registerSubsystem(tower_background);

  CopyAndSubtractJets *copy_subtract_jet = new CopyAndSubtractJets();
  copy_subtract_jet->SetFlowModulation(do_flow);
  copy_subtract_jet->Verbosity(Input::VERBOSITY);
  se->registerSubsystem(copy_subtract_jet);

  DetermineTowerBackground *tower_background_2 = new DetermineTowerBackground();
  tower_background_2->SetBackgroundOutputName("TowerBackground_Sub2");
  tower_background_2->SetFlow(do_flow);
  tower_background_2->SetSeedType(1);
  tower_background_2->SetSeedJetPt(7);
  tower_background_2->Verbosity(Input::VERBOSITY);
  se->registerSubsystem(tower_background_2);

  SubtractTowers *subtract_tower = new SubtractTowers();
  subtract_tower->SetFlowModulation(do_flow);
  subtract_tower->Verbosity(Input::VERBOSITY);
  se->registerSubsystem(subtract_tower);

  std::cout << "Registering everything but final jet finding" << std::endl;

  JetReco *subtracted_jets = new JetReco("subtracted_jets");
  subtracted_jets->add_input(new TowerJetInput(Jet::CEMC_TOWER_SUB1));
  subtracted_jets->add_input(new TowerJetInput(Jet::HCALIN_TOWER_SUB1));
  subtracted_jets->add_input(new TowerJetInput(Jet::HCALOUT_TOWER_SUB1));
  subtracted_jets->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.2), "AntiKt_Tower_sub_r02");
  subtracted_jets->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.3), "AntiKt_Tower_sub_r03");
  subtracted_jets->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.4), "AntiKt_Tower_sub_r04");
  subtracted_jets->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.5), "AntiKt_Tower_sub_r05");
  subtracted_jets->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.6), "AntiKt_Tower_sub_r06");
  subtracted_jets->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.7), "AntiKt_Tower_sub_r07");
  subtracted_jets->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.8), "AntiKt_Tower_sub_r08");
	subtracted_jets->set_algo_node("ANTIKT");
	subtracted_jets->set_input_node("TOWER");
	subtracted_jets->Verbosity(Input::VERBOSITY);
  se->registerSubsystem(subtracted_jets);

  



  std::cout << "Registering my module" << std::endl;
  
  // Register rho background subtractor
  rho_background *rho_background_sys = new rho_background("rho_background");
  se->registerSubsystem(rho_background_sys);

  // Register tree builder
  jet_tree_builder *tree_builder = new jet_tree_builder("jet_tree_builder");
  se->registerSubsystem(tree_builder);

  std::cout << "I'm about to run..." << std::endl;
  //run over the data
  se->run(nEvents);
  std::cout << "I ran!" << std::endl;
  //complete
  se->End();
  std::cout << "All done" << std::endl;
  delete se;
}
