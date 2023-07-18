#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"

struct report{
    int participants;
    int profit;
};

static size_t parse_report(MYSQL *conn, MYSQL_STMT *stmt, struct report **ret)
{
    int status;
	size_t row = 0;
	MYSQL_BIND param[2];

	int participants;
    int profit;

	if (mysql_stmt_store_result(stmt)) {
		fprintf(stderr, " mysql_stmt_execute(), 1 failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		exit(0);
	}
    
	*ret = malloc(mysql_stmt_num_rows(stmt) * sizeof(struct report));

	memset(param, 0, sizeof(param));
	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &participants;
	param[0].buffer_length = sizeof(participants);

	param[1].buffer_type = MYSQL_TYPE_LONG;
	param[1].buffer = &profit;
	param[1].buffer_length = sizeof(profit);

	
	if(mysql_stmt_bind_result(stmt, param)) {
		finish_with_stmt_error(conn, stmt, "Unable to bind column parameters\n", true);
	}

	while (true) {
		status = mysql_stmt_fetch(stmt);

		if (status == 1 || status == MYSQL_NO_DATA)
			break;

		(*ret)[row].participants = participants;
        (*ret)[row].profit = profit;

		row++;
	}			
	return row;
}

static void assignBusIti (MYSQL *conn)
{
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[2];

	//Input for the assignment
	int codBus;
	char codBus_to_convert[46];
	int codIti;
	char codIti_to_convert[46];

	//Get the required information
	printf("\nAutobus Code: ");
	getInput(46, codBus_to_convert, false);
	printf("\nItinerary Code: ");
	getInput(46, codIti_to_convert, false);

	//Apply proper type conversion
	codBus = atoi(codBus_to_convert);
	codIti = atoi(codIti_to_convert);

	//Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call assegna_autobus_itinerario(?,?)", conn)){
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize assignment of the Autobus on the Itinerary statement\n", false);
	}

	//Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &codBus;
	param[0].buffer_length = sizeof(codBus);

	param[1].buffer_type = MYSQL_TYPE_LONG;
	param[1].buffer = &codIti;
	param[1].buffer_length = sizeof(codIti);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for Autobus to Itinerary assignment\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "An error occurred while assign the Autobus to Itinerary.");
	} else {
		printf("Autobus correctly assigned...\n");
	}

	mysql_stmt_close(prepared_stmt);
}

static void assignTripHotel (MYSQL *conn)
{
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[5];

	//Input for the assignment
	int numero;
	char numero_to_convert[46];
	int codIti;
	char codIti_to_convert[46];
	char indirizzo[46];
	char name[46];
	char citta[46];

	//Get the required information
	printf("\nTrip number: ");
	getInput(46, numero_to_convert, false);
	printf("\nItinerary code: ");
	getInput(46, codIti_to_convert, false);
	printf("\nHotel address: ");
	getInput(46, indirizzo, false);
	printf("\nHotel name: ");
	getInput(46, name, false);
	printf("\nHotel city: ");
	getInput(46, citta, false);

	//Apply proper type conversion
	numero = atoi(numero_to_convert);
	codIti = atoi(codIti_to_convert);

	//Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call assegna_tappa_albergo(?,?,?,?,?)", conn)){
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize assignment of the Trip on the Hotel statement\n", false);
	}

	//Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &numero;
	param[0].buffer_length = sizeof(numero);

	param[1].buffer_type = MYSQL_TYPE_LONG;
	param[1].buffer = &codIti;
	param[1].buffer_length = sizeof(codIti);

	param[2].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[2].buffer = indirizzo;
	param[2].buffer_length = strlen(indirizzo);

	param[3].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[3].buffer = name;
	param[3].buffer_length = strlen(name);

	param[4].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[4].buffer = citta;
	param[4].buffer_length = strlen(citta);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for Trip to Hotel assignment\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "An error occurred while assign the Trip to Hotel.");
	} else {
		printf("Trip correctly assigned...\n");
	}

	mysql_stmt_close(prepared_stmt);
}

