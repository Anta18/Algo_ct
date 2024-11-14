#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------ */
/*        Hash Functions          */
/* ------------------------------ */

// Integer hash function: simple modulo operation
unsigned int hash_int(int key, unsigned int table_size)
{
    return (unsigned int)(key % (int)table_size);
}

// String hash function: DJB2 algorithm
unsigned int hash_string(const char *str, unsigned int table_size)
{
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) - hash) + c; // hash * 31 + c
    return hash % table_size;
}

/* ------------------------------ */
/*      Separate Chaining         */
/* ------------------------------ */

// Key types
typedef enum
{
    INT_KEY,
    STRING_KEY
} KeyType;

// Structure for key-value pair in separate chaining
typedef struct SC_KeyValue
{
    KeyType type;
    union
    {
        int int_key;
        char *str_key;
    } key;
    void *value; // Generic pointer to store value
    struct SC_KeyValue *next;
} SC_KeyValue;

// Structure for separate chaining hash table
typedef struct SeparateChainingHashTable
{
    SC_KeyValue **buckets;
    unsigned int size;  // Number of buckets
    unsigned int count; // Number of key-value pairs
} SeparateChainingHashTable;

// Function to create a separate chaining hash table
SeparateChainingHashTable *sc_create_table(unsigned int size)
{
    SeparateChainingHashTable *table = malloc(sizeof(SeparateChainingHashTable));
    if (!table)
    {
        fprintf(stderr, "Memory allocation failed for SeparateChainingHashTable.\n");
        exit(EXIT_FAILURE);
    }
    table->size = size;
    table->count = 0;
    table->buckets = calloc(table->size, sizeof(SC_KeyValue *));
    if (!table->buckets)
    {
        fprintf(stderr, "Memory allocation failed for SeparateChainingHashTable buckets.\n");
        free(table);
        exit(EXIT_FAILURE);
    }
    return table;
}

// Function to insert a key-value pair into separate chaining hash table
void sc_insert(SeparateChainingHashTable *table, SC_KeyValue *pair);

// Function to search for a key in separate chaining hash table
SC_KeyValue *sc_search(SeparateChainingHashTable *table, SC_KeyValue *key);

// Function to delete a key from separate chaining hash table
int sc_delete(SeparateChainingHashTable *table, SC_KeyValue *key);

// Function to resize separate chaining hash table
void sc_resize(SeparateChainingHashTable *table);

// Implementation of sc_insert
void sc_insert(SeparateChainingHashTable *table, SC_KeyValue *pair)
{
    unsigned int index;
    if (pair->type == INT_KEY)
    {
        index = hash_int(pair->key.int_key, table->size);
    }
    else
    {
        index = hash_string(pair->key.str_key, table->size);
    }

    // Insert at the beginning of the linked list
    pair->next = table->buckets[index];
    table->buckets[index] = pair;
    table->count++;

    // Check load factor and resize if necessary
    float load_factor = (float)table->count / table->size;
    if (load_factor > 0.75)
    {
        sc_resize(table);
    }
}

// Implementation of sc_search
SC_KeyValue *sc_search(SeparateChainingHashTable *table, SC_KeyValue *key)
{
    unsigned int index;
    if (key->type == INT_KEY)
    {
        index = hash_int(key->key.int_key, table->size);
    }
    else
    {
        index = hash_string(key->key.str_key, table->size);
    }

    SC_KeyValue *current = table->buckets[index];
    while (current)
    {
        if (current->type == key->type)
        {
            if (key->type == INT_KEY && current->key.int_key == key->key.int_key)
            {
                return current;
            }
            if (key->type == STRING_KEY && strcmp(current->key.str_key, key->key.str_key) == 0)
            {
                return current;
            }
        }
        current = current->next;
    }
    return NULL;
}

