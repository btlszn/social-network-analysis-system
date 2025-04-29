#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_NAME_LENGTH 50
#define MAX_USERS 1000
#define MAX_CONNECTIONS 50
#define RED 0
#define BLACK 1

// Kullan�c� yap�s�
typedef struct User {
    int id;
    char name[MAX_NAME_LENGTH];
    int connections[MAX_CONNECTIONS];
    int connection_count;
    float influence_score;
    int community_id;
} User;

// K�rm�z�-Siyah A�a� D���m�
typedef struct RBNode {
    int user_id;
    int color;
    struct RBNode *left, *right, *parent;
} RBNode;

// Graf yap�s�
typedef struct Graph {
    User users[MAX_USERS];
    int user_count;
    RBNode *rb_root;
} Graph;

// Ziyaret�i kay�t yap�lar�
typedef struct {
    bool visited[MAX_USERS];
} VisitedTracker;

// DFS i�in y���n yap�s�
typedef struct {
    int items[MAX_USERS];
    int top;
} Stack;

// BFS i�in kuyruk yap�s�
typedef struct {
    int items[MAX_USERS];
    int front, rear;
} Queue;

// Y���n fonksiyonlar�
void initStack(Stack *s) {
    s->top = -1;
}

void push(Stack *s, int value) {
    if (s->top < MAX_USERS - 1) {
        s->items[++(s->top)] = value;
    }
}

int pop(Stack *s) {
    if (s->top >= 0) {
        return s->items[(s->top)--];
    }
    return -1; // Hata durumu
}

bool isStackEmpty(Stack *s) {
    return s->top == -1;
}

// Kuyruk fonksiyonlar�
void initQueue(Queue *q) {
    q->front = q->rear = -1;
}

void enqueue(Queue *q, int value) {
    if (q->rear == MAX_USERS - 1) {
        return; // Kuyruk dolu
    }
    
    if (q->front == -1) {
        q->front = 0;
    }
    
    q->items[++(q->rear)] = value;
}

int dequeue(Queue *q) {
    if (q->front == -1 || q->front > q->rear) {
        return -1; // Kuyruk bo�
    }
    
    int value = q->items[q->front++];
    
    if (q->front > q->rear) {
        q->front = q->rear = -1;
    }
    
    return value;
}

bool isQueueEmpty(Queue *q) {
    return (q->front == -1 || q->front > q->rear);
}

// K�rm�z�-Siyah A�a� fonksiyonlar�
RBNode* createNode(int user_id) {
    RBNode* node = (RBNode*)malloc(sizeof(RBNode));
    node->user_id = user_id;
    node->color = RED;
    node->left = node->right = node->parent = NULL;
    return node;
}

void leftRotate(Graph *graph, RBNode *x) {
    RBNode *y = x->right;
    x->right = y->left;
    
    if (y->left != NULL) {
        y->left->parent = x;
    }
    
    y->parent = x->parent;
    
    if (x->parent == NULL) {
        graph->rb_root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    
    y->left = x;
    x->parent = y;
}

void rightRotate(Graph *graph, RBNode *y) {
    RBNode *x = y->left;
    y->left = x->right;
    
    if (x->right != NULL) {
        x->right->parent = y;
    }
    
    x->parent = y->parent;
    
    if (y->parent == NULL) {
        graph->rb_root = x;
    } else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }
    
    x->right = y;
    y->parent = x;
}

void fixInsert(Graph *graph, RBNode *k) {
    RBNode *u;
    
    while (k != graph->rb_root && k->parent->color == RED) {
        if (k->parent == k->parent->parent->right) {
            u = k->parent->parent->left;
            
            if (u != NULL && u->color == RED) {
                u->color = BLACK;
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                k = k->parent->parent;
            } else {
                if (k == k->parent->left) {
                    k = k->parent;
                    rightRotate(graph, k);
                }
                
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                leftRotate(graph, k->parent->parent);
            }
        } else {
            u = k->parent->parent->right;
            
            if (u != NULL && u->color == RED) {
                u->color = BLACK;
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                k = k->parent->parent;
            } else {
                if (k == k->parent->right) {
                    k = k->parent;
                    leftRotate(graph, k);
                }
                
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                rightRotate(graph, k->parent->parent);
            }
        }
    }
    
    graph->rb_root->color = BLACK;
}

