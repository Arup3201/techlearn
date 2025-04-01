#include<stdio.h>
#include<wchar.h>
#include<locale.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdbool.h>

#define MAX_BUFFER_SIZE 1024

enum CharacterClass {
	WHITESPACE, 
	ALPHABETIC, 
	NUMERIC, 
	ALPHANEUMERIC, 
	HEX, 
	SPECIAL,
	ILLEGAL
};

typedef enum CharacterClass CharacterClass;

bool isWhitespace(char *z) {
	if(z[0] == 0x009 || z[0] == 0x00a || z[0] == 0x00c || z[0] == 0x00d || z[0] == 0x020) {
		return true;
	}
	
	// comment --
	if(z[0] == 0x002d && z[1] == 0x002d) {
		return true;
	}

	// multi-line coment /* ... */
	if(z[0] == 0x002f && z[1] == 0x002a) {
		int i=3;
		while(z[i] != EOF) {
			if(z[i-1] == 0x002a && z[i] == 0x002f) {
				return true;
			}

			i++;
		}

		return false;
	}

	return false;
}


CharacterClass getCharacterClass(wchar_t uchar) {
	if(uchar == 0x009 || uchar == 0x00a || uchar == 0x00c || uchar == 0x00d || uchar == 0x020) {
		return WHITESPACE;
	}

	if((uchar >= 0x0041 && uchar <= 0x005a) || (uchar >= 0x0061 && uchar <= 0x007a) || (uchar == 0x005f) || (uchar > 0x007f) || (uchar >= 0x0030 && uchar <= 0x0039)) {
		return ALPHANEUMERIC;
	}

	if((uchar >= 0x0041 && uchar <= 0x005a) || (uchar >= 0x0061 && uchar <= 0x007a) || (uchar == 0x005f) || (uchar > 0x007f)) {
		return ALPHABETIC;
	}

	if(uchar >= 0x0030 && uchar <= 0x0039) {
		return NUMERIC;
	}

	return ILLEGAL;
}

int main(int argc, char *argv[]) {
	if(argc < 2) {
		fprintf(stderr, "Usage: %s filename.c\n", argv[0]);
		return 1;
	}

	// enable unicode
	setlocale(LC_ALL, "");

	char *tFilename = argv[1];
	int fd = open(tFilename, O_RDONLY, S_IRUSR);
	char buffer[MAX_BUFFER_SIZE];
	read(fd, buffer, MAX_BUFFER_SIZE);

	int i=0;
	char ch = buffer[i];
	while(ch != '\0') {
		fprintf(stdout, "%c -> u%04X\n", ch, (wchar_t)ch);
		switch(getCharacterClass(ch)) {
			case WHITESPACE:
				fprintf(stdout, "WHITESPACE\n");
				break;
			case ALPHABETIC:
				fprintf(stdout, "ALPHETIC\n");
				break;
			case NUMERIC:
				fprintf(stdout, "NUMERIC\n");
				break;
			case ALPHANEUMERIC:
				fprintf(stdout, "ALPHANEUMERIC\n");
				break;
			case HEX:
				fprintf(stdout, "HEX\n");
				break;
			case SPECIAL:
				fprintf(stdout, "SPECIAL\n");
				break;
			case ILLEGAL:
				fprintf(stdout, "ILLEGAL\n");
				break;
		}
		ch = buffer[++i];
	}

	return 1;
}
