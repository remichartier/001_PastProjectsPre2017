# sccs_id=@(#)gencode	1.5 07/27/93

cd $1
touch $2
echo "\nVERIFYING $1 SDL....."
geodeverif $2
echo "\nBUILDING SOURCE FROM $1 SDL....."
geodeb_psos $2 $3 $4 $5 $6 $7
export FIELDS_DIR=../dbfields
export AWK_SCRIPT_DIR="../ppc"
rm -f Makefile
echo "\nPOST-PROCESSING GENERATED $1 SOURCE....."
ppc
echo "\nCOPYING MAIN PROGRAM FOR $1 SOURCE....."
cp ../main/igtmain.c .
echo "\nBUILDING NEW MAKE FILE FOR $1 SOURCE....."
mkmf -F ../data/C.p PROGRAM=$1 2>&1 | grep -v '/h/'
cd -
