
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "hash.h"
#include "records.h"
//#include <math.h>
#define LINESZ 1024

typedef struct{
	double value;
	int position;
	int *distances;
}entries;

typedef struct{
	int distance_from_centroid;
	int position;
	int centroid_position;
	int distance_from_2nd_centroid;
	int second_centroid_position;
	int *distances;
	int has_been_centroid;
	int is_centroid;
	int which_centroid;
}data;

typedef struct{
	int position;
	double value;
	int *distances;
	int sum_distances;
	int total;
}centroids;


centroids** plus_init(int **distance_matrix_hamming,data **data_array,int amount);
int assign3(data **data_array,centroids **centroids_array,int u);
int assign2(data **data_array,centroids *temp_centroid,centroids **centroids_array,int u);
int assign(data **data_array,centroids **centroids_array,int u);
int find_distance2(char *sto_hashtable,char *apexw,int mikos,int amount);
void hamming2(char *path,int k,int L,char *query,char *output);
void quicksort(entries **entries_array,int first,int last);
void init_data_array(int amount,data **data_array,entries **entries_array);
int search(int D[1000],int new,int amount,int posa);

centroids** concentrate_init(int **distance_matrix_hamming,int *line_total,double *v,entries **entries_array,int amount,data **data_array);

int main(int argc, char** argv){

	char path[256],query[256],output[256];
	int i=0;
	int k=0,L=0;
	char gramma[50],gramma2[50];
	for(i=0;i<argc;i++){
		if(strcmp(argv[i],"-d")==0){
				strcpy(path,argv[i+1]);
				printf("%s\n",path);
		}
		if(strcmp(argv[i],"-q")==0){
				strcpy(query,argv[i+1]);
				printf("%s\n",query);
		}
		if(strcmp(argv[i],"-o")==0){
				strcpy(output,argv[i+1]);
				printf("%s\n",output);
		}
		if(strcmp(argv[i],"-k")==0){
			sscanf(argv[i+1], "%d", &k);
		}
		if(strcmp(argv[i],"-L")==0){
			sscanf(argv[i+1], "%d", &L);
		}
	}
	i=0;
	if(L==0){
		L=5;
	}
	if(k==0){
		k=4;
	}
	printf(" k : %d L : %d \n",k,L);
 	FILE* stream = fopen(path, "r");


	fscanf(stream,"%s %s ",gramma,gramma2);
	printf("%s  \n", gramma2);

	if(strcmp(gramma2,"hamming")==0){
		hamming2(path,k,L,query,output);
	}
	if(strcmp(gramma2,"matrix")==0){
		matrix(path,k,L,query,output);
	}
	if(strcmp(gramma2,"euclidean")==0){
		cosine_eucl(path, query, output, k  ,L  );
		//matrix(path,k,L,query,output);
	}
	return 1;
}


