CREATE TABLE users (id uuid UNIQUE NOT NULL, username TEXT NOT NULL, password TEXT NOT NULL, create_at TIMESTAMP, last_login TIMESTAMP);

CREATE TABLE sessions(id UUID UNIQUE NOT NULL, user_id UUID, expire_at TIMESTAMP, created_at TIMESTAMP, updated_at TIMESTAMP);

CREATE TABLE clients (id UUID UNIQUE NOT NULL, user_id UUID NOT NULL, name TEXT NOT NULL, data_sheet TEXT NOT NULL, application_id TEXT, time_created TIMESTAMP, time_last_updated TIMESTAMP, UNIQUE (user_id, name, data_sheet));
