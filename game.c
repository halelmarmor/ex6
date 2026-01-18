#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "utils.h"

// Map display functions
static void displayMap(GameState* g) {
    if (!g->rooms) return;
    
    // Find bounds
    int minX = 0, maxX = 0, minY = 0, maxY = 0;
    for (Room* r = g->rooms; r; r = r->next) {
        if (r->x < minX) minX = r->x;
        if (r->x > maxX) maxX = r->x;
        if (r->y < minY) minY = r->y;
        if (r->y > maxY) maxY = r->y;
    }
    
    int width = maxX - minX + 1;
    int height = maxY - minY + 1;
    
    // Create grid
    int** grid = malloc(height * sizeof(int*));
    for (int i = 0; i < height; i++) {
        grid[i] = malloc(width * sizeof(int));
        for (int j = 0; j < width; j++) grid[i][j] = -1;
    }
    
    for (Room* r = g->rooms; r; r = r->next)
        grid[r->y - minY][r->x - minX] = r->id;
    
    printf("=== SPATIAL MAP ===\n");
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (grid[i][j] != -1) printf("[%2d]", grid[i][j]);
            else printf("    ");
        }
        printf("\n");
    }
    
    for (int i = 0; i < height; i++) free(grid[i]);
    free(grid);
}
// Monster functions
void freeMonster(void* data) {
    if (data == NULL) {
        return;
    }
    Monster* monster = (Monster*)data;
    free(monster->name);
    free(monster);
}

int compareMonsters(void* a, void* b) {
    Monster* monster1 = (Monster*)a;
    Monster* monster2 = (Monster*)b;
    int nameCmp = strcmp(monster1->name, monster2->name);
    if (nameCmp != 0) {
        return nameCmp;
    }
    if (monster1->attack != monster2->attack) {
        return monster1->attack - monster2->attack;
    }
    if (monster1->hp != monster2->hp) {
        return monster1->hp - monster2->hp;
    }
    return monster1->type - monster2->type;
}

void printMonster(void* data) {
    Monster* myMonster = (Monster*)data;
    printf("\t[%s] Type: ", myMonster->name);
    switch (myMonster -> type) {
        case PHANTOM:
            printf("Phantom");
            break;
        case SPIDER:
            printf("Spider");
            break;
        case DEMON:
            printf("Demon");
            break;
        case GOLEM:
            printf("Golem");
            break;
        case COBRA:
            printf("Cobra");
            break;
    }
    printf(", Attack: %d, HP: %d\n", myMonster -> attack, myMonster -> hp);
}

// Item functions
void freeItem(void* data) {
    if (data == NULL) {
        return;
    }
    Item* item = (Item*)data;
    free(item->name);
    free(item);
}

int compareItems(void* a, void* b) {
    Item* item1 = (Item*)a;
    Item* item2 = (Item*)b;
    int nameCmp = strcmp(item1->name, item2->name);
    if (nameCmp != 0) {
        return nameCmp;
    }
    if (item1->value != item2->value) {
        return item1->value - item2->value;
    }
    return item1->type - item2->type;
}

void printItem(void* data){
    Item* i = (Item*)data;
    if (i->type == ARMOR) {
        printf("\t[ARMOR] %s - Value: %d\n", i->name, i->value);
    } else {
        printf("\t[SWORD] %s - Value: %d\n", i->name, i->value);
    }
}

