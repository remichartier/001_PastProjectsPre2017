rc -r -i\borlandc\include colors1.rc
bcc -c -w-par -P -W -2 colors1.c fonction.c
tlink /c /n /Tw /L\borlandc\lib c0ws colors1 fonction, colors1, NUL, import mathws cws, colors1
rc -t colors1.res
exit

