#!/bin/bash
NORMAL_FILE="normal_file.txt"
HIDDEN_FILE=".hidden_file.txt"
LONG_FILE="01234567890123456789012345678901234567890123456789012345678901234567890123456789.txt"
DISKFILE="diskfile.bin" 
SIZE=11100
SHORT_SLEEP=1
LONG_SLEEP=4

# Creation & deletion
    echo "Creating&deleting diskfile..."
    sleep $SHORT_SLEEP

        ./main -c $DISKFILE $SIZE 
        sleep $LONG_SLEEP

        ./main -d $DISKFILE $SIZE
        sleep $SHORT_SLEEP
        
        echo ""
        echo "Press Enter to continue..."
        read
        clear && clear

# File operations
    echo "Adding normal files..."
    sleep $SHORT_SLEEP

        ./main -c $DISKFILE $SIZE            # Create disk
        sleep $SHORT_SLEEP
        
        # Normal files
        ./main -a   $DISKFILE $NORMAL_FILE  # Copy file
        sleep $SHORT_SLEEP
        ./main -ls  $DISKFILE
        ./main -inm $DISKFILE               # Show inode bitmap
        ./main -dbm $DISKFILE               # Show block bitmap
        sleep $SHORT_SLEEP

        echo ""
        echo "Press Enter to continue..."
        read
        clear && clear
    
#     echo "Adding hidden files..."
#     sleep $SHORT_SLEEP
#         ./main -a   $DISKFILE $HIDDEN_FILE  # Copy file
#         sleep $SHORT_SLEEP
#         ./main -ls  $DISKFILE
#         ./main -inm $DISKFILE               # Show inode bitmap
#         ./main -dbm $DISKFILE               # Show block bitmap
#         sleep $SHORT_SLEEP
        
#         echo ""
#         echo "Press Enter to continue..."
#         read
#         clear && clear

#     echo "Listing ALL files..."
#     sleep $SHORT_SLEEP

#         ./main -lsh $DISKFILE               # List
#         sleep $SHORT_SLEEP

#         echo ""
#         echo "Press Enter to continue..."
#         read
#         clear && clear

#     echo "Removing file.."
#     sleep $SHORT_SLEEP
#         ./main -r $DISKFILE $NORMAL_FILE  # Copy file
#         sleep $SHORT_SLEEP
#         ./main -ls  $DISKFILE
#         ./main -inm $DISKFILE               # Show inode bitmap
#         ./main -dbm $DISKFILE               # Show block bitmap
#         sleep $SHORT_SLEEP
        
#         echo ""
#         echo "Press Enter to continue..."
#         read
#         clear && clear

#     echo "Adding file with long name (>64 bytes)"
#     echo "Interpretation: File is small but the name takes up extra data blocks."
#     echo "One block hole now occupied by new data."
#     sleep $SHORT_SLEEP

#         ./main -a   $DISKFILE $LONG_FILE    # Copy file
#         sleep $SHORT_SLEEP
#         ./main -ls  $DISKFILE
#         ./main -inm $DISKFILE               # Show inode bitmap
#         ./main -dbm $DISKFILE               # Show block bitmap
#         sleep $SHORT_SLEEP
        
#         echo ""
#         echo "Press Enter to continue..."
#         read
#         clear && clear

# # Saving disk to file
#     echo "Removing old & creating new diskfile..."
#     sleep $SHORT_SLEEP

#         ./main -d $DISKFILE $SIZE
#         ./main -c $DISKFILE $SIZE 
#         sleep $SHORT_SLEEP

#         echo ""
#         echo "Press Enter to continue..."
#         read
#         clear && clear
    
#     echo "Adding file..."
#     sleep $SHORT_SLEEP

#         ./main -a $DISKFILE $NORMAL_FILE
#         ./main -lsh $DISKFILE               # List
#         sleep $SHORT_SLEEP
        
#         echo ""
#         echo "Press Enter to continue..."
#         read
#         clear && clear
    
    echo "File extracting test"
    sleep $SHORT_SLEEP

        echo ""
        echo "See normal_file.txt content by hand."
        read

        echo ""
        echo "Command run: rm normal_file.txt"
        rm normal_file.txt
        read

        ./main -e $DISKFILE $NORMAL_FILE
        sleep $SHORT_SLEEP

        echo ""
        echo "Press Enter to continue..."
        read
        clear && clear

echo "Tests finished."
./main -d $DISKFILE
sleep $SHORT_SLEEP


