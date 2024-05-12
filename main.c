#define SEPARATOR ','

#define ADD '+'
#define SUB '-'
#define MUL '*'
#define DIV '/'

// Precendences
#define LOWEST 0
#define ADD_OP 1
#define SUB_OP 1
#define MUL_OP 2
#define DIV_OP 2

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>

void usage(FILE *stream)
{
    fprintf(stream, "Usage ./celk <input.csv>\n");
};

char *get_file_contents(const char *file_path, size_t *size)
{
    char *buffer;
    FILE *fp = fopen(file_path, "r");

    // Seeks to the end of file to get size
    fseek(fp, 0, SEEK_END);
    long found_size = ftell(fp);
    if (found_size < 0)
    {
        fprintf(stderr, "[Error] File is too big!");
        goto error;
    }

    // Reset seek
    fseek(fp, 0, SEEK_SET);
    // + 1 to store terminator
    buffer = (char *)malloc(found_size + 1);
    if (buffer == NULL)
    {
        fprintf(stderr, "[ERROR] Issue allocating memory");
        goto error;
    }

    size_t bytes_read = fread(buffer, 1, found_size, fp);
    if (bytes_read != found_size)
    {
        fprintf(stderr, "[ERROR] Problem reading file");
        goto error;
    }
    if (ferror(fp))
    {
        fprintf(stderr, "[ERROR] Problem reading file");
        goto error;
    }

    buffer[found_size] = '\0';
    if (size)
    {
        *size = found_size;
    }
    fclose(fp);
    return buffer;

error:
    // kinda like 'defer' in Go or Zig
    if (fp)
    {
        fclose(fp);
    }
    if (buffer)
    {
        free(buffer);
    }
    return NULL;
};

struct Cell
{
    char *contents;
    int r;
    int c;
};

struct Parser
{
    char ***cells;
    char *contents;
    int l;
    int r;
};

struct Parser *new_parser(char ***cell_grid)
{
    struct Parser *p = malloc(sizeof(struct Parser));
    p->cells = cell_grid;
    p->l = 0;
    p->r = 0;
    return p;
}

char *slice_str(struct Parser *p)
{
    char *slice = malloc(sizeof(char) * (p->r - p->l));
    for (int i = p->l; i < p->r; i++)
    {
        slice[i - p->l] = p->contents[i];
    }
    return slice;
}
char *copy_str(int l, int r, char *src_str)
{
    if (src_str[l] == SEPARATOR || src_str[l] == '\n')
    {
        l++;
    }
    int size = r - l;
    char *new_str = (char *)malloc(sizeof(char) * size);
    for (int i = 0; i <= size; i++)
    {
        new_str[i] = src_str[l + i];
    }
    return new_str;
};

bool is_alpha(char c)
{
    switch (c)
    {
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
    case 'G':
    case 'H':
    case 'I':
    case 'J':
    case 'K':
    case 'L':
    case 'M':
    case 'N':
    case 'O':
    case 'P':
    case 'Q':
    case 'R':
    case 'S':
    case 'T':
    case 'U':
    case 'V':
    case 'W':
    case 'X':
    case 'Y':
    case 'Z':
        return true;
    default:
        return false;
    }
    return false;
}

bool _is_number(char c)
{
    switch (c)
    {
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '0':
        return true;
    default:
        return false;
    }
}

bool is_number(char *c)
{
    for (int i = 0; i < strlen(c); i++)
    {
        if (!_is_number(c[i]))
        {
            return false;
        }
    }
    return true;
}
char *remove_spaces(char *str)
{
    int count = 0;
    int len_of_str = strlen(str);
    for (int i = 0; i < len_of_str; i++)
    {
        if (str[i] != ' ')
        {
            count++;
        }
    }
    char *new_str = (char *)malloc(sizeof(char) * count);

    int num_added = 0;
    for (int i = 0; i < len_of_str; i++)
    {
        if (str[i] != ' ')
        {
            new_str[num_added++] = str[i];
        }
    }
    return new_str;
};

int get_precedence(char c)
{
    switch (c)
    {
    case MUL:
        return MUL_OP;
    case DIV:
        return DIV_OP;
    case ADD:
        return ADD_OP;
    case SUB:
        return SUB_OP;
    default:
        return LOWEST;
    }
}

int parse_eval(char *contents, int *l, int *r, int max_len, int precedence)
{
    return -1;
}

