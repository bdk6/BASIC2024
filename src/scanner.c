/////////////////////////////////////////////////////////////////////////////
/// scanner.c
/// Copyright (c) 2024 William R Cooke
/// Scanner for BASIC2024
/////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <malloc.h>



//char * source = NULL;
FILE * source = NULL;



uint8_t* chars = NULL;
int max_chars = 0;
int current_chars = 0;

static void init_chars(void)
{
  chars = malloc(32); // start with 32 bytes
  if (chars != NULL)
  {
    memset(chars, 0, 32);
    max_chars = 31;
    current_chars = 0;
  }

}

static int add_char(uint8_t ch)
{
  int rtn = -1;
  if (current_chars >= max_chars)
  {
    int new_max = max_chars * 2 + 1;;
    uint8_t* temp = malloc( new_max + 1); // double the size
    if (temp != NULL)
    {
      memcpy( temp, chars, max_chars + 1); // copy to new
      free(chars);
      chars = temp;
      max_chars = new_max;
    }
  }
  // TODO fix return val on error
  chars[current_chars++] = ch;
  return current_chars;
}

static void clear_chars(void)
{
  current_chars = 0;
  memset(chars, 0, max_chars + 1);
}

/*

+
-
*
/
**
\
^
&
|
**
<
>
<=
>=
<>
=
~
NOT
AND
OR
MOD
<<
>>
(
)
,
:
;

TOK_INT           = 256,  // dig{dig} | $xdig{xdig} | 0xxdig{xdig} | 0bbdig{bdig}
TOK_FLOAT,                // ( dig{dig}.{dig} | .dig{dig} ) [ (e|E) (+|-) dig{dig}] 
TOK_IDENT,                // ltr {idchar}[ % | $ | # | ! ]
TOK_

*/

typedef double bas_float;
typedef int64_t bas_int;

#define isletter(c) (isalpha(c) || (c) == '_') 
#define isletternumber(c) (isletter(c) || isdigit(c))


typedef enum token
{
  TOK_UNDEF          = 0,

  TOK_EXCLAM         = '!',
  TOK_DQUOTE         = '"',
  TOK_DOLLAR         = '$',
  TOK_PERCENT        = '%',

  TOK_AMP            = '&',
  TOK_SQUOTE         = '\'',

  TOK_LPAR           = '(',
  TOK_RPAR           = ')',
  TOK_STAR           = '*',
  TOK_PLUS           = '+',
  TOK_COMMA          = ',',
  TOK_MINUS          = '-',
  TOK_PERIOD         = '.',
  TOK_SLASH          = '/',
  TOK_COLON          = ':',
  TOK_SEMICOLON      = ';',
  TOK_LESS           = '<',
  TOK_EQ             = '=',
  TOK_GREAT          = '>',
  TOK_QUESTION       = '?',
  TOK_AT             = '@',
  TOK_LBRACKET       = '[',
  TOK_BACKSLASH      = '\\',
  TOK_RBRACKET       = ']',
  TOK_CARET          = '^',
  TOK_UNDERSCORE     = '_',
  TOK_BACKTICK       = '`',
  TOK_LBRACE         = '{',
  TOK_PIPE           = '|',
  TOK_RBRACE         = '}',
  TOK_TILDE          = '~',


  TOK_LESSEQ         = 0x80,
  TOK_NOTEQ,
  TOK_SHLEFT,
  TOK_GRTEQ,
  TOK_SHRIGHT,
  TOK_POWER,         // **

  // ...
  TOK_EOL,
  TOK_EOF,           // end of file
  TOK_LINENUM,       // line number
  TOK_LABEL,
  TOK_IDENT,
  TOK_IDENT_INT,     // letter{alnum}%
  TOK_IDENT_SNG,     // letter{alnum}!
  TOK_IDENT_DBL,     // letter{alnum}#
  TOK_IDENT_STR,     // letter{alnum}$
  TOK_INT,
  TOK_FLOAT,
  TOK_STRING,

  // keywords
  TOK_ASC,
  TOK_CHRSTR,
  TOK_DATA,
  TOK_DIM,
  TOK_ELSE,
  TOK_END,
  TOK_ENDIF,
  TOK_FOR,
  TOK_GLOBAL,
  TOK_GOSUB,
  TOK_GOTO,
  TOK_IF,
  TOK_INPUT,
  TOK_LEFTSTR,
  TOK_LEN,
  TOK_LET,
  TOK_LIST,
  TOK_MIDSTR,
  TOK_NEXT,
  TOK_PERMANENT,
  TOK_PRINT,
  TOK_RANDOMIZE,
  TOK_READ,
  TOK_REM,
  TOK_REPEAT,
  TOK_RESTORE,
  TOK_RETURN,
  TOK_RIGHTSTR,
  TOK_RND,
  TOK_STEP,
  TOK_STOP,
  TOK_STRSTR,
  TOK_THEN,
  TOK_UNTIL,
  TOK_WEND,
  TOK_WHILE,
  ///  Find last number
  TOK_MAX
} token_t;

