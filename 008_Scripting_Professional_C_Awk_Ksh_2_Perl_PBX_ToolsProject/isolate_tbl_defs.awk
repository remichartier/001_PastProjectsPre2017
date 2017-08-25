# sccs_id=@(#)isolate_tbl_defs	1.3 05/07/93
# sccs_id=@(#)isolate_tbl_defs	1.1 04/30/93
# 

# This awk script is used during C code generation. Because table fields
# appear to SDL to be ADT operators, the C code generation process adds
# external declarations to the header files of each generated C file.

# Other awk scripts strip out the headers and insert the #defines for
# IML fields. However, some field-related information is not described
# as #defines but as extern variable declarations. This script isolates
# that information into one output stream so that it may be incorporated
# into the hpredefs file

/\#ifndef [A-Z0-9]{3}_DEF/, /\#endif/ {
   if (!match ($0, /ifndef/) && !match ($0, /endif/) && !match ($0, /DEF/))
       print
}
