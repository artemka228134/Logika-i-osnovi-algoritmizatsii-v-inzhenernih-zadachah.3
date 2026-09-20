#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <windows.h>

/*
 * ============================================================================
 * Пензенский государственный университет
 * Кафедра «Вычислительная техника»
 * Дисциплина: «Логика и основы алгоритмизации в инженерных задачах»
 *
 * ЛАБОРАТОРНАЯ РАБОТА № 3 (ОСНОВНОЕ ЗАДАНИЕ)
 * Тема: «Динамические структуры данных. Приоритетная очередь»
 *
 * Выполнили: студенты группы 25ВВВ1
 *            Ячменев М. А., Аверин А. А.
 * Приняли:   к.т.н., доцент Юрова О. В.
 *            ст. преподаватель Деев М. В.
 *
 * Пенза, 2026 г.
 * ============================================================================
 */

/* Структура элемента динамического списка */
struct node {
    char inf[256];        // полезная информация (наименование объекта)
    int priority;         // приоритет элемента
    struct node *next;    // ссылка на следующий элемент списка
};

/* ============================================================================
 * БАЗОВЫЙ ОДНОСВЯЗНЫЙ СПИСОК (по методическому пособию)
 * ============================================================================ */
struct node *head = NULL;
struct node *last = NULL;

struct node *get_struct(const char *s) {
    struct node *p = (struct node*)malloc(sizeof(struct node));
    if (p == NULL) {
        printf("Ошибка выделения динамической памяти!\n");
        exit(1);
    }
    strncpy(p->inf, s, 255);
    p->inf[255] = '\0';
    p->priority = 0;
    p->next = NULL;
    return p;
}

void spstore(const char *s) {
    struct node *p = get_struct(s);
    if (head == NULL) {
        head = p;
        last = p;
    } else {
        last->next = p;
        last = p;
    }
    printf("[Список] Элемент \"%s\" добавлен в список.\n", s);
}

void review(void) {
    struct node *struc = head;
    if (struc == NULL) {
        printf("[Список] Список пуст.\n");
        return;
    }
    printf("[Список] Содержимое списка:\n");
    int i = 1;
    while (struc != NULL) {
        printf("  %d) Имя: \"%s\"\n", i++, struc->inf);
        struc = struc->next;
    }
}

struct node *find(const char *name) {
    struct node *struc = head;
    int pos = 1;
    while (struc != NULL) {
        if (strcmp(name, struc->inf) == 0) {
            printf("[Список] Элемент \"%s\" найден на позиции %d.\n", name, pos);
            return struc;
        }
        struc = struc->next;
        pos++;
    }
    printf("[Список] Элемент \"%s\" не найден!\n", name);
    return NULL;
}

void del(const char *name) {
    struct node *struc = head;
    struct node *prev = NULL;
    int flag = 0;

    if (head == NULL) {
        printf("[Список] Список пуст, удаление невозможно.\n");
        return;
    }

    if (strcmp(name, struc->inf) == 0) {
        flag = 1;
        head = struc->next;
        if (head == NULL) last = NULL;
        free(struc);
        printf("[Список] Элемент \"%s\" успешно удален.\n", name);
        return;
    }

    prev = struc;
    struc = struc->next;
    while (struc != NULL) {
        if (strcmp(name, struc->inf) == 0) {
            flag = 1;
            prev->next = struc->next;
            if (struc == last) last = prev;
            free(struc);
            printf("[Список] Элемент \"%s\" успешно удален.\n", name);
            return;
        }
        prev = struc;
        struc = struc->next;
    }

    if (flag == 0) {
        printf("[Список] Элемент \"%s\" для удаления не найден.\n", name);
    }
}

void list_clear(void) {
    struct node *curr = head;
    while (curr != NULL) {
        struct node *tmp = curr;
        curr = curr->next;
        free(tmp);
    }
    head = NULL;
    last = NULL;
}

/* ============================================================================
 * ПРИОРИТЕТНАЯ ОЧЕРЕДЬ (ОСНОВНОЕ ЗАДАНИЕ)
 * ============================================================================ */
struct node *pq_head = NULL;

void pq_enqueue(const char *name, int priority) {
    struct node *p = (struct node*)malloc(sizeof(struct node));
    if (p == NULL) {
        printf("[Приоритетная очередь] Ошибка выделения памяти!\n");
        return;
    }
    strncpy(p->inf, name, 255);
    p->inf[255] = '\0';
    p->priority = priority;
    p->next = NULL;

    if (pq_head == NULL || priority > pq_head->priority) {
        p->next = pq_head;
        pq_head = p;
    } else {
        struct node *curr = pq_head;
        while (curr->next != NULL && curr->next->priority >= priority) {
            curr = curr->next;
        }
        p->next = curr->next;
        curr->next = p;
    }
    printf("[Приоритетная очередь] Добавлен: \"%s\" (Приоритет: %d).\n", name, priority);
}

int pq_dequeue(char *out_name, int *out_priority) {
    if (pq_head == NULL) {
        printf("[Приоритетная очередь] Очередь пуста!\n");
        return 0;
    }
    struct node *temp = pq_head;
    if (out_name != NULL) strcpy(out_name, temp->inf);
    if (out_priority != NULL) *out_priority = temp->priority;
    printf("[Приоритетная очередь] Извлечен наивысший приоритет: \"%s\" (Приоритет: %d).\n",
           temp->inf, temp->priority);
    pq_head = pq_head->next;
    free(temp);
    return 1;
}

