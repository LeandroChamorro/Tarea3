#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "Map.h"
#include "Stack.h"
#include "heap.h"
#include <ctype.h>

//ESTRUCTURAS
typedef struct{
  char nombre[30]; //Nombre de la tarea
  int prioridad; //Prioridad de la tarea
  int contPre; //Contador de tareas precedentes a esta tarea
  bool completada; //Booleano que sirve para la tercera función
  List *precedentes; // Lista de tareas precedentes a esta tarea
}tipoTarea;

typedef struct{
  int accion; //si agregó, 1, si añadió precedencia, 2 y si completó la tarea, 3.  
  tipoTarea *tareaCom; //Se guarda la tarea con la que hay que hacer la acción
  List *precedentes;
}tipoAccion;


//FUNCIONES SECUNDARIAS
int is_equal_string(void * key1, void * key2) {
    if(strcmp((char*)key1, (char*)key2)==0) return 1;
    return 0;
}

//Función para solicitar una cadena de caracteres.
void solicitarString(char *cadena, const char *mensaje){
  printf("%s\n", mensaje);
  fflush(stdin);
  scanf("%[^\n]s", cadena);
  getchar();
}

//Función para buscar una tarea en una lista
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

//OPCION 1
void agregarTarea(Map *mapaTarea, Stack * acciones){
  tipoTarea *tarea; //Se crea la tarea.
  tarea = malloc(sizeof(tipoTarea));
  
  solicitarString(tarea->nombre,"Ingrese nombre de la tarea: ");
  printf("Ingrese prioridad de la tarea: \n");
  fflush(stdin);
  scanf("%d", &tarea->prioridad);
  getchar();

  //Se completan todos los parámetros de la variable tipoTarea
  tarea->precedentes = createList();
  tarea->contPre = 0;
  tarea->completada = false;

  //Se crea la variable acción y se le asigna 1, correspondiente a que se agregó a una tarea
  //Luego se va hacia la pila de acciones
  tipoAccion *accion = malloc(sizeof(tipoAccion));
  accion->accion = 1;
  accion->tareaCom=tarea;
  stack_push(acciones, accion);

  //La tarea se inserta en el mapa de tareas
  char *nombreTarea=malloc(100*sizeof(char));
  strcpy(nombreTarea, tarea->nombre);
  insertMap(mapaTarea, nombreTarea, tarea);
  printf("\nLa tarea fué añadida de manera exitosa\n\n");
}

//OPCION 2
void establecerPrecedencia(Map *mapaTarea, Stack *acciones){
  char tarea1[30],tarea2[30];
  
  solicitarString(tarea1,"Ingrese nombre de la que va a ser precedente");
  solicitarString(tarea2, "Ingrese nombre de la tarea a la que se le va a asignar el presedente");

  //Se buscan ambas tareas en el mapa
  tipoTarea *tareaPre1 = searchMap(mapaTarea, tarea1);
  tipoTarea *tareaPre2 = searchMap(mapaTarea, tarea2);
  
  if(tareaPre1==NULL){
    printf("\nLa tarea 1 no existe\n\n");

    return;
  }
  if(tareaPre2==NULL){
    printf("\nLa tarea 2 no existe\n\n");

    return;
  }
  
  //La tarea 1 se agrega a la lista de precedentes de la tarea 2
  pushBack(tareaPre2->precedentes,tareaPre1);
  tareaPre2->contPre++;
  
  //Se crea la variable acción y se le asigna 2, correspondiente a que 
  //se establece precedencia entre dos tareas.
  //Luego se va hacia la pila de acciones
  tipoAccion *accion=malloc(sizeof(tipoAccion));
  accion->accion = 2;
  accion->tareaCom=tareaPre2;
  //strcpy(accion->presedencia, tarea1);
  stack_push(acciones, accion);

  printf("\nLa tarea %s fué añadida como precedente de %s de manera exitosa\n\n",tareaPre1->nombre,tareaPre2->nombre);
}

