# DMOD backend adaptation

This fork starts from TF-PSA-Crypto 1.1.1. The `dmod/main` baseline includes
unchanged generated core sources from the official bundled release archive.
The DMOD changes are conditional on `MBEDTLS_DMOD_C`; ordinary configurations
retain the upstream declarations and behavior.

DMOD already relocates `DMOD_GLOBAL_POINTER` objects in `.got`. This fork uses
that interface without changing DMF, SDK or loader code:

- PK dispatch callbacks and names live in typed, volatile GOT slots. During
  module startup `mbedtls_dmod_pk_init()` copies them into mutable descriptors.
- ECC precomputed coordinates use pointer-only GOT arrays. Startup populates
  MPI pointer fields while preserving upstream limb counts, signs, point data,
  and fixed-point optimization. Low-order X25519 rejection constants use the
  same existing GOT mechanism.
- Immutable OID/hash/curve names are inline character arrays in DMOD builds;
  sentinel checks use existing numeric identifiers/lengths. No pointer fixup
  is needed for those strings.

Call `mbedtls_dmod_pk_init()` and `mbedtls_dmod_ecp_init()` once after loading
and before PSA/key/curve operations. Startup is externally serialized; these
functions allocate no heap. Volatile GOT reads prevent GCC from replacing
initialized fields with unrelocated constant aggregates. GNU typeof preserves
callback types. Internal descriptor layouts differ in DMOD mode and must not
be mixed across configurations; dmtls keeps this backend ABI private.

The dmtls integration audits final ELF relocation metadata and rejects stored
absolute pointers outside sections already supported by the legacy loader.
It also runs OpenSSL interoperability and actual MCU diagnostics. Updating the
upstream baseline requires rerunning that audit and those tests. Arithmetic,
verification rules, entropy policy and cryptographic constants are unchanged.
