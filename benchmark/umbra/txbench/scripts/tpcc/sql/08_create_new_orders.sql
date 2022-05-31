CREATE TABLE new_orders
(
    no_o_id INTEGER NOT NULL,
    no_d_id INTEGER NOT NULL,
    no_w_id INTEGER NOT NULL,
    PRIMARY KEY (no_w_id, no_d_id, no_o_id)
) PARTITION BY HASH (no_w_id) WITH (STORAGE = paged);
