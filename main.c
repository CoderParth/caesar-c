#include <stdio.h>
#include <stdlib.h>

const char *HELP =
    "To encode a file, type: ./main <filename> <shift_value> <encode>\n"
    "To decode, type: ./main <filename> <shift_value> <decode> \n";

const char *OUT_FILE_INSTRUCTION =
    "Please provide a name for the  output file. \n"
    "Do not provide file extension, as the extension of the original file will "
    "be used. \n"
    "> ";

void *safeMalloc(size_t size) {
  void *ptr = malloc(size);
  if (ptr == NULL) {
    printf("Memory allocation failed");
    exit(1);
  }
  return ptr;
}

int len(char *s) {
  int i = 0;
  int n = 0;
  while (s[i] != '\0') {
    i++;
    n++;
  }
  return n;
}

int areEqual(char *s1, char *s2) {
  int m = len(s1), n = len(s2);
  if (m != n)
    return 0;
  for (int i = 0; i < m; i++) {
    if (s1[i] != s2[i])
      return 0;
  }
  return 1;
}

char *append(char *s1, char *s2) {
  int m = len(s1), n = len(s2), si = 0;
  char *s = safeMalloc(sizeof(char) * (m + n) + 1);
  for (int i = 0; i < m; i++) {
    s[si] = s1[i];
    si++;
  }
  for (int i = 0; i < n; i++) {
    s[si] = s2[i];
    si++;
  }
  s[si] = '\0';
  return s;
}

// Uppercase letters ('A' to 'Z') have ASCII values from 65 (for 'A') to 90 (for
// 'Z'). Lowercase letters ('a' to 'z') have ASCII values from 97 (for 'a') to
// 122 (for 'z').
char *encode(char *input, int shiftValue) {
  int n = len(input), e = 0;
  char *encoded = safeMalloc(sizeof(char) * (n + 1));
  if (shiftValue < 0)
    shiftValue += 26;

  for (int i = 0; i < n; i++) {
    if (input[i] >= 'A' && input[i] <= 'Z') {
      char enc = input[i] + shiftValue;
      if ((int)enc > 90) {
        enc -= 26;
      }
      encoded[e] = enc;
      e++;
      continue;
    }
    if (input[i] >= 'a' && input[i] <= 'z') {
      char enc = input[i] + shiftValue;
      if ((int)enc > 122) {
        enc -= 26;
      }
      encoded[e] = enc;
      e++;
      continue;
    }
    encoded[e] = input[i];
    e++;
  }
  encoded[e] = '\0';
  return encoded;
}

char *decode(char *input, int shiftValue) {
  int n = len(input), e = 0;
  char *decoded = safeMalloc(sizeof(char) * (n + 1));
  for (int i = 0; i < n; i++) {
    if (input[i] >= 'A' && input[i] <= 'Z') {
      char dec = input[i] - shiftValue;
      if ((int)dec < 65) {
        dec += 26;
      }
      decoded[e] = dec;
      e++;
      continue;
    }
    if (input[i] >= 'a' && input[i] <= 'z') {
      char dec = input[i] - shiftValue;
      if ((int)dec < 97) {
        dec += 26;
      }
      decoded[e] = dec;
      e++;
      continue;
    }
    decoded[e] = input[i];
    e++;
  }
  return decoded;
}

char *cipher(char *input, int shiftValue, char *cipherType) {
  if (areEqual(cipherType, "encode"))
    return encode(input, shiftValue);
  if (areEqual(cipherType, "decode"))
    return decode(input, shiftValue);
  printf("Not a valid cipher type. Please either write 'encode' or 'decode'\n");
  exit(1);
}

char *readFile(char *fileName) {
  FILE *file = fopen(fileName, "r");
  if (file == NULL) {
    printf("Error opening file");
    exit(1);
  }
  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);
  fseek(file, 0, SEEK_SET);
  char *content = safeMalloc(file_size + 1);
  size_t bytesRead = fread(content, 1, file_size, file);
  content[bytesRead] = '\0';
  fclose(file);
  return content;
}

FILE *createFile(char *fileName) {
  FILE *fptr;
  fptr = fopen(fileName, "w");
  return fptr;
}

void writeToFile(FILE *fptr, char *s) {
  fprintf(fptr, "%s", s);
  fclose(fptr);
}

char *askForFileName() {
  char *fName = safeMalloc(sizeof(char) * 100);
  printf("%s ", OUT_FILE_INSTRUCTION);
  scanf("%99s", fName);
  return fName;
}

char *reverse(char *s) {
  int n = len(s);
  char *rev = safeMalloc(sizeof(char) * n);
  int e = 0;
  for (int i = n - 1; i >= 0; i--) {
    rev[e] = s[i];
    e++;
  }
  rev[e] = '\0';
  return rev;
}

char *fileExt(char *fName) {
  int n = len(fName);
  char *ext = safeMalloc(sizeof(char) * n);
  int e = 0;
  for (int i = n - 1; i >= 0; i--) {
    if (fName[i] == '.')
      break;
    ext[e] = fName[i];
    e++;
  }
  ext[e] = '\0';
  return reverse(ext);
}

void cli(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Please provide a file name. Type '--help' for instructions. \n");
    exit(1);
  }
  if (areEqual(argv[1], "--help")) {
    printf("%s \n", HELP);
    exit(1);
  }
  char *fileName = argv[1];
  if (argc < 3) {
    printf("Please provide a shift value \n");
    exit(1);
  }
  int shiftValue = atoi(argv[2]);
  if (argc < 4) {
    printf("Please mention either 'encode' or 'decode' at the end of the "
           "command \n");
    exit(1);
  }
  char *cipherType = argv[3];
  char *input = readFile(fileName);
  char *c = cipher(input, shiftValue, cipherType);
  char *outputFile = askForFileName();
  char *ext = fileExt(fileName);
  char *newFileName = append(outputFile, ".");
  newFileName = append(newFileName, ext);
  FILE *f = createFile(newFileName);
  writeToFile(f, c);
  printf("%s has been created on this directory \n", newFileName);
  free(input);
  free(c);
  free(ext);
  free(outputFile);
  free(newFileName);
}

int main(int argc, char *argv[]) {
  cli(argc, argv);
  return 0;
}
