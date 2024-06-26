/**************************************************************************//**
 * @file     crypto.h
 * @version  V1.10
 * @brief    Cryptographic Accelerator driver header file
 *
 * @copyright (C) 2017 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#ifndef __CRYPTO_H__
#define __CRYPTO_H__

#ifdef __cplusplus
extern "C"
{
#endif

/** @addtogroup Standard_Driver Standard Driver
  @{
*/

/** @addtogroup CRYPTO_Driver CRYPTO Driver
  @{
*/

/** @addtogroup CRYPTO_EXPORTED_CONSTANTS CRYPTO Exported Constants
  @{
*/

#define PRNG_KEY_SIZE_128       0UL     /*!< PRNG Key size 128 bits                  \hideinitializer */
#define PRNG_KEY_SIZE_163       1UL     /*!< PRNG Key size 163 bits                  \hideinitializer */
#define PRNG_KEY_SIZE_192       2UL     /*!< PRNG Key size 192 bits                  \hideinitializer */
#define PRNG_KEY_SIZE_224       3UL     /*!< PRNG Key size 224 bits                  \hideinitializer */
#define PRNG_KEY_SIZE_233       4UL     /*!< PRNG Key size 233 bits                  \hideinitializer */
#define PRNG_KEY_SIZE_255       5UL     /*!< PRNG Key size 255 bits                  \hideinitializer */
#define PRNG_KEY_SIZE_256       6UL     /*!< PRNG Key size 256 bits                  \hideinitializer */
#define PRNG_KEY_SIZE_283       7UL     /*!< PRNG Key size 283 bits                  \hideinitializer */
#define PRNG_KEY_SIZE_384       8UL     /*!< PRNG Key size 384 bits                  \hideinitializer */
#define PRNG_KEY_SIZE_409       9UL     /*!< PRNG Key size 409 bits                  \hideinitializer */
#define PRNG_KEY_SIZE_512       10UL    /*!< PRNG Key size 512 bits                  \hideinitializer */
#define PRNG_KEY_SIZE_521       11UL    /*!< PRNG Key size 521 bits                  \hideinitializer */
#define PRNG_KEY_SIZE_571       12UL    /*!< PRNG Key size 571 bits                  \hideinitializer */

#define PRNG_SEED_CONT          0UL     /*!< PRNG using current seed                 \hideinitializer */
#define PRNG_SEED_RELOAD        1UL     /*!< PRNG reload new seed                    \hideinitializer */

#define AES_KEY_SIZE_128        0UL     /*!< AES select 128-bit key length           \hideinitializer */
#define AES_KEY_SIZE_192        1UL     /*!< AES select 192-bit key length           \hideinitializer */
#define AES_KEY_SIZE_256        2UL     /*!< AES select 256-bit key length           \hideinitializer */
#define AES_KEY_SIZE_Pos        (2)     /*!< AES key size selection vit field position   \hideinitializer */

#define AES_MODE_DECRYPT        0       /*!< AES decrypt mode \hideinitializer */
#define AES_MODE_ENCRYPT        1       /*!< AES encrypt mode \hideinitializer */

#define AES_MODE_ECB            0UL     /*!< AES select ECB mode                     \hideinitializer */
#define AES_MODE_CBC            1UL     /*!< AES select CBC mode                     \hideinitializer */
#define AES_MODE_CFB            2UL     /*!< AES select CFB mode                     \hideinitializer */
#define AES_MODE_OFB            3UL     /*!< AES select OFB mode                     \hideinitializer */
#define AES_MODE_CTR            4UL     /*!< AES select CTR mode                     \hideinitializer */
#define AES_MODE_CBC_CS1        0x10UL  /*!< AES select CBC CS1 mode                 \hideinitializer */
#define AES_MODE_CBC_CS2        0x11UL  /*!< AES select CBC CS2 mode                 \hideinitializer */
#define AES_MODE_CBC_CS3        0x12UL  /*!< AES select CBC CS3 mode                 \hideinitializer */
#define AES_MODE_GCM            0x20UL  /*!< AES select GCM (Galois/Counter Mode)    \hideinitializer */
#define AES_MODE_GHASH          0x21UL  /*!< AES select GHASH (Galois Hash Function) \hideinitializer */
#define AES_MODE_CCM            0x22UL  /*!< AES select CCM (Counter with CBC-MAC Mode)   \hideinitializer */
#define AES_OPMODE_Pos          (8)     /*!< AES operation mode bit filed position   \hideinitializer */
#define AES_OPMODE_Msk          (0xfful << AES_OPMODE_Pos) /*!< AES operation mode bit filed mask   \hideinitializer */

