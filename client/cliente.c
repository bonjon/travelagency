#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"

static void deletePrenotation (MYSQL *conn)
{
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[1];

	//Input for the cancelation
	int codice;
	char cod[46];

	//Get the required information 
	printf("\nPrenotation code: ");
	getInput(46, cod, false);
    
	//Apply proper type conversion
	codice = atoi(cod);
    
	//Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call cancella_prenotazione(?)", conn)){
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize cancelation of the Prenotation statement\n", false);
	}

	//Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &codice;
	param[0].buffer_length = sizeof(codice);


	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for Prenotation deletion\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "An error occurred while delete the Prenotation.");
	} 
	
	// Control if delete changed rows or not
	if (mysql_stmt_affected_rows(prepared_stmt) != 0){
		printf("Prenotation correctly deleted...\n");
	} else {
        printf("You insert wrong codes\n");
    }

	mysql_stmt_close(prepared_stmt);

}

static void insertPrenotation (MYSQL *conn) 
{
    MYSQL_STMT *prepared_stmt;
    MYSQL_BIND param[3];
    
	// Input for the registration routine
	int partecipanti;
	char part[46];
	int codIti;
	char nome[46];
	int codice;

	// Get the required information
	printf("\nPrenotation number of participants : ");
	getInput(46, part, false);
	printf("\nitinerary code: ");
	getInput(46, nome, false);
	
	//Apply proper type convertions
	partecipanti = atoi(part);
	codIti = atoi(nome);

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call inserisci_prenotazione(?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize Prenotation insertion statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &partecipanti;
	param[0].buffer_length = sizeof(&partecipanti);

	param[1].buffer_type = MYSQL_TYPE_LONG;
	param[1].buffer = &codIti;
	param[1].buffer_length = sizeof(codIti);

	param[2].buffer_type = MYSQL_TYPE_LONG; // OUT
	param[2].buffer = &codice;
	param[2].buffer_length = sizeof(codice);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for Prenotation insertion\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while adding the Prenotaion.");
		goto out;
	}

	// Get back the ID of the newly-added bus
	memset(param, 0, sizeof(param));
	param[0].buffer_type = MYSQL_TYPE_LONG; // OUT
	param[0].buffer = &codice;
	param[0].buffer_length = sizeof(codice);
	
	if(mysql_stmt_bind_result(prepared_stmt, param)) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not retrieve output parameter", true);
	}
	
	// Retrieve output parameter
	if(mysql_stmt_fetch(prepared_stmt)) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not buffer results", true);
	}

	printf("Prenotation correctly added with Code %d, please save this code if you want to dismiss the prenotation...\n", codice);

    out:
	mysql_stmt_close(prepared_stmt);
} 

static void displayItinerary (MYSQL *conn) 
{
	MYSQL_STMT *prepared_stmt;
    int status;

	if(!setup_prepared_stmt(&prepared_stmt, "call stampa_itinerari()", conn)){
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize itinerary list statement\n", false);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not retrieve itinerary list\n", true);
        goto out;        
    }

	// We have multiple result sets here!
	do {
		// Skip OUT variables (although they are not present in the procedure...)
		if(conn->server_status & SERVER_PS_OUT_PARAMS) {
			goto next;
		}
        
        dump_result_set(conn, prepared_stmt, "List of Itineraries\n");

		// more results? -1 = no, >0 = error, 0 = yes (keep looking)
	    next:
		status = mysql_stmt_next_result(prepared_stmt);
		if (status > 0)
			finish_with_stmt_error(conn, prepared_stmt, "Unexpected condition", true);
		
	} while (status == 0);
    
    out:
	mysql_stmt_close(prepared_stmt);
}

static void displayTrips (MYSQL *conn) 
{
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[1];
    int status;

	//Input for the displaying
	char nome[46];
	int codIti;

	//Get the required information
	printf("\nitinerary code: ");
	getInput(46, nome, false);

	codIti = atoi(nome);

	if(!setup_prepared_stmt(&prepared_stmt, "call stampa_tappe(?)", conn)){
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize trip list statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));
	
	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &codIti;
	param[0].buffer_length = sizeof(codIti);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for trip list\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not retrieve trip list\n", true);
        goto out;
    }

    // We have multiple result sets here!
	do {
		// Skip OUT variables (although they are not present in the procedure...)
		if(conn->server_status & SERVER_PS_OUT_PARAMS) {
			goto next;
		}
        
        dump_result_set(conn, prepared_stmt, "List of city touched by this itinerary\n");

		// more results? -1 = no, >0 = error, 0 = yes (keep looking)
	    next:
		status = mysql_stmt_next_result(prepared_stmt);
		if (status > 0)
			finish_with_stmt_error(conn, prepared_stmt, "Unexpected condition", true);
		
	} while (status == 0);
    
    out:
	mysql_stmt_close(prepared_stmt);	
}

void run_as_cliente (MYSQL *conn)
{
	char options[5] = {'1','2','3','4','5'};
	char op;

	printf("Switching to client role...\n");

	if(!parse_config("users/cliente.json", &conf)) {
		fprintf(stderr, "Unable to load client configuration\n");
		exit(EXIT_FAILURE);
	}

	if(mysql_change_user(conn, conf.db_username, conf.db_password, conf.database)) {
		fprintf(stderr, "mysql_change_user() failed\n");
		exit(EXIT_FAILURE);
	}

	while(true) {
		printf("\033[2J\033[H");
		printf("*** What should I do for you? ***\n\n");
		printf("1) Delete Prenotation\n");
		printf("2) Insert Prenotation to an itinerary\n");
		printf("3) Show list of itinerary available\n");
		printf("4) Show list of trips of an itinerary\n");
		printf("5) Quit\n");

		op = multiChoice("Select an option", options, 5);

		switch(op) {
			case '1':
				deletePrenotation(conn);
				break;
				
			case '2':
				insertPrenotation(conn);
				break;
				
			case '3':
				displayItinerary(conn);
				break;

			case '4':
				displayTrips(conn);
				break;

			case '5':
				return;

			default:
				fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
				abort();
		}

		getchar();
	}
}