void insertRB(Graph *graph, int user_id) {
    RBNode *node = createNode(user_id);
    RBNode *y = NULL;
    RBNode *x = graph->rb_root;
    
    while (x != NULL) {
        y = x;
        if (node->user_id < x->user_id) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    
    node->parent = y;
    
    if (y == NULL) {
        graph->rb_root = node;
    } else if (node->user_id < y->user_id) {
        y->left = node;
    } else {
        y->right = node;
    }
    
    if (node->parent == NULL) {
        node->color = BLACK;
        return;
    }
    
    if (node->parent->parent == NULL) {
        return;
    }
    
    fixInsert(graph, node);
}

// K�rm�z�-Siyah A�a�'ta kullan�c� arama
int searchUserRB(RBNode *root, int user_id) {
    if (root == NULL) {
        return -1; // Kullan�c� bulunamad�
    }
    
    if (root->user_id == user_id) {
        return root->user_id;
    }
    
    if (user_id < root->user_id) {
        return searchUserRB(root->left, user_id);
    } else {
        return searchUserRB(root->right, user_id);
    }
}

// Graf olu�turma ve ba�latma
void initGraph(Graph *graph) {
    graph->user_count = 0;
    graph->rb_root = NULL;
}

// Yeni kullan�c� ekleme
int addUser(Graph *graph, char *name) {
    if (graph->user_count >= MAX_USERS) {
        return -1; // Maksimum kullan�c� say�s�na ula��ld�
    }
    
    int user_id = graph->user_count;
    
    strcpy(graph->users[user_id].name, name);
    graph->users[user_id].id = user_id;
    graph->users[user_id].connection_count = 0;
    graph->users[user_id].influence_score = 0.0;
    graph->users[user_id].community_id = -1; // Ba�lang��ta topluluk atanmad�
    
    // K�rm�z�-Siyah A�aca ekleme
    insertRB(graph, user_id);
    
    return graph->user_count++;
}

// Kullan�c�lar aras�nda arkada�l�k ba�lant�s� olu�turma (�ift y�nl�)
bool createFriendship(Graph *graph, int user_id1, int user_id2) {
    if (user_id1 < 0 || user_id1 >= graph->user_count || 
        user_id2 < 0 || user_id2 >= graph->user_count || 
        user_id1 == user_id2) {
        return false;
    }
    
    // Ba�lant� zaten var m� kontrol et
    for (int i = 0; i < graph->users[user_id1].connection_count; i++) {
        if (graph->users[user_id1].connections[i] == user_id2) {
            return true; // Ba�lant� zaten mevcut
        }
    }
    
    // Ba�lant� ekle (her iki y�nde)
    if (graph->users[user_id1].connection_count < MAX_CONNECTIONS) {
        graph->users[user_id1].connections[graph->users[user_id1].connection_count++] = user_id2;
    }
    
    if (graph->users[user_id2].connection_count < MAX_CONNECTIONS) {
        graph->users[user_id2].connections[graph->users[user_id2].connection_count++] = user_id1;
    }
    
    return true;
}

// DFS ile belirli mesafedeki arkada�lar� bulma
void findFriendsAtDistance(Graph *graph, int start_user_id, int distance) {
    if (start_user_id < 0 || start_user_id >= graph->user_count) {
        printf("Ge�ersiz kullan�c� ID'si!\n");
        return;
    }
    
    int *distances = (int *)malloc(graph->user_count * sizeof(int));
    bool *visited = (bool *)malloc(graph->user_count * sizeof(bool));
    
    for (int i = 0; i < graph->user_count; i++) {
        distances[i] = -1;
        visited[i] = false;
    }
    
    // BFS ile mesafeleri hesaplama
    Queue q;
    initQueue(&q);
    enqueue(&q, start_user_id);
    visited[start_user_id] = true;
    distances[start_user_id] = 0;
    
    while (!isQueueEmpty(&q)) {
        int current = dequeue(&q);
        
        // E�er istenilen mesafede de�ilse devam et
        if (distances[current] > distance) {
            continue;
        }
        
        // Bu kullan�c�n�n t�m ba�lant�lar�n� kontrol et
        for (int i = 0; i < graph->users[current].connection_count; i++) {
            int neighbor = graph->users[current].connections[i];
            
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                distances[neighbor] = distances[current] + 1;
                enqueue(&q, neighbor);
            }
        }
    }
    
    // Tam olarak istenilen mesafedeki arkada�lar� yazd�r
    printf("%d mesafe uzakl�ktaki arkada�lar (%s):\n", distance, graph->users[start_user_id].name);
    for (int i = 0; i < graph->user_count; i++) {
        if (distances[i] == distance) {
            printf("- %s (ID: %d)\n", graph->users[i].name, i);
        }
    }
    
    free(distances);
    free(visited);
}

