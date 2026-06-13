//////////////////////////////////////////////////////////////////////////////
///  parser.c
///  Copyright 2024 William R Cooke
///  Parser for basic 2024
//////////////////////////////////////////////////////////////////////////////


int p_program(void)
{
  return 0;
}

int p_stline(void)
{
  return 0;
}

int p_lineid(void)
{
  return 0;
}

int p_linenum(void)
{
  return 0;
}

int p_label(void)
{
  return 0;
}

int p_statement(void)
{
  return 0;
}

int p_datast(void)
{
  return 0;
}

int p_dimst(void)
{
  return 0;
}

int p_elsest(void)
{
  return 0;
}

int p_endst(void)
{
  return 0;
}

int p_endifst(void)
{
  return 0;
}

int p_forst(void)
{
  return 0;
}

int p_globalst(void)
{
  return 0;
}

int p_gosubst(void)
{
  return 0;
}

int p_gotost(void)
{
  return 0;
}

int p_ifst(void)
{
  return 0;
}

int p_inputst(void)
{
  return 0;
}

int p_letst(void)
{
  return 0;
}

int p_listst(void)
{
  return 0;
}

int p_nextst(void)
{
  return 0;
}

int p_printst(void)
{
  return 0;
}

int p_readst(void)
{
  return 0;
}

int p_remst(void)
{
  return 0;
}

int p_restorest(void)
{
  return 0;
}

int p_returnst(void)
{
  return 0;
}

int p_stopst(void)
{
  return 0;
}

int p_thenst(void)
{
  return 0;
}

int p_wendst(void)
{
  return 0;
}

int p_whilest(void)
{
  return 0;
}

// Expressions
int exp_or(void)
{
  return 0;
}

int exp_xor(void)
{
  return 0;
}

// Logical AND: and
// exp := bor { "and" bor }
int exp_and(void)
{
  return 0;
}

// Bitwise OR: |
// bor := bxor { "|" bxor }
int exp_bor(void)
{
  return 0;
}

// Bitwise XOR: ^
// bxor := band { "^" band }
int exp_bxor(void)
{
  return 0;
}

// Bitwise AND: &
// band := comparison { "&" comparison }
int exp_band(void)
{
  return 0;
}

// Comparison: < <= <> = >= >
// comparison := shift [ relop shift ]
int exp_relate(void)
{
  return 0;
}

// Shift left or right: << >>
// shift := addexp [ shiftop addexp ]
int exp_shift(void)
{
  return 0;
}

// Add, subtract: + -
// addexp := term { addop term }
int exp_add(void)
{
  return 0;
}

// Multiply, divide, modulo:  * / mod
// term := factor { mulop factor }
int exp_mul(void)
{
  return 0;
}

// Raise to a power: **
int exp_pow(void)
{
  return 0;
}
