#!/usr/local/bin/perl

# 

# This script has been translated from earlier programs in ksh, awk, and C.
# which were :
#	ksh : ppc
#	awk : remove_dmy_call, isolate tbl_defs, 
#	      remove_gu_field_lines, remove_gu_prefix
#	C   : gfi.c

# By now, everything is gathered in that single program.



$FIELDS_DIR = "../../dbfields";
$AWK_SCRIPT_DIR = ".";


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

#   * AWK_SCRIPT_DIR is the directory in which the script that ppc
#     uses to do work reside. If given no value, it defaults to the
#     directory in which ppc resides



$D = '.' if $D eq ""; 
if ( $AWK_SCRIPT_DIR eq "" ){
	$AWK_SCRIPT_DIR=`ksh -c \"whence $0\"`;
	$AWK_SCRIPT_DIR=`$_` if $AWK_SCRIPT_DIR eq "";
	$AWK_SCRIPT_DIR=`dirname $AWK_SCRIPT_DIR`;
}
$FIELDS_DIR='unspecified.' if $FIELDS_DIR eq "";

print "-------------------------------------------------------------\n";
print "PREPROCESSING GENERATED CODE\n";
print "-------------------------------------------------------------\n";
print "Target directory is $D\n";
print "FIELDS_DIR is $FIELDS_DIR\n";
print "AWK_SCRIPT_DIR is $AWK_SCRIPT_DIR\n";
print "-------------------------------------------------------------\n";

if  ($FIELDS_DIR eq 'unspecified.') {
	print "Since one or more environmental variables were unspecified, C\n";
	print "code postprocessing cannot succeed. The following list shows\n";
	print "what each environmental variable represents:\n\n";
	print "FIELDS_DIR      The directory containing header files for all\n";
	print "                call processing database fields\n\n";
	print "AWK_SCRIPT_DIR  The directory that contains the awk scripts\n";
	print "                that ppc uses to perform code postprocessing.\n";
	print "                If this variable is unspecified, then it\n";
	print "                defaults to the directory containing the ppc\n";
	print "                script\n";
	print "-------------------------------------------------------------\n";
	exit();
}



# Remove all the user files
############################

if (!system "ls -l $D/u_*.h > /dev/null 2>&1"){
	print "\nRemoving all user u_\*.h files\n";
	`rm $D/u_*.h`;
}



# Remove all the i_*.h files
#############################

if (!system "ls -l $D/i_*.[ch] > /dev/null 2>&1"){
       print "\nRemoving all user i_\*.[ch] files\n";
       `rm $D/i_*.[ch]`;
}



# Remove all the dmy_call files
################################

if ( !system "ls -l $D/p_dmy_call* > /dev/null 2>&1"){
       print "\nRemoving all p_dmy_call\* files\n";
       `rm $D/p_dmy_call*`;
}



# Remove the dmy_caller process from c_declar.h
################################################

# Because table fields
# appear to SDL to be ADT operators, the C code generation process adds
# external declarations to the header files of each generated C file.

# However, table fields are actually C macros, and leaving the external
# declarations in the header files causes link errors. Therefore, this
# script strips the external declarations from the header files. This
# script should be run over all .h files (but not .c files) generated
# by the C code generator


if ( -f "$D/c_declar.h" ){	# if file exists and is a regular file
	print "\nRemoving references to dmy_caller in $D/c_declar.h\n";
	`mv $D/c_declar.h $D/temp`;	
	open(CATDECLAR,"$D/temp") || die "can't open a temporary file\n";
	open(DECLAR,">$D/c_declar.h") || die "can't create file $D/c_declar.h\n";

# If the line does not contain any reference to a user-defined FIELDS
# operator or ADT, then print it

	while(<CATDECLAR>){
			print DECLAR unless /dmy_call/ ;	
	}
	`rm $D/temp`;
	close(DECLAR);	
}



# Consolidate table-specific information into one output stream
################################################################

# This script is used during C code generation. Because table fields
# appear to SDL to be ADT operators, the C code generation process adds
# external declarations to the header files of each generated C file.

# Other scripts strip out the headers and insert the #defines for
# IML fields. However, some field-related information is not described
# as #defines but as extern variable declarations. This script isolates
# that information into one output stream so that it may be incorporated
# into the hpredefs file


