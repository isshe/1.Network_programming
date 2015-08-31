#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include "my.h"


char *gf_time(void)
{
    struct  timeval     tv;
    time_t  t;
    static  char        str[30];
    char    *ptr = NULL;

    if (gettimeofday(&tv, NULL) < 0)
    {
         err_sys("gettimeofday error");
    }

    t = tv.tv_sec ;         //POSIX says tv.tv_secis time_t; some BSDs don't agree
    ptr = ctime(&t);    //ptr格式: sat Aug 22 15:48:34 2015 
    	
   	printf("ptr = %s\n", ptr);

    strcpy(str, &ptr[11]);          //str格式: 15:48:34 2015   

	printf("str = %s, str_len = %ld\n", str, strlen(str));
	
	// +8 是从第九位开始(第九个元素的地址), 前面八位不变, -8 是减去前面8位长度! 帅啊!!!
    snprintf(str+8, sizeof(str) - 8, ".%06ld", tv.tv_usec);  //小数点后6位

    return(str);
}


int main(void)
{
	printf("time = %s\n", gf_time());
	
	return 0;
}
