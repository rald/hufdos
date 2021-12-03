#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mem.h>


#define BUFFER_MAX 16384
#define FREQ_MAX 256
#define PATH_MAX 256


long getFileSize(char* filename) {
	struct stat statbuf;

	if(stat(filename,&statbuf)==-1) {
        	printf("failed to stat %s.\n",filename);
					return -1;
	}

	return statbuf.st_size;
}


unsigned long* countCharFreq(char *filename) {

	unsigned long* freq=calloc(FREQ_MAX,sizeof(unsigned long));
	FILE *fin;
	unsigned char buf[BUFFER_MAX];
	unsigned long gt=0;
	long sz=0;
	int max=0,i=0;
	struct stat statbuf;


	sz=getFileSize(filename);
	if(sz==-1) {
		printf("cannot get size of file: %s\n",filename);
		return NULL;
	}


	fin=fopen(filename,"rb");

	if(!fin) {
		printf("cannot open file %s.\n",filename);
		return NULL;
	}

	setvbuf(fin,NULL,_IOFBF,BUFFER_MAX);


	for(i=0;i<256;i++) freq[i]=0;

	gt=0;
	while(gt<sz) {

		max=(int)(sz-gt>=BUFFER_MAX?BUFFER_MAX:(unsigned long)sz-gt);

		memset(buf,0,BUFFER_MAX);

		if(fread(buf,max,1,fin)!=1) {
			printf("error reading file %s.\n",filename);
			return NULL;
		}

		gt+=max;

		printf("%10d %10lu / %10ld == %6.2f%%\n",max,gt,sz,(float)gt/sz*100.0);

		for(i=0;i<max;i++) {
			freq[buf[i]]++;
		}

	}

	fclose(fin);

	return freq;
}



int main(int argc,char **argv) {
	unsigned long *freq=NULL;
	unsigned long tf=0;
	int i;

	if(argc!=2) {
		printf("syntax: %s filename\n",argv[0]);
		return 1;
	}

	freq=countCharFreq(argv[1]);

	tf=0;
	for(i=0;i<256;i++) {
		printf("%02X %c %10lu\n",i,isalnum((unsigned char)i)||ispunct((unsigned char)i)?i:' ',freq[i]);
		tf+=freq[i];
	}
	printf("total: %lu\n",tf);


	return 0;
}
