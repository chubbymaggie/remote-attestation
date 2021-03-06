/*
 *	DO_HASH BLOCK IMPLEMENTATION (SHA1 VERSION)
 */

#include "ra_do_hash.h"
#include "ra_defines.h"

#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>

/*
 * 		buffer, allocated here, must be free'd elsewhere.
 * 		If error returned buffer is freed here
 */
RA_RESULT ra_do_hash_NAYjDD3l2s(
        uint8_t *nonce_buffer,
        uint32_t nonce_size,
        uint8_t *data_buffer,
        uint32_t data_size,
        uint8_t *AID,
        uint64_t attestator_number,
        void *key,
        uint8_t key_size,
        uint8_t **packed_data,
        uint32_t *packed_data_size) {


    size_t buffer_to_hash_size;
    //ra_printf("(%s) Doing sha1 hash\n", __FUNCTION__);


    buffer_to_hash_size = nonce_size + data_size + AID_SIZE +sizeof(attestator_number);
    uint8_t *buffer_to_hash = (uint8_t *) malloc(buffer_to_hash_size);
    if (buffer_to_hash == NULL) return RA_ERROR_MEMORY;

    if (memcpy(buffer_to_hash, nonce_buffer, nonce_size) == NULL) {
        *packed_data_size = 0;
        free(buffer_to_hash);
        return RA_ERROR_MEMORY;
    }
    if (memcpy(buffer_to_hash + nonce_size, data_buffer, data_size) == NULL) {
        *packed_data_size = 0;
        free(buffer_to_hash);
        return RA_ERROR_MEMORY;
    }
    if (memcpy(buffer_to_hash + nonce_size + data_size, AID, AID_SIZE) == NULL) {
        *packed_data_size = 0;
        free(buffer_to_hash);
        return RA_ERROR_MEMORY;
    }

    if (memcpy(buffer_to_hash + nonce_size + data_size + AID_SIZE, &attestator_number, sizeof(attestator_number)) ==
        NULL) {
        *packed_data_size = 0;
        free(buffer_to_hash);
        return RA_ERROR_MEMORY;
    }


    if ((*packed_data = (uint8_t *) malloc(SHA_DIGEST_LENGTH)) == NULL) {
        *packed_data_size = 0;
        free(buffer_to_hash);
        return RA_ERROR_MEMORY;
    }


    SHA1(buffer_to_hash, buffer_to_hash_size, *packed_data);


    free(buffer_to_hash);
    *packed_data_size = SHA_DIGEST_LENGTH;
    return RA_SUCCESS;
}
