CREATE TABLE district
(
    d_id        INTEGER NOT NULL,
    d_w_id      INTEGER NOT NULL,
    d_name      VARCHAR(10),
    d_street_1  VARCHAR(20),
    d_street_2  VARCHAR(20),
    d_city      VARCHAR(20),
    d_state     CHAR(2),
    d_zip       CHAR(9),
    d_tax       DECIMAL(4, 4),
    d_ytd       DECIMAL(12, 2),
    d_next_o_id INTEGER,
    PRIMARY KEY (d_w_id, d_id)
) PARTITION BY HASH (d_w_id) WITH (STORAGE = paged);
