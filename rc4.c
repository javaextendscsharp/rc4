#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

void swap(unsigned char *a, unsigned char *b) 
{
  unsigned char t = *a;
  *a = *b;
  *b = t;
}

void RC4(unsigned char *key, unsigned char *data, int data_len) 
{
  unsigned char S[256];
  unsigned char T[256];
  int key_len = strlen((char *)key);
  int i, j;

  // Key-scheduling algorithm (KSA)
  for (i = 0; i < 256; i++) 
  {
    S[i] = i;
    T[i] = key[i % key_len];
  }
  for (i = 0, j = 0; i < 256; i++) 
  {
    j = (j + S[i] + T[i]) % 256;
    swap(&S[i], &S[j]);
  }

  // Pseudo-random generation algorithm (PRGA)
  for (i = 0, j = 0; i < data_len; i++) 
  {
    j = (j + S[i]) % 256;
    swap(&S[i], &S[j]);
    data[i] ^= S[(S[i] + S[j]) % 256];
  }
}

void hex_encode(unsigned char *data, int data_len, char *hex_str) 
{
  static const char hex_chars[] = "0123456789abcdef";
  int i;
  for (i = 0; i < data_len; i++) 
  {
    hex_str[i * 2] = hex_chars[data[i] >> 4];
    hex_str[i * 2 + 1] = hex_chars[data[i] & 0x0f];
  }
  hex_str[i * 2] = '\0';
}

void hex_decode(char *hex_str, unsigned char *data, int data_len) 
{
  int i;
  for (i = 0; i < data_len; i++) 
  {
    unsigned char c = hex_str[i * 2];
    if (c >= '0' && c <= '9') 
    {
      data[i] = (c - '0') << 4;
    } 
    else if (c >= 'a' && c <= 'f') 
    {
      data[i] = (c - 'a' + 10) << 4;
    }

    c = hex_str[i * 2 + 1];

    if (c >= '0' && c <= '9') 
    {
      data[i] |= c - '0';
    } 
    else if (c >= 'a' && c <= 'f') 
    {
      data[i] |= c - 'a' + 10;
    }
  }
}

int main(void) 
{
  FILE *fp_in, *fp_out;
  uint8_t key[] = {0x01, 0x02, 0x03, 0x04, 0x05};
  unsigned char data[] = {};
  char hex_str[13];
  int data_len = strlen((char*)data);

 // Open the input file
 fp_in = fopen("input.txt", "r");

 if(!fp_in) 
 {
    fprintf(stderr, "Error: unable to open input file\n");
    return 1;
 }

// Read the data from the input file
if(fread(data, 1, data_len, fp_in) != data_len) 
{
    fprintf(stderr, "Error: unable to read input file\n");
    fclose(fp_in);
    return 1;
}

// Encrypt the data
RC4(key, data, data_len);

// Convert the encrypted data to hexadecimal string
hex_encode(data, data_len, hex_str);

// Open the output file
fp_out = fopen("output.txt", "w");
if(!fp_out) 
{
    fprintf(stderr, "Error: unable to open output file\n");
    fclose(fp_in);
    return 1;
}

// Write the encrypted data as hexadecimal string to the output file
if(fprintf(fp_out, "%s\n", hex_str) < 0) 
{
    fprintf(stderr, "Error: unable to write output file\n");
    fclose(fp_in);
    fclose(fp_out);
    return 1;
}

// Close the files
fclose(fp_in);
fclose(fp_out);

return 0;
}






