// INCLUSAO DE BIBLIOTECAS:

#include <stdio.h>                              // Inclui funcoes de entrada e saida padrao.
#include <stdlib.h>                             // Inclui funcoes de alocacao de memoria, controle de processo, conversoes, etc.
#include <string.h>                             // Inclui funcoes para manipulacao de strings.
#include <stdbool.h>                            // Inclui definicoes para tipo booleano.
#include <unistd.h>                             // Inclui a funcao sleep.

//===========================================================================================================================================

// DEFINICAO DE CONSTANTES:

#define MAX_PASSWORD_LENGTH 15                  // Define previamente o maximo de caracteres que podem ser usados para a criacao de senhas.
#define MAX_USERNAME_LENGTH 15                  // Define previamente o maximo de caracteres que podem ser usados na criacao de um usuario.

//===========================================================================================================================================

// DEFINICAO DE ESTRUTURA PARA ARMAZENAR DADOS DOS ITENS DE CADA USUARIO:

typedef struct{
    int codigo;                                 // Codigo do item registrado.
    char nome[25];                              // Nomes dados aos itens a serem cadastrados.
    float preco;                                // Preco dos itens que serao cadastrados.
    bool disponibilidade;                       // Disponibilidade do item em 'Sim'(1) ou 'Nao'(0).
} Item;                                         // Mudando o nome das variaveis.

//===========================================================================================================================================

// FUNCOES GERAIS DECLARADAS:

void LimpaTela();                               // Funcao rapida para limpar a tela.
void RegisterUser(int *num_users);              // Funcao que registra um novo usuario.
void LoginUser(int *user, int *num_users);      // Funcao que permite o login do usuario se o que foi informado esta registrado.
bool DeleteAccount(int *user);                  // Funcao que apaga a conta logada.
void RegisterItem(int *user, int *num_itens);   // Funcao que registra um novo item.
void Index(int *user, int *num_users);          // Funcao que exibe o sumario de cada usuario.
void EditUser(int *user, int *num_users);       // Funcao que edita um usuario.
void EditItem(int *user, int *num_itens);       // Funcao que modifica um item.
void DeleteItem(int *user, int *num_itens);     // Funcao que remove um item.
void ListUsers(int *num_users);                 // Funcao que lista os usuarios registrados.

//===========================================================================================================================================

// FUNCAO QUE PERMITE A LIMPEZA DE TELA:

void LimpaTela(){
    
    #ifdef __linux__
        system("clear");
    #else
        system("cls");
    #endif

} // Fim da funcao de Limpar a Tela.

//===========================================================================================================================================

// FUNCAO DE REGISTRO DE USUARIO:

void RegisterUser(int *num_users){

    char c;                                     // Buffer para ler um caractere do arquivo.
    char username[MAX_USERNAME_LENGTH];         // Armazena o nome de usuario digitado.
    char password[MAX_PASSWORD_LENGTH];         // Armazena a senha do usuario digitada.
    char cnfpswd[MAX_PASSWORD_LENGTH];          // Usado para confirmar/comparar a senha antes de concluir o registro do usuario.
    FILE *fusers;                               // Arquivo de usuarios.
    FILE *fitems;                               // Arquivo de itens.
    char fusername[35];                         // Usado para verificar se o nome de usuario inserido consta em fusers.
    char *delim;                                // Delimitador usado para separar o nome de usuario da senha no arquivo.

    printf(" =====================================================================================================\n");
    printf("|                                               REGISTRO                                              |\n");
    printf(" =====================================================================================================\n");

    // Contagem de usuarios registrados:
    *num_users = 0;
    fusers = fopen("users.txt", "a+");
    for (c = getc(fusers); c != EOF; c = getc(fusers))
        if (c == '\n')
            (*num_users)++;
    rewind(fusers);

    // Leitura do nome de usuario a ser registrado:
    printf("\nNovo usuario: ");
    fgets(username, MAX_USERNAME_LENGTH, stdin);
    if(strchr(username, ';') != NULL){      // Previne entrada de caractere proibido no nome de usuario.
        printf("\nCaractere invalido inserido, tente novamente.");
        fclose(fusers);
        return;
    } // Fim do IF.
    username[strcspn(username, "\n")] = 0;

    // Leitura da senha a ser registrada:
    printf("Nova senha: ");
    fgets(password, MAX_PASSWORD_LENGTH, stdin);
    if(strchr(password, ';') != NULL){      // Previne entrada de caractere proibido na senha do usuario.
        printf("\nCaractere invalido inserido, tente novamente.");
        fclose(fusers);
        return;
    } // Fim do IF.
    password[strcspn(password, "\n")] = 0;

    // Confirmacao da senha inserida:
    printf("Confirme a senha digitada: ");
    fgets(cnfpswd, MAX_PASSWORD_LENGTH, stdin);
    cnfpswd[strcspn(cnfpswd, "\n")] = 0;
    if(!strcmp(cnfpswd,password)){
        for(int count = 0; count <= *num_users; count++){       // Faz a leitura de linhas de fusers.
            fgets(fusername, 35, fusers);
            delim = strchr(fusername, ';');
            if (delim != NULL)
                *delim = '\0';      // Delimita a quantidade de caracteres gravados em fusername ate o primeiro ';'.
            if(!strcmp(username, fusername)){       // Caso o nome de usuario ja esteja registrado, volta para a tela inicial.
                printf("\nNome de usuario ja registrado, tente novamente.");
                fclose(fusers);
                return;
            } // Fim do IF.
        } // Fim do FOR.
        rewind(fusers);

        // Caso as duas senhas correspondam e o nome de usuario esteja livre, finaliza o registro.
        fprintf(fusers, "%s;%s;\n", username, password);        // Grava as credenciais do usuario no arquivo, separadas por ';', e reserva uma linha para o proximo usuario.
        fitems = fopen("items.txt", "a");
        fputs("#", fitems);     // Delimita o campo de itens em items.txt do usuario registrado pelo caractere '#'.
        fclose(fitems);
        printf("\nRegistro efetuado com sucesso!\n");
        fclose(fusers);
        return;
    } // Fim do IF.

    // Caso as senhas nao correspondam, imprime a mensagem de erro e volta para a tela inicial.
    else{
        printf("\nConfirmacao invalida! Digite a senha novamente.");
        return;
    } // Fim do ELSE.

} // Fim da funcao de Registro de Usuario.

//===========================================================================================================================================

// FUNCAO DE LOGIN DO USUARIO:

