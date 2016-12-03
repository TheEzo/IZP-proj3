/**
 * Kostra programu pro 3. projekt IZP 2015/16
 *
 * Jednoducha shlukova analyza
 * Complete linkage
 * http://is.muni.cz/th/172767/fi_b/5739129/web/web/clsrov.html
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX
#include <string.h>

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);

    // TODO

    c->obj = (struct obj_t*) malloc(sizeof(struct obj_t) * cap);
    if(c->obj == NULL){
        printf("Chybicka\n");
        return;
    }
    c->size = 0;
    c->capacity = cap;

    return;
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
    // TODO
    /*for(int i = 0; i < c->capacity; i++)
        free(&c->obj[i]);*/
    free(c->obj);
    c->size = 0;
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
    // TODO
    while(c->size >= c->capacity)
        resize_cluster(c, c->capacity + CLUSTER_CHUNK);
    c->obj[c->size].id = obj.id;
    c->obj[c->size].x = obj.x;
    c->obj[c->size].y = obj.y;
    c->size += 1;
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);

    // TODO
    for(int i = 0; i < c2->size; i++)
        append_cluster(c1, c2->obj[i]);
    sort_cluster(c1);
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);

    // TODO

    narr -= 1;


    //clear_cluster(&carr[idx]);
    //free(&carr[idx].obj);
    //for(int i = 0; i < carr[idx].capacity; i++)
        //free(&carr[idx].obj[i]);
    carr[idx].size = 0;
    for(int i = 0; i < carr[narr].size; i++) {
        append_cluster(&carr[idx], carr[narr].obj[i]);
    }

    //clear_cluster(&carr[narr]);


    /*
    for(int i = idx; i < narr; i++){
        clear_cluster(&carr[i]);
        for(int j = 0; j <  carr[i + 1].size; j++)
            append_cluster(&carr[i], carr[i+1].obj[j]);
    }
    clear_cluster(&carr[narr]);
    return narr;
*/
    return narr;
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);

    // TODO


    float x = o1->x - o2->x;
    float y = o1->y - o2->y;
    float distance = sqrtf(x*x + y*y);
    if(distance < 0)
        distance *= -1;
    return distance;
}

/*
 Pocita vzdalenost dvou shluku.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    // TODO


    float distance = 0;
    float lowest_d = 0;
    for(int i = 0; i < c1->size; i++)
        for(int j = 0; j < c2->size; j++){
            distance = obj_distance(&c1->obj[i], &c2->obj[j]);
            if(distance < 0)
                distance *= -1;
            if(distance > lowest_d)
                lowest_d = distance;
        }
    return lowest_d;
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);

    // TODO


    float distance = 0;
    float lowest_d = INT_MAX;
    for(int i = 0; i < narr; i++)
        for(int j = i+1; j < narr; j++){
            distance = cluster_distance(&carr[i], &carr[j]);
            if(distance < lowest_d){
                lowest_d = distance;
                *c1 = i;
                *c2 = j;
            }
        }

}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = a;
    const struct obj_t *o2 = b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr)
{
    assert(arr != NULL);

    // TODO
    FILE *soubor;
    soubor = fopen(filename, "r");
    if(soubor == NULL){
        fprintf(stderr, "Soubor %s se nepodarilo otevrit.\n", filename);
        return 0;
    }
    char *pch;
    char radek[100];
    int cap = 0;
    char *null_ptr;

    fgets(radek, 100, soubor);

    pch = strtok (radek,"capacity=\n");
    while (pch != NULL){
        cap = strtol(pch, &null_ptr, 10);
        pch = strtok (NULL, "=\n");
    }
    *arr = (struct cluster_t*) malloc(sizeof(struct cluster_t) * cap);
    int i = 0;
    while(fgets(radek, 100, soubor) != NULL || i < cap){
        init_cluster(&(*arr)[i], 1);
        int j = 0;
        pch = strtok (radek," ");
        (*arr)[i].size += 1;
        while (pch != NULL){
            switch (j){
                case 0:
                    (*arr)[i].obj->id = atoi(pch);
                    break;
                case 1:
                    (*arr)[i].obj->x = atof(pch);
                    break;
                case 2:
                    (*arr)[i].obj->y = atof(pch);

            }
            j++;
            pch = strtok (NULL, " ");
        }
        i++;
    }



    if(fclose(soubor) == EOF){
        fprintf(stderr, "Soubor %s se nepodarilo zavrit.\n", filename);
        return 0;
    }
/*
    struct obj_t append;
    append.id = 10;
    append.x = 5;
    append.y = 5;
    append_cluster(*arr, append);
*/

    return cap;
}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

int main(int argc, char *argv[])
{
    struct cluster_t *clusters;

    // TODO
    if(argc != 3){
        fprintf(stderr, "Pocet argumentu neni spravny.\n");
        return 0;
    }

    char *end_ptr = NULL;
    int N = strtol(argv[2], &end_ptr, 10);
    int puvodni_velikost;
    if(*end_ptr == 0) {
        int c1;
        int c2;
        int cluster_count = load_clusters(argv[1], &clusters);
        puvodni_velikost = cluster_count;
        while(N < cluster_count){
            find_neighbours(clusters, cluster_count, &c1, &c2);
            //printf("Budou slouceny clustery %d a %d.\n", c1, c2);
            merge_clusters(&clusters[c1], &clusters[c2]);
            cluster_count = remove_cluster(clusters, cluster_count, c2);
            //print_cluster(&clusters[cluster_count]);
        }
        print_clusters(clusters, N);
        for(int i = 0; i < puvodni_velikost; i++)
            //for(int j = 0; j < clusters[i].capacity; j++)
                //free(&clusters[i].obj[j]);
            clear_cluster(&clusters[i]);
        free(clusters);
        clusters = NULL;
    }
    return 0;
}