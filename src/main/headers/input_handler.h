#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>

void show_help(void);
void pull_out_values(char user_input[], char *str, int index);
void prompt_user_intput(regex_t *regexs);

#endif