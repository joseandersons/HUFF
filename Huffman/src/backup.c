_Bool write_bit_stream(int compressed_file, int fd, char **table){
	unsigned char character, byteFile = 0;
    int j = 0, bit_amount = 0, byte_size = 7;

    while(1){
    	ssize_t size = read(fd, &character, 1);
    	if(size <= 0)
    		break;

        j = 0;
        while(table[character][j] != '\0'){
            if(table[character][j] != '0'){
                byteFile = set_bit(byteFile, byte_size);
            }
            
            bit_amount++;
            byte_size--;
            j++;

            if(bit_amount == 8){
                write(compressed_file, &byteFile, 1);
                byteFile = 0;
                byte_size = 7;
                bit_amount = 0;
            }
        }
    }

    if(bit_amount != 0){
        write(compressed_file, &byteFile, 1);
    }

    return 1;
}