#!/usr/bin/env bash
#####################################################
#                    compile.sh                     #
#                    02/17/2024                     #
#             Written By David Holdeman             #
#    Compile rusEFI - use '-b' to build a bundle    #
#####################################################

PS3="Select a build by entering its number: "

# This full path of the firmware directory
FDIR=$(cd "$(dirname "$0")/.."; pwd -P)

# Check for -b flag
if [ "$1" == "-b" ]; then
	B="bundle"
	shift
fi

MI="$1"

shift
MAKE_ARGS=$*

# If the user passed a meta-info file
if [ ! -z "$MI" ]; then
	# If the file exists relative to the firmware directory, do nothing
	if [ -f "$FDIR/$MI" ]; then
		true
		# If the file exists relative to our current directory, get the full path
	elif [ -f "$MI" ]; then
		MI=$(realpath "$MI")
	else
		echo "Could not find $MI"
		exit 1
	fi
else
	# If no file was passed, we let the user select
	# Get all meta-info.env files, prepend the directory they are in, sort by that directory,
	#   then remove the directory to leave only the path.
	MIS=$(find "$FDIR/config/boards" -name "meta-info*.env" | awk -F '/' '{print $(NF-1),$0}' | sort | cut -d' ' -f 2-)
	OPTS=()
	while IFS= read -r M; do
		# Get the name of the directory
		DIR=$(basename $(dirname "$M"))
		# Get the build name part of the meta-info file
		NAME=$(basename "$M" | sed -r 's/meta-info-(.*)\.env/\1/')
		# NAME will contain meta-info.env if the regex didn't match
		if [ "$NAME" == "meta-info.env" ]; then
			NAME="default"
		fi
		OPTS+=("$DIR $NAME")
	done <<< "$MIS"
fi

select OPT in "${OPTS[@]}"; do
	# REPLY is the index
	MI=$(echo "$MIS" | head -n $REPLY | tail -n 1)
	break
done

# cd to the firmware directory
cd "$FDIR"

# get the path of the meta-info file relative to the firmware directory,
#   because that's what common_script_read_meta_env.inc expects
MI=$(realpath --relative-to="$FDIR" "$MI")

source config/boards/common_script_read_meta_env.inc "$MI"
make $B -j$(nproc) -r $MAKE_ARGS
