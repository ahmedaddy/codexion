#include "codexion.h"

void	print_queue(t_priority_queue *q, int size)
{
	int i = 0;
	while (i < size)
	{
		printf("Coder ID: %d, Deadline: %ld\n", q->heap[i].coder_id, q->heap[i].deadline);
		i++;
	}
}

void	destroy_priority_queue(t_priority_queue *pq)
{
	if (pq)
	{
		if (pq->heap)
			free(pq->heap);
		free(pq);
	}
}

void	swap(t_queue_node *a, t_queue_node *b)
{
	t_queue_node tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void heap_up(t_priority_queue *pq, int index)
{
	int	parent;

	while(index > 0)
	{
		parent = (index - 1) / 2;
		if (pq->heap[index].deadline <= pq->heap[parent].deadline)
		{
			swap(&pq->heap[index], &pq->heap[parent]);
			index = parent;
		}
		else
			break;
	}
}

void heap_down(t_priority_queue *pq, int index)
{
	int smallest;
	int left;
	int right;

	while (1)
	{
		smallest = index;
		left = (index * 2) + 1;
		right = (index * 2) + 2;

		if (left && pq->heap[left].deadline < pq->heap[smallest].deadline)
			smallest = left;
		if (right && pq->heap[right].deadline < pq->heap[smallest].deadline)
			smallest = right;
		if (smallest != index)
		{
			swap(&pq->heap[smallest], &pq->heap[index]);
			printf("Swapped Coder ID: %d with Coder ID: %d at indices %d and %d\n", pq->heap[smallest].coder_id, pq->heap[index].coder_id, index, smallest);
			index = right;
		}
		else
			break ;
	}
}

t_priority_queue	*create_priority_queue(int capacity)
{
	t_priority_queue	*pq;

	if (!(pq = malloc(sizeof(t_priority_queue))))
		return (NULL);
	if (!(pq->heap = malloc(sizeof(t_queue_node) * capacity)))
	{
		free(pq);
		return (NULL);
	}
	pq->size = 0;
	pq->capacity = capacity;
	return (pq);
}

int pq_pop(t_priority_queue *pq)
{
	int	coder_id;

	if (pq->size <= 0)
		return (-1);
	coder_id = pq->heap[0].coder_id;
	pq->heap[0] = pq->heap[pq->size - 1];
	pq->size--;
	if (pq->size > 0)
		heap_down(pq, 0);
	return (coder_id);
}

void pq_push(t_priority_queue *pq, int coder_id, long deadline)
{
	if (pq->size == pq->capacity)
	{
		destroy_priority_queue(pq);
		return ;
	}
	pq->heap[pq->size].deadline = deadline;
	pq->heap[pq->size].coder_id = coder_id;
	heap_up(pq, pq->size);
	pq->size += 1;
}

int	main(int ac, char *av[])
{
	t_priority_queue *pq = create_priority_queue(5);
	pq_push(pq, 1, 10);
	pq_push(pq, 2, 5);
	pq_push(pq, 3, 15);
	pq_push(pq, 4, 7);
	pq_push(pq, 5, 3);
	// pq_push(pq, 6, 12);
	while(pq->size > 0)
	{
		print_queue(pq, pq->size);
		printf("##############################################\n");
		int coder_id = pq_pop(pq);
		printf("Popped Coder ID: %d\n", coder_id);
		printf("##############################################\n");
		if (pq->size > 0)
			print_queue(pq, pq->size);
	}
	// int coder_id = pq_pop(pq);
	// printf("%d\n", coder_id);
	// print_queue(pq, pq->size);
	// destroy_priority_queue(pq);
	return (0);
}