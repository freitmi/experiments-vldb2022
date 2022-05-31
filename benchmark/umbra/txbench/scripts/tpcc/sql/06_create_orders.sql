CREATE TABLE orders
(
    o_id         INTEGER NOT NULL,
    o_c_id       INTEGER NOT NULL,
    o_d_id       INTEGER NOT NULL,
    o_w_id       INTEGER NOT NULL,
    o_entry_d    DATE,
    o_carrier_id INTEGER,
    o_ol_cnt     INTEGER,
    o_all_local  INTEGER,
    PRIMARY KEY (o_w_id, o_d_id, o_id)
) PARTITION BY HASH (o_w_id) WITH (STORAGE = paged);
