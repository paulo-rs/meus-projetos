/*
=====================
CONTROLE DE GASTOS
=====================
*/
#include <fcntl.h>
#include <io.h>
#include <locale.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#if defined(__linux__) // any linux distribution
    #define PLATFORM "linux"
    #include "ncurses.h"
#elif defined(_WIN32) // any windows system
    #define PLATFORM "windows"
    #include <windows.h>
    #include <conio.h>
#else
    #define PLATFORM "Is not linux or windows"
#endif

#include <ctype.h>

//using namespace std;

/*
Black        |   0
Blue         |   1
Green        |   2
Cyan         |   3
Red          |   4
Magenta      |   5
Brown        |   6
Light Gray   |   7
Dark Gray    |   8
Light Blue   |   9
Light Green  |   10
Light Cyan   |   11
Light Red    |   12
Light Magenta|   13
Yellow       |   14
White        |   15
*/
#define LIGHTCYAN 11
#define RED 4
#define LIGHTRED 12
#define BLUE 1
#define WHITE 15
#define LIME 10
#define YELLOW 14
#define KLEFT 75
#define KRIGHT 77

#define LIGHTCYAN2 "\033[1;36m"
#define LIGHTRED2 "\033[1;31m"
#define YELLOW2 "\033[1;33m"
#define LIME2  "\033[1;32m"
#define WHITE2 "\033[0m"


typedef struct REG {
	int cod;
	char item[40];
	char tipo[1];
	float valor;
	char local[40];
	char data[11];
	struct REG *prox;
} NODE;


void inserir();
void mostrar(char *tipo);
void deletar();
void alterar();
NODE *aloca();
void save();
void load();
void printhtml();
void repl(char c,int wid);
void emptylist();
void freeList();
void editStrField(char *label,char *valor);
void SetColor(int ForgC);
void SetColor2(char *color);
void saycolor(char *str,int color);

NODE *Lista=NULL;

int main() {
	Lista = aloca();
	int choice=-1;
	Lista->prox = NULL;

	Lista->cod = 0;
	setlocale(LC_ALL,"");
	strcpy(Lista->item,"");
	strcpy(Lista->tipo,"");
	strcpy(Lista->local,"");
	strcpy(Lista->data,"");
	Lista->valor = 0;

	load();
	int inicio=1;

	while(choice!=0) {
		system("cls");

		time_t t = time(NULL);
		struct tm tm = *localtime(&t);
		//SetColor(LIGHTCYAN);
		repl('=',90);
		SetColor2(LIME2);
		// _setmode(_fileno(stdout), _O_U16TEXT);

		printf("MENU - CONTROLE DE GASTOS\n");
		printf("PRS Software - (c) Paulo Roberto Silva 2018 - %d/%d/%d %d:%02d\n",
		       tm.tm_mday, tm.tm_mon + 1,tm.tm_year + 1900, tm.tm_hour, tm.tm_min);
		SetColor2(LIGHTCYAN2);
		repl('=',90);
		SetColor2(WHITE2);
        	printf("1)Inclusão 2)Exclusão 3)Alteração 4)Mostra todos 5)Receitas 6)Despesas 7)Limpa lista  8)Imp. HTML 0)Sai\n");
			mostrar("T");
			inicio=0;


		scanf("%d",&choice);
		SetColor2(WHITE2);
		switch(choice) {
			case 1:
				inserir();
				break;
			case 2:
				deletar();
				break;
			case 3:
				alterar();
				break;
			case 4:
				mostrar("T");
				system("PAUSE");
				break;
			case 5:
				mostrar("R");
				system("PAUSE");
				break;
			case 6:
				mostrar("D");
				system("PAUSE");
				break;
			case 7:
				emptylist();
				system("PAUSE");
				break;
			case 8:
				printhtml();
				break;
			case 0:
				save();
				freeList();
				break;
			default:
				printf("\nOpcao invalida\n");
		}
	}


	return 0;


}

NODE *aloca() {
	return malloc(sizeof(NODE));
}

