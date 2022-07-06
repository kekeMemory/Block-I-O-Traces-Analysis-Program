#include <stdio.h>
#include <stdlib.h>
#include<string.h>

#define bytes_sec 512;
typedef struct 
{
	char DevNo[100];
	int CPU_ID;
	long long record_ID;
	double timestamp;
	int pid;
	char action;
	char IOType[100];
	long sector_offset;
	char flat;
	int sector_size;
	char process_name[100];

}DataIO;

DataIO TraceLine[50000000];
int line = 0;

void ReadFiles(){
	FILE *fp;
	char file_name[50];
	printf("Please input the file name: \n");
	scanf("%s",file_name);
	fp = fopen(file_name,"r");
	if (fp==NULL){
		printf("Read file Error!");
		exit(-1);
	}

	char trace_fields[200];
	while(fgets(trace_fields,200,fp)!=NULL)
	{
		int row =0;
		char cache[100];
		//printf("Line %d data:%s\n",line,trace_fields);
		char* token = strtok(trace_fields," ");
		while(token){
			//printf ("line: %d, row: %d, token: %s\t",line,row,token);
			sprintf(cache,"%s",token); //copy from token to cache
			switch (row){
			case 0:
				sprintf(TraceLine[line].DevNo,"%s",cache);
				//printf("DevNo:%s,",TraceLine[line].DevNo);
				break;
			case 1:
				TraceLine[line].CPU_ID = atoi(cache);
				//printf("CPU:%d,",TraceLine[line].CPU_ID);
				break;
			case 2:
				TraceLine[line].record_ID = atoll(cache);
				//printf("record ID:%lld,",TraceLine[line].record_ID);
				break;
			case 3:
				TraceLine[line].timestamp = atof(cache);
				//printf("timestamp: %lf,",TraceLine[line].timestamp);
				break;
			case 4:
				TraceLine[line].pid = atoi(cache);
				//printf("pid: %d,",TraceLine[line].pid);
				break;
			case 5:
				TraceLine[line].action = cache[0];
				//printf("action: %c,",TraceLine[line].action);
				break;
			case 6:
				sprintf(TraceLine[line].IOType,"%s",cache);
				//printf("IO Type:%s,",TraceLine[line].IOType);
				break;
			case 7:
				TraceLine[line].sector_offset = atoll(cache);
				//printf("lsn:%ld,",TraceLine[line].sector_offset);
				break;
			case 8:
				TraceLine[line].flat = cache[0];
				//printf("flat is: %c",TraceLine[line].flat);
				break;
			case 9:
				TraceLine[line].sector_size = atoi(cache);
				//printf("size:%d,",TraceLine[line].sector_size);
				break;
			case 10:
				sprintf(TraceLine[line].process_name,"%s",cache);
				//printf("process name:%s,",TraceLine[line].process_name);
				break;

			}
			token = strtok(NULL," ");
			row++;
			//printf("\n");
		}
		if(TraceLine[line].action == 'D' ){
			line++;
			continue;
		}
	}

	fclose(fp);
	printf("*************Finish Reading*************\n\n");

	/*
	for(int i=0;i<line;i++){
		printf("Read Line %d: %s,%d,%lld,%.9f,%d,%c,%s,%ld,%d,%s\n",(i+1),TraceLine[i].DevNo,TraceLine[i].CPU_ID,TraceLine[i].record_ID,TraceLine[i].timestamp,TraceLine[i].pid,TraceLine[i].action,TraceLine[i].IOType,TraceLine[i].sector_offset,TraceLine[i].sector_size,TraceLine[i].process_name);
	}
	*/
}

void NewFile(){
	long  offset_plus=0;
	char new_file_name[100];
	printf("Please input the new file name you want to save: \n");
	scanf("%s",new_file_name);
	FILE* nfp = fopen(new_file_name, "w");
	fprintf(nfp,"timestamp,IOType,sector_offset,sector_size\n");
	for (int i=0;i<line;i++){
		if (TraceLine[i].action == 'D'){
			if (strcmp(TraceLine[i].IOType ,"R")== 0 || strcmp(TraceLine[i].IOType,"W")==0){
				if(TraceLine[i].sector_size>1){
					offset_plus=TraceLine[i].sector_offset-1;
					for(int plus_lines=0;plus_lines<TraceLine[i].sector_size;plus_lines++){
						offset_plus++;
						fprintf(nfp,"%.9f,%s,%ld,%d\n",TraceLine[i].timestamp,TraceLine[i].IOType,offset_plus,1);
					}
				}
				else
					fprintf(nfp,"%.9f,%s,%ld,%d\n",TraceLine[i].timestamp,TraceLine[i].IOType,TraceLine[i].sector_offset,TraceLine[i].sector_size);

			}
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

