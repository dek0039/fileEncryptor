# File-Encryptor
A simple but solid plain text-based-file encryptor and decryptor written in C++ 20. 

## Usage
### Encrypting a File
To encrypt a file, use the following command in cmd:
``File-Encryptor.exe <file path to encrypt>, <should decrypt (leave empty for encryption) >``

## "Algorithm" 
The algorithm consists of five stages. In stage one we mutate the string by replacing some alphabetical characters with other not-so-random characters from the substitution map. 
Stage two inserts our identifier key which will be used to decrypt the file later on. In Stage three we XOR the characters in the string by a pseudo-random 2 byte number(0-255).
Stage four copies and add's itself to itself. Basically doubling itself. In stage five we reverse the string. 

For decryption we first undo the reversing then the doubling. And then the magic starts where we XOR our strings first twelve(identifier string length) through 0 to 255 till it matches with our original identifier. When matched we store the key to XOR the rest of the file.
Then we undo our mutation via our map.



Please don't use this.