//OPCION 3
void mostrarTareas(Map* mapaTarea, Heap* montarea){ 
  if((tipoTarea*)firstMap(mapaTarea) == NULL){
    printf("\nNo se han ingresado tareas\n\n");
    return;
  }
  
  List* listaMostrar = createList();//Lista de tareas que se mostrarán
  List* precedentesGeneral = createList();//Lista de tareas con precedentes


  //Obtenemos las tareas sin precedentes y se mandan al montículo
  //Las tareas con precedentes se van a la lista de las tareas con precedentes
  for(tipoTarea *tareaAct = (tipoTarea*)firstMap(mapaTarea) ; tareaAct != NULL ; tareaAct = nextMap(mapaTarea)){
    if(tareaAct->contPre == 0){
      heap_push(montarea, tareaAct , tareaAct->prioridad);
    }
    else{
      pushBack(precedentesGeneral, tareaAct);
    }
  }

  //Agregamos las demas tareas que tienen precedentes al montículo,
  //revisando tarea por tarea si la raiz del montículo es
  //precedente de alguna de las tareas con precedentes, si es asi se 
  //marca y se revisa si la tarea con precedentes ya no tenga precedentes,
  //en ese caso se agregará al montículo.
  //Además, cuando se saca la raiz del montículo, se inserta en una lista,
  //de esta forma quedando ordenada de la manera que se requiere.
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

  int numact = 1;
  printf("\nTareas por hacer: \n");
  for(tipoTarea *tareaAct = (tipoTarea*)firstList(listaMostrar) ; tareaAct != NULL ; tareaAct = nextList(listaMostrar)){
    tareaAct->completada = false;
    printf("%i.- Tarea: %s , Prioridad: %i ",numact,tareaAct->nombre, tareaAct->prioridad);

    if(firstList(tareaAct->precedentes) != NULL)printf(", Tareas precedentes: ");
    for(tipoTarea* recList = firstList(tareaAct->precedentes) ; recList != NULL ; recList = nextList(tareaAct->precedentes) ){
      printf(" %s",recList->nombre);
    }
    printf("\n");
    numact++;
  }
  printf("\n");
}

//OPCION 4
void marcarCompletada(Map *mapaTarea, Stack* acciones){
  char tarea[30];
  solicitarString(tarea,"Ingrese nombre de la tarea a completar");

  tipoTarea *tareaElim = searchMap(mapaTarea, tarea);
  //Se busca la tarea y se verifica que exista
  if(tareaElim==NULL){
    printf("\nLa tarea que quiere marcar como completada no existe o ya fue completada\n\n");
    return;
  }
  bool encontrada=false;
  List* precedente = createList();
  //Se revisa si la tarea tiene precedentes, si tiene se advierte, si se quiere
  //seguir con la completación, se elimina de la lista de precedentes de esa tarea,
  //y esa misma tarea se agrega en la lista de precedentes de la variable acción,
  //para que luego al deshacer acción vuelva esta tarea a todas las que la 
  //tenían como precedente.
  for(tipoTarea *tareaAct = (tipoTarea*)firstMap(mapaTarea) ; tareaAct != NULL ; tareaAct = nextMap(mapaTarea)){      
    for(tipoTarea * preActual = firstList(tareaAct->precedentes) ; preActual != NULL ; preActual = nextList(tareaAct->precedentes)){
      if(strcmp(preActual->nombre,tareaElim->nombre)==0){
        int numero;
        
        if(encontrada==false){
          printf("Esta tarea tiene precedentes, ¿Está seguro que quiere marcarla como completada? Presione 1 para completar, 2 para no completar\n");
          encontrada=true;
          fflush(stdin);
          scanf("%d", &numero);
          getchar();
          if(numero==2) return;
        }
        
        pushBack(precedente, tareaAct);
        popCurrent(tareaAct->precedentes);
        tareaAct->contPre--;
      }
    }
  }
    
  //Se crea la variable acción y se le asigna 3, correspondiente a que 
  //se completó una tarea.
  //Luego se va hacia la pila de acciones
  printf("\nLa tarea se marcó como completada con éxito\n\n");
  tipoAccion *accion=malloc(sizeof(tipoAccion));
  accion->precedentes=precedente;
  accion->accion=3;
  accion->tareaCom = tareaElim;
  stack_push(acciones, accion);  
  eraseMap(mapaTarea, tarea);
}