// �ki kullan�c� aras�ndaki ortak arkada�lar� bulma
void findCommonFriends(Graph *graph, int user_id1, int user_id2) {
    if (user_id1 < 0 || user_id1 >= graph->user_count ||
        user_id2 < 0 || user_id2 >= graph->user_count) {
        printf("Ge�ersiz kullan�c� ID'si!\n");
        return;
    }
    
    printf("Ortak arkada�lar (%s ve %s):\n", 
           graph->users[user_id1].name, 
           graph->users[user_id2].name);
    
    int common_count = 0;
    
    // Her bir arkada��n ortak olup olmad���n� kontrol et
    for (int i = 0; i < graph->users[user_id1].connection_count; i++) {
        int friend1 = graph->users[user_id1].connections[i];
        
        for (int j = 0; j < graph->users[user_id2].connection_count; j++) {
            if (friend1 == graph->users[user_id2].connections[j]) {
                printf("- %s (ID: %d)\n", graph->users[friend1].name, friend1);
                common_count++;
                break;
            }
        }
    }
    
    if (common_count == 0) {
        printf("Ortak arkada� yok.\n");
    } else {
        printf("Toplam %d ortak arkada� bulundu.\n", common_count);
    }
}

// Etki alan� hesaplama - bir kullan�c�n�n ne kadar etki sahibi oldu�unu belirleme
void calculateInfluence(Graph *graph) {
    // Her kullan�c� i�in �ncelikle basit bir etki puan� hesapla
    // Ba�lang��ta ba�lant� say�s� ve ikinci derece ba�lant�lar kullan�labilir
    
    for (int i = 0; i < graph->user_count; i++) {
        // Do�rudan ba�lant�lar i�in puan
        float score = graph->users[i].connection_count;
        
        // �kinci derece ba�lant�lar i�in daha d���k bir katsay�yla puan ekle
        int second_degree = 0;
        for (int j = 0; j < graph->users[i].connection_count; j++) {
            int friend_id = graph->users[i].connections[j];
            second_degree += graph->users[friend_id].connection_count;
        }
        
        score += (float)second_degree * 0.1; // �kinci derece ba�lant�lara daha d���k a��rl�k ver
        
        graph->users[i].influence_score = score;
    }
    
    // S�ralayarak en etkili kullan�c�lar� bul
    int *sorted_indices = (int *)malloc(graph->user_count * sizeof(int));
    for (int i = 0; i < graph->user_count; i++) {
        sorted_indices[i] = i;
    }
    
    // Basit bir kabarc�k s�ralama kullan
    for (int i = 0; i < graph->user_count - 1; i++) {
        for (int j = 0; j < graph->user_count - i - 1; j++) {
            if (graph->users[sorted_indices[j]].influence_score < 
                graph->users[sorted_indices[j + 1]].influence_score) {
                int temp = sorted_indices[j];
                sorted_indices[j] = sorted_indices[j + 1];
                sorted_indices[j + 1] = temp;
            }
        }
    }
    
    // En etkili kullan�c�lar� yazd�r
    printf("En etkili kullan�c�lar:\n");
    int display_count = graph->user_count > 10 ? 10 : graph->user_count;
    
    for (int i = 0; i < display_count; i++) {
        int idx = sorted_indices[i];
        printf("%d. %s (ID: %d) - Etki Puan�: %.2f\n", 
               i + 1, graph->users[idx].name, idx, graph->users[idx].influence_score);
    }
    
    free(sorted_indices);
}

