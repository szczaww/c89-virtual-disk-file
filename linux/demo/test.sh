#!/bin/bash
NORMAL_FILE="./linux/demo/normal-file.txt"
HIDDEN_FILE="./linux/demo/normal-file.txt"
LONG_FILE="./linux/demo/looooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooong-name-file.txt"

DISKFILE="./build/diskfile.bin" 
BUILT_FILE="./build/main"

SIZE=11100
SHORT_SLEEP=1
LONG_SLEEP=3


# Creation & deletion
    echo "Creating&deleting diskfile..."
    sleep $SHORT_SLEEP

        echo $(pwd)
        echo $(ls)
        $BUILT_FILE -c $DISKFILE $SIZE 
        sleep $LONG_SLEEP

        $BUILT_FILE -d $DISKFILE $SIZE
        sleep $SHORT_SLEEP
        
        echo ""
        echo "Press Enter to continue..."
        read
        clear && clear

# File operations
    echo "Adding normal files..."
    sleep $SHORT_SLEEP

        $BUILT_FILE -c $DISKFILE $SIZE            # Create disk
        sleep $SHORT_SLEEP
        
        # Normal files
        $BUILT_FILE -a   $DISKFILE $NORMAL_FILE  # Copy file
        sleep $SHORT_SLEEP
        $BUILT_FILE -ls  $DISKFILE
        $BUILT_FILE -inm $DISKFILE               # Show inode bitmap
        $BUILT_FILE -dbm $DISKFILE               # Show block bitmap
        sleep $SHORT_SLEEP

        echo ""
        echo "Press Enter to continue..."
        read
        clear && clear
    
    echo "Adding hidden files..."
    sleep $SHORT_SLEEP
        $BUILT_FILE -a   $DISKFILE $HIDDEN_FILE  # Copy file
        sleep $SHORT_SLEEP
        $BUILT_FILE -ls  $DISKFILE
        $BUILT_FILE -inm $DISKFILE               # Show inode bitmap
        $BUILT_FILE -dbm $DISKFILE               # Show block bitmap
        sleep $SHORT_SLEEP
        
        echo ""
        echo "Press Enter to continue..."
        read
        clear && clear

    echo "Listing ALL files..."
    sleep $SHORT_SLEEP

        $BUILT_FILE -lsh $DISKFILE               # List
        sleep $SHORT_SLEEP

        echo ""
        echo "Press Enter to continue..."
        read
        clear && clear

    echo "Removing file.."
    sleep $SHORT_SLEEP
        $BUILT_FILE -r $DISKFILE $NORMAL_FILE  # Copy file
        sleep $SHORT_SLEEP
        $BUILT_FILE -ls  $DISKFILE
        $BUILT_FILE -inm $DISKFILE               # Show inode bitmap
        $BUILT_FILE -dbm $DISKFILE               # Show block bitmap
        sleep $SHORT_SLEEP
        
        echo ""
        echo "Press Enter to continue..."
        read
        clear && clear

    echo "Adding file with long name (>64 bytes)"
    echo "Interpretation: File is small but the name takes up extra data blocks."
    echo "One block hole now occupied by new data."
    sleep $SHORT_SLEEP

        $BUILT_FILE -a   $DISKFILE $LONG_FILE    # Copy file
        sleep $SHORT_SLEEP
        $BUILT_FILE -ls  $DISKFILE
        $BUILT_FILE -inm $DISKFILE               # Show inode bitmap
        $BUILT_FILE -dbm $DISKFILE               # Show block bitmap
        sleep $SHORT_SLEEP
        
        echo ""
        echo "Press Enter to continue..."
        read
        clear && clear

# Saving disk to file
    echo "Removing old & creating new diskfile..."
    sleep $SHORT_SLEEP

        $BUILT_FILE -d $DISKFILE $SIZE
        $BUILT_FILE -c $DISKFILE $SIZE 
        sleep $SHORT_SLEEP

        echo ""
        echo "Press Enter to continue..."
        read
        clear && clear
    
    echo "Adding file..."
    sleep $SHORT_SLEEP

        $BUILT_FILE -a $DISKFILE $NORMAL_FILE
        $BUILT_FILE -lsh $DISKFILE               # List
        sleep $SHORT_SLEEP
        
        echo ""
        echo "Press Enter to continue..."
        read
        clear && clear
    
    echo "File extracting test"
    sleep $SHORT_SLEEP

        echo ""
        echo "See normal_file.txt content by hand."
        read

        echo ""
        echo "Command run: rm normal_file.txt"
        rm normal_file.txt
        read

        $BUILT_FILE -e $DISKFILE $NORMAL_FILE
        sleep $SHORT_SLEEP

        echo ""
        echo "Press Enter to continue..."
        read
        clear && clear

echo "Tests finished."
$BUILT_FILE -d $DISKFILE
sleep $SHORT_SLEEP