// Implementation of sc_delete
int sc_delete(SeparateChainingHashTable *table, SC_KeyValue *key)
{
    unsigned int index;
    if (key->type == INT_KEY)
    {
        index = hash_int(key->key.int_key, table->size);
    }
    else
    {
        index = hash_string(key->key.str_key, table->size);
    }

    SC_KeyValue *current = table->buckets[index];
    SC_KeyValue *prev = NULL;

    while (current)
    {
        if (current->type == key->type)
        {
            if ((key->type == INT_KEY && current->key.int_key == key->key.int_key) ||
                (key->type == STRING_KEY && strcmp(current->key.str_key, key->key.str_key) == 0))
            {
                // Found the key to delete
                if (prev)
                {
                    prev->next = current->next;
                }
                else
                {
                    table->buckets[index] = current->next;
                }
                // Free allocated memory
                if (current->type == STRING_KEY)
                {
                    free(current->key.str_key);
                }
                // Assuming value is dynamically allocated; adjust as needed
                // free(current->value);
                free(current);
                table->count--;
                return 1; // Successfully deleted
            }
        }
        prev = current;
        current = current->next;
    }
    return 0; // Key not found
}

// Implementation of sc_resize
void sc_resize(SeparateChainingHashTable *table)
{
    unsigned int new_size = table->size * 2;
    SC_KeyValue **new_buckets = calloc(new_size, sizeof(SC_KeyValue *));
    if (!new_buckets)
    {
        fprintf(stderr, "Memory allocation failed during SeparateChainingHashTable resizing.\n");
        return;
    }

    // Rehash all existing keys
    for (unsigned int i = 0; i < table->size; i++)
    {
        SC_KeyValue *current = table->buckets[i];
        while (current)
        {
            SC_KeyValue *next = current->next;
            unsigned int index;
            if (current->type == INT_KEY)
            {
                index = hash_int(current->key.int_key, new_size);
            }
            else
            {
                index = hash_string(current->key.str_key, new_size);
            }

            // Insert into new bucket
            current->next = new_buckets[index];
            new_buckets[index] = current;

            current = next;
        }
    }

    free(table->buckets);
    table->buckets = new_buckets;
    table->size = new_size;
}

/* ------------------------------ */
/*        Open Addressing         */
/* ------------------------------ */

// Status of an entry in open addressing
typedef enum
{
    EMPTY,
    OCCUPIED,
    DELETED
} OAEntryStatus;

// Structure for key-value pair in open addressing
typedef struct OA_KeyValue
{
    KeyType type;
    union
    {
        int int_key;
        char *str_key;
    } key;
    void *value;
    OAEntryStatus status;
} OA_KeyValue;

// Structure for open addressing hash table
typedef struct OpenAddressingHashTable
{
    OA_KeyValue *entries;
    unsigned int size;  // Size of the table
    unsigned int count; // Number of OCCUPIED entries
} OpenAddressingHashTable;

// Probing strategies
typedef unsigned int (*ProbeFunction)(unsigned int hash1, int key, unsigned int i, unsigned int table_size);

// Linear Probing
unsigned int linear_probe_func(unsigned int hash1, int key, unsigned int i, unsigned int table_size)
{
    return (hash1 + i) % table_size;
}

// Quadratic Probing
unsigned int quadratic_probe_func(unsigned int hash1, int key, unsigned int i, unsigned int table_size)
{
    return (hash1 + i * i) % table_size;
}

// Double Hashing helper functions
unsigned int double_hash_second(int key, unsigned int table_size)
{
    // Second hash function must be non-zero and relatively prime to table_size
    // For simplicity, using 7 as a prime number less than table_size
    return 7 - (key % 7);
}

unsigned int double_probe_func(unsigned int hash1, int key, unsigned int i, unsigned int table_size)
{
    unsigned int hash2 = double_hash_second(key, table_size);
    return (hash1 + i * hash2) % table_size;
}

