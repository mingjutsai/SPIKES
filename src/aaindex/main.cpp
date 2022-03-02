#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void index_sort(int *pi, float *p, int no);
int get_aa_no(char c);
float av(float *seq, int seq_len);
void seq2aaindex(char *seq, int *genes);
void read_seq(char *filename);
void read_aaindex();
void parse_command_line(int argc, char **argv, char *input_file_name, char *output_file_name);
void exit_with_help();
void read_select(int &no_features, int *gene);

int av_type;
int initial_from_select;

int aaindex_no;
float **aaindex;

int max_seq_len;
int min_seq_len;
int seq_no;
char **seq;
float *label;

int main(int argc, char **argv){
	int i;
	char input_file_name[1024];
	char output_file_name[1024];
	
	parse_command_line(argc, argv, input_file_name, output_file_name);
	
	read_aaindex();
	
	int no_features;
	int *genes = new int [aaindex_no];
	if(initial_from_select == 1 ){
		read_select(no_features, genes);
	}else{
		for(int i = 0; i< aaindex_no; i++)
			genes[i] = 1;
	}

	/*
	for(i = 0; i < aaindex_no; i++)
		if(genes[i] == 1)
			printf("%d, ", i+1);
	*/
	
	read_seq(input_file_name);

	int *seq_i = new int[seq_no];
	index_sort(seq_i, label, seq_no);
		
	for(i=0; i<seq_no; i++)
	{
		//printf("%d | %d | %d\n ", i, seq_i[i], label[seq_i[i]]);
		printf("%g", label[seq_i[i]]);
		seq2aaindex(seq[seq_i[i]], genes);
	}
	
	/*
	printf("%d\n",aaindex_no);
	for(i=0; i<aaindex_no; i++){
		for(j=0; j<20; j++)
			printf("%f ",aaindex[i][j]);
		printf("\n");
	}
	*/
}

//read "select" file;	no_features, genes -> must be allocated previously
void read_select(int &no_features, int *genes){
	FILE *fp = fopen("src/aaindex/select","r");
	
	if(fp == NULL)
	{
		fprintf(stderr,"can't open input file %s\n"," src/aaindex/select");
		exit(1);
	}

	no_features = 0;
	while(1)
	{
		int c = fgetc(fp);
		switch(c)
		{
			case ',':
				++no_features;
				break;
			case EOF:
				goto re;
			default:
				;
		}
	}
re:
	rewind(fp);
	
	//printf("Total Number of selected features: %d\n", no_features);
	
	int *feature = new int [no_features];
	
	for(int i = 0; i< aaindex_no; i++)
		genes[i] = 0;
	
	for(int i = 0; i < no_features; i++){
		fscanf(fp,"%d,", &feature[i]);
		genes[feature[i]-1] = 1;
	}
	fclose(fp);
}

//return sorted index array; 	index 	-> pi
//				value 	-> p
//				no_data	-> no
void index_sort(int *pi, float *p, int no)
{
 	int i, j, t;
	for(i=0; i< no; i++)
		pi[i] = i;
	for(i=1; i< no; i++)
	{
		j=i-1;
		t = pi[i];
		while(j>=0 && p[t] < p[pi[j]]){
			pi[j+1] = pi[j];
			j--;
		}
		pi[j+1] = t;
	}
}

int get_aa_no(char c)
{
	int aa_no;
	switch(c){
		case 'A': aa_no = 0; break;	
		case 'R': aa_no = 1; break;	
		case 'N': aa_no = 2; break;	
		case 'D': aa_no = 3; break;	
		case 'C': aa_no = 4; break;	
		
		case 'Q': aa_no = 5; break;	
		case 'E': aa_no = 6; break;	
		case 'G': aa_no = 7; break;
		case 'H': aa_no = 8; break;
		case 'I': aa_no = 9; break;
				
		case 'L': aa_no = 10; break;
		case 'K': aa_no = 11; break;
		case 'M': aa_no = 12; break;
		case 'F': aa_no = 13; break;
		case 'P': aa_no = 14; break;
				
		case 'S': aa_no = 15; break;
		case 'T': aa_no = 16; break;
		case 'W': aa_no = 17; break;
		case 'Y': aa_no = 18; break;
		case 'V': aa_no = 19; break;
			
		default: aa_no = -1; break;
	}
	return aa_no;
}