void hamming2(char *path,int k,int L,char *query,char *output){

	int j=0,mikos,i=0,o=0,z=0,bucket=0,aut=0,amount=0;
	char c;
	char buff[80];
	unsigned long long item1;
	char item[64],number[64],gramma[50],gramma2[64],result[k];
	int **tuxaioi= malloc (sizeof (int*) * k);       //////////edw apothikeuontai oi tuxaioi arithmoi gia to hash
	for(i=0;i<k;i++){
		tuxaioi[i]=(int*)malloc(sizeof(int) * L);
	}
		


 	FILE* stream = fopen(path, "r");
	FILE* fp = fopen(output, "w");
	i=0;
	fscanf(stream,"%s %s ",gramma,gramma2);
	//printf("%s  \n", gramma2);



	while(fgets (buff, LINESZ, stream)!=NULL){
		sscanf(buff,"%s %s",item,number);
		//printf( "%s \t",item);
		//printf(" %s \n",number);
		mikos=strlen(number);	///// vriskoume poso megaloi einai oi arithmoi	
		amount++;   		///////kai to plithos tous
	}
	//printf("mikos %d \n",mikos);
	//printf("amount %d \n",amount);
	char **inputs;			///// edw apothikeuontai ta dedomena apo ta datasets
	inputs=malloc(sizeof(char*)*amount);
	for(i=0;i<amount;i++){
		inputs[i]=(char*)malloc(sizeof(char)*100);
	}
	i=0;
	rewind(stream);
	fscanf(stream,"%s %s ",gramma,gramma2);
	while(j<L){ 		//////////paragontai oi tuxaioi arithmoi
		while(i<k){
			int tuxaios=rand()%(mikos+1);
			//printf("tuxaios %d \n",tuxaios);
			tuxaioi[i][j]=tuxaios;
			i++;


		}
		i=0;
		j++;
	}
	i=0;
	j=0;


	memset(result, 0, sizeof result);
	initHashtable(64,L);		/////arxikopoiisi hashtable
	for(o=0;o<amount;o++){
		if (fgets (buff, LINESZ, stream)){		////diavazoume enan ena ta dedomena
			sscanf(buff,"%s %s",item,number);
			//printf( "%s \t",item);
			//printf(" %s \n",number);
			number_ptr noumero=malloc(sizeof(number_ptr));
			noumero->name= malloc(sizeof(char) * 100);
			noumero->id= malloc(sizeof(char) * 100);
			strcpy(inputs[o],number);
			strcpy(noumero->name,number);
			strcpy(noumero->id,item);
			noumero->position=o;
			while(j<L){
			bucket=hamming_hash(k,L,number,tuxaioi,j);		/////kaloume ti sunartisi pou epistrefei se poio bucket prepei na mpei kathe entry
						//printf("bucket %d \n",bucket);
			bucketentry1( noumero , bucket ,j);		///// vazoume to entry sto antistoixo bucket
			
			j++;
			}
		j=0;
		}
	}
	
	
	int **distance_matrix_hamming=malloc(sizeof(int* )*amount);	
	for(i=0;i<amount;i++){
		distance_matrix_hamming[i]=malloc(sizeof(int)*amount);

	}
	int line_total[1000];
	int total_sum=0;
	double v[1000];	
	entries **entries_array=malloc(sizeof(entries)*amount);
	
	for(i=0;i<amount;i++){
		entries_array[i]=malloc(sizeof(entries));


	}
////// afou diavasame ola ta dedomena kaloume ti sunartisi pou dexetai ta queries
	//hamming_Search(query,k,L,tuxaioi,mikos,amount,inputs,output);
	for(i=0;i<amount;i++){
		line_total[i]=0;
		for(j=0;j<amount;j++){
			distance_matrix_hamming[i][j]=find_distance(inputs[i],inputs[j],mikos,amount);
			fprintf(fp,"%d \t",distance_matrix_hamming[i][j]);
			line_total[i]+=distance_matrix_hamming[i][j];
		}

		//printf(" sum distance %d \n",line_total[i]);
		fprintf(fp," \n");
		total_sum+=line_total[i];
		//printf("total sum %d \n",total_sum);
	}
	/*for(i=0;i<5;i++){
		for(j=0;j<amount;j++){
			printf("%d \t",distance_matrix_hamming[i][j]);

		}
			printf(" sum distance %d \n",line_total[i]);
			printf(" \n");
	}*/
	double smallest=1;

	data **data_array=malloc(sizeof(data*)*amount);
	init_data_array(amount,data_array,entries_array);
	/*for(i=0;i<5;i++){
	for(j=0;j<5;j++){
		printf(" data array %d \n",data_array[i]->distances[j]);

	}
	}*/
	centroids **centroids_array=malloc(sizeof(centroids*)*5);
	//centroids_array=concentrate_init(distance_matrix_hamming,line_total,v,entries_array,amount,data_array);
	centroids_array=plus_init(distance_matrix_hamming,data_array,amount);
	//for(i=0;i<5;i++){
		//printf("after %.15f \n",centroids_array[i]->value);
		//printf("position %d \n",centroids_array[i]->position);
	//}
	//printf("smallest %.15f \n",smallest);
	int u,total=0,h,new_total=0;
	//int temp,temp_pos;
	for(u=0;u<5;u++){

			//int kati=assign3(data_array,centroids_array,u);
			//total+=kati;
			printf(" centroid %d  \n",centroids_array[u]->position);
		//printf(" \t distance %d position %d \n ",data_array[i]->distance_from_centroid,data_array[i]->centroid_position);
	}
	/*for(u=0;u<1000;u++){
		printf(" centroid %d second %d  \n",data_array[u]->centroid_position,data_array[u]->second_centroid_position);
	}*/
	/*centroids *temp_centroid=malloc(sizeof(centroids));
	for(u=0;u<5;u++){
		for(i=0;i<10;i++){
			if(data_array[i]->is_centroid==0){
			temp_centroid->position=centroids_array[u]->position;
			temp_centroid->value=centroids_array[u]->value;
			temp_centroid->distances=centroids_array[u]->distances;
			temp_centroid->sum_distances=centroids_array[u]->sum_distances;
			temp_centroid->total=centroids_array[u]->total;
		//printf("	%d	temp centroid %d hastotal distance %d \n",i,temp_centroid->position,temp_centroid->sum_distances);
			centroids_array[u]->position=data_array[i]->position;
			centroids_array[u]->distances=data_array[i]->distances;
			centroids_array[u]->value=0;
			centroids_array[u]->sum_distances=0;
			//printf("PRE centroid %d hastotal distance %d \n",centroids_array[u]->position,centroids_array[u]->sum_distances);
			for(j=0;j<5;j++){
				int edw=assign3(data_array,centroids_array,j);
			}
			//printf("AFTER centroid %d hastotal distance %d \n",centroids_array[u]->position,centroids_array[u]->sum_distances);
			for(h=0;h<5;h++){
				new_total+=centroids_array[u]->sum_distances;
			}
			//printf("NEW TOTAL %d TOTAL %d \n",new_total,total);
			if(new_total>total){
				centroids_array[u]->position=temp_centroid->position;
				centroids_array[u]->value=temp_centroid->value;
				centroids_array[u]->distances=temp_centroid->distances;
				centroids_array[u]->sum_distances=temp_centroid->sum_distances;
				centroids_array[u]->total=temp_centroid->total;
				//centroids_array[u]=temp_centroid;
				
			}
			else {
				//data_array[temp_centroid->position]->is_centroid=0;
				data_array[i]->is_centroid=1;
				total=new_total;
			}
			new_total=0;
		}
		}
	}
	/*for(u=0;u<5;u++){
		for(i=0;i<10;i++){
			temp_centroid->position=data_array[i]->position;
			temp_centroid->value=0;
			temp_centroid->distances=data_array[i]->distances;
			temp_centroid->sum_distances=0;
			temp_centroid->total=0;
			printf("PRE centroid %d hastotal distance %d \n",centroids_array[u]->position,centroids_array[u]->sum_distances);
			int edw=assign2(data_array,temp_centroid,centroids_array,u);
			if(centroids_array[u]->sum_distances>temp_centroid->sum_distances){
				//data_array[i]->is_centroid==1;
				//data_array[centroids_array[u]->position]->is_centroid=0;
				centroids_array[u]=temp_centroid;
			}
			printf("AFTER centroid %d hastotal distance %d temp %d \n",centroids_array[u]->position,centroids_array[u]->sum_distances,temp_centroid->sum_distances);
		}
	}
	for(u=0;u<5;u++){
		printf(" centroid %d hastotal distance %d \n",centroids_array[u]->position,centroids_array[u]->sum_distances);
	}
*/
/*	for(i=0;i<k;i++){
		free(tuxaioi[i]);
	}
	free(tuxaioi);
	for(i=0;i<amount;i++){
		free(inputs[i]);
	}
	free(inputs);*/
}

