/*
 * Program: Speedup calculation of matrix multiplication with
 *          multi-processing and multi-threading.
 * Author:  Aditya Agarwal and Kushagra Indurkhya
 * Roll# :  CS19B1003 & CS19B1017
 */
#include <stdlib.h> /* for exit, atoi */
#include <stdio.h>  /* for fprintf */
#include <errno.h>  /* for error code eg. E2BIG */
#include <getopt.h> /* for getopt */
#include <assert.h> /* for assert */
#include <pthread.h>/* for threading */
#include <time.h>   /* for time calculations */
#include <unistd.h> 
#include <sys/wait.h>
#include <sys/types.h>
/*
 * Forward declarations
 */
void usage(int argc, char *argv[]);
void checkMultiplicable();
void init_matrix();
void allocateMemory();
void freeMemory();
void input_matrix(int *mat, int nrows, int ncols);
void output_matrix(int *mat, int nrows, int ncols);
void row_multiplier(int i);
unsigned long long single_thread_mm();
void *multi_thread_helper(void *arg);
unsigned long long multi_thread_mm();
void mult_process_helper(int i);
unsigned long long multi_process_mm();


int *A, *B, *C;
int crows, ccols;
int arows, acols, brows, bcols;
char interactive = 0;

int main(int argc, char *argv[])
{
	int c;
	/* Loop through each option (and its's arguments) and populate variables */
	while (1) {
		int this_option_optind = optind ? optind : 1;
		int option_index = 0;
		static struct option long_options[] = {
			{"help",	no_argument,		0, 'h'},
			{"ar",		required_argument,	0, '1'},
			{"ac",		required_argument,	0, '2'},
			{"br",		required_argument,	0, '3'},
			{"bc",		required_argument,	0, '4'},
			{"interactive",	no_argument, 		0, '5'},
			{0,		0,			0,  0 }
		};

		c = getopt_long(argc, argv, "h1:2:3:4:5", long_options, &option_index);
		if (c == -1)
			break;

		switch (c) {
		case 0:
			fprintf(stdout, "option %s", long_options[option_index].name);
			if (optarg)
				fprintf(stdout, " with arg %s", optarg);
				fprintf(stdout, "\n");
			break;

		case '1':
			arows = atoi(optarg);
			break;

		case '2':
			acols = atoi(optarg);
			break;

		case '3':
			brows = atoi(optarg);
			break;

		case '4':
			bcols = atoi(optarg);
			break;

		case '5':
			interactive = 1;
			break;

		case 'h':
		case '?':
			usage(argc, argv);

		default:
			fprintf(stdout, "?? getopt returned character code 0%o ??\n", c);
			usage(argc, argv);
		}
	}
	if (optind != argc) {
		fprintf(stderr, "Unexpected arguments\n");
		usage(argc, argv);
	}


	unsigned long long time_single=0, time_multi_process=0, time_multi_thread=0;

	/* Add your code here */
    checkMultiplicable();
    allocateMemory();
    time_single = single_thread_mm();
    time_multi_thread = multi_thread_mm();
    time_multi_process = multi_process_mm();


  //Printing time outputs
	fprintf(stdout, "Time taken for single threaded: %llu us\n",
			time_single);
	fprintf(stdout, "Time taken for multi process: %llu us\n",
			time_multi_process);
	fprintf(stdout, "Time taken for multi threaded: %llu us\n",
			time_multi_thread);
	fprintf(stdout, "Speedup for multi process : %4.2f x\n",
			(double)time_single/time_multi_process);
	fprintf(stdout, "Speedup for multi threaded : %4.2f x\n",
			(double)time_single/time_multi_thread);

    freeMemory();
	exit(EXIT_SUCCESS);
}

/*
 * Show usage of the program
 */
void usage(int argc, char *argv[])
{
	fprintf(stderr, "Usage:\n");
	fprintf(stderr, "%s --ar <rows_in_A>  --ac <cols_in_A>"
			" --br <rows_in_B>  --bc <cols_in_B>"
			" [--interactive]\n",
			argv[0]);
	exit(EXIT_FAILURE);
}

