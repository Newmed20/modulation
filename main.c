/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjadid <mjadid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 21:58:52 by mjadid            #+#    #+#             */
/*   Updated: 2024/10/29 22:48:54 by mjadid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <math.h>

int main()
{
    int M, m;
    int X, Y;
    while(1)
    {
        printf("Soit X le nombre de colonnes et de lignes.\n");
        printf("Soit Y le nombre de symboles exclus.\n");
        printf("Veuillez entrer la valeur de M (le niveau) pour calculer les valeurs de (X,Y) associées : ");
        scanf("%d", &M);

        if(M == 0)
            break;

        m = (int)(log(M) / log(2));

        if (pow(2, m) != M) {
            printf("Erreur : M doit être une puissance de 2.\n");
            return 1;
        }

        if (m % 2 == 0) {
            X = (int)pow(2, m / 2); 
            Y = 0;
            printf("\033[0;32m(%d colonnes ; %d lignes)\033[0m\n", X, X);

        } else {
            X = ((int)pow(2, (m + 1) / 2) + (int)pow(2, (m - 1) / 2)) / 2;
            if(X%2 != 0)
                X++;
            Y = X * X - M;
            printf("\033[0;32m(%d colonnes ; %d lignes) - %d (nombre de symboles exclus)\033[0m\n", X, X, Y);

        }
        printf("\n\n\n\n\n");
    }

    return 0;
}

