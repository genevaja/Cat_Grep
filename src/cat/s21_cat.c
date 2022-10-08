#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct flags {
    int b;
    int e;
    int n;
    int s;
    int t;
    int v;
    int no_flag;
} flags;

int flag_parsing(int argc, char **argv, flags *flag_sum);
void flag_output(flags *flag_sum);
void flag_init(flags *flag_sum);
void do_flags(char a, flags *flag_sum);
void do_flags2(char *str, flags *flag_sum);
void file_prepare(int argc, char **argv, flags *flag_sum);
int file_output(char *file_name, flags *flag_sum);
void format_out(char *file_name, flags flag_sum);
void errors(int number, char* string);

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("EMPTY\n");
        return 0;
    }
    flags flag_sum;
    flag_init(&flag_sum);
    if (flag_parsing(argc, argv, &flag_sum) != 1) {
        file_prepare(argc, argv, &flag_sum);
    } else {
        return 1;
    }
}

void flag_init(flags *flag_sum) {
    flag_sum->b = 0;
    flag_sum->e = 0;
    flag_sum->n = 0;
    flag_sum->s = 0;
    flag_sum->t = 0;
    flag_sum->v = 0;
    flag_sum->no_flag = 1;
}

void file_prepare(int argc, char **argv, flags *flag_sum) {
    int avail = 0;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-') {
            file_output(argv[i], flag_sum);
            avail = 1;
        }
    }
    if (avail == 0)
        printf("NOTHING TO DO\n");
}

int file_output(char *file_name, flags *flag_sum) {
    int c;
    FILE *file = NULL;
    file = fopen(file_name, "r");
    if (file == NULL) {
        printf("%s\n", strerror(2));
        return 0;
    }
    // Посчитать количество строк
    int row_count = 0;
    while ((c = fgetc(file)) != EOF) {
        if (c == '\n') {
            row_count++;
        }
    }
    fseek(file, 0, SEEK_SET);
    if (flag_sum->no_flag == 1) {
        while ((c = fgetc(file)) != EOF) {
            putc(c, stdout);
        }
        fclose(file);
    } else {
        format_out(file_name, *flag_sum);
        fclose(file);
    }
    return 0;
}

void format_out(char* file, flags flag_sum) {
    int string = 1;
    char was, be, willbe;
    FILE *fp;
    if ((fp = fopen(file, "r")) == NULL) {
        errors(1, file);
    } else {
        was = '\n';
        be = fgetc(fp);
        while (be != EOF) {
            willbe = fgetc(fp);
            if (flag_sum.s && willbe == '\n' && be == '\n' && was == '\n')
                continue;
            if (was == '\n' && (flag_sum.b || flag_sum.n)) {
                if (flag_sum.b) {
                    if (be != '\n')
                        printf("%6d\t", string++);
                } else if (flag_sum.n) {
                    printf("%6d\t", string++);
                }
            }
            if (be == '\n' && flag_sum.e) {
                printf("$");
            }
            if (be == '\t' && flag_sum.t) {
                printf("^I");
            } else if (be == '\n' || be == ' ' || be == '\t') {
                printf("%c", be);
            } else if (flag_sum.v && ((be < 33 && be >= 0) || be == 127)) {
                be = be == 127? '?': be + 64;
                printf("^%c", be);
            } else {
               printf("%c", be);
            }
            was = be;
            be = willbe;
        }
        fclose(fp);
    }
}

int flag_parsing(int argc, char **argv, flags *flag_sum) {
    char *GNU[] = {"--number-nonblank", "--number", "--squeeze-blank", "-E", "-T"};
    char *cat = "-benstET";
    int avail = 0;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][1] != '-') {
            for (size_t j = 1; j < strlen(argv[i]); j++) {
                if ((strchr(cat, argv[i][j])) != NULL) {
                    do_flags(argv[i][j], flag_sum);
                } else {
                    printf("NO FLAGS\n");
                    return 1;
                }
            }
        } else if (argv[i][0] == '-' && argv[i][1] == '-') {
            for (size_t k = 0; k < 5; k++) {
                if ((strcmp(GNU[k], argv[i])) == 0) {
                    do_flags2(argv[i], flag_sum);
                    avail = 1;
                }
            }
            if (avail == 0)
                printf("NO\n");
        }
    }
    return 0;
}

void do_flags(char a, flags *flag_sum) {
    // printf("MESSAGE FROM DO_FLAGS\n");
    if (a == 'b') {
        flag_sum->b = 1;
        flag_sum->no_flag = 0;
    } else if (a == 'e') {
        flag_sum->e = 1;
        flag_sum->v = 1;
        flag_sum->no_flag = 0;
    } else if (a == 'n') {
        flag_sum->n = 1;
        flag_sum->no_flag = 0;
    } else if (a == 's') {
        flag_sum->s = 1;
        flag_sum->no_flag = 0;
    } else if (a == 't') {
        flag_sum->t = 1;
        flag_sum->v = 1;
        flag_sum->no_flag = 0;
    } else if (a == 'E') {
        flag_sum->e = 1;
        flag_sum->v = 0;
        flag_sum->no_flag = 0;
    } else if (a == 'T') {
        flag_sum->t = 1;
        flag_sum->v = 0;
        flag_sum->no_flag = 0;
    }
}

void do_flags2(char *str, flags *flag_sum) {
    // printf("MESSAGE FROM DO_FLAGS2\n");
    if (strcmp(str, "--number-nonblank\0") == 0) {
        flag_sum->b =  1;
        flag_sum->no_flag = 0;
    } else if (strcmp(str, "--number\0") == 0) {
        flag_sum->n =  1;
        flag_sum->no_flag = 0;
    } else if (strcmp(str, "--squeeze-blank\0") == 0) {
        flag_sum->s =  1;
        flag_sum->no_flag = 0;
    }
}

void flag_output(flags *flag_sum) {
        printf("Flag_sum->b: %d\n", flag_sum->b);
        printf("Flag_sum->e: %d\n", flag_sum->e);
        printf("Flag_sum->n: %d\n", flag_sum->n);
        printf("Flag_sum->s: %d\n", flag_sum->s);
        printf("Flag_sum->t: %d\n", flag_sum->t);
}

void errors(int number, char* string) {
    if (number == 1) {
        printf("%s: No such file or directory\n", string);
    } else if (number == 2) {
        printf("%s: No such flag\n", string);
    }
}
