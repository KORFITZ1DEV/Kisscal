#include "../../headers/file_handler/get_cal_suite.h"

#include "../../../main/sources/file_handler.c"
#include "../../headers/CuTest.h"
#include "../../headers/file_handler/compare_helper.h"

void get_cal_test_week_45(CuTest* tc) {
    clr_file();

    calendar actual = get_cal(time(NULL));

    calendar expected = {.time = time(NULL)};
    int dow, hod;
    for (dow = 0; dow < DAYS_IN_WEEK; dow++) {
        expected.days[dow].time = time(NULL);

        for (hod = 0; hod < HOURS_IN_DAY; hod++) {
            expected.days[dow].events[hod].title[0] = '\0';
            expected.days[dow].events[hod].start_time = -1;
            expected.days[dow].events[hod].end_time = -1;
            expected.days[dow].events[hod].valid = 0;
        }
    }

    CuAssertTrue(tc, cmp_cals(actual, expected));
}

CuSuite* get_suite_get_cal_test() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, get_cal_test_week_45);
    return suite;
}