void LoginUser(int *user, int *num_users){

    char username[MAX_USERNAME_LENGTH];         // Armazena o nome de usuario digitado.
    char password[MAX_PASSWORD_LENGTH];         // Armazena a senha do usuario digitada.
    FILE *fusers;                               // Arquivo de usuarios.
    char fusername[35];                         // Usado para verificar se o nome de usuario inserido consta em fusers.
    char fpassword[35];                         // Usado para verificar se a senha inserida consta na mesma linha onde se encontra fusername.
    char *delim;                                // Delimitador usado para separar parametros de uma linha em ';'.
    char c;                                     // Buffer para ler um caractere do arquivo.

    printf(" =====================================================================================================\n");
    printf("|                                                LOGIN                                                |\n");
    printf(" =====================================================================================================\n");

    // Abertura de arquivo:
    *num_users = 0;
    fusers = fopen("users.txt", "r");
    if(!fusers){
        printf("\nNenhum usuario registrado. Voltando para a tela inicial.");
        sleep(2);
        return;
    } // Fim do IF.

    // Contagem de usuarios registrados:
    for (c = getc(fusers); c != EOF; c = getc(fusers))
        if (c == '\n')
            (*num_users)++;
    rewind(fusers);
    if(!(*num_users)){      // Caso o arquivo esteja em branco, volta para a tela inicial.
        printf("\nNenhum usuario registrado. Voltando para a tela inicial.");
        sleep(2);
        return;
    } // Fim do IF.

    // Leitura do nome de usuario:
    printf("\nNome de usuario: ");
    fgets(username, MAX_USERNAME_LENGTH, stdin);
    username[strcspn(username, "\n")] = 0;

    // Leitura da senha:
    printf("Senha: ");
    fgets(password, MAX_USERNAME_LENGTH, stdin);
    password[strcspn(password, "\n")] = 0;

    // Verificacao do nome de usuario inserido:
    for(*user = 0; *user <= *num_users; (*user)++){
        fgets(fusername, 35, fusers);       // Le cada linha do arquivo.
        delim = strchr(fusername, ';');
        if (delim != NULL)
            *delim = '\0';      // Delimita a quantidade de caracteres gravados em fusername ate o primeiro ';'.
        if(!strcmp(username, fusername))        // Interrompe o loop se o nome de usuario inserido tiver uma correspondencia no arquivo.
            break;
    } // Fim do FOR.
    rewind(fusers);

    // Verificacao da senha inserida:
    for(int count = 0; count <= *user; count++)
        fgets(fpassword, 35, fusers);       // Le a linha do arquivo em que se encontra a string fusername.
    for(int count = 0; count <= (int)(strlen(fpassword) - strlen(fusername)); count++)      // Move os caracteres lidos apos o primeiro ';' para o comeco da string fpassword.
        fpassword[count] = fpassword[(strlen(fusername) + 1) + count];
    delim = strchr(fpassword, ';');
    if (delim != NULL)
        *delim = '\0';      // Delimita a quantidade de caracteres gravados em fpassword ate o segundo ';'.

    // Caso as credenciais inseridas correspondam as de determinada linha do arquivo, o login eh bem-sucedido e retorna 1.
    if(!strcmp(fusername, username) && !strcmp(fpassword, password)){
        printf("\nLogin bem-sucedido!\n");
        sleep(2);
        fclose(fusers);
        Index(&(*user), &(*num_users));
        return;
    } // Fim do IF.

    // Caso nao haja correspondencia, volta para a tela inicial.
    printf("\nNome de usuario ou senha incorretos!\n");
    sleep(2);
    fclose(fusers);
    return;

} // Fim da funcao de Login do Usuario.

//===========================================================================================================================================

// FUNCAO QUE LISTA OS USUARIOS REGISTRADOS:

void ListUsers(int *num_users){

    FILE *fusers;                               // Arquivo de usuarios.
    char c;                                     // Buffer para ler um caractere do arquivo.
    char fusername[35];                         // Armazena um nome de usuario do arquivo.
    char *delim;                                // Delimitador usado para separar parametros de uma linha em ';'.

    printf(" =====================================================================================================\n");
    printf("|                                          LISTA DE USUARIOS                                          |\n");
    printf(" =====================================================================================================\n\n");

    // Abertura de arquivo:
    fusers = fopen("users.txt", "r");
    if (!fusers) {
        printf("Nenhum usuario registrado.\n");
        sleep(2);
        return;
    } // Fim do IF.

    // Contagem de usuarios registrados.
    *num_users = 0;
    for (c = getc(fusers); c != EOF; c = getc(fusers))
        if (c == '\n')
            (*num_users)++;
    rewind(fusers);

    // Exibe os nomes dos usuarios na tela.
    for(int count = 1; count <= *num_users; count++){       // Faz a leitura de linhas de fusers.
        fgets(fusername, 35, fusers);
        delim = strchr(fusername, ';');
        if (delim != NULL)
            *delim = '\0';      // Delimita os caracteres gravados em fusername ate ';'.
        printf("    %d. %s\n", count, fusername);
    } // Fim do FOR.

    // Finalizando...
    fclose(fusers);
    printf("\nPressione Enter para voltar ao menu principal...");
    getchar();

} // Fim da funcao de Listar Usuarios.

//===========================================================================================================================================

// FUNCAO QUE MOSTRA O SUMARIO DE ITENS E A MEDIA DOS DISPONIVEIS:

