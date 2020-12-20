#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX 10000
#define Infinite 999999999

  typedef struct node {                                                     //Lista doblemente enlazada,en la cual guardaremos el nombre de cada lugar y cierta informacion de los grafos 
 
    char  lugar [30];
    
    int visitado;
    float distancia;
    
	struct node *siguiente;
	struct node *anterior;
	
	struct connect *conexiones;
	
    }Lugares;
    
  typedef struct connect {                                                //Lista doblemente enlazada la cual contiene las caracteristicas de las conexiones entre un sitio y otro
  	
  	Lugares *Conecta;
  	
  	char NombreRuta[30];
  	
  	float P;
  	float B;
  	float C;
  	
  	struct connect *siguiente;
  	struct connect *anterior;
  	
  	
  }Conexion;  
  
  typedef struct walk {                                                  //Lista doblemente enlazada,encargada de guardar las rutas recorridas (los caminos)
  	
  	struct connect *conexion;
  	  	
  	struct walk *siguiente;
  	struct walk *anterior;
  	  	
  }Camino;
  


 /* Variables globales */
 
 FILE *archivoLugaresRutas=NULL;
 FILE *archivoClima=NULL;
 
 
 /*Declarando todas las funciones utilizadas en el programa*/
 
 int CompareStrings(char nombre1[30],char nombre2[30]);                                                          // Compara dos cadenas de caracteres para verificar si son exactamente iguales
 
 Lugares *crearCasilla (Lugares *nodo,char nombre[30]);                                                        // Crea una lista doblemente enlazada en la cual se guardaran los nombres de los lugares y otros datos para el grafo en si  
 
 Conexion *crearConexion (Conexion *p,Lugares *Sitio, float P, float B, float C,char nombre[30]);            // Crea un nodo con un puntero que guarda la direccion de la conexion a la que puede apuntar un sitio y guarda sus respectivos valores P B C
 
 Camino *crearCamino (Conexion *p ,Camino *c,Camino *ant);                                                 // Crea una lista la cual esta encargada de realizar un recorrido desde un inicio en un sitio hasta un final en otro
 
 Camino *anadirSig (Camino *c );                                                                         // le añade al nodo apuntador "siguiente" de la lista de tipo Camino el correspondiente,cabe destacar que la funcion crear camino solo añade direcciones al puntero "anterior"
 
 Lugares *Busqueda (Lugares *Sitios,char nombre[30]);                                                  // Busca dentro de la lista donde se guardan los lugares el nodo de un sitio especifico y devuelve un puntero a dicha ubicacion
 
 Conexion *BusquedaRuta (Lugares *Sitios,char nombre[30]);                                           // Busca dentro de la lista donde se guardan los lugares,en caso de existir,el nombre de las rutas q conecta dicho lugar
  
 void salirPrograma ();                                                                            // Funcion la cual cierra el programa en caso de cualquier limitacion,ya sea falta de recursos del sistema etc..
 
 void TeclaContinuar ();                                                                         // Pide al usuario presionar una tecla para continuar con el programa y limpia la pantalla
 
 int valorcorrecto (int modalidad, int maximo, int minimo);                                    // Verifica si un valor esta dentro de un limite establecido
 
 FILE *validaArchivo (char nombreArchivo[20]);                                               // Se encarga de verificar la existencia de el archivo que el usuario esta ingresando        
 
 char limpiarCad (char nombre[70]);                                                        // Limpia una cadena de caracteres  
 
 void bienvenida ();                                                                     // Pantalla principal del programa
 
 void menu ();                                                                         // Primera faceta del programa,se encarga de pedir todos los datos necesarios para iniciar con la funcion del programa
 
 Lugares *crearGrafo (Lugares *grafo, FILE *archivo, int opcionClimaRuta );          // Se encarga de unir todos los lugares existentes en base a las rutas dadas 
 
 Lugares *inicializa( Lugares *p ,int opcion);                                     // Resetea el valor de las distancias de cada sitio en Infinite,con el fin de que se pueda volver a evaluar una ruta
 
 float suma(Camino *p, int opcion);                                              // Suma todos los valores de un recorrido,en base a la opcion solicitada
 
 void rutaView (Lugares *final,Camino *r, int op);                             // Muestra todo un recorrido realizado
 
 Camino *calculate (Lugares *sitio,Lugares *inicio,Lugares *final,Camino *mostrar,Camino *ant,int opcion,Lugares *grafo);  // Funcion encargada de buscar un mejor recorrido en todo el mapa,cada vez que halla encontrado un recorrido este se ira guardando en una auxiliar y se ira sobreescribiendo en el caso de encontrar un recorrido mejor
 
 Camino *calculate2do (Camino *mejorRuta,Camino *recorrido,Camino *mejorRuta2,Lugares *inicio,Lugares *final,Lugares *grafo, int opcion );  // Funcion encargada de buscar la segunda mejor ruta,basado en el algortimo de YEN
 
 void mejorCamino (Lugares *inicio,Lugares *final,Lugares *grafo);    // Funcion encargada de encontrar la mejor ruta desde un punto de inicio hasta un punto final


 /*Programa Principal (inicio)*/
 
 int main () {
 	
 	int finProgram=1;
 	
 
 	bienvenida();
 	
 	while(finProgram!=2) {
 	
	menu();
 	
	printf("Desea Volver a ejecutar el programa?\n1.Si  2.No \n");
	
	scanf("%i",&finProgram);
	
 	finProgram=valorcorrecto(finProgram,2,1);
 	
 	TeclaContinuar();
 		
 		
	 }
		
	  
 }

 /*Programa Principal (fin)*/



 /*Funciones auxiliares(inicio)*/
 
 int CompareStrings(char nombre1[30],char nombre2[30]) {
 	
 	int o=0;
 	int k=0;
 	
 	
 	
 	
 	while(nombre1[o]!='\0' && nombre2[k]!='\0') {
 		
 		if( nombre1[o]!=nombre2[k] )
		 return 1; 
 		
 		o++;
 		k++;
 		
	 }
	
	if(nombre1[o]=='\0' && nombre2[k]=='\0' )
	return 0;
	
	else 
	return 1;  
 	
 	
 	
 	
 	
 }

 Lugares *crearCasilla (Lugares *nodo,char nombre[30]) {  
 
	Lugares *casilla;
	Lugares *aux;
	
	
	casilla=(Lugares*)malloc(sizeof(Lugares));               
	
	if(!casilla) salirPrograma;                                     
	
	strcpy(casilla->lugar,nombre);
	
	casilla->visitado=0;
	casilla->distancia=Infinite;
	
	casilla->conexiones=NULL;
	
	casilla->siguiente=NULL;
	
	if(nodo==NULL){
		casilla->anterior=NULL;                              
		nodo=casilla;
		return nodo;
	}
	
	else
	
	aux=nodo;                
	
	while(aux->siguiente!=NULL)
		aux=aux->siguiente;
		
	casilla->anterior=aux;   
	aux->siguiente=casilla;

	return nodo;

 }
 
 Conexion *crearConexion (Conexion *p,Lugares *Sitio, float P, float B, float C,char nombre[30]) {
 	
 	Conexion *casilla;
 	Conexion *aux;
 	
 	casilla=(Conexion*)malloc(sizeof(Conexion)); 
 	
 	if(!casilla) salirPrograma;  
 	
 	strcpy(casilla->NombreRuta,nombre);
 	
 	casilla->Conecta=Sitio;
 	
 	casilla->P=P;
 	casilla->B=B;
 	casilla->C=C;
 	
 	casilla->siguiente=NULL;
 	
 	if(p==NULL){
 		
		casilla->anterior=NULL;                              
		p=casilla;
		
		return p;
	}
    
	aux=p;
	
	while(aux->siguiente!=NULL)
	aux=aux->siguiente;
		
	casilla->anterior=aux;   
	aux->siguiente=casilla;

	return p;	
 	
 	
 }
 
 
 Camino *crearCamino (Conexion *p,Camino *c,Camino *ant) {
 	
 	
 	
 	c=(Camino*)malloc(sizeof(Camino));

 	c->conexion=p;
 			
 	
 			
 	c->anterior=ant;
 			
 	return c;		
 		
 }
 
 
 Camino *anadirSig (Camino *c) {
 	
 	
 Camino *s=c;
 
 Camino *ant=NULL;
	  	
	  	
	  	
	  	while(s->anterior!=NULL) {
	  		ant=s;
	  		
	  		s=s->anterior;
	  		
	  		s->siguiente=ant;
	  		
	  		
	  		
	  		
	  		
		  }	
 	
 	
 	return s;
 	
 	
 }       
 
 
 
 
 Lugares *Busqueda (Lugares *Sitios,char nombre[30]) {
 	
 	Lugares *aux=Sitios;
 	
 	while (Sitios!=NULL){
 		
 		
 		if(CompareStrings(nombre,Sitios->lugar)==0){
 			aux=Sitios;
 			return aux;
		 }
 		Sitios=Sitios->siguiente;
 		
	 }
 	

 	
 }
 
 
  Conexion *BusquedaRuta (Lugares *Sitios,char nombre[30]) {
  	
  	Conexion *recorridoAux=NULL;
  	Conexion *aux=NULL;
  	char nombreaux[30];
  	
  	
  	while (Sitios!=NULL) {
  	 	
  	 	if (Sitios->conexiones!=NULL) {
  	 		
  	 		
  	 		recorridoAux=Sitios->conexiones;
  	 		
  	 		while (recorridoAux!=NULL){
  	 			
  	 			
  	 			
  	 			
  	 			if (CompareStrings(nombre,recorridoAux->NombreRuta)==0){
  	 				
  	 				aux=recorridoAux;
  	 			    return aux;
  	 				
				}
				   
  	 			recorridoAux=recorridoAux->siguiente;
  	 			
			}  
					
		}
  	 	
  	 	Sitios=Sitios->siguiente;
	}
  	
  	
  }
 
 

 void salirPrograma () { 
                                                 
	printf("Ocurrio un problema,se procedera a salir del programa\n");
	
	TeclaContinuar();
	exit(1);                                                                    
 }


 void TeclaContinuar () {         
                                        
	printf("||||||||||||||||||||||||||||||||||||||||||||\n");
	printf("|||| Para continuar presiona una tecla |||||\n");
	printf("||||||||||||||||||||||||||||||||||||||||||||\n");
	
	fflush(stdin);  
	
	while ( getchar() != '\n');  
	system("clear");                                                   
 }
 
 
 int valorcorrecto (int modalidad, int maximo, int minimo) {  
           
	if(modalidad>maximo || modalidad<minimo){
		
		while(modalidad>maximo || modalidad<minimo){                       
		
			printf("valor invalido por favor intente nuevamente\n");
			scanf("%i",&modalidad);
		}
		
		return(modalidad);                                          
		
	}else return(modalidad);                                       
 }
 
 
 FILE *validaArchivo (char nombreArchivo[20]) {
	
	FILE *archivo=NULL;
	
	char aux[20];
	
	archivo= fopen(nombreArchivo,"r");	
	
	while (archivo==NULL){
		
    	printf("El archivo que ingreso no existe por favor vuelva a ingresar el nombre de su archivo \n");
    	
		scanf("%s",&aux);
		
		archivo= fopen(aux,"r");	
		
		nombreArchivo=aux;
		
		}
		
    printf("Su archivo %s existe \n",nombreArchivo);
		
	TeclaContinuar();
		
	return archivo;
	
 }
 
 
  char limpiarCad (char nombre[70]) {
 	
    int  i=0,n=0;
 	
 	n=strlen(nombre);
 	
 	for(i=0;i<n;i++){
 		nombre[i]='\0';
	 }
	 
	 return nombre[70];
 	
 }
 
 

  /*Funciones auxiliares(fin)*/
  
  
  
 /*Funciones Base del programa(inicio)*/
 
 
 
 void ingresaTexto (char valor[23]) {
 	
 	printf("||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
	printf("||Por favor ingresa el nombre del archivo donde guardas los datos de %s  || \n",valor);
	printf("||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
 	
 }
 
 
 void bienvenida () {
 	
 	printf("||||||||||||||||||||||||||||||\n");
	printf("|||| Bienvenido a Mapas  ||||| \n");
	printf("||||||||||||||||||||||||||||||\n");
	printf("\n");

	TeclaContinuar();
 	

 } 
 

 void menu () {
 	
 	Lugares *nombreLugares=NULL;

	char nombreArchivo[20];
	
	int opcion=0;
	
	
	ingresaTexto("los lugares y las rutas");
	
	scanf("%s",&nombreArchivo);
	
	archivoLugaresRutas = validaArchivo(nombreArchivo);
	
	nombreLugares=crearGrafo(nombreLugares,archivoLugaresRutas,1);

   
    
    
    
   	printf("|| Incluir archivo de las condiciones climaticas en las rutas? || \n");
	
	printf(" 1.Si  2.No \n");
	
	scanf("%i",&opcion);
	
	opcion=valorcorrecto(opcion, 2, 1);
	
	if (opcion==1) {
		
		
		ingresaTexto("el clima de las rutas  ");
		
	    scanf("%s",&nombreArchivo);
		
		archivoClima=validaArchivo(nombreArchivo);
		
		nombreLugares=crearGrafo(nombreLugares,archivoClima,2);
		
	}
	
	printf("A continuacion se mostraran todos los sitios\n");
	printf("con sus respectivas rutas y sus costes\n");
	TeclaContinuar();
	
	int i=1;
	
	Lugares *aux=nombreLugares;
	Conexion *aux2=NULL;
	
	
	while(aux!=NULL){
		
		
	printf("\n");	
	printf("%i)          Nombre del Lugar: %s          \n",i,aux->lugar);
	
	if(aux->conexiones!=NULL){
		
	aux2=aux->conexiones;
	while (aux2!=NULL) {
		
	printf("---------------------------------------------------------------\n");	
	printf("|  *Nombre de la ruta: %s\n",aux2->NombreRuta);
	printf("|  *Conexion con:%s\n",aux2->Conecta->lugar);
	printf("|  *Coste a PIE: %f seg(s)\n",aux2->P);	
	printf("|  *Coste EN BUS: %f seg(s)\n",aux2->B);	
	printf("|  *Coste en CARRO: %f seg(s)\n",aux2->C);
	printf("---------------------------------------------------------------\n");	
	aux2=aux2->siguiente;
	
	}
	
	
	}else{
		printf("---------------------------------------------------------------\n");
		printf("|  *No posee conexion con ningun sitio\n");
		printf("---------------------------------------------------------------\n");
	}
	
	aux=aux->siguiente;
	i++;
	
	printf("\n");
		
	}
	
    
    char sitios[30];
    Lugares *inicio=NULL;
    Lugares *llegada=NULL;
    
    
	i=0;
	
	printf("------------------------------\n");
	printf("| **POR FAVOR\n| INGRESE\n");
	
	
	printf("| *SU PUNTO DE PARTIDA:\n");
	printf("------------------------------\n");
	
    scanf("%s",&sitios);
    
    inicio=Busqueda(nombreLugares,sitios); 
	
    printf("------------------------------\n");
	printf("| *SU PUNTO DE LLEGADA:\n");
	printf("------------------------------\n");
	scanf("%s",&sitios);
	
	llegada=Busqueda(nombreLugares,sitios);
	
	
	mejorCamino (inicio,llegada,nombreLugares);
    
    
   

	
 }


 Lugares *crearGrafo (Lugares *grafo, FILE *archivo, int opcionClimaRuta ) {
 	
 	int n=0,i=0;
 	
 	char Paux[10],Baux[10],Caux[10];
 	
	float P=0,B=0,C=0;
 	
 	Lugares *nodo=NULL;
 	
 	char leer[70];
 	
 	int endOf;
	
	
	
	
	
	
	if(opcionClimaRuta==1){	
		
	char nombreRuta[30];
	char ruta1[30]="\0";	
	char ruta2[30]="\0";
	
	
	endOf=fscanf(archivo," %[^\n]",&leer); 
	endOf=fscanf(archivo," %[^\n]",&leer);	
	
	
	
	
	while (CompareStrings("Rutas",leer)!=0) {
		
		
		grafo=crearCasilla(grafo,leer);
		endOf=fscanf(archivo," %[^\n]",&leer);
		
		
	}
	
	
	endOf=fscanf(archivo," %[^\n]",&leer);
	
	
	
	while (endOf!=EOF) {
		
		
		
			while (leer[n]!='-' && leer[n+1]!='>'  ) {
			
			nombreRuta[n]=leer[n];
			n++;
		}
		
			
			
			
		
		n=n+2;
		
		
		while(leer[n]!=':'){
			ruta1[i]=leer[n];
			i++;
			n++;
			
		}
		
		
		
		i=0;
		n++;
		
		while ( leer[n]!='='  ) {
			
			ruta2[i]=leer[n];
			i++;
			n++;
			
		}
		
	
	
	
	i=0;
	n=n+3;
	
	
	
	
	while (leer[n]!=';') {
		
		Paux[i]=leer[n];
		n++;
		i++;
			
	}
	
	P=atof(Paux);
	
	
	
	i=0;
	n=n+3;
	
	while (leer[n]!=';') {
		
		Baux[i]=leer[n];
		n++;
		i++;
			
	}
	
	B=atof(Baux);
	

	
	i=0;
	n=n+3;
	

	while (leer[n]!='\0') {
		
		Caux[i]=leer[n];
		n++;
		i++;
			
	}
	
    C=atof(Caux);
	
	
	
	
	
 Lugares *partida=NULL,*llegada=NULL;
	
	partida=Busqueda (grafo,ruta1);
	
	llegada=Busqueda (grafo,ruta2);
	
	partida->conexiones=crearConexion(partida->conexiones,llegada,P,B,C,nombreRuta); 
	
		
	
	P=B=C=i=n=0;
	
	Caux[10]=limpiarCad(Caux);
	Baux[10]=limpiarCad(Baux);
	Paux[10]=limpiarCad(Paux);
	leer[70]=limpiarCad(leer);
	nombreRuta[30]=limpiarCad(nombreRuta);
	ruta1[30]=limpiarCad(ruta1);
	ruta2[30]=limpiarCad(ruta2);
    partida=NULL;
	llegada=NULL;	

	endOf=fscanf(archivo," %[^\n]",&leer);
		
	}

	close(archivo);
	
	
	
	}
	
	if (opcionClimaRuta==2) {
		
		char nombreAux[30];
		
		
		
		i=n=0;
		
		endOf=fscanf(archivo," %[^\n]",&leer);
		
		while (endOf!=EOF) {
		
		
		
		while(leer[n]!='=' ){
			
			nombreAux[i]=leer[n];
			n++;
			i++;
			
			
		}
		
	i=0;
	n=n+3;
	
	
	while (leer[n]!=';') {
		
		Paux[i]=leer[n];
		n++;
		i++;
			
	}
	
	P=atof(Paux);
	
	
	
	i=0;
	n=n+3;
	
	while (leer[n]!=';') {
		
		Baux[i]=leer[n];
		n++;
		i++;
			
	}
	
	B=atof(Baux);
	

	
	i=0;
	n=n+3;
	

	while (leer[n]!='\0') {
		
		Caux[i]=leer[n];
		n++;
		i++;
			
	}
	
	C=atof(Caux);
	
	
	Conexion *rutaClima=NULL;
	
	rutaClima=BusquedaRuta (grafo,nombreAux);
		
	rutaClima->P=rutaClima->P*P;
	rutaClima->B=rutaClima->B*B;
	rutaClima->C=rutaClima->C*C;
	
	
	P=B=C=i=n=0;
	
	Caux[10]=limpiarCad(Caux);
	Baux[10]=limpiarCad(Baux);
	Paux[10]=limpiarCad(Paux);
	leer[70]=limpiarCad(leer);
    nombreAux[30]=limpiarCad(nombreAux);	
	
	rutaClima=NULL;
		
		
	endOf=fscanf(archivo," %[^\n]",&leer);	
		
	
	}	
		
	close(archivo);	
		
	}


		
	
	return grafo;
 }
 
 Lugares *inicializa( Lugares *p ,int opcion) {
 	
 	Lugares *aux=p;
 	
 	while (aux!=NULL) {
 		if(opcion==1)
 		aux->distancia=Infinite;
 		if(opcion==2)
 		aux->visitado=0;
 		aux=aux->siguiente;
 		
 		
	 }
  	
  	return p;
 	
 }
 
 float suma(Camino *p, int opcion) {
 	
 	Camino *aux=p;
 	
 	float acumula=0;
 	
 	while(aux!=NULL) {
 		
 		if (opcion==1){
 			
 			acumula=acumula+(aux->conexion->P);
 			
 			aux=aux->anterior;
		 }
		else 
		if (opcion==2) {
			
			acumula=acumula+(aux->conexion->B);
			
			aux=aux->anterior;
		} 
		else 
		
		if (opcion==3) {
			
			acumula=acumula+(aux->conexion->C);
			
			aux=aux->anterior;
			
		}
		 
 		
 		
 		
 		
 		
 		
	 }
 	
 	return acumula;
 		
 	
 }
 
 
 void rutaView (Lugares *final,Camino *r, int op) {
 	if (op == 1){
		if(final->distancia==Infinite) 
			printf("| No se pudo Calcular ninguna ruta\n");
 		else {
	 
 			printf("| valor de la ruta mas optima %f \n",final->distancia);
 			printf("| \n");
			printf("| FIN >> ");
			printf("(%s)(%f)",r->conexion->NombreRuta,r->conexion->P);
	
			r=r->anterior; 
	 
	 		while (r!=NULL) {
	 	
				printf("<--(%s)(%f)",r->conexion->NombreRuta,r->conexion->P);
	 	
				r=r->anterior;
	 	
			}
	 
    		printf(" << INICIO\n"); 
    	 
			printf("| \n");
	
	 		}
	}
 	else
 		if (op == 2){
			if(final->distancia==Infinite) 
				printf("| No se pudo Calcular ninguna ruta\n");
 			else {
	 
 				printf("| valor de la ruta mas optima %f \n",final->distancia);
 				printf("| \n");
				printf("| FIN >> ");
				printf("(%s)(%f)",r->conexion->NombreRuta,r->conexion->B);
	
				r=r->anterior; 
	 
	 			while (r!=NULL) {
	 	
					printf("<--(%s)(%f)",r->conexion->NombreRuta,r->conexion->B);
	 	
					r=r->anterior;
	 	
				}
	 
    			printf(" << INICIO\n"); 
    	 
				printf("| \n");
	
	 		}
		}
 	
 	else {
 		
 		if (op == 3){
 			
			if(final->distancia==Infinite) 
				printf("| No se pudo Calcular ninguna ruta\n");
 			else {
	 
 				printf("| valor de la ruta mas optima %f \n",final->distancia);
 				printf("| \n");
				printf("| FIN >> ");
				printf("(%s)(%f)",r->conexion->NombreRuta,r->conexion->C);
	
				r=r->anterior; 
	 
	 			while (r!=NULL) {
	 	
					printf("<--(%s)(%f)",r->conexion->NombreRuta,r->conexion->C);
	 	
					r=r->anterior;
	 	
				}
	 
    			printf(" << INICIO\n"); 
    	 
				printf("| \n");
	
	 		}
		}
 	}
 	
 }
 


Camino *auxiliar=NULL;

 
 Camino *calculate (Lugares *sitio,Lugares *inicio,Lugares *final,Camino *mostrar,Camino *ant,int opcion,Lugares *grafo) {
 	
 	
 	Conexion *aux;
 	
 	
	
 
  if (strcmp(sitio->lugar,final->lugar)==0 ) {
  	
  	
 	auxiliar=ant;
	}
 
  else {
 	

 	
 	if(sitio->conexiones!=NULL) {
 	
	 aux=sitio->conexiones;	
 	 
 	 
 	 	
 	while(aux!=NULL) {
 		
 		
		if(opcion==1) {
			
			if (sitio->distancia+aux->P<aux->Conecta->distancia && aux->P!=0 && aux->Conecta->visitado!=1 ) {
 				
 			aux->Conecta->distancia=sitio->distancia+aux->P;
 			
 			mostrar=crearCamino(BusquedaRuta(grafo,aux->NombreRuta),mostrar,ant);
 			
 			calculate(aux->Conecta,inicio,final,mostrar->siguiente,mostrar,opcion,grafo);
			
		
			}	
		}
		
		
		if(opcion==2) {
			
			if (sitio->distancia+aux->B<aux->Conecta->distancia && aux->B!=0 && aux->Conecta->visitado!=1) {
 				
 			aux->Conecta->distancia=sitio->distancia+aux->B;
 			
 			mostrar=crearCamino(BusquedaRuta(grafo,aux->NombreRuta),mostrar,ant);
 			
 			calculate(aux->Conecta,inicio,final,mostrar->siguiente,mostrar,opcion,grafo);
			
			
			}
			
		}
		
		
		if(opcion==3) {          
			
			
			if (sitio->distancia+aux->C<aux->Conecta->distancia && aux->C!=0 && aux->Conecta->visitado!=1 ) {
					
 			aux->Conecta->distancia=sitio->distancia+aux->C;
 			
 			mostrar=crearCamino(BusquedaRuta(grafo,aux->NombreRuta),mostrar,ant);
 			
 			calculate(aux->Conecta,inicio,final,mostrar->siguiente,mostrar,opcion,grafo);
 			
 			
			 }
			
			
		}
 		aux=aux->siguiente;
 		
 		
		 }
 		
 		
	    }
 		
 	
	 }
	 
	
	  	
 }
 
  Camino *calculate2do (Camino *mejorRuta,Camino *recorrido,Camino *mejorRuta2,Lugares *inicio,Lugares *final,Lugares *grafo, int opcion ) {
  	
  	
  float distanciaNodo=0;
  
  
  Conexion *aux=inicio->conexiones;
  
  Camino *aux2=mejorRuta;
   
  
  if(mejorRuta!=NULL && inicio!=final ) {
  
  
  while(aux!=NULL )  {
  
	
  	
  	if (CompareStrings(aux->NombreRuta,aux2->conexion->NombreRuta)==0) {
  	
  	if(opcion==1) {
  		distanciaNodo=aux->P;
  		aux->P=0;
	  }
	if(opcion==2) {
  		distanciaNodo=aux->B;
  		aux->B=0;
	  }  
  	if(opcion==3) {
  		distanciaNodo=aux->C;
  		aux->C=0;
	  }
  	
  	
  	break;
  	
  	
  	
  }
  
 
  
  aux=aux->siguiente;
  	
  
 }	
 
 
  inicio->visitado=1; 
   
  inicio->distancia=0;	
 
 	
  if (opcion==1)
  calculate ( inicio,inicio,final,NULL,NULL,1,grafo);
  if (opcion==2)
  calculate ( inicio,inicio,final,NULL,NULL,2,grafo);
  if (opcion==3)
  calculate ( inicio,inicio,final,NULL,NULL,3,grafo);
  
  
  
  if(opcion==1) {
  	
  	if(distanciaNodo!=0)
  aux->P=distanciaNodo;	
  	
  }
  
  if(opcion==2) {
  	
  	if(distanciaNodo!=0)
  aux->B=distanciaNodo;	
  	
  }
  
  if(opcion==3) {
  	
  	if(distanciaNodo!=0)
  aux->C=distanciaNodo;	
  	
  }
  
  
  
  
  
 
 
 if(mejorRuta2==NULL && recorrido==NULL && final->distancia!=Infinite ) {
 	
 	mejorRuta2=auxiliar;
 	
 	
 	recorrido=crearCamino (aux2->conexion ,recorrido,NULL); 
 	
 	
 	
 } else {
 	
 	if (mejorRuta2==NULL && final->distancia!=Infinite) {
 		
 		Camino *aux3 = recorrido;
 		
 		Camino *aux4 = NULL;
 		
 		Camino *auxDe4 = NULL;
 		
 		
 		aux4 =crearCamino (aux3->conexion ,aux4,aux3->anterior);
 		
 		auxDe4=aux4;
 		
 		aux3=aux3->anterior;
 		
 		while(aux3!=NULL) {
 			
 			aux4 =crearCamino (aux3->conexion ,aux4,aux3->anterior);
 			
 			
 			aux3=aux3->anterior;
 			
		 }
		
	    
 		aux3=NULL;
 		aux3=auxiliar;
 		
 		while(aux3->anterior!=NULL) 
 		aux3=aux3->anterior;
		 
		 
 		
 		aux3->anterior=auxDe4;
 		
 		mejorRuta2=auxiliar;
 		
 		
 		
	 } 
	 
	 
	 if(( opcion==1 && mejorRuta2!=NULL && ( suma(recorrido,1)+suma(auxiliar,1))<( suma(mejorRuta2,1))) || (opcion==2 && mejorRuta2!=NULL && ( suma(recorrido,2)+suma(auxiliar,2))<( suma(mejorRuta2,2))) || (opcion==3 && mejorRuta2!=NULL && ( suma(recorrido,3)+suma(auxiliar,3))<( suma(mejorRuta2,3))) )  {
	 	
	 	
	 	Camino *aux3 = recorrido; 
 		
 		Camino *aux4 = NULL;
 		
 		Camino *auxDe4 = NULL;
 		
 		
 		aux4 =crearCamino (aux3->conexion ,aux4,aux3->anterior);
 		
 		auxDe4=aux4;
 		
 		aux3=aux3->anterior;
 		
 		
 		while(aux3!=NULL) {
 			
 			aux4 =crearCamino (aux3->conexion ,aux4,aux3->anterior);
 			
 			
 			aux3=aux3->anterior;
 			
		 }
 		
 		aux3=NULL;
 		aux3=auxiliar;
 		
 		while(aux3->anterior!=NULL) 
		 aux3=aux3->anterior;
 		
 		aux3->anterior=auxDe4;
 		
 		mejorRuta2=auxiliar;
 		
 			
	 	
	 }


 	recorrido=crearCamino (aux2->conexion ,recorrido,aux2->anterior);
 	
 	
 }
  
  grafo=inicializa( grafo, 1); 
  
      
       
  calculate2do (mejorRuta->siguiente,recorrido,mejorRuta2,mejorRuta->conexion->Conecta,final,grafo,opcion);
 
 
 
 
 } else {
 	
 	
 	
 	
 	
 	auxiliar=mejorRuta2;
 	
 	if(opcion==1)
 	final->distancia=suma(mejorRuta2,1);
 	if(opcion==2)
 	final->distancia=suma(mejorRuta2,2);
 	if(opcion==3)
 	final->distancia=suma(mejorRuta2,3);
 	
 	if(auxiliar!=NULL) {
 		
 	 printf("| \n");	
 	
 	 printf("|                                    << 2da Mejor Ruta >> : \n");
 	 
 	 printf("| \n");
 	 if(opcion==1)
	 rutaView(final,auxiliar,1);	
 	 if(opcion==2)
	 rutaView(final,auxiliar,2);	
 	 if(opcion==3)
	 rutaView(final,auxiliar,3);
	 	
	 } else 
	 printf("| No existe una 2da mejor Ruta \n");
 	
 	
 
 	
   }
 	
 	
 }


 
 
  void mejorCamino (Lugares *inicio,Lugares *final,Lugares *grafo) {
  	
  	
  	
  	if (inicio==final) 
  		
  		printf("|| No Puedes colocar el incio y el final igual !! ||\n");
  		
  	else {
	  	
	  	printf("---------------------------------------------Pie--------------------------------------------\n");
	  	
	  	inicio->distancia=0;
	  	
	  	calculate ( inicio,inicio,final,NULL,NULL,1,grafo);
	  	
	  	rutaView(final,auxiliar,1);
	  	
	    grafo=inicializa( grafo,1);
	    
	  	inicio->distancia=0;
	    
	    if( auxiliar!=NULL ) {
	    
	    auxiliar=anadirSig(auxiliar);
	    
		calculate2do(auxiliar,NULL,NULL,inicio,final,grafo,1);
	    
	    grafo=inicializa( grafo,2);
	    
	    grafo=inicializa( grafo,1);	
	    		
	    	
		}
	    
	    
	  	
	    inicio->distancia=0;
	    
	    auxiliar=NULL;
	    
	    
	    
	  	printf("--------------------------------------------------------------------------------------------\n");
	  	
	  	printf("---------------------------------------------Bus--------------------------------------------\n");
	  	
	  	inicio->distancia=0;
	  	
	  	calculate ( inicio,inicio,final,NULL,NULL,2,grafo);
	  	
	  	rutaView(final,auxiliar,2);
	  	
	    grafo=inicializa( grafo,1);
	    
	  	inicio->distancia=0;
	    
	    if( auxiliar!=NULL ) {
	    
	    auxiliar=anadirSig(auxiliar);
	    
		calculate2do(auxiliar,NULL,NULL,inicio,final,grafo,2);
	    
	    grafo=inicializa( grafo,2);
	    
	    grafo=inicializa( grafo,1);	
	    		
	    	
		}
		
		inicio->distancia=0;
	    
	    auxiliar=NULL;
	  	
	  	printf("--------------------------------------------------------------------------------------------\n");
	  	
	  	printf("---------------------------------------------Carro------------------------------------------\n");
	  	
	  	inicio->distancia=0;
	  	
	  	calculate ( inicio,inicio,final,NULL,NULL,3,grafo);
	  	
	  	rutaView(final,auxiliar,3);
	  	
	    grafo=inicializa( grafo,1);
	    
	  	inicio->distancia=0;
	    
	    if( auxiliar!=NULL ) {
	    
	    auxiliar=anadirSig(auxiliar);
	    
		calculate2do(auxiliar,NULL,NULL,inicio,final,grafo,3);
	    
	    grafo=inicializa( grafo,2);
	    
	    grafo=inicializa( grafo,1);	
	    		
	    	
		}
		
		inicio->distancia=0;
	    
	    auxiliar=NULL;
	  	
	  	
	  	printf("--------------------------------------------------------------------------------------------\n");
	  	
	  	
	  	
	  			  	
	  	
	  }
	   	
	  	
	  }

 
 

 



 /*Funciones Base del programa(fin)*/