#define AES_NO_SWAP             0UL     /*!< AES MA35 not swap input and output data   \hideinitializer */
#define AES_OUT_SWAP            1UL     /*!< AES swap output data                    \hideinitializer */
#define AES_IN_SWAP             2UL     /*!< AES swap input data                     \hideinitializer */
#define AES_IN_OUT_SWAP         3UL     /*!< AES swap both input and output data     \hideinitializer */

#define SHA_MODE_SEL_SHA1       0UL     /*!< SHA engine select SHA1                  \hideinitializer */
#define SHA_MODE_SEL_SHA2       0UL     /*!< SHA engine select SHA2                  \hideinitializer */
#define SHA_MODE_SEL_SHA3       1UL     /*!< SHA engine select SHA3                  \hideinitializer */
#define SHA_MODE_SEL_SM3        2UL     /*!< SHA engine select SM3                   \hideinitializer */
#define SHA_MODE_SEL_MD5        4UL     /*!< SHA engine select MD5                   \hideinitializer */

#define SHA_MODE_SHA1           0UL     /*!< SHA select SHA-1 160-bit                \hideinitializer */
#define SHA_MODE_SHA224         5UL     /*!< SHA select SHA-224 224-bit              \hideinitializer */
#define SHA_MODE_SHA256         4UL     /*!< SHA select SHA-256 256-bit              \hideinitializer */
#define SHA_MODE_SHA384         7UL     /*!< SHA select SHA-384 384-bit              \hideinitializer */
#define SHA_MODE_SHA512         6UL     /*!< SHA select SHA-512 512-bit              \hideinitializer */
#define SHA_MODE_SHAKE128       0UL     /*!< SHA select SHA-3 SHAKE128               \hideinitializer */
#define SHA_MODE_SHAKE256       1UL     /*!< SHA select SHA-3 SHAKE256               \hideinitializer */

#define SHA_NO_SWAP             0UL     /*!< SHA MA35 not swap input and output data   \hideinitializer */
#define SHA_OUT_SWAP            1UL     /*!< SHA swap output data                    \hideinitializer */
#define SHA_IN_SWAP             2UL     /*!< SHA swap input data                     \hideinitializer */
#define SHA_IN_OUT_SWAP         3UL     /*!< SHA swap both input and output data     \hideinitializer */

#define CRYPTO_DMA_FIRST        0x4UL   /*!< MA35 first encrypt/decrypt in DMA cascade \hideinitializer */
#define CRYPTO_DMA_ONE_SHOT     0x5UL   /*!< MA35 one shot encrypt/decrypt with DMA      \hideinitializer */
#define CRYPTO_DMA_CONTINUE     0x6UL   /*!< MA35 continuous encrypt/decrypt in DMA cascade \hideinitializer */
#define CRYPTO_DMA_LAST         0x7UL   /*!< MA35 last encrypt/decrypt in DMA cascade          \hideinitializer */