// Function to create an open addressing hash table
OpenAddressingHashTable *oa_create_table(unsigned int size)
{
    OpenAddressingHashTable *table = malloc(sizeof(OpenAddressingHashTable));
    if (!table)
    {
        fprintf(stderr, "Memory allocation failed for OpenAddressingHashTable.\n");
        exit(EXIT_FAILURE);
    }
    table->size = size;
    table->count = 0;
    table->entries = calloc(table->size, sizeof(OA_KeyValue));
    if (!table->entries)
    {
        fprintf(stderr, "Memory allocation failed for OpenAddressingHashTable entries.\n");
        free(table);
        exit(EXIT_FAILURE);
    }
    // Initialize all entries as EMPTY
    for (unsigned int i = 0; i < table->size; i++)
    {
        table->entries[i].status = EMPTY;
    }
    return table;
}

// Function to insert a key-value pair into open addressing hash table
void oa_insert(OpenAddressingHashTable *table, OA_KeyValue pair, ProbeFunction probe_func);

// Function to search for a key in open addressing hash table
OA_KeyValue *oa_search(OpenAddressingHashTable *table, OA_KeyValue key, ProbeFunction probe_func);

// Function to delete a key from open addressing hash table
int oa_delete(OpenAddressingHashTable *table, OA_KeyValue key, ProbeFunction probe_func);

// Function to resize open addressing hash table
void oa_resize(OpenAddressingHashTable *table, unsigned int new_size, ProbeFunction probe_func);

// Implementation of oa_insert
void oa_insert(OpenAddressingHashTable *table, OA_KeyValue pair, ProbeFunction probe_func)
{
    // Check load factor and resize if necessary
    float load_factor = (float)table->count / table->size;
    if (load_factor >= 0.7)
    {
        // Choose a new size (next prime number is ideal, but for simplicity, double the size)
        unsigned int new_size = table->size * 2;
        oa_resize(table, new_size, probe_func);
    }

    unsigned int hash1;
    if (pair.type == INT_KEY)
    {
        hash1 = hash_int(pair.key.int_key, table->size);
    }
    else
    {
        hash1 = hash_string(pair.key.str_key, table->size);
    }

    unsigned int first_deleted = table->size; // To track first DELETED slot

    for (unsigned int i = 0; i < table->size; i++)
    {
        unsigned int index = probe_func(hash1, (pair.type == INT_KEY) ? pair.key.int_key : 0, i, table->size);
        if (table->entries[index].status == EMPTY)
        {
            if (first_deleted != table->size)
            {
                index = first_deleted;
            }
            table->entries[index] = pair;
            table->entries[index].status = OCCUPIED;
            table->count++;
            return;
        }
        else if (table->entries[index].status == DELETED)
        {
            if (first_deleted == table->size)
            {
                first_deleted = index;
            }
        }
        else if (table->entries[index].status == OCCUPIED)
        {
            // Check if the key already exists and update
            int keys_equal = 0;
            if (pair.type == INT_KEY && table->entries[index].type == INT_KEY &&
                pair.key.int_key == table->entries[index].key.int_key)
            {
                keys_equal = 1;
            }
            if (pair.type == STRING_KEY && table->entries[index].type == STRING_KEY &&
                strcmp(pair.key.str_key, table->entries[index].key.str_key) == 0)
            {
                keys_equal = 1;
            }
            if (keys_equal)
            {
                table->entries[index].value = pair.value;
                return;
            }
        }
    }

    // If we reach here, table is full
    printf("Open Addressing Hash Table is full! Insertion failed.\n");
}

// Implementation of oa_search
OA_KeyValue *oa_search(OpenAddressingHashTable *table, OA_KeyValue key, ProbeFunction probe_func)
{
    unsigned int hash1;
    if (key.type == INT_KEY)
    {
        hash1 = hash_int(key.key.int_key, table->size);
    }
    else
    {
        hash1 = hash_string(key.key.str_key, table->size);
    }

    for (unsigned int i = 0; i < table->size; i++)
    {
        unsigned int index = probe_func(hash1, (key.type == INT_KEY) ? key.key.int_key : 0, i, table->size);
        if (table->entries[index].status == EMPTY)
        {
            return NULL; // Key not found
        }
        if (table->entries[index].status == OCCUPIED)
        {
            int keys_equal = 0;
            if (key.type == INT_KEY && table->entries[index].type == INT_KEY &&
                key.key.int_key == table->entries[index].key.int_key)
            {
                keys_equal = 1;
            }
            if (key.type == STRING_KEY && table->entries[index].type == STRING_KEY &&
                strcmp(key.key.str_key, table->entries[index].key.str_key) == 0)
            {
                keys_equal = 1;
            }
            if (keys_equal)
            {
                return &table->entries[index];
            }
        }
    }
    return NULL; // Key not found after full table probe
}

