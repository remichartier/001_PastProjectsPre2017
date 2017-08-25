# sccs_id=@(#)remove_gu_field_lines	1.1 04/30/93
# 
#
# This awk script is used during C code generation. Because table fields
# appear to SDL to be ADT operators, the C code generation process adds
# external declarations to the header files of each generated C file.
#
# However, table fields are actually C macros, and leaving the external
# declarations in the header files causes link errors. Therefore, this
# awk script strips the external declarations from the header files. This
# script should be run over all .h files (but not .c files) generated
# by the C code generator

# If the line does not contain any reference to a user-defined FIELDS
# operator or ADT, then print it
!/[Gg][Uu]_[0-9A-Z_][0-9A-Z_]*_FIELDS/ {
   print
}
