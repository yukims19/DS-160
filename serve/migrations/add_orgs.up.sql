CREATE TABLE orgs (id uuid UNIQUE NOT NULL, organization TEXT NOT NULL);
ALTER TABLE users ADD COLUMN org_id uuid;