bool is_op(char c)
{
    switch (c)
    {
    case '+':
    case '-':
    case '*':
    case '/':
        return true;
    default:
        return false;
    }
}

char *strip_whitespace(char *str)
{
    int space_count = 0;
    int len = strlen(str);
    for (int i = 0; i < len; i++)
    {
        if (str[i] == ' ')
        {
            space_count++;
        }
    }
    char *new_str = malloc(sizeof(char) * (len - space_count));
    int ix = 0;
    for (int i = 0; i < len; i++)
    {
        if (str[i] != ' ')
        {
            new_str[ix++] = str[i];
        }
    }
    return new_str;
}

int perform_op(char op, int l, int r)
{
    switch (op)
    {
    case '+':
        return l + r;
    case '-':
        return l - r;
    case '*':
        return l * r;
    case '/':
        return l / r;
    default:
        fprintf(stderr, "[ERROR] Invalid Operation");
        exit(1);
    }
}
int parse_eval_cell(struct Parser *parser, int max_len, int precendence)
{
    int l = NULL;
    while (parser->r < max_len)
    {
        char curr_char = parser->contents[parser->r];
        if (is_op(curr_char))
        {
            int new_precendence = get_precedence(curr_char);
            if (new_precendence < precendence)
            {
                if (l == NULL)
                {
                    l = atoi(slice_str(parser));
                }
                return l;
            }
            else
            {
                if (l == NULL)
                {
                    l = atoi(slice_str(parser));
                }
                parser->r++;
                parser->l = parser->r;
                int r = parse_eval_cell(parser, max_len, new_precendence);
                l = perform_op(curr_char, l, r);
            }
        }
        else
        {
            parser->r++;
        }
    }
    if (l == NULL)
    {
        return atoi(slice_str(parser));
    }
    return l;
}

void reset_parser(struct Parser *parser, char *contents)
{
    parser->contents = contents;
    parser->l = 0;
    parser->r = 0;
}
int eval_cell(struct Parser *parser, int i, int j)
{
    char *cell_contents = parser->cells[i][j];
    if (is_number(cell_contents))
    {
        return atoi(cell_contents);
    }
    else
    {
        char *contents = parser->cells[i][j];
        int max_len = strlen(contents);
        reset_parser(parser, contents);
        return parse_eval_cell(parser, max_len, LOWEST);
    }
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        usage(stderr);
        fprintf(stderr, "[ERROR] Input .csv file not provided\n");
        exit(1);
    }
    const char *file_name = argv[1];
    printf("Parsing: %s\n", file_name);

    size_t content_size;
    char *contents = get_file_contents(file_name, &content_size);
    if (contents == NULL)
    {
        fprintf(stderr, "[ERROR] Could not read file %s\n", file_name);
        exit(1);
    }

    int st = 0;
    // int ed = 0;
    int cell_count = 0;
    struct Cell *cells = malloc(100 * sizeof(struct Cell));

    int row = 0;
    int col = 0;
    for (int i = 0; i < content_size + 1; i++)
    {
        char c = contents[i];
        if (c == SEPARATOR || c == '\n' || c == '\0')
        {

            struct Cell cell = {
                .contents = remove_spaces(copy_str(st, i - 1, contents)),
                .r = row,
                .c = col,
            };
            cells[cell_count++] = cell;
            if (c == SEPARATOR)
            {
                col++;
            }
            else
            {
                col = 0;
                row++;
            }
            st = i;
        }
    }
    free(contents);

    // for (int i = 0; i < cell_count; i++)
    // {
    //     printf(
    //         "(%d, %d) [%s]\n",
    //         cells[i].r,
    //         cells[i].c,
    //         cells[i].contents);
    // }
    int num_rows = cells[cell_count - 1].r;
    int num_cols = cells[cell_count - 1].c;

    char ***cell_grid = malloc(sizeof(char *) * num_rows + 1);
    for (int i = 0; i < row; i++)
    {
        cell_grid[i] = malloc(sizeof(char *) * num_cols + 1);
    }

    for (int i = 0; i < cell_count; i++)
    {
        cell_grid[cells[i].r][cells[i].c] = strip_whitespace(cells[i].contents);
    }

    struct Parser *p = new_parser(cell_grid);
    for (int i = 0; i <= num_rows; i++)
    {
        for (int j = 0; j <= num_cols; j++)
        {
            printf("[%d]\t", eval_cell(p, i, j));
        }
        printf("\n");
    }
    return 0;
}