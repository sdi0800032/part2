int HT_CreateIndex( char *fileName, char attrType, char* attrName, int attrLength, int buckets) {
    char filetype[10] = "static";
    int fd ;
    long unsigned int position = 0;
	void *block;
    printf("size of block is %d bytes\n" , block_size);
    printf("size of record is %lu bytes \n" , sizeof(Record));
    printf("each block can contain up to %lu records (saving some bytes for info)\n" , (block_size - sizeof(Record))/sizeof(Record));		// keep some bytes in ea. block to store additional info
    printf("Requested buckets : %d  \n" , buckets );
    
	BF_Init();
	if( BF_CreateFile(fileName) < 0 ) BF_PrintError("error while creating file");
	fd=BF_OpenFile(fileName);
	if( BF_AllocateBlock(fd)<0) BF_PrintError("error while allocating block");
		
	if( BF_ReadBlock(fd, 0, &block)< 0 ) BF_PrintError("error while reading block");
	//save info in first block
	memcpy( block + position , filetype , 10*sizeof(char));
	position += 10*sizeof(char) ;
	memcpy( block + position , &attrType , sizeof(char));
	position+= sizeof(char);
	memcpy( block + position  , attrName , 15*sizeof(char));
	position+= 15*sizeof(char);
	memcpy( block + position  , &attrLength , sizeof(int));
	position+= sizeof(int);
	memcpy( block + position  , &buckets , sizeof(int));
	position+= sizeof(int);
	//done saving 
	
	if( BF_WriteBlock( fd , 0) < 0 ) BF_PrintError("error while writing block");
	if(BF_CloseFile(fd) < 0 ) BF_PrintError("error while closing file");
    return -1; 
}


unsigned long hash_function( char *field1 , int field2  , HT_info *hptr){
	unsigned long result = 5381;
	int c;
	printf("%s field 1 \n" , field1);
	if( field1 == NULL ){
		// we're given an integer to process
		result = field2 % ( hptr->numBuckets );
	}else{
		// we're given a string to process
		// (from stackoverflow)
		while ( c = *field1++ ) {
			result = ((result << 5) + result) + c; 				/* hash * 33 + c */
		}
		result = result % ( hptr->numBuckets );
	}
	return result;
}

HT_info* HT_OpenIndex(char *fileName) {
	char filetype[20];
    HT_info *static_info;
    int fd , size_needed , blocks_needed , current_size , i , j , ptr;
    long unsigned int position = 0 , capacity;
    void *block;
    static_info = malloc(sizeof(HT_info));
    static_info->attrName = malloc(sizeof(char)*15);
    fd=BF_OpenFile(fileName);
    if( BF_ReadBlock(fd, 0, &block)< 0 ) BF_PrintError("error while reading block");
    
    //get filetype
    memcpy( filetype , block+position, 10*sizeof(char));
    position += 10*sizeof(char);
    printf("type of file is : %s \n " , filetype);
    if(!strcmp(filetype,"static\n")){
		printf("wrong type of file , exiting \n");
		return NULL;
	}
    //store key type
	memcpy( &(static_info->attrType) , block+position ,sizeof(char));
	position += sizeof(char);
	//store attrname
	memcpy( static_info->attrName , block+position , 15*sizeof(char));
	position += 15*sizeof(char);
	//store key length
	memcpy( &(static_info->attrLength) , block+position , sizeof(int));
	position += sizeof(int);
	//store requested buckets
	memcpy( &(static_info->numBuckets) , block+position , sizeof(int));
	position += sizeof(int);
	//store fd
	static_info->fileDesc = fd;
	//compute how many blocks we'll need to store the "hash table" 
	size_needed = sizeof(int) * static_info->numBuckets ;
	if(size_needed < block_size) blocks_needed = 1;
	else{
		blocks_needed = 1;
		current_size = block_size;
		while( current_size < size_needed){
			current_size += block_size;
			blocks_needed ++;
		}
	}
	printf("we will need %d blocks to store our hash table \n" , blocks_needed);
	// allocate blocks to hold the hash table , range of id blocks is : [1 , blocks_needed(for hashtable)] ( 0 is holding basic info )
	for(i=0; i<blocks_needed; i++){
		if( BF_AllocateBlock(fd)<0) BF_PrintError("error while allocating block");
	}

	// allocate all essential starting buckets ( none overflow yet )
	// range of id buckets is : [blocks_needed(for hashtable) + 1 , total_buckets + blocks_needed(for hashtable) + 1]
	for(i=0; i<static_info->numBuckets; i++){
		if( BF_AllocateBlock(fd)<0) BF_PrintError("error while allocating block");	
	}
	capacity = block_size / sizeof(int) ; // how many pointers (int values) can contain each block
	
	ptr = blocks_needed + 1;
	// link hashtable to all initial blocks
	for(i=1; i<=blocks_needed; i++){
		position = 0;
		if( BF_ReadBlock(fd, i  , &block)< 0 ) BF_PrintError("error while reading block");
		for(j=0; j<capacity; j++){
			memcpy(block+position, &ptr , sizeof(int));
			position+= sizeof(int);
			ptr++;
			if(ptr > static_info->numBuckets) break;
		}
		if( BF_WriteBlock( fd , i) < 0 ) BF_PrintError("error while writing block");
	}
    return static_info;
} 


int HT_CloseIndex( HT_info* header_info ) {
    if(header_info->fileDesc){
		printf("closing file...\n");
	}
	if(BF_CloseFile(header_info->fileDesc) < 0 ) BF_PrintError("error while closing file");
	free((header_info->attrName));
	free(header_info);
    
    return -1;
    
}
