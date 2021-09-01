/*
 ============================================================================
 Name        : tp0.c
 Author      : Axel
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "tp0.h"
#include <commons/log.h>
#include <commons/config.h>
#include <readline/readline.h>

t_log* logger;
t_config* configuration;
int configOk=1;
char* clave;

int main(void) {
	logger = log_create("logTP0", "TP0 LOG", true, LOG_LEVEL_DEBUG);
	configuration = config_create("tp0.config");
	if(configuration==NULL){
		log_error(logger, "Verifique el archivo de configuracion de entrada. (¿Este existe?)");
		exit(-1);
	}

	if (config_has_property(configuration, "CLAVE")) {

		clave = config_get_string_value(configuration, "CLAVE");

	} else {

		log_error(logger, "Error al obtener la CLAVE desde el tp0.config");

		configOk = 0;
	}
	if (!configOk) {
		log_error(logger, "Debido a errores en las configuraciones, se aborta la ejecución... (REVISE ARCH. CONFIGURACIONES)");
		exit(-1);

	}

	log_info(logger, clave);


	leer_consola();


	log_destroy(logger);
	config_destroy(configuration);
	return EXIT_SUCCESS;
}


void leer_consola(){
	char * linea;

	while(1) {
		    linea = readline(">");


		    if (strcmp(linea, "\0")==0){
		    	log_info(logger, "Finalizado ingreso de usuario por consola.");
		    	break;
		    }else{
		    	log_info(logger, linea);
		    }

		  }
	}



