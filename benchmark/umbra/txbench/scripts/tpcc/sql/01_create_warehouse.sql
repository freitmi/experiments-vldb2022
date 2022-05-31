CREATE TABLE warehouse
(
    w_id       INTEGER NOT NULL,
    w_name     VARCHAR(10),
    w_street_1 VARCHAR(20),
    w_street_2 VARCHAR(20),
    w_city     VARCHAR(20),
    w_state    CHAR(2),
    w_zip      CHAR(9),
    w_tax      DECIMAL(4, 4),
    w_ytd      DECIMAL(12, 2),
    PRIMARY KEY (w_id)
) PARTITION BY HASH (w_id) WITH (STORAGE = paged);
