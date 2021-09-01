#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/
	int conexion;
	char* ip;
	char* puerto;
	char* valor;
	int configOk=1;

	t_log* logger;
	t_config* config;

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	log_info(logger, "<Cliente> Iniciado.");
	log_info(logger, "Hola! Soy un log.");

	config = iniciar_config();


	// Usando el config creado previamente
	// Lee las variables de IP, Puerto y Valor

	if(config==NULL){
			log_error(logger, "Verifique el archivo de configuracion de entrada. (¿Este existe?)");
			exit(-1);
		}

		if (config_has_property(config, "IP")) {

			ip = config_get_string_value(config, "IP");


		} else {

			log_error(logger, "Error al obtener la IP desde el client.config");

			configOk = 0;
		}
		if (config_has_property(config, "PUERTO")) {

			puerto = config_get_string_value(config, "PUERTO");

		} else {

			log_error(logger, "Error al obtener el PUERTO desde el client.config");

			configOk = 0;
		}
		if (config_has_property(config, "VALOR")) {

			valor = config_get_string_value(config, "VALOR");

		} else {

			log_error(logger, "Error al obtener el VALOR desde el client.config");

			configOk = 0;
		}

		if (!configOk) {
			log_error(logger, "Debido a errores en las configuraciones, se aborta la ejecución... (REVISE ARCH. CONFIGURACIONES)");
			exit(-1);

		}



	//Loggear valor de config
	log_debug(logger, "La IP de SERVIDOR es: %s, su puerto: %s, el valor: %s", ip, puerto,valor);


	leer_consola(logger);


	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo porque lo necesitaremos para lo que sigue.
	log_debug(logger, "Intentando conectar con SERVIDOR.");
	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	if(conexion==-1){
		log_error(logger, "Problema al levantar conexión con SERVIDOR en IP:%s PUERTO:%s (¿SERVIDOR se encuentra levantado?)",ip,puerto);
		exit(-1);
	}
	log_info(logger,"Conexión establecida con SERVIDOR.");
	//enviar CLAVE al servirdor
	enviar_mensaje(valor,conexion);
	paquete(conexion);

	free(ip);
	free(valor);
	free(puerto);
	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("logClient", "CLIENTE LOG", true, LOG_LEVEL_DEBUG);

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("client.config");;

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;
	while(1){
	//El primero te lo dejo de yapa
	leido = readline(">");

	// Acá la idea es que imprimas por el log lo que recibis de la consola.

	   if (strcmp(leido, "\0")==0){
			log_info(logger, "Finalizado ingreso de usuario por consola.");
			break;
		}else{
			log_info(logger, leido);
		}
	}

}

void paquete(int conexion)
{
	//Ahora toca lo divertido!

	char* leido;
	t_paquete* paquete;
	paquete = crear_paquete();

	leido = readline("Ingrese (Vacio para salir)>");

	while(strcmp(leido, "\0")!=0){
		/*printf("Leido: %s",leido);
		printf("Length of string %zu \n",strlen(leido)+1);*/

		agregar_a_paquete(paquete,leido,strlen(leido)+1);
		free(leido);
		leido = readline("Ingrese (Vacio para salir)>");

	}
	enviar_paquete(paquete,conexion);
	eliminar_paquete(paquete);
	free(leido);



}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	//Y por ultimo, para cerrar, hay que liberar lo que utilizamos (conexion, log y config) con las funciones de las commons y del TP mencionadas en el enunciado
	log_info(logger, "Finalizado el envio de datos. Cliente Cerrado.");
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);

}
