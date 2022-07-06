#include <stdio.h>
#include <stdlib.h>
#include<string.h>

#define bytes_sec 512;
typedef struct 
{
	double timestamp;
	double response;
	char IOType;
	int LUN;
	long long offset;
	int size;

}DataIO;

DataIO TraceLine[5000000];
int line = 0;

void ReadFiles(){
	FILE *fp;
	char file_name[50];
	char first_raw[200];
	printf("Please input the file name: \n");
	scanf("%s",file_name);
	fp = fopen(file_name,"r");
	if (fp==NULL){
		printf("Read file Error!");
		exit(-1);
	}
	fgets(first_raw,sizeof(first_raw),fp); //skip 1st header

	char trace_fields[100];
	while(-1 != fscanf(fp,"%s",trace_fields))
	{
		int row =0;
		char cache[100];
		char* token = strtok(trace_fields,",");
		while(token){
			sprintf(cache,"%s",token); //copy from token to cache

			switch (row){
			case 0:
				TraceLine[line].timestamp = atof(cache);
				//printf("Line %d: %lf\t",line,TraceLine[line].timestamp);
				break;
			case 1:
				TraceLine[line].response = atof(cache);
				//printf("%lf\t",TraceLine[line].response);
				break;
			case 2:
				TraceLine[line].IOType = cache[0];
				//printf("%c\t",TraceLine[line].IOType);
				break;
			case 3:
				TraceLine[line].LUN = atoi(cache);
				//printf("%d\t",TraceLine[line].LUN);
				break;
			case 4:
				TraceLine[line].offset = atoll(cache);
				//printf("%lld\t",TraceLine[line].offset);
				break;
			case 5:
				TraceLine[line].size = atoi(cache);
				//printf("%d\t\n",TraceLine[line].size);
				break;
			}

			token = strtok(NULL,",");
			row++;
		}
		line++;
	}
	fclose(fp);
	printf("*************Finish Reading*************\n\n");
	/*for(int i=0;i<line;i++){
		printf("Read Line %d: %lf,%lf,%c,%d,%lld,%d\n",i,TraceLine[i].timestamp,TraceLine[i].response,TraceLine[i].IOType,TraceLine[i].LUN,TraceLine[i].offset,TraceLine[i].size);
	}*/
}

void NewFile(){
	int size_sector=0;
	long long offset_sector=0;
	long long offset_plus=0;
	char new_file_name[100];
	printf("Please input the new file name you want to save: \n");
	scanf("%s",new_file_name);
	FILE* nfp = fopen(new_file_name, "w");
	fprintf(nfp,"timestamp,IOType,sector_offset,sector_size\n");
	for (int i=0;i<line;i++){
		size_sector = TraceLine[i].size/bytes_sec;
		offset_sector = TraceLine[i].offset/bytes_sec;
		if(size_sector>1){
			offset_plus=offset_sector-(long)1;
			for(int plus_lines=0;plus_lines<size_sector;plus_lines++){
				offset_plus++;
				fprintf(nfp,"%lf,%c,%lld,%d\n",TraceLine[i].timestamp,TraceLine[i].IOType,offset_plus,1);
				printf("Original line %d: %lf,%lf,%c,%d,%lld,%d\n:",(i+2),TraceLine[i].timestamp,TraceLine[i].response,TraceLine[i].IOType,TraceLine[i].LUN,TraceLine[i].offset,TraceLine[i].size);
				printf("Modify line %d: %lf,%c,%lld,%d\n",(i+2),TraceLine[i].timestamp,TraceLine[i].IOType,offset_plus,1);
			}
		}
		else
			fprintf(nfp,"%lf,%c,%lld,%d\n",TraceLine[i].timestamp,TraceLine[i].IOType,offset_sector,size_sector);
			printf("No need modify line %d: %lf,%lf,%c,%d,%lld,%d\n:",(i+2),TraceLine[i].timestamp,TraceLine[i].response,TraceLine[i].IOType,TraceLine[i].LUN,TraceLine[i].offset,TraceLine[i].size);

	}
	fclose(nfp);
	printf("Finish Modify!\n");

}


int main(int argc, char* argv[]) {

	printf("Start!!\n");
	ReadFiles();
	printf("Read %d lines traces\n", line);
	NewFile();
	printf("Finish\n");
	return 0;
}