void Index(int *user, int *num_users){

    char str[20];                               // Buffer para ser usado na funcao fgets.
    short op2;                                  // Variavel de escolha (opcao).
    int num_itens;                              // Contador de itens cadastrados para o usuario logado.
    FILE *fitems;                               // Arquivo de itens.
    char c;                                     // Buffer para ler um caractere do arquivo.

    // Exibicao do sumario de itens cadastrados:
    do{
        float soma_precos = 0;                  // Utilizado para somar o precos de todos os itens disponiveis.
        int disponiveis = 0;                    // Contador usado para contar os itens disponiveis.

        LimpaTela();

        // Abertura de arquivo:
        fitems = fopen("items.txt", "r");
        if(!fitems){
            printf("Nao foi possivel abrir 'items.txt', reinicie o programa.");
            sleep(2);
            exit(-1);
        }

        printf(" =====================================================================================================\n");
        printf("|                                    SUMARIO DOS ITENS CADASTRADOS:                                   |\n");
        printf(" =====================================================================================================\n");
        printf(" =====================================================================================================\n");
        printf("|     Codigo     |                Nome                 |      Preco      |       Disponibilidade      |\n");
        printf(" =====================================================================================================\n");

        // Processo de leitura dos itens e calculo de media:

        // Contagem de itens cadastrados pelo usuario:
        for(int count2 = 0; count2 <= *user; count2++){                 // Le o arquivo ate chegar no campo do usuario logado.
            long pos = ftell(fitems);                                   // Obtem a posicao inicial dos itens do usuario no arquivo.
            num_itens = 0;                                              // Reinicia a contagem de itens para o usuario logado.
            for(c = getc(fitems); c != EOF; c = getc(fitems)){          // Le o arquivo caractere por caractere.
                if(c == '#')
                    break;                                              // '#' define o fim dos itens cadastrados para o usuario logado.
                if(c == '\n')
                    num_itens++;                                        // '\n' define o fim de um determinado item cadastrado pelo usuario.
            } // Fim do FOR.
            if(count2 == *user)
                fseek(fitems, pos, SEEK_SET);                           // Volta para a posicao pos se count2 for igual ao numero do usuario logado.
        } // Fim do FOR.

        // Alocacao de memoria:
        Item *itens = (Item*)malloc(num_itens * sizeof(Item));          // Aloca memoria para ler os dados de cada item registrado pelo usuario logado.
        if(itens == NULL){
            LimpaTela();
            printf("Erro: Memoria insuficiente, reinicie o programa.");
            sleep(2);
            fclose(fitems);
            exit(-1);
        }

        // Leitura dos itens cadastrados pelo usuario:
        for(int item = 0; item < num_itens; item++){
            char codigo[21], preco[21];

            // Limpeza de strings.
            codigo[0] = '\0';
            preco[0] = '\0';
            itens[item].nome[0] = '\0';

            // Leitura do codigo do item:
            for(c = getc(fitems); c != EOF; c = getc(fitems)){
                if(c == ',')
                    break;                                              // ',' define o fim do codigo do item.
                strncat(codigo, &c, 1);                                 // Copia o caractere 'c' no final de 'codigo'.
            } // Fim do FOR.
            itens[item].codigo = atoi(codigo);      // String para inteiro.

            // Leitura do nome do item:
            for(c = getc(fitems); c != EOF; c = getc(fitems)){
                if(c == ',')
                    break;                                              // ',' define o fim do nome do item.
                strncat(itens[item].nome, &c, 1);                       // Copia o caractere 'c' no final de 'itens[item].nome'.
            } // Fim do FOR.

            // Leitura do preco do item:
            for(c = getc(fitems); c != EOF; c = getc(fitems)){
                if(c == ',')
                    break;                                              // ',' define o fim do preco do item.
                strncat(preco, &c, 1);                                  // Copia o caractere 'c' no final de 'preco'.
            } // Fim do FOR.
            itens[item].preco = atof(preco);        // String para float.

            // Leitura da disponibilidade do item:
            c = getc(fitems);

            // Imprime na tela os dados do item.
            printf("| %-14d | %-35s | %-15.2f |", itens[item].codigo, itens[item].nome, itens[item].preco);
            if(c == '0')
                printf(" %-26s |\n", "Nao disponivel");
            else{
                printf(" %-26s |\n", "Disponivel");
                soma_precos += itens[item].preco;
                disponiveis++;
            } // Fim do ELSE.

            c = getc(fitems);       // Pula o caractere '\n' do arquivo.
        } // Fim do FOR.

        printf(" =====================================================================================================\n");

        // Calculo e exibição da media dos itens:
        if(disponiveis > 0){
            float media_precos = soma_precos / disponiveis;
            printf("\nPreco medio dos produtos disponiveis: %.2f\n", media_precos);
        } // Fim do IF.
        else{
            printf("\nNenhum produto disponivel.\n");
        } // Fim do ELSE.
        fclose(fitems);
        free(itens);

        // Menu de opcoes:
        printf("\nSELECIONE UMA OPCAO:\n");
        printf("=> CADASTRAR ITEM (digite 1)\n");
        printf("=> EDITAR ITEM (digite 2)\n");
        printf("=> REMOVER ITEM (digite 3)\n");
        printf("=> EDITAR CONTA (digite 4)\n");
        printf("=> FAZER LOGOUT (digite 5)\n");
        printf("=> APAGAR CONTA (digite 0)\n\n");
        printf("Digite: ");
        fgets(str, 3, stdin);
        op2 = atoi(str);        // String para inteiro.
        switch(op2){

            // Caso seja escolhida a opcao 1, volta ao cadastro de itens.
            case 1:
                RegisterItem(&(*user), &num_itens);
                break;

            // Caso seja escolhida a opcao 2, edita um item.
            case 2:
                EditItem(&(*user), &num_itens);
                break;

            // Caso seja escolhida a opcao 3, remove um item.
            case 3:
                DeleteItem(&(*user), &num_itens);
                break;

            // Caso seja escolhida a opcao 4, edita a conta.
            case 4:
                EditUser(&(*user), &(*num_users));
                break;

            // Caso seja escolhida a opcao 5, volta para a tela inicial.
            case 5:
                return;

            // Caso seja escolhida a opcao 0, apaga a conta.
            case 0:
                if(DeleteAccount(&(*user)))
                    return;
                break;

            // Caso digite algum outro caractere, volta para o sumario.
            default:
                printf("Opcao invalida, tente novamente.\n");
                sleep(2);
                break;

        } // Fim do SWITCH.
    } while(op2 != 5);

}   // Fim da funcao de Sumario.

//===========================================================================================================================================

// FUNCAO DE CADASTRO DE ITENS:

