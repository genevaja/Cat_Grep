CC=gcc -Wall -Werror -Wextra -std=c11

all: s21_cat s21_grep

s21_cat: 
	$(CC) ./cat/s21_cat.c -o ./cat/s21_cat

s21_grep: 
	$(CC) ./grep/s21_grep.c -o ./grep/s21_grep

test_cat:
	bash ./cat/own_test.sh

test_grep:
	bash ./grep/grep.sh

rebuild: clean all

clean:
	rm -f ./cat/s21_cat
	rm -f ./grep/s21_grep
	rm -f ./grep/log.txt
	rm -f ./log*

sanitazer: s21_cat_sanitazer s21_grep_sanitazer

s21_cat_sanitazer: 
	$(CC) ./cat/s21_cat.c -o ./cat/s21_cat -fsanitize=address

s21_grep_sanitazer: 
	$(CC) ./grep/s21_grep.c -o ./grep/s21_grep -fsanitize=address

test_grep_val:
	bash ./grep/val.sh