static void createUser (MYSQL *conn)
{
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[3];
	char options[5] = {'1','2', '3'};
	char r;

	// Input for the registration routine
	char username[46];
	char password[46];
	char ruolo[46];

	// Get the required information
	printf("\nUsername: ");
	getInput(46, username, false);
	printf("password: ");
	getInput(46, password, true);
	printf("Assign a possible role:\n");
	printf("\t1) Cliente\n");
	printf("\t2) Guida\n");
	printf("\t3) Agenzia\n");
	r = multiChoice("Select role", options, 3);

	// Convert role into enum value
	switch(r) {
		case '1':
			strcpy(ruolo, "cliente");
			break;
		case '2':
			strcpy(ruolo, "guida");
			break;
		case '3':
			strcpy(ruolo, "agenzia");
			break;
		default:
			fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
			abort();
	}

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call crea_utente(?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize user insertion statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[0].buffer = username;
	param[0].buffer_length = strlen(username);

	param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[1].buffer = password;
	param[1].buffer_length = strlen(password);

	param[2].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[2].buffer = ruolo;
	param[2].buffer_length = strlen(ruolo);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for user insertion\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "An error occurred while adding the user.");
	} else {
		printf("User correctly added...\n");
	}

	mysql_stmt_close(prepared_stmt);
}

static void generateReport (MYSQL *conn)
{
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[1];
	int status;
    size_t row = 0;
    int guadagno;
    int participants;
    bool first = true;
    struct report *rep;

	//Input for the genearation
	int codIti;
	char codIti_to_convert[46];

	//Get the required information
    printf("\nItinerary code: ");
	getInput(46, codIti_to_convert, false);

	//Apply proper conversion
	codIti = atoi(codIti_to_convert);

	//Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call genera_report(?)", conn)){
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize the generation of report statement\n", false);
	}

	//Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &codIti;
	param[0].buffer_length = sizeof(codIti);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for the generation of report assignment\n", true);
	}
	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "An error occurred while generation of report.");
		goto out;
	}
	
    // We have multiple result sets here!
	do {
		// Skip OUT variables (although they are not present in the procedure...)
		if(conn->server_status & SERVER_PS_OUT_PARAMS) {
			goto next;
		}
		
		if(first) {
            parse_report(conn, prepared_stmt, &rep);
            first = false;
        }
        else{
            printf("\nNumber of Participants: %d\nProfit of this Itinerary: %d\n", rep[row].participants, rep[row].profit);
            row++;
        }

		// more results? -1 = no, >0 = error, 0 = yes (keep looking)
	    next:
		status = mysql_stmt_next_result(prepared_stmt);
		if (status > 0)
			finish_with_stmt_error(conn, prepared_stmt, "Unexpected condition", true);
		
	} while (status == 0);

	out:
	mysql_stmt_close(prepared_stmt);
}

static void insertHotel (MYSQL *conn) 
{
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[9];

	//Input for the assignment
	char indirizzo[46];
	char nome[46];
	char costo_convert[46];
	char capienza_convert[46];
	int costo_persona;
	int capienza_max;
	char referente[46];
	char email[46];
	char tel[46];
	char fax[46];
	char citta[46];

	//Get the required information
	printf("\nHotel address: ");	
	getInput(46, indirizzo, false);
	printf("\nHotel Name: ");
	getInput(46, nome, false);
	printf("\nHotel cost: ");
	getInput(46, costo_convert, false);
	printf("\nHotel capacity: ");
	getInput(46, capienza_convert, false);
	printf("\nHotel referent: ");
	getInput(46, referente, false);
	printf("\nHotel e-mail: ");
	getInput(46, email, false);
	printf("\nHotel telephone: ");
	getInput(46, tel, false);
	printf("\nHotel fax: ");
	getInput(46, fax, false);
	printf("\nHotel City: ");
	getInput(46, citta, false);

	//Aplly proper type convertions
	costo_persona = atoi(costo_convert);
	capienza_max = atoi(capienza_convert);

	//Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call inserisci_albergo(?,?,?,?,?,?,?,?,?)", conn)){
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize insertion of the Hotel statement\n", false);
	}

	//Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[0].buffer = indirizzo;
	param[0].buffer_length = strlen(indirizzo);

	param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[1].buffer = nome;
	param[1].buffer_length = strlen(nome);

	param[2].buffer_type = MYSQL_TYPE_LONG;
	param[2].buffer = &costo_persona;
	param[2].buffer_length = sizeof(costo_persona);

	param[3].buffer_type = MYSQL_TYPE_LONG;
	param[3].buffer = &capienza_max;
	param[3].buffer_length = sizeof(capienza_max);

	param[4].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[4].buffer = referente;
	param[4].buffer_length = strlen(referente);

	param[5].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[5].buffer = email;
	param[5].buffer_length = strlen(email);

	param[6].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[6].buffer = tel;
	param[6].buffer_length = strlen(tel);

	param[7].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[7].buffer = fax;
	param[7].buffer_length = strlen(fax);

	param[8].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[8].buffer = citta;
	param[8].buffer_length = strlen(citta);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for Hotel insertion assignment\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "An error occurred while insert the Hotel.");
	} else {
		printf("Hotel correctly inserted...\n");
	}

	mysql_stmt_close(prepared_stmt);
}

