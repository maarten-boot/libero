/*===========================================================================*
 *                                                                           *
 *  calcul.h    Prototypes for expression calculation function               *
 *                                                                           *
 *  Written:    91/05/06    Pieter Hintjens                                  *
 *  Revised:    95/03/10                                                     *
 *                                                                           *
 *===========================================================================*/

#define INVALID_TOKEN       1           /*  calculate() feedbacks            */
#define TOKEN_EXPECTED      2
#define LEFT_PAR_EXPECTED   3
#define RIGHT_PAR_EXPECTED  4
#define QUOTES_EXPECTED     5
#define OPERAND_OVERFLOW    6
#define OPERATOR_OVERFLOW   7
#define NUMBER_EXPECTED     8
#define STRING_EXPECTED     9
#define UNKNOWN_FUNCTION    10

typedef struct {                        /*  Def. of a calculate function     */
    union {                             /*    Union of following:            */
        double (*i) (int param);        /*      function takes int           */
        double (*l) (long param);       /*      function takes long          */
        double (*d) (double param);     /*      function takes double        */
        double (*s) (char *param);      /*      function takes string        */
        } fct;
    char name [10];                     /*    Name - up to 9 characters      */
    char type;                          /*    Type: i, l, s, or d            */
    } calc_fct;

typedef struct {                        /*  Parameter block for calculate:   */
    char     *expression;               /*    expression to parse            */
    double    result;                   /*    result of expression           */
    int       error_posn;               /*    offset of error in expression  */
    calc_fct *flist;                    /*    pointer to array of calc_fct   */
    } calc_control;                     /*    - ends in NULL pointer         */

int calculate (calc_control *control);