int find_distance2(char *sto_hashtable,char *apexw,int mikos,int amount){
	int i=0;
	int distance=0;
	for(i=0;i<64;i++){
		if(sto_hashtable[i]!=apexw[i]){
			distance++;
		}

	}
	return distance;



}



void quicksort(entries **entries_array,int first,int last){
 	int pivot,j,i;
	entries **Array,*tmp;		
	Array=entries_array;
	if(first<last){
        	pivot=first;
        	i=first;
        	j=last;
        	while(i<j){
                	while(Array[i]->value<=Array[pivot]->value && i<last)
                 		i++;
             		while(Array[j]->value>Array[pivot]->value)
                 		j--;
             		if(i<j){
                 		tmp = Array[i];                 
				Array[i] = Array[j];			
				Array[j] = tmp;			 
            			}
         		}
			tmp=Array[pivot];
         		Array[pivot]=Array[j];
			Array[j] = tmp;	
         		quicksort(Array,first,j-1);
         		quicksort(Array,j+1,last);
	}
}


void init_data_array(int amount,data **data_array,entries **entries_array){
	int i=0;	
	for(i=0;i<amount;i++){
		data_array[i]=malloc(sizeof(data));
		data_array[i]->distance_from_centroid=100;
		data_array[i]->centroid_position=0;
		data_array[i]->distance_from_2nd_centroid=50;
		data_array[i]->second_centroid_position=0;
		data_array[i]->distances=entries_array[i]->distances;
		data_array[i]->has_been_centroid=0;
		data_array[i]->which_centroid=-1;
		data_array[i]->is_centroid=0;
		data_array[i]->position=i;
	}
	
}


