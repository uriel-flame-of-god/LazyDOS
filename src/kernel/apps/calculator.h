/* calculator.h  –  LazyDOS “it-calculates-sometimes” evaluator */
#ifndef CALCULATOR_H
#define CALCULATOR_H

void calculator_run(void);          /* interactive mode */
int  calc_is_command(const char *s);/* recognises "calc" or "calculator" */

#endif