struct node *pq_find(const char *name) {
    struct node *curr = pq_head;
    int pos = 1;
    while (curr != NULL) {
        if (strcmp(curr->inf, name) == 0) {
            printf("[Приоритетная очередь] Найден элемент \"%s\" на позиции %d (Приоритет: %d).\n",
                   name, pos, curr->priority);
            return curr;
        }
        curr = curr->next;
        pos++;
    }
    printf("[Приоритетная очередь] Элемент \"%s\" не найден!\n", name);
    return NULL;
}

int pq_del(const char *name) {
    if (pq_head == NULL) {
        printf("[Приоритетная очередь] Очередь пуста, удаление невозможно.\n");
        return 0;
    }
    struct node *curr = pq_head;
    struct node *prev = NULL;

    if (strcmp(curr->inf, name) == 0) {
        pq_head = curr->next;
        free(curr);
        printf("[Приоритетная очередь] Элемент \"%s\" успешно удален.\n", name);
        return 1;
    }

    while (curr != NULL && strcmp(curr->inf, name) != 0) {
        prev = curr;
        curr = curr->next;
    }

    if (curr == NULL) {
        printf("[Приоритетная очередь] Элемент \"%s\" для удаления не найден.\n", name);
        return 0;
    }

    prev->next = curr->next;
    free(curr);
    printf("[Приоритетная очередь] Элемент \"%s\" успешно удален.\n", name);
    return 1;
}

void pq_review(void) {
    struct node *curr = pq_head;
    if (curr == NULL) {
        printf("[Приоритетная очередь] Очередь пуста.\n");
        return;
    }
    printf("[Приоритетная очередь] Содержимое (по убыванию приоритета):\n");
    int i = 1;
    while (curr != NULL) {
        printf("  %d) \"%s\" | Приоритет: %-2d %s\n", i, curr->inf, curr->priority,
               (i == 1) ? "<-- НАИВЫСШИЙ ПРИОРИТЕТ" : "");
        curr = curr->next;
        i++;
    }
}

void pq_clear(void) {
    struct node *curr = pq_head;
    while (curr != NULL) {
        struct node *tmp = curr;
        curr = curr->next;
        free(tmp);
    }
    pq_head = NULL;
}

void run_demo(void) {
    char buf[256];
    int pr = 0;
    printf("\n============================================================\n");
    printf("     ДЕМОНСТРАЦИЯ РАБОТЫ ПРИОРИТЕТНОЙ ОЧЕРЕДИ               \n");
    printf("============================================================\n");
    pq_clear();
    pq_enqueue("Фоновая_синхронизация", 1);
    pq_enqueue("Критический_сбой_системы", 10);
    pq_enqueue("Отправка_логов", 2);
    pq_enqueue("Срочный_запрос_пользователя", 8);
    pq_enqueue("Запрос_базы_данных_1", 5);
    pq_enqueue("Запрос_базы_данных_2", 5);
    pq_review();

    printf("\n[Поиск элемента]:\n");
    pq_find("Отправка_логов");
    pq_find("Несуществующий_элемент");

    printf("\n[Удаление элемента]:\n");
    pq_del("Отправка_логов");
    pq_review();

    printf("\n[Последовательное обслуживание по приоритету]:\n");
    while (pq_dequeue(buf, &pr)) {}
    pq_review();
    pq_clear();
    printf("\n============================================================\n");
    printf("               ТЕСТИРОВАНИЕ ЗАВЕРШЕНО                       \n");
    printf("============================================================\n\n");
}

int main(void) {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");

    int choice = -1;
    char name[256];
    int priority = 0;

    while (choice != 0) {
        printf("\n========================================================\n");
        printf("  ЛАБОРАТОРНАЯ РАБОТА №3: ПРИОРИТЕТНАЯ ОЧЕРЕДЬ          \n");
        printf("========================================================\n");
        printf(" 1. Запустить автоматическое тестирование\n");
        printf(" 2. Добавить элемент с приоритетом\n");
        printf(" 3. Извлечь элемент с наивысшим приоритетом\n");
        printf(" 4. Поиск элемента по имени\n");
        printf(" 5. Удалить элемент по имени\n");
        printf(" 6. Просмотреть все элементы очереди\n");
        printf(" 0. Выход из программы\n");
        printf("--------------------------------------------------------\n");
        printf("Выберите пункт меню: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Некорректный ввод!\n");
            continue;
        }

        switch (choice) {
            case 1:
                run_demo();
                break;
            case 2:
                printf("Введите наименование объекта: ");
                scanf("%s", name);
                printf("Введите приоритет (целое число): ");
                if (scanf("%d", &priority) == 1) {
                    pq_enqueue(name, priority);
                } else {
                    printf("Ошибка ввода приоритета.\n");
                    while (getchar() != '\n');
                }
                break;
            case 3:
                pq_dequeue(name, &priority);
                break;
            case 4:
                printf("Введите имя для поиска: ");
                scanf("%s", name);
                pq_find(name);
                break;
            case 5:
                printf("Введите имя для удаления: ");
                scanf("%s", name);
                pq_del(name);
                break;
            case 6:
                pq_review();
                break;
            case 0:
                printf("Очистка памяти и выход...\n");
                list_clear();
                pq_clear();
                printf("Память освобождена. Программа завершена.\n");
                break;
            default:
                printf("Неверный выбор.\n");
                break;
        }
    }
    return 0;
}
