#include<iostream>
#include<gmp.h>
#include<stdlib.h>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <malloc.h>
#include <unistd.h>
#include <cstring>

using namespace std;

int main(int argc,char **argv)
{
    cout << "Encrypting ..." << endl;
    char* encrypted_file = (char *) malloc(strlen(argv[1])+4);
    strcpy(encrypted_file, argv[1]);
    strcpy(encrypted_file+strlen(argv[1]), "___");

    mpz_t msg,cipher,n,e;
    mpz_init(msg);
    mpz_init(cipher);
    mpz_init(n);
    mpz_init(e);
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
    mpz_set_str(e,e_str,10);


    FILE *f,*ef;
    f = fopen(argv[1],"rb");
    ef = fopen(encrypted_file,"wb");
    int block_size = mpz_sizeinbase(n, 2) / 8;
    char* block = (char *) malloc(block_size);
    int i, read_count;
    do
    {
        for (i=0; i<block_size; i++) block[i] = 0;
        read_count = fread(block, 1, block_size - 1, f);
        mpz_import(msg, block_size, -1, sizeof(block[0]), 1, 0, block);
        mpz_powm(cipher, msg, e, n);
        for (i=0; i<block_size; i++) block[i] = 0;
        size_t _;
        mpz_export(block, &_, -1, sizeof(block[0]), 1, 0, cipher);
        fwrite(block, 1, block_size, ef);
    }
    while (read_count == block_size - 1);

    mpz_set_ui(cipher, read_count);
    mpz_out_str(ef, 16, cipher); // no of bytes of last block

    mpz_clear(e);
    mpz_clear(n);
    mpz_clear(msg);
    mpz_clear(cipher);

    fclose(f);
    fclose(ef);
    free(block);

    remove(argv[1]);

    return 0;
}
