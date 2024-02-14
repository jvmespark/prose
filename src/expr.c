#include "../headers/defs.h"
#include "../headers/data.h"
#include "../headers/decl.h"

static struct ASTnode *primary(void) {
  struct ASTnode *n;
  int id;

  switch (Token.token) {
    case T_INTLIT:
      n = mkastleaf(A_INTLIT, Token.intvalue);
      break;
    case T_IDENT:
      id = findglob(Text);
      if (id == -1)
        fatals("Unknown variable", Text);
      n = mkastleaf(A_IDENT, id);
      break;
    default:
      fatald("Syntax error, token", Token.token);
  }
  scan(&Token);
  return n;
}



static int arithop(int tokentype) {
  switch (tokentype) {
    case T_PLUS:
      return (A_ADD);
    case T_MINUS:
      return (A_SUBTRACT);
    case T_STAR:
      return (A_MULTIPLY);
    case T_SLASH:
      return (A_DIVIDE);
    default:
      fprintf(stderr, "syntax error on line %d, token %d\n", Line, tokentype);
      exit(1);
  }
}


static int OpPrec[] = { 0, 10, 10, 20, 20, 0 };



static int op_precedence(int tokentype) {
  int prec = OpPrec[tokentype];
  if (prec == 0) {
    fprintf(stderr, "syntax error on line %d, token %d\n", Line, tokentype);
    exit(1);
  }
  return (prec);
}



struct ASTnode *binexpr(int ptp) {
  struct ASTnode *left, *right;
  int tokentype;

  left = primary();
  tokentype = Token.token;

  if (tokentype == T_SEMI)
    return (left);

  while (op_precedence(tokentype) > ptp) {
    scan(&Token);
    right = binexpr(OpPrec[tokentype]);    
    left = mkastnode(arithop(tokentype), left, right, 0);
    tokentype = Token.token;
    if (tokentype == T_SEMI)
      return (left);
  }

  return (left);
}
