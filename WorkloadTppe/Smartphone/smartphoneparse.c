#include <stdio.h>
#include <stdlib.h>
#include<string.h>

typedef struct 
{
	long long sector_offset;
	int sector_size;
	int byte_size;
	int IOType;
	double req_gen_time;
	double req_start_time;
	double req_issue_time;
	double req_finish_time;

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

	char trace_fields[500];

	
	while(fgets(trace_fields,500,fp)!=NULL)
	{
		int row =0;
		char cache[100];
		char* token = strtok(trace_fields,"\t\t");
		while(token){
			sprintf(cache,"%s",token); //copy from token to cache

			switch (row){
			case 0:
				TraceLine[line].sector_offset = atoll(cache);
				//printf("Line %d: %lld\t",line,TraceLine[line].sector_offset);
				break;
			case 1:
				TraceLine[line].sector_size = atoi(cache);
				//printf("%d\t",TraceLine[line].sector_size);
				break;
			case 2:
				TraceLine[line].byte_size = atoi(cache);
				//printf("%d\t",TraceLine[line].byte_size);
				break;
			case 3:
				TraceLine[line].IOType = atoi(cache);
				//printf("%d\t",TraceLine[line].IOType);
				break;
			case 4:
				TraceLine[line].req_gen_time = atof(cache);
				//printf("%lf\t",TraceLine[line].req_gen_time);
				break;
			case 5:
				TraceLine[line].req_start_time = atof(cache);
				//printf("%lf\t",TraceLine[line].req_start_time);
				break;
			case 6:
				TraceLine[line].req_issue_time = atof(cache);
				//printf("%lf\t",TraceLine[line].req_issue_time);
				break;
			case 7:
				TraceLine[line].req_finish_time = atof(cache);
				//printf("%lf\n",TraceLine[line].req_finish_time);
				break;
			}

			token = strtok(NULL,"\t\t");
			row++;
		}
		line++;
	}
	
	fclose(fp);
	printf("*************Finish Reading*************\n\n");
	/*
	for(int i=0;i<line;i++){
		printf("Read Line %d: %lld,%d,%d,%d,%lf,%lf,%lf,%lf,\n",(i+1),TraceLine[i].sector_offset,TraceLine[i].sector_size,TraceLine[i].byte_size,TraceLine[i].IOType,TraceLine[i].req_gen_time,TraceLine[i].req_start_time,TraceLine[i].req_issue_time,TraceLine[i].req_finish_time);
	}*/
}

void NewFile(){
	long long offset_plus=0;
	char new_file_name[100];
	printf("Please input the new file name you want to save: \n");
	scanf("%s",new_file_name);
	FILE* nfp = fopen(new_file_name, "w");
	fprintf(nfp,"timestamp,IOType,sector_offset,sector_size\n");

	for (int i=0;i<line;i++){
		if(TraceLine[i].IOType==0 || TraceLine[i].IOType==1){
			if(TraceLine[i].sector_size>1){
				offset_plus=TraceLine[i].sector_offset-1;
				for(int plus_lines=0;plus_lines<TraceLine[i].sector_size;plus_lines++){
				offset_plus++;
				fprintf(nfp,"%lf,%d,%lld,%d\n",TraceLine[i].req_issue_time,TraceLine[i].IOType,offset_plus,1);
				printf("Original line %d: %lld,%d,%d,%d,%lf,%lf,%lf,%lf,\n",(i+1),TraceLine[i].sector_offset,TraceLine[i].sector_size,TraceLine[i].byte_size,TraceLine[i].IOType,TraceLine[i].req_gen_time,TraceLine[i].req_start_time,TraceLine[i].req_issue_time,TraceLine[i].req_finish_time);
				printf("Modify line %d: %lf,%d,%lld,%d\n",(i+1),TraceLine[i].req_issue_time,TraceLine[i].IOType,offset_plus,1);
			}
			}
			else
				fprintf(nfp,"%lf,%d,%lld,%d\n",TraceLine[i].req_issue_time,TraceLine[i].IOType,TraceLine[line].sector_offset,TraceLine[line].sector_size);
		}
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