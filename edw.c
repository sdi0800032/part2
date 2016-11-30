






cos_items_ptr items;
char *filename;

// an dwsei arxeio eukleidia kane auto

***
items = store_vectors( path , NULL , NULL , 0 );
filename = store_distances(  items, items->current_items);			// to filename einai to arxeio csv me to distance matrix
***

// to print mh to valeis , tsekare to mia fora gia test mono
// printf("output file is %s \n" , filename);



oi domes pou prp na oristoun gia na leitourghsei einai 

struct COS_items {
	double **item_array;
	int current_items;
	int current_dimension;
	int total_dimensions;
	int *centroid;
	int *temp_centroid;
	double *distance_to_centroid;
};

typedef struct COS_items *cos_items_ptr ;
cos_items_ptr store_vectors(char * , char * , char * , int );
void store_item_coords( double  , cos_items_ptr  ) ;
cos_items_ptr allocate_space_items( int );
char* store_distances( cos_items_ptr , int );







//kai oi ulopoihseis einai autes (to menu de to 3anavala)


cos_items_ptr allocate_space_items( int num ){					// allocate space for the given vectors in the DATAFILE	
	cos_items_ptr ptr;
	int elements , i;
	if(num>0) elements = num;
	else{
		printf("invalid number of items \n");
		return NULL;
	}
	ptr = malloc(sizeof (struct COS_items));
	ptr->item_array = malloc(sizeof(double) * elements );
	ptr->centroid = malloc(sizeof(int) * elements );
	ptr->temp_centroid = malloc(sizeof(int) * elements );
	ptr->distance_to_centroid = malloc(sizeof(double)*elements);
	for(i=0; i<elements; i++){
		ptr->item_array[i] = malloc(sizeof(double) * 5000 );		
		ptr->distance_to_centroid[i] = 100000000;
		ptr->centroid[i] = -1;
	}
	ptr->current_items = 0;
	ptr->current_dimension = 0;
	return ptr;
}


char* store_distances( cos_items_ptr items, int elements){
	int i,j,dim , val;
	FILE *fp;
	char *name = "Distance_Matrix.csv" ;
	fp = fopen( name , "w+");
	fprintf(fp , "%s" , "@metric_space matrix\n");
	fprintf(fp , "%s" , "@items\n");
	for(i=0; i<elements; i++){
		if(i==elements-1) {
			fprintf(fp , "%s%d" , "item" , i+1 );
			break;
		}
		fprintf(fp , "%s%d%s" , "item" , i+1 , ",");
	}
	fprintf(fp , "%s" , "\n");
	double distance = 0.0;
	for(i=0; i<elements; i++){
		for(j=0; j<elements; j++){
			distance = 0.0;
			for(dim = 0; dim<items->total_dimensions; dim++){
				distance += pow( (items->item_array[i][dim] - items->item_array[j][dim])  , 2 ); 
			}
			distance = sqrt(distance);
			distance += 0.5;
			val = distance;
			fprintf(fp , "%d%s" , val , "	");
		}
		fprintf(fp , "%s" , "\n");
	}
	fclose(fp);
	return name;
}
void store_item_coords( double num , cos_items_ptr ptr ){
	ptr->item_array[ptr->current_items][ptr->current_dimension] = num;
	ptr->current_dimension ++ ;
}



cos_items_ptr store_vectors(char *path , char *query_file , char *output_file , int K ){
	cos_items_ptr items_ptr;
	double item_coords;
	int elements=-2 ;				// starting at -2 to ignore the 2 titles in the euclidean files
	char * line = NULL;
    size_t len = 0;
    ssize_t read;
	char *token;
    printf("%s\n",path);
    FILE* fp ;
    fp = fopen(path, "r");
    if( fp == NULL) {
		printf("error opening file , exiting ... \n");
	}
	while ((read = getline(&line, &len, fp)) != -1) {		// searched how many items are in file to dynamically allocate array of ''element'' size
		elements++;
	}
	printf("Items in file :%d\n",elements);					
	items_ptr = allocate_space_items( elements * 2) ;
	printf("allocating space for %d elements..\n" , elements);
	fclose(fp);
	fp = fopen(path, "r");
    if( fp == NULL) {
		printf("error opening file , exiting ... \n");
	}
	read = getline(&line, &len, fp);
	read = getline(&line, &len, fp);			
	 while ((read = getline(&line, &len, fp)) != -1) {
        token = strtok(line, "	");					
        while( token != NULL ) {
			token = strtok(NULL, "	");
			if(token!=NULL){
				item_coords = atof( token );				
				store_item_coords( item_coords , items_ptr );			// save item coords in our structure
			}
		}
		items_ptr->current_items ++; 
		items_ptr->total_dimensions = items_ptr->current_dimension; 
		items_ptr->current_dimension = 0;
    }
    return items_ptr;

}






