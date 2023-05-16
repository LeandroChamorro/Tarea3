#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "Map.h"
#include "Stack.h"
#include "heap.h"

//ESTRUCTURAS
typedef struct{
  char nombre[30];
  int prioridad;
  int contPre;
  bool completada;
  List *precedentes;
}tipoTarea;

typedef struct{
  int accion; //si agregó, 1, si añadiò precedencia, 2 y si completó la tarea 3.  
  char presedencia[30];
  tipoTarea *tareaCom;
}tipoAccion;


//FUNCIONES SECUNDARIAS
int is_equal_string(void * key1, void * key2) {
    if(strcmp((char*)key1, (char*)key2)==0) return 1;
    return 0;
}

void solicitarString(char *cadena, const char *mensaje){
  printf("%s\n", mensaje);
  fflush(stdin);
  scanf("%[^\n]s", cadena);
  getchar();
}

tipoTarea* buscarTarea(List *lista, char *nombre){
  for (tipoTarea* p = firstList(lista) ; p != NULL ; p = nextList(lista)){
      //Se busca si el nombre coincide
      if (strcmp(p->nombre, nombre) == 0){
        return p;
      }
    }
  return NULL;
}


//FUNCIONES PRIMARIAS

//OPCIÒN 1
void agregarTarea(Map *mapaTarea, Stack * acciones){
  tipoTarea *tarea;
  tarea = malloc(sizeof(tipoTarea));
  
  solicitarString(tarea->nombre,"ingrese nombre\n");
  printf("Ingrese prioridad de la tarea: \n");
  fflush(stdin);
  scanf("%d", &tarea->prioridad);
  getchar();
  
  tarea->precedentes = createList();
  tarea->contPre = 0;
  tarea->completada = false;
  
  tipoAccion *accion = malloc(sizeof(tipoAccion));
  accion->accion = 1;
  accion->tareaCom=tarea;
  stack_push(acciones, accion);
  
  char *nombreTarea=malloc(100*sizeof(char));
  strcpy(nombreTarea, tarea->nombre);
  insertMap(mapaTarea, nombreTarea, tarea);
}

//OPCION 2
void establecerPrecedencia(Map *mapaTarea, Stack *acciones){
  char tarea1[30],tarea2[30];
  
  solicitarString(tarea1,"Ingrese nombre de la tarea 1\n");
  solicitarString(tarea2, "Ingrese nombre de la tarea 2\n");
  
  tipoTarea *tareaPre1 = searchMap(mapaTarea, tarea1);
  tipoTarea *tareaPre2 = searchMap(mapaTarea, tarea2);
  
  if(tareaPre1==NULL){
    printf("La tarea 1 no existe\n");
    return;
  }
  if(tareaPre2==NULL){
    printf("La tarea 2 no existe\n");
    return;
  }

  tipoAccion *accion=malloc(sizeof(tipoAccion));
  accion->accion = 2;
  accion->tareaCom=tareaPre2;
  strcpy(accion->presedencia, tarea1);
  stack_push(acciones, accion);
  
  pushBack(tareaPre2->precedentes,tareaPre1);
  tareaPre2->contPre++;
}

//OPCION 3
void mostrarTareas(Map* mapaTarea, Heap* montarea){ 
  List* listaMostrar = createList();//Lista de tareas
  List* precedentesGeneral = createList();//Lista de tareas con precedentes

  //Obtenemos las tareas sin precedentes
  for(tipoTarea *tareaAct = (tipoTarea*)firstMap(mapaTarea) ; tareaAct != NULL ; tareaAct = nextMap(mapaTarea)){
    if(tareaAct->contPre == 0){
      heap_push(montarea, tareaAct , tareaAct->prioridad);
    }
    else{
      pushBack(precedentesGeneral, tareaAct);
    }
  }

  //Agregamos las demas tareas al monticulo
  while(firstList(precedentesGeneral)!=NULL){
    tipoTarea *root = heap_top(montarea);
    heap_pop(montarea);
    pushBack(listaMostrar,root);
    root->completada = true;
    
    for(tipoTarea *tareaAct = (tipoTarea*)firstList(precedentesGeneral) ; tareaAct != NULL ; tareaAct = nextList(precedentesGeneral)){
      int cont=0;
      
      for(tipoTarea * preActual = firstList(tareaAct->precedentes) ; preActual != NULL ; preActual = nextList(tareaAct->precedentes)){
        if(preActual->completada==true){
          cont++;
        }
      }
      
      if(cont==tareaAct->contPre){
        heap_push(montarea, tareaAct , tareaAct->prioridad);
        popCurrent(precedentesGeneral);
      }
    }
  }  
  
  while(heap_top(montarea) != NULL){
    tipoTarea *root = heap_top(montarea);
    heap_pop(montarea);
    pushBack(listaMostrar,root);
  }

  for(tipoTarea *tareaAct = (tipoTarea*)firstList(listaMostrar) ; tareaAct != NULL ; tareaAct = nextList(listaMostrar)){
    tareaAct->completada = false;
    printf("%s , %i \n",tareaAct->nombre, tareaAct->prioridad);
  }
}

