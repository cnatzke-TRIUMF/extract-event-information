#!/bin/bash

#set -euo pipefail
#IFS=$'\n\t'

# --- HELPER FUNCTIONS --------------------------------------------------------
# Get values from a .cfg file
function iniget() {
    if [[ $# -lt 2 || ! -f $1 ]]; then
        echo "usage: iniget <file> [--list|<section> [key]]"
        return 1
    fi
    local inifile=$1

    if [ "$2" == "--list" ]; then
        for section in $(cat $inifile | grep "^\\s*\[" | sed -e "s#\[##g" | sed -e "s#\]##g"); do
            echo $section
        done
        return 0
    fi

    local section=$2
    local key
    [ $# -eq 3 ] && key=$3

    # This awk line turns ini sections => [section-name]key=value
    local lines=$(awk '/\[/{prefix=$0; next} $1{print prefix $0}' $inifile)
    lines=$(echo "$lines" | sed -e 's/[[:blank:]]*=[[:blank:]]*/=/g')
    while read -r line ; do
        if [[ "$line" = \[$section\]* ]]; then
            local keyval=$(echo "$line" | sed -e "s/^\[$section\]//")
            if [[ -z "$key" ]]; then
                echo $keyval
            else
                if [[ "$keyval" = $key=* ]]; then
                    echo $(echo $keyval | sed -e "s/^$key=//")
                fi
            fi
        fi
    done <<<"$lines"
}

# convert analysis trees into csv files
function convert_tree() {
    for file in $ANALYSIS_DIR/analysis"$1"_*.root; do
        fullname=${file##*/}
        filename=${fullname%%.*}
        subrun=${filename##*_}
        output_file=run_${1}_$subrun.csv
        if [[ ! -f $OUTPUT_DIR/$output_file ]]; then
            echo "CSV file $output_file does not exist, processing analysis tree ..."
            if [[ $3 == "true" ]]; then
                # verbosity check
                #echo "extract_events $file $2"
                #echo "tar czf $output_file.gz data.csv"
                #echo "mv $output_file.gz $OUTPUT_DIR"
                # commands
                $SORT_CODE $file $2
                tar czf $output_file.gz data.csv
                mv $output_file.gz $OUTPUT_DIR
            else
                # verbose check
                #echo "extract_events $file $2"
                #echo "mv data.csv $OUTPUT_DIR/$output_file"
                # commands
                $SORT_CODE $file $2
                mv data.csv $OUTPUT_DIR/$output_file
            fi
        elif [[ -f $OUTPUT_DIR/$output_file ]]; then
            echo "CSV file $output_file already exists, skipping ..."
        fi
    done
}


# --- MAIN FUNCTION -----------------------------------------------------------
# parse config file
TOP_DIR=$(pwd)
CFG_FILE="$TOP_DIR/convert_config.cfg"
SORT_CODE="$TOP_DIR/extract_events"
ANALYSIS_DIR=$(iniget $CFG_FILE directories analysis_dir)
OUTPUT_DIR=$(iniget $CFG_FILE directories output_dir)
CAL_FILE=$(iniget $CFG_FILE options calibration_file)
COMPRESSION=$(iniget $CFG_FILE options compression)


if [ $# -eq 0 ]; then
        echo "Hello there"
elif [ $# -eq 1 ]; then
    convert_tree $1 $CAL_FILE $COMPRESSION
elif [ $# -eq 2 ]; then
    for ((i=$1;i<=$2;i++)); do
        convert_tree $i $CAL_FILE $COMPRESSION
    done
fi
