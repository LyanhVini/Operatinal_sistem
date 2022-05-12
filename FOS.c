/* Simulador de Escalonamento de Processos*/
#include <stdio.h>
#include <stdlib.h>
#include<time.h>

/* Estrutura */
struct processos {
    int id;                   /* Identifição do processo*/              
    int t;                    /* Tempo de duração do processo*/
    int prioridade;           /* Prioridade do processo*/           
    int execucao;             /* Tempo de execução do processo*/
    int espera;               /* Tempo de espera do processo*/
    struct processos *prox;
};
/* Declarações de Protótipo de função */
struct processos *init_processos (int id, int t, int prioridade);
void fcfs (struct processos *proc);
void listprocs (struct processos *proc);
void prioridade (struct processos *proc);
void rr (struct processos *proc, int quantum);
void sjf (struct processos *proc);
void loterry(struct processos *proc);


 
int main (void) {
    
    struct processos *plist, *ptmp;
    plist       = init_processos(1, 10, 3);
    plist->prox = init_processos(2,  1, 1); ptmp = plist->prox;
    ptmp->prox  = init_processos(3,  2, 3); ptmp = ptmp->prox;
    ptmp->prox  = init_processos(4,  1, 4); ptmp = ptmp->prox;
    ptmp->prox  = init_processos(5,  5, 2);
    /* Simulações executadas*/
    listprocs(plist);
    fcfs(plist);
    sjf(plist);     
    prioridade(plist);
    rr(plist,0);
    void loterry();

    
    while (plist != NULL) {
        ptmp = plist;
        plist = plist->prox;
        free(ptmp);
    };
    return(0);
};

/* Inicialização de entrada da lista de processos*/
struct processos *init_processos (int id, int t, int prioridade) {
    struct processos *proc;
 proc = (struct processos*)malloc(sizeof(struct processos)); 
  if (proc == NULL) {
        printf("Erro Fatal: Falha Alocacao de memoria.\nFinalizar.\n");
        exit(1);
    };
    proc->id = id;
    proc->t = t;
    proc->prioridade = prioridade;
    proc->execucao = 0;
    proc->espera = 0;
    proc->prox = NULL;
    return(proc);
};

/* Escalonamento FCFS - o primeiro que chega 
/* é o primeiro a sair, ou seja, será executado primeiro */
void fcfs (struct processos *proc) {
    int tempo = 0, inicio, fim;
  struct processos *tmp = proc;
  printf("\tEscalonamento FCFS\n");
    printf("\n");
  while (tmp != NULL) {
    inicio = tempo;
    tempo += tmp->t;
    fim = tempo;
    printf("Processo: %d\tTempo: %d\tEspera: %d\tRetorno: %d\n", tmp->id, tempo, inicio, fim);
    tmp = tmp->prox;
  };
    printf("\n\n");
};

/* Listando Processos */
void listprocs (struct processos *proc) {
  struct processos *tmp = proc;
  printf("\t==Listagem dos Processos a serem escalonados: ==\n");
  printf("\n");
  while (tmp != NULL) {
    printf("Processo: %d\tPrioridade: %d\tTempo: %d\n", tmp->id, tmp->prioridade, tmp->t);
    tmp = tmp->prox;
  };
  printf("\n");
 };
/* Simulação de Processos por Prioridade
 * Obs: O processo de menor valor de prioridade obtem
 * prioridade maior na fila de processos */
