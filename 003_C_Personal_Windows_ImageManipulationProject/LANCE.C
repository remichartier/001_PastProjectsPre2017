#include<process.h>
#define TEMPSMAX	 2000
#define R1		'd'
#define R11		'D'
#define R2		'w'
#define R21		'W'
int reponse();
main()
{
	int i;
	printf("\n");
	printf("\t\t浜様様様様様様様様様様様様様様様様様様融\n");
	printf("\t\t�     TAPEZ: \"D\" POUR DOS.             �\n");
	printf("\t\t�       OU   \"W\" POUR WINDOWS.         �\n");
	printf("\t\t藩様様様様様様様様様様様様様様様様様様夕\n");
	i=reponse();
	if(i==1)
	system("d.bat");
	if(i==2)
	system("w.bat");

}


int reponse()
{
	int i=0;
	int j=0;
	char c;

	while(j<=TEMPSMAX)
	{
		if(kbhit())
		{
			c = getch();
			if((c==R1)||(c==R11))
			{
				i=1;
				return(1);
			}
			if((c==R2)||(c==R21))
			{
				i=1;
				return(2);
			}
		}
		j++;
	}
	if((j>TEMPSMAX)&&(i==0))
		return(2);
}



