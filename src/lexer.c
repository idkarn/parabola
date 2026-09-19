#include "lexer.h"
#include <stdio.h>

lexer_state_t calc_next_state(char c)
{
    switch (c)
    {
    case ' ':
    case '\n':
    case '\t':
        return WAITING;
/*
 * compiler-specific code!
 * MSVC is not supported.
 */
#if defined(__GNUC__) || defined(__clang__)
    case '0' ... '9':
    case 'a' ... 'z':
    case 'A' ... 'Z':
        return TERMINAL;
    default:
        return SPECIAL1;
#else
    default:
        if ((c >= '0' && c <= '9') ||
            (c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z'))
            return TERMINAL;
        return SPECIAL1;
#endif
    }
}

// checks if the whole NULL-terminated string is a number
char isnum(char *s)
{
    char *ptr = s;
    while (*ptr != 0)
    {
        if ((*ptr < '0' || *ptr > '9') && (*ptr != '.'))
            return 0;
        ptr++;
    };
    return 1;
}

// checks if the whole NULL-terminated string is a number
char is_word_alnum(char *s)
{
    char *ptr = s;
    while (*ptr != 0)
    {
        if (isalnum(*ptr) == 0)
            return 0;
        ptr++;
    };
    return 1;
}

token_t extract_token(char *lexeme)
{
    token_t t = {};

    if (*lexeme == '(')
        t.kind = PAR_OPEN;
    else if (*lexeme == ')')
        t.kind = PAR_CLOSING;
    else if (*lexeme == '{')
        t.kind = BRACE_OPEN;
    else if (*lexeme == '}')
        t.kind = BRACE_CLOSING;
    else if (*lexeme == ';')
        t.kind = SEMICOLON;
    else if (*lexeme == '+' || *lexeme == '-' || *lexeme == '*' || *lexeme == '/' || *lexeme == '>' || *lexeme == '<' || *lexeme == '=')
    {
        t.kind = EXPR_OPR;
        strcpy(t.raw_value, lexeme);
    }
    else if (strcmp(lexeme, ":=") == 0)
        t.kind = ASSIGN;
    else if (strcmp(lexeme, "print") == 0)
        t.kind = PRINT;
    else if (strcmp(lexeme, "input") == 0)
        t.kind = INPUT;
    else if (strcmp(lexeme, "int") == 0)
        t = (token_t){ .kind = TYPE, .raw_value = "int" };
    else if (strcmp(lexeme, "float") == 0)
        t = (token_t){ .kind = TYPE, .raw_value = "float" };
    else if (isnum(lexeme) /* || check for float */)
    {
        t.kind = VALUE;
        strcpy(t.raw_value, lexeme);
    }
    else if (is_word_alnum(lexeme))
    {
        t.kind = ID;
        strcpy(t.raw_value, lexeme);
    }
    else
        t.kind = NONE;

    return t;
}

// !! possible out-of-range for `tokens`
size_t analyze_into(char *code, token_t *tokens)
{
    lexer_state_t state = WAITING;
    size_t i = 0;

    char *ptr = code;
    char temp[64] = {0};
    char tidx = 0; // temp's length

    lexer_state_t next_state;
    while (*ptr != 0)
    {
        next_state = calc_next_state(*ptr);

        if (state == WAITING && next_state != WAITING)
        {
            state = next_state;
            temp[tidx++] = *ptr;
        }
        else if (next_state == WAITING && tidx > 0)
        {
            tokens[i++] = extract_token(temp);
            memset(temp, 0, sizeof(temp));
            tidx = 0;
            state = WAITING;
        }
        else if (next_state == SPECIAL1 && *ptr == '.' && isnum(temp))
        {
            temp[tidx++] = '.';
        }
        else if (state != next_state && next_state != WAITING)
        {
            state = next_state;
            tokens[i++] = extract_token(temp);
            memset(temp, 0, sizeof(temp));
            temp[tidx = 0] = *ptr, tidx++;
        }
        else if (state == SPECIAL1 && next_state == SPECIAL1)
        {
            if ((tidx == 1) && ((*temp == ':' && *ptr == '=') || (*temp == '!' && *ptr == '=')))
            {
                temp[tidx++] = *ptr;
                state = SPECIAL2;
            }
            else
            {
                tokens[i++] = extract_token(temp);
                memset(temp, 0, sizeof(temp));
                temp[tidx = 0] = *ptr, tidx++;
            }
        }
        else if (state == next_state && next_state != WAITING)
        {
            temp[tidx++] = *ptr;
        }

        ptr++;
    }
    if (tidx > 0)
        tokens[i++] = extract_token(temp);

    return i;
}

void print_tokens(token_t *toks, size_t n)
{
    printf("TOKENS:\n");
    for (int j = 0; j < n; j++)
    {
        printf("%d, ", toks[j].kind);
    }
    printf("\nEND\n");
}