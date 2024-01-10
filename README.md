# File-Encryptor
A simple but solid plain text-based-file encryptor(that generates a randomized result each time its ran) and decryptor written in C++ 20. 

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

# Input
![image](https://github.com/dek0039/fileEncryptor/assets/154227154/6d872150-33e8-49fe-93e5-5ceed6eb125e)
# Output
![image](https://github.com/dek0039/fileEncryptor/assets/154227154/23130a69-4843-4fb9-9b89-7eeb93e30872)
# Output(ran again with same file)
![image](https://github.com/dek0039/fileEncryptor/assets/154227154/584c41fe-97da-4d2f-8679-70c8a4b8a0f2)
# CMD
![image](https://github.com/dek0039/fileEncryptor/assets/154227154/ef7aa7ef-bc0d-4aa2-b3f7-8c9054339089)




Please don't use this.

