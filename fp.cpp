#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/Support/Casting.h" 
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Metadata.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/IR/CallSite.h"

   using namespace llvm;

   namespace {

       struct Fprofiler: public ModulePass {

           static char ID;

          Fprofiler (): ModulePass(ID) {}
 
        virtual bool runOnModule(Module &M) {
         
                long dcallsites=0,icallsites=0,numf=0;
                long intfun=0;                

             for (auto &F : M) { 
                  Function *pf = &F;
                   
                     if (pf->isIntrinsic()) { intfun++; continue;}
                          numf++;

                    //errs()<<"In function"<<pf->getName()<<"\n";

             for (auto &B : F) 
             for (auto &I : B) {
                   
                 Instruction *pi = &I;
              if ( isa<DbgInfoIntrinsic>(pi))  continue;  



           if (isa<CallInst>(pi)) {
                      CallSite cs= CallSite( cast<CallInst>(pi));

               // Function *fun = ci->getCalledFunction();
                      if( !cs.isIndirectCall()) dcallsites++;
                      else  {  
                                MDNode *N = pi->getMetadata("dbg"); 
                                DILocation* Loc = cast<DILocation>(N);      
                               unsigned Line = Loc->getLine();
                              StringRef File = Loc->getFilename();
                     errs() << " Indirect call in  file "<< File<<" at  Line " << Line << "\n";
                                  icallsites++; 
                 }
              }
             }
          }
             
            errs()<< "Total number of functions " << numf <<"\n";  
            errs() << " Total callsites  " << dcallsites+icallsites << "\n";
            errs() << " Direct callsites  " << dcallsites << "\n";
            errs() << " InDirect callsites  " << icallsites << "\n";
            errs() << " Intrinsic functions " << intfun <<"\n";
          return false;
    }

    };

  }

       char Fprofiler::ID = 0;
   static RegisterPass<Fprofiler> X("fprofiler", "Function Profiler Pass",false,true);





