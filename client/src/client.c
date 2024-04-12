#include "client.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <readline/history.h>
#include <readline/readline.h>
int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */
	
	logger = iniciar_logger();
	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	log_info(logger,"Hola! Soy un log");

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();
	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	valor = config_get_string_value(config,"CLAVE");	//LA funcion devuelve un string
	ip="192.168.0.18"//config_get_string_value(config,"IP");
	puerto=config_get_string_value(config,"PUERTO");

	// Loggeamos el valor de config
	//log_info(logger,ip); // MUESTRO EL IP 
	//log_info(logger,puerto); // MUESTRO EL PUERTO
	//log_info(logger,valor); // MUESTRO LA CLAVE


	/* ---------------- LEER DE CONSOLA ---------------- */

	//leer_consola(logger);
	

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);
	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor,conexion);
	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
    t_log* nuevo_logger = log_create("tp0.log","LOG",true,LOG_LEVEL_INFO);

    if(nuevo_logger == NULL){
        printf("No se puede crear el logger\n");
        exit(EXIT_FAILURE);
    }

    return nuevo_logger;
} 

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("cliente.config");
	if(nuevo_config == NULL){
        printf("No se pudo encontrar la config\n");
        exit(EXIT_FAILURE);
    }
	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;
	leido = readline(">");
	log_info(logger,">> %s",leido);
    while (strcmp(leido,"")!=0) {
		free(leido);
		leido = readline(">");
		log_info(logger,">> %s",leido);
    }
	// ¡No te olvides de liberar las lineas antes de regresar!
	free(leido);
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete = crear_paquete();
	// Leemos y esta vez agregamos las lineas al paquete
	leido = readline(">");
	//log_info(logger,">> %s",leido);
    while (strcmp(leido,"")!=0) {
		agregar_a_paquete(paquete,leido,strlen(leido)+1);
		free(leido);
		leido = readline(">");
    }
	free(leido);
	// ENVIAR PAQUETE
	enviar_paquete(paquete,conexion);
	// ELIMINAR PAQUETE
	eliminar_paquete(paquete);
	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!

}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
    /* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
      con las funciones de las commons y del TP mencionadas en el enunciado */
    liberar_conexion(conexion);
	log_destroy(logger);
	config_destroy(config);
} 