#include<iostream>
#include<gmp.h>
#include<stdlib.h>
#include <stdio.h>
#include <fstream>

using namespace std;

int main(){
    cout << "Generating keys .... " << endl;
    mpz_t p,q,n,fi,e,d,temp;
    mpz_init(p);
    mpz_init(q);
    mpz_init(n);
    mpz_init(fi);
    mpz_init(e);
    mpz_init(d);
    mpz_init(temp);
    int length=1024;
    char e_str[1024];
    char d_str[1024];
    char n_str[2048];

    char *str = new char[length+1];
    str[0] = '1';
    str[length] = '\0';

    //finding p
    for(int i=1;i<length;i++){
        str[i] = char((rand()%2)+48);
    }
    mpz_set_str(p,str,2);
    mpz_nextprime(p,p);

    //finding q
    for (int i=1;i<length;i++){
        str[i] = char((rand()%2)+48);
    }
    mpz_set_str(q,str,2);
    mpz_nextprime(q,q);

    //calculate n = p * q
    mpz_mul(n,p,q);

    //calculate fi
    mpz_sub_ui(p,p,1);
    mpz_sub_ui(q,q,1);
    mpz_mul(fi,p,q);


    //calculate e
    mpz_set_ui(e,3);
    while(1){
        mpz_gcd(temp,e,fi);
        if (!mpz_cmp_ui(temp,1))
            break;
        mpz_nextprime(e,e);
    }

    //calculate private key
    mpz_invert(d,e,fi);

    mpz_get_str(n_str,10,n);
    mpz_get_str(e_str,10,e);
    mpz_get_str(d_str,10,d);

    ofstream file;
    file.open("keys.txt",std::ios_base::app);
    file << n_str << " " << e_str << " " << d_str << endl;
    file.close();
    cout << "keys successfully generated.";
    return 0;
}