void RegisterItem(int *user, int *num_itens){

    char str[20];                               // Buffer para ser usado na funcao fgets.
    int novos_itens = 0;                        // Define o numero de itens a serem cadastrados.
    FILE *fitems;                               // Arquivo de itens original.
    FILE *fitemsNew;                            // Arquivo de itens novo.
    char c;                                     // Buffer para ler um caractere do arquivo.
    char *delim;                                // Delimitador usado para separar o codigo do item dos outros dados no arquivo.
    char fcodigo[55];                           // Armazena o codigo de um determinado item do arquivo.
    long pos;

    // Leitura do numero de itens a ser cadastrados:
    for(;;){
        LimpaTela();
        printf(" =====================================================================================================\n");
        printf("|                                          CADASTRO DE ITENS                                          |\n");
        printf(" =====================================================================================================\n");

        printf("\nQuantos itens deseja cadastrar? (Digite apenas numeros inteiros positivos): ");
        fgets(str, 3, stdin);
        novos_itens = atoi(str);        // String para inteiro.
        if(novos_itens > 0)                                             // Verifica se o numero de itens a serem cadastrados eh valido.
            break;
        printf("\nNumero invalido de itens, tente novamente.\n");
        sleep(2);
    } // Fim do FOR.

    // Alocacao de memoria:
    Item *itens = (Item*)malloc(novos_itens * sizeof(Item));            // Aloca memoria para ler os dados dos itens que se deseja cadastrar.
    if(itens == NULL){
        LimpaTela();
        printf("\nErro: Memoria insuficiente, reinicie o programa.");
        sleep(2);
        exit(-1);
    }

    // Abertura de arquivos:
    fitems = fopen("items.txt", "r+");
    if(!fitems){
        printf("\nNao foi possivel abrir 'items.txt', reinicie o programa.");
        sleep(2);
        exit(-1);
    }
    fitemsNew = fopen("items.tmp", "w+");                               // Cria um novo arquivo de itens temporario para sobrescrever o original.

    // Posicionando a linha e coluna em que o item sera registrado:
    for(int count = 0; count <= *user; count++){                        // Le o arquivo ate chegar no campo do usuario logado.
        pos = ftell(fitems);                                            // Obtem a posicao inicial dos itens do usuario no arquivo.
        for(c = getc(fitems); c != EOF; c = getc(fitems)){              // Le o arquivo caractere por caractere.
            fputc(c, fitemsNew);                                        // Copia caracteres de "items.txt" para "items.tmp".
            if(c == '#')
                break;                                                  // '#' define o fim dos itens cadastrados para o usuario logado.
        } // Fim do FOR.
    } // Fim do FOR.
    fseek(fitemsNew, -1, SEEK_CUR);                                     // Volta uma posicao de "items.tmp".

    // Processo de cadastro para todo item que se deseja cadastrar:
    for(int item = 0; item < novos_itens; item++){
        fflush(fitemsNew);      // Salva o conteudo que ja foi escrito em "items.tmp".

        // Executa o loop ate o cadastro for bem-sucedido, isto eh, nao ha dados invalidos e o codigo do item esta disponivel:
        for(;;){
            LimpaTela();
            printf(" =====================================================================================================\n");
            printf("|                                          CADASTRO DE ITENS                                          |\n");
            printf(" =====================================================================================================\n");
            printf("\nItem %d/%d:\n", item + 1,novos_itens);

            // Coleta de informações do item:

            // CODIGO DO ITEM.
            printf("Codigo do item (Digite apenas numeros inteiros positivos): ");
            fgets(str, 20, stdin);
            str[strcspn(str, "\n")] = '\0';
            itens[item].codigo = atoi(str);     // String para inteiro.
            if(itens[item].codigo <= 0){
                printf("\nEntrada invalida para o codigo do item, tente novamente.");
                sleep(2);
            } // Fim do IF.
            else{

                // Verifica se o codigo do item ja esta em uso:
                FILE *disp_codigo;
                disp_codigo = fopen("items.tmp", "r");      // Abre "items.tmp" novamente para verificar se o codigo do item ja esta em uso.
                if(!disp_codigo){
                    printf("\nNao foi possivel abrir 'items.tmp', reinicie o programa.");
                    sleep(2);
                    exit(-1);
                }
                fseek(disp_codigo, pos, SEEK_SET);      // Vai para a posicao do arquivo em que comecam os itens do usuario logado.
                for(int count = 0; count < *num_itens; count++){        // Le cada item cadastrado pelo usuario.
                    fgets(fcodigo, 55, disp_codigo);
                    delim = strchr(fcodigo, ',');
                    if (delim != NULL)
                        *delim = '\0';      // Delimita a quantidade de caracteres gravados em fcodigo ate o primeiro ','.
                    if(!strcmp(str, fcodigo))       // Quebra o loop se for encontrada uma correspondencia.
                        break;
                } // Fim do FOR.
                fclose(disp_codigo);
                if(!strcmp(str, fcodigo)){      // Compara o codigo inserido com o codigo lido no arquivo.
                    printf("\nCodigo do item ja registrado, tente novamente.");
                    sleep(2);
                } // Fim do IF.

                else{
                    // NOME DO ITEM.
                    printf("Nome do item: ");
                    fgets(itens[item].nome, 25, stdin);
                    if(strchr(itens[item].nome, ',') != NULL || strchr(itens[item].nome, '#') != NULL){     // Previne entrada de caracteres proibidos no nome do item.
                        printf("\nCaractere invalido inserido, tente novamente.");
                        sleep(2);
                    } // Fim do IF.
                    else{
                        itens[item].nome[strcspn(itens[item].nome, "\n")] = '\0';

                        // PRECO DO ITEM.
                        printf("Preco do item (use ponto '.' para dividir a parte decimal): ");
                        fgets(str, 20, stdin);
                        itens[item].preco = atof(str);      // String para float.
                        if(!atof(str) || (itens[item].preco < 0)){
                            printf("\nEntrada invalida para o preco do item, tente novamente.");
                            sleep(2);
                        } // Fim do IF.
                        else{

                            // DISPONIBILIDADE DO ITEM.
                            printf("Disponibilidade do item (0 para nao, 1 para sim): ");
                            fgets(str, 3, stdin);
                            if(str[0] != '0' && str[0] != '1'){
                                printf("\nEntrada invalida para a disponibilidade do item, tente novamente.");
                                sleep(2);
                            } // Fim do IF.
                            else{
                                itens[item].disponibilidade = atoi(str);        // String para inteiro.

                                // Escreve os dados coletados em "items.tmp", separados por ',':
                                fprintf(fitemsNew, "%d,%s,%.2f,%d\n", itens[item].codigo, itens[item].nome, itens[item].preco, itens[item].disponibilidade);
                                (*num_itens)++;     // Incrementa o numero de itens cadastrados.
                                break;      // Quebra o loop, resultando em cadastro bem-sucedido.

                            } // Fim do ELSE.
                        } // Fim do ELSE.
                    } // Fim do ELSE.
                } // Fim do ELSE.
            } // Fim do ELSE.
        } // Fim do FOR.
    } // Fim do FOR.

    // Finalizando...
    fseek(fitems, -1, SEEK_CUR);        // Volta uma posicao de "items.txt".
    for(c = getc(fitems); c != EOF; c = getc(fitems))
        fputc(c, fitemsNew);        // Copia todos os caracteres de "items.txt" em "items.tmp" a partir da posicao atual.
    fclose(fitemsNew);
    fclose(fitems);
    remove("items.txt");
    rename("items.tmp", "items.txt");
    free(itens);
    LimpaTela();
    return;

} // Fim da funcao de Cadastro de Itens.

//===========================================================================================================================================

// FUNCAO PARA EDITAR ITEM:

