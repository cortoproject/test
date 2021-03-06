/* This is a managed file. Do not delete this comment. */

#include <corto.test>
ut_tls test_suiteKey;

void test_erase(void) {
    int i;
    for (i = 0; i < 255; i++) {
        fprintf(stderr, "\b");
    }
}

#define FIND(p, i) corto(CORTO_LOOKUP, {.parent=p, .id=i})

corto_string test_id(corto_id buffer, corto_object testcase) {
    corto_string result = NULL;
    corto_object testroot = FIND(root_o, "test");
    ut_assert(testroot != NULL, "testroot disappeared?");
    result = corto_path(buffer, testroot, testcase, "/");
    *(char*)strchr(result, '(') = '\0';
    corto_release(testroot);
    return result;
}

corto_string test_command(corto_id buffer, corto_string lib, corto_object testcase) {
    corto_id testcaseId, library;
    test_id(testcaseId, testcase);
    sprintf(library, "%s/%s", ut_cwd(), lib);
    ut_path_clean(library, library);
    sprintf(buffer, "corto -l %s %s", library, testcaseId);
    return buffer;
}

void test_assert(
    bool condition,
    const char *str_condition,
    int32_t __line)
{
    test_SuiteData this = ut_tls_get(test_suiteKey);
    if (!this) {
        ut_error("test.fail called but no testsuite is running!");
        abort();
    }

    this->assertCount++;
    if (!condition) {
        char *assertMsg = ut_asprintf("%d: assert(%s)", __line, str_condition);
        test_fail(assertMsg);
        corto_dealloc(assertMsg);
    }
}

void test_assertEqual(
    corto_any a,
    corto_any b,
    const char *str_a,
    const char *str_b,
    int32_t __line)
{
    corto_equalityKind eq;
    char *assertMsg = NULL;
    test_SuiteData this = ut_tls_get(test_suiteKey);
    if (!this) {
        ut_error("test.fail called but no testsuite is running!");
        abort();
    }

    this->assertCount++;
    eq = corto_ptr_compare(a.value, a.type, b.value);
    if (eq != CORTO_EQ) {
        assertMsg = ut_asprintf("%d: assert(%s == %s)", __line, str_a, str_b);
        test_fail(assertMsg);
        corto_dealloc(assertMsg);
    }
}

void test_assertflt(
    double f1,
    double f2,
    const char *str_f1,
    const char *str_f2,
    int32_t __line)
{
    char *assertMsg = NULL;
    test_SuiteData this = ut_tls_get(test_suiteKey);
    if (!this) {
        ut_error("test.fail called but no testsuite is running!");
        abort();
    }

    this->assertCount++;
    if (f1 != f2) {
        char *si1, *si2;
        if (isdigit(*str_f1) || (*str_f1 == '-')) {
            si1 = strdup(str_f1);
        } else {
            si1 = ut_asprintf("%s (%f)", str_f1, f1);
        }

        if (isdigit(*str_f2) || (*str_f2 == '-')) {
            si2 = strdup(str_f2);
        } else {
            si2 = ut_asprintf("%s (%f)", str_f2, f2);
        }

        assertMsg = ut_asprintf("%d: %s != %s", __line, si1, si2);
        test_fail(assertMsg);
        corto_dealloc(assertMsg);
        corto_dealloc(si1);
        corto_dealloc(si2);
    }
}

void test_assertint(
    uint64_t i1,
    uint64_t i2,
    const char *str_i1,
    const char *str_i2,
    int32_t __line)
{
    char *assertMsg = NULL;
    test_SuiteData this = ut_tls_get(test_suiteKey);
    if (!this) {
        ut_error("test.fail called but no testsuite is running!");
        abort();
    }

    this->assertCount++;
    if (i1 != i2) {
        char *si1, *si2;
        if (isdigit(*str_i1) || (*str_i1 == '-')) {
            si1 = strdup(str_i1);
        } else {
            si1 = ut_asprintf("%s (%lld)", str_i1, i1);
        }

        if (isdigit(*str_i2) || (*str_i2 == '-')) {
            si2 = strdup(str_i2);
        } else {
            si2 = ut_asprintf("%s (%lld)", str_i2, i2);
        }

        assertMsg = ut_asprintf("%d: %s != %s", __line, si1, si2);
        test_fail(assertMsg);
        corto_dealloc(assertMsg);
        corto_dealloc(si1);
        corto_dealloc(si2);
    }
}

