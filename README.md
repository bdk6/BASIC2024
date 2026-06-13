# BASIC2024
A new BASIC interpreter and compiler

"Improved" BASIC, modeled somewhat on MS BASIC

Data Types:
boolean         (result only)  subset of int
integer            a%          64 (32) bits
float              a[! | #]    double (single) precision
string             a$          size up to max int size
arrays of all      a%(i,j,k)   up to n(?) dimensions

type casting:
bool -> int -> float
|->     |->    |->   string


Literals:
Int:
    d{d}           decimal
    $h{h}          hex
    ?b{b}          binary
    0xh{h}         hex
    0b{b}          binary
    'x'            char

Float:
    .d{d}[e[+-]d{d}
    d{d}.{d}[e[+-]{d}]
    d{d}


Operators:
            INT   FLT   STR
  NOT
  ~          x
  +          x     x     x
  -          x     x
  *          x     x
  /          x     x
  MOD        x     x
  **       x     x
  <<
  >>
  =          x     x     x
  <          x     x     x
  >          x     x     x
  <=         x     x     x
  >=         x     x     x
  <>         x     x     x
  &          x
  |          x
  ^          x
  AND
  OR
  NOT

precedence
NOT ! ~ 
**
* / mod
+ -
<< >>
< <= <> > >= =
& 
^
|
AND
OR

LTR   := a-z | A-Z
DGIT  := 0-9
XDGT  "= DGT | a-f | A-F
IDChar := LTR | DGT | "_"
Ident := LTR {IDChar}[ "$" | "%" | "!" | "#" ]

exp     := 

term    := factor { (+|-) factor }.
power   :=
factor  := number | var | string | fncall | "(" exp ")".


  
  

Program := STLine {STLine}
STLine  := [LineNum | Label] Statement {: Statement}
LineID    := LineNum | Label
LineNum := Digit { Digit}
Label   := ident":"

Statement := REMST | LETST | IFST | ...
DATAst    :=  "DATA" valuelist
DIMst     :=  "DIM" 
ELSEst    :=  "ELSE" [
ENDst     :=
ENDIFst   :=  "ENDIF"
FORst     :=  "FOR" var "=" exp "TO" exp [ "STEP" exp ]
GLOBALst  :=  "GLOBAL"  varlist
GOSUBst   :=  "GOSUB" LineID
GOTOst    :=  "GOTO" LineID
IFst      :=  "IF" exp [ "THEN" statement [ "ELSE" statement]
INPUTst   :=  "INPUT" [string] varlist
LETst     :=  ["LET"] var "=" exp
LISTst    :=
NEXTst    :=  "NEXT"  [var]
PRINTst   :=  "PRINT" printlist
RANDOMIZE_st :- "RANDOMIZE" ...
READst    :=  "READ"  varlist
REMst     :=  "REM" {char} newline
RESTOREst :=  "RESTORE" 
RETURNst  :=  "RETURN" [exp]
STOPst    :=
THENst    :=
WENDst    :=  "WEND"
WHILEst   :=  "WHILE" exp 


Functions:
RND()
ASC()
STR$()
CHR$()

