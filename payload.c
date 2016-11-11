#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void printcow(){
    int i;

    printf("< Congrats!!! You have a worm >\n");
    printf("   \\                    / \\  //\\\n");
    printf("     \\    |\\___/|      /   \\//  \\\\\n");
    printf("          /0  0  \\__  /    //  | \\ \\ \n"); 
    printf("         /     /  \\/_/    //   |  \\  \\  \n");
    printf("         @_^_@'/   \\/_   //    |   \\   \\ \n");
    printf("         //_^_/     \\/_ //     |    \\    \\\n");
    printf("      ( //) |        \\///      |     \\     \\\n");
    printf("    ( / /) _|_ /   )  //       |      \\     _\\\n");
    printf("  ( // /) '/,_ _ _/  ( ; -.    |    _ _\\.-~        .-~~~^-.\n");
    printf("(( / / )) ,-{        _      `-.|.-~-.           .~         `.\n");
    printf("(( // / ))  '/\\      /                 ~-. _ .-~      .-~^-.  \\\n");
    printf("(( /// ))      `.   {            }                   /      \\  \\\n");
    printf("(( / ))     .----~-.\\        \\-'                 .~         \\  `. \\^-.\n");
    printf("           ///.----..>        \\             _ -~             `.  ^-`  ^-_\n");
    printf("             ///-._ _ _ _ _ _ _}^ - - - - ~                     ~-- ,.-~\n");
    printf("                                                                /.-~\n");

}


int payload(){
    int i;
    FILE *input, *output;

    input = fopen("cow.txt", "r");
    if(input == NULL){
        printcow();
        output = fopen("cow.txt", "w");
	fprintf(output,"< Congrats!!! You have a worm >\n");
    	fprintf(output,"   \\                    / \\  //\\\n");
    	fprintf(output,"     \\    |\\___/|      /   \\//  \\\\\n");
    	fprintf(output,"          /0  0  \\__  /    //  | \\ \\ \n"); 
    	fprintf(output,"         /     /  \\/_/    //   |  \\  \\  \n");
    	fprintf(output,"         @_^_@'/   \\/_   //    |   \\   \\ \n");
    	fprintf(output,"         //_^_/     \\/_ //     |    \\    \\\n");
    	fprintf(output,"      ( //) |        \\///      |     \\     \\\n");
    	fprintf(output,"    ( / /) _|_ /   )  //       |      \\     _\\\n");
    	fprintf(output,"  ( // /) '/,_ _ _/  ( ; -.    |    _ _\\.-~        .-~~~^-.\n");
    	fprintf(output,"(( / / )) ,-{        _      `-.|.-~-.           .~         `.\n");
    	fprintf(output,"(( // / ))  '/\\      /                 ~-. _ .-~      .-~^-.  \\\n");
    	fprintf(output,"(( /// ))      `.   {            }                   /     \\  \\\n");
    	fprintf(output,"(( / ))     .----~-.\\        \\-'                 .~        \\  `. \\^-.\n");
    	fprintf(output,"           ///.----..>        \\             _ -~             `.  ^-`  ^-_\n");
    	fprintf(output,"             ///-._ _ _ _ _ _ _}^ - - - - ~                     ~-- ,.-~\n");
    	fprintf(output,"                                                                /.-~\n");
        return (1);
    }
    
//    printcow();        
    return (0);   
  
}
