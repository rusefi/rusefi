

rusEfi firmware development model is "stable master". We do not have release branches, we only have master with 
continuous integration and *limited* automated testing.

By default, each Pull Request is expected to be complete and expected to not break existing functionality.
If a Pull Request is a work-in-progress or draft or not intended for merge yet, it should be clearly marked with "[WIP]" prefix in the title.

Pull Requests which are not complete, not marked WIP or fail gate check should be closed to reduce a chance of confusion.
It is usually helpful to discuss proposed changes prior to Pull Requests - we have Slack to collaborate and coordinate so let's use!



TODO: we have an urgent need to make gate check more reliable & expand what's included into gate check

The primary hardware platform for rusEfi is stm32f4 - as of November 2019, only stm32f4 has hardware test continuous integration.

    

See also [Functional tests CI info](jenkins/functional_test_and_build_bundle/readme.md)