void EditItem(int *user, int *num_itens){

    char str[25];                               // Buffer para ser usado na funcao fgets.
    FILE *fitems;                               // Arquivo de itens original.
    FILE *fitemsNew;                            // Arquivo de itens novo.
    Item item;                                  // Armazena os dados novos do item a ser editado.
    char *delim;                                // Delimitador usado para separar o codigo do item dos outros dados no arquivo.
    char c;                                     // Buffer para ler um caractere do arquivo.
    char fcodigo[55];                           // Armazena o codigo de um item do arquivo.
    char fpreco[20];                            // Armazena o preco original do item a ser editado.
    char fdisponibilidade[1];                   // Armazena a disponibilidade original do item a ser editado.
    int pos_item;                               // Obtem a linha do item que se deseja editar da lista de itens do usuario.
    long pos_dado;                              // Obtem a posicao de um dado do item que se deseja editar no arquivo.

    LimpaTela();
    printf(" =====================================================================================================\n");
    printf("|                                              EDITAR ITEM                                            |\n");
    printf(" =====================================================================================================\n");
    printf("\nDigite o codigo do item que deseja editar: ");
    fgets(str, 20, stdin);
    str[strcspn(str, "\n")] = '\0';

    // Abertura de arquivos:
    fitems = fopen("items.txt", "r+");
    if(!fitems){
        printf("\nNao foi possivel abrir 'items.txt', reinicie o programa.");
        sleep(2);
        exit(-1);
    }
    fitemsNew = fopen("items.tmp", "w+");

    // Verifica se o item referente ao codigo inserido existe:
    for(int count = 0; count < *user; count++){                         // Le o arquivo ate chegar no campo do usuario logado.
        for(c = getc(fitems); c != EOF; c = getc(fitems)){              // Le o arquivo caractere por caractere.
            if(c == '#')
                break;                                                  // '#' define o fim dos itens cadastrados para o usuario logado.
        } // Fim do FOR.
    } // Fim do FOR.
    for(pos_item = 0; pos_item < *num_itens; pos_item++){               // Compara o codigo inserido com os dos itens cadastrados.
        fgets(fcodigo, 55, fitems);
        delim = strchr(fcodigo, ',');
        if (delim != NULL)
            *delim = '\0';      // Delimita a quantidade de caracteres gravados em fusername ate o primeiro ','.
        if(!strcmp(str, fcodigo))
            break;
    } // Fim do FOR.
    if(strcmp(str, fcodigo)){
        printf("\nItem nao encontrado, tente novamente.");
        sleep(2);
        fclose(fitemsNew);
        fclose(fitems);
        remove("items.tmp");
        return;
    } // Fim do IF.
    rewind(fitems);

    // Copiando os itens anteriores aos do usuario logado em "items.tmp":
    for(int count = 0; count < *user; count++){                         // Le o arquivo ate chegar no campo do usuario logado.
        for(c = getc(fitems); c != EOF; c = getc(fitems)){              // Le o arquivo caractere por caractere.
            fputc(c, fitemsNew);                                        // Copia caracteres de "items.txt" para "items.tmp".
            if(c == '#')
                break;                                                  // '#' define o fim dos itens cadastrados para o usuario logado.
        } // Fim do FOR.
    } // Fim do FOR.

    // Copiando os itens anteriores ao que sera editado em "items.tmp":
    for(int count = 0; count < pos_item; count++){                      // Le o arquivo ate chegar no item que se deseja remover.
        for(c = getc(fitems); c != EOF; c = getc(fitems)){              // Le o arquivo caractere por caractere.
            fputc(c, fitemsNew);                                        // Copia caracteres de "items.txt" para "items.tmp".
            if(c == '\n')
                break;                                                  // '\n' define o fim de um determinado item cadastrado pelo usuario.
        } // Fim do FOR.
    } // Fim do FOR.

    // Escreve o codigo do item que sera editado em "items.tmp":
    do
    {
        c = getc(fitems);
        fputc(c, fitemsNew);
    } while (c != ',' && c != EOF);

    printf("\nDigite os novos dados do item (Codigo: %s):\n", fcodigo);

    // Le o nome atual do item:
    pos_dado = ftell(fitems);           // Salva a posicao atual do arquivo para retornar depois de ler o nome atual do item.
    for(c = getc(fitems); c != EOF; c = getc(fitems)){
        if(c == ',')
            break;                      // ',' define o fim do nome do item.
        strncat(item.nome, &c, 1);      // Copia o caractere 'c' no final de 'item.nome'.
    } // Fim do FOR.
    fseek(fitems, pos_dado, SEEK_SET);  // Retorna para pos.

    // Muda o nome do item:
    printf("Novo nome do item (deixe em branco para manter '%s'): ", item.nome);
    fgets(str, 25, stdin);
    str[strcspn(str, "\n")] = '\0';
    if(str[0] != '\0'){       // Se nao estiver em branco, atualiza o nome.

        // Verifica a entrada de caractere proibido no nome do item:
        if(strchr(str, ',') != NULL || strchr(str, '#') != NULL){
            printf("\nCaractere invalido inserido, tente novamente.");
            sleep(2);
            fclose(fitems);
            fclose(fitemsNew);
            remove("items.tmp");
            return;
        } // Fim do IF.

        fprintf(fitemsNew, "%s,", str);     // Salva o novo nome do item no arquivo.

        // Pula o nome do item no arquivo original:
        do c = getc(fitems);
        while(c != ',' && c != EOF);

    } // Fim do IF.

    // Copia o nome do item original no arquivo novo:
    else
        do
        {
            c = getc(fitems);
            fputc(c, fitemsNew);
        } while (c != ',' && c != EOF);

    // Le o preco atual do item:
    pos_dado = ftell(fitems);       // Salva a posicao atual do arquivo para retornar depois de ler o preco atual do item.
    for(c = getc(fitems); c != EOF; c = getc(fitems)){
        if(c == ',')
            break;      // ',' define o fim do preco do item.
        strncat(fpreco, &c, 1);     // Copia o caractere 'c' no final de 'item.preco'.
    } // Fim do FOR.
    item.preco = atof(fpreco);      // String para float.
    fseek(fitems, pos_dado, SEEK_SET);      // Retorna para pos.

    // Muda o preco do item:
    printf("Novo preco do item (deixe em branco para manter '%.2f'): ", item.preco);
    fgets(str, 25, stdin);
    str[strcspn(str, "\n")] = '\0';
    if(str[0] != '\0'){       // Se nao estiver em branco, atualiza o preco.
        item.preco = atof(str);     // String para float.
        fprintf(fitemsNew, "%.2f,", item.preco);        // Salva o preco novo do item no arquivo.

        // Pula o preco do item no arquivo original:
        do c = getc(fitems);
        while(c != ',' && c != EOF);

    } // Fim do IF.

    // Copia o preco original do item no arquivo novo:
    else
        do
        {
            c = getc(fitems);
            fputc(c, fitemsNew);
        } while (c != ',' && c != EOF);

    // Le a disponibilidade atual do item:
    c = getc(fitems);
    fdisponibilidade[0] = c;
    item.disponibilidade = atoi(fdisponibilidade);      // String para inteiro.
    fseek(fitems, -1, SEEK_CUR);

    // Muda a disponibilidade do item:
    printf("Nova disponibilidade do item (0 para nao, 1 para sim, deixe em branco para manter '%d'): ", item.disponibilidade);
    fgets(str, 3, stdin);
    str[strcspn(str, "\n")] = '\0';
    if(str[0] != '\0'){       // Se nao estiver em branco, atualiza a disponibilidade.
        item.disponibilidade = atoi(str);       // String para inteiro.
        fprintf(fitemsNew, "%d\n", item.disponibilidade);       // Salva a disponibilidade nova do item no arquivo.

        // Pula a disponibilidade do item no arquivo original.
        do c = getc(fitems);
        while(c != '\n' && c != EOF);

    } // Fim do IF.

    // Copia a disponibilidade original do item no arquivo novo:
    else
        do
        {
            c = getc(fitems);
            fputc(c, fitemsNew);
        } while (c != '\n' && c != EOF);

    // Copia o restante dos caracteres de "items.txt" em "items.tmp":
    for(c = getc(fitems); c != EOF; c = getc(fitems))                   // Le o arquivo caractere por caractere.
        fputc(c, fitemsNew);                                            // Copia caracteres de "items.txt" para "items.tmp".

    // Sobrescrevendo o arquivo original:
    fclose(fitemsNew);
    fclose(fitems);
    remove("items.txt");
    rename("items.tmp", "items.txt");

    printf("\nItem editado com sucesso.");
    sleep(2);
    return;

}// Fim da funcao de Editar Item.