/*! ECC curve   */
typedef enum
{
	/*!< ECC curve                \hideinitializer */
	CURVE_P_192  = 0x01,                /*!< ECC curve P-192          \hideinitializer */
	CURVE_P_224  = 0x02,                /*!< ECC curve P-224          \hideinitializer */
	CURVE_P_256  = 0x03,                /*!< ECC curve P-256          \hideinitializer */
	CURVE_P_384  = 0x04,                /*!< ECC curve P-384          \hideinitializer */
	CURVE_P_521  = 0x05,                /*!< ECC curve P-521          \hideinitializer */
	CURVE_K_163  = 0x11,                /*!< ECC curve K-163          \hideinitializer */
	CURVE_K_233  = 0x12,                /*!< ECC curve K-233          \hideinitializer */
	CURVE_K_283  = 0x13,                /*!< ECC curve K-283          \hideinitializer */
	CURVE_K_409  = 0x14,                /*!< ECC curve K-409          \hideinitializer */
	CURVE_K_571  = 0x15,                /*!< ECC curve K-571          \hideinitializer */
	CURVE_B_163  = 0x21,                /*!< ECC curve B-163          \hideinitializer */
	CURVE_B_233  = 0x22,                /*!< ECC curve B-233          \hideinitializer */
	CURVE_B_283  = 0x23,                /*!< ECC curve B-283          \hideinitializer */
	CURVE_B_409  = 0x24,                /*!< ECC curve B-409          \hideinitializer */
	CURVE_B_571  = 0x25,                /*!< ECC curve K-571          \hideinitializer */
	CURVE_KO_192 = 0x31,                /*!< ECC 192-bits "Koblitz" curve   \hideinitializer */
	CURVE_KO_224 = 0x32,                /*!< ECC 224-bits "Koblitz" curve   \hideinitializer */
	CURVE_KO_256 = 0x33,                /*!< ECC 256-bits "Koblitz" curve   \hideinitializer */
	CURVE_BP_256 = 0x41,                /*!< ECC Brainpool 256-bits curve   \hideinitializer */
	CURVE_BP_384 = 0x42,                /*!< ECC Brainpool 256-bits curve   \hideinitializer */
	CURVE_BP_512 = 0x43,                /*!< ECC Brainpool 256-bits curve   \hideinitializer */
	CURVE_SM2_256 = 0x50,               /*!< ECC SM2 curve            \hideinitializer */
	CURVE_25519  = 0x51,
	CURVE_UNDEF,                        /*!< Invalid curve            \hideinitializer */
}
E_ECC_CURVE;                            /*!< ECC curve                \hideinitializer */

/*! @}*/ /* end of group CRYPTO_EXPORTED_CONSTANTS */

/** @addtogroup CRYPTO_EXPORTED_MACROS CRYPTO Exported Macros
  @{
*/

/*----------------------------------------------------------------------------------------------*/
/*  Macros                                                                                      */
/*----------------------------------------------------------------------------------------------*/

/**
  * @brief This macro enables PRNG interrupt.
  * @param crpt     Specified crypto module
  * @return None
  * \hideinitializer
  */
#define PRNG_ENABLE_INT(crpt)       ((crpt)->INTEN |= CRPT_INTEN_PRNGIEN_Msk)

/**
  * @brief This macro disables PRNG interrupt.
  * @param crpt     Specified crypto module
  * @return None
  * \hideinitializer
  */
#define PRNG_DISABLE_INT(crpt)      ((crpt)->INTEN &= ~CRPT_INTEN_PRNGIEN_Msk)

/**
  * @brief This macro gets PRNG interrupt flag.
  * @param crpt     Specified crypto module
  * @return PRNG interrupt flag.
  * \hideinitializer
  */
#define PRNG_GET_INT_FLAG(crpt)     ((crpt)->INTSTS & CRPT_INTSTS_PRNGIF_Msk)

/**
  * @brief This macro clears PRNG interrupt flag.
  * @param crpt     Specified crypto module
  * @return None
  * \hideinitializer
  */
