#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "Map.h"
#include "Stack.h"

typedef struct{
  char nombre[30];
  int prioridad;
  int contPre;
  List *precedentes;
}tipoTarea;

typedef struct{
  int accion; //si agregó, 1, si completó la tarea, 2 y si añadió precedencia 3.  
  char presedencia[30];
  tipoTarea tareaCom;
}tipoAccion;

int is_equal_int(void * key1, void * key2) {
    if(*(int*)key1 == *(int*)key2) return 1;
    return 0;
}

void solicitarString(char *cadena, const char *mensaje){
  printf("%s\n", mensaje);
  fflush(stdin);
  scanf("%[^\n]s", cadena);
  getchar();
}

void agregarTarea(List* tareas,Map *mapaTarea, Stack * acciones){
  tipoTarea *tarea;
  tarea = malloc(sizeof(tipoTarea));
  
  solicitarString(tarea->nombre,"ingrese nombre\n");
  printf("Ingrese prioridad de la tarea: \n");
  fflush(stdin);
  scanf("%d", &tarea->prioridad);
  getchar();
  
  tarea->precedentes = createList();
  tarea->contPre = 0;

  tipoAccion *accion = malloc(sizeof(tipoAccion));
  accion->accion = 1;
  stack_push(acciones, accion);
  
  insertMap(mapaTarea , &tarea->prioridad , tarea);
  pushBack(tareas,tarea);
}


void menu(List *tareas, Map *mapaTarea, Stack *acciones){
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
    scanf("%d", &opcion);
    getchar();
    //Se utiliza un switch para acceder a las opciones de cada función
    switch(opcion){
      case 1: agregarTarea(tareas,mapaTarea, acciones);
      break; 
            
      //case 2: establecerPrecedencia(tareas,acciones);
      break;
          
      //case 3: mostrarTareas(tareas);
      break;

      //case 4: marcarCompletada(tareas);
      break;
      
      //case 5: deshacerAccion(tareas);
      break;
        
      //case 6: cargarDatos(tareas);
      break;
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
  List *tareas = createList();
  Map * mapaTarea = createMap(is_equal_int);
  menu(tareas, mapaTarea, acciones);
  
  return 0;
}