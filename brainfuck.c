/*
 Copyright (c) 2024 Jokke <https://github.com/JokkeeZ>

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
*/
#include <stdio.h>
#include <stdlib.h>

#define CELL_COUNT 30000

struct Brainfuck
{
	long data_ptr;
	char cells[CELL_COUNT];
};

void loop(char c, long* instr_ptr, char* buffer, struct Brainfuck* bf)
{
	int i = 1;
	if (c == '[' && bf->cells[bf->data_ptr] == 0) {
		while (i > 0) {
			(*instr_ptr)++;

			if (buffer[*instr_ptr] == '[') {
				i++;
			} else if (buffer[*instr_ptr] == ']') {
				i--;
			}
		}
	} else if (c == ']' && bf->cells[bf->data_ptr] != 0) {
		while (i > 0) {
			(*instr_ptr)--;

			if (buffer[*instr_ptr] == ']') {
				i++;
			} else if (buffer[*instr_ptr] == '[') {
				i--;
			}
		}
	}
}

void bf_exec(struct Brainfuck* bf, char* buffer, long buffer_len)
{
	long instr_ptr = 0;
	while (instr_ptr < buffer_len) {
		switch (buffer[instr_ptr]) {
			case '>':
				bf->data_ptr++;
			break;
			case '<':
				bf->data_ptr--;
			break;
			case '+':
				bf->cells[bf->data_ptr]++;
			break;
			case '-':
				bf->cells[bf->data_ptr]--;
			break;
			case '.':
				putchar(bf->cells[bf->data_ptr]);
			break;
			case ',':
				bf->cells[bf->data_ptr] = getchar();
			break;
			case '[':
			case ']':
				loop(buffer[instr_ptr], &instr_ptr, buffer, bf);
			break;
			default: break;
		}

		instr_ptr++;
	}
}

int main(int argc, char* argv[])
{
	if (argc != 2) {
		printf("Expected a path to brainfuck file as an argument.\nUsage: ./brainfuck ./file.bf\n");
		return 1;
	}

	FILE* file = fopen(argv[1], "rb");
	if (file == NULL) {
        fprintf(stderr, "Could not open the file.\n");
        return 1;
	}

	fseek(file, 0, SEEK_END);

	long file_len = ftell(file);
	rewind(file);

	char* file_buffer = (char*)malloc(file_len * sizeof(char));
    if (file_buffer == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        fclose(file);
        return 1;
    }

    if (!fread(file_buffer, sizeof(char), file_len, file)) {
        fprintf(stderr, "Failed to read the brainfuck code file.\n");
        free(file_buffer);
        fclose(file);
        return 1;
    }

    fclose(file);

	struct Brainfuck bf;
	bf_exec(&bf, file_buffer, file_len);

	printf("\n");
	free(file_buffer);
	return 0;
}