#define PRNG_CLR_INT_FLAG(crpt)     ((crpt)->INTSTS = CRPT_INTSTS_PRNGIF_Msk)

/**
  * @brief This macro enables AES interrupt.
  * @param crpt     Specified crypto module
  * @return None
  * \hideinitializer
  */
#define AES_ENABLE_INT(crpt)        ((crpt)->INTEN |= (CRPT_INTEN_AESIEN_Msk|CRPT_INTEN_AESEIEN_Msk))

/**
  * @brief This macro disables AES interrupt.
  * @param crpt     Specified crypto module
  * @return None
  * \hideinitializer
  */
#define AES_DISABLE_INT(crpt)       ((crpt)->INTEN &= ~(CRPT_INTEN_AESIEN_Msk|CRPT_INTEN_AESEIEN_Msk))

/**
  * @brief This macro gets AES interrupt flag.
  * @param crpt     Specified crypto module
  * @return AES interrupt flag.
  * \hideinitializer
  */
#define AES_GET_INT_FLAG(crpt)      ((crpt)->INTSTS & (CRPT_INTSTS_AESIF_Msk|CRPT_INTSTS_AESEIF_Msk))

/**
  * @brief This macro clears AES interrupt flag.
  * @param crpt     Specified crypto module
  * @return None
  * \hideinitializer
  */
#define AES_CLR_INT_FLAG(crpt)      ((crpt)->INTSTS = (CRPT_INTSTS_AESIF_Msk|CRPT_INTSTS_AESEIF_Msk))

/**
  * @brief This macro enables AES key protection.
  * @param crpt     Specified crypto module
  * @return None
  * \hideinitializer
  */
#define AES_ENABLE_KEY_PROTECT(crpt)  ((crpt)->AES_CTL |= CRPT_AES_CTL_KEYPRT_Msk)

/**
  * @brief This macro disables AES key protection.
  * @param crpt     Specified crypto module
  * @return None
  * \hideinitializer
  */
#define AES_DISABLE_KEY_PROTECT(crpt) ((crpt)->AES_CTL = ((crpt)->AES_CTL & ~CRPT_AES_CTL_KEYPRT_Msk) | (0x16UL<<CRPT_AES_CTL_KEYUNPRT_Pos)); \
                                      ((crpt)->AES_CTL &= ~CRPT_AES_CTL_KEYPRT_Msk)


/**
  * @brief This macro enables SHA interrupt.
  * @param crpt     Specified crypto module
  * @return None
  * \hideinitializer
  */
#define SHA_ENABLE_INT(crpt)        ((crpt)->INTEN |= (CRPT_INTEN_HMACIEN_Msk|CRPT_INTEN_HMACEIEN_Msk))

/**
  * @brief This macro disables SHA interrupt.
  * @param crpt     Specified crypto module
  * @return None
  * \hideinitializer
  */
#define SHA_DISABLE_INT(crpt)       ((crpt)->INTEN &= ~(CRPT_INTEN_HMACIEN_Msk|CRPT_INTEN_HMACEIEN_Msk))

/**
  * @brief This macro gets SHA interrupt flag.
  * @param crpt     Specified crypto module
  * @return SHA interrupt flag.
  * \hideinitializer
  */
#define SHA_GET_INT_FLAG(crpt)      ((crpt)->INTSTS & (CRPT_INTSTS_HMACIF_Msk|CRPT_INTSTS_HMACEIF_Msk))

/**
  * @brief This macro clears SHA interrupt flag.
  * @param crpt     Specified crypto module
  * @return None
  * \hideinitializer
  */
#define SHA_CLR_INT_FLAG(crpt)      ((crpt)->INTSTS = (CRPT_INTSTS_HMACIF_Msk|CRPT_INTSTS_HMACEIF_Msk))

/**
  * @brief This macro enables ECC interrupt.
  * @param crpt     Specified crypto module
  * @return None
  * \hideinitializer
  */
