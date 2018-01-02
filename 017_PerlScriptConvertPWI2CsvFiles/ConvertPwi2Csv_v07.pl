#!/usr/bin/perl

# First Step : open a file and convert it to .csv file.

# to access directory via terminal windows : cd ~/00_Mirrored/tocopyhomepc/docs/023 informatique/20180101_0607_ConvertPwi2ExcelFile/02_PerlScripts

$FOLDER = "1mydivers";

$WORKING_DIR = "../01_OriginalData";

$RESULT_DIR = "../03_Processed";

$D = $WORKING_DIR if $D eq "";

$PRINT_DEBUG = 1;

print "print_debug = $PRINT_DEBUG \n";

# Need to process all files in 1 folder and output all findings in only 1 output .csv file

#call subroutine subroutine convert_folder_2_cvs_file(DataPath, FolderName)

# Main()

# List all folders in Data folder to process.
open(LS,"ls $WORKING_DIR|") || die "can't execute ls\n";
$LIST_DIR = <LS>;
if (!/not found/){      # return of ls if file not found : "not found"  

	while ($LIST_DIR){
			chomp($LIST_DIR);
			if ($PRINT_DEBUG) {print "Folder to process \"$LIST_DIR\"\n";}
			# Then Process each folder
			if ($PRINT_DEBUG) {print "Here should call  convert_folder_2_cvs_file($WORKING_DIR, $LIST_DIR);\n";}
			convert_folder_2_cvs_file($WORKING_DIR, $LIST_DIR);
			$LIST_DIR = <LS>; #go to next folder
			chomp($LIST_DIR);
			if ($PRINT_DEBUG) {print "NEXT Folder to process should be \"$LIST_DIR\"\n";}
		}
}

close(LS);



# End Main()



############
## END ...##
############


#############################################
##  subroutine convert_folder_2_cvs_file(DataPath, FolderName)
#############################################
sub convert_folder_2_cvs_file{
	if ($PRINT_DEBUG) {print "Enter convert_folder_2_cvs_file($_[0], $_[1])\n";}
	#retrieve sub input arguments :
	$DATAPATH = $_[0];
	$FOLDERNAME = $_[1];

	#Here list all file names in this directory, then will process all files content
	open(LS2,"ls $DATAPATH/$FOLDERNAME/*.pwi|") || die "can't execute ls\n";
	$_ = <LS2>;
	if (!/not found/){      # return of ls if file not found : "not found"  

		#Open result file with filename = Foldername.csv
		open(RESULTS,">$RESULT_DIR/$FOLDERNAME.csv")|| die "	can't create file $RESULT_DIR/$FOLDERNAME.csv\n";
		if ($PRINT_DEBUG) {print "	Output File \"$RESULT_DIR/$FOLDERNAME.csv\" open successfully\n";}

		while ($_){
			chomp($_);
			# Process file
			if ($PRINT_DEBUG) {print "		Processing file $_";}
			convert_file_2_cvs_line("$_", RESULTS);
			$_ = <LS2>;
		}

		#Close $RESULT_DIR/foldername.csv
		close(RESULTS);
		if ($PRINT_DEBUG) {print "	File \"$RESULT_DIR/$FOLDERNAME.csv\" closed\n";}
	}

	close(LS2);

} # end of sub convert_folder_2_cvs_file()


#############################################
##  subroutine convert_file_2_cvs_line(filename, resultFileHandle)
#############################################

sub convert_file_2_cvs_line{

	if ($PRINT_DEBUG) {print "Enter convert_file_2_cvs_line($_[0], $_[1])\n";}
	# retrieve routine input arguments
	$FILENAME=$_[0];
	if ($PRINT_DEBUG) {print "		Retrieve argument subroutine arg0=$FILENAME \n";}
	$OUTPUTFILE = $_[1];
	if ($PRINT_DEBUG) {print "		Retrieve argument subroutine arg1=$OUTPUTFILE \n";}

	open(FILE,"<$FILENAME")|| die "		can't create file $FILENAME\n";
	if ($PRINT_DEBUG) {print "		Input File \"$FILENAME\" open successfully\n";}

	# read file for valuable characters and write them into result .csv file
	while(<FILE>){

		# Here need to catch all the strings of each line starting by \00, ending by \C4
		# Final version : matches all strings starting with \00, and having 2 or more ligible characters including some non alphabet characters, and only catching those
		if (/\00[A-Za-z0-9%\-\s,\.;:=><'"]{2,}/) { 
			if ($PRINT_DEBUG) {print "trouvé  string \"$&\" \n";} 
			#remove the "\00" in front of each string
			$string = $&;
			$string =~ s/\00//;
					
				# Write into the result file
				print $OUTPUTFILE "$string ; ";
		}					 
	}

	close(FILE); 
	if ($PRINT_DEBUG) {print "		File \"$FILENAME\" closed\n"};

	# step to next file name

	# add a newline into result file to separate files data in different lines on the result file
	print $OUTPUTFILE "\n"; 


} # end of sub convert_file_2_cvs_line()