//OPCION 5
void deshacerAccion(Map *mapaTarea, Stack* acciones){
    
  tipoAccion *accionn=malloc(sizeof(tipoAccion));
  //Se busca la última accion.
  accionn=stack_top(acciones);

  //Si la acción es nula significa que no quedan acciones que deshacer
  if(accionn==NULL){
    printf("\nNo hay acciones que deshacer\n\n");
    return;
  }

  //Si la última acción fue agregar una tarea, 
  //entonces se borra del mapa de tareas
  if(accionn->accion == 1){
    tipoTarea *basura=malloc(sizeof(tipoTarea));
    basura=eraseMap(mapaTarea, accionn->tareaCom->nombre);
  }

  //Si la última acción fue agregar precedencia entre
  //2 tareas, entonces se elimina la tarea de la 
  //lista de precedentes de la tarea correspondiente
  if(accionn->accion==2){
    tipoTarea *tarea=searchMap(mapaTarea, accionn->tareaCom->nombre);
    popBack(tarea->precedentes);
    tarea->contPre--;
  }

  //Si la última acción fue completar una tarea,
  //entonces esta se vuelve a insertar en el mapa.
  //Además esta tarea vuelve a todo el resto de tareas que la tenían como precedente
  if(accionn->accion==3){
    char *aux=malloc(100*sizeof(char));
    strcpy(aux, accionn->tareaCom->nombre);
    insertMap(mapaTarea, aux, accionn->tareaCom);
    for(tipoTarea *aux=firstList(accionn->precedentes); aux!=NULL; aux=nextList(accionn->precedentes)){
      pushBack(aux->precedentes, accionn->tareaCom);
      aux->contPre++;
    }
  }

  //Se elimina la acción con la que se estaba trabajando
  popFront(acciones);
  printf("\nSe ha deshecho la última accion con éxito\n\n");
}

//Opción 6
void cargarDatos(Map *mapaTarea){
  char archivo[100];
  //Se le pide al usuario que ingrese el nombre del archivo de donde desea importar las tareas
  printf("Ingrese el nombre del archivo que quiere ingresar:\n");
  //fflush(stdin);
  scanf("%[^\n]s",archivo);
  getchar();
  
  //Se abre el archivo
  FILE *fp=fopen(archivo, "r");
  if(fp==NULL){
    printf("\n===============================================================\n");
    printf("                   Error al importar archivo...\n");
    printf("     Asegúrese de importar al programa con el mismo nombre\n");
    printf("===============================================================\n\n");
    return;
  }
  char linea[100];
  //Se obtiene la primera línea (Que no nos sirve porque son las descripciones de las columnas)
  fgets(linea,101,fp);

  
  //A partir de aqui las lineas son importante porque tienen la información que necesitamos
  while(fgets(linea,101,fp)!=NULL){
    //Es una tarea por linea, por lo que aquí se crea
    tipoTarea *tarea;
    tarea=malloc(sizeof(tipoTarea));
    int j=0;
    //Se empieza a obtener cada parámetro a través de strtok, asi guardandose en sus variables correspondientes
    linea[strlen(linea)-1] = 0;
    char *ch = strtok(linea,", ");
    strcpy(tarea->nombre,ch);        
    tarea->contPre = 0;
    tarea->completada = false;

    ch = strtok(NULL,", ");
    tarea->prioridad=atoi(ch);

    tarea->precedentes=createList();
    
    ch = strtok(NULL,", ");    
    while(ch != NULL){

      tipoTarea* tareaPre = searchMap(mapaTarea,ch);
      
      pushBack(tarea->precedentes, tareaPre);
      
      tarea->contPre++;
      ch = strtok(NULL," ");
    }
    
   
    char *aux2=malloc(100*sizeof(char));
    strcpy(aux2, tarea->nombre);
    insertMap(mapaTarea, aux2, tarea);
  }
  
  printf("\n===============================================================\n");
  printf("        La importación de tareas fue hecha con éxito\n");
  printf("===============================================================\n\n");
  fclose(fp);
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
    //fflush(stdin);
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
      
      case 5: deshacerAccion(mapaTarea, acciones);
      break;
        
      case 6: cargarDatos(mapaTarea);
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
        printf("⠀⠀⠀⠀⠀⠈⠙⠛⠛⠋⠀⠀⠀⠀  ⠀⠙⠶⣤⣀⣀⣤⡶⠟⠁\n\n");
        printf("Creado por: Alex Aravena y Leandro Chamorro\n");
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