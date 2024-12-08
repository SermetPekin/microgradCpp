Test Selection:
  --gtest_list_tests
      List the names of all tests instead of running them. The name of
      TEST(Foo, Bar) is "Foo.Bar".
  --gtest_filter=POSITIVE_PATTERNS[-NEGATIVE_PATTERNS]
      Run only the tests whose name matches one of the positive patterns but
      none of the negative patterns. '?' matches any single character; '*'
      matches any substring; ':' separates two patterns.
  --gtest_also_run_disabled_tests
      Run all disabled tests too.

Test Execution:
  --gtest_repeat=[COUNT]
      Run the tests repeatedly; use a negative count to repeat forever.
  --gtest_shuffle
      Randomize tests' orders on every iteration.
  --gtest_random_seed=[NUMBER]
      Random number seed to use for shuffling test orders (between 1 and
      99999, or 0 to use a seed based on the current time).
  --gtest_recreate_environments_when_repeating
      Sets up and tears down the global test environment on each repeat
      of the test.

Test Output:
  --gtest_color=(yes|no|auto)
      Enable/disable colored output. The default is auto.
  --gtest_brief=1
      Only print test failures.
  --gtest_print_time=0
      Don't print the elapsed time of each test.
  --gtest_output=(json|xml)[:DIRECTORY_PATH/|:FILE_PATH]
      Generate a JSON or XML report in the given directory or with the given
      file name. FILE_PATH defaults to test_detail.xml.
  --gtest_stream_result_to=HOST:PORT
      Stream test results to the given server.

Assertion Behavior:
  --gtest_death_test_style=(fast|threadsafe)
      Set the default death test style.
  --gtest_break_on_failure
      Turn assertion failures into debugger break-points.
  --gtest_throw_on_failure
      Turn assertion failures into C++ exceptions for use by an external
      test framework.
  --gtest_catch_exceptions=0
      Do not report exceptions as test failures. Instead, allow them
      to crash the program or throw a pop-up (on Windows).