void test_assertstr(
    const char *s1,
    const char *s2,
    const char *str_s1,
    const char *str_s2,
    int32_t __line)
{
    char *assertMsg = NULL;
    test_SuiteData this = ut_tls_get(test_suiteKey);
    if (!this) {
        ut_error("test.fail called but no testsuite is running!");
        abort();
    }

    this->assertCount++;
    if (s1 || s2) {
        if ((!s1 && s2) || (s1 && !s2) || strcmp(s1, s2)) {
            if ((s1 && strchr(s1, '\n')) || (s2 && strchr(s2, '\n'))) {
                assertMsg = ut_asprintf("%d: \n%s:\n%s\n%s:\n%s\n", __line, str_s1, s1, str_s2, s2);
            } else {
                assertMsg = ut_asprintf("%d: %s (\"%s\") != %s (\"%s\")", __line, str_s1, s1, str_s2, s2);
            }

            test_fail(assertMsg);
            corto_dealloc(assertMsg);
        }
    }
}

void test_assertref(
    corto_object o1,
    corto_object o2,
    const char *str_o1,
    const char *str_o2,
    int32_t __line)
{
    char *assertMsg = NULL;
    test_SuiteData this = ut_tls_get(test_suiteKey);
    if (!this) {
        ut_error("test.fail called but no testsuite is running!");
        abort();
    }

    this->assertCount++;
    if (o1 != o2) {
        assertMsg = ut_asprintf("%d: %s (%s) != %s (%s)",
            __line,
            str_o1,
            corto_fullpath(NULL, o1),
            str_o2,
            corto_fullpath(NULL, o2));
        test_fail(assertMsg);
        corto_dealloc(assertMsg);
    }
}


void test_empty(void)
{
    test_SuiteData this = ut_tls_get(test_suiteKey);
    if (!this) {
        ut_error("test/fail called but no testsuite is running!");
        abort();
    }

    ut_log("#[yellow]EMPTY#[normal]: %s%s: missing implementation\n",
        this->tearingDown ? corto_idof(corto_parentof(this->testcase)) : "",
        this->tearingDown ? "/teardown" : test_id(NULL, this->testcase));
    /* Run teardown before exit, prevent infinite recursion if assert is called
     * in teardown. */
    if (!this->tearingDown) {
        this->tearingDown = TRUE;
        test_SuiteData_teardown(this);
    }

    exit(1);
}

void test_fail(
    const char *err)
{
    int i;
    test_SuiteData this = ut_tls_get(test_suiteKey);
    if (!this) {
        ut_error("test.fail called but no testsuite is running!");
        abort();
    }

    for (i = 0; i < 255; i++) {
        fprintf(stderr, "\b");
    }

    for (i = 0; i < 80; i++) {
        fprintf(stderr, " ");
    }

    for (i = 0; i < 80; i++) {
        fprintf(stderr, "\b");
    }

    ut_raise();
    ut_log("#[red]FAIL#[normal]: %s%s:%s\n",
        this->tearingDown ? corto_idof(corto_parentof(this->testcase)) : "",
        this->tearingDown ? "/teardown" : test_id(NULL, this->testcase),
        err ? err : "");
    /* Run teardown before exit, prevent infinite recursion if assert is called
     * in teardown. */
    if (!this->tearingDown) {
        this->tearingDown = TRUE;
        test_SuiteData_teardown(this);
    }

    exit(-1);
}

bool test_runslow(void)
{
    char *runslow = getenv("CORTO_TEST_RUNSLOW");
    if (runslow && !strcmp("CORTO_TEST_RUNSLOW", "TRUE")) {
        return TRUE;
    } else {
        return FALSE;
    }
}

void test_setTimeout(
    corto_time *t)
{
    test_SuiteData this = ut_tls_get(test_suiteKey);
    if (!this) {
        ut_error("test::setTimeout called but no testsuite is running!");
        abort();
    }

    corto_lock(this);
    this->timeout = *t;
    /* When running in CI, give tests extra time */
    if (ut_getenv("CI")) {
        t->sec *= 6;
        t->nanosec *= 6;
    }

    corto_unlock(this);
}

int cortomain(int argc, char *argv[]) {
    CORTO_UNUSED(argc);
    CORTO_UNUSED(argv);
    if (ut_tls_new(&test_suiteKey, NULL)) {
        return -1;
    }

    return 0;
}

void test_thread_init_suite(
    test_SuiteData suite)
{
    ut_tls_set(test_suiteKey, suite);
}