void seq2aaindex(char *seq, int *genes)
{
	int i, j;
	int aa_no;
	int gene_no = 0;
	int seq_len = strlen(seq);
	float *aa_seq = new float[seq_len];
	for(i=0 ; i< aaindex_no ; i++){
		if(genes[i] == 1){
			gene_no++;
			for(j=0 ; j< seq_len ; j++)
			{
				aa_no = get_aa_no(seq[j]);
				if(aa_no == -1){
					printf("Error: %s - %c can't be recognized\n",seq, seq[j]);
					exit(1);
				}
				else
					aa_seq[j] = aaindex[i][aa_no];
			}
			if(av_type == 0){
				float value = av(aa_seq, seq_len);
				printf(" %d:%f", gene_no, value);
				//printf(" %d:%d:%f", gene_no, (i+1), value);
			}else if(av_type == 1){
				for(j=0 ; j< seq_len ; j++)
				{
					printf(" %d:%f", ((gene_no-1)*seq_len + j + 1), aa_seq[j]);
				}		
			}
		}
	}
	printf("\n");
	
	delete [] aa_seq;
}

float av(float *seq, int seq_len)
{
	int i;
	float value = 0;
	for(i=0 ; i< seq_len ; i++){
		value += seq[i];
	}
	return (value/seq_len);
}

void read_seq(char *filename)
{
	int i;
	int seq_len = 0;
	
	max_seq_len = 0;
	min_seq_len = 999999;
	seq_no = 0;
	
	FILE *fp = fopen(filename,"r");
	if(fp == NULL)
	{
		fprintf(stderr,"can't open input file %s\n",filename);
		exit(1);
	}
	while(1)
	{
		char c = fgetc(fp);
		switch(c)
		{
			case '\t':
				if(seq_len > max_seq_len)
					max_seq_len = seq_len;
				if(seq_len < min_seq_len)
					min_seq_len = seq_len;
				break;
			case '\n':
				seq_len = 0;
				seq_no++;
				break;
			case EOF:
				goto re;
			default:
				seq_len++;
				;
		}
	}
re:
	rewind(fp);
	
	label = new float [seq_no];
	seq = new char *[seq_no];
	for(i=0; i<seq_no; i++)
		seq[i] = new char[max_seq_len];
	
	for(i=0; i<seq_no; i++)
	{
		fscanf(fp, "%s\t%f", seq[i], &label[i]);
		//printf("%s\t%d\t%d\n", seq[i], label[i], strlen(seq[i]));
		
		//printf("%d", label[i]);
		//seq2aaindex(seq[i]);
	}
	
	fclose(fp);
}

void read_aaindex()
{
	int i, j;
	aaindex_no=0;
	
	FILE *fp = fopen("src/aaindex/aaindex1.tab","r");
	if(fp == NULL)
	{
		fprintf(stderr,"can't open input file %s\n", "src/aaindex/aaindex1.tab" );
		exit(1);
	}
	while(1)
	{
		char c = fgetc(fp);
		switch(c)
		{
			case '\n':
				aaindex_no++;
				break;
			case EOF:
				goto re;
			default:
				;
		}
	}
re:
	rewind(fp);

	aaindex = new float * [aaindex_no];
	for(i=0; i<aaindex_no; i++)
		aaindex[i] = new float[20];
	i=0;
	for(i=0; i<aaindex_no; i++)
	{
		for(j=0; j<20; j++)
			fscanf(fp, "%f", &aaindex[i][j]);
	}
	
	fclose(fp);
}

void parse_command_line(int argc, char **argv, char *input_file_name, char *output_file_name)
{
	int i;
	// default parameter
	av_type = 0;
	initial_from_select = 0;
	
	for(i=1;i<argc;i++)
	{
		if(argv[i][0] != '-') break;
		
		if(++i>=argc)
			exit_with_help();
		
		switch(argv[i-1][1])
		{
			case 'a':
				av_type = atoi(argv[i]);
				break;
			case 'A':
				initial_from_select = atoi(argv[i]);
				break;
			default:
				fprintf(stderr,"unknown option\n");
				exit_with_help();
		}
	}
	// check  parameter
	
	// determine filenames
	if(i>=argc)
		exit_with_help();
	
	strcpy(input_file_name, argv[i]);
}

void exit_with_help()
{
	printf(
	"Usage: encode [options] sequence_file\n"
	"options:\n"
	"--== feature encoding ==--\n"
	"-a av_type : use average as features (default 0)\n"
	"	0 -- average\n"
	"	1 -- flat\n"
	"--== feature selection ==--\n"
	"-A read_select : read select file (default 0)\n"
	"       0 -- all feature\n"
	"       1 -- read select\n"
	);
	exit(1);
}
