// Tell emacs that this is a C++ source
//  -*- C++ -*-.
#ifndef JET_TREE_BUILDER_H
#define JET_TREE_BUILDER_H

#include <TTree.h>

#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllHistoManager.h>

#include <string>
#include <vector>

class PHCompositeNode;

class jet_tree_builder : public SubsysReco
{
 public:

  jet_tree_builder(const std::string &name = "jet_tree_builder");

  ~jet_tree_builder() override;

  /** Called during initialization.
      Typically this is where you can book histograms, and e.g.
      register them to Fun4AllServer (so they can be output to file
      using Fun4AllServer::dumpHistos() method).
   */
  int Init(PHCompositeNode *topNode) override;

  /** Called for first event when run number is known.
      Typically this is where you may want to fetch data from
      database, because you know the run number. A place
      to book histograms which have to know the run number.
   */
  int InitRun(PHCompositeNode *topNode) override;

  /** Called for each event.
      This is where you do the real work.
   */
  int process_event(PHCompositeNode *topNode) override;

  /// Clean up internals after each event.
  int ResetEvent(PHCompositeNode *topNode) override;

  /// Called at the end of each run.
  int EndRun(const int runnumber) override;

  /// Called at the end of all processing.
  int End(PHCompositeNode *topNode) override;

  /// Reset
  int Reset(PHCompositeNode * /*topNode*/) override;

  void Print(const std::string &what = "ALL") const override;

 private:
    Fun4AllHistoManager *hist_manager;
    
    TTree *jet_data;
    double jet_R;

    int MAX_JETS;
    int num_jets;
    int g_num_jets;

    double *eta, *phi, *pt, *z;
    uint *constituents;
    double *g_eta, *g_phi, *g_pt, *g_z;
    uint *g_constituents;

    void setup_tree();

    
};

#endif // JET_TREE_BUILDER_H
