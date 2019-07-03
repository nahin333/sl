#include<iostream>
#include<gmp.h>
#include<stdlib.h>
#include <stdio.h>
#include <fstream>
#include<unistd.h>
#include <malloc.h>
#include<cstring>

using namespace std;

int main(int argc,char **argv)
{
    cout << "Decrypting ....";
    char* decrypted_file = (char *) malloc(strlen(argv[1]));
    char* encrypted_file = (char *) malloc(strlen(argv[1])+4);
    strcpy(encrypted_file, argv[1]);
    strcpy(decrypted_file, argv[1]);
    strcpy(encrypted_file+strlen(argv[1]), "___");


    mpz_t cipher,decrypted_msg,n,d;

    mpz_init(cipher);
    mpz_init(decrypted_msg);
    mpz_init(n);
    mpz_init(d);
    char dummy[8200];
    int filesize = 38671;

    char n_str[2048];
    char e_str[2048];
    char d_str[2048];
    char temp[2048];
    char name[50];
    char pass[50];


    ifstream file;
    file.open("keys.txt");

    while(1)
    {
        file >> name >> pass >> n_str >> e_str >> d_str;
        if (!strcmp(name,argv[2]))
        {
            break;
        }
    }

    file.close();
    mpz_set_str(n,n_str,10);
    mpz_set_str(d,d_str,10);



    FILE *f,*df;
    f = fopen(encrypted_file,"rb");
    df = fopen(decrypted_file,"wb");
    int block_size = mpz_sizeinbase(n, 2) / 8; // no of bytes
    char* block = (char *) malloc(block_size);
    int i, read_count;
    for (i=0; i<block_size; i++) block[i] = 0;
    while ((read_count = fread(block, 1, block_size, f)) == block_size)
    {
        mpz_import(cipher, block_size, -1, sizeof(block[0]), 1, 0, block);
        mpz_powm(decrypted_msg, cipher, d, n);
        size_t _;
        for (i=0; i<block_size; i++) block[i] = 0;
        mpz_export(block, &_, -1, sizeof(block[0]), 1, 0, decrypted_msg);
        fwrite(block, 1, block_size-1, df);
        for (i=0; i<block_size; i++) block[i] = 0;
    }
    mpz_set_str(decrypted_msg, block, 16);
    read_count = mpz_get_ui(decrypted_msg);
    fseek(df,-(block_size - 1 - read_count),SEEK_END);
    ftruncate(fileno(df), ftell(df));

    mpz_clear(d);
    mpz_clear(n);
    mpz_clear(decrypted_msg);
    mpz_clear(cipher);

    fclose(f);
    fclose(df);
    free(block);

    remove(encrypted_file);

    return 0;
}
