rc -r -i\borlandc\include image1.rc
bcc -c -w-par -P -W -2 image1.c fonction.c
tlink /c /n /Tw /L\borlandc\lib c0ws image1 fonction, image1, NUL, import mathws cws, image1
rc -t image1.res
exit