void inserir() {

	NODE *aux,*novo;
	aux = Lista;
	int cod;
	printf("Entrada de items\n");
	repl('=',90);


	novo = aloca();
	while(aux->prox != NULL) {
		aux = aux->prox;
	}
	cod = aux->cod+1;

	novo->cod = cod;
	printf("\nitem # %d\n ",cod);
	editStrField("Descricao: ",novo->item);
	editStrField("Tipo (R)eceita (D)espesa: ",novo->tipo);
	printf("Valor: ");
	fflush(stdin);
	scanf("%f", &(novo->valor) ); /* quando é valor mnumerico tem que colocar sinal & antes do valor*/
	editStrField("Local: ",novo->local);
	editStrField("Data dd/mm/aaaa: ",novo->data);


	novo->prox = NULL;
	if(aux==NULL) {
		aux = novo;
		aux->prox = NULL;
	} else
		aux->prox = novo;
	save();
}

void mostrar(char *tipo) {

	if((Lista == NULL) || (Lista->prox == NULL)) {
		printf("Lista esta vazia\n");
		return;
	}

	NODE *aux;
	aux = Lista;
	float r,d;

	char tipoSaldo[10];
	repl('=',90);
	if(strcmp(tipo,"T")==0)
	printf("RECEITAS E DESPESAS CADASTRADAS\n");
	else if(strcmp(tipo,"D")==0)
	printf("DESPESAS\n");
	else if(strcmp(tipo,"R")==0)
	printf("RECEITAS\n");

	repl('=',90);
	printf("%-4s %-30s %-5s %-8s %-25s %s\n","#", "Item","Tipo","Valor","Local","Data")	;
	repl('-',90);

	while(aux != NULL) {
		if( aux->cod==0) {
			aux = aux->prox;
			continue;
		}

		if(strcmp(tipo,"T")!=0)
			if(strcmp(aux->tipo,tipo)!=0) {
				aux = aux->prox;
				continue;
			}

		/*TOTALIZA GANHOS GASTOS E SALDO*/
		if (strcmp(strupr(aux->tipo),"D") ==0 ) { /* camparar strings tem que usar fncao strcmp()*/
			SetColor(LIGHTRED);
			d+=aux->valor;
		} else if (strcmp(strupr(aux->tipo),"R") ==0 ) {
			SetColor(LIGHTCYAN);
			r+=aux->valor;
		} else
			SetColor(WHITE);

		printf("%-4d %-30s %-5s %-8.2f %-25s %s\n",aux->cod, strupr(aux->item),strupr(aux->tipo)
		       ,aux->valor,strupr(aux->local),aux->data);
		SetColor(WHITE);
		aux = aux->prox;
	}
	SetColor(WHITE);
	repl('-',90);
	if(strcmp(tipo,"T")==0) {
		printf("Totais >> Ganhos=R$ %.2f - Gastos=R$ %8.2f - Saldo=R$ %.2f",r,d,r-d);
		if(r>d) {
			SetColor(LIGHTCYAN);
			printf(" POSITIVO\n");
		}

		else {
			SetColor(LIGHTRED);
			printf(" NEGATIVO\n");

		}
	} else if(strcmp(tipo,"R")==0)
		printf("Total receitas:=R$ %.2f\n",r);
	else if(strcmp(tipo,"D")==0)
		printf("Total despesas:=R$ %.2f\n",d);
	SetColor(WHITE);
	repl('-',90);
}

void emptylist() {
	if(Lista->prox == NULL) {
		printf("Lista ja esta vazia\n");
		return;
	}
	freeList();
	Lista = aloca();
	Lista->prox = NULL;
	if(remove("gastos.txt") == 0)
		printf("Lista esvaziada\n");
	else
		printf("Erro ao esvaziar lista\n");
	system("PAUSE");


}


void freeList() {
	/*LIMPA LISTA - LIBERA MEMORIA OCUPADA PELOS NODES*/
	NODE* tmp;
	while (Lista != NULL) {
		tmp = Lista;
		Lista = Lista->prox;
		free(tmp);
	}
}