//===========================================================================================================================================

// FUNCAO PARA APAGAR UM ITEM:

void DeleteItem(int *user, int *num_itens) {

    char str[20];                               // Armazena o codigo do item que se deseja remover.
    FILE *fitems;                               // Arquivo de itens original.
    FILE *fitemsNew;                            // Arquivo de itens novo.
    char *delim;                                // Delimitador usado para separar o codigo do item dos outros dados no arquivo.
    char c;                                     // Buffer para ler um caractere do arquivo.
    char fcodigo[55];                           // Armazena o codigo de um item do arquivo.
    int item;                                   // Obtem a linha do item que se deseja remover da lista de itens do usuario.

    LimpaTela();
    printf(" =====================================================================================================\n");
    printf("|                                             REMOVER ITEM                                            |\n");
    printf(" =====================================================================================================\n");
    printf("\nDigite o codigo do item que deseja remover: ");
    fgets(str, 20, stdin);
    str[strcspn(str, "\n")] = '\0';

    // Abertura de arquivos:
    fitems = fopen("items.txt", "r+");
    if(!fitems){
        printf("\nNao foi possivel abrir 'items.txt', reinicie o programa.");
        sleep(2);
        exit(-1);
    }
    fitemsNew = fopen("items.tmp", "w+");

    // Verifica se o item referente ao codigo inserido existe:
    for(int count = 0; count < *user; count++){                         // Le o arquivo ate chegar no campo do usuario logado.
        for(c = getc(fitems); c != EOF; c = getc(fitems)){              // Le o arquivo caractere por caractere.
            if(c == '#')
                break;                                                  // '#' define o fim dos itens cadastrados para o usuario logado.
        } // Fim do FOR.
    } // Fim do FOR.
    for(item = 0; item < *num_itens; item++){                           // Compara o codigo inserido com os dos itens cadastrados.
        fgets(fcodigo, 55, fitems);
        delim = strchr(fcodigo, ',');
        if (delim != NULL)
            *delim = '\0';      // Delimita a quantidade de caracteres gravados em fusername ate o primeiro ','.
        if(!strcmp(str, fcodigo))
            break;
    } // Fim do FOR.
    if(strcmp(str, fcodigo)){
        printf("\nItem nao encontrado, tente novamente.");
        sleep(2);
        fclose(fitemsNew);
        fclose(fitems);
        remove("items.tmp");
        return;
    } // Fim do IF.
    rewind(fitems);

    // Copiando os itens anteriores aos do usuario logado em "items.tmp":
    for(int count = 0; count < *user; count++){                         // Le o arquivo ate chegar no campo do usuario logado.
        for(c = getc(fitems); c != EOF; c = getc(fitems)){              // Le o arquivo caractere por caractere.
            fputc(c, fitemsNew);                                        // Copia caracteres de "items.txt" para "items.tmp".
            if(c == '#')
                break;                                                  // '#' define o fim dos itens cadastrados para o usuario logado.
        } // Fim do FOR.
    } // Fim do FOR.

    // Copiando os itens anteriores ao que sera apagado em "items.tmp":
    for(int count = 0; count < item; count++){                          // Le o arquivo ate chegar no item que se deseja remover.
        for(c = getc(fitems); c != EOF; c = getc(fitems)){              // Le o arquivo caractere por caractere.
            fputc(c, fitemsNew);                                        // Copia caracteres de "items.txt" para "items.tmp".
            if(c == '\n')
                break;                                                  // '\n' define o fim de um determinado item cadastrado pelo usuario.
        } // Fim do FOR.
    } // Fim do FOR.

    // Pulando o item que sera apagado:
        do
            c = getc(fitems);
        while(c != '\n' && c != EOF);

    // Copiando os itens posteriores ao que sera apagado em "items.tmp":
    for(c = getc(fitems); c != EOF; c = getc(fitems))                   // Le o arquivo caractere por caractere.
        fputc(c, fitemsNew);                                            // Copia caracteres de "items.txt" para "items.tmp".

    // Sobrescrevendo o arquivo original:
    fclose(fitemsNew);
    fclose(fitems);
    remove("items.txt");
    rename("items.tmp", "items.txt");

    printf("\nItem excluido com sucesso.");
    sleep(2);
    return;

} // Fim da funcao de Apagar Item.

//===========================================================================================================================================

// FUNCAO PARA EDITAR USUARIO LOGADO:

