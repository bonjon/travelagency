-- MySQL Workbench Forward Engineering

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';

-- -----------------------------------------------------
-- Schema mydb
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `mydb` ;
USE `mydb` ;

-- -----------------------------------------------------
-- Table `mydb`.`Città`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`Città` (
  `Nome` VARCHAR(45) NOT NULL,
  `Tipo` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`Nome`))
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8mb4
COLLATE = utf8mb4_0900_ai_ci;


-- -----------------------------------------------------
-- Table `mydb`.`Albergo`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`Albergo` (
  `Indirizzo` VARCHAR(45) NOT NULL,
  `Nome` VARCHAR(45) NOT NULL,
  `CostoPersona` INT NOT NULL,
  `CapienzaMax` INT NOT NULL,
  `Referente` VARCHAR(45) NOT NULL,
  `E-Mail` VARCHAR(45) NOT NULL,
  `Telefono` VARCHAR(45) NOT NULL,
  `Fax` VARCHAR(45) NOT NULL,
  `NomeCittà` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`Indirizzo`, `Nome`, `NomeCittà`),
  INDEX `fk_Albergo_Città_idx` (`NomeCittà` ASC) VISIBLE,
  CONSTRAINT `fk_Albergo_Città_idx`
    FOREIGN KEY (`NomeCittà`)
    REFERENCES `mydb`.`Città` (`Nome`)
    ON DELETE RESTRICT
    ON UPDATE RESTRICT)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8mb4
COLLATE = utf8mb4_0900_ai_ci;


-- -----------------------------------------------------
-- Table `mydb`.`Guida`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`Guida` (
  `CF` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`CF`))
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8mb4
COLLATE = utf8mb4_0900_ai_ci;