// Game functions
Room* findRoomById(GameState* g, int id) {
    Room* current = g->rooms;
    while (current != NULL) {
        if (current->id == id) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}
int roomExistsAt(GameState* g, int x, int y) {
    Room* current = g->rooms;
    while (current != NULL) {
        if (current->x == x && current->y == y) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}
void printRoomLegend(GameState *g) {
    printf("=== ROOM LEGEND ===\n");
    int maxId = -1;
    Room* current = g->rooms;
    while (current != NULL) {
        if (current->id > maxId) {
            maxId = current->id;
        }
        current = current->next;
    }
    for (int id = maxId; id >= 0; id--) {
        current = g->rooms;
        while (current != NULL) {
            if (current->id == id) {
                printf("ID %d: [M:%c] [I:%c]\n", current->id,
                    current->monster? 'V' : 'X', current->item? 'V' : 'X');
                break;
            }
            current = current->next;
        }
    }
    printf("===================\n");
}
void addRoom(GameState* g) {
    if (g->rooms == NULL) {
        Room* r = malloc(sizeof(Room));
        r->id = 0;
        r->x = 0;
        r->y = 0;
        r->visited= 0;
        r->monster = NULL;
        r->item = NULL;
        r->next = NULL;
        g->rooms = r;
        g->roomCount = 1;
        int addMonster = getInt("Add monster? (1=Yes, 0=No) ");
        if (addMonster ==1) {
            char* monsterName = getString("Monster name: ");
            int monsterType = getInt("Type (0-4): ");
            int monsterHp = getInt("HP: ");
            int monsterAttack = getInt("Attack: ");
            Monster* m = malloc(sizeof(Monster));
            m->name = monsterName;
            m->type = monsterType;
            m->hp = monsterHp;
            m->maxHp = monsterHp;
            m->attack = monsterAttack;
            r->monster = m;
        }
        int addItem = getInt("Add item? (1=Yes, 0=No): ");
        if (addItem == 1) {
            char* itemName = getString("Item name: ");
            int itemType = getInt("Type (0=Armor, 1=Sword): ");
            int itemValue = getInt("Value: ");
            Item* item = malloc(sizeof(Item));
            item->name = itemName;
            item->type = itemType;
            item->value = itemValue;
            r->item = item;
        }
        printf("Created room 0 at (0,0)\n");
        return;
    }
    displayMap(g);
    printRoomLegend(g);
    int baseId = getInt("Attach to room ID: \n");
    int direction = getInt("Direction (0=Up,1=Down,2=Left,3=Right): \n");
    Room* base = findRoomById(g, baseId);
    int newX = base->x;
    int newY = base->y;
    if (direction == 0) newY--;
    else if (direction == 1) newY++;
    else if (direction == 2) newX--;
    else if (direction == 3) newX++;
    if (roomExistsAt(g, newX, newY)) {
        printf("Room exists there\n");
        return;
    }
    Room* r = malloc(sizeof(Room));
    if (r == NULL) {
        return;
    }
    r->id = g->roomCount;
    r->x = newX;
    r->y = newY;
    r->visited = 0;
    r->monster = NULL;
    r->item = NULL;
    r->next = g->rooms;
    g->rooms = r;
    g->roomCount++;
    int addMonster = getInt("Add monster? (1=Yes, 0=No): ");
    if (addMonster ==1) {
        char* monsterName = getString("Monster name: ");
        int monsterType = getInt("Type (0-4): ");
        int monsterHp = getInt("HP: ");
        int monsterAttack = getInt("Attack: ");
        Monster* m = malloc(sizeof(Monster));
        m->name = monsterName;
        m->type = monsterType;
        m->hp = monsterHp;
        m->maxHp = monsterHp;
        m->attack = monsterAttack;
        r->monster = m;
    }
    int addItem = getInt("Add item? (1=Yes, 0=No): ");
    if (addItem == 1) {
        char* itemName = getString("Item name: ");
        int itemType = getInt("Type (0=Armor, 1=Sword): ");
        int itemValue = getInt("Value: ");
        Item* item = malloc(sizeof(Item));
        item->name = itemName;
        item->type = itemType;
        item->value = itemValue;
        r->item = item;
    }
    printf("Created room %d at (%d,%d)\n", r->id, r->x, r->y);
}

void initPlayer(GameState* g) {
    if (g->rooms == NULL) {
        printf("Create rooms first\n");
        return;
    }
    if (g->player != NULL) {
        if (g->player->bag != NULL) {
            bstFree(g->player->bag->root, freeItem);
            free(g->player->bag);
        }
        if (g->player->defeatedMonsters != NULL) {
            bstFree(g->player->defeatedMonsters->root, freeMonster);
            free(g->player->defeatedMonsters);
        }
        free(g->player);
    }
    Player* p = malloc(sizeof(Player));
    if (p == NULL) {
        return;
    }
    p->hp = g->configMaxHp;
    p->maxHp = g->configMaxHp;
    p->baseAttack = g->configBaseAttack;
    p->bag = createBST(compareItems, printItem, freeItem);
    p->defeatedMonsters = createBST(compareMonsters, printMonster, freeMonster);
    p->currentRoom = g->rooms;
    g->player = p;
}

void playGame(GameState* g) {
    if (g->player == NULL) {
        printf("Init player first\n");
        return;
    }
    while (1) {
        displayMap(g);
        printRoomLegend(g);
        Room* r = g->player->currentRoom;
        printf (" --- Room %d ---\n", r->id);
        if (r->monster != NULL) {
            printf("Monster: %s (HP:%d)\n", r->monster->name, r->monster->hp);
        }
        if (r->item != NULL) {
            printf("Item: %s\n", r->item->name);
        }
        printf("HP: %d/%d\n", g->player->hp, g->player->maxHp);
        int choice = getInt("1.Move 2.Fight 3.Pickup 4.Bag 5.Defeated 6.Quit\n");
        switch (choice) {
            case 1: {
                if (r->monster != NULL) {
                    printf("Kill monster first\n");
                    break;
                }
                int direction = getInt("Direction (0=Up,1=Down,2=Left,3=Right): ");
                int newX = r->x;
                int newY = r->y;
                if (direction == 0) newY--;
                else if (direction == 1) newY++;
                else if (direction == 2) newX--;
                else newX++;
                Room* target = g->rooms;
                int found = 0;
                while (target != NULL) {
                    if (target->x == newX && target->y == newY) {
                        g->player->currentRoom = target;
                        target->visited = 1;
                        found = 1;
                        break;
                    }
                    target = target->next;
                }
                if (!found) {
                    printf("No room there\n");
                }
                break;
            }
            case 2: {
                if (r->monster == NULL) {
                    printf("No monster\n");
                    break;
                }
                Monster* m = r->monster;
                while (m->hp > 0 && g->player->hp > 0) {
                    m->hp -= g->player->baseAttack;
                    printf("You deal %d damage. Monster HP: %d\n", g->player->baseAttack, m->hp);
                    if (m->hp <= 0) break;
                    g->player->hp -= m->attack;
                    printf("Monster deals %d damage. Your HP: %d\n", m->attack, g->player->hp);
                }
                    if (g->player->hp <= 0) {
                    printf("--- YOU DIED ---\n");
                    freeGame(g);
                    exit(0);
                    }
                printf("Monster defeated!\n");
                g->player->defeatedMonsters->root=bstInsert(g->player->defeatedMonsters->root, m,
                    g->player->defeatedMonsters->compare);
                r->monster = NULL;
                break;
            }
            case 3: {
                if (r->monster != NULL) {
                    printf("Kill monster first\n");
                    break;
                }
                if (r->item == NULL) {
                    printf("No item here\n");
                    break;
                }
                void* found = bstFind(g->player->bag->root, r->item, g->player->bag->compare);
                if (found != NULL) {
                    printf("Duplicate item.\n");
                    break;
                }
                printf("Picked up %s\n", r->item->name);
                g->player->bag->root = bstInsert(g->player->bag->root, r->item, g->player->bag->compare);
                r->item = NULL;
                break;
            }
            case 4: {
                printf("\n=== INVENTORY ===\n");
                int order = getInt("1.Preorder 2.Inorder 3.Postorder\n");
                if (order == 1) {
                    bstPreorder(g->player->bag->root, g->player->bag->print);
                } else if (order == 2) {
                    bstInorder(g->player->bag->root, g->player->bag->print);
                } else {
                    bstPostorder(g->player->bag->root, g->player->bag->print);
                }
                break;
            }
            case 5: {
                printf("=== DEFEATED MONSTERS ===\n");
                int order = getInt("1.Preorder 2.Inorder 3.Postorder\n");
                if (order == 1) {
                    bstPreorder(g->player->defeatedMonsters->root, g->player->defeatedMonsters->print);
                } else if (order == 2) {
                    bstInorder(g->player->defeatedMonsters->root, g->player->defeatedMonsters->print);
                } else {
                    bstPostorder(g->player->defeatedMonsters->root, g->player->defeatedMonsters->print);
                }
                break;
            }
            case 6:
                return;
            default:
                break;
        }
        int victory = 1;
        Room* current = g->rooms;
        while (current != NULL) {
            if (current->visited == 0 || current->monster != NULL) {
                victory = 0;
                break;
            }
            current = current->next;
        }
        if (victory) {
            printf("***************************************\n");
            printf("             VICTORY!                  \n");
            printf(" All rooms explored. All monsters defeated. \n");
            printf("***************************************\n");
            freeGame(g);
            exit(0);
        }
    }
}

void freeGame(GameState* g) {
    if (g == NULL) return;
    Room* current = g->rooms;
    while (current != NULL) {
        Room* next = current->next;
        if (current->monster != NULL) {
            freeMonster(current->monster);
        }
        if (current->item != NULL) {
            freeItem(current->item);
        }
        free(current);
        current = next;
    }
    if (g->player != NULL) {
        if(g->player->bag != NULL) {
            bstFree(g->player->bag->root, freeItem);
            free(g->player->bag);
        }
        if (g->player->defeatedMonsters != NULL) {
            bstFree(g->player->defeatedMonsters->root, freeMonster);
            free(g->player->defeatedMonsters);
        }
        free(g->player);
    }
    free(g);
}