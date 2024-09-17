/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_controller.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eouhrich <eouhrich@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 21:48:12 by eouhrich          #+#    #+#             */
/*   Updated: 2024/09/16 15:29:50 by eouhrich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_heap	**get_heap(void)
{
	static t_heap	*head;

	return (&head);
}

// allocat memory and save to free later,
// returns pointer (void *) to the allocated memory
void	*mallocate(size_t size)
{
	t_heap	*new;
	t_heap	**heap;

	new = (t_heap *)malloc(sizeof(t_heap));
	if (new == NULL)
	{
		print_err("minishell: allocation failed, exiting ..\n");
		exiter(1);
	}
	new->ptr = malloc(size);
	if (new->ptr == NULL)
	{
		free(new);
		print_err("minishell: allocation failed, exiting ..\n");
		exiter(1);
	}
	heap = get_heap();
	if (new != NULL)
	{
		new->next = *heap;
		*heap = new;
	}
	return (new->ptr);
}

// free all alloccated memory by heap controller
void	free_all_heap(void)
{
	t_heap	**heap;
	t_heap	*tmp;

	clean_pool();
	heap = get_heap();
	while (*heap != NULL)
	{
		tmp = *heap;
		(*heap) = (*heap)->next;
		free(tmp->ptr);
		free(tmp);
	}
}

static int	check_first(t_heap **heap, t_heap *tmp, void *ptr)
{
	if (ptr == tmp->ptr)
	{
		*heap = tmp->next;
		free(tmp->ptr);
		free(tmp);
		return (1);
	}
	return (0);
}

// free specefic ptr allocated by heap controller
void	ft_free(void *ptr)
{
	t_heap	**heap;
	t_heap	*tmp;
	t_heap	*to_delete;

	heap = get_heap();
	tmp = *heap;
	if (tmp == NULL)
		return ;
	if (check_first(heap, tmp, ptr))
		return ;
	while (tmp != NULL && tmp->next != NULL)
	{
		if (tmp->next->ptr == ptr)
		{
			to_delete = tmp->next;
			tmp->next = tmp->next->next;
			free(to_delete->ptr);
			free(to_delete);
			return ;
		}
		tmp = tmp->next;
	}
}
