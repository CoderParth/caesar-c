A simple CLI tool that implements caesar cipher algorithm in C.  

The usage of internal libraries and apis have been limited as much as I could. 

The cli command looks like the following, once you have compiled the main.c file:
```
./main <filename> <shift_value> <cipher-type>
```

Example:

To encode:

```
./main test.txt 4 encode
```
 
To decode:
```
./main test.txt 4 decode
```

A shift value determines how many positions each letter in the plaintext is shifted in the alphabet. If the shift value is 4, each letter of the text will be replaced by the letter that comes 4 positions later in the alphabet.

For example:

A becomes E

B becomes F

C becomes G


And so on...