/*
 * Input a given 2D matrix
 */
void input_matrix(int *mat, int rows, int cols)
{
	for (int i=0; i<rows; i++) {
		for (int j=0; j<cols; j++) {
			fscanf(stdin, "%d", mat+(i*cols+j));
		}
	}
}


/*
 * Output a given 2D matrix
 */
void output_matrix(int *mat, int rows, int cols)
{
	for (int i=0; i<rows; i++) {
		for (int j=0; j<cols; j++) {
			printf("%d ", *(mat+(i*cols+j)));
		}
		fprintf(stdout, "\n");
	}
}


void checkMultiplicable()
{
    if(acols != brows) exit(EXIT_FAILURE);
}


void allocateMemory(){
    crows = arows;
    ccols = bcols;
    A = malloc(sizeof(int)*arows*acols);
    B = malloc(sizeof(int)*brows*bcols);
    C = malloc(sizeof(int)*crows*ccols);
}


void freeMemory(){
    free(A);
    free(B);
}

void init_matrix()
{
    for(int i=0; i < (arows); i++)
        for(int j=0; j < acols; j++)
            A[j+(i*acols)] = rand()%10;

    for(int i=0; i < (brows); i++)
        for(int j=0; j < bcols; j++)
            B[j+(i*bcols)] = rand()%10;
}


void row_multiplier(int i)
{
  for(int j=0; j < ccols; j++){
        C[i*ccols+j] = 0;
        for(int k=0; k < brows; k++){
            C[i*ccols+j] += A[i*acols+k]*B[k*bcols+j];
        }
    }
}


unsigned long long single_thread_mm()
{  
    if(interactive){
        printf("Enter A:\n");
        input_matrix(A, arows, acols);
        printf("Enter B:\n");
        input_matrix(B, brows, bcols);
    } else {init_matrix();}
    time_t start = clock(), end;
    for(int i=0;i<crows;i++)
        row_multiplier(i);
    
    end = clock();
    if(interactive) {
        printf("Result:\n");
        output_matrix(C, crows, ccols);
    }
    return (end-start)/1e3;
}


unsigned long long multi_thread_mm(){
    if(interactive){
        printf("Enter A:\n");
        input_matrix(A, arows, acols);
        printf("Enter B:\n");
        input_matrix(B, brows, bcols);
    } else {init_matrix();}
    pthread_t threads[crows];
    int rc;
    long t;
    for(t=0; t<crows; t++){
        int th = pthread_create(&threads[t], NULL, multi_thread_helper, (void*)t);
        if(th)
            exit(EXIT_FAILURE);
        
    }
    time_t start = clock(),end;
    for(int i=0; i<(crows); i++) pthread_join(threads[i], NULL);
    end = clock();
    if(interactive) {
        printf("Result:\n");
        output_matrix(C, crows, ccols);
    }
    return (end-start)/1e3;
}


void *multi_thread_helper(void *arg)
{
    long t_arg = (long)arg;
    row_multiplier((int)t_arg);
    pthread_exit(NULL);
}


void mult_process_helper(int i)
{
    row_multiplier(i);
}


unsigned long long multi_process_mm()
{
    if(interactive){
        printf("Enter A:\n");
        input_matrix(A, arows, acols);
        printf("Enter B:\n");
        input_matrix(B, brows, bcols);
    } else {init_matrix();}
    
    time_t start=clock(),end;
    int status = 0;
    wait(NULL);
    pid_t child_pid, wpid;
    for (int id=0; id<crows; id++) {
        if ((child_pid = fork()) == 0) {
            mult_process_helper(id);
            exit(0);
        }
        if(child_pid < 0){
            exit(EXIT_FAILURE);
        }
    }
    while ((wpid = wait(&status)) > 0);
    end = clock();
    if(interactive) {
        printf("Result:\n");
        output_matrix(C, crows, ccols);
    }
    return (end-start)/1e3;
}