#define ECC_ENABLE_INT(crpt)        ((crpt)->INTEN |= (CRPT_INTEN_ECCIEN_Msk|CRPT_INTEN_ECCEIEN_Msk))

/**
  * @brief This macro disables ECC interrupt.
  * @param crpt     Specified crypto module
  * @return None
  * \hideinitializer
  */
#define ECC_DISABLE_INT(crpt)       ((crpt)->INTEN &= ~(CRPT_INTEN_ECCIEN_Msk|CRPT_INTEN_ECCEIEN_Msk))

/**
  * @brief This macro gets ECC interrupt flag.
  * @param crpt     Specified crypto module
  * @return ECC interrupt flag.
  * \hideinitializer
  */
#define ECC_GET_INT_FLAG(crpt)      ((crpt)->INTSTS & (CRPT_INTSTS_ECCIF_Msk|CRPT_INTSTS_ECCEIF_Msk))

/**
  * @brief This macro clears ECC interrupt flag.
  * @param crpt     Specified crypto module
  * @return None
  * \hideinitializer
  */
#define ECC_CLR_INT_FLAG(crpt)      ((crpt)->INTSTS = (CRPT_INTSTS_ECCIF_Msk|CRPT_INTSTS_ECCEIF_Msk))

/**
  * @brief This macro enables RSA interrupt.
  * @param crpt     Specified crypto module
  * @return None
  * \hideinitializer
  */
#define RSA_ENABLE_INT(crpt)        ((crpt)->INTEN |= (CRPT_INTEN_RSAIEN_Msk|CRPT_INTEN_RSAEIEN_Msk))

/**
  * @brief This macro disables RSA interrupt.
  * @param crpt     Specified crypto module
  * @return None
  * \hideinitializer
  */
#define RSA_DISABLE_INT(crpt)       ((crpt)->INTEN &= ~(CRPT_INTEN_RSAIEN_Msk|CRPT_INTEN_RSAEIEN_Msk))

/**
  * @brief This macro gets RSA interrupt flag.
  * @param crpt     Specified crypto module
  * @return ECC interrupt flag.
  * \hideinitializer
  */
#define RSA_GET_INT_FLAG(crpt)      ((crpt)->INTSTS & (CRPT_INTSTS_RSAIF_Msk|CRPT_INTSTS_RSAEIF_Msk))

/**
  * @brief This macro clears RSA interrupt flag.
  * @param crpt     Specified crypto module
  * @return None
  * \hideinitializer
  */
#define RSA_CLR_INT_FLAG(crpt)      ((crpt)->INTSTS = (CRPT_INTSTS_RSAIF_Msk|CRPT_INTSTS_RSAEIF_Msk))


/*@}*/ /* end of group CRYPTO_EXPORTED_MACROS */


/** @addtogroup CRYPTO_EXPORTED_FUNCTIONS CRYPTO Exported Functions
  @{
*/

/*---------------------------------------------------------------------------------------------------------*/
/*  Functions                                                                                      */
/*---------------------------------------------------------------------------------------------------------*/

