#!/usr/bin/env bash

set -e

PROJECT_BOARD="${1}"; [ "$#" -ge 1 ] && shift

#--

log_message() {
    message="${1}"
    echo -e "\n##\n## ${message}\n##\n"
}

config_board() {
    board="${1}"

    target=
    folder=
    ini=

    case "${board}" in
        *)
            target="${board}"
            folder="${board}"
            ini="rusefi_${board}.ini"
            ;;&
        alphax-*|harley81|hellen*)
            echo "match" 
            folder="hellen/${board}"
            ;;&
        hellen88bmw*)
            ini="rusefi_hellen88bmw.ini"
            ;;
        hellenNA6)
            folder="hellen/hellen64_miataNA6_94"
            ini="rusefi_hellenNA6.ini"
            ;;
        hellen128)
            ini="rusefi_hellen128mercedes.ini"
            ;;
        frankenso*)
            folder="frankenso"
            ;;
        kinetis)
            ini="rusefi_kin.ini"
            ;;
        mre_*)
            folder="microrusefi"
            ;;&
        mre_f4*)
            ini="rusefi_mre_f4.ini"
            ;;
        mre_f7*)
            # TODO(nms): console-settings: firmware/config/boards/nucleo_f767/rusefi_console_properties.xml
            ini="rusefi_mre_f7.ini"
            ;;
        prometheus_*)
            folder="prometheus"
            ;;
        proteus_*)
            folder="proteus"
            ;;&
        proteus_f4_*)
            ini="rusefi_proteus_f4.ini"
            ;;
        proteus_f7_*|proteus_legacy)
            ini="rusefi_proteus_f7.ini"
            ;;
        stm32*_nucleo)
            # TODO(nms): skip-config
            ini="no"
            ;;&
        stm32f429_nucleo)
            folder="nucleo_f429"
            ;;
        stm32f767_nucleo)
            # TODO(nms): console-settings: firmware/config/boards/nucleo_f767/rusefi_console_properties.xml
            folder="nucleo_f767"
            ;;
        stm32h743_nucleo)
            folder="nucleo_h743"
            ;;
        subaru_eg33_f7)
            folder="subaru_eg33"
            ;;
        # TODO(nms): *-discovery seems to follow normal patterns, minus unset ini-file
        # f429-discovery)
        #     # TODO(nms): unset ini-file; same as ini=no?
        #     ini="no"
        #     ;;
    esac

    ./gen_config_board.sh "${folder}" "${target}" "${ini}"
}

# --

log_message "Generate Live Documentation"
./gen_live_documentation.sh

log_message "Generate Enum Strings"
./gen_enum_to_string.sh

log_message "Generate Default Config"
./gen_config_default.sh

#log_message "Generate PROJECT_BOARD Config"
#case "${PROJECT_BOARD}" in
#    kinetis)
#        ./config/boards/kinetis/config/gen_kinetis_config.sh
#        ;;
#    cypress)
#        ./config/boards/cypress/config/gen_cypress_config.sh
#        ;;
#    subaru_eg33_f7)
#        ./config/boards/subaru_eg33/config/gen_subaru_config.sh
#        ;;
#    *)
#        config_board "${PROJECT_BOARD}"
#        ;;
#esac

log_message "Build Firmware"
#./clean_compile_two_versions.sh "PROJECT_BOARD=${PROJECT_BOARD}" "$@"
cd ..; OS="Windows_NT" misc/jenkins/functional_test_and_build_bundle/build_current_bundle.sh