// Topluluk tespiti (basit Union-Find algoritmas� kullanarak)
void detectCommunities(Graph *graph) {
    // Union-Find yap�s� i�in parent dizisi
    int *parent = (int *)malloc(graph->user_count * sizeof(int));
    
    // Ba�lang��ta her d���m kendi toplulu�unda
    for (int i = 0; i < graph->user_count; i++) {
        parent[i] = i;
    }
    
    // Find operasyonu - bir d���m�n k�k d���m�n� (toplulu�unu) bul
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]); // Path compression
        }
        return parent[x];
    }
    
    // Union operasyonu - iki toplulu�u birle�tir
    void unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        
        if (rootX != rootY) {
            parent[rootY] = rootX;
        }
    }
    
    // T�m ba�lant�lar i�in union i�lemi uygula
    for (int i = 0; i < graph->user_count; i++) {
        for (int j = 0; j < graph->users[i].connection_count; j++) {
            unite(i, graph->users[i].connections[j]);
        }
    }
    
    // Topluluklar� hesapla
    for (int i = 0; i < graph->user_count; i++) {
        graph->users[i].community_id = find(i);
    }
    
    // Topluluklardaki kullan�c� say�lar�n� hesapla
    int *community_sizes = (int *)calloc(graph->user_count, sizeof(int));
    for (int i = 0; i < graph->user_count; i++) {
        community_sizes[graph->users[i].community_id]++;
    }
    
    // Topluluklar� yazd�r
    printf("Tespit edilen topluluklar:\n");
    
    for (int i = 0; i < graph->user_count; i++) {
        if (community_sizes[i] > 0) {
            printf("Topluluk %d (%d �ye):\n", i, community_sizes[i]);
            
            for (int j = 0; j < graph->user_count; j++) {
                if (graph->users[j].community_id == i) {
                    printf("- %s (ID: %d)\n", graph->users[j].name, j);
                }
            }
            printf("\n");
        }
    }
    
    free(parent);
    free(community_sizes);
}

// Kullan�c� bilgilerini yazd�rma
void printUserInfo(Graph *graph, int user_id) {
    if (user_id < 0 || user_id >= graph->user_count) {
        printf("Ge�ersiz kullan�c� ID'si!\n");
        return;
    }
    
    User *user = &graph->users[user_id];
    
    printf("Kullan�c� Bilgileri:\n");
    printf("ID: %d\n", user->id);
    printf("�sim: %s\n", user->name);
    printf("Arkada� Say�s�: %d\n", user->connection_count);
    printf("Etki Puan�: %.2f\n", user->influence_score);
    printf("Topluluk ID: %d\n", user->community_id);
    
    printf("Arkada�lar:\n");
    for (int i = 0; i < user->connection_count; i++) {
        int friend_id = user->connections[i];
        printf("- %s (ID: %d)\n", graph->users[friend_id].name, friend_id);
    }
}

// Bellek temizleme
void freeRBNode(RBNode *node) {
    if (node != NULL) {
        freeRBNode(node->left);
        freeRBNode(node->right);
        free(node);
    }
}

void freeGraph(Graph *graph) {
    freeRBNode(graph->rb_root);
}

// Kullan�c� giri�i i�in yard�mc� fonksiyon
int getIntInput(const char *prompt, int min, int max) {
    int value;
    char input[50];
    
    while (1) {
        printf("%s", prompt);
        fgets(input, sizeof(input), stdin);
        
        // Giri�i kontrol et
        if (sscanf(input, "%d", &value) == 1 && value >= min && value <= max) {
            return value;
        }
        
        printf("Ge�ersiz giri�! L�tfen %d ile %d aras�nda bir say� girin.\n", min, max);
    }
}

// Kullan�c� ekleme men�s�
void addUsersMenu(Graph *graph) {
    printf("\n--- Kullan�c� Ekleme Men�s� ---\n");
    
    while (1) {
        char name[MAX_NAME_LENGTH];
        printf("\nKullan�c� ad� (��kmak i�in 'q' girin): ");
        fgets(name, sizeof(name), stdin);
        
        // ��k�� kontrol�
        if (name[0] == 'q' && (name[1] == '\n' || name[1] == '\0')) {
            break;
        }
        
        // Sat�r sonu karakterini kald�r
        name[strcspn(name, "\n")] = '\0';
        
        if (strlen(name) == 0) {
            printf("Ge�ersiz isim! L�tfen tekrar deneyin.\n");
            continue;
        }
        
        int id = addUser(graph, name);
        if (id == -1) {
            printf("Maksimum kullan�c� say�s�na ula��ld�!\n");
            break;
        }
        
        printf("'%s' kullan�c�s� eklendi (ID: %d)\n", name, id);
    }
}