int assign(data **data_array,centroids **centroids_array,int u){
	int temp,temp_pos,i;
		for(i=0;i<1000;i++){

			if(data_array[i]->distance_from_centroid>data_array[i]->distances[centroids_array[u]->position]){

				temp=data_array[i]->distance_from_centroid;
				temp_pos=data_array[i]->centroid_position;
				data_array[i]->distance_from_centroid=data_array[i]->distances[centroids_array[u]->position];
				data_array[i]->centroid_position=centroids_array[u]->position;
			//printf(" DATA array %d \n ",data_array[i]->distance_from_centroid);
				if(data_array[i]->is_centroid==0){
					centroids_array[u]->sum_distances+=data_array[i]->distance_from_centroid;
				}
				//printf("i %d sum distance %d \n",i,centroids_array[u]->sum_distances);			
				if(data_array[i]->distance_from_2nd_centroid>temp){
									
					data_array[i]->distance_from_2nd_centroid=temp;
					data_array[i]->second_centroid_position=temp_pos;}
				if(data_array[i]->which_centroid!=-1){
					centroids_array[data_array[i]->which_centroid]->sum_distances-=data_array[i]->distance_from_2nd_centroid;
				}
				data_array[i]->which_centroid=u;
				
				
			}
			else if(data_array[i]->distance_from_2nd_centroid>data_array[i]->distances[centroids_array[u]->position]){
				data_array[i]->distance_from_2nd_centroid=data_array[i]->distances[centroids_array[u]->position];
				data_array[i]->second_centroid_position=centroids_array[u]->position;
				
			}

		}
		return centroids_array[u]->sum_distances;
}