void deletar() {
	int cod;
	NODE *aux,*atual;
	aux = Lista;
	mostrar("T");
	printf("Codigo do item para deletar -1 para cancelar: ");
	fflush(stdin);
	scanf("%d",&cod);
	if(cod==-1) return;

	while ((aux->prox != NULL) && (cod != aux->cod)) {
		atual =  aux;
		aux = aux->prox;
	}
	if (aux != NULL) {
		atual->prox = aux->prox;
		printf("Item deletado: %s\n",aux->item);
	} else
		puts("Item nao encontrado\n");
	system("PAUSE");
}

void alterar() {
	int cod;
	char s[40];
	NODE *aux,*atual;
	aux = Lista;
	mostrar("T");
	printf("Codigo do item para alterar -1 para cancelar: ");
	fflush(stdin);
	scanf("%d",&cod);
	if(cod==-1) return;
	while ((aux->prox != NULL) && (cod != aux->cod)) {
		aux = aux->prox;
	}
	aux->cod = cod;
	printf("\nitem # %d\n ",cod);
	editStrField("Descricao (%s): ",aux->item);
	editStrField("Tipo (R)eceita (D)espesa (%s): ",aux->tipo);
	printf("Valor (%-8.2f): ",aux->valor);
	fflush(stdin);
	scanf("%f", &(aux->valor) ); /* quando é valor mnumerico tem que colocar sinal & antes do valor*/
	editStrField("Local (%s): ",aux->local);
	editStrField("Data dd/mm/aaaa (%s) : ",aux->data);
}

void repl(char c,int wid) {
	int i;
	char *s;
	for (i = 0;  i < wid;  i++)
		printf("%c", c);
	printf("\n");
}

void editStrField(char *label,char *valor) {
	char s[40];
	if(strlen(valor)>0)
		printf(label,valor);
	else
		printf(label);

	fflush(stdin);
	strcpy(s,valor);
	gets(s);
	if(strlen(s)>0)
		strcpy(valor,s);
	return;
}

void save() {
	FILE *f;
	f = fopen("gastos.txt","w");
	NODE *ptr;
	ptr = Lista;
	if(f == NULL) {
		printf("ERRO: NAO FOI POSSIVEL CRIAR ARQUIVO DE DADOS. ");
	}

	while(ptr != NULL) {
		if(ptr->cod > 0)
			fprintf(f,"%d|%s|%s|%-10.2f|%s|%s\n",
			        ptr->cod,strupr(ptr->item),strupr(ptr->tipo)
			        ,ptr->valor,strupr(ptr->local),ptr->data);
		ptr = ptr->prox;
	}
	fclose(f);
	return;
}

void load() {
	/*LE LINHA POR LINHA DO ARQUIVO, SEPARA POR | (STRTOK)
	ALOCA PONTEIRO NOVO E ATRIBUI DADOS SEPARADOS*/
	FILE *fp;
	char line[100];
	char *pt;
	int i = 0;

	fp = fopen ("gastos.txt","r");
	NODE *aux,*novo;
	aux = Lista;
	while(fgets(line,100,fp) != NULL) {
		if(strlen(line)<=1) continue;
		/* remove caracter de newline \n*/
		char *pos;
		if ((pos=strchr(line, '\n')) != NULL)
			*pos = '\0';


		/* PEGA PONTEIRO DO FIM DA LISTA*/
		while(aux->prox != NULL)
			aux = aux->prox;
		novo = aloca();
		pt = strtok (line,"|");

		i = 0;
		while (pt != NULL) {
			if(i==0)
				novo->cod = atoi(pt);
			else if(i==1)
				strcpy(novo->item,pt);
			else if(i==2)
				strcpy(novo->tipo,pt);
			else if(i==3)
				novo->valor = atof(pt);
			else if(i==4)
				strcpy(novo->local,pt);
			else if(i==5)
				strcpy(novo->data,pt);

			pt = strtok (NULL, "|");
			i++;
		}
		novo->prox = NULL;
		if(aux==NULL) {
			aux = novo;
			aux->prox = NULL;
		} else
			aux->prox = novo;
	}
	fclose(fp);
}