void EditUser(int *user, int *num_users){

    char c;
    char username[MAX_USERNAME_LENGTH];         // Armazena o nome de usuario digitado.
    char password[MAX_PASSWORD_LENGTH];         // Armazena a senha do usuario digitada.
    FILE *fusers;                               // Arquivo de usuarios original.
    FILE *fusersNew;                            // Arquivo de usuarios novo.
    char fusername[35];                         // Armazena o nome de usuario do arquivo.
    char fpassword[35];                         // Armazena a senha do arquivo.
    long pos_user;                              // Armazena a posicao de um dado do usuario.
    char *delim;                                // Delimitador usado para separar o nome de usuario da senha no arquivo.

    LimpaTela();
    printf(" =====================================================================================================\n");
    printf("|                                             EDITAR CONTA                                            |\n");
    printf(" =====================================================================================================\n");

    // Abertura de arquivos:
    fusers = fopen("users.txt", "r+");
    if(!fusers){
        printf("\nNao foi possivel abrir 'users.txt', reinicie o programa.");
        sleep(2);
        exit(-1);
    }
    fusersNew = fopen("users.tmp", "w+");

    if (!fusers) {
        printf("\nNenhum usuario registrado.\n");
        sleep(2);
        return;
    } // Fim do IF.

    // Copiando os usuarios anteriores ao que sera editado em "users.tmp":
    for(int count = 0; count < *user; count++){                         // Le o arquivo ate chegar no campo do usuario que sera editado.
        for(c = getc(fusers); c != EOF; c = getc(fusers)){              // Le o arquivo caractere por caractere.
            fputc(c, fusersNew);                                        // Copia caracteres de "items.txt" para "items.tmp".
            if(c == '\n')
                break;                                                  // '\n' define o fim das credenciais do usuario.
        } // Fim do FOR.
    } // Fim do FOR.

    // Le o nome de usuario atual:
    pos_user = ftell(fusers);       // Salva a posicao atual do arquivo para retornar depois de ler o nome atual do usuario.
    fusername[0] = '\0';
    for(c = getc(fusers); c != EOF; c = getc(fusers)){
        if(c == ';')
            break;      // ';' define o fim do nome de usuario.
        strncat(fusername, &c, 1);      // Copia o caractere 'c' no final de 'fusername'.
    } // Fim do FOR.
    fseek(fusers, pos_user, SEEK_SET);      // Retorna para pos_user.

    // Muda o nome de usuario:
    printf("\nNovo nome de usuario (deixe em branco para manter '%s'): ", fusername);
    fgets(username, 25, stdin);
    username[strcspn(username, "\n")] = '\0';
    if(username[0] != '\0'){       // Se nao estiver em branco, atualiza o nome de usuario.

        // Verifica a entrada de caractere proibido no nome de usuario:
        if(strchr(username, ';') != NULL){
            printf("\nCaractere invalido inserido, tente novamente.");
            sleep(2);
            fclose(fusers);
            fclose(fusersNew);
            remove("users.tmp");
            return;
        } // Fim do IF.

        // Verifica se o novo nome de usuario esta em uso:
        pos_user = ftell(fusers);       // Salva a posicao atual do arquivo para retornar depois de verificar se o nome de usuario inserido esta em uso.
        rewind(fusers);
        for(int count = 0; count <= *num_users; count++){       // Faz a leitura de linhas de fusers.
            fgets(fusername, 35, fusers);
            delim = strchr(fusername, ';');
            if (delim != NULL)
                *delim = '\0';      // Delimita a quantidade de caracteres gravados em fusername ate o primeiro ';'.
            if(!strcmp(username, fusername)){       // Caso o nome de usuario ja esteja registrado, volta para a tela inicial.
                printf("\nNome de usuario em uso, tente novamente.");
                sleep(2);
                fclose(fusers);
                fclose(fusersNew);
                remove("users.tmp");
                return;
            } // Fim do IF.
        } // Fim do FOR.
        fseek(fusers, pos_user, SEEK_SET);      // Retorna para pos_user.

        fprintf(fusersNew, "%s;", username);        // Salva o novo nome de usuario no arquivo.

        // Pula o nome de usuario no arquivo original:
        do c = getc(fusers);
        while(c != ';' && c != EOF);

    } // Fim do IF.

    // Copia o nome de usuario original no arquivo novo:
    else
        do
        {
            c = getc(fusers);
            fputc(c, fusersNew);
        } while (c != ';' && c != EOF);

    // Le a senha atual:
    pos_user = ftell(fusers);
    for(c = getc(fusers); c != EOF; c = getc(fusers)){
        if(c == ';')
            break;      // ',' define o fim do nome do item.
        strncat(fpassword, &c, 1);      // Copia o caractere 'c' no final de 'fpassword'.
    } // Fim do FOR.
    fseek(fusers, pos_user, SEEK_SET);

    // Muda a senha do usuario:
    printf("Nova senha (deixe em branco para manter '%s'): ", fpassword);
    fgets(password, 25, stdin);
    password[strcspn(password, "\n")] = '\0';
    if(password[0] != '\0'){       // Se nao estiver em branco, atualiza a senha.

        // Verifica a entrada de caractere proibido no nome de usuario:
        if(strchr(password, ';') != NULL){
            printf("\nCaractere invalido inserido, tente novamente.");
            sleep(2);
            fclose(fusers);
            fclose(fusersNew);
            remove("users.tmp");
            return;
        } // Fim do IF.

        fprintf(fusersNew, "%s;", password);        // Salva a nova senha no arquivo.

        // Pula a senha no arquivo original:
        do c = getc(fusers);
        while(c != ';' && c != EOF);

    } // Fim do IF.

    // Copia a senha original no arquivo novo:
    else
        do
        {
            c = getc(fusers);
            fputc(c, fusersNew);
        } while (c != ';' && c != EOF);

    // Copia o restante dos caracteres de "users.txt" em "users.tmp":
    for(c = getc(fusers); c != EOF; c = getc(fusers))                   // Le o arquivo caractere por caractere.
        fputc(c, fusersNew);                                            // Copia caracteres de "items.txt" para "items.tmp".

    // Finalizando:
    fclose(fusers);
    fclose(fusersNew);
    remove("users.txt");
    rename("users.tmp", "users.txt");
    return;

} // Fim da funcao de Editar Usuario.

//===========================================================================================================================================

// FUNCAO PARA APAGAR A CONTA LOGADA:

