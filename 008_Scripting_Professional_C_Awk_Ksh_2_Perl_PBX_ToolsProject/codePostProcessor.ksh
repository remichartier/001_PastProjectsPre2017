#!/bin/ksh

# 

# ppc - C code generation post processor

# This script handles all postprocessing required before compilation of the
# generated code can occur. The script takes one argument, the directory
# in which the postprocessing should occur

# Ppc takes one optional parameter, the directory containing the generated
# code, which ppc must postprocess before it can be compiled. If no parameter
# is specified, then this value defaults to the current directory.

# Several environmental variables are important to ppc:

#   * FIELDS_DIR is a directory containing #defines for call processing
#     fields. During postprocessing, ppc creates a file called tables.h
#     that contains only the most important field information -- base ptr,
#     entry length, plane length, and table length. Given no value
#     it defaults to the current directory

#   * AWK_SCRIPT_DIR is the directory in which the awk scripts that ppc
#     uses to do work reside. If given no value, it defaults to the
#     directory in which ppc resides

D=${1:-.}
if [[ -z ${AWK_SCRIPT_DIR} ]] then
   export AWK_SCRIPT_DIR=`whence $0`
   export AWK_SCRIPT_DIR=${AWK_SCRIPT_DIR:-$0}
   export AWK_SCRIPT_DIR=`dirname $AWK_SCRIPT_DIR`
fi
export FIELDS_DIR=${FIELDS_DIR:-'unspecified.'}

echo -------------------------------------------------------------
echo PREPROCESSING GENERATED CODE
echo -------------------------------------------------------------
echo Target directory is $D
echo FIELDS_DIR is $FIELDS_DIR
echo AWK_SCRIPT_DIR is $AWK_SCRIPT_DIR
echo -------------------------------------------------------------

if [[ ($FIELDS_DIR = 'unspecified.') ]]
then
echo Since one or more environmental variables were unspecified, C
echo code postprocessing cannot succeed. The following list shows
echo what each environmental variable represents:
echo
echo "FIELDS_DIR      The directory containing header files for all"
echo "                call processing database fields"
echo
echo "AWK_SCRIPT_DIR  The directory that contains the awk scripts"
echo "                that ppc uses to perform code postprocessing."
echo "                If this variable is unspecified, then it"
echo "                defaults to the directory containing the ppc"
echo "                script"
echo -------------------------------------------------------------
exit
fi

#
# Remove all the user files
#
ls -1 $D/u_*.h > /dev/null 2>&1 && 
echo &&
echo Removing all user u_\*.h files && 
rm $D/u_*.h

#
# Remove all the i_*.h files
#
ls -1 $D/i_*.[ch] > /dev/null 2>&1 && 
echo &&
echo Removing all i_\*.[ch] files && 
rm $D/i_*.[ch]

#
# Remove all the dmy_call files
#
ls -1 $D/p_dmy_call* > /dev/null 2>&1 && 
echo &&
echo Removing all p_dmy_call\* files && 
rm $D/p_dmy_call*

#
# Remove the dmy_caller process from c_declar.h
#
if [[ -f $D/c_declar.h ]] then
   echo
   echo Removing references to dmy_caller in $D/c_declar.h
   cp $D/c_declar.h $D/tmp
   awk -f $AWK_SCRIPT_DIR/remove_dmy_call $D/tmp > $D/c_declar.h
   rm $D/tmp
fi

#
# Consolidate table-specific information into one output stream
#
if [[ ! -s $D/tables.h ]] then
   ls -1 $FIELDS_DIR/*.h > /dev/null 2>&1 && 
   echo &&
   echo "Consolidating table information into tables.h" &&
   echo "---------------------------------------------" &&
   for F in `ls -1 ${FIELDS_DIR}/*.h`
   do
       echo "  $F"
       awk -f $AWK_SCRIPT_DIR/isolate_tbl_defs $F >> $D/tables.h
   done
fi

#
# The awk scripts remove_gu_prefix and include_field_defs use an executable 
# called gfi. Make gfi
#
(cd $AWK_SCRIPT_DIR; make)

#
# Remove any gu_*_FIELD references in the [sbp]_*.h files
#
ls -1 $D/[sbp]_*.h > /dev/null 2>&1 && 
echo &&
echo "Removing field forward declarations" &&
echo "-----------------------------------" &&
for F in `ls -1 $D/[sbp]_*.h`
do
   echo "  $F"
   cp $F $D/tmp
   awk -f $AWK_SCRIPT_DIR/remove_gu_field_lines $D/tmp > $F
   rm $D/tmp
done

#
# Remove the gu_XXXXX prefix from any gu_XXXXX_xxxxx calls in the 
# [sbp]_*.h files 
#
ls -1 $D/[sb]_*.h > /dev/null 2>&1 && 
echo &&
echo Removing Geode-generated operator prefix &&
echo ---------------------------------------- &&
for F in `ls -1 $D/[sb]_*.h`
do
   echo "  $F"
   cp $F $D/tmp
   awk -f $AWK_SCRIPT_DIR/remove_gu_prefix $D/tmp > $F
   rm $D/tmp
done

#
# Remove the gu_XXXXX prefix from any gu_XXXXX_xxxxx calls in the p_*.c 
# files and include field definitions
#
ls -1 $D/p_*.c > /dev/null 2>&1 && 
echo &&
echo Removing Geode-generated operator prefix and &&
echo including field definitions &&
echo -------------------------------------------- &&
for F in `ls -1 $D/p_*.c`
do
   # Get the name of the current file, sans extension
   H=${F%.c}

   # Process the current .c file
   echo "  $F"
   cp $F $D/tmp
   awk -f $AWK_SCRIPT_DIR/remove_gu_prefix $D/tmp >$F 2>>$D/${H}_a
   rm $D/tmp

   sort -u $D/${H}_a > $D/${H}_b
   awk 'length > 50 { print }' $D/${H}_b >>$D/${H}.h
   awk 'length <= 50 { print }' $D/${H}_b >>$D/${H}.h
   rm $D/${H}_a
   rm $D/${H}_b
done
