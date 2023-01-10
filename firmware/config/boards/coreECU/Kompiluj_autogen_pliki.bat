@echo off

cd ../../../
call gen_config.bat
call gen_config_board.bat
call gen_config_common.sh
call gen_config_default.sh
call gen_default_everything.sh
call gen_enum_to_string.bat
call gen_live_documentation.bat
call gen_output_channels.sh
call gen_ptrace_enums.bat
call gen_signature.sh
call generate_docs.bat