int assign2(data **data_array,centroids *temp_centroid,centroids **centroids_array,int u){
	int temp,temp_pos,i,j;
		for(i=0;i<10;i++){
		//	if(data_array[i]->is_centroid==0){
			if(data_array[i]->distances[temp_centroid->position]>data_array[i]->distances[data_array[i]->second_centroid_position]){

				temp=data_array[i]->distance_from_centroid;
				temp_pos=data_array[i]->centroid_position;
				data_array[i]->distance_from_centroid=data_array[i]->distances[data_array[i]->second_centroid_position];
				data_array[i]->centroid_position=data_array[i]->second_centroid_position;
				data_array[i]->distance_from_2nd_centroid=temp;
				data_array[i]->second_centroid_position=temp_pos;

				for(j=0;j<5;j++){
					if(centroids_array[j]->position==data_array[i]->centroid_position)
						centroids_array[j]->sum_distances+=data_array[i]->distance_from_centroid;
					if(centroids_array[j]->position==data_array[i]->second_centroid_position)
						centroids_array[j]->sum_distances-=data_array[i]->distance_from_2nd_centroid;
				}
				//printf(" DATA array %d \n ",data_array[i]->distance_from_centroid);
			}
							
			else if(data_array[i]->distances[temp_centroid->position]<data_array[i]->distances[data_array[i]->centroid_position]){
				temp=data_array[i]->distance_from_centroid;
				temp_pos=data_array[i]->centroid_position;
				//data_array[i]->distance_from_2nd_centroid=temp;
				//data_array[i]->second_centroid_position=temp_pos;
				data_array[i]->distance_from_centroid=data_array[i]->distances[temp_centroid->position];
				data_array[i]->centroid_position=temp_centroid->position;
				for(j=0;j<5;j++){
					if(centroids_array[j]->position==data_array[i]->centroid_position)
						//centroids_array[j]->sum_distances+=data_array[i]->distance_from_centroid;
						temp_centroid->sum_distances+=data_array[i]->distance_from_centroid;
					if(centroids_array[j]->position==data_array[i]->second_centroid_position)
						centroids_array[j]->sum_distances-=data_array[i]->distance_from_2nd_centroid;
				}

			//}	
			}
			printf("i %d sum distance %d temp %d \n",i,centroids_array[u]->sum_distances,temp_centroid->sum_distances);
		}
	return centroids_array[u]->sum_distances;
		
}

int assign3(data **data_array,centroids **centroids_array,int u){

	int temp,temp_pos,i,j;
		for(i=0;i<1000;i++){

			if(data_array[i]->distances[centroids_array[u]->position]>data_array[i]->distances[data_array[i]->second_centroid_position]){


				temp=data_array[i]->distance_from_centroid;
				temp_pos=data_array[i]->centroid_position;
				data_array[i]->distance_from_centroid=data_array[i]->distances[data_array[i]->second_centroid_position];
				data_array[i]->centroid_position=data_array[i]->second_centroid_position;
				data_array[i]->distance_from_2nd_centroid=temp;
				data_array[i]->second_centroid_position=temp_pos;

				for(j=0;j<5;j++){
					if(centroids_array[j]->position==data_array[i]->centroid_position)
						centroids_array[j]->sum_distances+=data_array[i]->distance_from_centroid;
					//if(centroids_array[j]->position==data_array[i]->second_centroid_position)
						//centroids_array[j]->sum_distances-=data_array[i]->distance_from_2nd_centroid;
				}
			}
			else if(data_array[i]->distances[centroids_array[u]->position]<data_array[i]->distances[data_array[i]->centroid_position]){
				temp=data_array[i]->distance_from_centroid;
				temp_pos=data_array[i]->centroid_position;
				//data_array[i]->distance_from_2nd_centroid=temp;
				//data_array[i]->second_centroid_position=temp_pos;
				data_array[i]->distance_from_centroid=data_array[i]->distances[centroids_array[u]->position];
				data_array[i]->centroid_position=centroids_array[u]->position;
				for(j=0;j<5;j++){
					if(centroids_array[j]->position==data_array[i]->centroid_position)
						centroids_array[j]->sum_distances+=data_array[i]->distance_from_centroid;
						//temp_centroid->sum_distances+=data_array[i]->distance_from_centroid;
					if(centroids_array[j]->position==data_array[i]->second_centroid_position)
						centroids_array[j]->sum_distances-=data_array[i]->distance_from_2nd_centroid;
				}
			}
			
		}
		return centroids_array[u]->sum_distances;
}