void prioridade (struct processos *proc) {
  int tempo, inicio, fim, maior;
  struct processos *copia, *tmpsrc, *tmp, *maiorprimeiro;
  printf("\tEscalonamento por Prioridade\n");
   printf("\n");
  
     /* Replicando Lista de Processos */
  tmpsrc = proc;
  copia = tmp = NULL;
  while (tmpsrc != NULL) {
    if (copia == NULL) {
    copia = init_processos(tmpsrc->id, tmpsrc->t, tmpsrc->prioridade);
    tmp = copia;
    } else {
    tmp->prox = init_processos(tmpsrc->id, tmpsrc->t, tmpsrc->prioridade);
    tmp = tmp->prox;
    };
    tmpsrc = tmpsrc->prox;
  };
  /* Programa Principal */
  tempo = 0;
  while (copia != NULL) {
    
          /* Localiza o proximo processo */
    maiorprimeiro = NULL;
    maior = copia->prioridade;
    tmp = copia->prox;
    tmpsrc = copia;
    while (tmp != NULL) {
    if (tmp->prioridade < maior) {
      maior = tmp->prioridade;
      maiorprimeiro = tmpsrc;
    };
    tmpsrc = tmp;
    tmp = tmp->prox;
    };
     if (maiorprimeiro == NULL) {
    /* Verifica se o primeiro processo possui maior prioridade */
    inicio = tempo;
    tempo += copia->t;
    fim = tempo;
    printf("Processo: %d\tSurto: %d\tEspera: %d\tRetorno: %d\n", copia->id, tempo, inicio, fim);
    tmpsrc = copia->prox;
    free(copia);
    copia = tmpsrc;
    } else {
    /* Verifica se o primeiro processo não possui maior prioridade */
    tmp = maiorprimeiro->prox;
    inicio = tempo;
    tempo += tmp->t;
    fim = tempo;
    printf("Processo: %d\tSurto: %d\tEspera: %d\tRetorno: %d\n", tmp->id, tempo, inicio, fim);
    maiorprimeiro->prox = tmp->prox;
    free(tmp);
    };
  };
  printf("\n\n");
};
/* Escalonamento Round-Robin */
void rr (struct processos *proc, int quantum) {
  int jobsremain, passes;
  struct processos *copia, *tmpsrc, *tmp, *slot;
  printf("\tEscalonamento Round-Robin - Quantum: %d)\n", quantum);
  printf("\n");
  tmpsrc = proc;
  copia = tmp = NULL;
  while (tmpsrc != NULL) {
    if (copia == NULL) {
    copia = init_processos(tmpsrc->id, tmpsrc->t, tmpsrc->prioridade);
    tmp = copia;
    } else {
    tmp->prox = init_processos(tmpsrc->id, tmpsrc->t, tmpsrc->prioridade);
    tmp = tmp->prox;
    };
    tmpsrc = tmpsrc->prox;
  };
  /* Programa rotina de análise de prioridade  */
  jobsremain = 2;
  slot = NULL;
  while (jobsremain) {
    jobsremain = 0;
    /* Seleciona o próximo processo efetuando sua alocação */
    if (slot == NULL) {
    slot = copia;
    jobsremain = 2;
    } else {
    passes = 0;
    do {
      if (slot->prox == NULL) {
        passes++;
        slot = copia;
      } else {
        slot = slot->prox;
      };
    } while (passes <= 3 && slot->t == slot->execucao);
    if (passes <= 3) {
      jobsremain = 2;
    };
    };
    /* Executa um ciclo */
    tmp = copia;
    while (tmp != NULL) {
    if (tmp->t > tmp->execucao) {
      if (tmp == slot) {
        tmp->execucao += quantum;
      } else {
        tmp->espera += quantum;
      };
    };
    tmp = tmp->prox;
    };
  };
  /* Exibe os resultados e elimina as replicações */
  tmp = copia;
  while (tmp != NULL) {
    printf("Processo: %d\tTempo: %d\tEspera: %d\tRetorno: %d\n", tmp->id, tmp->t, tmp->espera, tmp->execucao + tmp->espera);
    tmpsrc = tmp;
    tmp = tmp->prox;
    free(tmpsrc);
  };
  printf("\n");
};

/* Escalonamento SJF*/
void sjf (struct processos *proc) {
  int tempo, inicio, fim, shortest;
  struct processos *copia, *tmpsrc, *tmp, *beforeshortest;
  printf("\tEscalonamento SJF\n");
  printf("\n");
  /* Lista de processos é replicada */
  tmpsrc = proc;
  copia = tmp = NULL;
  while (tmpsrc != NULL) {
    if (copia == NULL) {
    copia = init_processos(tmpsrc->id, tmpsrc->t, tmpsrc->prioridade);
    tmp = copia;
    } else {
    tmp->prox = init_processos(tmpsrc->id, tmpsrc->t, tmpsrc->prioridade);
    tmp = tmp->prox;
    };
    tmpsrc = tmpsrc->prox;
  };
  tempo = 0;
  while (copia != NULL) {
    /* Encontra o proximo processo*/
    beforeshortest = NULL;
    shortest = copia->t;
    tmp = copia->prox;
    tmpsrc = copia;
    while (tmp != NULL) {
    if (tmp->t < shortest) {
      shortest = tmp->t;
      beforeshortest = tmpsrc;
    };
    tmpsrc = tmp;
    tmp = tmp->prox;
    };
    /* Executa processo e remove ráplica da lista de processos */
    if (beforeshortest == NULL) {
    /* Aloca o primeiro processo caso o mesmo seja menor */
    inicio = tempo;
    tempo += copia->t;
    fim = tempo;
    printf("Processo: %d\tTempo: %d\tEspera: %d\tRetorno: %d\n", copia->id, tempo, inicio, fim);
    tmpsrc = copia;
    copia = copia->prox;
    free(tmpsrc);
    } else {
    /* Aloca o primeiro processo caso não haja 
    ocorrencia de outro menor 
    */
    tmp = beforeshortest->prox;
    inicio = tempo;
    tempo += tmp->t;
    fim = tempo;
    printf("Processo: %d\tTempo: %d\tEspera: %d\tRetorno: %d\n", tmp->id, tempo, inicio, fim);
    beforeshortest->prox = tmp->prox;
    free(tmp);
    }
  }
     printf("\n\n\n\tAperte qualquer tecla para execultar o escalonamento RR\n\n\n");
     printf("\n");
     system ("pause");  
}

