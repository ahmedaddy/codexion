/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debugger.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaddy <aaddy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 14:59:03 by aaddy             #+#    #+#             */
/*   Updated: 2026/07/23 14:17:20 by aaddy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	print_requests_queue(t_dongle *dongle)
{
	int i;

	printf("Dongle %d request queue: ", dongle->id);
	for (i = 0; i < dongle->request_queue->size; i++)
	{
		printf("%d ", dongle->request_queue->heap[i].coder_id);
	}
	printf("\n");
}