centroids** concentrate_init(int **distance_matrix_hamming,int *line_total,double *v,entries **entries_array,int amount,data **data_array){

	double smallest=1;
	int i,j;
	for(i=0;i<amount;i++){
		for(j=0;j<amount;j++){
			v[i]=(double)distance_matrix_hamming[i][j]/(double)line_total[j];
			entries_array[i]->value+=(double)distance_matrix_hamming[i][j]/(double)line_total[j];

		}	
		entries_array[i]->position=i;
		entries_array[i]->distances=distance_matrix_hamming[i];
		if(entries_array[i]->value<smallest){
			smallest=entries_array[i]->value;
		}
	}
	quicksort(entries_array,0,(amount-1));
	centroids **centroids_array=malloc(sizeof(centroids*)*5);
	for(i=0;i<5;i++){
		centroids_array[i]=malloc(sizeof(centroids));
		centroids_array[i]->value=entries_array[i]->value;
		centroids_array[i]->position=entries_array[i]->position;
		centroids_array[i]->distances=entries_array[i]->distances;
		centroids_array[i]->sum_distances=0;
		data_array[centroids_array[i]->position]->is_centroid=1;
		//printf("after %.15f \n",centroids_array[i]->value);
		//printf("position %d \n",centroids_array[i]->position);
	}
	printf("smallest %.15f \n",smallest);
	return centroids_array;
}


centroids** plus_init(int **distance_matrix_hamming,data **data_array,int amount){
	int first=rand()%(amount);
	int sum=0,i,posa=0;
	int D[1000];
	centroids **centroids_array=malloc(sizeof(centroids*)*5);
	for(i=0;i<5;i++){
		centroids_array[i]=malloc(sizeof(centroids));
	}
	centroids_array[0]->position=first;
	centroids_array[0]->distances=distance_matrix_hamming[centroids_array[0]->position];
	centroids_array[0]->sum_distances=0;
	data_array[centroids_array[0]->position]->is_centroid=1;
	D[0]=distance_matrix_hamming[centroids_array[0]->position][0];
	for(i=0;i<amount;i++){
		data_array[i]->centroid_position=centroids_array[0]->position;
		data_array[i]->distance_from_centroid=distance_matrix_hamming[i][centroids_array[0]->position];
		sum+=data_array[i]->distance_from_centroid;
		centroids_array[0]->sum_distances+=data_array[i]->distance_from_centroid;
		data_array[i]->which_centroid=0;
		if(i>0)
			D[i]=D[i-1]+data_array[i]->distance_from_centroid;
	}
	posa++;
	printf(" prwto centroid %d sum %d \n",centroids_array[0]->position,sum);
	int u=1;
	while(posa<5){
		int new=rand()%sum;

		int where=search(D,new,amount,posa);
		printf("new %d where %d \n",new,where);
		centroids_array[u]->position=where;
		centroids_array[u]->distances=distance_matrix_hamming[centroids_array[u]->position];
		centroids_array[u]->sum_distances=0;
		for(i=0;i<amount;i++){
			if(data_array[i]->distance_from_centroid>distance_matrix_hamming[i][centroids_array[u]->position]){
				centroids_array[data_array[i]->which_centroid]->sum_distances-=data_array[i]->distance_from_centroid;
				data_array[i]->centroid_position=centroids_array[u]->position;
				data_array[i]->distance_from_centroid=distance_matrix_hamming[i][centroids_array[u]->position];
				centroids_array[u]->sum_distances+=data_array[i]->distance_from_centroid;
			}
			D[0]=data_array[i]->distance_from_centroid;
			if(i>0)
				D[i]=D[i-1]+data_array[i]->distance_from_centroid;
			
		}
		sum=D[amount-posa];
		u++;
		posa++;

	}
	return centroids_array;
}



int search(int D[1000],int new,int amount,int posa){
	int i;
	for(i=0;i<(amount-posa);i++){
		if(new>D[i] && new<=D[i+1]){
			return i;
		}
	}


}
