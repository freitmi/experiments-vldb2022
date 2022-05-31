CREATE TABLE order_line
(
    ol_o_id        INTEGER NOT NULL,
    ol_d_id        INTEGER NOT NULL,
    ol_w_id        INTEGER NOT NULL,
    ol_number      INTEGER NOT NULL,
    ol_i_id        INTEGER,
    ol_supply_w_id INTEGER,
    ol_delivery_d  DATE,
    ol_quantity    INTEGER,
    ol_amount      DECIMAL(6, 2),
    ol_dist_info   CHAR(24),
    PRIMARY KEY (ol_w_id, ol_d_id, ol_o_id, ol_number, ol_i_id)
) PARTITION BY HASH (ol_w_id) WITH (STORAGE = paged);