// ESCALONADOR SORTEIO
// INICIANDO AS VARIÁVEIS

void loterry(struct processos *proc)
{
	int n,i,j,k,temp=65,flag=0;
	char process[20];
	int brust[20],priority[20],pos;
	int time=0,quantom=1,tbt=0;
	int z=0,lottery[20],ticket[20][20],q=0;
	printf("Selecione o número de processos: ");
	scanf("%d",&n);
	if(n<=0)
	{
		printf("\n\n::::  Invalid Value Of Number Of Process  ::::");
		exit(0);
	}
	for(i=0;i<n;i++)
	{
		process[i] = temp;
		temp+=1;
	}
	for(i=0;i<n;i++)
	{
		printf("\nEnter The Brust Time For Process %c: ",process[i]);
		scanf("%d",&brust[i]);
		printf("Enter The Priority For Process %c(b/w 1 to %d): ",process[i],n);
		scanf("%d",&priority[i]);
	}
	
	//sorting burst time, priority and process number in ascending order using selection sort
    for(i=0;i<n;i++)
    {
        pos=i;
        for(j=i+1;j<n;j++)
        {
            if(priority[j]<priority[pos])
                pos=j;
        }
 
        temp=process[i];
        process[i]=process[pos];
        process[pos]=temp;
 
        temp=brust[i];
        brust[i]=brust[pos];
        brust[pos]=temp;
 
        temp=priority[i];
        priority[i]=priority[pos];
        priority[pos]=temp;
        
        if(brust[i]<0)
		{
			flag = 1;
		}
    }
    
    if(flag==1)
    {
    	printf("\n\n:::  Invalid Time Entered ::: \n");
    	exit(0);
	}
    
    printf("\n |Priority |  Process   |  Brust |");
	
	for(i=0;i<n;i++)
	{
		printf("\n |   %d \t   |    %c    \t|   %d    |",priority[i],process[i],brust[i]);
		tbt = tbt + brust[i];
	}
	
	printf("\n\n:::::::::   Quantom time is 1 Sec   ::::::::: \n\n");
	
	//assign one or more lottery numbers to each process
	int p=1,m_ticket=0;
	printf("\n\n\nPriority  process  Brust  Lottery  Tickets");
	for(i=0;i<n;i++)
	{
		lottery[i] = (brust[i]/quantom) + (n-priority[i]);
		for (z=0;z<lottery[i];z++) 
		{
            ticket[i][z] = p++;
            m_ticket = p;
        }
      			
		printf("\n  %d\t    %c\t    %d \t    %d\t",priority[i],process[i],brust[i],lottery[i]);
		for(z=0;z<lottery[i];z++)
		{
			if(ticket[i][z]<10)
				printf("  ::%d:: ",ticket[i][z]);
			else
				printf("  ::%d::",ticket[i][z]);
    	}   
	}
	
	while(time!=tbt)
	{
	
	int winner = (rand()%m_ticket-1)+ 1;
    for(i =0;i<n;i++)
        for(z=0;z<lottery[i];z++)
            if(ticket[i][z]==winner)
                q=i;
                
    printf("\n\n-------------------------------------");
    printf("<<<<   Winner: %d   >>>>",winner);
    printf("-------------------------------------\n");
	
	if ((brust[q]>0))  
	{
        brust[q]-=quantom;
        
        if (brust[q]>0) 
		{
            time+=quantom;
        } 
		else 
		{
            time+=(brust[q]+quantom);
        }
		
		if(brust[q]<0) 
		{
                brust[q]=0;
        }
	    printf("\n\t\t\t\t  Process That Are Running Is: %c",process[q]);
	    printf("\n\t   (Total Time << Remaining Brust Time Of This Process << process ):  ( %d  <<  %d  <<  %c )\n",time,brust[q],process[q]);
       
	}
    else
    {
    	printf("\n\t\t     >>>>>>Related Process With This Ticket Has Been Completed<<<<<<\n");
	}
 }	
}
