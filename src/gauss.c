/* #define DEV */

#include <stdio.h>
#include <unistd.h>
#include <math.h>

struct element
{
    unsigned int ordinal;
    struct element *next;
};

#ifdef DEV
static void print_state(unsigned int, double[]);
#endif

int main(int argc, char *argv[])
{
    /* NOTE: count, i, j, k can be located in .ordinal in columns, rows, calculated_columns, calculated_rows for space optimization */
    unsigned int count, i, j, k;
    double *coefficients, max;
    struct element columns, rows, calculated_columns, calculated_rows, *previous_column, *previous_row, *current_column, *current_row, *max_column_preceding, *max_row_preceding, *last_calculated_column, *last_calculated_row;
    /* number of equations */
    scanf("%u", &count);
    /* allocate array of equations coefficients, list of crossed columns, list of crossed rows */
    coefficients = sbrk(((count + 1) * sizeof(double) + sizeof(struct element) * 2) * count);
    /* list of crossed columns */
    columns.next = (struct element*)&coefficients[(count + 1) * count];
    /* list of crossed columns */
    rows.next = &columns.next[count];
    /* initialize crossed columns & crossed rows */
    for(i = 0; i < count; ++i)
    {
        columns.next[i] = (struct element){.ordinal = i, .next = &columns.next[i + 1]};
        rows.next[i] = (struct element){.ordinal = i, .next = &rows.next[i + 1]};
    }
    /* initialize calculated columns & calculated rows */
    last_calculated_column = &calculated_columns;
    last_calculated_row = &calculated_rows;
    /* coefficients */
    for(i = 0; i < (count + 1) * count; ++i)
    {
        scanf("%lf", &coefficients[i]);
    }

#ifdef DEV
    printf("\n");
#endif

    /* gaussian elimination */
    for(i = 0; i < count; ++i)
    {
        /* find maximum coefficient */
        max = 0.0;
        previous_row = &rows;
        for(j = 0; j < count - i; ++j)
        {
            current_row = previous_row->next;
            previous_column = &columns;
            for(k = 0; k < count - i; ++k)
            {
                current_column = previous_column->next;
                if(fabs(coefficients[(count + 1) * current_row->ordinal + current_column->ordinal]) > fabs(max))
                {
                    max_column_preceding = previous_column;
                    max_row_preceding = previous_row;
                    max = coefficients[(count + 1) * current_row->ordinal + current_column->ordinal];
                }
                previous_column = current_column;
            }
            previous_row = current_row;
        }
        /* cross out max column & max row */
        last_calculated_column->next = max_column_preceding->next;
        last_calculated_row->next = max_row_preceding->next;
        last_calculated_column = last_calculated_column->next;
        last_calculated_row = last_calculated_row->next;
        max_column_preceding->next = last_calculated_column->next;
        max_row_preceding->next = last_calculated_row->next;

#ifdef DEV
        print_state(count, coefficients);
        printf("%lg\n\n", max);
#endif

        /* divide row by maximum coefficient */
        current_column = &columns;
        for(j = 0; j < count - i - 1; ++j)
        {
            current_column = current_column->next;
            coefficients[(count + 1) * last_calculated_row->ordinal + current_column->ordinal] /= max;
        }
        coefficients[(count + 1) * last_calculated_row->ordinal + count] /= max;
        /* add multiplicated row to other rows */
        for(j = 0; j < count; ++j)
        {
            if(last_calculated_row->ordinal != j)
            {
                current_column = &columns;
                for(k = 0; k < count - i - 1; ++k)
                {
                    current_column = current_column->next;
                    coefficients[(count + 1) * j + current_column->ordinal] -= coefficients[(count + 1) * last_calculated_row->ordinal + current_column->ordinal] * coefficients[(count + 1) * j + last_calculated_column->ordinal];
                }
                coefficients[(count + 1) * j + count] -= coefficients[(count + 1) * last_calculated_row->ordinal + count] * coefficients[(count + 1) * j + last_calculated_column->ordinal];
            }
        }

#ifdef DEV
        for(j = 0; j < count; ++j)
        {
            if(last_calculated_row->ordinal != j)
            {
                coefficients[(count + 1) * j + last_calculated_column->ordinal] = 0.0;
            }
            else
            {
                coefficients[(count + 1) * j + last_calculated_column->ordinal] = 1.0;
            }
        }
#endif

    }

#ifdef DEV
    print_state(count, coefficients);
#endif

    for(i = 0; i < count; ++i)
    {
        current_column = &calculated_columns;
        current_row = &calculated_rows;
        do
        {
            current_column = current_column->next;
            current_row = current_row->next;
        } while(current_column->ordinal != i);
        printf("%lf\n", coefficients[(count + 1) * current_row->ordinal + count]);
    }
    return 0;
    (void)argc;
    (void)argv;
}

#ifdef DEV
static void print_state(unsigned int count, double coefficients[])
{
    unsigned int i, j;
    for(i = 0; i < count; ++i)
    {
        for(j = 0; j <= count; ++j)
        {
            printf("%lf ", coefficients[(count + 1) * i + j]);
        }
        printf("\n");
    }
    printf("\n");
    return;
}
#endif
