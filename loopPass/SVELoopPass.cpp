#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/IR/DebugLoc.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/IR/Dominators.h"
#include "llvm/InitializePasses.h"
#include "llvm/ADT/APFloat.h"

using namespace llvm;

#define DEBUG_TYPE "SVELoopPass"

namespace llvm {
void initializeSVELoopPassPass(llvm::PassRegistry&);
}

namespace {

  class SVELoopPass: public LoopPass {
  public:
    static char ID;
    SVELoopPass() : LoopPass(ID) {}
    bool runOnLoop(Loop *L, LPPassManager &LPM) override;
    void getAnalysisUsage(AnalysisUsage &AU) const override {
      AU.addRequired<DominatorTreeWrapperPass>();
    }

  };

  bool SVELoopPass::runOnLoop(Loop *L, LPPassManager &LPM) {
    bool is_innermost = true;
    bool has_controlflow = true;      // TO CATCH CONTROL FLOW ONLY switch to false and uncomment lines 47-58
    //errs()  << "Looking at Line #" << L->getLocRange().getStart()->getLine() << '\n';
    for (Loop *loop : L->getLoopsInPreorder()) {
      if (loop != L) {                   // contains another loop
        is_innermost = false;
        //errs()  << "    Killed with Line #" << loop->getLocRange().getStart()->getLine() << '\n';
        break;
      }
    }
    DominatorTree &DT = LPM.getAnalysis<DominatorTreeWrapperPass>().getDomTree();
    if (is_innermost) {                  // is an innermost loop
/*      for (BasicBlock *B : L->getBlocks()) {
        //errs() << "Checking " << B->front().getDebugLoc().getLine() << '\n';
        for (BasicBlock *P : predecessors(B)) {
          //errs() << "   Does " << P->front().getDebugLoc().getLine() << " dominate " << B->front().getDebugLoc().getLine() << "? ";
          if (!DT.dominates(P, B) && L->contains(P) && B != L->getHeader()) {
            has_controlflow = true;
            //errs() << "no\n";
          } else{
            //errs() << "yes\n";
          }
        }
      }*/
      if (has_controlflow) {
        errs() << L->getLocRange().getStart()->getLine() << '\n';
      }
    }
    return false;
  }


} // end of anonymous namespace



char SVELoopPass::ID = 0;
INITIALIZE_PASS_BEGIN(SVELoopPass, "SVELoopPass", "SVELoopPass", false, false)
INITIALIZE_PASS_DEPENDENCY(DominatorTreeWrapperPass)
INITIALIZE_PASS_END(SVELoopPass, "SVELoopPass", "SVELoopPass", false, false)
static RegisterPass<SVELoopPass> X("SVELoopPass", "SVELoopPass: pass that prints the starting line of each loop", true, true);