//OPCION 4
void marcarCompletada(Map *mapaTarea, Stack* acciones){
  char tarea[30];
  solicitarString(tarea,"Ingrese nombre de la tarea a eliminar\n");

  tipoTarea *tareaElim = searchMap(mapaTarea, tarea);
  //Ya tengo la tarea
  
  //Reviso si tiene precedentes
  for(tipoTarea *tareaAct = (tipoTarea*)firstMap(mapaTarea) ; tareaAct != NULL ; tareaAct = nextMap(mapaTarea)){      
    for(tipoTarea * preActual = firstList(tareaAct->precedentes) ; preActual != NULL ; preActual = nextList(tareaAct->precedentes)){
      if(strcmp(preActual->nombre,tareaElim->nombre)){
        printf("Esta tarea tiene precedentes, ¿Está seguro que quiere marcarla como completada? Presione 1 para completar, 2 para no completar\n");
        int numero;
        fflush(stdin);
        scanf("%d", &numero);
        getchar();
        while(true){
          if(numero!=1 && numero!=2){
            printf("Ingrese una opción válida\n");
            fflush(stdin);
            scanf("%d", &numero);
            getchar();
          }else{
            if(numero==1){
              tipoAccion *accion=malloc(sizeof(tipoAccion));
              accion->accion=2;
              accion->tareaCom = tareaElim;
              stack_push(acciones, accion);  
              eraseMap(mapaTarea, tarea);
              return;
            }else{
              return;
            }
          }
        }
      }
    }
  }
  printf("La tarea se marcó como completada con éxito\n");
  tipoAccion *accion=malloc(sizeof(tipoAccion));
  accion->accion=3;
  accion->tareaCom = tareaElim;
  stack_push(acciones, accion);
    
  eraseMap(mapaTarea, tarea);
}





