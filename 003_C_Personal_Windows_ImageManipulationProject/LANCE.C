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
	printf("\t\tษออออออออออออออออออออออออออออออออออออออป\n");
	printf("\t\tบ     TAPEZ: \"D\" POUR DOS.             บ\n");
	printf("\t\tบ       OU   \"W\" POUR WINDOWS.         บ\n");
	printf("\t\tศออออออออออออออออออออออออออออออออออออออผ\n");
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