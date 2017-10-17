#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <MKL25Z4.h>

	void my_strcpy(const char *src, char *dst, int num)
{
	for (int i = 0; i < num; i++){
		if (src[i] != '\0')
			dst[i] = src[i];
		else
			dst[i] = '\0';
	}
}


void my_capitalize(char *str, int num)
{
	for(int i = 0; i < sizeof(str); i++){
		if((*(str + i) >= 'a') && (*(str + i) <= 'z'))
			  *(str + i) = *(str + i) - 32;
		else if((*(str + i) >= 'A') && (*(str + i) <= 'Z'))
			  continue;
		else 
			  continue;
	}
}
int main(void)
{
	  char a[] = "Hello world!";
	  char b[20];	
	  my_strcpy(a, b, sizeof(a));
	  my_capitalize(b, sizeof(a));
	
	  return 0;
}
