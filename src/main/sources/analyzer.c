#include "../headers/analyzer.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../headers/datatypes.h"
#include "../headers/file_handler.h"
#include "../headers/time_handler.h"

#define HOURS_IN_WEEK HOURS_IN_DAY *DAYS_IN_WEEK
#define SECS_IN_DAY HOURS_IN_DAY *MINS_IN_HOUR *SECS_IN_MIN

static void load_free_time_arr(int *secs_free, int *secs_non_events, calendar *cal, time_t current_time);
static void prn_hour_use(time_t current_time);
static void prn_free_hour_use(time_t current_time, float left, float total);
static void prn_loading_bar(float used, float total);

void analyze(calendar *cal) {
    time_t current_time = time(NULL);
    int secs_free[DAYS_IN_WEEK], secs_non_events[DAYS_IN_WEEK];
    load_free_time_arr(secs_free, secs_non_events, cal, current_time);

    float free_hours_left_across_week = 0;
    float free_hours_across_week = 0;

    int i;
    for (i = 0; i < DAYS_IN_WEEK; i++) {
        free_hours_left_across_week += (float)secs_free[i] / SECS_IN_HOUR;
        free_hours_across_week += (float)secs_non_events[i] / SECS_IN_HOUR;
    }

    printf("\n-----------------------------------------------------------------------------------------------------------\n\n");
    char day_str[4];
    load_dow_string(day_str, get_t_data(current_time, t_dow));
    printf("It is currently %s %02d:%02d %d/%d week: %d year: %d\n",
           day_str,
           get_t_data(current_time, t_hour),
           get_t_data(current_time, t_min),
           get_t_data(current_time, t_dom),
           get_t_data(current_time, t_mon),
           get_t_data(current_time, t_week),
           get_t_data(current_time, t_year));

    prn_hour_use(current_time);
    prn_free_hour_use(current_time, free_hours_left_across_week, free_hours_across_week);

    printf("\n-----------------------------------------------------------------------------------------------------------\n");
}

static void load_free_time_arr(int *secs_free, int *secs_non_events, calendar *cal, time_t current_time) {
    time_t current_cal_time = get_cal_time_from_day_time(current_time);
    int in_week = cal->time - current_cal_time;

    int dow;
    if (in_week == 0) {
        dow = get_t_data(current_time, t_dow);
    } else if (in_week < 0) {
        dow = DAYS_IN_WEEK;
    } else {
        dow = 0;
    }

    int i, j;
    for (i = 0; i < DAYS_IN_WEEK; i++) {
        if (i < dow) {
            secs_free[i] = 0;
        } else if (in_week == 0 && i == dow) {
            int mins = get_t_data(current_time, t_min);
            mins += get_t_data(current_time, t_hour) * MINS_IN_HOUR;
            secs_free[i] = SECS_IN_DAY - mins * SECS_IN_MIN;
        } else {
            secs_free[i] = SECS_IN_DAY;
        }

        secs_non_events[i] = SECS_IN_DAY;

        for (j = 0; j < CONTENT_IN_DAY; j++) {
            event event = cal->days[i].events[j];
            if (event.valid && event.end_time > current_time) {
                if (current_time > event.start_time && current_time < event.end_time) {
                    secs_free[i] -= event.end_time - current_time;
                } else {
                    secs_free[i] -= event.end_time - event.start_time;
                }
            }
            secs_non_events[i] -= event.end_time - event.start_time;
        }
    }
}

static void prn_hour_use(time_t current_time) {
    float hours = get_t_data(current_time, t_hour);
    int mins = get_t_data(current_time, t_min);
    hours += (float)mins / MINS_IN_HOUR;
    hours += get_t_data(current_time, t_dow) * HOURS_IN_DAY;

    printf("\nYou have spent %.1f hour(s) of %d total\n%.1f hour(s) remain\n",
           hours,
           HOURS_IN_WEEK,
           HOURS_IN_WEEK - hours);
    prn_loading_bar(hours, HOURS_IN_WEEK);
}

static void prn_free_hour_use(time_t current_time, float left, float total) {
    printf("\nYou have spent %.1f free hour(s) of %.1f total\n%.1f free hour(s) remain\n",
           total - left,
           total,
           left);
    prn_loading_bar(total - left, total);

    float sleep_time_left = (DAYS_IN_WEEK - (get_t_data(current_time, t_dow) + 1)) * 8 + 4;
    printf("Assuming you sleep 8 hours a night, you really have %.1f free hour(s) left\n", left - sleep_time_left);
}

static void prn_loading_bar(float used, float total) {
    int num_tiles = 50;
    float ratio = (float)used / total;
    int used_tiles = (float)num_tiles * ratio;

    printf("[");
    int i;
    for (i = 0; i < num_tiles; i++) {
        if (i <= used_tiles) {
            printf("#");
        } else {
            printf("-");
        }
    }

    printf("] | %.0f:%.0f\n", ratio * 100, 100 - ratio * 100);
}