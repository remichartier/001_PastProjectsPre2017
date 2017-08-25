#include <stdio.h>

void transfert_memoire_buffer_32bits(
		unsigned long far *buffer1 ,
		unsigned long far *buffer2 ,
		unsigned int nbelements )
{
	asm push es
	asm push ds
	asm les di,buffer1
	asm lds si,buffer2
	asm mov cx,nbelements	// voir sur des longs nbelements si ca marche
	asm rep movsd


	asm pop ds
	asm pop es
}

void transfert_port_buffer_32bits(
		unsigned long far *buffer1 ,
		unsigned int nbelements )
{
	asm les di, buffer1
	asm mov dx, 0x00
	asm mov cx,nbelements
	asm rep insd
}

void rec32(unsigned long far *hpBuffer, unsigned long Length)
{
	unsigned long		 i, NbDwordsLeft = 0, NbBlocks = 0, NbBytesLeft = 0;
	unsigned long		 dwAD_MEMOIRE;


	NbBlocks = Length / 65536L;               // Buffer en nombre de segment de 64Ko
	NbDwordsLeft = (Length % 65536L) >> 2; 		// Buffer en nombre de mots de 32 bits (entier)
	NbBytesLeft = Length & 0x03; 							// Octets eventuellement restants

	dwAD_MEMOIRE = (unsigned long)0x5800;//AD_MEMOIRE;


	for (i=0; i<NbBlocks; i++)
	{
		//asm   push		ds
		//asm		lds			si, hpBuffer
		asm   mov 		cx, 16384
		asm   mov			edx, dwAD_MEMOIRE					// Sélection adresse pour les accès ultérieurs
		//asm 	rep outsd
		//asm		pop 		ds
		asm push es//
		asm les di,hpBuffer//
		asm rep insd//
		asm pop es//

		hpBuffer = hpBuffer + 16384;  					// segt suivant
	}

	if (NbDwordsLeft != 0)
	{
			//asm   push		ds                      // Boucle sur Data résiduelles
			//asm		lds			si, hpBuffer            // (moins de 64000 octets)
			asm   mov 		cx, NbDwordsLeft
			asm   mov			edx, dwAD_MEMOIRE
			//asm 	rep outsd
			//asm		pop 		ds
			asm push ds //
			asm les di,hpBuffer//
			asm rep insd//
			asm pop es


			hpBuffer = hpBuffer + NbDwordsLeft;

			if (NbBytesLeft != 0)
			{
				asm   les     di, hpBuffer
				asm		mov 		eax, es:[di]
				asm   mov			edx, dwAD_MEMOIRE
				//asm 	out			dx, eax
				asm in eax,dx
			}
	}
	//return TRUE;
}


void rec16(unsigned int far *hpBuffer, unsigned long Length)
{
	unsigned long		 i, NbDwordsLeft = 0, NbBlocks = 0, NbBytesLeft = 0;
	unsigned long		 dwAD_MEMOIRE;


	NbBlocks = Length / 65536L;               // Buffer en nombre de segment de 64Ko
	NbDwordsLeft = (Length % 65536L) >> 1;//commentaire faux 		// Buffer en nombre de mots de 32 bits (entier)
	NbBytesLeft = Length & 0x01; 							// Octets eventuellement restants

	dwAD_MEMOIRE = (unsigned long)0x5800;//AD_MEMOIRE;


	for (i=0; i<NbBlocks; i++)
	{
		//asm   push		ds
		//asm		lds			si, hpBuffer
		asm   mov 		cx, 32768//16384
		asm   mov			edx, dwAD_MEMOIRE					// Sélection adresse pour les accès ultérieurs
		//asm 	rep outsd
		//asm		pop 		ds
		asm push es//
		asm les di,hpBuffer//
		asm rep insw//
		asm pop es//

		hpBuffer = hpBuffer + 32768;  					// segt suivant
	}

	if (NbDwordsLeft != 0)
	{
			//asm   push		ds                      // Boucle sur Data résiduelles
			//asm		lds			si, hpBuffer            // (moins de 64000 octets)
			asm   mov 		cx, NbDwordsLeft
			asm   mov			edx, dwAD_MEMOIRE
			//asm 	rep outsd
			//asm		pop 		ds
			asm push ds //
			asm les di,hpBuffer//
			asm rep insw//
			asm pop es


			hpBuffer = hpBuffer + NbDwordsLeft;

			if (NbBytesLeft != 0)
			{
				asm   les     di, hpBuffer
				asm		mov 		eax, es:[di]
				asm   mov			edx, dwAD_MEMOIRE
				//asm 	out			dx, eax
				asm in eax,dx
			}
	}
	//return TRUE;
}
