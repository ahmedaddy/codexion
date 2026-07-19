/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaddy <aaddy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 14:58:49 by aaddy             #+#    #+#             */
/*   Updated: 2026/07/19 12:26:32 by aaddy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	swap_nodes(t_heap_node *node1, t_heap_node *node2)
{
	t_heap_node	tmp;

	tmp = *node1;
	*node1 = *node2;
	*node2 = tmp;
}

t_priority_queue	*create_pq_queue(int capacity)
{
	t_priority_queue	*pq;

	pq = malloc(sizeof(t_priority_queue));
	if (!pq)
		return (NULL);
	pq->heap = malloc(sizeof(t_heap_node) * capacity);
	if (!pq->heap)
	{
		free(pq);
		return (NULL);
	}
	pq->capacity = capacity;
	pq->size = 0;
	return (pq);
}

void	heap_up(t_priority_queue *pq, int index)
{
	int	parent;

	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (pq->heap[index].deadline < pq->heap[parent].deadline)
		{
			swap_nodes(&pq->heap[index], &pq->heap[parent]);
			index = parent;
		}
		else
			break ;
	}
}

void	heap_down(t_priority_queue *pq)
{
	int	left;
	int	right;
	int	smallest;
	int	index;

	index = 0;
	while (1)
	{
		smallest = index;
		left = (index * 2) + 1;
		right = (index * 2) + 2;
		if (left < pq->size
			&& pq->heap[smallest].deadline > pq->heap[left].deadline)
			smallest = left;
		if (right < pq->size
			&& pq->heap[smallest].deadline > pq->heap[right].deadline)
			smallest = right;
		if (smallest != index)
		{
			swap_nodes(&pq->heap[smallest], &pq->heap[index]);
			index = smallest;
		}
		else
			break ;
	}
}

void	pq_push(t_priority_queue *pq, int coder_id, long deadline)
{
	if (pq->size >= pq->capacity)
	{
		printf("Priority queue is full, \
cannot push coder_id: %d, deadline: %ld\n", coder_id, deadline);
		return ;
	}
	pq->heap[pq->size].coder_id = coder_id;
	pq->heap[pq->size].deadline = deadline;
	heap_up(pq, pq->size);
	pq->size++;
}

int	pq_pop(t_priority_queue *pq)
{
	int	coder_id;

	coder_id = pq->heap[0].coder_id;
	pq->heap[0] = pq->heap[pq->size - 1];
	pq->size--;
	if (pq->size > 0)
		heap_down(pq);
	return (coder_id);
}

void	clean_pq(t_priority_queue *pq)
{
	if (!pq)
		return ;
	free(pq->heap);
	free(pq);
}

// int main()
// {
// 	t_priority_queue    *pq;

// 	pq = create_pq_queue(6);
// 	pq_push(pq, 55, 99);
// 	pq_push(pq, 1, 3);
// 	pq_push(pq, 3, 7);
// 	pq_push(pq, 2, 1);
// 	pq_push(pq, 4, 6);
// 	pq_push(pq, 87, 2);
// 	// printf("%d\n", pq->heap[0].coder_id);
// 	int i = 0;
// 	int j = 0;

// 	while(i < pq->capacity)
// 	{
// 		// j = 0;
// 		// while (j < pq->size)
// 		// {
// 		// 	printf("Coder ID: %d, Deadline: %ld\n", pq->heap[j].coder_id, pq->heap[j].deadline);
// 		// 	j++;
// 		// }
// 		printf("%d\n", pq_pop(pq));
// 		i++;
// 	}
// 	return 0;
// }