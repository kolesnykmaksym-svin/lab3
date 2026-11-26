#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node
{
    char symbol;
    struct Node* next;
} Node;

typedef struct
{
    Node* top;
} Stack;

int is_empty(Stack* stack)
{
    return stack->top == NULL;
}

int is_full(Stack* stack)
{
    Node* temp = malloc(sizeof(Node));
    (void)stack;
    if (temp == NULL)
        return 1;
    free(temp);
    return 0;
}

int push(Stack* stack, char symbol)
{
    Node* newNode = malloc(sizeof(Node));
    if (newNode == NULL)
        return 0;
    newNode->symbol = symbol;
    newNode->next = stack->top;
    stack->top = newNode;
    return 1;
}

int pop(Stack* stack, char* symbol)
{
    Node* temp = stack->top;
    if (temp == NULL)
        return 0;
    *symbol = temp->symbol;
    stack->top = temp->next;
    free(temp);
    return 1;
}

int peek(Stack* stack, char* symbol)
{
    if (is_empty(stack))
        return 0;
    *symbol = stack->top->symbol;
    return 1;
}

void clear_stack(Stack* stack)
{
    char temp;
    while (pop(stack, &temp))
    {
    }
}

int is_operand(char c)
{
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int precedence(char op)
{
    if (op == '*' || op == '/')
        return 2;
    return op == '+' || op == '-';
}

int infix_to_postfix(char infix[], char postfix[])
{
    Stack stack = {0};
    char topOp = 0;
    int ok = 1;
    int j = 0;

    for (int i = 0; infix[i] != '\0' && ok; i++)
    {
        char c = infix[i];
        if (is_operand(c))
        {
            postfix[j++] = c;
            if (!is_operand(infix[i + 1]))
                postfix[j++] = ' ';
        }
        else if (c == '(' || precedence(c) > 0)
        {
            while (c != '(' && peek(&stack, &topOp) && precedence(topOp) >= precedence(c))
            {
                pop(&stack, &topOp);
                j += sprintf(postfix + j, "%c ", topOp);
            }
            if (!push(&stack, c))
            {
                printf("Stack overflow: cannot push '%c' at position %d\n", c, i + 1);
                ok = 0;
            }
        }
        else if (c == ')')
        {
            topOp = 0;
            while (pop(&stack, &topOp) && topOp != '(')
                j += sprintf(postfix + j, "%c ", topOp);
            if (topOp != '(')
            {
                printf("Stack is empty: extra closing bracket ')' at position %d\n", i + 1);
                ok = 0;
            }
        }
        else if (c != ' ')
        {
            printf("Invalid symbol '%c' at position %d\n", c, i + 1);
            ok = 0;
        }
    }
    while (ok && pop(&stack, &topOp))
    {
        if (topOp == '(')
        {
            printf("Unclosed bracket '(' in the expression\n");
            ok = 0;
        }
        else
            j += sprintf(postfix + j, "%c ", topOp);
    }
    clear_stack(&stack);
    if (ok && j == 0)
        printf("Expression has no operands\n");
    if (j > 0)
        postfix[j - 1] = '\0';
    return ok && j > 0;
}

int main(void)
{
    char infix[256];
    char postfix[512];

    while (1)
    {
        printf("\nEnter an infix expression or \"stop\" to exit:\n");
        if (fgets(infix, sizeof(infix), stdin) == NULL)
            break;
        infix[strcspn(infix, "\n")] = '\0';
        if (strcmp(infix, "stop") == 0)
            break;
        if (infix[0] == '\0')
        {
            printf("Empty expression\n");
            continue;
        }
        if (infix_to_postfix(infix, postfix))
            printf("Postfix form: %s\n", postfix);
    }

    return 0;
}