if (!( -s "$D/tables.h") ){	# if file exits and has size >= 0
	open(LS,"ls $FIELDS_DIR/*.h|") || die "can't execute ls\n";
	$_ = <LS>;
	if (!/not found/){      # return of ls if file not found : "not found"  
		print "\nConsolidating table information into tables.h\n";
		print "---------------------------------------------\n";
	        open(TABLES,">$D/tables.h")|| die "can't create file $D/tables.h\n";
		while ($_){	
			open(FILE,"$FIELDS_DIR/$_")||die "can't read file $FIELDS_DIR/$_\n";
			while(<FILE>){
				if (/\#ifndef [A-Z0-9]{3}_DEF/){
					while(!/\#endif/){
						print TABLES if (!/ifndef/ &&
						!/endif/ && !/DEF/);
						$_  = <FILE>;
					}
				}
			}
			$_ = <LS>;
			close(FILE);
		}
		close(TABLES);
	}
}


# Remove any gu_*_FIELD references in the [sbp]_*.h files
##########################################################

# this script is used during C code generation. Because table fields
# appear to SDL to be ADT operators, the C code generation process adds
# external declarations to the header files of each generated C file.

# However, table fields are actually C macros, and leaving the external
# declarations in the header files causes link errors. Therefore, this
# script strips the external declarations from the header files. This
# script should be run over all .h files (but not .c files) generated
# by the C code generator


open(LS,"ls $D/[sbp]_*.h|")||die "can't execute ls\n";
$_ = <LS>;
if (!/not found/){
	print "\nRemoving field forward declarations\n";
	print   "-----------------------------------\n";
	while ($_){
		chop;
		print "  $_\n";
		`mv $D/$_ $D/temp`;
		open(FILE,">$D/$_")||die "can't create file $D/$_\n";
		open(TEMP,"$D/temp")||die "can't open a temporary file\n";

# If the line does not contain any reference to a user-defined FIELDS
# operator or ADT, then print it

		while(<TEMP>){
			print FILE if !/[Gg][Uu]_[0-9A-Z_][0-9A-Z_]*_FIELDS/;
		}
		$_ = <LS>;
		close(FILE);
	}
}
`rm $D/temp`;




# Remove the gu_XXXXX prefix from any gu_XXXXX_xxxxx calls in the
# [sbp]_*.h files
##################################################################

$INCLUDE = $FIELDS_DIR;
open(LS,"ls $D/[sb]_*.h|")||die "can't execute a ls\n";
$_ = <LS>;
if (!/not found/){
       print "\nRemoving Geode-generated operator prefix\n";
       print   "----------------------------------------\n";
       while ($_){
               chop;
               print "  $_\n";
		`mv $D/$_ $D/temp`;
               open(FILE,">$D/$_")||die "can't create file $D/$_";
		open(TEMP,"$D/temp")||die "can't open a temporary file\n";
		open( FILEA,">/dev/null")||die "can't open file /dev/null\n";
		&remove_gu_prefix;
		$_=<LS>;
		close(FILE);
	}
	`rm $D/temp`;
}



# remove_gu_prefix
###################

# This script is used during C code generation. During the C code
# generation process, the Geode C code generator prepends some tokens to
# the names of ADT operators defined by the user. This script strips the
# tokens back off the operators in order to make the linking process
# easier, particularly in the case of field macros.

# Field macros (of which there are thousands) appear to be ADT operators
# to the C code generator. The C code generator assumes that ADT operators
# are C functions that will be linked into the generated code, but the field
# macros are actually C macros. Since they are #included instead of
# linked in, the prepended tokens must be stripped off so that the compiler
# can resolve the symbol name.

# In addition, this script capitalizes the field names so that the resulting
# code will link; TLC, the program which automatically generates header
# files containing capitalized field names, whereas the Geode C code
# generator automatically generates code contained lowercase operator names


