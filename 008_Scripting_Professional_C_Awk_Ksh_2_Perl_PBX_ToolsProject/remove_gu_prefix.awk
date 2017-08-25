# sccs_id=@(#)remove_gu_prefix	1.3 05/06/93
# sccs_id=@(#)remove_gu_prefix	1.1 04/30/93
# 

# This awk script is used during C code generation. During the C code
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

BEGIN {
   INCLUDE = ENVIRON ["FIELDS_DIR"]
   AWK_SCRIPT_DIR = ENVIRON ["AWK_SCRIPT_DIR"]
}

#
# If the line contains a field expression, then change the prefix, which
# may vary, into a static prefix
#
/gu_[A-Z0-9_]+FIELDS/ {
   gsub (/gu_[A-Z0-9_]+FIELDS/, "gu_FIELDS")
}

#
# then capitalize the field expression and strip off the temporary prefix
#
/gu_FIELDS_[a-z][a-z0-9]{5}_(a|d|e|(ep)|(db)|(dp))/ {
   #
   # While a field expression exists in the string, lowercase it
   #
   while (match ($0, /gu_FIELDS_[a-z][a-z0-9]{5}_(a|d|e|(ep)|(db)|(dp))/)) {
       field_name = substr ($0, RSTART+10, RLENGTH-10)
       field_name = toupper (field_name)
       sub (/gu_FIELDS_[a-z][a-z0-9]{5}_(a|d|e|(ep)|(db)|(dp))/, field_name)

       table_name = sprintf ("%s/%s.h", INCLUDE,
           tolower (substr (field_name, 1, 3)))
       command = sprintf ("%s/gfi %s %s", AWK_SCRIPT_DIR, field_name, table_name)
       system (command)
   }
}

#
# Process all other user ADT operators
#
/gu_[A-Z0-9_]+_[a-z0-9_][a-z0-9_]*/ {
   gsub ("gu_[A-Z0-9_]+_", "", $0)
}

#
# Print the (possibly modified) input record
#
{
   print
}