void printhtml() {
	FILE *f;
	f = fopen("gastos.html","w");
	char fontcolor[10];
	NODE *ptr;
	float r,d;
	ptr = Lista;

	if(f == NULL) {
		printf("ERRO: NAO FOI POSSIVEL CRIAR ARQUIVO DE HTML. ");
		return;
	}
	puts("Imprimindo HTML...");
	fprintf(f,"%s\n","<!DOCTYPE html>");
	fprintf(f,"%s\n","<meta charset=\"UTF8\">");
	fprintf(f,"%s\n","<html lang=\"pt-br\">");
	fprintf(f,"%s\n","<head>");

	/* CSS - ESTILOS*/
	fprintf(f,"%s\n","<style>");
	fprintf(f,"%s\n","body{font-family: Helvetica,Arial, sans-serif;}");
	fprintf(f,"%s\n","table {    border-collapse: collapse;}");
	fprintf(f,"%s\n","table, th, td {    border: 1px solid black;}");
	fprintf(f,"%s\n","</style>");

	fprintf(f,"%s\n","<title>Controle de gastos</title>");
	fprintf(f,"%s\n","</head>");
	fprintf(f,"%s\n","<body>");

	fprintf(f,"%s\n","<table  border=\"1\"”>");
	/* CABECALHO*/
	fprintf(f,"%s\n","<tr bgcolor=\"#e6ffcc\"\n>");
	fprintf(f,"%s\n","<th colspan=\"5\"> Controle de gastos</th>");
	fprintf(f,"%s\n","</tr>");
	fprintf(f,"%s\n","<tr bgcolor=\"#e6ffcc\"\n>");
	fprintf(f,"%s\n","<th>Item</th>");
	fprintf(f,"%s\n","<th>Tipo</th>");
	fprintf(f,"%s\n","<th>Valor</th>");
	fprintf(f,"%s\n","<th>Local</th>");
	fprintf(f,"%s\n","<th>Data</th>");
	fprintf(f,"%s\n","</tr>");

	while(ptr != NULL) {
		if(ptr->cod == 0) {
			ptr = ptr->prox;
			continue;
		}
		//puts(ptr->tipo);
		if(strcmp(strupr(ptr->tipo),"D")==0)
			strcpy(fontcolor,"#ffb3b3");
		else if(strcmp(strupr(ptr->tipo),"R")==0)
			strcpy(fontcolor,"#ccf5ff");
		else
			strcpy(fontcolor,"white");



		fprintf(f,"<tr bgcolor=\"%s\">\n",fontcolor);
		fprintf(f,"<td>%s</td><td>%s</td><td>%-10.2f</td><td>%s</td><td>%s</td>\n",
		        strupr(ptr->item),strupr(ptr->tipo),
		        ptr->valor,strupr(ptr->local),ptr->data);
		fprintf(f,"%s\n","</tr>");

		if (strcmp(strupr(ptr->tipo),"D") ==0 ) /* camparar strings tem que usar fncao strcmp()*/
			d+=ptr->valor;
		else if (strcmp(strupr(ptr->tipo),"R") ==0 )
			r+=ptr->valor;
		ptr = ptr->prox;
	}
	fprintf(f,"%s\n","<tr bgcolor=\"#e6ffcc\">");
	fprintf(f,"<th colspan=\"5\">Totais >> Ganhos=R$ %8.2f - Gastos=R$ %.2f - Saldo=R$ %.2f </th>\n",r,d,r-d);
	fprintf(f,"%s\n","</tr>");
	fprintf(f,"%s\n","</table>");
	fprintf(f,"%s\n","</body>");
	fprintf(f,"%s\n","</html>");
	fclose(f);
	puts("Concluido\n");


	(void)system("gastos.html");

	return;
}

void SetColor(int ForgC) {
	WORD wColor;
	/*We will need this handle to get the current background attribute*/
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;


	if(GetConsoleScreenBufferInfo(hStdOut, &csbi)) {
		wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
		SetConsoleTextAttribute(hStdOut, wColor);
	}
	return;
}

void SetColor2(char *color) {
  printf("%s",color);
  return;
}

void saycolor(char *str,int color) {

	SetColor(color);
	printf( str );
	SetColor(WHITE);
	return;
}

