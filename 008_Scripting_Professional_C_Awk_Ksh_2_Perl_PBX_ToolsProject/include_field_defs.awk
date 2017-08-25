# sccs_id=@(#)include_field_defs	1.3 05/07/93
# sccs_id=@(#)include_field_defs	1.1 04/30/93
# 

# This awk script searches for field definitions within a C file. If it
# finds one, then it retrieves the field information from the fields
# directory 

BEGIN {
   INCLUDE = ENVIRON ["FIELDS_DIR"]
   AWK_SCRIPT_DIR = ENVIRON ["AWK_SCRIPT_DIR"]
}

/[A-Z][A-Z0-9]{5}_(A|D|E|(EP)|(DB)|(DP))/ {
   #
   # While a field expression exists in the string, process the line
   #
   line = $0
   while (match (line, /[A-Z][A-Z0-9]{5}_(A|D|E|(EP)|(DB)|(DP))/)) {
       field_name = substr (line, RSTART, RLENGTH)

       sub (/[A-Z][A-Z0-9]{5}_(A|D|E|(EP)|(DB)|(DP))/, "", line)

       table_name = sprintf ("%s/%s.h", INCLUDE,
           tolower (substr (field_name, 1, 3)))
       command = sprintf ("%s/gfi %s %s", 
           AWK_SCRIPT_DIR, field_name, table_name)
       system (command)
   }
}

{
   print
}
