#include "pqueue.h"

int main(void) {

	PQ_Node *pq=NULL;

	PQ_Push(&pq,0,0,NULL,NULL);

	PQ_Print(&pq);

	return 0;
}