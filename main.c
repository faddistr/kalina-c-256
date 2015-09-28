#include "dstu/kalina_256_256.h"
#include "crypto.h"
#include <stdlib.h>



typedef struct
{
    uint64_t            round_keys[ ((kalina_256_256_rounds_num+1)*kalina_256_64_key_len)];
    uint8_t             big_table[16384];
    tkalina_256         kalina;
    bool                is_key;
    bool                is_iv;
}tcrypto_private;


tcrypto_error crypto_init        (void **handler,    void *s);
tcrypto_error crypto_set_key     (void *handler,     void *k);
tcrypto_error crypto_set_iv      (void *handler,     void *i);
tcrypto_error crypto_encrypt     (void *handler,     void *out, void *in, const uint32_t size, const bool last);
tcrypto_error crypto_decrypt     (void *handler,     void *out, void *in, const uint32_t size, const bool last);
void          crypto_end         (void *handler);


tcrypto_error crypto_init(void **handler,void *s){
    tcrypto_private *ptr;


    if(s==NULL){
        return CRYPTO_BAD_ARG;
    }

    ptr=*handler=malloc(sizeof(tcrypto_private));

    if(ptr==NULL){
        return CRYPTO_NO_MEM;
    }

    kalina_make_awesome_table(ptr->big_table, s);
    ptr->is_key=false;
    ptr->is_iv=false;

    return CRYPTO_OK;
}

void crypto_end(void *handler){
    if(handler){
        free(handler);
    }
}

tcrypto_error crypto_set_key(void *handler, void *k){
    tcrypto_private *ptr;

    if( handler==NULL ){
        return CRYPTO_BAD_ARG;
    }
    ptr=(tcrypto_private *)handler;

    kalina_256_256_generate_round_keys(ptr->round_keys,k,ptr->big_table);
    ptr->is_key=true;
    ptr->is_iv=false;

    return CRYPTO_OK;
}

tcrypto_error crypto_set_iv(void *handler, void *iv){
    tcrypto_private *ptr;

    if( handler==NULL ){
        return CRYPTO_BAD_ARG;
    }

    ptr=(tcrypto_private *)handler;

    if( !ptr->is_key ){
        return CRYPTO_NO_KEY;
    }

    kalina_256_256_prepare(&ptr->kalina,iv,ptr->round_keys,ptr->big_table);
    ptr->is_iv=true;

    return CRYPTO_OK;
}

tcrypto_error crypto_encrypt     (void *handler, void *out, void *in, const uint32_t size, const bool last){
    tcrypto_private *ptr;

    if( handler==NULL ){
        return CRYPTO_BAD_ARG;
    }
    ptr=(tcrypto_private *)handler;

    if( !ptr->is_key ){
        return CRYPTO_NO_KEY;
    }

    if( !ptr->is_iv ){
        return CRYPTO_NO_IV;
    }

    kalina_256_256_CFB_E(&ptr->kalina,out,in,size,0,last);

    return CRYPTO_OK;
}

tcrypto_error crypto_decrypt     (void *handler, void *out, void *in, const uint32_t size, const bool last){
    tcrypto_private *ptr;

    if( handler==NULL ){
        return CRYPTO_BAD_ARG;
    }
    ptr=(tcrypto_private *)handler;

    if( !ptr->is_key ){
        return CRYPTO_NO_KEY;
    }

    if( !ptr->is_iv ){
        return CRYPTO_NO_IV;
    }

    kalina_256_256_CFB_D(&ptr->kalina,out,in,size,0,last);

    return CRYPTO_OK;
}
