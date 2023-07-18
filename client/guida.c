#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"

static void displayItineraries (MYSQL *conn)
{
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[1];
    int status;

	//Input for the displaying
	char cf[46];

	//Get the required information
	printf("\nFiscal code of the guide is: ");
	getInput(46, cf, false);

	if(!setup_prepared_stmt(&prepared_stmt, "call stampa_itinerario_guida(?)", conn)){
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize itineraries list statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));
	
	param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[0].buffer = cf;
	param[0].buffer_length = strlen(cf);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for itineraries list\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not retrieve itineraries list\n", true);
        goto out;
	}

	// We have multiple result sets here!
	do {
		// Skip OUT variables (although they are not present in the procedure...)
		if(conn->server_status & SERVER_PS_OUT_PARAMS) {
			goto next;
		}
        
        dump_result_set(conn, prepared_stmt, "List of itineraries at you associated\n");

		// more results? -1 = no, >0 = error, 0 = yes (keep looking)
	    next:
		status = mysql_stmt_next_result(prepared_stmt);
		if (status > 0)
			finish_with_stmt_error(conn, prepared_stmt, "Unexpected condition", true);
		
	} while (status == 0);
    
    out:
	mysql_stmt_close(prepared_stmt);
}

void run_as_guida (MYSQL *conn)
{
	char options[2] = {'1','2'};
	char op;

	printf("Switching to guide role...\n");

	if(!parse_config("users/guida.json", &conf)) {
		fprintf(stderr, "Unable to load guide configuration\n");
		exit(EXIT_FAILURE);
	}

	if(mysql_change_user(conn, conf.db_username, conf.db_password, conf.database)) {
		fprintf(stderr, "mysql_change_user() failed\n");
		exit(EXIT_FAILURE);
	}

	while(true) {
		printf("\033[2J\033[H");
		printf("*** What should I do for you? ***\n\n");
		printf("1) Show itineraries at you associated\n");
		printf("2) Quit\n");

		op = multiChoice("Select an option", options, 2);

		switch(op) {
			case '1':
				displayItineraries(conn);
				break;
				
			case '2':
				return;

			default:
				fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
				abort();
		}

		getchar();
	}
}