bool DeleteAccount(int *user){

    FILE *fusers;                               // Arquivo de usuarios original.
    FILE *fitems;                               // Arquivo de itens original.
    FILE *fusersNew;                            // Arquivo de usuarios novo.
    FILE *fitemsNew;                            // Arquivo de itens novo.
    char c;                                     // Buffer para ler um caractere do arquivo.
    char confirmacao[3];                        // Confirmacao de exclusao de conta.

    do
    {
        LimpaTela();
        printf(" =====================================================================================================\n");
        printf("|                                            EXCLUIR CONTA                                            |\n");
        printf(" =====================================================================================================\n");
        printf("\nTem certeza de que deseja excluir sua conta? Digite 'S' para confirmar, 'N' para voltar: ");
        fgets(confirmacao, 3, stdin);

        if(confirmacao[0] == 'S' || confirmacao[0] == 's'){

            // Abertura de arquivos:
            fitems = fopen("items.txt", "r+");
            if(!fitems){
                printf("\nNao foi possivel abrir 'items.txt', reinicie o programa.");
                sleep(2);
                exit(-1);
            }
            fitemsNew = fopen("items.tmp", "w+");

            // Copiando os itens anteriores aos do usuario que sera apagado em "items.tmp":
            for(int count = 0; count < *user; count++){                         // Le o arquivo ate chegar no campo do usuario logado.
                for(c = getc(fitems); c != EOF; c = getc(fitems)){              // Le o arquivo caractere por caractere.
                    fputc(c, fitemsNew);                                        // Copia caracteres de "items.txt" para "items.tmp".
                    if(c == '#')
                        break;                                                  // '#' define o fim dos itens cadastrados para o usuario logado.
                } // Fim do FOR.
            } // Fim do FOR.

            // Pulando os itens do usuario que sera apagado:
            do
                c = getc(fitems);
            while(c != '#' && c != EOF);

            // Copiando os itens posteriores aos do usuario que sera apagado em "items.tmp":
            for(c = getc(fitems); c != EOF; c = getc(fitems))                   // Le o arquivo caractere por caractere.
                fputc(c, fitemsNew);                                            // Copia caracteres de "items.txt" para "items.tmp".

            // Sobrescrevendo o arquivo original:
            fclose(fitems);
            fclose(fitemsNew);
            remove("items.txt");
            rename("items.tmp", "items.txt");

            // Abertura de arquivos:
            fusers = fopen("users.txt", "r+");
            if(!fusers){
                printf("\nNao foi possivel abrir 'users.txt', reinicie o programa.");
                sleep(2);
                exit(-1);
            }
            fusersNew = fopen("users.tmp", "w+");

            // Copiando os usuarios anteriores ao que sera apagado em "users.tmp":
            for(int count = 0; count < *user; count++){                         // Le o arquivo ate chegar no campo do usuario logado.
                for(c = getc(fusers); c != EOF; c = getc(fusers)){              // Le o arquivo caractere por caractere.
                    fputc(c, fusersNew);                                        // Copia caracteres de "users.txt" para "users.tmp".
                    if(c == '\n')
                        break;                                                  // '#' define o fim dos itens cadastrados para o usuario logado.
                } // Fim do FOR.
            } // Fim do FOR.

            // Pulando as credenciais do usuario que sera apagado:
            do
                c = getc(fusers);
            while(c != '\n' && c != EOF);

            // Copiando os usuarios posteriores ao que sera apagado em "users.tmp":
            for(c = getc(fusers); c != EOF; c = getc(fusers))                   // Le o arquivo caractere por caractere.
                fputc(c, fusersNew);                                            // Copia caracteres de "users.txt" para "users.tmp".

            // Sobrescrevendo o arquivo original:
            fclose(fusers);
            fclose(fusersNew);
            remove("users.txt");
            rename("users.tmp", "users.txt");

            printf("\nConta excluida com sucesso.");
            sleep(2);
            return 1;
        } // Fim do IF.

        if(confirmacao[0] == 'N' || confirmacao[0] == 'n')
            return 0;

        if(confirmacao[0] != 'S' && confirmacao[0] != 'N' && confirmacao[0] != 's' && confirmacao[0] != 'n'){
            printf("\nEntrada invalida, tente novamente.");
            sleep(2);
        } // Fim do IF.

    }while(confirmacao[0] != 'S' && confirmacao[0] != 'N' && confirmacao[0] != 's' && confirmacao[0] != 'n');
    return 0;

} // Fim da funcao de Apagar Usuario.

//===========================================================================================================================================

// FUNCAO PRINCIPAL, ONDE SE ENCONTRA A TELA INICIAL DO SISTEMA:

int main(){

    char str[2];                                // Buffer para ser usado na funcao fgets.
    short op1;                                  // Variavel de escolha (opcao) utilizada na TELA INICIAL.
    int user = 0;                               // Seletor de usuarios.
    int num_users = 0;                          // Usado para verificar o numero de usuarios registrados.

    for(;;){
        LimpaTela();
        do{
            printf(" =====================================================================================================\n");
            printf("|                                         OLA! SEJA BEM-VINDO!                                        |\n");
            printf(" =====================================================================================================\n");

            // Menu de opcoes:
            printf("\nSELECIONE UMA OPCAO:\n");
            printf("=> REGISTRAR USUARIO (digite 1)\n");
            printf("=> FAZER LOGIN (digite 2)\n");
            printf("=> LISTAR USUARIOS (digite 3)\n");
            printf("=> ENCERRAR PROGRAMA (digite 0)\n");
            printf("\nDigite um numero: ");
            fgets(str, 3, stdin);
            op1 = atoi(str);        // String para inteiro.
            switch(op1){

                // Caso seja escolhida a opcao 1, chama a funcao de registro:
                case 1:
                    LimpaTela();
                    RegisterUser(&num_users);
                    sleep(2);
                    break; // Fim do CASE 1.

                // Caso seja escolhida a opcao 2, chama a funcao de login:
                case 2:
                    LimpaTela();
                    LoginUser(&user, &num_users);
                    break; // Fim do CASE 2.

                // Caso seja escolhida a opção 3, lista os usuarios registrados:
                case 3:
                    LimpaTela();
                    ListUsers(&num_users);
                    break; // Fim do CASE 3.

                // Caso seja escolhida a opcao 0, encerra o programa:
                case 0:
                    printf("\nFechando sistema...");
                    sleep(2);
                    LimpaTela();
                    return 0;
                    break; // Fim do CASE 0.

                // Caso digite algum outro caractere, volta para a tela inicial.
                default:
                    printf("\nOpcao invalida, tente novamente.");
                    sleep(2);
                    LimpaTela();

            } // Fim do SWITCH.
        }while(op1 != 1 && op1 != 2 && op1 != 3);
    } // Fim do FOR.

} // Fim da funcao MAIN.