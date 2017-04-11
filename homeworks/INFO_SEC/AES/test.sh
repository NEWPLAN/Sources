#!/bin/bash
echo "----------------------- START ------------------------"
echo "Preklad(gcc):"
gcc -Wall -Werror -g -std=c99 -o AES AES.c

echo "--- Test AES-128 -------------------------------------"
echo "PLAINTEXT: 00112233445566778899aabbccddeeff (in hex)"
echo "KEY: 000102030405060708090a0b0c0d0e0f (in hex)"
echo "EXPECTED ENCODED TEXT: 69c4e0d86a7b0430d8cdb78070b4c55a (in hex, ECB)"
echo "encrypting in ECB..."
./AES -if plaintext-128.dat -of encrypted-128.dat -e -m ECB -k 000102030405060708090a0b0c0d0e0f
echo "decrypting in ECB..."
./AES -if encrypted-128.dat -of decrypted-128.dat -d -m ECB -k 000102030405060708090a0b0c0d0e0f
echo "comparing..."
cmp --silent plaintext-128.dat decrypted-128.dat && echo "OK" || echo "files are different"
echo "encrypting in CBC..."
./AES -if plaintext-128.dat -of encrypted-cbc-128.dat -e -m CBC -k 000102030405060708090a0b0c0d0e0f -iv 000102030405060708090a0b0c0d0e0f
echo "decrypting in CBC..."
./AES -if encrypted-cbc-128.dat -of decrypted-cbc-128.dat -d -m CBC -k 000102030405060708090a0b0c0d0e0f -iv 000102030405060708090a0b0c0d0e0f
echo "comparing..."
cmp --silent plaintext-128.dat decrypted-cbc-128.dat && echo "OK" || echo "files are different"

echo "--- Test AES-192 -------------------------------------"
echo "PLAINTEXT: 00112233445566778899aabbccddeeff (in hex)"
echo "KEY: 000102030405060708090a0b0c0d0e0f1011121314151617 (in hex)"
echo "EXPECTED ENCODED TEXT: dda97ca4864cdfe06eaf70a0ec0d7191 (in hex, ECB)"
echo "encrypting in ECB..."
./AES -if plaintext-192.dat -of encrypted-192.dat -e -m ECB -k 000102030405060708090a0b0c0d0e0f1011121314151617
echo "decrypting in ECB..."
./AES -if encrypted-192.dat -of decrypted-192.dat -d -m ECB -k 000102030405060708090a0b0c0d0e0f1011121314151617
echo "comparing..."
cmp --silent plaintext-192.dat decrypted-192.dat && echo "OK" || echo "files are different"
echo "encrypting in CBC..."
./AES -if plaintext-192.dat -of encrypted-cbc-192.dat -e -m CBC -k 000102030405060708090a0b0c0d0e0f1011121314151617 -iv 000102030405060708090a0b0c0d0e0f
echo "decrypting in CBC..."
./AES -if encrypted-cbc-192.dat -of decrypted-cbc-192.dat -d -m CBC -k 000102030405060708090a0b0c0d0e0f1011121314151617 -iv 000102030405060708090a0b0c0d0e0f
echo "comparing..."
cmp --silent plaintext-192.dat decrypted-cbc-192.dat && echo "OK" || echo "files are different"

echo "--- Test AES-256 -------------------------------------"
echo "PLAINTEXT: 00112233445566778899aabbccddeeff (in hex)"
echo "KEY: 000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f (in hex)"
echo "EXPECTED ENCODED TEXT: 8ea2b7ca516745bfeafc49904b496089 (in hex, ECB)"
echo "encrypting in ECB..."
./AES -if plaintext-256.dat -of encrypted-256.dat -e -m ECB -k 000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f
echo "decrypting in ECB..."
./AES -if encrypted-256.dat -of decrypted-256.dat -d -m ECB -k 000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f
echo "comparing..."
cmp --silent plaintext-256.dat decrypted-256.dat && echo "OK" || echo "files are different"
echo "encrypting in CBC..."
./AES -if plaintext-256.dat -of encrypted-cbc-256.dat -e -m CBC -k 000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f -iv 000102030405060708090a0b0c0d0e0f
echo "decrypting in CBC..."
./AES -if encrypted-cbc-256.dat -of decrypted-cbc-256.dat -d -m CBC -k 000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f -iv 000102030405060708090a0b0c0d0e0f
echo "comparing..."
cmp --silent plaintext-256.dat decrypted-cbc-256.dat && echo "OK" || echo "files are different"

echo "----------------------- END --------------------------"
