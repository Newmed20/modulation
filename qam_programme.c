/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   qam_programme.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjadid <mjadid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 09:23:25 by mjadid            #+#    #+#             */
/*   Updated: 2025/01/02 09:38:03 by mjadid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <math.h>
#include <string.h>

int main()
{
    int M, m;
    int X, Y;

    while (1)
    {
        printf("\nVeuillez entrer le niveau M  : ");
        scanf("%d", &M);

        printf("Veuillez entrer le bruit de phase Bϕ : ");
        double Bphi;
        scanf("%lf", &Bphi);

        printf("Veuillez entrer le bruit de l'energie Be  : ");
        double Be;
        scanf("%lf", &Be);

        m = (int)(log(M) / log(2));

        if (pow(2, m) != M)
        {
            printf("Erreur : M doit être une puissance de 2.\n");
            return 1;
        }

        char filename[50];
        sprintf(filename, "%d-QAM.txt", M); // Nom du fichier basé sur M
        FILE *file = fopen(filename, "w");
        if (file == NULL)
        {
            printf("Erreur : Impossible de créer le fichier.\n");
            return 1;
        }

        if (m % 2 == 0)
        {
            X = (int)pow(2, m / 2);
            Y = 0;
            fprintf(file, "#TABLEAU DE CORDONNEES POUR %d-QAM#\n", M);
        }
        else
        {
            X = ((int)pow(2, (m + 1) / 2) + (int)pow(2, (m - 1) / 2)) / 2;
            if (X % 2 != 0)
                X++;
            Y = X * X - M;
            fprintf(file, "(%d colonnes ; %d lignes) - %d (nombre de symboles exclus)\n", X, X, Y);
        }

        fprintf(file, "\n%-8s|%-10s|%-10s|%-15s|%-14s|%-27s|%-14s\n", "CODE D", "CODE B", "coord", "Énergie", "Phase", "Phase_Bruitée", "Énergie_Bruitée");

        int k = (X / 2) - 1;
        int max = 2 * k + 1;
        int min = max * -1;

        int code = 0;
        for (int i = max; i >= min; i -= 2)
        {
            for (int j = max; j >= min; j -= 2)
            {
                if (code >= M)
                    break; // Quitte la boucle interne si on a généré M valeurs

                // Calculer l'énergie
                double E = pow(i, 2) + pow(j, 2);

                // Calculer la phase (en radians)
                double phase = atan2(j, i);

                // Convertir la phase en multiples de pi
                char phase_str[20];
                if (phase > 0)
                    phase = phase / M_PI;
                else
                    phase = (phase / M_PI) + 2;
                sprintf(phase_str, "%.2f*pi", phase);

                // Calcul de la phase bruitée
                char phase_noise[40];
                double phase_min = phase - Bphi;
                double phase_max = phase + Bphi;
                sprintf(phase_noise, "[%.2f*pi; %.2f*pi]", phase_min, phase_max);

                // Calcul de l'énergie bruitée
                double en_min = E - Be;
                double en_max = E + Be;
                char energie_noise[40];
                sprintf(energie_noise, "[%.2f; %.2f]", en_min, en_max);

                // Conversion du code en binaire
                char binary[33];
                for (int b = m - 1; b >= 0; b--)
                {
                    binary[m - 1 - b] = ((code >> b) & 1) ? '1' : '0';
                }
                binary[m] = '\0';

                // Écriture des résultats avec alignement
                fprintf(file, "%-8d|%-10s|(%3d,%3d) |E = %-10.2f|φ = %-10s|φ ∈%-23s|E ∈%-10s\n", code, binary, i, j, E, phase_str, phase_noise, energie_noise);
                code++;
            }
            if (code >= M)
                break; // Quitte la boucle externe si on a généré M valeurs
        }

        fclose(file);
        printf("\033[0;32m\n\nLes résultats ont été écrits dans le fichier : %s\033[0m\n", filename);
        printf("\n\n\n\n\n");
    }

    return 0;
}