void menu(Map *mapaTarea, Stack *acciones, Heap *montarea){
  //Se crea una variable "opcion" la cual será una condicionante para el ciclo "while" base de nuestro programa
  int opcion = 1;
  while(opcion != 0){
    printf("╔════════════════════════════•°🜧°•════════════════════════════╗\n");
    printf("║ Si desea agregar una tarea presione 1                       ║\n");
    printf("║ Si desea establecer precedentes entre 2 tareas presione 2   ║\n");
    printf("║ Si desea mostrar las tareas por hacer presione 3            ║\n");
    printf("║ Si desea marcar una tarea como completada presione 4        ║\n");
    printf("║ Si desea deshacer la última acción presione 5               ║\n");
    printf("║ Si desea cargar datos de tareas desde un archivo de texto   ║\n");
    printf("║ en especifico presione 6                                    ║\n");
    printf("║ Si no desea hacer nada más presione 0                       ║\n");
    printf("╚════════════════════════════•°🜥°•════════════════════════════╝\n\n");
    //Se cambia el valor de la variable "opcion" con un valor que desee el usuario realizar
    fflush(stdin);
    scanf("%d", &opcion);
    getchar();
    //Se utiliza un switch para acceder a las opciones de cada función
    switch(opcion){
      case 1: agregarTarea(mapaTarea, acciones);
      break; 
            
      case 2: establecerPrecedencia(mapaTarea, acciones);
      break;
          
      case 3: mostrarTareas(mapaTarea, montarea);
      break;

      case 4: marcarCompletada(mapaTarea, acciones);
      break;
      
      //case 5: 
        //printf("holi");
        //deshacerAccion(mapaTarea, acciones);
      //break;
        
      //case 6: cargarDatos(tareas);
      //break;
      //en caso de ser cero se imprime lo sgte. Para finalizar el programa
      case 0:
        printf("⠀⠀⠀⠀⠀⠀⠀⢀⣤⠖⠛⠉⠉⠛⠶⣄⡤⠞⠛⠛⠙⠳⢤⡀\n");
        printf("⠀⠀⠀⠀⠀⠀⢠⠟⠁⠀⠀⠀⠀⠀⠀ ⠀⢰⡆⠀⠀⠐⡄⠻⡄\n");
        printf("⠀⠀⠀⠀⠀⠀⡾⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠛⠦⠤⣤⣇ ⢷\n");
        printf("⠀⠀⠀⠀⠀⠀⢳⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀   ⡼\n");
        printf("⠀⠀⠀⠀⠀⠀⠘⣆⢰⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⣼⠃\n");
        printf("⠀⠀⠀⠀⠀⠀⠀⠙⣎⢳⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡾⠃\n");
        printf("⠀⠀⠀⠀⠀⠀⠀⠀⠈⢳⣝⠳⣄⡀⠀⠀⠀⠀⠀⢀⡴⠟⠁\n");
        printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠳⢮⣉⣒⣖⣠⠴⠚⠉\n");
        printf("⠀⠀⠀⣀⣴⠶⠶⢦⣀⠀⠀⠀⠀⠀⠉⠁⠀⠀⠀⠀ ⣠⣤⣤⣀⠀\n");
        printf("⠀⢀⡾⠋⠀⠀⠀⠀⠉⠧⠶⠒⠛⠛⠛⠛⠓⠲⢤⣴⡟⠅⠀⠀⠈⠙⣦⠀\n");
        printf("⠀⣾⠁⠀⠀⠀⠀⠀⠀⠀⣠⡄⠀⠀⠀⣀⠀⠀⠀ ⠀⠀⠀⠀⠀⠀⠸⣇\n");
        printf("⠀⣿⡀⠀⠀⠀⠀⠀⢀⡟⢁⣿⠀⢠⠎⢙⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣽\n");
        printf("⠀⠈⢻⡇⠀⠀⠀⠀⣾⣧⣾⡃⠀⣾⣦⣾⠇⠀⠀⠀⠀⠀⠀⠀ ⠀⣼⠇\n");
        printf("⠀⢰⡟⠀⡤⠴⠦⣬⣿⣿⡏⠀⢰⣿⣿⡿⢀⡄⠤⣀⡀⠀⠀⠀⠰⢿⡁\n");
        printf("⠀⡞⠀⢸⣇⣄⣤⡏⠙⠛⢁⣴⡈⠻⠿⠃⢚⡀⠀⣨⣿⠀⠀⠀⠀⢸⡇\n");
        printf("⢰⡇⠀⠀⠈⠉⠁⠀⠀⠀⠀⠙⠁⠀⠀⠀⠈⠓⠲⠟⠋⠀⠀⠀⠀⢀⡇\n");
        printf("⠈⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠇\n");
        printf("⠀⢹⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⡄\n");
        printf("⠀⠀⠻⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣽⠋⣷\n");
        printf("⠀⠀⢰⣿⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⡾⠃⠀⣿⡇\n");
        printf("⠀⠀⢸⡯⠈⠛⢶⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣤⠾⠋ ⠀⠀⣿⡇\n");
        printf("⠀⠀⠈⣷    ⠉⠛⠶⢶⣶⠶⠶⢶⡶⠾⠛⠉⠀⠀⠀   ⣿⡇\n");
        printf("⠀⠀⠀⠈⠳⣤  ⠀⣀⡶⠟⠁⠀⠀⠘⢷⡄⠀     ⢀⣾⡿\n");
        printf("⠀⠀⠀⠀⠀⠈⠙⠛⠛⠋⠀⠀⠀⠀  ⠀⠙⠶⣤⣀⣀⣤⡶⠟⠁\n");
        break;
    }
  }
}

int main(void) {
  Stack *acciones = stack_create();
  Map * mapaTarea = createMap(is_equal_string);
  Heap *montarea = createHeap();
  menu(mapaTarea, acciones, montarea);
  
  return 0;
}