static void insertAutobus (MYSQL *conn)
{
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[3];

	// Input for the registration routine
	int capienza;
	char capienza_convert[46];
	int costo_giorn;
	char costo_convert[46];
	int codice;

	// Get the required information
	printf("\nAutobus capacity: ");
	getInput(46, capienza_convert, false);
	printf("\nAutobus daily cost: ");
	getInput(46, costo_convert, false);
	
	//Apply proper type convertions
	capienza = atoi(capienza_convert);
	costo_giorn = atoi(costo_convert);

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call inserisci_autobus(?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize Autobus insertion statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &capienza;
	param[0].buffer_length = sizeof(capienza);

	param[1].buffer_type = MYSQL_TYPE_LONG;
	param[1].buffer = &costo_giorn;
	param[1].buffer_length = sizeof(costo_giorn);

	param[2].buffer_type = MYSQL_TYPE_LONG; // OUT
	param[2].buffer = &codice;
	param[2].buffer_length = sizeof(codice);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for Autobus insertion\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while adding the Autobus.");
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

	printf("Autobus correctly added with Code %d...\n", codice);

    out:
	mysql_stmt_close(prepared_stmt);
}

static void insertCity (MYSQL *conn)
{
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[2];

	//Input for the assignment
	char nome[46];
	char tipo[46];

	//Get the required information
	printf("\nCity Name: ");
	getInput(46, nome, false);
	printf("\nCity type, write Italian if it's Italian otherwise European: ");
	getInput(46, tipo, false);

	//Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call inserisci_citta(?,?)", conn)){
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize insertion of the City statement\n", false);
	}

	//Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[0].buffer = nome;
	param[0].buffer_length = strlen(nome);

	param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[1].buffer = tipo;
	param[1].buffer_length = strlen(tipo);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for City insertion assignment\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "An error occurred while insert the City.");
	} else {
		printf("City correctly inserted...\n");
	}

	mysql_stmt_close(prepared_stmt);
}

static void insertGuide (MYSQL *conn)
{
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[1];

	//Input for the assignment
	char cf[46];

	//Get the required information
	printf("\nGuide fiscal code: ");
	getInput(46, cf, false);

	//Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call inserisci_guida(?)", conn)){
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize insertion of the Guide statement\n", false);
	}

	//Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[0].buffer = cf;
	param[0].buffer_length = strlen(cf);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for Guide insertion assignment\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "An error occurred while insert the Guide.");
	} else {
		printf("Guide correctly inserted...\n");
	}

	mysql_stmt_close(prepared_stmt);
}

