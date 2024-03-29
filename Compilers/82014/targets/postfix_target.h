#ifndef __XPL_SEMANTICS_POSTFIX_TARGET_H__
#define __XPL_SEMANTICS_POSTFIX_TARGET_H__

#include <cdk/basic_target.h>
#include <cdk/symbol_table.h>
#include <cdk/ast/basic_node.h>
#include <cdk/compiler.h>
#include <cdk/emitters/postfix_ix86_emitter.h>
#include "targets/postfix_writer.h"
#include "targets/symbol.h"

namespace xpl {

  class postfix_target: public cdk::basic_target {
    static postfix_target _self;

  private:
    inline postfix_target() :
        cdk::basic_target("asm") {
    }

  public:
    bool evaluate(std::shared_ptr<cdk::compiler> compiler) {
      // this symbol table will be used to check identifiers
      // during code generation
      cdk::symbol_table<xpl::symbol> symtab;

      // this is the backend postfix machine
      cdk::postfix_ix86_emitter pf(compiler);
      compiler->debug(true);

      // generate assembly code from the syntax tree
      postfix_writer writer(compiler, symtab, pf);
      compiler->ast()->accept(&writer, 0);
      for(size_t i = 0; i < writer._specialFunctionsVector.size(); i++){
        pf.EXTERN(writer._specialFunctionsVector[i]);
      }
      return true;
    }

  };

} // xpl

#endif
