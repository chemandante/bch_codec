# bch_codec
User BCH (Bose-Chaudhuri-Hocquenghem) encode/decode library based on bch module from linux kernel

License is GPL.

This is a fork of [mborgerding/bch_codec](https://github.com/mborgerding/bch_codec), which, in turn, is a fork of the bch.c file in the linux kernel written by Ivan Djelic at Parrot.
It closely follows the original with the following enhancements:
  * all kernel-specific functions have been removed
  * added bit level functions (rather than packed bytes) support for BCH messages, codewords, parity words
  * added error correction interface functions

Differences from [mborgerding/bch_codec](https://github.com/mborgerding/bch_codec) include:

  * Ability to use your own malloc/free and printf functions (uncomment BCH_USE_OWN_MALLOC and BCH_USE_OWN_PRINTF defines in bch_codec.h)
  * Ability to use some instructions of Texas Instruments TMS320C64xx processors (uncomment BCH_USE_TMS320C6400_INTRINSICS define in bch_codec.h)
