# sccs_id=@(#)remove_dmy_call	1.1 04/30/93
# 
#
# This awk script is used during C code generation. It is used to eliminate
# any dummy processes from the generated C code. This script should be
# invoked on the generated file c_declar.h during the code generation
# process

# If the line does not contain any reference to a variable called
# dmy_call, print it
!/dmy_call/ {
   print
}
