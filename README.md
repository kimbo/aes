# AES

This repo contains a proof of concept AES implementation in C. It supports 128, 192, and 256 bit keys.

It was done for my own personal education and is **NOT SUITABLE FOR PRODUCTION**.

## Setup

```
git clone https://github.com/kimbo/aes.git
cd aes
make
./aes
```

## Usage

```
Usage: ./aes <key> <decrypt/encrypt> <msg>
```

Given a 128 bit key `604a843ed6a8eddb6e985b37f0e0a36a` and a message `39c5925f2b4447956f3c08054895864a`,
you can encrypt it like so:

```
$ ./aes 604a843ed6a8eddb6e985b37f0e0a36a encrypt 39c5925f2b4447956f3c08054895864a
# ... lots of debug info
round[10].output    4ff6c50f676232ac7df39c9ec436e49b
```

Then to decrypt it, you can do the following:
```
$ ./aes 604a843ed6a8eddb6e985b37f0e0a36a decrypt 4ff6c50f676232ac7df39c9ec436e49b
# ...
round[10].output    39c5925f2b4447956f3c08054895864a
```

Tada!

## Tests

Tests include unit tests and also the tests in FIPS Appendix C for 128, 192, and 256 bit keys.

```
git clone https://github.com/kimbo/aes.git
cd aes
make test # build the tests
./test # run unit tests and tests in FIPS Appendix C
```