#include <stdio.h>
#include <regex.h>

int main (int argc, char ** argv) {
  if (argc < 2) {
    printf("Usage: my_grep PATTERNS [FILES]... | my_grep PATTERNS (read from stdin)\n");
    return 0;
  }
  regex_t regex;
  regmatch_t match[1];
  char regex_error[100];
  int reti;
  char line[1024];

  reti = regcomp(&regex, argv[1], REG_EXTENDED | REG_ICASE);

  if (reti) {
    regerror(reti, &regex, regex_error, sizeof(regex_error));
    fprintf(stderr, "%s\n", regex_error);
    return -1;
  }

  if (argc == 2) {
    while (fgets(line, sizeof(line), stdin) != NULL) {
      reti = regexec(&regex, line, 1, match, 0);

      if (!reti) {
        printf("%s", line);
      }
    }
  } else {
    int qtd_params = argc - 2;
    FILE *fp;
    for (int i = 0; i < qtd_params; i++) {
      if ((fp = fopen(argv[i + 2], "r")) == NULL) {
        fprintf(stderr, "Error opening file %s\n", argv[i + 2]);
      } else {
        while (fgets(line, sizeof(line), fp) != NULL) {
          reti = regexec(&regex, line, 1, match, 0);

          if (!reti) {
            if (qtd_params > 1) {
              printf("%s:%s", argv[i + 2], line);
            } else {
              printf("%s", line);
            }
          }
        }
        fclose(fp);
      }
    }
  }

  regfree(&regex);
  return 0;
}

