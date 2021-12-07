#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../headers/cal_renderer.h"
#include "../headers/datatypes.h"
#include "../headers/file_handler.h"
#include "../headers/ics_handler.h"
#include "../headers/input_handler.h"
#include "../headers/regex_handler.h"
#include "../headers/time_handler.h"

int main() {
    srand(time(NULL));

    setup_regex();

    clr_file();
    /*Courser segfault hvis over clr_file()*/
    setup_renderer();
    prn_cal();

    int run = 1;
    while (run) {
        run = prompt_user_input();
        printf("%d %d %ld\n", get_t_data(current_cal.time, t_week), get_t_data(current_cal.time, t_year), current_cal.time);
    }

    return (0);
}