// Arkada�l�k ba�lant�s� ekleme men�s�
void addConnectionsMenu(Graph *graph) {
    printf("\n--- Arkada�l�k Ba�lant�lar� Ekleme Men�s� ---\n");
    
    if (graph->user_count < 2) {
        printf("En az 2 kullan�c� olmal�d�r!\n");
        return;
    }
    
    while (1) {
        // Kullan�c� listesini g�ster
        printf("\nMevcut kullan�c�lar:\n");
        for (int i = 0; i < graph->user_count; i++) {
            printf("%d: %s\n", i, graph->users[i].name);
        }
        
        int user1 = getIntInput("\n1. kullan�c� ID'si (��kmak i�in -1 girin): ", -1, graph->user_count - 1);
        if (user1 == -1) break;
        
        int user2 = getIntInput("2. kullan�c� ID'si: ", 0, graph->user_count - 1);
        
        if (user1 == user2) {
            printf("Bir kullan�c� kendisiyle arkada� olamaz!\n");
            continue;
        }
        
        if (createFriendship(graph, user1, user2)) {
            printf("%s ve %s arkada� oldu!\n", graph->users[user1].name, graph->users[user2].name);
        } else {
            printf("Ba�lant� olu�turulamad�!\n");
        }
    }
}

// Analiz men�s�
void analysisMenu(Graph *graph) {
    printf("\n--- Analiz Men�s� ---\n");
    
    while (1) {
        printf("\n1. Belirli mesafedeki arkada�lar� bul\n");
        printf("2. Ortak arkada�lar� bul\n");
        printf("3. Topluluklar� tespit et\n");
        printf("4. Etki alan�n� hesapla\n");
        printf("5. Kullan�c� bilgilerini g�ster\n");
        printf("0. Ana men�ye d�n\n");
        
        int choice = getIntInput("Se�iminiz: ", 0, 5);
        
        if (choice == 0) break;
        
        switch (choice) {
            case 1: {
                int user_id = getIntInput("Kullan�c� ID'si: ", 0, graph->user_count - 1);
                int distance = getIntInput("Mesafe (1-5): ", 1, 5);
                findFriendsAtDistance(graph, user_id, distance);
                break;
            }
            case 2: {
                int user1 = getIntInput("1. kullan�c� ID'si: ", 0, graph->user_count - 1);
                int user2 = getIntInput("2. kullan�c� ID'si: ", 0, graph->user_count - 1);
                findCommonFriends(graph, user1, user2);
                break;
            }
            case 3:
                detectCommunities(graph);
                break;
            case 4:
                calculateInfluence(graph);
                break;
            case 5: {
                int user_id = getIntInput("Kullan�c� ID'si: ", 0, graph->user_count - 1);
                printUserInfo(graph, user_id);
                break;
            }
        }
    }
}

// Ana men�
void mainMenu(Graph *graph) {
    while (1) {
        printf("\n--- Ana Men� ---\n");
        printf("1. Kullan�c� ekle\n");
        printf("2. Arkada�l�k ba�lant�lar� ekle\n");
        printf("3. Analiz yap\n");
        printf("4. ��k��\n");
        
        int choice = getIntInput("Se�iminiz: ", 1, 4);
        
        switch (choice) {
            case 1:
                addUsersMenu(graph);
                break;
            case 2:
                addConnectionsMenu(graph);
                break;
            case 3:
                analysisMenu(graph);
                break;
            case 4:
                return;
        }
    }
}

// Ana fonksiyon
int main() {
    printf("Sosyal A� Analizi ve �li�ki A�ac� Program�\n");
    printf("=========================================\n\n");
    
    Graph g;
    initGraph(&g);
    
    // Ana men�y� ba�lat
    mainMenu(&g);
    
    // Belle�i temizle
    freeGraph(&g);
    
    return 0;
}