// Implementation of oa_delete
int oa_delete(OpenAddressingHashTable *table, OA_KeyValue key, ProbeFunction probe_func)
{
    unsigned int hash1;
    if (key.type == INT_KEY)
    {
        hash1 = hash_int(key.key.int_key, table->size);
    }
    else
    {
        hash1 = hash_string(key.key.str_key, table->size);
    }

    for (unsigned int i = 0; i < table->size; i++)
    {
        unsigned int index = probe_func(hash1, (key.type == INT_KEY) ? key.key.int_key : 0, i, table->size);
        if (table->entries[index].status == EMPTY)
        {
            return 0; // Key not found
        }
        if (table->entries[index].status == OCCUPIED)
        {
            int keys_equal = 0;
            if (key.type == INT_KEY && table->entries[index].type == INT_KEY &&
                key.key.int_key == table->entries[index].key.int_key)
            {
                keys_equal = 1;
            }
            if (key.type == STRING_KEY && table->entries[index].type == STRING_KEY &&
                strcmp(key.key.str_key, table->entries[index].key.str_key) == 0)
            {
                keys_equal = 1;
            }
            if (keys_equal)
            {
                // Free allocated memory for string keys
                if (table->entries[index].type == STRING_KEY)
                {
                    free(table->entries[index].key.str_key);
                }
                // Assuming value is dynamically allocated; adjust as needed
                // free(table->entries[index].value);
                table->entries[index].status = DELETED;
                table->count--;
                return 1; // Successfully deleted
            }
        }
    }
    return 0; // Key not found after full table probe
}

// Implementation of oa_resize
void oa_resize(OpenAddressingHashTable *table, unsigned int new_size, ProbeFunction probe_func)
{
    OA_KeyValue *old_entries = table->entries;
    unsigned int old_size = table->size;

    // Create new table
    OA_KeyValue *new_entries = calloc(new_size, sizeof(OA_KeyValue));
    if (!new_entries)
    {
        fprintf(stderr, "Memory allocation failed during OpenAddressingHashTable resizing.\n");
        return;
    }
    for (unsigned int i = 0; i < new_size; i++)
    {
        new_entries[i].status = EMPTY;
    }

    // Update table properties
    table->entries = new_entries;
    table->size = new_size;
    table->count = 0;

    // Re-insert old OCCUPIED entries
    for (unsigned int i = 0; i < old_size; i++)
    {
        if (old_entries[i].status == OCCUPIED)
        {
            oa_insert(table, old_entries[i], probe_func);
        }
    }

    // Free old entries
    free(old_entries);
}

/* ------------------------------ */
/*          Helper Functions       */
/* ------------------------------ */

// Function to create a key-value pair for separate chaining
SC_KeyValue *sc_create_pair(KeyType type, int int_key, const char *str_key, void *value)
{
    SC_KeyValue *pair = malloc(sizeof(SC_KeyValue));
    if (!pair)
    {
        fprintf(stderr, "Memory allocation failed for SC_KeyValue.\n");
        exit(EXIT_FAILURE);
    }
    pair->type = type;
    if (type == INT_KEY)
    {
        pair->key.int_key = int_key;
    }
    else
    {
        pair->key.str_key = strdup(str_key);
        if (!pair->key.str_key)
        {
            fprintf(stderr, "Memory allocation failed for SC_KeyValue string key.\n");
            free(pair);
            exit(EXIT_FAILURE);
        }
    }
    pair->value = value;
    pair->next = NULL;
    return pair;
}

