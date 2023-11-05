CREATE TABLE CatalogoSensor (
    idTipoSensor INT PRIMARY KEY AUTO_INCREMENT,
    nombre VARCHAR(255)
);

CREATE TABLE CatalogoActuador (
    idTipoActuador INT PRIMARY KEY AUTO_INCREMENT,
    nombre VARCHAR(255)
);

CREATE TABLE Sensor (
    idSensor INT PRIMARY KEY AUTO_INCREMENT,
    idTipoSensor INT,
    FOREIGN KEY (idTipoSensor) REFERENCES CatalogoSensor(idTipoSensor)
);


CREATE TABLE Actuador (
    idActuador INT PRIMARY KEY AUTO_INCREMENT,
    idTipoActuador INT,
    idSensor INT,
    FOREIGN KEY (idTipoActuador) REFERENCES CatalogoActuador(idTipoActuador),
    FOREIGN KEY (idSensor) REFERENCES Sensor(idSensor)
);

CREATE TABLE DatosSensor (
    idDatoSensor INT PRIMARY KEY AUTO_INCREMENT,
    idSensor INT,
    valor FLOAT,
    fecha TIMESTAMP,
    FOREIGN KEY (idSensor) REFERENCES Sensor(idSensor)
);

-- Insertar tipos de sensores en el catálogo
INSERT INTO CatalogoSensor (nombre) VALUES ('Humedad');
INSERT INTO CatalogoSensor (nombre) VALUES ('Temperatura');

-- Insertar tipos de actuadores en el catálogo
INSERT INTO CatalogoActuador (nombre) VALUES ('Pantalla LCD');
INSERT INTO CatalogoActuador (nombre) VALUES ('Extractor de aire');

-- Insertar sensores en el invernadero
INSERT INTO Sensor (idTipoSensor) VALUES (1); -- Asociado al tipo de sensor de humedad
INSERT INTO Sensor (idTipoSensor) VALUES (2); -- Asociado al tipo de sensor de temperatura

-- Insertar actuadores en el invernadero
INSERT INTO Actuador (idTipoActuador, idSensor) VALUES (1, 1); -- Asociado al tipo de actuador de pantalla LCD y al sensor de humedad
INSERT INTO Actuador (idTipoActuador, idSensor) VALUES (2, 2); -- Asociado al tipo de actuador de extractor de aire y al sensor de temperatura

select * from catalogosensor;