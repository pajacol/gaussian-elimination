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
#else
#define count columns.ordinal
#define i rows.ordinal
#define j calculated_columns.ordinal
#define k calculated_rows.ordinal
#endif

int main(int argc, char *argv[])
{
    /* NOTE: count, i, j, k can be located in .ordinal in columns, rows, calculated_columns, calculated_rows for space optimization */

#ifdef DEV
    unsigned int count, i, j, k;
#endif

    double *coefficients, max;
    struct element columns, rows, calculated_columns, calculated_rows, *current_column, *current_row, *last_calculated_column, *last_calculated_row;
    /* number of equations */
    scanf("%u", &count);
    /* allocate array of equations coefficients, list of crossed columns, list of crossed rows */
    coefficients = sbrk(((count + 1) * sizeof(double) + (sizeof(struct element) << 1)) * count);
    /* list of crossed columns */
    columns.next = (struct element*)&coefficients[(count + 1) * count];
    /* list of crossed rows */
    rows.next = &columns.next[count];
    /* initialize crossed columns & crossed rows */
    for(i = 0; i < count; ++i)
    {
        columns.next[i].ordinal = i;
        columns.next[i].next = &columns.next[i + 1];
        rows.next[i].ordinal = i;
        rows.next[i].next = &rows.next[i + 1];
    }
    /* initialize last calculated columns & last calculated rows */
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
        current_row = &rows;
        for(j = 0; j < count - i; ++j)
        {
            current_column = &columns;
            for(k = 0; k < count - i; ++k)
            {
                if(fabs(coefficients[(count + 1) * current_row->next->ordinal + current_column->next->ordinal]) > fabs(max))
                {
                    last_calculated_column->next = current_column;
                    last_calculated_row->next = current_row;
                    max = coefficients[(count + 1) * current_row->next->ordinal + current_column->next->ordinal];
                }
                current_column = current_column->next;
            }
            current_row = current_row->next;
        }
        /* cross out max column & max row */
        current_column = last_calculated_column->next;
        current_row = last_calculated_row->next;
        last_calculated_column->next = current_column->next;
        last_calculated_row->next = current_row->next;
        current_column->next = current_column->next->next;
        current_row->next = current_row->next->next;
        last_calculated_column = last_calculated_column->next;
        last_calculated_row = last_calculated_row->next;

#ifdef DEV
        print_state(count, coefficients);
        printf("%g\n\n", max);
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
        printf("%f\n", coefficients[(count + 1) * current_row->ordinal + count]);
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
            printf("%f ", coefficients[(count + 1) * i + j]);
        }
        printf("\n");
    }
    printf("\n");
    return;
}
#endif
