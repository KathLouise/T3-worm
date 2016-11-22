all: word brute

word: wordlist.c
	gcc -g wordlist.c -o wordlist

brute: brutexor.c
	gcc -g brutexor.c -lcrypto -o brutexor	

clean: 
	rm -f *~ wordlist brutexor
	
distclean:	clean
	rm -f dicionario.txt 

