#include <G4_CEmc_Spacal.C>
#include <G4_HcalIn_ref.C>
#include <G4_HcalOut_ref.C>
#include <G4_TopoClusterReco.C>
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllServer.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libjetbackground.so)

namespace Input {
  int VERBOSITY;
}

namespace G4HIJETS {
  bool do_flow;
}

void BuildJetTrees(const std::string& filename, int nEvents=0){

  Fun4AllServer *se = Fun4AllServer::instance();

  Input::VERBOSITY = 0;

  //load the data
  Fun4AllInputManager *hitsin = new Fun4AllDstInputManager("DSTIN");
	hitsin->AddListFile(filename);
  hitsin->Verbosity(Input::VERBOSITY);
  se->registerInputManager(hitsin);

  //run over the data
  se->run(nEvents);

  //complete
  se->End();
  std::cout << "All done" << std::endl;
  delete se;
}