sub remove_gu_prefix{

#
# If the line contains a field expression, then change the prefix, which
# may vary, into a static prefix
#

   while(<TEMP>){
	    s/gu_([A-Z0-9_]+)FIELDS/gu_FIELDS/g;

#
# then capitalize the field expression and strip off the temporary prefix
#

       if (m/gu_FIELDS_([a-z][a-z0-9]{5}_(a|d|e|(ep)|(db)|(dp)))/){

#
# While a field expression exists in the string, lowercase it
#

           while(/gu_FIELDS_([a-z][a-z0-9]{5}_(a|d|e|(ep)|(db)|(dp)))/){
	        $field_name=$1;
	        $field_name =~ tr/a-z/A-Z/;
	        s/gu_FIELDS_([a-z][a-z0-9]{5}_(a|d|e|(ep)|(db)|(dp)))/$field_name/;
	        $line=$_;
	        $field2 = substr($field_name,0,3);
	        $field2 =~ tr/A-Z/a-z/;
	        $table_name = sprintf("%s/%s.h", $INCLUDE,$field2);
	        $field_name =~ /(^_)+_((.)+)/;
  	        $field3 = $2;
	        $field3 =~ tr/A-Z/a-z/;
	        $field_name .= "($field3)";		
	        if (!$database{$field_name} ){
	           if(!open(TABLE,"$table_name")){
	             print FILEA "Unable to open file $table_name for reading.\n";
		     exit(0);
	           }
	           while(<TABLE>){
		      if(/^#.?define (.{6}_[A-Z0-9]+[^ ]+) +([^ ]+)/){
		        $database{$1} = $2;
		        chop $database{$1};
		      }
	           }
		   close(TABLE);
	        }
	        if (!$database{$field_name} ){
	            print FILEA "$fiels_name not in $table_name\n";
	            exit(0);
	        }
	        else{
	            if(!($database{$field_name} =~ /,/)){
		        $field4=$database{$field_name};
		        print FILEA "#define $field4    $database{$field4}\n";
	            }
	            print FILEA "#define $field_name    $database{$field_name}\n";
	        }
	        $_ = $line;
           }
       }

#
# Process all other user ADT operators
#

	    s/gu_[A-Z0-9_]+_([a-z0-9_][a-z0-9_]*)/\1/g;

#
# Print the (possibly modified) input record
#

       print FILE;
   }
}




# Remove the gu_XXXXX prefix from any gu_XXXXX_xxxxx calls in the p_*.c
# files and include field definitions
########################################################################

open(LS,"ls $D/p_*.c|")||die "can't execute ls\n";
$_ = <LS>;
if (!/not found/){
       print "\nRemoving Geode-generated operator prefix and\n";
       print "including field definitions\n";
       print "----------------------------------------------\n";
       while ($_){
               chop;

		# Get the name of the current file, sans extension

		/(p_(\S+))\.c/;
		$H = $1;
		print "$H\n";
		$A=$H;$A.='_a';

		# Process the current .c file

		`mv $D/$_ $D/tmp`;
               open(FILE,">$D/$_")||die "can't create file $D/$_\n";
		open(TEMP,"$D/tmp")|| die "can't open a temporary file\n";
		open(FILEA,">$D/$A")|| die "can't create file $D/$A\n" ;
		&remove_gu_prefix;		
		close(FILEA);
		`rm $D/tmp`;
		$B=$H;$B.='_b';
		open (FILEB,">$D/$B")|| die "can't create file $D/$B\n";
		open(FILEA,"$D/$A")|| die "can't read file $D/$A\n";
		@file = <FILEA>;
		print FILEB sort @file;
		$C = $H; $C.= '.h';
		open( FILEH, ">$D/$C")|| die "can't create file $D/$C\n";
		open (FILEB,"$D/$B")|| die "can't read file $D/$B\n";
		while(<FILEB>){
			if( $_ ne $memory){
				if( length() > 50){
					print FILEH;
					$memory = $_;
				}
			}
			
		}
		open (FILEB,"$D/$B")|| die "can't read file $D/$B\n";
		$memory='';
       	while(<FILEB>){
			if( $_ ne $memory){
				if( length() <= 50){
					print FILEH;
					$memory = $_;
				}
			}
       	}
		`rm $D/$A`;
		`rm $D/$B`;
		$_=<LS>;
		#close(FILEH);
	}
}



			############
			## END ...##
			############


