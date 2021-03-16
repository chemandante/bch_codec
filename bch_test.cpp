//#include <iostream>
//#include <algorithm>
#include <vector>
//#include <stdlib.h>
#include "bch_codec.h"
//#include <string.h>
using namespace std;

#ifdef BCH_USE_OWN_PRINTF
	extern "C" int bch_printf(const char * __restrict _format, ...);
	#define printf		bch_printf
#endif

namespace {
    vector<unsigned int> randperm(int n,int k) // like the matlab/octave function
    {
        vector<unsigned int> res;
        if (2*k > n) {
            res.resize(n);
            for (int i=0;i<n;++i) 
                res[i] = i;
            random_shuffle( res.begin(),res.end());
            res.resize(k);
        }else{
            res.reserve(k);
            for (int i=0;i<k;++i) {
                int next = rand() % n;
                while ( find(res.begin(),res.end(),next) != res.end() ) {
                    next = rand() % n;
                }
                res.push_back(next);
            }
        }
        return res;
    }
}

int _bch_test(int m,int t,unsigned int prim_poly,int ntrials)
{

    bch_control * bch = init_bch(m,t,prim_poly);
    printf("initialized m = %d, n = %d, t = %d, ecc_bits = %d, ecc_bytes = %d\n", bch->m, bch->n, bch->t, bch->ecc_bits, bch->ecc_bytes);

    int N = (1<<m)-1;
    int msgBits = N - bch->ecc_bits;

    printf("running %d trials of BCH decoding with up to %d errors\n", ntrials, t);
    vector<uint8_t> data( msgBits / 8 );
    vector<uint8_t> dataClean;
    printf("data.size() = %d\n", data.size());

    for (int trial=0;trial < ntrials;++trial) {
        // make a random message
        for (size_t k=0;k<data.size();++k)
            data[k] = rand()&0xFF;
        dataClean = data;

        // encode it
        vector<uint8_t> ecc( bch->ecc_bytes ,0);
        encode_bch(bch, &data[0],data.size(),&ecc[0]);

        // introduce up to t errors
        int nerrs = rand() % (t+1);
        vector<unsigned int> errLocIn = randperm( data.size()*8 ,nerrs );
        for (size_t k=0;k < errLocIn.size();++k) {
           int i = errLocIn[k];
           data[i>>3] ^= (1<<(i&7));
        }

        // decode and make sure the right errors were corrected
        vector<unsigned int> errLocOut(t);
        int nerrFound = decode_bch(bch, &data[0], data.size(),&ecc[0], NULL,NULL,&errLocOut[0]);
        if (nerrFound != nerrs ) {
            printf("decode_bch return value = %d, expected %d\n", nerrFound, nerrs);
            if (nerrFound<0)
                printf(strerror(-nerrFound));
            	printf("\n");
            return 1;
        }
        errLocOut.resize(nerrFound);

        sort( errLocIn.begin(),errLocIn.end());
        sort( errLocOut.begin(),errLocOut.end());
        if (errLocIn != errLocOut) {
        	printf("Input Errors!= Found Errors !!!!\n");
            return 1;
        }

        correct_bch(bch,&data[0],data.size(),&errLocOut[0],nerrFound);

        if (dataClean != data) {
        	printf("data not corrected\n");
            return 1;
        }
    }

    free_bch(bch);
    return 0;
}
