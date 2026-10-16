/* DMOD integration for the TF-PSA-Crypto fork.
 * Copyright The Mbed TLS Contributors
 * SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later
 */
#ifndef MBEDTLS_DMOD_H
#define MBEDTLS_DMOD_H
#if defined(MBEDTLS_DMOD_C)
#include <dmod.h>
#define MBEDTLS_DMOD_NULL_TEXT ""
#define MBEDTLS_DMOD_CONST
/* Call once during module initialization, after the loader fixes its GOT,
 * before PSA initialization or any key/curve operations. No heap is used. */
void mbedtls_dmod_pk_init(void);
void mbedtls_dmod_ecp_init(void);
#else
#define MBEDTLS_DMOD_NULL_TEXT NULL
#define MBEDTLS_DMOD_CONST const
#endif
#endif
