#include <fcgi_stdio.h>

int main(int argc, char **argv) {
	while(FCGI_Accept() >= 0) {
		printf("Content-type: text/html\n\nHello world.\n");
	}
	return 0;
}
