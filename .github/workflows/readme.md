Here is a diagram of which configure scripts are used for which workflows.
This is not a complete diagram of all workflows, nor does it show everything that these jobs do.

For up-to-date information check the GitHib workflow actions (GHA) definition files *.yaml

The workflows are triggered by events: push & pull_request

```mermaid
flowchart TD
    gha_firmware["Firmware"]
    firmware_build[["build-firmware.yaml"]]
    gha_Console["Console"]
    Console_build[["build-FOME-console.yaml"]]
    gha_Simulator["Simulator"]
    Simulator_build[["build-simulator.yaml"]]
    gha_generate_ECU_configs["Generate configs for all supported ECUs"]
    generate_ECU_configs[["gen-configs.yaml"]]
    gha_UnitTests["Unit tests"]
    UnitTests_build[["build-unit-tests.yaml"]]
    gha_Hardware_CI["Hardware Continous Integration"]
    Hardware_CI[["hardware-ci.yaml"]]
   

    gha_firmware --> firmware_build
    gha_generate_ECU_configs ---> generate_ECU_configs
    gha_Console ---> Console_build
    gha_Simulator ---> Simulator_build
    gha_UnitTests ---> UnitTests_build
    gha_Hardware_CI ---> Hardware_CI

    
    
```
