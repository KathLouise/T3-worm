#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void printcow(){
    int i;

    printf("< moOh >\n");
    printf("  \\ ^__^\n");
    printf("    (oo)\\_______\n");
    printf("    (__)\\       )\\/\\\n");
    printf("        ||----w |\n");
    printf("        ||     ||\n");
}


int payload(){
    int i;
    FILE *input, *output;

    input = fopen("cow.txt", "r");
    if(input == NULL){
        printcow();
        output = fopen("cow.txt", "w");
        fprintf(output, "< moOh >\n");
        fprintf(output, "  \\ ^__^\n");
        fprintf(output, "    (oo)\\_______\n");
        fprintf(output, "    (__)\\       )\\/\\\n");
        fprintf(output, "        ||----w |\n");
        fprintf(output, "        ||     ||\n");
        return (1);
    }
    
    printcow();        
    return (0);   
  
}
