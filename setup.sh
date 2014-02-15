#!/bin/sh
#
# setup.sh - executed upon entering the directory.
#

OSM_DIR="/home/casperbhansen/Development/OSM"
SRC_DIR="$OSM_DIR/src"
TEST_DIR="$OSM_DIR/src/tests"
FYAMS_DIR="$OSM_DIR/fyams/bin"

if [[ $PATH != *$FYAMS_DIR* ]]
then
    eval 'export PATH=$FYAMS_DIR:$PATH'
fi

# harddisk control
UTIL="$SRC_DIR/util/tfstool"
HD="$SRC_DIR/fyams.harddisk"

hd () {
    
    if ! [ -e $UTIL ];
    then
        echo "Couldn't find TFS tool at '$UTIL'."
        return 1
    fi
    
    if [[ "$1" == "list" ]];
    then
        $UTIL list $HD
    fi
    
    if [[ "$1" == "write" ]];
    then
        if [[ "$2" != "" ]];
        then
            $UTIL write $HD $TEST_DIR/$2 $2
        else
            echo "Usage (write): $0 write <file>"
            return 2
        fi
    fi
    
    if [[ "$1" == "delete" ]];
    then
        if [[ "$2" != "" ]];
        then
            $UTIL delete $HD $2
        else
            echo "Usage (delete): $0 delete <file>"
            return 2
        fi
    fi
}