typedef struct keyword
{
  token_t value;
  char*   name;
} keyword_t;

keyword_t keywords[] = 
{
  {TOK_ASC,       "ASC"},
  {TOK_CHRSTR,    "chr$"},
  {TOK_DATA,      "data"},
  {TOK_DIM,       "dim"},
  {TOK_ELSE,      "else"},
  {TOK_END,       "end"},
  {TOK_ENDIF,     "endif"},
  {TOK_FOR,       "for"},
  {TOK_GLOBAL,    "global"},
  {TOK_GOSUB,     "gosub"},
  {TOK_GOTO,      "goto"},
  {TOK_IF,        "if"},
  {TOK_INPUT,     "input"},
  {TOK_LEFTSTR,   "left$"},
  {TOK_LEN,       "len"},
  {TOK_LET,       "let"},
  {TOK_LIST,      "list"},
  {TOK_MIDSTR,    "mid$"},
  {TOK_NEXT,      "next"},
  {TOK_PERMANENT, "permanent"},
  {TOK_PRINT,     "print"},
  {TOK_RANDOMIZE, "randomize"},
  {TOK_READ,      "read"},
  {TOK_REM,       "rem"},
  {TOK_REPEAT,    "repeat"},
  {TOK_RESTORE,   "restore"},
  {TOK_RETURN,    "return"},
  {TOK_RIGHTSTR,  "right$"},
  {TOK_RND,       "rnd"},
  {TOK_STEP,      "step"},
  {TOK_STOP,      "stop"},
  {TOK_STRSTR,    "str$"},
  {TOK_THEN,      "then"},
  {TOK_UNTIL,     "until"},
  {TOK_WEND,      "wend"},
  {TOK_WHILE,     "while"},

  // ///  Find last number
  // TOK_MAX
  
};


