CREATE TABLE users (id SERIAL, username TEXT, password TEXT);

CREATE TABLE clients (id SERIAL, user_id INT NOT NULL, name TEXT NOT NULL, data_sheet TEXT NOT NULL, application_id TEXT, time_created TIMESTAMP, time_last_updated TIMESTAMP);
