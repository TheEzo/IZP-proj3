/**
 * Kostra programu pro 3. projekt IZP 2016/17
 *
 * xwilla00
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

#define VELIKOST_RADKU 1000

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
    c->size = 0;
    c->capacity = cap;
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
    // TODO
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
    if(c->size >= c->capacity)
        resize_cluster(c, c->capacity + CLUSTER_CHUNK);
    if(c == NULL)
        fprintf(stderr, "Chyba pri rozsirovani clusteru.\n");
    c->obj[c->size].id = obj.id;
    c->obj[c->size].x = obj.x;
    c->obj[c->size].y = obj.y;
    c->size++;
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
    // do clusteru [idx] se ulozi cluster [narr-1] (posledni cluster)
    narr -= 1;
    carr[idx].size = 0;
    for(int i = 0; i < carr[narr].size; i++) {
        append_cluster(&carr[idx], carr[narr].obj[i]);
    }
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
    // pythagorova veta - velikost vektoru
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
    // hleda se vzdalenost dvou nejvzdalenejsich objektu shluku
    float distance = 0;
    float biggest_d = 0;
    for(int i = 0; i < c1->size; i++)
        for(int j = 0; j < c2->size; j++){
            distance = obj_distance(&c1->obj[i], &c2->obj[j]);
            if(distance < 0)
                distance *= -1;
            if(distance > biggest_d)
                biggest_d = distance;
        }
    return biggest_d;
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

    if((soubor = fopen(filename, "r")) == NULL){
        fprintf(stderr, "Soubor %s se nepodarilo otevrit.\n", filename);
        return 0;
    }

    char *pch;
    char radek[VELIKOST_RADKU];
    int cap = 0;
    char *end_ptr = NULL;
    int id = 0;
    float souradnice = 0;
    int soupatko = 1;

    if(fgets(radek, VELIKOST_RADKU, soubor) == NULL){
        fprintf(stderr, "Soubor je prazdny, nebo ma spatny format.\n");
        return 0;
    }
    //nacteni poctu shluku - prvni radek souboru s danym formatem
    pch = strtok (radek,"count= \n");
    cap = strtol(pch, NULL, 10);
    if(cap <= 0){
        fprintf(stderr, "Pocet shluku je v souboru zapsany nespravne.\n(musi byt na prvnim radku ve formatu \"count=CISLO\")\n");
        return 0;
    }
    *arr = (struct cluster_t*) malloc(sizeof(struct cluster_t) * cap);
    if(*arr == NULL){
        fprintf(stderr, "Pole clusteru se nepodarilo alokovat.\n");
        return 0;
    }

    int c_free = 0;

    // nacitani dat ze souboru do clusteru;
    int i = 0;
    while(i < cap && soupatko){
        if(fgets(radek, VELIKOST_RADKU, soubor) == NULL){
            fprintf(stderr, "Pocet dat zapsanych v souboru %s neodpovida poctu zaznamenanem na prvnim radku.\n", filename);
            c_free = 1;
        }

        init_cluster(&(*arr)[i], 1);
        if(&(arr)[i] == NULL){
            fprintf(stderr, "%d. cluster se nepodarilo alokovat.\n", i+1);
            return 0;
        }
        int j = 0;
        //oddelovace CR a LF, z duvodu prenositelnosti kodu
        pch = strtok (radek," \x0a\x0d");
        while (pch != NULL && soupatko){
            end_ptr = NULL;
            switch (j){
                case 0:
                    id = strtol(pch, &end_ptr, 10);
                    if(*end_ptr == 0 && id >= 0)
                        (*arr)[i].obj->id = id;
                    else
                        soupatko = 0;
                        //(*arr)[i].obj = NULL;
                    break;
                case 1:
                    souradnice = strtof(pch, &end_ptr);
                    if(*end_ptr == 0 && souradnice <= 1000 && souradnice >= 0)
                        (*arr)[i].obj->x = souradnice;
                    else
                        soupatko = 0;
                    //(*arr)[i].obj = NULL;
                    break;
                case 2:
                    souradnice = strtof(pch, &end_ptr);
                    if(*end_ptr == 0 && souradnice <= 1000 && souradnice >= 0)
                        (*arr)[i].obj->y = souradnice;
                    else
                        soupatko = 0;
                        //(*arr)[i].obj = NULL;
                    break;
            }
            j++;
            pch = strtok (NULL, " \x0a\x0d");
        }
        // kontrola nekompletniho radku v souboru
        if(j != 3)
            soupatko = 0;
        (*arr)[i].size++;
        i++;
    }

    if(fclose(soubor) == EOF){
        fprintf(stderr, "Soubor %s se nepodarilo zavrit.\n", filename);
        return 0;
    }

    if(!soupatko || c_free){
        if(!c_free)
            fprintf(stderr, "V souboru %s jsou neplatna data.\n", filename);
        for(int j = 0; j < i; j++)
            clear_cluster(&(*arr)[j]);
        free(*arr);
        *arr = NULL;
        return 0;
    }


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
    int N;
    char *end_ptr = NULL;  //slouzi pro kontrolu N ze vstupu
    int puvodni_velikost;
    int soupatko = 0;

    if(!(argc == 2 || argc == 3)){
        fprintf(stderr, "Zadany pocet parametru neni spravny.\n./proj3 NAZEV_SOUBORU [N]\n");
        return 0;
    }
    if(argc == 2){
        N = 1;
        soupatko = 1;
    }
    if(argc == 3) {
        N = strtol(argv[2], &end_ptr, 10);
        soupatko = 1;
    }

    if(N <= 0){
        fprintf(stderr, "./proj3 NAZEV_SOUBORU [N]\nParametr N musi byt cislo vetsi nez 0.\n");
        return 0;
    }

    if(soupatko) {
        //c1 a c2 - uchovavaji indexy clusteru pro slouceni
        int c1;
        int c2;
        int cluster_count = load_clusters(argv[1], &clusters);

        if(cluster_count == 0)
            soupatko = 0;

        if(N > cluster_count && soupatko){
            fprintf(stderr, "V souboru je prilis malo shluku.\n");
            soupatko = 0;
        }
        puvodni_velikost = cluster_count; //mnozstvi alokovanych clusteru po provedeni load_clusters()
        // cilovy pocet clusteru
        while(N < cluster_count && soupatko){
            find_neighbours(clusters, cluster_count, &c1, &c2);
            merge_clusters(&clusters[c1], &clusters[c2]);
            if(&clusters[c1] == NULL){
                fprintf(stderr, "Cluster %d se pri shlukovani nepodarilo rozsirit.\n", c1 + 1);
                soupatko = 0;
            }
            cluster_count = remove_cluster(clusters, cluster_count, c2);
            if(&clusters[c2] == NULL){
                fprintf(stderr, "Chyba pri rozsirovani clusteru.\n");
                soupatko = 0;
            }
        }
        if(soupatko)
            print_clusters(clusters, N);

        // uvolneni pameti pro jednotlivé clustery
        for(int i = 0; i < puvodni_velikost; i++)
            clear_cluster(&clusters[i]);
        // uvolneni pameti ukazatele na pole clusteru
        free(clusters);
        clusters = NULL;
    }
    return 0;
}