/* Keywords from GW BASIC ver. 3.23?  https://hwiegman.home.xs4all.nl/gw-man/index.html#google_vignette
lines max 255 chars
0-65529 line numbers
system returns to os
lowercase allowed -> converted to upper
&H &O hex and octal
E / ! single ,  D / # / >= 8 digitsdouble
var nmmes up to 40 chars signifcant and letters, digits, dec pt
$ string
% integer
! single
# double
DEFINT DEFSTR DEFSNG DEFDBL
DEF FN
up to 255 dimensions of arrays
float -> int rounds
^ power
\ int division
mod modulus division: float rounded to int, then division, then remainder
= <> < > <= >= 
AND OR NOT XOR EQV (XNOR)

ABS fn   Absolute value of numeric expression
ASC fn   ASCII value of first char of string
ATN fn   Arctangent of x in radians (-pi/2 to pi/2)
AUTO cm  [line number][,[increment]]
BEEP st  800 hz for 1/4 sec
BLOAD cm BLOAD filename[,offset]  load an image file anywhere in user memory
BSAVE cm filename, offset, length save memory
CALL st  CLL numvar[(variables)] call machine language subroutine
CDBL fn convert x to double precison number
CHAIN st [MERGE] filename[,[line][,[ALL][,DELETE range]]]
CHDIR cm pathname
CHR$ fn  return string of single char for value x
CINT fn  round number to next int
CIRCLE st (xcenter, ycenter), radius[,[color][,[start],[end][,aspect]]]
CLEAR cm set all numeric vars to zero, all strings to null, close all files
CLOSE st [[#] filenumber[,[#]filenumer]...]
CLS st [n] clear screen: n = 0:clears all text n graphics, 1: only graphics viewport, 2 only text window
COLOR st [foreground][,[bacground][,border]]
         [bacground],[palette]]
         [foreground][,[background]]
COM(n) st COM(n) ON or OFF or STOP enable or disable of trapping com activity
COMMON st variables  pass variables to chained program -- 100 COMMON A, B, C, D(), G$
CONT cm resume exectuon after break
COS fn cosine of radians x
CSNG st (x) convert number to single precision
CSRLIN var return current line of cursor (1 to 25)
CVI CVS CVD fn (2,4,8 byte string) convert string to numbers

DATA stconstants stores numeric and string constants to be read by READ statements
DATE$ st = v$, v$ = DATE$  v$: mm-dd-yy mm/dd/yy mm-dd-yyyy mm/dd/yyyy returns mm-dd-yyyy
DEF FN st : DEF FNname[arguments] expression
DEFINT /SNG/DBL/STR st letters A, B, H, I-M
DEF SEG [= address] assign segment address to be used by bload, bsave, call, peek, poke, usr
DEF USR st [n]=integer specify start address of an assy sub to be called by USR fn, n=0 to 9
DELETE deletes line number(s)
DIM st var(subscripts)[,var(subscripts)] -- zeros array
DRAW st string expression

EDIT cm line number or .
END st terminate program, close files, return to command level
ENVIRON st string allows altering environment variables
ENVIRON$ fn (parmid) (nthparm)  Retrieves specified string from environment
EOF fn (file number) returns -1 if end of file reached, 0 otherwise
ERASE st list of array variables:  ERASE A,B,C
ERDEV ERDEV$ va Returns value or name of device causing error
ERL ERR va return error code or line number of an error
ERROR st integer expression Simulate an error (0 to 255)
EXP fn (x) Return e to power of x
EXTERR fn (n) return extended error information

FIELD st [#] filenum, width, AS stringvar [,width AS stringvar] ...  Allocate space for random file var
FILES cm [pathname] print names of files in directory
FIX fn (x) Truncate x to whole number
FOR st var=x TO y [STEP z]
FRE fn (x%) or (x) return number of available bytes in allocated string memory (x$ and x are dummy args)

GET st [#]file number[,record number]  read record from random disk file into randome buffer
GET st (graphics) (x1,y1)-(x2,y2), array name  trans image from screen
GOSUB st linue number
GOTO st line number branch to line number

HEX$ fn(x) return string representing hex value of x 32 -> "20"

IF st exp[,] THEN statements | GOTO line number [,] [ ELSE statements | GOTO linenumber]
INKEY$ var return one char read from keyboard
INP fn (n) return byte from port n
INPUT st [;] [prompt string;] list of variables   | [prompt string,] list of variables
INPUT# st file number, variable list read items from seq file
INPUT$ fn (x[,[#]file number)]return string of x characters from keyboard or file
INSTR fn ([n,] x$,y$)  search for first occurrence of y$ in x$, return posn (0)
INT fn (x) truncate to whole number (next lowest negative (floor)
IOCTL st [#]file number, string send control data string to char dev
IOCTL$ fn ([#]file number) read control data string from device

KEY st key number, string allow rapid entry of up to 15 characters (define macro)
KEY(n) st initiate and terminate key capture KEY(n) ON KEY(n) OFF KEY(n) STOP
KILL cm filename delete file from disk


LEFT$ fn (x$, n) returns left most n characters of x$
LEN fn (x$) returns length of string x$
LET st variable = expr
LINE st [(x1,y1)] - (x2,y2) [,[attribute][,B[F]][,style]]  draw graphic line
LINE INPUT st [;][prompt string;] string var
LINE INPUT# st file number, string variable read entire line without delim from seq file
LIST cm [line number][-line number] [,filename]
LLIST cm [line number][-line number] (always returns to command line)
LOAD cm filename[,r] load file from disk [and run]
LOC (fn) (file number) return current position in file
LOCATE st [row][,[col][,[cursor][,[start][,stop]]]]  move cursor on screen
LOCK st [#]n [,[record number][TO record number]]
LOF fn (file number) returns # bytes allocaed to file
LOG fn (x) Natural log of x
LPOS fn (x) current posn of line printer head within line printer buffer: dummy x
LPRINT [list of expressions[[;]
LSET st string var = string expr Move data from memory to random file buffer left justify

MERGE cm filename merge lines from an ascii prog file into current prog
MID$ fn (x$, n[,m])  Return m chars starting at n
MID$ st (stringexp1, n[,m]) = stringexp2 replace portion of stringexp1
MKDIR cm pathname Create subdirectory
MKD$ fn (double precision expr) convert double to string
MKI$ fn (integer expression) convert int to string
MKS$ fn (single expression) convert single to string










*/

