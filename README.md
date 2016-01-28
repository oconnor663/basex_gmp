# Implementing saltpack armor in C

Saltpack's [base 62 encoding
scheme](https://github.com/keybase/client/blob/master/go/saltpack/specs/saltpack_armor.md)
uses a large block size, 32 bytes, to get a few extra percentage points of
packing efficiency. That means that we need to use bignums for encoding and
decoding, which are a lot slower than 64-bit native integers. Our first
implementation turned out to be unacceptably slow, and we were worried that we
might've made some bad decisions with the format.

This is a simple implementation of base 62 encoding in C, using the [GMP bignum
library](https://gmplib.org/), which we made just to see how fast it would be.
In a stroke of luck, it turns out GMP's default alphabet is the same as
saltpack's, so all this code does is loop over its input and call library
functions. The result turned out to be about as fast as GnuPG (slower in
encoding, faster in decoding), which is good enough for us.

An example run on my machine:

```bash
# Create a 100 MB file full of random bytes.
$ head -c 100000000 /dev/urandom > /tmp/randfile

# Encode and decode it with gpg.
$ time gpg --enarmor < /tmp/randfile > /tmp/gpgfile
gpg --enarmor < /tmp/randfile > /tmp/gpgfile  0.90s user 0.04s system 99% cpu 0.944 total
$ time gpg --dearmor < /tmp/gpgfile > /dev/null
gpg --dearmor < /tmp/gpgfile > /dev/null  1.44s user 0.03s system 99% cpu 1.469 total

# Encode and decode it with this base 62 implementation.
$ make
$ time ./base62 < /tmp/randfile > /tmp/base62file
./base62 < /tmp/randfile > /tmp/base62file  1.27s user 0.05s system 99% cpu 1.325 total
$ time ./base62 -d < /tmp/base62file > /dev/null
./base62 -d < /tmp/base62file > /dev/null  1.29s user 0.02s system 99% cpu 1.307 total
```