void Crypto_Init(void);
void PRNG_Config(CRPT_T *crpt, uint32_t u32KeySize, uint32_t u32SeedReload, uint32_t u32Seed);
int  PRNG_Start(CRPT_T *crpt);
void PRNG_Read(CRPT_T *crpt, uint32_t u32RandKey[]);
int  PRNG_ReSeed(CRPT_T *crpt, uint32_t u32Seed);
void AES_Open(CRPT_T *crpt, uint32_t u32EncDec, uint32_t u32OpMode, uint32_t u32KeySize, uint32_t u32SwapType);
int  AES_Start(CRPT_T *crpt, int is_sm4, uint32_t u32DMAMode);
void AES_SetKey(CRPT_T *crpt, uint32_t au32Keys[], uint32_t u32KeySize);
void AES_SetInitVect(CRPT_T *crpt, uint32_t au32IV[]);
void AES_SetDMATransfer(CRPT_T *crpt, uint32_t u32FBIAddr, uint32_t u32FBOAddr, uint32_t u32SrcAddr, uint32_t u32DstAddr, uint32_t u32TransCnt);
void SHA_Open(CRPT_T *crpt, uint32_t u32OpMode, uint32_t u32SwapType, uint32_t hmac_key_len);
int  SHA_Start(CRPT_T *crpt, uint32_t u32DMAMode);
void SHA_SetDMATransfer(CRPT_T *crpt, uint32_t u32SrcAddr, uint32_t u32TransCnt);
void SHA_Read(CRPT_T *crpt, uint32_t u32Digest[]);
int ECC_IsPrivateKeyValid(CRPT_T *crpt, E_ECC_CURVE ecc_curve,  char private_k[]);
int ECC_GeneratePublicKey(CRPT_T *crpt, E_ECC_CURVE ecc_curve, char *private_k, char public_k1[], char public_k2[]);
int ECC_Mutiply(CRPT_T *crpt, E_ECC_CURVE ecc_curve, char x1[], char y1[], char *k, char x2[], char y2[]);
int ECC_GenerateSecretZ(CRPT_T *crpt, E_ECC_CURVE ecc_curve, char *private_k, char public_k1[], char public_k2[], char secret_z[]);
int ECC_GenerateSignature(CRPT_T *crpt, E_ECC_CURVE ecc_curve, char *message, char *d, char *k, char *R, char *S);
int ECC_VerifySignature(CRPT_T *crpt, E_ECC_CURVE ecc_curve, char *message, char *public_k1, char *public_k2, char *R, char *S);

int AES_Start_KS(CRPT_T *crpt, uint32_t u32DMAMode, int ksel, int knum);
int ECC_Write_N(CRPT_T *crpt, E_ECC_CURVE ecc_curve);
int ECC_GeneratePublicKey_KS(CRPT_T *crpt, E_ECC_CURVE ecc_curve, int k_ksnum, int is_ecdh, char public_k1[], char public_k2[]);
int ECC_Mutiply_KS(CRPT_T *crpt, E_ECC_CURVE ecc_curve, int x1_ksnum, char x1[], int y1_ksnum, char y1[], int k_ksnum, char *k, char x2[], char y2[]);
int ECC_GenerateSecretZ_KS(CRPT_T *crpt, E_ECC_CURVE ecc_curve, int k_ksnum, char *private_k, char public_x[], char public_y[], int z_to_ks, int z_owner, char secret_z[]);
int ECC_GenerateSignature_KS(CRPT_T *crpt, E_ECC_CURVE ecc_curve, char *message, int d_ksnum, int k_ksnum, char *R, char *S);
int ECC_VerifySignature_KS(CRPT_T *crpt, E_ECC_CURVE ecc_curve, char *message, int x_ksnum, int y_ksnum, char *R, char *S);

int RSA_Encrypt(CRPT_T *crpt, int ras_blen, uint8_t *data, int dlen, uint8_t *pubk, int klen, uint8_t *n, int nlen, uint8_t *out, int *olen);
int RSA_Decrypt(CRPT_T *crpt, int ras_blen, uint8_t *data, int dlen, uint8_t *privk, int klen, uint8_t *n, int nlen, uint8_t *out, int *olen);

void Hex2Reg(char input[], uint32_t volatile reg[]);
void Reg2Hex(int count, uint32_t volatile reg[], char output[]);
void Hex2RegEx(char input[], uint32_t volatile reg[], int shift);

/*@}*/ /* end of group CRYPTO_EXPORTED_FUNCTIONS */

/*! @}*/ /* end of group CRYPTO_Driver */

/*! @}*/ /* end of group Standard_Driver */

#ifdef __cplusplus
}
#endif

#endif  /* __CRYPTO_H__ */
