See https://wiki.rusefi.com/Dev-Quality-Control

TL, DR: just follow [tests](tests) folder as examples. [tests/nitrous_control](tests/nitrous_control) is a great starting point!

gcc/makefile/gtest

7. Run `./test.sh` to build and run all tests. You can also run `make -j$(nproc)` followed by `./build/rusefi_test`.
8. Execute tests on your PC/Mac, it's expected to say SUCCESS and not fail :) Googletest will also print results summary.
9. To run only one test use `./test.sh TEST_NAME` (which builds first) or ```build/rusefi_test --gtest_filter=*TEST_NAME*```.

In this folder we have rusEFI unit tests using https://github.com/google/googletest

Unit tests are not aware of ChibiOS or ARM or else, they are just plain C/C++ which you build for your desktop, not your MCU.

Note:
If you're using VSCode, we have launch tasks available to debug your tests!
see `.vscode/launch.json` "Debug Unit Tests (gdb)"

[Code Coverage Report](https://rusefi.com/docs/unit_tests_coverage/)

for make the coverage locally you need to install [gcovr](https://gcovr.com/en/stable/installation.html)

Build and run the coverage with `./run_coverage.sh`.
the report will be on `unit_tests/gcov_working_area/gcov/index.html`

See also [https://wiki.rusefi.com/Build-Server-and-Automation](https://wiki.rusefi.com/Build-Server-and-Automation)

# Triggers Images

[Trigger images](https://wiki.rusefi.com/All-Supported-Triggers) generation is still a two-step manual process:

Step 1: Invoke unit_tests. One of the unit_tests artifacts is triggers.txt which is a snapshot of all trigger definitions.

Step 2: Once we have triggers.txt updated by unit_tests we can invoke firmware/gen_trigger_images.bat in order
to generate actual trigger images.

### Two-step TDD

We are trying to https://en.wikipedia.org/wiki/Test-driven_development

Ideal change happens as two commits:

* first we add coverage for _current_ behaviour. In case of a defect we add *green* coverage confirming defect.
* second step would be change in production code alongside of *change* in coverage.

### Testing 101

**Basic Test Setup**
```cpp
TEST(ModuleName, TestDescription) {
    EngineTestHelper eth(engine_type_e::TEST_ENGINE);

    // Your test logic here
    ASSERT_EQ(expectedValue, actualValue);
}
```

**Mocking Sensor Inputs**
```cpp
TEST(SensorModule, TestWithMockedSensor) {
    EngineTestHelper eth;

    // Mock sensor reading
    Sensor::setMockValue(SensorType::Clt, 80.0f);

    // Test logic using mocked sensor
    ASSERT_NEAR(80.0f, Sensor::get(SensorType::Clt), 0.1f);
}
```

**Testing Time-Based Functions**
```cpp
TEST(TimeModule, TestWithTimeAdvance) {
    EngineTestHelper eth;

    // Advance virtual time
    eth.moveTimeForwardUs(1000); // Advance 1ms
    eth.moveTimeForwardMs(1); // Advance 1ms

    // Test time-dependent behavior
    // ...
}
```
**Testing with Engine Module Mocks**
```cpp
TEST(ModuleInteraction, TestWithMockedModule) {
    EngineTestHelper eth(engine_type_e::TEST_ENGINE);

    MockIgnitionController ignController;

    EXPECT_CALL(ignController, getIgnState).WillRepeatedly(Return(true));

    engine->module<IgnitionController>().set(&ignController);
    EXPECT_TRUE(engine->module<IgnitionController>()->getIgnState());
}
```
