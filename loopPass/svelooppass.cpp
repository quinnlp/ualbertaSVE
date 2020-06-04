#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/IR/DebugLoc.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/DebugInfoMetadata.h"

using namespace llvm;

#define DEBUG_TYPE "svelooppass"

namespace {

  class MyFirstLoopPass: public LoopPass {
  public:
    static char ID;
    MyFirstLoopPass() : LoopPass(ID) {}
    bool runOnLoop(Loop *L, LPPassManager &LPM) override;
  };

  bool MyFirstLoopPass::runOnLoop(Loop *L, LPPassManager &LPM) {

    //errs() << "loop at line #" << L->getLocRange().getStart()->getLine() << '\n';
    if (L->getNumBlocks() > 0) {        // if the loop has control flow
      bool is_innermost = true;
      for (Loop *O : L->getLoopsInPreorder()) {
        //errs() << "innerLoop at line #" << O->getLocRange().getStart()->getLine() << '\n';
        if (O != L) {                   // contains another loop
          is_innermost = false;  
          break;    
        }
      }
      if (is_innermost) {                      // is an innermost loop
        errs()  << "Line #" << L->getLocRange().getStart()->getLine() 
        << "\t# Blocks: " << L->getNumBlocks() <<'\n';
      }
    }
    return false;
  }

} // end of anonymous namespace

char MyFirstLoopPass::ID = 0;
static RegisterPass<MyFirstLoopPass> X("svelooppass",
  "svelooppass: pass that prints the starting line of each loop", true, true);