static void insertItinerary (MYSQL *conn)
{
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[6];
	printf("\nDate is in form YYYY-MM-DD");

	// Input for the registration routine
	int costo;
	char costo_convert[46];
	char dataPartenza[11];
	MYSQL_TIME dataP;
	MYSQL_TIME dataR;
	char dataRientro[11];
	char nome[101];
	char cfGuida[46];
	int codice;

	// Get the required information
	printf("\nItinerary cost: ");
	getInput(46, costo_convert, false);
	printf("\nItinerary departure date: ");
	getInput(11, dataPartenza, false);
	printf("\nItinerary return date: ");
	getInput(11, dataRientro, false);
	printf("\nItinerary name: ");
	getInput(101, nome, false);
	printf("\nItinerary Guide fiscal code: ");
	getInput(46, cfGuida, false);

	//Apply proper type convertions
	costo = atoi(costo_convert);
	dataP = convertTime (dataPartenza);
	dataR = convertTime (dataRientro);

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call inserisci_itinerario(?, ?, ?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize Itinerary insertion statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &costo;
	param[0].buffer_length = sizeof(costo);

	param[1].buffer_type = MYSQL_TYPE_DATE;
	param[1].buffer = &dataP;
	param[1].buffer_length = sizeof(dataP);

	param[2].buffer_type = MYSQL_TYPE_DATE;
	param[2].buffer = &dataR;
	param[2].buffer_length = sizeof(dataR);

	param[3].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[3].buffer = nome;
	param[3].buffer_length = strlen(nome);

	param[4].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[4].buffer = cfGuida;
	param[4].buffer_length = strlen(cfGuida);

	param[5].buffer_type = MYSQL_TYPE_LONG; // OUT
	param[5].buffer = &codice;
	param[5].buffer_length = sizeof(codice);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for Itinerary insertion\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while adding the Itinerary.");
		goto out;
	}

	// Get back the ID of the newly-added itinerary
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

	printf("Itinerary correctly added with Code %d...\n", codice);
    
    out:
	mysql_stmt_close(prepared_stmt);
}

static void insertTrip (MYSQL *conn)
{
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[4];

	// Input for the registration routine
	int codIti;
	char codIti_to_convert[46];
	char nomeCitta[46];
	char tipo[46];
	int codice;

	// Get the required information
	printf("\nItinerary code: ");
	getInput(100, codIti_to_convert, false);
	printf("\nTrip name of City: ");
	getInput(46, nomeCitta, false);
	printf("\nTrip type, write Italian if it's Italian otherwise European: ");
	getInput(46, tipo, false);

	//Apply proper convertion
	codIti = atoi(codIti_to_convert);

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call inserisci_tappa(?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize Trip insertion statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &codIti;
	param[0].buffer_length = sizeof(codIti);

	param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[1].buffer = nomeCitta;
	param[1].buffer_length = strlen(nomeCitta);

	param[2].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[2].buffer = tipo;
	param[2].buffer_length = strlen(tipo);

	param[3].buffer_type = MYSQL_TYPE_LONG; // OUT
	param[3].buffer = &codice;
	param[3].buffer_length = sizeof(codice);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for Trip insertion\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while adding the Trip.");
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

	printf("Trip correctly added with Number %d...\n", codice);

    out:
	mysql_stmt_close(prepared_stmt);
}

void run_as_agenzia (MYSQL *conn) 
{
    char options[11] = {'1','2','3','4','5','6','7','8','9','a','q'};
	int op;

	printf("\nSwitching to agency role...");

	if(!parse_config("users/agenzia.json", &conf)) {
		fprintf(stderr, "Unable to load agency configuration\n");
		exit(EXIT_FAILURE);
	}

	if(mysql_change_user(conn, conf.db_username, conf.db_password, conf.database)) {
		fprintf(stderr, "mysql_change_user() failed\n");
		exit(EXIT_FAILURE);
	}

	while(true){
		printf("\033[2J\033[H");
		printf("*** What should I do for you? ***\n\n");
		printf("1) Assign an Autobus to an Itinerary\n");
		printf("2) Assign a Trip to an Itinerary and to an Hotel\n");
		printf("3) Create a new user\n");
		printf("4) Generate a report of a Itinerary and its profit\n");
		printf("5) Insert a new Hotel\n");
		printf("6) Insert a new Autobus\n");
		printf("7) Insert a new City\n");
		printf("8) Insert a new Guide\n");
		printf("9) Insert a new Itinerary\n");
		printf("a) Insert a new Trip\n");
		printf("q) Quit\n");

		op = multiChoice("Select an option", options, 12);

		switch(op) {
			case '1':
				assignBusIti(conn);
				break;

			case '2':
				assignTripHotel(conn);
				break;	

			case '3':
				createUser(conn);
				break;

			case '4':
				generateReport(conn);
				break;

			case '5':
				insertHotel(conn);
				break;

			case '6':
				insertAutobus(conn);
				break;

            case '7':
				insertCity(conn);
				break;

			case '8':
				insertGuide(conn);
				break;

			case '9':
				insertItinerary(conn);
				break;

			case 'a':
				insertTrip(conn);
				break;

			case 'q':
				return;
				
			default:
				fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
				abort();
		}

		getchar();
	}
}