-- -----------------------------------------------------
-- Table `mydb`.`Itinerario`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`Itinerario` (
  `Codice` INT NOT NULL AUTO_INCREMENT,
  `Costo` INT NOT NULL,
  `DataPartenza` DATE NOT NULL,
  `DataRientro` DATE NOT NULL,
  `CFGuida` VARCHAR(45) NOT NULL,
  `Nome` VARCHAR(100) NOT NULL,
  PRIMARY KEY (`Codice`),
  INDEX `fk_Itinerario_Guida_idx` (`CFGuida` ASC) VISIBLE,
  CONSTRAINT `fk_Itinerario_Guida_idx`
    FOREIGN KEY (`CFGuida`)
    REFERENCES `mydb`.`Guida` (`CF`)
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
AUTO_INCREMENT = 20
DEFAULT CHARACTER SET = utf8mb4
COLLATE = utf8mb4_0900_ai_ci;


-- -----------------------------------------------------
-- Table `mydb`.`Tappa`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`Tappa` (
  `Numero` INT NOT NULL AUTO_INCREMENT,
  `CodItinerario` INT NOT NULL,
  `NomeCittà` VARCHAR(45) NOT NULL,
  `Tipo` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`Numero`, `CodItinerario`),
  INDEX `fk_Tappa_Itinerario_idx` (`CodItinerario` ASC) VISIBLE,
  INDEX `fk_Tappa_Città_idx` (`NomeCittà` ASC) VISIBLE,
  CONSTRAINT `fk_Tappa_Città_idx`
    FOREIGN KEY (`NomeCittà`)
    REFERENCES `mydb`.`Città` (`Nome`)
    ON DELETE RESTRICT
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_Tappa_Itinerario_idx`
    FOREIGN KEY (`CodItinerario`)
    REFERENCES `mydb`.`Itinerario` (`Codice`)
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
AUTO_INCREMENT = 6
DEFAULT CHARACTER SET = utf8mb4
COLLATE = utf8mb4_0900_ai_ci;


-- -----------------------------------------------------
-- Table `mydb`.`Assegnazione`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`Assegnazione` (
  `NumeroTappa` INT NOT NULL,
  `CodItinerario` INT NOT NULL,
  `IndirizzoAlbergo` VARCHAR(45) NOT NULL,
  `NomeAlbergo` VARCHAR(45) NOT NULL,
  `CittàAlbergo` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`NumeroTappa`, `CodItinerario`),
  INDEX `fk_Assegnazione_Albergo1_idx` (`IndirizzoAlbergo` ASC, `NomeAlbergo` ASC, `CittàAlbergo` ASC) VISIBLE,
  CONSTRAINT `fk_Assegnazione_Albergo1`
    FOREIGN KEY (`IndirizzoAlbergo` , `NomeAlbergo` , `CittàAlbergo`)
    REFERENCES `mydb`.`Albergo` (`Indirizzo` , `Nome` , `NomeCittà`)
    ON DELETE RESTRICT
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_Assegnazione_Tappa1`
    FOREIGN KEY (`NumeroTappa` , `CodItinerario`)
    REFERENCES `mydb`.`Tappa` (`Numero` , `CodItinerario`)
    ON DELETE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8mb4
COLLATE = utf8mb4_0900_ai_ci;


-- -----------------------------------------------------
-- Table `mydb`.`Autobus`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`Autobus` (
  `Codice` INT NOT NULL AUTO_INCREMENT,
  `Capienza` INT NOT NULL,
  `CostoGiornaliero` INT NOT NULL,
  PRIMARY KEY (`Codice`))
ENGINE = InnoDB
AUTO_INCREMENT = 5
DEFAULT CHARACTER SET = utf8mb4
COLLATE = utf8mb4_0900_ai_ci;


-- -----------------------------------------------------
-- Table `mydb`.`Associazione`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`Associazione` (
  `CodItinerario` INT NOT NULL,
  `CodAutobus` INT NOT NULL,
  PRIMARY KEY (`CodItinerario`, `CodAutobus`),
  INDEX `fk_Associazione_Autobus_idx` (`CodAutobus` ASC) VISIBLE,
  INDEX `fk_Associazione_Itinerario_idx` (`CodItinerario` ASC) VISIBLE,
  CONSTRAINT `fk_Associazione_Autobus_idx`
    FOREIGN KEY (`CodAutobus`)
    REFERENCES `mydb`.`Autobus` (`Codice`)
    ON DELETE RESTRICT
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_Associazione_Itinerario_idx`
    FOREIGN KEY (`CodItinerario`)
    REFERENCES `mydb`.`Itinerario` (`Codice`)
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8mb4
COLLATE = utf8mb4_0900_ai_ci;


-- -----------------------------------------------------
-- Table `mydb`.`Pernottamento`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`Pernottamento` (
  `CodItinerario` INT NOT NULL,
  `IndirizzoAlbergo` VARCHAR(45) NOT NULL,
  `NomeAlbergo` VARCHAR(45) NOT NULL,
  `CittàAlbergo` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`CodItinerario`, `IndirizzoAlbergo`, `NomeAlbergo`, `CittàAlbergo`),
  INDEX `fk_Itinerario_has_Albergo_Albergo1_idx` (`IndirizzoAlbergo` ASC, `NomeAlbergo` ASC, `CittàAlbergo` ASC) VISIBLE,
  INDEX `fk_Itinerario_has_Albergo_Itinerario1_idx` (`CodItinerario` ASC) VISIBLE,
  CONSTRAINT `fk_Itinerario_has_Albergo_Albergo1`
    FOREIGN KEY (`IndirizzoAlbergo` , `NomeAlbergo` , `CittàAlbergo`)
    REFERENCES `mydb`.`Albergo` (`Indirizzo` , `Nome` , `NomeCittà`)
    ON DELETE RESTRICT
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_Itinerario_has_Albergo_Itinerario1`
    FOREIGN KEY (`CodItinerario`)
    REFERENCES `mydb`.`Itinerario` (`Codice`)
    ON DELETE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8mb4
COLLATE = utf8mb4_0900_ai_ci;


-- -----------------------------------------------------
-- Table `mydb`.`Prenotazione`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`Prenotazione` (
  `Codice` INT NOT NULL AUTO_INCREMENT,
  `NumPartecipanti` INT NOT NULL,
  `CodItinerario` INT NOT NULL,
  PRIMARY KEY (`Codice`),
  INDEX `fk_Prenotazione_Itinerario_idx` (`CodItinerario` ASC) VISIBLE,
  CONSTRAINT `fk_Prenotazione_Itinerario_idx`
    FOREIGN KEY (`CodItinerario`)
    REFERENCES `mydb`.`Itinerario` (`Codice`)
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
AUTO_INCREMENT = 13
DEFAULT CHARACTER SET = utf8mb4
COLLATE = utf8mb4_0900_ai_ci;


-- -----------------------------------------------------
-- Table `mydb`.`utenti`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`utenti` (
  `username` VARCHAR(45) NOT NULL,
  `password` CHAR(32) NOT NULL,
  `ruolo` ENUM('agenzia', 'guida', 'cliente') NOT NULL,
  PRIMARY KEY (`username`))
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8mb4
COLLATE = utf8mb4_0900_ai_ci;

USE `mydb` ;

-- -----------------------------------------------------
-- Placeholder table for view `mydb`.`Partecipanti`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`Partecipanti` (`Codice` INT, `NumPartecipanti` INT);

-- -----------------------------------------------------
-- procedure assegna_albergo_itinerario
-- -----------------------------------------------------

DELIMITER $$
USE `mydb`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `assegna_albergo_itinerario`(in var_indirizzo varchar(45), in var_citta varchar(45), in var_nome varchar(45), in var_itinerario int)
BEGIN
    insert into `Pernottamento` values (var_itinerario, var_indirizzo, var_nome, var_citta);
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure assegna_autobus_itinerario
-- -----------------------------------------------------

DELIMITER $$
USE `mydb`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `assegna_autobus_itinerario`(IN var_codice_autobus int, IN var_codice int)
BEGIN
	insert into `Associazione` (`CodItinerario`, `CodAutobus`) values (var_codice, var_codice_autobus);
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure assegna_tappa_albergo
-- -----------------------------------------------------

DELIMITER $$
USE `mydb`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `assegna_tappa_albergo`(IN var_numero int, IN var_codice int, IN var_indirizzo varchar(45), IN var_nome varchar(45), IN var_citta varchar(45))
BEGIN
	declare exit handler for sqlexception
    begin
        rollback;  -- rollback any changes made in the transaction
        resignal;  -- raise again the sql exception to the caller
    end;
    set transaction isolation level read committed;
    start transaction;
	insert into `Assegnazione` values (var_numero, var_codice, var_indirizzo, var_nome, var_citta);
	commit;
    call assegna_albergo_itinerario(var_indirizzo, var_citta, var_nome, var_codice);
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure cancella_prenotazione
-- -----------------------------------------------------

DELIMITER $$
USE `mydb`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `cancella_prenotazione`(IN var_codice int)
BEGIN
	delete from `Prenotazione` where `Prenotazione`.`Codice` = var_codice;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure crea_utente
-- -----------------------------------------------------

DELIMITER $$
USE `mydb`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `crea_utente`(IN username varchar(45), IN pass varchar(45), IN ruolo varchar(45))
BEGIN
	insert into utenti values (username, MD5(pass), ruolo);
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure genera_report
-- -----------------------------------------------------

DELIMITER $$
USE `mydb`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `genera_report`(IN var_id int)
BEGIN
    declare var_giorni int;
    declare var_notti int;
    declare exit handler for sqlexception
    begin
        rollback;  -- rollback any changes made in the transaction
        resignal;  -- raise again the sql exception to the caller
    end;
    set transaction isolation level serializable;
    start transaction;
    select datediff(`DataRientro`, `DataPartenza`) + 1
		from `Itinerario` 
        where `Codice` = var_id
        into var_giorni;
    set var_notti = var_giorni -1;
    select `PartecipantiComplessivi` as 'Numero Partecipanti', (`Itinerario`.`Costo`*`PartecipantiComplessivi` - `CostoGiornaliero`* var_giorni - `CostoPersona`*var_notti*`PartecipantiComplessivi`) as 'Guadagno'
		from `Itinerario` join `Partecipanti` join `Autobus` join `Albergo`
        where `Itinerario`.`Codice` = var_id and `Partecipanti`.`IdItinerario` = var_id;
    commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure inserisci_albergo
-- -----------------------------------------------------

DELIMITER $$
USE `mydb`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `inserisci_albergo`(IN var_indirizzo varchar(45), IN var_nome varchar(45), IN var_costo_persona int, IN var_capienza_max int, IN var_referente varchar(45), IN var_email varchar(45), IN var_tel varchar(45), IN var_fax varchar(45), IN var_citta varchar(45))
BEGIN
	insert into `Albergo` (`Indirizzo`, `Nome`, `CostoPersona`, `CapienzaMax`, `Referente`, `E-Mail`, `Telefono`, `Fax`,`NomeCittà`) values (var_indirizzo, var_nome, var_costo_persona, var_capienza_max, var_referente, var_email, var_tel, var_fax,var_citta);
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure inserisci_autobus
-- -----------------------------------------------------

DELIMITER $$
USE `mydb`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `inserisci_autobus`(IN var_capienza int, IN var_costo_giorn int, OUT var_codice int)
BEGIN
	insert into `Autobus` (`Capienza`, `CostoGiornaliero`) values (var_capienza, var_costo_giorn);
	set var_codice = last_insert_id();
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure inserisci_citta
-- -----------------------------------------------------

DELIMITER $$
USE `mydb`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `inserisci_citta`(IN var_nome varchar(45), IN var_tipo varchar(45))
BEGIN
	insert into `Città` values (var_nome, var_tipo);
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure inserisci_guida
-- -----------------------------------------------------

DELIMITER $$
USE `mydb`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `inserisci_guida`(IN var_CF varchar(45))
BEGIN
	insert into `Guida` (`CF`) values (var_CF);
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure inserisci_itinerario
-- -----------------------------------------------------

DELIMITER $$
USE `mydb`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `inserisci_itinerario`(IN var_costo int, IN var_dataP DATE, in var_dataR DATE, in var_nome varchar(100), IN var_guida varchar(45),out var_codice int)
BEGIN
	insert into `Itinerario` (`Costo`, `DataPartenza`, `DataRientro`,`CFGuida`,`Nome`) values (var_costo, var_dataP, var_dataR, var_guida,var_nome);
	set var_codice = last_insert_id();
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure inserisci_prenotazione
-- -----------------------------------------------------

DELIMITER $$
USE `mydb`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `inserisci_prenotazione`(IN var_num_partecipanti int, IN var_itinerario int, out var_codice_prenotazione int)
BEGIN
	insert into `Prenotazione` (`NumPartecipanti`,`CodItinerario`) values (var_num_partecipanti, var_itinerario);
	set var_codice_prenotazione = last_insert_id();
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure inserisci_tappa
-- -----------------------------------------------------

DELIMITER $$
USE `mydb`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `inserisci_tappa`(IN var_codice int, IN var_nome_citta varchar(45), IN var_tipo varchar(45), OUT var_numero int)
BEGIN
    insert into `Tappa` (`CodItinerario`, `NomeCittà`, `Tipo`) values (var_codice, var_nome_citta, var_tipo);
    set var_numero = last_insert_id();
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure login
-- -----------------------------------------------------

DELIMITER $$
USE `mydb`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `login`(IN var_username varchar(45), IN var_pass varchar(45), OUT var_role int)
BEGIN
	declare var_user_role ENUM('agenzia', 'guida', 'cliente');
    
	select `ruolo` from `utenti`
		where `username` = var_username
        and `password` = md5(var_pass)
        into var_user_role;
        
        -- See the corresponding enum in the client
		if var_user_role = 'agenzia' then
			set var_role = 1;
		elseif var_user_role = 'guida' then
			set var_role = 2;
		elseif var_user_role = 'cliente' then
			set var_role = 3;
		else
			set var_role = 4;
		end if;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure stampa_itinerari
-- -----------------------------------------------------

DELIMITER $$
USE `mydb`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `stampa_itinerari`()
BEGIN
	set transaction read only;
    set transaction isolation level read committed;
	select `Nome`, `Codice` from `Itinerario`;
    commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure stampa_itinerario_guida
-- -----------------------------------------------------

DELIMITER $$
USE `mydb`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `stampa_itinerario_guida`(IN var_CF varchar(45))
BEGIN
	set transaction read only;
    set transaction isolation level read committed;
	select `Nome` 
		from `Itinerario`
        where `CFGuida` = var_CF;
    commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure stampa_tappe
-- -----------------------------------------------------

DELIMITER $$
USE `mydb`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `stampa_tappe`(IN var_itinerario int)
BEGIN
    set transaction read only;
    set transaction isolation level read committed;
	select `NomeCittà` as 'Città Toccate dal Viaggio' from `Tappa` where `CodItinerario` = var_itinerario;
    commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- View `mydb`.`Partecipanti`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `mydb`.`Partecipanti`;
USE `mydb`;
CREATE  OR REPLACE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `mydb`.`Partecipanti` (`IdItinerario`,`PartecipantiComplessivi`) AS select `mydb`.`Itinerario`.`Codice` AS `Codice`,sum(`mydb`.`Prenotazione`.`NumPartecipanti`) AS `NumPartecipanti` from (`mydb`.`Itinerario` join `mydb`.`Prenotazione` on((`mydb`.`Itinerario`.`Codice` = `mydb`.`Prenotazione`.`CodItinerario`))) group by `mydb`.`Itinerario`.`Codice`;
CREATE USER 'agenzia' IDENTIFIED BY 'password';

GRANT EXECUTE ON procedure `mydb`.`assegna_albergo_itinerario` TO 'agenzia';
GRANT EXECUTE ON procedure `mydb`.`assegna_autobus_itinerario` TO 'agenzia';
GRANT EXECUTE ON procedure `mydb`.`assegna_tappa_albergo` TO 'agenzia';
GRANT EXECUTE ON procedure `mydb`.`crea_utente` TO 'agenzia';
GRANT EXECUTE ON procedure `mydb`.`inserisci_itinerario` TO 'agenzia';
GRANT EXECUTE ON procedure `mydb`.`genera_report` TO 'agenzia';
GRANT EXECUTE ON procedure `mydb`.`inserisci_albergo` TO 'agenzia';
GRANT EXECUTE ON procedure `mydb`.`inserisci_autobus` TO 'agenzia';
GRANT EXECUTE ON procedure `mydb`.`inserisci_citta` TO 'agenzia';
GRANT EXECUTE ON procedure `mydb`.`inserisci_guida` TO 'agenzia';
GRANT EXECUTE ON procedure `mydb`.`inserisci_tappa` TO 'agenzia';
CREATE USER 'cliente' IDENTIFIED BY 'password';

GRANT EXECUTE ON procedure `mydb`.`cancella_prenotazione` TO 'cliente';
GRANT EXECUTE ON procedure `mydb`.`inserisci_prenotazione` TO 'cliente';
GRANT EXECUTE ON procedure `mydb`.`stampa_itinerari` TO 'cliente';
GRANT EXECUTE ON procedure `mydb`.`stampa_tappe` TO 'cliente';
CREATE USER 'login' IDENTIFIED BY 'password';

GRANT EXECUTE ON procedure `mydb`.`login` TO 'login';
CREATE USER 'guida' IDENTIFIED BY 'password';

GRANT EXECUTE ON procedure `mydb`.`stampa_itinerario_guida` TO 'guida';

SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
USE `mydb`;

DELIMITER $$
USE `mydb`$$
CREATE DEFINER = CURRENT_USER TRIGGER `mydb`.`Città_BEFORE_INSERT` BEFORE INSERT ON `Città` FOR EACH ROW
BEGIN
	if (NEW.`Tipo` <> 'Italian' and NEW.`Tipo` <> 'European') then
		signal sqlstate "45001" set message_text = "You must insert Italian or European";
	end if;
END$$

USE `mydb`$$
CREATE
DEFINER=`root`@`localhost`
TRIGGER `mydb`.`Albergo_BEFORE_INSERT`
BEFORE INSERT ON `mydb`.`Albergo`
FOR EACH ROW
BEGIN
	if NEW.CostoPersona <= 0 then
		signal sqlstate "45000" set message_text = "The cost of the Hotel must be greater than 0";
    end if;
    if NEW.CapienzaMax <= 0 then
		signal sqlstate "45001" set message_text = "The capacity of the hotel must be greater than 0";
	end if;
END$$

USE `mydb`$$
CREATE
DEFINER=`root`@`localhost`
TRIGGER `mydb`.`Itinerario_BEFORE_INSERT`
BEFORE INSERT ON `mydb`.`Itinerario`
FOR EACH ROW
BEGIN
	if NEW.Costo <= 0 then
		signal sqlstate "45000" set message_text = "The cost of the Itinerary must be greater than 0";
	end if;
    
    if datediff(NEW.DataRientro, NEW.DataPartenza) < 0 then
		signal sqlstate "45001" set message_text = "The return date may not be earlier than the departure date!";
	end if;
    
    if datediff(NEW.DataPartenza, now()) < 0 then
		signal sqlstate "45002" set message_text = "It's too late to programming a trip!!";
	end if;
    
    if datediff(NEW.DataRientro, NEW.DataPartenza) > 7 then
		signal sqlstate "45003" set message_text = "The maximum number of days is 7";
	end if;
END$$

USE `mydb`$$
CREATE
DEFINER=`root`@`localhost`
TRIGGER `mydb`.`Tappa_BEFORE_INSERT`
BEFORE INSERT ON `mydb`.`Tappa`
FOR EACH ROW
BEGIN
	declare var_tipo varchar(45);
    if (NEW.`Tipo` <> 'Italian' and NEW.`Tipo` <> 'European') then
		signal sqlstate "45001" set message_text = "You must insert Italian or European";
	end if;
	select `Tipo` from `Città` where `Nome` = NEW.`NomeCittà` into var_tipo;
    if (NEW.`Tipo` <> var_tipo) then
		signal sqlstate "45000" set message_text = "The trip and the city must be of the same type (italian or european)";
    end if;
END$$

USE `mydb`$$
CREATE
DEFINER=`root`@`localhost`
TRIGGER `mydb`.`Autobus_BEFORE_INSERT`
BEFORE INSERT ON `mydb`.`Autobus`
FOR EACH ROW
BEGIN
	if NEW.Capienza < 40 then
		signal sqlstate "45000" set message_text = "At least 40 seats";
    end if;
    if NEW.Capienza > 60 then
		signal sqlstate "45001" set message_text = "At most 60 seats";
	end if;
    if NEW.CostoGiornaliero <= 0 then
		signal sqlstate "45002" set message_text = "The cost of the autobus must be greater than 0";
    end if;
END$$

USE `mydb`$$
CREATE
DEFINER=`root`@`localhost`
TRIGGER `mydb`.`Prenotazione_BEFORE_INSERT`
BEFORE INSERT ON `mydb`.`Prenotazione`
FOR EACH ROW
BEGIN
	declare var_data date;
	select `DataPartenza` from `Itinerario` where `Codice` = NEW.`CodItinerario` into var_data;
	if var_data <= now() then
		signal sqlstate "45002" set message_text = "It's too late to register at this itinerary!";
	end if;
    if NEW.NumPartecipanti <= 0 then
		signal sqlstate "45000" set message_text = "The number of participants must be greater than 0";
	end if;
END$$

USE `mydb`$$
CREATE DEFINER = CURRENT_USER TRIGGER `mydb`.`Prenotazione_BEFORE_DELETE` BEFORE DELETE ON `Prenotazione` FOR EACH ROW
BEGIN
	declare var_data date;
	select `DataPartenza` from `Itinerario` where `Codice` = OLD.`CodItinerario` into var_data;
    if (datediff(var_data, now())) < 20 then
		signal sqlstate "45000" set message_text = "Cancelation has expired!";
	end if;
END$$


DELIMITER ;