int64_t tok_int      = 0;
double tok_float     = 0.0;
char* tok_string = NULL;

static int last_char = 0;
static int use_last = 0;

int open_file(char* filename)
{
  int rtn = 0;
  // close any previous file
  if (source != NULL)
  {
    fclose(source);
    source = NULL;
  }
  
  source = fopen(filename, "r");
  last_char = 0;
  use_last = 0;
  if(source == NULL)
  {
    rtn = -1;
  }


  return rtn;
}


static int next_char(void)
{
  int rtn = 0;
  if(use_last != 0)
  {
    rtn = last_char;
    use_last = 0;
  }
  else
  {
    rtn = last_char = getc(source);
  }
  return rtn;
}

static void unget(void)
{
  use_last = 1;
}


token_t scan(void)
{
  token_t rtn = TOK_UNDEF;
  int16_t ch = next_char();

  // skip whitespace
  while(ch == ' ' || ch == '\t')
  {
    ch = next_char();
  }

  if(isletter(ch))
  {
    // ident
    do
    {
      add_char(ch);
      ch = next_char();

    } while (isletternumber(ch));
    
  }
  else if(isdigit(ch) || ch == '$' || ch == '.')
  {
    // number
    int decimals = 0;
    // boolean 
  }
  // <, <=, <>, <<
  else if(ch == '<')
  {
    rtn = TOK_LESS;
    ch = next_char();
    if(ch == '=') rtn = TOK_LESSEQ;
    else if (ch == '>') rtn = TOK_NOTEQ;
    else if (ch == '<') rtn = TOK_SHLEFT;
    else unget();
  }
  // >, >=, >>
  else if(ch == '>')
  {
    rtn = TOK_GREAT;
    ch = next_char();
    if (ch == '=' )rtn = TOK_GRTEQ;
    else if (ch == '>') rtn = TOK_SHRIGHT;
    else unget();

  }
  // * **
  else if (ch == '*')
  {
    rtn = TOK_STAR;
    ch = next_char();
    if (ch == '*') rtn = TOK_POWER;
    else unget();

  }

  else if (ch == EOF)
  {
    rtn = TOK_EOF;
  }

  // all single chars
  else rtn = (token_t) ch;




  



  return rtn;
}

/////////////////////////////////////////////////////////////////////////////
///  @fn tokenize
///  @brief Converts source input line to tokens.
///  @param[in] src Pointer to the source input line.
///  @param[out] dest Pointer to the output token buffer.
///  @return Number of bytes used by tokenized line.
/////////////////////////////////////////////////////////////////////////////
int tokenize(uint8_t* src, uint8_t* dest, int available)
{
  int rtn = 0;
  token_t this_tok = TOK_UNDEF;

  do
  {
    /* code */
  } while (this_tok != TOK_EOL);
  


  return rtn;
}