// Function to create a key-value pair for open addressing
OA_KeyValue oa_create_oa_pair(KeyType type, int int_key, const char *str_key, void *value)
{
    OA_KeyValue pair;
    pair.type = type;
    if (type == INT_KEY)
    {
        pair.key.int_key = int_key;
    }
    else
    {
        pair.key.str_key = strdup(str_key);
        if (!pair.key.str_key)
        {
            fprintf(stderr, "Memory allocation failed for OA_KeyValue string key.\n");
            exit(EXIT_FAILURE);
        }
    }
    pair.value = value;
    pair.status = OCCUPIED;
    return pair;
}

/* ------------------------------ */
/*            Main Function        */
/* ------------------------------ */

int main()
{
    /* ------------------------------ */
    /*       Separate Chaining         */
    /* ------------------------------ */

    printf("=== Separate Chaining Hash Table ===\n");

    // Create separate chaining hash table with initial size 5
    SeparateChainingHashTable *sc_table = sc_create_table(5);

    // Insert integer key
    SC_KeyValue *sc_kv1 = sc_create_pair(INT_KEY, 42, NULL, "The answer");
    sc_insert(sc_table, sc_kv1);

    // Insert string key
    SC_KeyValue *sc_kv2 = sc_create_pair(STRING_KEY, 0, "hello", "world");
    sc_insert(sc_table, sc_kv2);

    // Insert another string key
    SC_KeyValue *sc_kv3 = sc_create_pair(STRING_KEY, 0, "foo", "bar");
    sc_insert(sc_table, sc_kv3);

    // Search for integer key
    SC_KeyValue sc_search_key;
    sc_search_key.type = INT_KEY;
    sc_search_key.key.int_key = 42;
    SC_KeyValue *sc_found = sc_search(sc_table, &sc_search_key);
    if (sc_found)
    {
        printf("Found INT key 42: %s\n", (char *)sc_found->value);
    }
    else
    {
        printf("INT key 42 not found.\n");
    }

    // Search for string key
    sc_search_key.type = STRING_KEY;
    sc_search_key.key.str_key = "hello";
    sc_found = sc_search(sc_table, &sc_search_key);
    if (sc_found)
    {
        printf("Found STRING key 'hello': %s\n", (char *)sc_found->value);
    }
    else
    {
        printf("STRING key 'hello' not found.\n");
    }

    // Delete a string key
    if (sc_delete(sc_table, &sc_search_key))
    {
        printf("Deleted STRING key 'hello'.\n");
    }
    else
    {
        printf("Failed to delete STRING key 'hello'.\n");
    }

    // Attempt to search for the deleted key
    sc_found = sc_search(sc_table, &sc_search_key);
    if (sc_found)
    {
        printf("Found STRING key 'hello' after deletion: %s\n", (char *)sc_found->value);
    }
    else
    {
        printf("STRING key 'hello' not found after deletion.\n");
    }

    /* ------------------------------ */
    /*        Open Addressing          */
    /* ------------------------------ */

    printf("\n=== Open Addressing Hash Table ===\n");

    // Create open addressing hash table with initial size 7
    OpenAddressingHashTable *oa_table = oa_create_table(7);

    // Insert integer key with linear probing
    OA_KeyValue oa_kv1 = oa_create_oa_pair(INT_KEY, 10, NULL, "Ten");
    oa_insert(oa_table, oa_kv1, linear_probe_func);

    // Insert string key with linear probing
    OA_KeyValue oa_kv2 = oa_create_oa_pair(STRING_KEY, 0, "key1", "value1");
    oa_insert(oa_table, oa_kv2, linear_probe_func);

    // Insert another integer key with linear probing
    OA_KeyValue oa_kv3 = oa_create_oa_pair(INT_KEY, 21, NULL, "Twenty-One");
    oa_insert(oa_table, oa_kv3, linear_probe_func);

    // Search for integer key
    OA_KeyValue oa_search_key;
    oa_search_key.type = INT_KEY;
    oa_search_key.key.int_key = 10;
    OA_KeyValue *oa_found = oa_search(oa_table, oa_search_key, linear_probe_func);
    if (oa_found)
    {
        printf("Found INT key 10: %s\n", (char *)oa_found->value);
    }
    else
    {
        printf("INT key 10 not found.\n");
    }

    // Search for string key
    oa_search_key.type = STRING_KEY;
    oa_search_key.key.str_key = "key1";
    oa_found = oa_search(oa_table, oa_search_key, linear_probe_func);
    if (oa_found)
    {
        printf("Found STRING key 'key1': %s\n", (char *)oa_found->value);
    }
    else
    {
        printf("STRING key 'key1' not found.\n");
    }

    // Delete a string key
    if (oa_delete(oa_table, oa_search_key, linear_probe_func))
    {
        printf("Deleted STRING key 'key1'.\n");
    }
    else
    {
        printf("Failed to delete STRING key 'key1'.\n");
    }

    // Attempt to search for the deleted key
    oa_found = oa_search(oa_table, oa_search_key, linear_probe_func);
    if (oa_found)
    {
        printf("Found STRING key 'key1' after deletion: %s\n", (char *)oa_found->value);
    }
    else
    {
        printf("STRING key 'key1' not found after deletion.\n");
    }

    // Insert using Quadratic Probing
    printf("\n--- Inserting with Quadratic Probing ---\n");
    OA_KeyValue oa_kv4 = oa_create_oa_pair(STRING_KEY, 0, "banana", "fruit");
    oa_insert(oa_table, oa_kv4, quadratic_probe_func);

    // Insert using Double Hashing
    printf("\n--- Inserting with Double Hashing ---\n");
    OA_KeyValue oa_kv5 = oa_create_oa_pair(INT_KEY, 17, NULL, "Seventeen");
    oa_insert(oa_table, oa_kv5, double_probe_func);

    // Search for new keys
    OA_KeyValue oa_search_key2;
    oa_search_key2.type = STRING_KEY;
    oa_search_key2.key.str_key = "banana";
    oa_found = oa_search(oa_table, oa_search_key2, quadratic_probe_func);
    if (oa_found)
    {
        printf("Found STRING key 'banana': %s\n", (char *)oa_found->value);
    }
    else
    {
        printf("STRING key 'banana' not found.\n");
    }

    oa_search_key2.type = INT_KEY;
    oa_search_key2.key.int_key = 17;
    oa_found = oa_search(oa_table, oa_search_key2, double_probe_func);
    if (oa_found)
    {
        printf("Found INT key 17: %s\n", (char *)oa_found->value);
    }
    else
    {
        printf("INT key 17 not found.\n");
    }

    // Delete an integer key
    if (oa_delete(oa_table, oa_kv5, double_probe_func))
    {
        printf("Deleted INT key 17.\n");
    }
    else
    {
        printf("Failed to delete INT key 17.\n");
    }

    // Attempt to search for the deleted integer key
    oa_found = oa_search(oa_table, oa_kv5, double_probe_func);
    if (oa_found)
    {
        printf("Found INT key 17 after deletion: %s\n", (char *)oa_found->value);
    }
    else
    {
        printf("INT key 17 not found after deletion.\n");
    }

    /* ------------------------------ */
    /*          Cleanup Code           */
    /* ------------------------------ */

    // Cleanup Separate Chaining Hash Table
    for (unsigned int i = 0; i < sc_table->size; i++)
    {
        SC_KeyValue *current = sc_table->buckets[i];
        while (current)
        {
            SC_KeyValue *next = current->next;
            if (current->type == STRING_KEY)
            {
                free(current->key.str_key);
            }
            // If value is dynamically allocated, free it here
            // free(current->value);
            free(current);
            current = next;
        }
    }
    free(sc_table->buckets);
    free(sc_table);

    // Cleanup Open Addressing Hash Table
    for (unsigned int i = 0; i < oa_table->size; i++)
    {
        if (oa_table->entries[i].status == OCCUPIED)
        {
            if (oa_table->entries[i].type == STRING_KEY)
            {
                free(oa_table->entries[i].key.str_key);
            }
            // If value is dynamically allocated, free it here
            // free(oa_table->entries[i].value);
        }
    }
    free(oa_table->entries);
    free(oa_table);

    return 0;
}
